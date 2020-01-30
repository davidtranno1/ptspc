#ifndef STOK_DATASET
#define STOK_DATASET

#include "params.h"
#include "customer_set.h"

#include <vector>
#include <limits>
#include <numeric>
#include <utility>

#include <Python.h>
#include <boost/python.hpp>

namespace stok {
    template<typename Row, typename T>
    auto append_to(Row& row, const T& datum) -> decltype(row.push_back(datum)) {
        return row.push_back(datum);
    }

    template<typename Row, typename T>
    auto append_to(Row& row, const T& datum) -> decltype(row.append(datum)) {
        return row.append(datum);
    }

    template<typename Row, typename Container = std::vector<Row>>
    struct Dataset {
        const Params& params;
        Container c;

        float max_depot_dist;
        float min_depot_dist;
        float avg_depot_dist;
        float tot_outsourcing_cost;
        float longest_arc;
        float instance_sz;

        explicit Dataset(const Params& p) : params{p} {
            const auto& instance = params.instance;

            instance_sz = static_cast<float>(instance.number_of_vertices());

            float tot_depot_dist = 0.0f;
            max_depot_dist = 0.0f;
            min_depot_dist = std::numeric_limits<float>::max();
            avg_depot_dist = 0.0f;

            tot_outsourcing_cost = 0.0f;
            longest_arc = 0.0f;

            for(auto i = 0ul; i < instance.number_of_vertices(); ++i) {
                for(auto j = i + 1ul; j < instance.number_of_vertices(); ++j) {
                    const auto d = instance.get_distance(i, j);
                    if(d > longest_arc) {
                        longest_arc = d;
                    }
                }

                if(i > 0u) {
                    const auto d = instance.get_distance(0u, i);

                    tot_outsourcing_cost += instance.outsource_cost[i];
                    tot_depot_dist += d;

                    if(d > max_depot_dist) {
                        max_depot_dist = d;
                    }

                    if(d < min_depot_dist) {
                        min_depot_dist = d;
                    }
                }
            }

            avg_depot_dist = tot_depot_dist / (instance_sz - 1u);
        }

        void add_row_for(const CustomerSet& offered, float cost) {
            auto [success, row] = row_for(offered);

            if(success) {
                append_to(row, cost);
                append_to(c, row);
            }
        }

        std::pair<bool, Row> row_for(const CustomerSet& offered) const {
            Row row;

            const auto& instance = params.instance;

            const auto n_offered = std::count(
                offered.begin(), offered.end(), true);
            const auto n_nonoffered = instance.number_of_vertices() -
                n_offered;

            if(n_offered == 0) { return std::make_pair(false, row); }

            for(const auto& cust_offered : offered) {
                append_to(row, cust_offered);
            }

            append_to(row, static_cast<float>(n_offered) / instance_sz);

            float offered_os_cost = 0.0f;
            std::vector<float> nonoffered_ddist, offered_ddist;

            nonoffered_ddist.reserve(n_nonoffered);
            offered_ddist.reserve(n_offered);

            for(auto i = 0ul; i < offered.size(); ++i) {
                const auto customer = i + 1u;

                if(offered[i]) {
                    offered_os_cost += instance.outsource_cost[customer];
                    offered_ddist.push_back(
                        instance.get_distance(0u, customer));
                } else {
                    nonoffered_ddist.push_back(
                        instance.get_distance(0u, customer));
                }
            }

            append_to(row, offered_os_cost / tot_outsourcing_cost);

            // V\O empty, use zero.
            if(nonoffered_ddist.empty()) {
                nonoffered_ddist.push_back(0.0f);
            }

            const auto [min_no_ddist, max_no_ddist] =
                std::minmax_element(
                    nonoffered_ddist.begin(), nonoffered_ddist.end());
            const auto [min_o_ddist, max_o_ddist] =
                std::minmax_element(
                    offered_ddist.begin(), offered_ddist.end());
            const auto tot_no_ddist =
                std::accumulate(
                    nonoffered_ddist.begin(), nonoffered_ddist.end(), 0.0f);
            const auto tot_o_ddist =
                std::accumulate(
                    offered_ddist.begin(), offered_ddist.end(), 0.0f);
            const auto avg_no_ddist =
                tot_no_ddist / nonoffered_ddist.size();
            const auto avg_o_ddist =
                tot_o_ddist / offered_ddist.size();

            append_to(row, *min_no_ddist / min_depot_dist);
            append_to(row, *min_o_ddist / min_depot_dist);

            append_to(row, *max_no_ddist / max_depot_dist);
            append_to(row, *max_o_ddist / max_depot_dist);

            append_to(row, avg_no_ddist / avg_depot_dist);
            append_to(row, avg_o_ddist / avg_depot_dist);

            float no_diam = 0.0f, o_diam = 0.0f;

            for(auto i = 0ul; i < instance.number_of_vertices(); ++i) {
                for(auto j = i + 1ul; j < instance.number_of_vertices(); ++j) {
                    const auto dist = instance.get_distance(i, j);

                    if((i == 0u || !offered[i-1u]) && !offered[j-1u]) {
                        if(dist > no_diam) {
                            no_diam = dist;
                        }
                    }

                    if((i == 0u || offered[i-1u]) && offered[j-1u]) {
                        if(dist > o_diam) {
                            o_diam = dist;
                        }
                    }
                }
            }

            append_to(row, no_diam / longest_arc);
            append_to(row, o_diam / longest_arc);

            return std::make_pair(true, row);
        }

        const Container& container() const {
            return c;
        }
    };

    using CsvDataset = Dataset<std::vector<float>>;
    using PyDataset = Dataset<boost::python::list, boost::python::list>;
}

#endif