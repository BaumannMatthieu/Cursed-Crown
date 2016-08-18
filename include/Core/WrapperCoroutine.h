#pragma once

#include <memory>

extern "C" 
{
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "../../include/LuaBridge/LuaBridge.h"

struct ManagerCoroutine {
    std::shared_ptr<luabridge::LuaRef> m_create;
    std::shared_ptr<luabridge::LuaRef> m_wake_up;
    std::shared_ptr<luabridge::LuaRef> m_terminated;
};

using ManagerCoroutinePtr = std::shared_ptr<ManagerCoroutine>;
