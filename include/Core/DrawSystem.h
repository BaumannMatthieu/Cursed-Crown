#pragma once

#include <memory>
#include <vector>

#include "../Core/Entity.h"

#include "../Core/MapComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/PlayAnimationComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/DrawComponent.h"
#include "../Core/PositionComponent.h"

#include "../Core/InputSystem.h"

#include "../Core/Log.h"
#include "../Core/System.h"
#include "../Core/Clock.h"
#include "../Core/Maths.h"

class Draw {
    public:
        void run(std::vector<EntityPtr>& entitys) {
            MapComponentPtr map = nullptr;
            DrawComponentPtr draw = nullptr;

            for(auto entity : entitys) {
                std::vector<ComponentPtr> components;
                
                if(System<Draw>::findComponent<MapComponent, DrawComponent>(entity, components)) {
                    map = std::static_pointer_cast<MapComponent>(components[0]);
                    draw = std::static_pointer_cast<DrawComponent>(components[1]);

                    for(unsigned int i = 0; i < map->m_layers[0].m_vertexarrays.size(); ++i) {
                        unsigned int tileset_id = map->m_layers[0].m_tileset_id;
                        draw->m_window->draw(map->m_layers[0].m_vertexarrays[i], &(map->m_tilesets[tileset_id].m_resource.get()));
                    }

                    break;
                } 
            }

            unsigned int tile_id = 0;
            for(auto entity : entitys) {
                std::vector<ComponentPtr> components;
                
                if(System<Draw>::findComponent<SpriteComponent, PositionComponent, DrawComponent>(entity, components)) {
                    SpriteComponentPtr sprite = std::static_pointer_cast<SpriteComponent>(components[0]);
                    PositionComponentPtr position = std::static_pointer_cast<PositionComponent>(components[1]);
                    DrawComponentPtr draw_entity = std::static_pointer_cast<DrawComponent>(components[2]);

                    int sprite_tile_x = position->m_position.x/32;
                    int sprite_tile_y = position->m_position.y/32;
                    
                    for(unsigned int i = tile_id; i < map->m_layers[0].m_vertexarrays.size(); ++i) { 
                        int tile_x = i % map->m_width;
                        int tile_y = i / map->m_height;

                        if(tile_y < sprite_tile_y - 1 || tile_x < sprite_tile_x - 1) {
                            for(unsigned int id_layer = 1; id_layer < map->m_layers.size(); ++id_layer) {
                                unsigned int tileset_id = map->m_layers[id_layer].m_tileset_id;
                                int upper_tile = i - (id_layer -  1)*(map->m_width + 1);
                                if(upper_tile >= 0)
                                    draw->m_window->draw(map->m_layers[id_layer].m_vertexarrays[upper_tile], &(map->m_tilesets[tileset_id].m_resource.get()));
                            }
                        } else {
                            tile_id = i;
                            break;
                        }
                    }

                    if(System<Draw>::findComponent<AnimatedComponent>(entity, components)) {
                        AnimatedComponentPtr animation = std::static_pointer_cast<AnimatedComponent>(components[3]);

                        AnimationPtr current_anim = animation->m_animations[animation->m_animation_key];

                        unsigned int animation_time = animation->m_animation_time;
                        unsigned int frames = current_anim->m_frames;

                        unsigned int time_per_frame = animation_time/frames;
                        sf::Time ticks = Time::clock.getElapsedTime();

                        if(current_anim->m_repeat) {
                            current_anim->m_frame_id = (ticks.asMilliseconds()/time_per_frame) % frames;
                        } else {
                            //if(!current_anim->m_started) {
                            //    current_anim->m_frame_id = 0;
                            //}

                            if(System<Draw>::findComponent<PlayAnimationComponent>(entity, components)) {
                                unsigned int id_frame = ((ticks.asMilliseconds() - current_anim->m_time_begin)/time_per_frame) % frames;
                                if(current_anim->m_reverse) {
                                    current_anim->m_frame_id = frames - 1 - id_frame;

                                    if(current_anim->m_frame_id == 0) {
                                        current_anim->m_started = false;
                                        entity->deleteComponent<PlayAnimationComponent>();
                                    }
                                } else {
                                    current_anim->m_frame_id = id_frame;

                                    if(current_anim->m_frame_id == frames - 1) {
                                        current_anim->m_started = false;
                                        entity->deleteComponent<PlayAnimationComponent>();
                                    }
                                }
                            }
                        }
                        draw_entity->m_window->draw(current_anim->m_vertex_arrays[current_anim->m_frame_id], &(animation->m_resource.get()));
                    } else {
                        draw_entity->m_window->draw(*(sprite->m_vertex_array), &(sprite->m_resource.get()));
                    } 
                }
            }

            // Draw the rest of the map
            for(unsigned int i = tile_id; i < map->m_layers[0].m_vertexarrays.size(); ++i) {
                for(unsigned int id_layer = 1; id_layer < map->m_layers.size(); ++id_layer) {
                    unsigned int tileset_id = map->m_layers[id_layer].m_tileset_id;
                    draw->m_window->draw(map->m_layers[id_layer].m_vertexarrays[i], &(map->m_tilesets[tileset_id].m_resource.get()));
                }
            }
            //Draw::draw_interactable(entitys);
        }

        /*static void draw_interactable(EntityPtr entity) {
            for(auto entity : entitys) {
                std::vector<ComponentPtr> components;

                if(System<Draw>::findComponent<DrawComponent, AnimatedComponent>(entity, components)) {
                    DrawComponentPtr draw = std::static_pointer_cast<DrawComponent>(components[0]); 
                    AnimatedComponentPtr animation = std::static_pointer_cast<AnimatedComponent>(components[1]);

                    AnimationPtr current_anim = animation->m_animations[animation->m_animation_key];

                    unsigned int animation_time = animation->m_animation_time;
                    unsigned int frames = current_anim->m_frames;

                    unsigned int time_per_frame = animation_time/frames;
                    sf::Time ticks = Time::clock.getElapsedTime();

                    draw->m_window->draw(current_anim->m_vertex_arrays[(ticks.asMilliseconds()/time_per_frame) % frames], &(animation->m_resource.get()));
                } else if(System<Draw>::findComponent<DrawComponent, SpriteComponent>(entity, components)) {
                    DrawComponentPtr draw = std::static_pointer_cast<DrawComponent>(components[0]); 
                    SpriteComponentPtr sprite = std::static_pointer_cast<SpriteComponent>(components[1]);

                    draw->m_window->draw(sprite->m_vertex_array, &(sprite->m_resource.get()));
                }
            }
        }*/
        /*
        static void print_vertex_array(const sf::VertexArray& vertex_array) {
            for(unsigned int i = 0; i < vertex_array.getVertexCount(); ++i) {
                std::cout << "pos : " << vertex_array[i].position.x << " " << vertex_array[i].position.y << std::endl;
                std::cout << "texCoords : " << vertex_array[i].texCoords.x << " " << vertex_array[i].texCoords.y << std::endl;
            }
        }
        */
};
