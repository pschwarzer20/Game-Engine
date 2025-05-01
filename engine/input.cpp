

#include <SDL.h>
#include <unordered_set>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}


#include "input.h"

static std::unordered_set<int> heldKeys;

void process_input(SDL_Keysym &keysym, bool down, lua_State *luaState) {
    lua_getglobal(luaState, "engine");

    if (lua_istable(luaState, -1)) {
        int scancode = keysym.scancode;

        if (down) {
            lua_getfield(luaState, -1, "KeyDown");
            if (lua_isfunction(luaState, -1)) {
                lua_pushinteger(luaState, scancode);

                if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                    const char* error = lua_tostring(luaState, -1);
                    std::cerr << "Lua error in engine.KeyDown: " << error << std::endl;
                }
            }

            if (heldKeys.find(scancode) == heldKeys.end()){
                heldKeys.insert(scancode);

                lua_pop(luaState, 1);

                lua_getfield(luaState, -1, "KeyPressed");
                if (lua_isfunction(luaState, -1)) {
                    lua_pushinteger(luaState, scancode);
    
                    if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                        const char* error = lua_tostring(luaState, -1);
                        std::cerr << "Lua error in engine.KeyPressed: " << error << std::endl;
                    }
                }
            }
        }
        else
        {
            lua_getfield(luaState, -1, "KeyReleased");
            if (lua_isfunction(luaState, -1)) {
                lua_pushinteger(luaState, scancode);

                if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                    const char* error = lua_tostring(luaState, -1);
                    std::cerr << "Lua error in engine.KeyReleased: " << error << std::endl;
                }
            }

            heldKeys.erase(scancode);
        }
    }

    lua_pop(luaState, 1);
}
