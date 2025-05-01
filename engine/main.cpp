
#include <SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <filesystem>
#include <unordered_map>
namespace fs = std::filesystem;

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include "input.h"

bool isRunning = false;
lua_State* luaState;

// PLACEHOLDER
std::unordered_map<std::string, std::filesystem::file_time_type> loadedLuaFiles;
void loadLuaScripts(lua_State* L, const std::string& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".lua") {
            std::string filePath = entry.path().string();
            auto lastWrite = fs::last_write_time(entry);

            if (loadedLuaFiles.find(filePath) == loadedLuaFiles.end() || loadedLuaFiles[filePath] != lastWrite){
                loadedLuaFiles[filePath] = lastWrite;

                std::cout << "Loading Lua file: " << filePath << std::endl;
                if (luaL_dofile(L, filePath.c_str()) != LUA_OK) {
                    std::cerr << "Lua error in " << filePath << ": " << lua_tostring(L, -1) << std::endl;
                    lua_pop(L, 1);
                }
            }
        }
    }
}

int closeGame(lua_State* L) {
    isRunning = false;

    return 0;
}

void createLuaTables() {
    lua_newtable(luaState);

    // Add Close() to engine table
    lua_pushcfunction(luaState, closeGame);
    lua_setfield(luaState, -2, "Close");

    lua_setglobal(luaState, "engine");

    // PLACEHOLDER
    // Add global KEY_* constants
    for (int scancode = 0; scancode < SDL_NUM_SCANCODES; ++scancode) {
        const char* name = SDL_GetScancodeName(static_cast<SDL_Scancode>(scancode));
        if (name && name[0] != '\0') {
            std::string keyName = "KEY_";

            for (int i = 0; name[i]; ++i) {
                if (name[i] == ' ')
                    keyName += '_';
                else
                    keyName += std::toupper(name[i]);
            }

            lua_pushinteger(luaState, scancode);
            lua_setglobal(luaState, keyName.c_str());
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Starting...\n";

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "SDL initialized!\n";

    SDL_Window* window = SDL_CreateWindow("engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "SDL Window created.\n";

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "SDL Context created.\n";

    // Load OpenGL functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout << "GLAD initialized!\n";

    // Create Lua State
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    createLuaTables();
    std::cout << "Lua State created!\n";

    std::cout << "Running game loop...\n";
    isRunning = true;
    SDL_Event event;
    while (isRunning) {
        loadLuaScripts(luaState, "lua");

        while (SDL_PollEvent(&event)) {
            SDL_KeyboardEvent* key;
            key = &event.key;
            SDL_Keysym* keysym;
            keysym = &key->keysym;

            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_KEYUP:
                    process_input(*keysym, false, luaState);
                    break;

                case SDL_KEYDOWN:
                    process_input(*keysym, true, luaState);
                    break;

                default:
                    break;
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    std::cout << "Shutting down..\n";

    lua_close(luaState);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}