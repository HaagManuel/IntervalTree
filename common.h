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
    int id;

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
    int id;
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
float get_split_point_median(IntervalIdList &id_list)
{
    // point and is end
    std::vector<std::pair<float, bool>> points;
    int n = id_list.size();
    points.reserve(2 * n);

    for (int i = 0; i < n; i++)
    {
        points.emplace_back(id_list[i].interval.start, false);
        points.emplace_back(id_list[i].interval.end, true);
    }
    auto m = points.begin() + points.size() / 2;
    std::nth_element(points.begin(), m, points.end());
    return points[points.size() / 2].first;
}

float get_split_point_sweepline(IntervalIdList &id_list)
{
    // point and is end
    std::vector<std::pair<float, bool>> points;
    int n = id_list.size();
    points.reserve(2 * n);

    // sort set of start and end points
    for (int i = 0; i < n; i++)
    {
        points.emplace_back(id_list[i].interval.start, false);
        points.emplace_back(id_list[i].interval.end, true);
    }
    std::sort(points.begin(), points.end());

    // sweep line
    int active = 0;
    int left = 0;
    int best_balance = n;
    float best_point = 0;
    for (int i = 0; i < 2 * n - 1; i++)
    {
        auto [p, is_end] = points[i];
        if (!is_end)
        {
            active++;
        }
        else
        {
            active--;
            left++;
        }
        int right = n - active - left;
        int balance = std::abs(left - right);
        if (balance < best_balance)
        {
            best_balance = balance;
            best_point = (p + points[i + 1].first) / 2;
        }
        if (balance == 0 || left > right)
        {
            break;
        }
    }
    return best_point;
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