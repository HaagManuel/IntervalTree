#pragma once

#include <random>
#include <algorithm>

struct RandomGenerator
{
    RandomGenerator() : gen(0) {}

    RandomGenerator(int seed) : gen(seed) {}

    float random_float(float lower, float upper)
    {
        std::uniform_real_distribution<float> dis(lower, upper);
        return dis(gen);
    }

    std::vector<float> n_random_float(int n, float lower, float upper)
    {
        std::vector<float> xs(n);
        std::generate(xs.begin(), xs.end(), [&]()
                      { return random_float(lower, upper); });
        return xs;
    }

    std::mt19937 gen;
};
