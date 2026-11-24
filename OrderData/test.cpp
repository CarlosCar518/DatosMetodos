#include "expPlan.hpp"

#include <iostream>
#include <exception>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Uso: test_plan_reader <archivo.csv>\n";
        return 1;
    }

    try
    {
        auto plan = read_experiment_plan(argv[1]);

        std::cout << "Corridas leidas: "
                  << plan.size() << "\n\n";

        for (const ExperimentRun &run : plan)
        {
            std::cout
                << "Orden: " << run.order << '\n'
                << "ID: " << run.run_id << '\n'
                << "Tratamiento: " << run.treatment << '\n'
                << "Algoritmo: " << run.algorithm << '\n'
                << "Tamano: " << run.data_size << '\n'
                << "Replica: " << run.replicate << '\n'
                << "Semilla: " << run.seed << '\n'
                << "Semilla del plan: " << run.plan_seed
                << "\n\n";
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}