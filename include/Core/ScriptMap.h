#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "../Core/System.h"

#include "../Core/Script.h"
#include "../Core/MapComponent.h"

class ScriptMap : public Script {
    public:
        ScriptMap(EntityPtr entity, lua_State* lua_state, const std::string& script_file) : Script(lua_state, script_file) {
            std::vector<ComponentPtr> components;

            if(System<ScriptMap>::findComponent<MapComponent>(entity, components)) {
                MapComponentPtr map = std::static_pointer_cast<MapComponent>(components[0]); 
                
                LuaRef table_map = getGlobal(lua_state, "map");
                if(table_map.isTable()) {
                    map->m_version = LuaData<std::string>::wrap(table_map, "version");
                    map->m_luaversion = LuaData<std::string>::wrap(table_map, "luaversion");
                    map->m_orientation = LuaData<std::string>::wrap(table_map, "orientation");

                    map->m_width = LuaData<unsigned int>::wrap(table_map, "width");
                    map->m_height = LuaData<unsigned int>::wrap(table_map, "height");
                    map->m_tilewidth = LuaData<unsigned int>::wrap(table_map, "tilewidth");
                    map->m_tileheight = LuaData<unsigned int>::wrap(table_map, "tileheight");

                    map->m_origin_x = map->m_width*map->m_tilewidth/2;
                    map->m_origin_y = map->m_height*map->m_tileheight/2;

                    std::vector<std::shared_ptr<LuaRef>> tilesets = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(table_map, "tilesets");
                    for(unsigned int i = 0; i < tilesets.size(); ++i) {
                        if(tilesets[i]->isTable()) {
                            Tileset tileset;
                            tileset.m_name = LuaData<std::string>::wrap(*tilesets[i], "name");
                            tileset.m_firstgid = LuaData<unsigned int>::wrap(*tilesets[i], "firstgid");
                            tileset.m_tilewidth = LuaData<unsigned int>::wrap(*tilesets[i], "tilewidth");
                            tileset.m_tileheight = LuaData<unsigned int>::wrap(*tilesets[i], "tileheight");

                            tileset.m_image_width = LuaData<unsigned int>::wrap(*tilesets[i], "imagewidth");
                            tileset.m_image_height = LuaData<unsigned int>::wrap(*tilesets[i], "imageheight");

                            std::string texture_path = LuaData<std::string>::wrap(*tilesets[i], "image");
                            if(!tileset.m_resource.load(texture_path)) {
                                LOG_ERROR("Unable to load texture tileset : " + texture_path);
                            }
                            map->m_tilesets.push_back(tileset);
                        }     
                    }

                    std::vector<std::shared_ptr<LuaRef>> layers = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(table_map, "layers");

                    for(unsigned int i = 0; i < layers.size(); ++i) {
                        if(layers[i]->isTable()) {
                            Layer layer;
                            layer.m_name = LuaData<std::string>::wrap(*(layers[i]), "name");
                            layer.m_type = LuaData<std::string>::wrap(*(layers[i]), "type");

                            layer.m_x = LuaData<unsigned int>::wrap(*(layers[i]), "x");
                            layer.m_y = LuaData<unsigned int>::wrap(*(layers[i]), "y");

                            layer.m_width = LuaData<unsigned int>::wrap(*(layers[i]), "width");
                            layer.m_height = LuaData<unsigned int>::wrap(*(layers[i]), "height");

                            layer.m_tileset_id = LuaData<unsigned int>::wrap(*(layers[i]), "tileset_id");

                            layer.m_visible = LuaData<bool>::wrap(*(layers[i]), "visible");
                            layer.m_opacity = LuaData<float>::wrap(*(layers[i]), "opacity");
                            layer.m_encoding = LuaData<std::string>::wrap(*(layers[i]), "encoding");

                            layer.m_data = LuaData<std::vector<unsigned int>>::wrap(*(layers[i]), "data");

                            map->m_layers.push_back(layer);
                        }     
                    }

                    std::vector<std::shared_ptr<LuaRef>> objectsgroup = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(table_map, "objectsgroup");

                    for(unsigned int i = 0; i < objectsgroup.size(); ++i) {
                        if(objectsgroup[i]->isTable()) {
                            ObjectGroup objectgroup;
                            objectgroup.m_name = LuaData<std::string>::wrap(*(objectsgroup[i]), "name");
                            objectgroup.m_visible = LuaData<bool>::wrap(*(objectsgroup[i]), "visible");
                            objectgroup.m_opacity = LuaData<float>::wrap(*(objectsgroup[i]), "opacity");

                            std::vector<std::shared_ptr<LuaRef>> objects = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(*(objectsgroup[i]), "objects");

                            for(unsigned int j = 0; j < objects.size(); ++j) {
                                Object object;
                                object.m_name = LuaData<std::string>::wrap(*(objects[j]), "name");
                                object.m_type = LuaData<std::string>::wrap(*(objects[j]), "type");

                                object.m_shape = LuaData<std::string>::wrap(*(objects[j]), "shape");

                                object.m_x = LuaData<unsigned int>::wrap(*(objects[j]), "x");
                                object.m_y = LuaData<unsigned int>::wrap(*(objects[j]), "y");

                                object.m_width = LuaData<unsigned int>::wrap(*(objects[j]), "width");
                                object.m_height = LuaData<unsigned int>::wrap(*(objects[j]), "height");

                                if(object.m_type != "zone") {
                                    object.m_gid = LuaData<unsigned int>::wrap(*(objects[j]), "gid");
                                }

                                object.m_visible = LuaData<bool>::wrap(*(objects[j]), "visible");

                                objectgroup.m_objects.push_back(object);
                            }

                            map->m_objectsgroup.push_back(objectgroup);
                        }     
                    }

                    ScriptMap::computeVertexArray(map);
                } else {
                    LOG_ERROR("Table map cannot be found in " + m_script_file);
                }
            } else {
                LOG_ERROR("The Entity refers to any map component");
            }
        }

        void computeVertexArray(MapComponentPtr map) const {
            int tilewidth = map->m_tilewidth;
            int tileheight = map->m_tileheight;

            for(unsigned int i = 0; i < map->m_layers.size(); ++i) {
                Layer* layer = &(map->m_layers[i]); 
                
                for(unsigned int j = 0; j < layer->m_data.size(); ++j) {
                    unsigned int gid = layer->m_data[j];
                    
                    unsigned int id_tileset = ScriptMap::getIdTileset(gid, map->m_tilesets);

                    unsigned int firstgid = map->m_tilesets[id_tileset].m_firstgid;
                    unsigned int width_tileset = map->m_tilesets[id_tileset].m_image_width;
                    unsigned int width_tile = map->m_tilesets[id_tileset].m_tilewidth;
                    unsigned int height_tile = map->m_tilesets[id_tileset].m_tileheight;
                    
                    unsigned int num_tiles_width = width_tileset/width_tile;
                    
                    unsigned int tileset_x = 0;
                    unsigned int tileset_y = 0;

                    //if(gid != 0) {
                        tileset_x = (gid - firstgid) % num_tiles_width;
                        tileset_y = (gid - firstgid)/num_tiles_width;    

                        int pos_x = j % layer->m_width;
                        int pos_y = j / layer->m_width;

                        int ox = layer->m_x - map->m_tilewidth/2 + map->m_origin_x;
                        int oy = layer->m_y + map->m_origin_y;

                        sf::VertexArray vertex_array;
			            vertex_array.setPrimitiveType(sf::Quads);
			
			            vertex_array.append(sf::Vertex(sf::Vector2f(tilewidth*(pos_x - pos_y)/2 + ox, tileheight*(pos_x + pos_y)/2 + oy), 
					    sf::Vector2f(tileset_x*width_tile, tileset_y*height_tile)));

			            vertex_array.append(sf::Vertex(sf::Vector2f(tilewidth*(pos_x - pos_y)/2 + width_tile + ox, tileheight*(pos_x + pos_y)/2 + oy), 
								    sf::Vector2f((tileset_x + 1)*width_tile, tileset_y*height_tile)));
			            vertex_array.append(sf::Vertex(sf::Vector2f(tilewidth*(pos_x - pos_y)/2 + width_tile + ox, tileheight*(pos_x + pos_y)/2 + height_tile + oy), 
								    sf::Vector2f((tileset_x + 1)*width_tile, (tileset_y + 1)*height_tile)));
			            vertex_array.append(sf::Vertex(sf::Vector2f(tilewidth*(pos_x - pos_y)/2 + ox, tileheight*(pos_x + pos_y)/2 + height_tile + oy), 
								    sf::Vector2f(tileset_x*width_tile, (tileset_y + 1)*height_tile)));

                        layer->m_vertexarrays.push_back(vertex_array);
                    //}
                }
            }
        }

        unsigned int getIdTileset(unsigned int gid, std::vector<Tileset>& tilesets) const {
            if(gid == 0) {
                return 0;
            }

            for(int i = tilesets.size() - 1; i >= 0; --i) {
                if(gid >= tilesets[i].m_firstgid) {
                    return i;
                }
            }
            LOG_ERROR("No Tilesets corresponds to the gid");
            return tilesets.back().m_firstgid;
        }
};

using ScriptMapPtr = std::shared_ptr<ScriptMap>;
