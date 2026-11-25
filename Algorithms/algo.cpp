
#include "algo.hpp"

#include <cstddef>
#include <utility>

// ============================================================
// QUICK SORT
// Mediana de tres + particion de Hoare (dos vias)
// ============================================================

// Obtener la mediana de tres valores.
static std::int32_t median_of_three(
    std::int32_t a,
    std::int32_t b,
    std::int32_t c)
{
    if (a > b)
        std::swap(a, b);

    if (b > c)
        std::swap(b, c);

    if (a > b)
        std::swap(a, b);

    return b;
}

// Particion de dos vias utilizando el esquema de Hoare.
// Los indices low y high son inclusivos.
static std::ptrdiff_t partition_two_way(
    std::vector<std::int32_t> &data,
    std::ptrdiff_t low,
    std::ptrdiff_t high)
{
    const std::ptrdiff_t middle = low + (high - low) / 2;

    const std::int32_t pivot = median_of_three(
        data[static_cast<std::size_t>(low)],
        data[static_cast<std::size_t>(middle)],
        data[static_cast<std::size_t>(high)]);

    std::ptrdiff_t i = low - 1;
    std::ptrdiff_t j = high + 1;

    while (true)
    {
        do
        {
            ++i;
        } while (data[static_cast<std::size_t>(i)] < pivot);

        do
        {
            --j;
        } while (data[static_cast<std::size_t>(j)] > pivot);

        if (i >= j)
            return j;

        std::swap(
            data[static_cast<std::size_t>(i)],
            data[static_cast<std::size_t>(j)]);
    }
}

// Ordenamiento recursivo de Quick Sort.
// Se procesa iterativamente la particion mayor
// para limitar la profundidad de la pila.
static void quick_sort_recursive(
    std::vector<std::int32_t> &data,
    std::ptrdiff_t low,
    std::ptrdiff_t high)
{
    while (low < high)
    {
        const std::ptrdiff_t p = partition_two_way(
            data, low, high);

        const std::ptrdiff_t left_size = p - low + 1;
        const std::ptrdiff_t right_size = high - p;

        if (left_size < right_size)
        {
            quick_sort_recursive(data, low, p);
            low = p + 1;
        }
        else
        {
            quick_sort_recursive(data, p + 1, high);
            high = p;
        }
    }
}

void quick_sort(std::vector<std::int32_t> &data)
{
    if (data.size() < 2)
        return;

    quick_sort_recursive(
        data,
        0,
        static_cast<std::ptrdiff_t>(data.size() - 1));
}

// ============================================================
// MERGE SORT
// Division y combinacion con un unico arreglo auxiliar
// ============================================================

// Combinar dos segmentos previamente ordenados:
// [left, middle) y [middle, right).
static void merge(
    std::vector<std::int32_t> &data,
    std::vector<std::int32_t> &auxiliary,
    std::size_t left,
    std::size_t middle,
    std::size_t right)
{
    std::size_t i = left;
    std::size_t j = middle;
    std::size_t k = left;

    // Comparar y combinar los elementos.
    while (i < middle && j < right)
    {
        if (data[i] <= data[j])
        {
            auxiliary[k++] = data[i++];
        }
        else
        {
            auxiliary[k++] = data[j++];
        }
    }

    // Copiar los elementos restantes del primer segmento.
    while (i < middle)
    {
        auxiliary[k++] = data[i++];
    }

    // Copiar los elementos restantes del segundo segmento.
    while (j < right)
    {
        auxiliary[k++] = data[j++];
    }

    // Copiar el segmento combinado al arreglo original.
    for (std::size_t index = left; index < right; ++index)
    {
        data[index] = auxiliary[index];
    }
}

// Ordenamiento recursivo de Merge Sort.
// Trabaja sobre intervalos semiabiertos [left, right).
static void merge_sort_recursive(
    std::vector<std::int32_t> &data,
    std::vector<std::int32_t> &auxiliary,
    std::size_t left,
    std::size_t right)
{
    if (right - left <= 1)
        return;

    const std::size_t middle = left + (right - left) / 2;

    merge_sort_recursive(data, auxiliary, left, middle);

    merge_sort_recursive(data, auxiliary, middle, right);

    merge(data, auxiliary, left, middle, right);
}

void merge_sort(std::vector<std::int32_t> &data)
{
    if (data.size() < 2)
        return;

    // Se reserva un unico arreglo auxiliar.
    // No se reserva memoria en cada llamada recursiva.
    std::vector<std::int32_t> auxiliary(data.size());

    merge_sort_recursive(
        data,
        auxiliary,
        0,
        data.size());
}

// ============================================================
// HEAP SORT
// Max-heap binario sobre el arreglo original
// ============================================================

// Restaurar la propiedad de max-heap
// desde el nodo root hacia abajo.
static void sift_down(
    std::vector<std::int32_t> &data,
    std::size_t root,
    std::size_t count)
{
    while (root < count / 2)
    {
        const std::size_t left = 2 * root + 1;
        const std::size_t right = left + 1;

        std::size_t largest = root;

        // Comparar con el hijo izquierdo.
        if (left < count && data[left] > data[largest])
        {
            largest = left;
        }

        // Comparar con el hijo derecho.
        if (right < count && data[right] > data[largest])
        {
            largest = right;
        }

        // La propiedad del heap ya se cumple.
        if (largest == root)
            return;

        std::swap(data[root], data[largest]);

        root = largest;
    }
}

void heap_sort(std::vector<std::int32_t> &data)
{
    const std::size_t count = data.size();

    if (count < 2)
        return;

    // 1. Construir el max-heap.
    for (std::size_t start = count / 2; start > 0; --start)
    {
        sift_down(data, start - 1, count);
    }

    // 2. Extraer sucesivamente el maximo.
    for (std::size_t end = count; end > 1; --end)
    {
        std::swap(data[0], data[end - 1]);

        sift_down(data, 0, end - 1);
    }
}
