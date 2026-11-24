#include "data_gen.hpp"

#include <iostream>

int main()
{
    std::cout << "Generando datos ...\n";

    auto data = generate_data(10000, 12345);

    for (std::size_t i = 0; i < 10; i++)
    {
        std::cout << data[i] << '\n';
    }

    return 0;
}