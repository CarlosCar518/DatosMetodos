#include "expRun.hpp"

#include "algo.hpp"
#include "../GenData/data_gen.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

// Obtener fecha y hora del sistema.
static std::string get_current_time()
{
    auto now = std::chrono::system_clock::now();

    std::time_t time =
        std::chrono::system_clock::to_time_t(now);

    std::tm *local = std::localtime(&time);

    if (local == nullptr)
    {
        throw std::runtime_error("No se pudo obtener la hora");
    }

    std::ostringstream output;

    output << std::put_time(
        local,
        "%Y-%m-%d %H:%M:%S");

    return output.str();
}

// Seleccionar la función de ordenamiento.
using SortFunction = void (*)(
    std::vector<std::int32_t> &);

static SortFunction select_algorithm(
    const std::string &algorithm)
{
    if (algorithm == "quick_sort")
        return quick_sort;

    if (algorithm == "merge_sort")
        return merge_sort;

    if (algorithm == "heap_sort")
        return heap_sort;

    throw std::invalid_argument(
        "Algoritmo desconocido: " + algorithm);
}

// Ejecutar una corrida experimental.
ExperimentResult execute_run(
    const ExperimentRun &run,
    std::size_t real_order)
{
    // 1. Generar el arreglo.
    auto data = generate_data(
        run.data_size,
        run.seed);

    // 2. Crear una copia de referencia.
    // Esta preparación queda fuera del cronómetro.
    auto reference = data;

    std::sort(
        reference.begin(),
        reference.end());

    // 3. Seleccionar el algoritmo antes de medir.
    SortFunction sorter = select_algorithm(
        run.algorithm);

    // 4. Registrar la hora de inicio.
    std::string start_time = get_current_time();

    // 5. Medir exclusivamente el algoritmo.
    auto start = std::chrono::steady_clock::now();

    sorter(data);

    auto end = std::chrono::steady_clock::now();

    double elapsed =
        std::chrono::duration<double>(end - start).count();

    // 6. Verificar la correctitud fuera del cronómetro.
    bool correct = (data == reference);

    std::string incident = "";

    if (!correct)
    {
        incident = "resultado_no_coincide_con_referencia";
    }

    // 7. Construir el resultado.
    ExperimentResult result;

    result.run = run;
    result.real_order = real_order;
    result.start_time = start_time;
    result.elapsed_seconds = elapsed;
    result.correct = correct;
    result.incident = incident;

    return result;
}