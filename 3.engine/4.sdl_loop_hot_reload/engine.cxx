#include "engine.hxx"

#include <algorithm>
#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <SDL.h>

namespace pip
{

static std::array<std::string_view, 17> event_names = {
    { /// input events
      "left_pressed",
      "left_released",
      "right_pressed",
      "right_released",
      "up_pressed",
      "up_released",
      "down_pressed",
      "down_released",
      "select_pressed",
      "select_released",
      "start_pressed",
      "start_released",
      "button1_pressed",
      "button1_released",
      "button2_pressed",
      "button2_released",
      /// virtual console events
      "turn_off" }
};

std::ostream& operator<<(std::ostream& stream, const event e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value <= maximal)
    {
        stream << event_names[value];
        return stream;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

struct bind
{
    bind(SDL_Keycode k, std::string_view s, event pressed, event released)
        : key(k)
        , name(s)
        , event_pressed(pressed)
        , event_released(released)
    {
    }

    SDL_Keycode      key;
    std::string_view name;
    event            event_pressed;
    event            event_released;
};

const std::array<bind, 8> keys{
    { { SDLK_w, "up", event::up_pressed, event::up_released },
      { SDLK_a, "left", event::left_pressed, event::left_released },
      { SDLK_s, "down", event::down_pressed, event::down_released },
      { SDLK_d, "right", event::right_pressed, event::right_released },
      { SDLK_LCTRL, "button1", event::button1_pressed, event::button1_released },
      { SDLK_SPACE, "button2", event::button2_pressed, event::button2_released },
      { SDLK_ESCAPE, "select", event::select_pressed, event::select_released },
      { SDLK_RETURN, "start", event::start_pressed, event::start_released } }
};

static bool check_input(const SDL_Event& e, const bind*& result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys))
    {
        result = &(*it);
        return true;
    }
    return false;
}

class engine_impl final : public engine
{
public:
    /// create main window
    /// on success return empty string
    std::string initialize(std::string_view /*config*/) final
    {
        using namespace std;

        stringstream serr;

        const int init_result = SDL_Init(0);
        if (init_result != 0)
        {
            const char* err_message = SDL_GetError();
            serr << "error: failed call SDL_Init: " << err_message << endl;
            return serr.str();
        }

        return "";
    }
    /// pool event from input queue
    /// return true if more events in queue
    bool read_input(event& e) final
    {
        using namespace std;
        // collect all events from SDL
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            const bind* binding = nullptr;

            if (sdl_event.type == SDL_QUIT)
            {
                e = event::turn_off;
                return true;
            }
            else if (sdl_event.type == SDL_KEYDOWN)
            {
                if (check_input(sdl_event, binding))
                {
                    e = binding->event_pressed;
                    return true;
                }
            }
            else if (sdl_event.type == SDL_KEYUP)
            {
                if (check_input(sdl_event, binding))
                {
                    e = binding->event_released;
                    return true;
                }
            }
//            else if (sdl_event.type == SDL_CONTROLLERDEVICEADDED)
//            {
//                // TODO map controller to user
//                std::cerr << "controller added" << std::endl;
//                // continue with next event in queue
//                return read_input(e);
//            }
//            else if (sdl_event.type == SDL_CONTROLLERDEVICEREMOVED)
//            {
//                std::cerr << "controller removed" << std::endl;
//            }
//            else if (sdl_event.type == SDL_CONTROLLERBUTTONDOWN ||
//                     sdl_event.type == SDL_CONTROLLERBUTTONUP)
//            {
//                // TODO finish implementation
//                if (sdl_event.cbutton.state == SDL_PRESSED)
//                {
//                    e = event::button1_pressed;
//                }
//                else
//                {
//                    e = event::button1_released;
//                }
//                return true;
//            }
        }
        return false;
    }
    void deinitialization() final {}
};

static bool already_exist = false;

engine* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    engine* result = new engine_impl();
    already_exist  = true;
    return result;
}

void destroy_engine(engine* e)
{
    if (already_exist == false)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

engine::~engine() {}

} // end namespace om



pip::game* reload_game(pip::game*   old,
                      const char* library_name,
                      const char* tmp_library_name,
                      pip::engine& engine,
                      void*&      old_handle);

// clang-format off
#ifdef __cplusplus
extern "C"
#endif
int main(int /*argc*/, char* /*argv*/[])
// clang-format on
{
    std::unique_ptr<pip::engine, void (*)(pip::engine*)> engine(
        pip::create_engine(), pip::destroy_engine);

    std::string err = engine->initialize("");
    if (!err.empty())
    {
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "start app\n" << std::endl;



    using namespace std::string_literals;

    const char* library_name = "./libgame.so";

    using namespace std::filesystem;

    const char* tmp_library_file = "./temp.dll";

    void* game_library_handle{};
    pip::game* game = reload_game(
        nullptr, library_name, tmp_library_file, *engine, game_library_handle);

    auto time_during_loading = last_write_time(library_name);

    game->initialize();

    bool continue_loop = true;
    while (continue_loop)
    {
        auto current_write_time = last_write_time(library_name);

        if (current_write_time != time_during_loading)
        {
            file_time_type next_write_time;
            // wait while library file fishish to changing
            for (;;)
            {
                using namespace std::chrono;
                std::this_thread::sleep_for(milliseconds(100));
                next_write_time = last_write_time(library_name);
                if (next_write_time != current_write_time)
                {
                    current_write_time = next_write_time;
                }
                else
                {
                    break;
                }
            };

            std::cout << "reloading game" << std::endl;
            game = reload_game(game,
                               library_name,
                               tmp_library_file,
                               *engine,
                               game_library_handle);

            if (game == nullptr)
            {
                std::cerr << "next attemp to reload game..." << std::endl;
                continue;
            }

            time_during_loading = next_write_time;
        }

        pip::event event;

        while (engine->read_input(event))
        {
            std::cout << event << std::endl;
            switch (event)
            {
                case pip::event::turn_off:
                    continue_loop = false;
                    break;
                default:
                    game->on_event(event);
                    break;
            }
        }

        game->update();
        game->render();
    }

    engine->deinitialization();

    return EXIT_SUCCESS;
}

pip::game* reload_game(pip::game*   old,
                      const char* library_name,
                      const char* tmp_library_name,
                      pip::engine& engine,
                      void*&      old_handle)
{
    using namespace std::filesystem;

    if (old)
    {
        SDL_UnloadObject(old_handle);
    }

    if (std::filesystem::exists(tmp_library_name))
    {
        if (0 != remove(tmp_library_name))
        {
            std::cerr << "error: can't remove: " << tmp_library_name
                      << std::endl;
            return nullptr;
        }
    }

    try
    {
        copy(library_name, tmp_library_name); // throw on error
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: can't copy [" << library_name << "] to ["
                  << tmp_library_name << "]" << std::endl;
        return nullptr;
    }

    void* game_handle = SDL_LoadObject(tmp_library_name);

    if (game_handle == nullptr)
    {
        std::cerr << "error: failed to load: [" << tmp_library_name << "] "
                  << SDL_GetError() << std::endl;
        return nullptr;
    }

    old_handle = game_handle;

    void* create_game_func_ptr = SDL_LoadFunction(game_handle, "create_game");

    if (create_game_func_ptr == nullptr)
    {
        std::cerr << "error: no function [create_game] in " << tmp_library_name
                  << " " << SDL_GetError() << std::endl;
        return nullptr;
    }
    // void* destroy_game_func_ptr = SDL_LoadFunction(game_handle,
    // "destroy_game");

    typedef decltype(&create_game) create_game_ptr;

    auto create_game_func =
        reinterpret_cast<create_game_ptr>(create_game_func_ptr);

    pip::game* game = create_game_func(&engine);

    if (game == nullptr)
    {
        std::cerr << "error: func [create_game] returned: nullptr" << std::endl;
        return nullptr;
    }
    return game;
}
