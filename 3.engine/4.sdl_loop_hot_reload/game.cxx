#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>

#include "engine.hxx"

class my_concole_game : public pip::game
{
public:
    explicit my_concole_game(pip::engine&)
        : rotation_index{ 0 }
        , rotations_chars{ { '-', '/', '|', '\\' } }
    {
    }
    void initialize() override {}
    void on_event(pip::event) override {}
    void update() override
    {
        using namespace std;
        ++rotation_index;
        rotation_index %= rotations_chars.size();
        using namespace std::chrono;
        std::this_thread::sleep_for(milliseconds(20));
    }
    void render() const override
    {
        const char current_symbol = rotations_chars.at(rotation_index);
        std::cout << "\b" << current_symbol << std::flush;
        constexpr bool more = true;
        if constexpr (more)
        {
            std::cout << "\b\b\b" << current_symbol << current_symbol
                      << current_symbol << std::flush;
        }
    }

private:
    uint32_t                  rotation_index = 0;
    const std::array<char, 4> rotations_chars;
};

/// We have to export next two functions
pip::game* create_game(pip::engine* engine)
{
    if (engine != nullptr)
    {
        return new my_concole_game(*engine);
    }
    return nullptr;
}

void destroy_game(pip::game* game)
{
    delete game;
}
