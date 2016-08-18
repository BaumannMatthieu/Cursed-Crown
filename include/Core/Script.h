#pragma once

#include <memory>
#include <string>
#include <vector>

extern "C" 
{
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "../../include/LuaBridge/LuaBridge.h"
#include "../Core/Log.h"

using namespace luabridge;

class Script {
    public:
        Script(lua_State* lua_state, const std::string& script_file) : m_script_file(script_file), m_lua_state(lua_state) {
            if(luaL_dofile(lua_state, script_file.c_str()) != 0) {
                LOG_ERROR("unable to open " + script_file + " lua script.");
            } 
        }

    protected:
        template<typename DataType>
        struct LuaData {
            static DataType wrap(const LuaRef& table, const std::string& name) {
                if(table[name.c_str()].isNumber()) {
                    return table[name.c_str()].cast<DataType>();
                }
                LOG_ERROR(name + " of the character not found");    
                return 0;
            }
        };

    protected:
        std::string m_script_file;
        lua_State* m_lua_state;
};


template<>
struct Script::LuaData<std::vector<std::shared_ptr<LuaRef>>> {
    static std::vector<std::shared_ptr<LuaRef>> wrap(const LuaRef& table, const std::string& name) {
        LuaRef array_data = table[name.c_str()];
        if(array_data.isTable()) {

            std::vector<std::shared_ptr<LuaRef>> data(array_data.length());
            for(size_t i = 1; i <= data.size(); ++i) {
                data[i - 1] = std::make_shared<LuaRef>(array_data[i]);
            }
            return data;   
        }
        LOG_ERROR(name + " of the character not found");    
        return std::vector<std::shared_ptr<LuaRef>>();
    }  
};

template<typename DataType>
struct Script::LuaData<std::vector<DataType>> {
    static std::vector<DataType> wrap(const LuaRef& table, const std::string& name) {
        LuaRef array_data = table[name.c_str()];
        if(array_data.isTable()) {

            std::vector<DataType> data(array_data.length());
            for(size_t i = 1; i <= data.size(); ++i) {
                data[i - 1] = array_data[i].cast<DataType>();
            }
            return data;   
        }
        LOG_ERROR(name + " of the character not found");    
        return std::vector<DataType>();
    }  
};



template<>
struct Script::LuaData<std::string> {
    static std::string wrap(const LuaRef& table, const std::string& name) {
        if(table[name.c_str()].isString()) {
            return table[name.c_str()].cast<std::string>();
        }
        LOG_ERROR(name + " of the character not found");    
        return "Null";
    }
};

template<>
struct Script::LuaData<std::shared_ptr<LuaRef>> {
    static std::shared_ptr<LuaRef> wrap(const LuaRef& table, const std::string& name) {
        if(table[name.c_str()].isTable()) {
            return std::make_shared<LuaRef>(table[name.c_str()]);
        }
        LOG_ERROR(name + " of the character not found");    
        return nullptr;
    }
};

template<>
struct Script::LuaData<bool> {
    static bool wrap(const LuaRef& table, const std::string& name) {
        if(table[name.c_str()].isBoolean()) {
            return table[name.c_str()].cast<bool>();
        }
        LOG_ERROR(name + " of the character not found");    
        return false;
    }
};

using ScriptPtr = std::shared_ptr<Script>;
