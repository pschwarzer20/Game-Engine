
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include "input.h"


bool isRunning = false;
lua_State* luaState;

void createLuaTables() {
    lua_newtable(luaState);
    lua_setglobal(luaState, "engine");
}

// TEMPORARY
void process_input(SDL_Keysym keysym) {
    // Call engine.keypressed(key)
    lua_getglobal(luaState, "engine");
    if (lua_istable(luaState, -1)) {
        lua_getfield(luaState, -1, "KeyPressed");
        if (lua_isfunction(luaState, -1)) {
            lua_pushinteger(luaState, keysym.scancode);

            if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                const char* error = lua_tostring(luaState, -1);
                std::cerr << "Lua error in engine.keypressed: " << error << std::endl;
            }
        }
    }

    switch (keysym.scancode) {
    case SDL_SCANCODE_ESCAPE:
        isRunning = false;

        std::cout << "Closing game\n";
        break;
    default:
        break;
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

    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    createLuaTables();
    std::cout << "Lua State created!\n";

    if (luaL_dofile(luaState, "test.lua") != LUA_OK) {
        const char* error = lua_tostring(luaState, -1);
        printf("Lua error: %s\n", error);
    }

    std::cout << "Running game loop...\n";
    isRunning = true;
    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_KEYUP:
                    SDL_KeyboardEvent* key;
                    key = &event.key;
                    SDL_Keysym* keysym;
                    keysym = &key->keysym;

                    process_input(*keysym);

                    break;

                default:
                    break;
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    lua_close(luaState);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}