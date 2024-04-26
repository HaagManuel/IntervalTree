#include <iostream>

#include "common.h"
#include "input_generation.h"
#include "brute_force_line_stap.h"
#include "pointer_interval_tree.h"

int main()
{
    std::cout << "hello interval tree \n";

    IntervalList list = fixed_length_sequence(10, 3.0);

    sort_by_intervals(list);
    print_vector(list);

    BruteForceLineStab bf(list);
    PointerIntervalTree pt_tree(list);
    std::vector<int> ids;

    float p = 5;

    bf.stabbed_intervals(p, ids);
    print_vector(ids);
    ids.clear();
    
    pt_tree.stabbed_intervals(p, ids);
    print_vector(ids);
    ids.clear();

    return 0;
}