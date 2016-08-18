#pragma once

#include <memory>

#include "../Core/Script.h"
#include "../Core/WrapperCoroutine.h"

class ScriptCoroutineBinding : public Script {
    public:
        ScriptCoroutineBinding(ManagerCoroutinePtr& manager_threads, lua_State* lua_state, const std::string& script_file) : Script(lua_state, script_file) {
            LuaRef table_threads = getGlobal(lua_state, "thread_manager");
            if(table_threads.isTable()) {
                if(table_threads["create"].isFunction()) {     
                    manager_threads->m_create = std::make_shared<luabridge::LuaRef>(table_threads["create"]);       
                }
                if(table_threads["wake_up"].isFunction()) {     
                    manager_threads->m_wake_up = std::make_shared<luabridge::LuaRef>(table_threads["wake_up"]);  
                }
                if(table_threads["terminated"].isFunction()) {     
                    manager_threads->m_terminated = std::make_shared<luabridge::LuaRef>(table_threads["terminated"]);  
                }
            } 
        }
};

using ScriptCoroutineBindingPtr = std::shared_ptr<ScriptCoroutineBinding>;
