#pragma once

#include <vector>
#include <cassert>
#include "common.h"

struct BruteForceLineStab
{
    BruteForceLineStab(IntervalList &_intervals) : intervals(_intervals)
    {
    }

    // linear scan all intervals
    void stabbed_intervals(float point, std::vector<int> &ids)
    {
        int i = 0;
        for (auto &intv : intervals)
        {
            if (intv.contains_point(point))
            {
                ids.push_back(i);
            }
            i++;
        }
    }

    IntervalList &intervals;
};