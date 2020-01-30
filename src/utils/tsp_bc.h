#ifndef TSP_BC_H
#define TSP_BC_H

#ifndef IL_STD
#define IL_STD
#endif
#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>

#include <algorithm>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "../instance.h"
#include "cache.h"

namespace stok {
    /**
     * Cplex callback called on new integer solutions, and used to separate
     * violated subtour-elimination cuts. Because the solution is integer, we
     * don't need to solve max-flow problems. We just start from the depot and
     * follow the (sub)tour. If it doesn't visit all customers, we add the
     * corresponding cut forcing at most |S|-1 edges to be selected.
     */
    class SESeparator : public IloCplex::LazyConstraintCallbackI {
        const IloEnv& env;
        const IloArray<IloNumVarArray>& x;
        const long n;

    public:

        SESeparator(const IloEnv& env, IloArray<IloNumVarArray>& x, long n) :
            IloCplex::LazyConstraintCallbackI{env}, env{env}, x{x}, n{n} {}

        IloCplex::CallbackI* duplicateCallback() const override {
            return new(getEnv()) SESeparator{*this};
        }

        void main() override;
    };

    /**
     * Solves the TSP using a branch-and-cut algorithm. Subtour elimination
     * constraints are not initially added, and are separated for each new
     * integer solution found during the exploration of the branch-and-bound
     * tree. Returns the cost of the tour.
     */
    float bc_tsp_solve(const Instance& instance, const CustomerSet& custs);
}

#endif