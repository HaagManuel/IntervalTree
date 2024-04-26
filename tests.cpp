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

TEST(EqualSpacedIntervals, IntervalTree)
{
    int n = 10;
    float len = 3.5;
    IntervalList list = fixed_length_sequence(n, len);

    BruteForceLineStab bf(list);
    PointerIntervalTree pt_tree(list);
    std::vector<int> ids1;
    std::vector<int> ids2;

    for (float p = -2; p <= n + len + 2; p += 0.25)
    {
        bf.stabbed_intervals(p, ids1);
        pt_tree.stabbed_intervals(p, ids2);
        check_ids_same(ids1, ids2);
        ids1.clear();
        ids2.clear();
    }
}
