
#include "input.h"

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
