#include "tsp_bc.h"

namespace stok {
    void SESeparator::main() {
        std::vector<unsigned int> depot_subtour = { 0u };
        unsigned int prev_v = 0u;
        unsigned int current_v = 0u;

        do {
            const auto current_v_cpy = current_v;

            for(auto i = 0u; i < n; ++i) {
                if(i == current_v) { continue; }
                if(i == prev_v) { continue; }

                const auto s = std::min(current_v, i);
                const auto t = std::max(current_v, i);

                if(getValue(x[s][t]) > 0.5) {
                    depot_subtour.push_back(i);
                    prev_v = current_v;
                    current_v = i;
                    break;
                }
            }

            if(current_v == current_v_cpy) {
                // This can happen in case of a 2-tour, because of line:
                // if(i == prev_v) { continue; } above.
                assert(prev_v == 0u);
                assert(getValue(x[0u][current_v] > 0.5));
                depot_subtour.push_back(0u);
                current_v = 0u;
            }
        } while(current_v != 0u);

        assert(depot_subtour.back() == 0u);

        // * A complete tour has size |V|.
        // * A complete depot_subtour has size |V| + 1 because of the
        //   repeated depot (0) at the beginning and at the end.
        // * Therefore, a hamiltonian depot_subtour has size n+1 and, in
        //   this case, it's not a *sub*tour, it's just a tour.
        if(static_cast<long>(depot_subtour.size()) != n + 1u) {
            IloExpr expr{env};

            for(auto i = 0u; i < depot_subtour.size() - 1u; ++i) {
                const auto s = std::min(
                    depot_subtour[i], depot_subtour[i+1]);
                const auto t = std::max(
                    depot_subtour[i], depot_subtour[i+1]);

                expr += x[s][t];
            }

            try {
                add(expr <= static_cast<IloNum>(depot_subtour.size() - 2u));
            } catch(IloException& e) {
                std::cerr << "Callback exception: " << e.getMessage() << "\n";
                throw;
            }
        }
    }

    float bc_tsp_solve(const Instance& instance, const CustomerSet& custs) {
        // Number of vertices in the TSP.
        // The depot (= 1), plus the customers (= # true in custs).
        const auto n = std::count(custs.begin(), custs.end(), true) + 1;
        std::vector<unsigned int> c = { 0u };

        c.reserve(n);
        for(auto i = 0u; i < custs.size(); ++i) {
            if(custs[i]) {
                // custs[i] == true means vertex i+1 is a selected customer. 
                c.push_back(i + 1u);
            }
        }

        if(n == 1) {
            return 0.0f;
        }

        if(n == 2) {
            return 2 * instance.get_distance(0u, c[1u]);
        }

        if(n == 3) {
            return  instance.get_distance(0u, c[1u]) +
                    instance.get_distance(c[1u], c[2u]) +
                    instance.get_distance(c[2u], 0u);
        }

        IloEnv env;
        IloModel model{env};

        // Variables x are x[smaller index][larger index].
        // Therefore, for a TSP with n vertices, they are:
        // x[0][1] x[0][2] ...   x[0][n-1]      n-1 columns
        //         x[1][2] ...   x[1][n-1]      n-2 columns
        // ...
        //                     x[n-2][n-1]      1 column
        //
        // n-1 rows (from index 0 to n-2)
        IloArray<IloNumVarArray> x{env, n - 1u};

        for(auto i = 0u; i < n - 1u; ++i) {// Max row index is n-2.
            x[i] = IloNumVarArray{env, n}; // Max column index is n-1, so I
                                           // need n slots, from 0 to n-1.
            
            for(auto j = i + 1u; j < n; ++j) {
                // We only create variables for j > i
                // because of symmetric edges.
                x[i][j] = IloNumVar{env, 0, 1, IloNumVar::Bool};
            }
        }

        IloExpr expr{env};

        // Incidence constraints.
        for(auto i = 0u; i < n; ++i) {
            for(auto j = 0u; j < n; ++j) {
                if(i < j) {
                    expr += x[i][j];
                } else if(j < i) {
                    expr += x[j][i];
                }
            }

            model.add(expr == 2);
            expr.clear();
        }

        // Objective function.
        for(auto i = 0u; i < n - 1u; ++i) {
            for(auto j = i + 1; j < n; ++j) {
                expr += x[i][j] * instance.get_distance(c[i], c[j]);
            }
        }

        model.add(IloObjective{env, expr, IloObjective::Minimize});
        expr.end();

        IloCplex cplex{model};
        cplex.use(IloCplex::Callback(new(env) SESeparator{env, x, n}));

        try {
            if(!cplex.solve()) {
                cplex.exportModel("cannot_solve.lp");
                std::cerr << "IloCplex::solve() returned false!\n";
            }
        } catch(const IloException& e) {
            cplex.exportModel("crash.lp");
            std::cerr << "Cplex exception: " << e.getMessage() << "\n";
            throw;
        }

        if(cplex.getMIPRelativeGap() > 0.001) {
            std::cerr << "Not solved to optimality!\n";
            throw std::runtime_error("Cplex could not solve the TSP");
        }

        return cplex.getObjValue();
    }
}