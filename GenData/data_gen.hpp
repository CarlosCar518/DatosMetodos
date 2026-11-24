#ifndef DATA_GENERATOR_HPP
#define DATA_GENERATOR_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::int32_t> generate_data(
    std::size_t size,
    std::uint32_t seed);

#endif