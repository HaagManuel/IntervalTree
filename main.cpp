#include <iostream>

#include "common.h"
#include "input_generation.h"
#include "brute_force_line_stap.h"

int main()
{
    std::cout << "hello interval tree \n";


    IntervalList list = fixed_length_sequence(10, 3.0);

    print_vector(list);

    BruteForceLineStab bf(list);
    std::vector<int> ids;

    float p = 5;
    bf.stabbed_intervals(p, ids);

    print_vector(ids);



    return 0;
}