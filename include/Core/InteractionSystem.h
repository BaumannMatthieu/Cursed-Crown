#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../Core/Entity.h"
#include "../Core/Player.h"

#include "../Core/ManagerThreads.h"

#include "../Core/ScriptComponent.h"
#include "../Core/PlayerComponent.h"
#include "../Core/PlayAnimationComponent.h"
#include "../Core/CaracteristicsComponent.h"
#include "../Core/InteractionComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/MovementComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/PositionComponent.h"
#include "../Core/EnemyComponent.h"
#include "../Core/DeathComponent.h"
#include "../Core/DamagePerSecondsComponent.h"

#include "../Core/Clock.h"
#include "../Core/Maths.h"
#include "../Core/Log.h"

#include "../Core/System.h"
#include "../Core/MovementSystem.h"

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

                                        std::string thread = (*(manager_threads->m_create))(*(script->m_handler), entity, this); 
                                        m_thread_id = thread;
                                    } else {
                                        // play animation updated with entityptr parameter
                                        playAnimation(player, "attack", Movement::getDirection(pos->m_position - pos_player->m_position));
                                        
                                        std::vector<ComponentPtr> damage_component;
                                        if(!System<Interaction>::findComponent<DamagePerSecondsComponent>(entity, damage_component)) {
                                            DamagePerSecondsComponentPtr damage = std::make_shared<DamagePerSecondsComponent>();
                                            damage->m_attacker = player;
                                            damage->m_time = Time::clock.getElapsedTime();
                                            damage->m_start = true;

                                            entity->addComponent<DamagePerSecondsComponent>(damage);
                                        }
                                    }
                                }
                            }
                        }      
                    }
                }

                player->deleteComponent<InteractionComponent>();
            } else {
                std::vector<ComponentPtr> animated_player;
                if(System<Interaction>::findComponent<AnimatedComponent>(player, animated_player)) {
                    AnimatedComponentPtr animated = std::static_pointer_cast<AnimatedComponent>(animated_player[0]);
                    if(animated->m_animation_key.first == "attack") {
                        animated->m_animation_key = std::pair<std::string, MovementComponent::Direction>("stance", animated->m_animation_key.second);
                    }
                }
            }
        }

        void say(const std::string& speech) const {
            std::cout << speech << std::endl;
        }

        static void playAnimation(EntityPtr entity, const std::string& name_animation, unsigned int direction) {
            std::vector<ComponentPtr> components;

            if(System<Interaction>::findComponent<AnimatedComponent>(entity, components)) {
                AnimatedComponentPtr animation = std::static_pointer_cast<AnimatedComponent>(components[0]);

                std::pair<std::string, MovementComponent::Direction> key(name_animation, static_cast<MovementComponent::Direction>(direction));
                animation->m_animation_key = key;
                if(animation->m_animations[key]->m_started) {
                    return;
                }

                animation->m_animations[key]->m_started = true;
                animation->m_animations[key]->m_time_begin = Time::clock.getElapsedTime().asMilliseconds();
                entity->addComponent<PlayAnimationComponent>(std::make_shared<PlayAnimationComponent>());
            }
        }

        void changeAlignment(EntityPtr entity, CaracteristicsComponent::Alignment alignement) const {
            std::vector<ComponentPtr> components;

            if(System<Interaction>::findComponent<CaracteristicsComponent>(entity, components)) {
                CaracteristicsComponentPtr caracts = std::static_pointer_cast<CaracteristicsComponent>(components[0]);
                caracts->m_alignment = alignement;

                if(alignement > CaracteristicsComponent::Alignment::NEUTRAL) {
                    entity->addComponent<EnemyComponent>(std::make_shared<EnemyComponent>());
                }
            }
        }

    private:
        std::string             m_thread_id;
};
