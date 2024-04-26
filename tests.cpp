#include <gtest/gtest.h>
#include <vector>
#include <iostream>

#include "input_generation.h"
#include "brute_force_line_stap.h"
#include "pointer_interval_tree.h"

void check_ids_same(std::vector<int> &a, std::vector<int> &b)
{
    if (a.size() != b.size())
    {
        std::cout << "id vectors have different sizes, " << a.size() << " != " << b.size() << "\n";
        ASSERT_TRUE(false);
    }
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    if (a != b)
    {
        std::cout << "id vectors differ \n";
        ASSERT_TRUE(false);
    }
}

template <typename A, typename B>
void check_equal_spaced_queries(A &algo1, B &algo2, float start, float stop, float step)
{
    std::vector<int> ids1, ids2;
    for (float p = start; p <= stop; p += step)
    {
        algo1.stabbed_intervals(p, ids1);
        algo2.stabbed_intervals(p, ids2);
        check_ids_same(ids1, ids2);

        ids1.clear();
        ids2.clear();
    }
}

TEST(EqualSpacedIntervals, IntervalTree)
{
    int n = 10;
    float len = 3.5;
    IntervalList list = fixed_length_sequence(n, len);

    BruteForceLineStab bf(list);
    PointerIntervalTree pt_tree(list);

    check_equal_spaced_queries(bf, pt_tree, -2, n + len + 2, 0.25);
}

TEST(RandomUniformIntervals, IntervalTree)
{
    int n = 1000;
    float min_len = 1;
    float max_len = 10;
    float max_range = 100;
    int seed = 3;

    IntervalList list = random_uniform_intervals(n, min_len, max_len, max_range, seed);
    BruteForceLineStab bf(list);
    PointerIntervalTree pt_tree(list);

    float start = -2;
    float end = max_range + 2;
    float step = 0.25;
    check_equal_spaced_queries(bf, pt_tree, start, end, step);
}
