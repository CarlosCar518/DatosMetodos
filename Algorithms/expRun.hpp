#ifndef EXPERIMENT_RUNNER_HPP
#define EXPERIMENT_RUNNER_HPP

#include "../OrderData/expPlan.hpp"

#include <cstddef>
#include <string>

struct ExperimentResult
{
    ExperimentRun run;

    std::size_t real_order;

    std::string start_time;

    double elapsed_seconds;

    bool correct;

    std::string incident;
};

ExperimentResult execute_run(
    const ExperimentRun &run,
    std::size_t real_order);

#endif