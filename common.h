#pragma once

#include <vector>
#include <iostream>
#include <fstream>

struct Interval
{
    float start;
    float end;

    bool contains_point(float p)
    {
        return start <= p && p <= end;
    }
};

using IntervalList = std::vector<Interval>;

std::ostream &operator<<(std::ostream &out, const Interval &i)
{
    out << "[" << i.start << ", " << i.end << "]";
    return out;
}

void sort_by_start(IntervalList &list)
{
    std::sort(list.begin(), list.end(), [&](auto &a, auto &b)
              { return a.start < b.start; });
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