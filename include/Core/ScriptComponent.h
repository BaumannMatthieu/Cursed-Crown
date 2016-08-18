#pragma once

#include <memory>

extern "C" 
{
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "../../include/LuaBridge/LuaBridge.h"

#include "../Core/Component.h"
#include "../Core/Resource.h"

struct ScriptComponent : public Component {
	Resource<sf::Image>				   m_image_clic;
	
    std::shared_ptr<luabridge::LuaRef> m_handler;
    std::shared_ptr<luabridge::LuaRef> m_attack;
};

using ScriptComponentPtr = std::shared_ptr<ScriptComponent>; 
