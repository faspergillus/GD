#include <iostream>
#include <string_view>

bool greeting(std::string_view user_name)
{
    std::cout << "Hello, " << user_name << std::endl;

    return std::cout.good();
}
