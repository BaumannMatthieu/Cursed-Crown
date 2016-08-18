#pragma once

#include <memory>
#include <map>
#include <set>

extern "C" 
{
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "../../include/LuaBridge/LuaBridge.h"
#include "../Core/Log.h"
#include "../Core/CaracteristicsComponent.h"

#include "../Core/Script.h"
#include "../Core/ScriptEntity.h"
#include "../Core/ScriptMap.h"
#include "../Core/ScriptCoroutineBinding.h"
#include "../Core/ScriptRegister.h"

#include "../Core/InteractionSystem.h"
#include "../Core/AttackSystem.h"

#include "../Core/ManagerThreads.h"

class ScriptManager {
    public:
        ScriptManager() : m_lua_state(luaL_newstate()) {
            // Open Lua libs
            luaL_openlibs(m_lua_state);
            // Referencing to Lua script our C++ objects
            getGlobalNamespace(m_lua_state)
                .beginClass<Interaction>("InteractSyst")
                    .addConstructor<void(*)(void)>()
                    .addFunction("say", &Interaction::say)
                    .addFunction("playAnimation", &Interaction::playAnimation)
                    .addFunction("changeAlignment", &Interaction::changeAlignment)
                .endClass()
                .beginClass<sf::Vector2f>("Position")
                    .addConstructor<void(*)(float, float)>()
                .endClass()
                .beginClass<EntityPtr>("Entity")
                    .addConstructor<void(*)()>()
                .endClass()
                .beginClass<Attack>("AttackSyst")
                    .addConstructor<void(*)(void)>()
                    .addFunction("deplace", &Attack::deplace)
                .endClass();
        
            m_main = std::make_shared<ScriptRegister>(m_lua_state, "register_scripts.lua");
        }

        template<typename ScriptType>
        void loadScript(EntityPtr entity, const std::string& name_script) {
            std::map<std::string, std::string> files = m_main->getScriptsFile();

            if(files.find(name_script) == files.end()) {
                LOG_ERROR("The script " + name_script + " has not been found in the script register !");
                return;
            }

            ScriptType(entity, m_lua_state, files[name_script]);
        }

        void loadThreadsScript(const std::string& name_script) {
            std::map<std::string, std::string> files = m_main->getScriptsFile();

            if(files.find(name_script) == files.end()) {
                LOG_ERROR("The script " + name_script + " has not been found in the script register !");
                return;
            }

            ScriptCoroutineBinding(manager_threads, m_lua_state, files[name_script]);
        }

        lua_State* get_lua_state() {
            return m_lua_state;
        }

    private:
        lua_State* m_lua_state;
        ScriptRegisterPtr   m_main;   

};
