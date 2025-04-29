
#include "input.h"

void process_input(SDL_Keysym &keysym, bool down, lua_State &luaState) {
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
            lua_getfield(luaState, -1, "KeyPressed");
            if (lua_isfunction(luaState, -1)) {
                lua_pushinteger(luaState, keysym.scancode);

                if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
                    const char* error = lua_tostring(luaState, -1);
                    std::cerr << "Lua error in engine.KeyPressed: " << error << std::endl;
                }
            }
        }
    }
}
