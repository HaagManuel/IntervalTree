#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>

#define LOG(x) std::cout << std::string(#x " = ") << (x) << "\n";

struct Interval
{
    float start;
    float end;

    bool contains_point(float p) const
    {
        return start <= p && p <= end;
    }

    bool operator<(const Interval &other) const
    {
        if (start == other.start)
        {
            return end < other.end;
        }
        else
        {
            return start < other.start;
        }
    }
};

struct PointId
{
    float point;
    float id;

    bool operator<(const PointId &other) const
    {
        if (point == other.point)
        {
            return id < other.id;
        }
        else
        {
            return point < other.point;
        }
    }
};

struct IntervalId
{
    Interval interval;
    float id;
};

using IntervalList = std::vector<Interval>;
using IntervalIdList = std::vector<IntervalId>;

IntervalIdList get_id_intervals(IntervalList &list)
{
    IntervalIdList id_list;
    id_list.reserve(list.size());
    for (uint i = 0; i < list.size(); i++)
    {
        id_list.emplace_back(list[i], i);
    }
    return id_list;
}

std::ostream &operator<<(std::ostream &out, const Interval &i)
{
    out << "[" << i.start << ", " << i.end << "]";
    return out;
}

void sort_by_intervals(IntervalList &list)
{
    std::sort(list.begin(), list.end());
}

bool check_intervals_sorted(IntervalList &list)
{
    for (uint i = 1; i < list.size(); i++)
    {
        if (list[i] < list[i - 1])
            return false;
    }
    return true;
}

template <typename T>
void print_vector(std::vector<T> &v, std::string sep = " ")
{
    for (auto &x : v)
    {
        std::cout << x << sep;
    }
    std::cout << "\n";
}