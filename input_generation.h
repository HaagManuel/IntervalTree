#pragma once

#include <algorithm>
#include <vector>

#include "common.h"
#include "random.h"

// [i, i + len], i = 0, ..., n - 1
IntervalList fixed_length_sequence(int num_intervals, float interval_length)
{
    IntervalList intervals;
    intervals.reserve(num_intervals);
    for (int i = 0; i < num_intervals; i++)
    {
        intervals.emplace_back(i, i + interval_length);
    }
    return intervals;
}

IntervalList random_uniform_intervals(int num_intervals, float min_len, float max_len, float max_range, RandomGenerator &rnd)
{
    assert(max_len < max_range);
    assert(min_len <= max_len);
    auto start_points = rnd.n_random_float(num_intervals, 0, max_range - max_len);
    auto lens = rnd.n_random_float(num_intervals, min_len, max_len);
    IntervalList intervals;
    intervals.reserve(num_intervals);
    for (int i = 0; i < num_intervals; i++)
    {
        intervals.emplace_back(start_points[i], start_points[i] + lens[i]);
    }
    return intervals;
}