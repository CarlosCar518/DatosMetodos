#ifndef RESULTS_WRITER_HPP
#define RESULTS_WRITER_HPP

#include "../Algorithms/expRun.hpp"

#include <string>

void create_results_file(
    const std::string &filename);

void append_result(
    const std::string &filename,
    const ExperimentResult &result);

#endif