#include <cstdlib>
#include <greeting.hxx>

int main(int argc, char *argv[])
{
    const char *user_var = std::getenv("USER");
  
    std::string_view user = user_var != nullptr ? user_var : "USER - environment variable not found";

    bool is_good = greeting(user);
    int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
    return result;
}
