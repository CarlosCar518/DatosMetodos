#include "./Algorithms/expRun.hpp"
#include "./GenData/data_gen.hpp"
#include "Writer/ResWritter.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Verificar argumentos.
    if (argc != 3)
    {
        std::cerr
            << "Uso: experiment <plan.csv> <resultados.csv>\n";

        return 1;
    }

    try
    {
        const std::string plan_filename = argv[1];
        const std::string results_filename = argv[2];

        // 1. Leer el plan experimental.
        auto plan = read_experiment_plan(plan_filename);

        if (plan.empty())
        {
            std::cerr << "El plan no contiene corridas.\n";
            return 1;
        }

        std::cout
            << "Corridas encontradas: "
            << plan.size()
            << "\n\n";

        // 2. Crear el archivo de resultados.
        create_results_file(results_filename);

        // 3. Ejecutar todas las corridas.
        for (std::size_t i = 0; i < plan.size(); ++i)
        {
            const ExperimentRun &current_run = plan[i];

            std::cout
                << "====================================\n"
                << "Corrida: " << i + 1 << "/" << plan.size() << '\n'
                << "ID: " << current_run.run_id << '\n'
                << "Tratamiento: " << current_run.treatment << '\n'
                << "Algoritmo: " << current_run.algorithm << '\n'
                << "Tamano: " << current_run.data_size << '\n'
                << "Replica: " << current_run.replicate << '\n'
                << "Semilla: " << current_run.seed << '\n'
                << "====================================\n";

            // Ejecutar y medir.
            ExperimentResult result = execute_run(
                current_run,
                i + 1);

            // Guardar el resultado.
            append_result(results_filename, result);

            // Mostrar información.
            std::cout
                << "Tiempo: "
                << result.elapsed_seconds
                << " segundos\n";

            std::cout
                << "Correctitud: "
                << (result.correct ? "CORRECTO" : "INCORRECTO")
                << '\n';

            if (!result.incident.empty())
            {
                std::cout
                    << "Incidencia: "
                    << result.incident
                    << '\n';
            }

            std::cout << '\n';
        }

        std::cout
            << "====================================\n"
            << "EXPERIMENTO FINALIZADO\n"
            << "Total de corridas: " << plan.size() << '\n'
            << "Resultados guardados en: "
            << results_filename << '\n'
            << "====================================\n";
    }
    catch (const std::exception &error)
    {
        std::cerr
            << "\nERROR: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}
