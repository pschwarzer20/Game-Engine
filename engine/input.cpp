

#include <SDL.h>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}


#include "input.h"

// Implement KeyPressed that basically just gets called once on process_input down = true
// resets when the key is released again

void process_input(SDL_Keysym &keysym, bool down, lua_State *luaState) {
    lua_getglobal(luaState, "engine");

    if (lua_istable(luaState, -1)) {
        if (down) {
            lua_getfield(luaState, -1, "KeyDown");
            if (lua_isfunction(luaState, -1)) {
                lua_pushinteger(luaState, keysym.scancode);

                if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                    const char* error = lua_tostring(luaState, -1);
                    std::cerr << "Lua error in engine.KeyDown: " << error << std::endl;
                }
            }
        }
        else
        {
            lua_getfield(luaState, -1, "KeyReleased");
            if (lua_isfunction(luaState, -1)) {
                lua_pushinteger(luaState, keysym.scancode);

                if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                    const char* error = lua_tostring(luaState, -1);
                    std::cerr << "Lua error in engine.KeyReleased: " << error << std::endl;
                }
            }
        }
    }
}
