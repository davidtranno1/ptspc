#include <iostream>
#include <sstream>
#include <cstring>
#include <boost/filesystem.hpp>

#include <ProgramOptions.hxx>
#include "instance.h"
#include "params.h"
#include "solver.h"
#include "tsp_solver.h"

int main(int argc, char** argv) {
    using namespace stok;
    using namespace boost::filesystem;

    po::parser p;

    p["instance"]
        .abbreviation('i')
        .description("PTSPC instance file")
        .type(po::string);

    p["solver"]
        .abbreviation('s')
        .description(
            "One of: enum, fwd, bck, fwdbck, bckfwd (default is enum)\n"
            "* enum: enumerates all possible offered sets\n"
            "* fwd: forward stepwise heuristic\n"
            "* bck: backward stepwise heuristic\n"
            "* fwdbck: alternates one forward and one backward step\n"
            "* bckfwd: alternates one backward and one forward step\n"
            "* tsp: just solves the tsp over all delivery points")
        .type(po::string)
        .fallback("enum");

    p["exp-cost-solver"]
        .abbreviation('e')
        .description(
            "One of: exact, mc, rtlr, rt (default is exact)\n"
            "* exact: computes the exact expected cost\n"
            "* mc: approximates the expected cost via monte-carlo simulation\n"
            "* rtlr: approximates the expected cost via a regression tree with linear regression in its leaves\n"
            "* rt: approximates the expected cost via a regression tree")
        .type(po::string)
        .fallback("exact");

    p["tsp-solver"]
        .abbreviation('t')
        .description(
            "One of: discorde, cplexbc (deafult is discorde)\n"
            "* discorde: uses the concorde TSP solver via the discode api\n"
            "* cplexbc: solves the TSP using a simple branch-and-cut algorithm"
            " and the solver CPLEX")
        .type(po::string)
        .fallback("discorde");

    p["monte-carlo-n"]
        .abbreviation('m')
        .description("Number of Monte Carlo simulations (default is 20)")
        .type(po::u32)
        .fallback(20u);

    p["ml-training-time"]
        .abbreviation('T')
        .description(
            "Number of seconds to spend to gather traning data"
            " (default is 60")
        .type(po::f32)
        .fallback(60.0f);

    p["recompute-exact"]
        .abbreviation('r')
        .description(
            "Whether to compute the exact expected cost of the best set found"
            " via heuristic expected cost solvers");

    p["draw-solution"]
        .abbreviation('d')
        .description("Whether to draw a picture of the solution");

    p["create-dataset"]
        .abbreviation('c')
        .description("Whether to create the dataset while solving the problem");

    p["randomise-sets"]
        .abbreviation('R')
        .description("Whether to randomise the starting sets for fwd, bck, "
                     "fwdbck and bckfwd heuristics.");

    p["help"]
        .abbreviation('h')
        .description("Displays this help screen")
        .callback([&] { std::cout << p << '\n'; });

    if(!p(argc, argv)) {
        std::cerr << "Error parsing programme options!\n";
        return 1;
    }

    if(p["help"].available()) {
        return 0;
    }

    if(!p["instance"].was_set()) {
        std::cerr << "You need to pass an instance path!\n";
        std::_Exit(1);
    }

    const auto instance = Instance{p["instance"].get().string};
    const auto params = Params{instance, p};

    // Shortcut if the user only wants the TSP solution.
    if(p["solver"].get().string == "tsp") {
        const auto solution = TspSolver{params}(
            CustomerSet(instance.number_of_deliveries(), false));
        
        std::cout << solution << "\n";
    } else {
        const auto solver = get_solver(params);
        const auto solution = (*solver)();

        std::cout << solution << "\n";
    }

    // Remove files created by concorde.
    for(const auto& file : directory_iterator(".")) {
        if(is_regular_file(file) &&
           (file.path().stem().string().find("concorde") != std::string::npos))
        {
            remove(file);
        }
    }

    return 0;
}