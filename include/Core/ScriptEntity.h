#pragma once

#include "../Core/System.h"

#include "../Core/Script.h"
#include "../Core/CaracteristicsComponent.h"
#include "../Core/EnemyComponent.h"
#include "../Core/ScriptComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/PositionComponent.h"

class ScriptEntity : public Script {
    public:
        ScriptEntity(EntityPtr entity, lua_State* lua_state, const std::string& script_file) : Script(lua_state, script_file) {
            std::vector<ComponentPtr> components;

            if(System<ScriptEntity>::findComponent<PositionComponent>(entity, components)) {
                PositionComponentPtr position = std::static_pointer_cast<PositionComponent>(components[0]); 
            
                LuaRef table_character = getGlobal(lua_state, "character");
                if(table_character.isTable()) {
                    std::shared_ptr<LuaRef> table_description = LuaData<std::shared_ptr<LuaRef>>::wrap(table_character, "description");
                    if(table_description != nullptr) {
                        CaracteristicsComponentPtr caracts = std::make_shared<CaracteristicsComponent>();

                        caracts->m_name = LuaData<std::string>::wrap(*table_description, "name");

                        caracts->m_life = LuaData<float>::wrap(*table_description, "life");
                        caracts->m_mana = LuaData<float>::wrap(*table_description, "mana");
                        
                        caracts->m_defense = LuaData<float>::wrap(*table_description, "defense");
                        caracts->m_touch = LuaData<float>::wrap(*table_description, "touch");
                        caracts->m_esquive = LuaData<float>::wrap(*table_description, "esquive");
                        caracts->m_displacement_speed = LuaData<float>::wrap(*table_description, "displacement_speed");
                        
                        caracts->m_max_weight = LuaData<float>::wrap(*table_description, "max_weight");
                        caracts->m_gold = LuaData<int>::wrap(*table_description, "gold");

                        caracts->m_alignment = static_cast<CaracteristicsComponent::Alignment>(LuaData<int>::wrap(*table_description, "alignment"));
                        caracts->m_caracteristics = LuaData<std::vector<int>>::wrap(*table_description, "caract");

                        entity->addComponent<CaracteristicsComponent>(caracts);

                    }

                    ScriptComponentPtr script = nullptr; 

                    std::shared_ptr<LuaRef> table_interaction = LuaData<std::shared_ptr<LuaRef>>::wrap(table_character, "interaction");
                    if(table_interaction != nullptr) { 
                        script = std::make_shared<ScriptComponent>();

                        if((*table_interaction)["handler"].isFunction()) {     
                            script->m_handler = std::make_shared<LuaRef>((*table_interaction)["handler"]);  
                        }

                        if(!script->m_image_clic.load(LuaData<std::string>::wrap(*table_interaction, "image_clic"))) {
                            LOG_ERROR("Unable to load clic image detection");
                        }
                    }

                    std::shared_ptr<LuaRef> table_attack = LuaData<std::shared_ptr<LuaRef>>::wrap(table_character, "attack");
                    if(table_attack != nullptr) { 
                        if(script == nullptr) {
                            script = std::make_shared<ScriptComponent>();
                        }

                        if((*table_attack)["attacking"].isFunction()) {     
                            script->m_attack = std::make_shared<LuaRef>((*table_attack)["attacking"]);  
                        }

                        if(!script->m_image_clic.load(LuaData<std::string>::wrap(*table_attack, "image_clic"))) {
                            LOG_ERROR("Unable to load clic image detection");
                        }

                        // Determination of the alignment of the entity
                        if(System<ScriptEntity>::findComponent<CaracteristicsComponent>(entity, components)) {
                            CaracteristicsComponentPtr caracts = std::static_pointer_cast<CaracteristicsComponent>(components[1]); 
                            if(caracts->m_alignment > CaracteristicsComponent::Alignment::NEUTRAL) {
                                entity->addComponent<EnemyComponent>(std::make_shared<EnemyComponent>());
                            }
                        }
                    }

                    if(script != nullptr) {
                        entity->addComponent<ScriptComponent>(script);
                    }

                    std::shared_ptr<LuaRef> table_sprite = LuaData<std::shared_ptr<LuaRef>>::wrap(table_character, "sprite");
                    std::shared_ptr<LuaRef> table_animation = LuaData<std::shared_ptr<LuaRef>>::wrap(table_character, "animation");

                    if(table_sprite != nullptr) {
                        SpriteComponentPtr sprite = std::make_shared<SpriteComponent>();

                        sprite->m_name = LuaData<std::string>::wrap(*table_sprite, "name");
                        if(!sprite->m_resource.load(LuaData<std::string>::wrap(*table_sprite, "image"))) {
                            LOG_ERROR("Unable to load sprite sheet");
                        }
                        sprite->m_origin = sf::Vector2f(LuaData<float>::wrap(*table_sprite, "origin_x"),
                                                        LuaData<float>::wrap(*table_sprite, "origin_y"));

                        sf::VertexArray* vertex_array = new sf::VertexArray;
			            vertex_array->setPrimitiveType(sf::Quads);

                        unsigned int texcoords_x = LuaData<unsigned int>::wrap(*table_sprite, "texcoords_x");
                        unsigned int texcoords_y = LuaData<unsigned int>::wrap(*table_sprite, "texcoords_y");

                        unsigned int width_sprite = LuaData<unsigned int>::wrap(*table_sprite, "width_sprite");
                        unsigned int height_sprite = LuaData<unsigned int>::wrap(*table_sprite, "height_sprite");

                        sf::Vector2f texCoords(texcoords_x, texcoords_y);

                        sf::Vector2f pos(sf::Vector2f(3200.f, 1600.f) + cart(position->m_position));
                        pos -= sprite->m_origin;

            			vertex_array->append(sf::Vertex(pos, texCoords));
            			vertex_array->append(sf::Vertex(pos + sf::Vector2f(width_sprite, 0.f), texCoords + sf::Vector2f(width_sprite, 0.f)));
            			vertex_array->append(sf::Vertex(sf::Vector2f(pos + sf::Vector2f(width_sprite, height_sprite)), texCoords + sf::Vector2f(width_sprite, height_sprite)));
            			vertex_array->append(sf::Vertex(sf::Vector2f(pos + sf::Vector2f(0.f, height_sprite)), texCoords + sf::Vector2f(0.f, height_sprite)));

                        sprite->m_vertex_array = vertex_array;

                        entity->addComponent<DrawComponent>(std::make_shared<DrawComponent>());
                        entity->addComponent<SpriteComponent>(sprite);
                        
                    } else if(table_animation != nullptr) {
                        SpriteComponentPtr sprite = std::make_shared<SpriteComponent>();
                        AnimatedComponentPtr animation = std::make_shared<AnimatedComponent>();

                        animation->m_name = LuaData<std::string>::wrap(*table_animation, "name");
                        if(!animation->m_resource.load(LuaData<std::string>::wrap(*table_animation, "image"))) {
                            LOG_ERROR("Unable to load animation sprite sheet");
                        }
                        animation->m_animation_time = LuaData<unsigned int>::wrap(*table_animation, "animation_time");

                        
                        sprite->m_width_sprite = LuaData<unsigned int>::wrap(*table_animation, "width_sprite");
                        sprite->m_height_sprite = LuaData<unsigned int>::wrap(*table_animation, "height_sprite");

                        sprite->m_width_texture = LuaData<unsigned int>::wrap(*table_animation, "width_texture");
                        sprite->m_height_texture = LuaData<unsigned int>::wrap(*table_animation, "height_texture");

                        sprite->m_origin = sf::Vector2f(LuaData<float>::wrap(*table_animation, "origin_x"),
                                                           LuaData<float>::wrap(*table_animation, "origin_y"));

                        std::vector<std::shared_ptr<LuaRef>> data_animation = LuaData<std::vector<std::shared_ptr<LuaRef>>>::wrap(*table_animation, "data");

                        for(unsigned int i = 0; i < data_animation.size(); ++i) {
                            AnimationPtr data = std::make_shared<Animation>();
                            data->m_name = LuaData<std::string>::wrap(*data_animation[i], "name");

                            data->m_frames = LuaData<unsigned int>::wrap(*data_animation[i], "frames");
                            data->m_first = LuaData<unsigned int>::wrap(*data_animation[i], "first");

                            data->m_started = false;
                            data->m_reverse = false;

                            data->m_repeat = LuaData<bool>::wrap(*table_animation, "repeated");

                            for(unsigned int i = 0; i < data->m_frames; ++i) {
                                unsigned int texcoords_x = ((i + data->m_first) * sprite->m_width_sprite) % sprite->m_width_texture;
                                unsigned int texcoords_y = ((i + data->m_first) / (sprite->m_width_texture/sprite->m_width_sprite))*sprite->m_width_sprite;
                                
                                //std::cout << "texcoord x : " << texcoords_x << " " << texcoords_y << std::endl;

                                sf::Vector2f texCoords(texcoords_x, texcoords_y);

                                sf::Vector2f pos(sf::Vector2f(3200.f, 1600.f) + cart(position->m_position));
                                pos -= sprite->m_origin;

        		                sf::VertexArray vertex_array;
        				        vertex_array.setPrimitiveType(sf::Quads);

                                vertex_array.append(sf::Vertex(pos, texCoords));
                				vertex_array.append(sf::Vertex(pos + sf::Vector2f(sprite->m_width_sprite, 0.f), texCoords + sf::Vector2f(sprite->m_width_sprite, 0.f)));
                				vertex_array.append(sf::Vertex(sf::Vector2f(pos + sf::Vector2f(sprite->m_width_sprite, sprite->m_height_sprite)), texCoords + sf::Vector2f(sprite->m_width_sprite, sprite->m_height_sprite)));
                				vertex_array.append(sf::Vertex(sf::Vector2f(pos + sf::Vector2f(0.f, sprite->m_height_sprite)), texCoords + sf::Vector2f(0.f, sprite->m_height_sprite)));

                                data->m_vertex_arrays.push_back(vertex_array);
                            }

                            animation->m_animations.insert(std::pair<std::string, AnimationPtr>(data->m_name, data));
                        }
                        animation->m_animation_key = "start";

                        sprite->m_vertex_array = &(animation->m_animations[animation->m_animation_key]->m_vertex_arrays[0]);

                        entity->addComponent<DrawComponent>(std::make_shared<DrawComponent>());
                        entity->addComponent<AnimatedComponent>(animation);
                        entity->addComponent<SpriteComponent>(sprite);
                    }
                } else {
                    LOG_ERROR("Table character cannot be found in " + m_script_file);
                }
            } else {
                LOG_ERROR("The Entity refers to any caracteristics component or script or animated");
            }
        }
};

using ScriptEntityPtr = std::shared_ptr<ScriptEntity>;
