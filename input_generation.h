#pragma once

#include <vector>

#include "common.h"

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
