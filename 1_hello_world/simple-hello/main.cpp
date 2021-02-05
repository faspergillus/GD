#include <iostream>

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "Hello, World!!" << std::endl;

    bool is_good = std::cout.good();
    int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;

    return result;
}
