#include "ResWritter.hpp"

#include <fstream>
#include <iomanip>
#include <stdexcept>

// Crear el archivo y escribir el encabezado.
void create_results_file(
    const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "No se pudo crear el archivo de resultados");
    }

    file
        << "orden_programado,"
        << "id_corrida,"
        << "tratamiento,"
        << "algoritmo,"
        << "tamano,"
        << "replica,"
        << "semilla,"
        << "semilla_plan,"
        << "orden_real,"
        << "fecha_hora_inicio,"
        << "tiempo_segundos,"
        << "correcto,"
        << "incidencia\n";
}

// Agregar una observación.
void append_result(
    const std::string &filename,
    const ExperimentResult &result)
{
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "No se pudo abrir el archivo de resultados");
    }

    const ExperimentRun &run = result.run;

    file << std::setprecision(17);

    file
        << run.order << ','
        << run.run_id << ','
        << run.treatment << ','
        << run.algorithm << ','
        << run.data_size << ','
        << run.replicate << ','
        << run.seed << ','
        << run.plan_seed << ','
        << result.real_order << ','
        << result.start_time << ','
        << result.elapsed_seconds << ','
        << (result.correct ? "TRUE" : "FALSE") << ','
        << result.incident
        << '\n';

    if (!file)
    {
        throw std::runtime_error(
            "Error al escribir el resultado");
    }
}