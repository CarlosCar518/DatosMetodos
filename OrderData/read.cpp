#include "expPlan.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>

// Eliminar espacios y caracteres de salto de línea
static std::string trim(const std::string &text)
{
    const std::string whitespace = " \t\r\n";

    const auto start = text.find_first_not_of(whitespace);

    if (start == std::string::npos)
        return "";

    const auto end = text.find_last_not_of(whitespace);

    return text.substr(start, end - start + 1);
}

// Separar una línea utilizando las comas
static std::vector<std::string> split_csv_line(
    const std::string &line)
{
    std::vector<std::string> fields;

    std::stringstream stream(line);
    std::string field;

    while (std::getline(stream, field, ','))
    {
        fields.push_back(trim(field));
    }

    // Detectar una última columna vacía
    if (!line.empty() && line.back() == ',')
    {
        fields.emplace_back();
    }

    return fields;
}

// Convertir un texto a un número entero sin signo
template <typename T>
static T parse_number(
    const std::string &text,
    const std::string &column)
{
    if (text.empty() || text.front() == '-')
    {
        throw std::runtime_error(
            "Valor numerico invalido en " + column);
    }

    std::size_t consumed = 0;

    unsigned long long value = std::stoull(text, &consumed);

    if (consumed != text.size())
    {
        throw std::runtime_error(
            "Valor numerico invalido en " + column);
    }

    if (value > std::numeric_limits<T>::max())
    {
        throw std::runtime_error(
            "Valor fuera de rango en " + column);
    }

    return static_cast<T>(value);
}

// Leer el archivo de planificación
std::vector<ExperimentRun> read_experiment_plan(
    const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "No se pudo abrir el archivo: " + filename);
    }

    std::string line;

    // Leer encabezado
    if (!std::getline(file, line))
    {
        throw std::runtime_error("El CSV esta vacio");
    }

    const std::vector<std::string> expected_header = {
        "orden_programado",
        "id_corrida",
        "tratamiento",
        "algoritmo",
        "tamano",
        "replica",
        "semilla",
        "semilla_plan"};

    if (split_csv_line(line) != expected_header)
    {
        throw std::runtime_error(
            "El encabezado del CSV no tiene el formato esperado");
    }

    std::vector<ExperimentRun> plan;

    std::size_t line_number = 1;

    // Leer las corridas
    while (std::getline(file, line))
    {
        ++line_number;

        if (trim(line).empty())
            continue;

        try
        {
            auto fields = split_csv_line(line);

            if (fields.size() != 8)
            {
                throw std::runtime_error(
                    "La fila debe tener exactamente 8 columnas");
            }

            ExperimentRun run;

            run.order = parse_number<std::size_t>(
                fields[0], "orden_programado");

            run.run_id = fields[1];
            run.treatment = fields[2];
            run.algorithm = fields[3];

            run.data_size = parse_number<std::size_t>(
                fields[4], "tamano");

            run.replicate = parse_number<std::size_t>(
                fields[5], "replica");

            run.seed = parse_number<std::uint32_t>(
                fields[6], "semilla");

            run.plan_seed = parse_number<std::uint32_t>(
                fields[7], "semilla_plan");

            // Validar el algoritmo
            if (run.algorithm != "quick_sort" &&
                run.algorithm != "merge_sort" &&
                run.algorithm != "heap_sort")
            {
                throw std::runtime_error(
                    "Algoritmo desconocido: " + run.algorithm);
            }

            if (run.data_size == 0 || run.replicate == 0)
            {
                throw std::runtime_error(
                    "Tamano o replica no validos");
            }

            if (run.run_id.empty() || run.treatment.empty())
            {
                throw std::runtime_error(
                    "Identificador o tratamiento vacio");
            }

            // Comprobar que el orden sea consecutivo
            if (run.order != plan.size() + 1)
            {
                throw std::runtime_error(
                    "Orden de ejecucion incorrecto");
            }

            plan.push_back(run);
        }
        catch (const std::exception &error)
        {
            throw std::runtime_error(
                "Error en la linea " +
                std::to_string(line_number) +
                ": " + error.what());
        }
    }

    return plan;
}