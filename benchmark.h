#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "common.h"
#include "input_generation.h"
#include "measure_time.h"
#include "brute_force_line_stap.h"
#include "array_interval_tree.h"
#include "pointer_interval_tree.h"
#include "random.h"

template <typename T>
float get_mean(std::vector<T> &v)
{
    assert(v.size() > 0);
    T sum = std::accumulate(v.begin(), v.end(), 0);
    return sum / v.size();
}

template <typename Algo>
void benchmark_construction(int repeats, std::vector<int> &sizes)
{
    float min_len = 1;
    float max_len = 10;
    float max_range = 100;
    int seed = 0;
    RandomGenerator rnd(seed);
    std::cout << "construction"
              << "\n";
    for (auto n : sizes)
    {
        std::vector<int> times;
        for (int i = 0; i < repeats; i++)
        {
            seed++;
            IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range, rnd);
            auto f = [&]()
            {
                Algo algo(list);
            };
            int timeMs = measureTimeMs(f);
            times.push_back(timeMs);
        }
        std::string unit = "ms";
        float mean = get_mean(times);
        std::cout << "n = " << n << " --> " << mean << " " << unit << "\n";
    }
    std::cout << "\n";
}

template <typename Algo>
void benchmark_query(int num_queries, std::vector<int> &sizes)
{
    float min_len = 1;
    float max_len = 10;
    float max_range = 5000;
    int seed = 0;
    RandomGenerator rnd(seed);

    std::cout << "query"
              << "\n";

    for (auto n : sizes)
    {
        std::vector<int> ids;
        IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range, rnd);
        std::vector<float> queries = rnd.n_random_float(num_queries, 0, max_range);
        Algo algo(list);
        long long num_hits = 0;
        auto f = [&]()
        {
            for (auto p : queries)
            {
                algo.stabbed_intervals(p, ids);
                num_hits += ids.size();
                ids.clear();
            }
        };
        int timeMs = measureTimeMs(f) + 0.0001;
        float mean = (float)timeMs / num_queries;
        float throughput = (float)num_queries / timeMs;
        float mean_hits = (float)num_hits / num_queries;
        std::string unit = "ms";
        std::cout << "n = " << n << " --> "
                  << mean << " " << unit << ", "
                  << mean_hits << " hits, "
                  << throughput << " MOPS, "
                  << "per query"
                  << "\n";
    }
    std::cout << "\n";
}

void print_csv_header()
{
    std::vector<std::string> names = {"num_intervals", "algo", "queries_micro", "construction_micro", "queries", "p_hit", "hits", "depth", "memory_bytes", "seed", "exp_name"};
    std::string sep = ",";
    std::cout << names[0];
    for (uint i = 1; i < names.size(); i++)
    {
        std::cout << sep << names[i];
    }
    std::cout << "\n";
}

template <typename Algo>
void benchmark_csv(int num_intervals, std::string algo_name, int num_queries, float prob_hit, int seed, std::string exp_name)
{
    std::cerr << num_intervals << " " << algo_name << " " << num_queries << " " << prob_hit << " " << seed << " " << exp_name << "\n";

    // all intervals have same length
    float min_len = 100;
    float max_len = 100;
    float max_range = 100 / prob_hit;
    RandomGenerator rnd(seed);
    IntervalList list = random_uniform_intervals(num_intervals, min_len, max_len, max_range, rnd);
    std::vector<float> queries = rnd.n_random_float(num_queries, 0, max_range);
    std::vector<int> ids;

    // construction
    auto construction = [&]()
    {
        Algo algo(list);
    };
    int time_construction = measureTimeMicroS(construction);

    // queries
    Algo algo(list);
    long long num_hits = 0;
    auto measure_queries = [&]()
    {
        for (auto p : queries)
        {
            algo.stabbed_intervals(p, ids);
            num_hits += ids.size();
            ids.clear();
        }
    };
    int time_queries = measureTimeMicroS(measure_queries);
    int depth = algo.get_depth();
    int memory_bytes = algo.get_memory_bytes();

    std::string sep = ",";
    std::cout << num_intervals << sep
              << algo_name << sep
              << time_queries << sep
              << time_construction << sep
              << num_queries << sep
              << prob_hit << sep
              << num_hits << sep
              << depth << sep
              << memory_bytes << sep
              << seed << sep
              << exp_name
              << "\n";
}

void create_csv()
{
    std::vector<int> ns;
    std::vector<float> ps;
    std::string algo;
    int queries;
    int runs;
    int min_n;
    std::string exp_name = "";

    auto set_ns = [&](int upper)
    {
        ns.clear();
        for (int n = min_n; n <= upper; n *= 2)
            ns.push_back(n);
    };

    auto run = [&](auto f)
    {
        for (int n : ns)
        {
            for (float p : ps)
            {
                for (int i = 0; i < runs; i++)
                {
                    int seed = i;
                    f(n, algo, queries, p, seed, exp_name);
                }
            }
        }
    };

    runs = 10;
    min_n = 1 << 10;

    exp_name = "exp1";
    ps = {0.001, 0.01, 0.1, 0.4, 0.8};

    algo = "brute_force";
    queries = 1e2;
    set_ns(1 << 20);
    run(benchmark_csv<BruteForceLineStab>);

    algo = "pt_interval_tree";
    queries = 1e3;
    set_ns(1 << 20);
    run(benchmark_csv<PointerIntervalTree>);

    exp_name = "exp2";
    ps = {1e-5, 1e-4, 0.001, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 1.0};
    ns = {1 << 20};

    algo = "brute_force";
    queries = 1e2;
    run(benchmark_csv<BruteForceLineStab>);

    algo = "pt_interval_tree";
    queries = 1e3;
    run(benchmark_csv<PointerIntervalTree>);
}