#include <iostream>

#include "common.h"
#include "input_generation.h"
#include "brute_force_line_stap.h"
#include "pointer_interval_tree.h"
#include "benchmark.h"

int main()
{
    std::cout << "hello interval tree \n";

    // int n = 5;
    // std::vector<int> sizes = {1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10};
    // benchmark_construction<PointerIntervalTree>(n, sizes);

    // int queries = 1000;
    // std::vector<int> sizes = {1000 * 100, 1000 * 1000, 1000 * 1000 * 10, 1000 * 1000 * 20, 1000 * 1000 * 40};

    // std::cout << "brute force"
    //           << "\n";
    // benchmark_query<BruteForceLineStab>(queries, sizes);

    // std::cout << "pointer interval tree"
    //           << "\n";
    // benchmark_query<PointerIntervalTree>(queries, sizes);

    int n = 1e6;
    float min_len = 1;
    float max_len = 10;
    float max_range = 5000;

    IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range);
    PointerIntervalTree pt_tree(list);
    pt_tree.nodes_per_level();

    return 0;
}