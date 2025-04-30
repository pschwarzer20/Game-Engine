
#pragma once


#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}


void process_input(SDL_Keysym &keysym, bool down, lua_State *luaState);
