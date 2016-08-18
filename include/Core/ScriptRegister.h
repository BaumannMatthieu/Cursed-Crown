#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "../Core/System.h"

#include "../Core/Script.h"

class ScriptRegister : public Script {
    public:
        ScriptRegister(lua_State* lua_state, const std::string& script_file) : Script(lua_state, script_file) {
        	LuaRef table_register = getGlobal(lua_state, "scripts");
            if(table_register.isTable()) {
        		std::vector<std::shared_ptr<LuaRef>> scripts = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(table_register, "data");
            	
            	for(unsigned int i = 0; i < scripts.size(); ++i) {
                    if(scripts[i]->isTable()) {
                        std::string name = LuaData<std::string>::wrap(*scripts[i], "name");
                        std::string file = LuaData<std::string>::wrap(*scripts[i], "file");

                        std::cout << name << " " << file << std::endl;
                 		
                        m_scripts_file.insert(std::make_pair(name, file));
                    }     
                }
            }	   
        }

        const std::map<std::string, std::string>& getScriptsFile() const {
        	return m_scripts_file;
        }

    private:
    	std::map<std::string, std::string>		m_scripts_file;
};

using ScriptRegisterPtr = std::shared_ptr<ScriptRegister>;