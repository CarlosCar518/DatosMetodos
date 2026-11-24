#ifndef EXPERIMENT_PLAN_HPP
#define EXPERIMENT_PLAN_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct ExperimentRun
{
    std::size_t order;
    std::string run_id;
    std::string treatment;
    std::string algorithm;

    std::size_t data_size;
    std::size_t replicate;

    std::uint32_t seed;
    std::uint32_t plan_seed;
};

std::vector<ExperimentRun> read_experiment_plan(
    const std::string &filename);

#endif