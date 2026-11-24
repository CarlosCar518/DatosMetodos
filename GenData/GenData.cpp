#include "data_gen.hpp"

#include <random>

std::vector<std::int32_t> generate_data(
    std::size_t size,
    std::uint32_t seed)
{
    std::mt19937 generator(seed);

    std::uniform_int_distribution<std::int32_t> distribution(
        0, 999999999);

    std::vector<std::int32_t> data(size);

    for (std::int32_t &value : data)
    {
        value = distribution(generator);
    }

    return data;
}