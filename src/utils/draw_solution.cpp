#include "draw_solution.h"
#include <boost/graph/adjacency_list.hpp>
#include <as/graph_plot.h>
#include <as/geometry.h>
#include <cassert>

namespace stok {
    void draw_solution(const Instance& instance, const std::string& instance_name, const Solution& solution) {
        using namespace boost;
        using namespace as;
        using namespace as::graph::plot;
        using Graph = adjacency_list<vecS, vecS, undirectedS, geo::TwoDimPoint>;
        using Vertex = graph_traits<Graph>::vertex_descriptor;

        Graph g;
        std::vector<Vertex> offered;

        assert(solution.offered.size() == instance.number_of_deliveries());

        for(auto i = 0u; i < instance.number_of_vertices(); ++i) {
            add_vertex(instance.get_coordinates(i), g);
            if(i > 0u && solution.offered[i - 1u]) {
                offered.push_back(i);
            }
        }

        PlottedGraph{g}
            .add_highlighted_vertices(offered)
            .add_highlighted_vertex(0u)
            .plot_png(instance_name + "-" + std::to_string(instance.number_of_vertices()) + ".png");
    }
}