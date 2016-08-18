#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../Core/Entity.h"

#include "../Core/ManagerThreads.h"

#include "../Core/ScriptComponent.h"
#include "../Core/PlayerComponent.h"
#include "../Core/Player.h"
#include "../Core/PlayAnimationComponent.h"
#include "../Core/CaracteristicsComponent.h"
#include "../Core/InteractionComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/PositionComponent.h"
#include "../Core/EnemyComponent.h"
#include "../Core/Clock.h"
#include "../Core/Maths.h"
#include "../Core/Log.h"
#include "../Core/System.h"

class Interaction {
    public:
        Interaction() : m_thread_id("") { 
        }

        void run(std::vector<EntityPtr>& entitys) {
            if(!((*manager_threads->m_terminated)(m_thread_id))) {
                return;
            }

            PositionComponentPtr pos_player;
            InteractionComponentPtr interaction_player = nullptr;

            std::vector<ComponentPtr> components;
            if(System<Interaction>::findComponent<PlayerComponent, InteractionComponent, PositionComponent>(player, components)) {
                interaction_player = std::static_pointer_cast<InteractionComponent>(components[1]);
                pos_player = std::static_pointer_cast<PositionComponent>(components[2]);
            }
            
            if(interaction_player != nullptr) {
                for(auto entity : entitys) {
                    std::vector<ComponentPtr> components;
                    
                    if(System<Interaction>::findComponent<ScriptComponent, SpriteComponent, PositionComponent>(entity, components)) {
                        ScriptComponentPtr script = std::static_pointer_cast<ScriptComponent>(components[0]);
                        SpriteComponentPtr sprite = std::static_pointer_cast<SpriteComponent>(components[1]);
                        PositionComponentPtr pos = std::static_pointer_cast<PositionComponent>(components[2]);

                        if(distance(pos_player->m_position, pos->m_position) <= 64.f) {
                            sf::Vector2f pos_entity_cart = (*sprite->m_vertex_array)[0].position;
                            sf::Vector2f pos_clic = interaction_player->m_clic;

                            int x = pos_clic.x - pos_entity_cart.x;
                            int y = pos_clic.y - pos_entity_cart.y;

                            if(x >= 0 && x < (int)sprite->m_width_sprite &&
                               y >= 0 && y < (int)sprite->m_height_sprite) {

                                sf::Color color_pixel = script->m_image_clic.get().getPixel(x, y);

                                if(color_pixel == sf::Color::Black) {
                                    if(!System<Interaction>::findComponent<EnemyComponent>(entity, components)) {
                                        m_entity = entity;

                                        std::string thread = (*(manager_threads->m_create))(*(script->m_handler), this); 
                                        m_thread_id = thread;
                                    } else {
                                        // play animation updated with entityptr parameter
                                        LOG("ATTACK");
                                    }
                                }
                            }
                        }      
                    }
                }

                player->deleteComponent<InteractionComponent>();
            } else {
                // play animation updated with entityptr parameter
            }
        }

        void say(const std::string& speech) const {
            std::cout << speech << std::endl;
        }


        void playAnimation(const std::string& name_animation) const {
            std::vector<ComponentPtr> components;

            if(System<Interaction>::findComponent<AnimatedComponent>(m_entity, components)) {
                AnimatedComponentPtr animation = std::static_pointer_cast<AnimatedComponent>(components[0]);
                animation->m_animation_key = name_animation;
                if(animation->m_animations[name_animation]->m_started) {
                    return;
                }

                animation->m_animations[name_animation]->m_started = true;
                animation->m_animations[name_animation]->m_time_begin = Time::clock.getElapsedTime().asMilliseconds();
                m_entity->addComponent<PlayAnimationComponent>(std::make_shared<PlayAnimationComponent>());
            }
        }

        void changeAlignment(CaracteristicsComponent::Alignment alignement) const {
            std::vector<ComponentPtr> components;

            if(System<Interaction>::findComponent<CaracteristicsComponent>(m_entity, components)) {
                CaracteristicsComponentPtr caracts = std::static_pointer_cast<CaracteristicsComponent>(components[0]);
                caracts->m_alignment = alignement;

                if(alignement > CaracteristicsComponent::Alignment::NEUTRAL) {
                    m_entity->addComponent<EnemyComponent>(std::make_shared<EnemyComponent>());
                }
            }
        }

    private:
        EntityPtr               m_entity;
        std::string             m_thread_id;
};
