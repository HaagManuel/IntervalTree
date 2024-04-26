#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "measure_time.h"

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
    std::cout << "construction"
              << "\n";
    for (auto n : sizes)
    {
        std::vector<int> times;
        for (int i = 0; i < repeats; i++)
        {
            seed++;
            IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range, seed);
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
    std::cout << "query"
              << "\n";

    RandomGenerator rnd;
    for (auto n : sizes)
    {
        std::vector<int> ids;
        seed++;
        IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range, seed);
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
        int timeMs = measureTimeMs(f);
        float mean = (float)timeMs / num_queries;
        float mean_hits = (float)num_hits / num_queries;
        std::string unit = "ms";
        std::cout << "n = " << n << " --> " << mean << " " << unit << ", " << mean_hits << " hits, per query"
                  << "\n";
    }
    std::cout << "\n";
}