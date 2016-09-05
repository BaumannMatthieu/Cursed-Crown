#pragma once

#include <memory>
#include <map>

#include "../Core/Entity.h"

#include "../Core/ScriptComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/PlayerComponent.h"
#include "../Core/EnemyComponent.h"
#include "../Core/CaracteristicsComponent.h"
#include "../Core/DamagePerSecondsComponent.h"
#include "../Core/Player.h"
#include "../Core/MovementComponent.h"
#include "../Core/ManagerThreads.h"

#include "../Core/InteractionSystem.h"

#include "../Core/Log.h"
#include "../Core/System.h"
#include "../Core/Clock.h"
#include "../Core/Maths.h"

class Attack {
    public:
	    void run(std::vector<EntityPtr>& entitys) {
	    	PositionComponentPtr pos_player = nullptr;
	    	
    		std::vector<ComponentPtr> components;
	    	if(System<Attack>::findComponent<PlayerComponent, PositionComponent>(player, components)) {
	    		pos_player = std::static_pointer_cast<PositionComponent>(components[1]); 
	    	}

	        for(auto entity : entitys) {
	        	if((*manager_threads->m_terminated)(m_threads[entity])) {
		        	std::vector<ComponentPtr> components_entity;
		        	
		        	if(System<Attack>::findComponent<PositionComponent, ScriptComponent, EnemyComponent, CaracteristicsComponent>(entity, components_entity)) {
		        		if(!System<Attack>::findComponent<DeathComponent>(player, components_entity)) {
			        		PositionComponentPtr pos = std::static_pointer_cast<PositionComponent>(components_entity[0]); 
			        		ScriptComponentPtr script = std::static_pointer_cast<ScriptComponent>(components_entity[1]);

			        		if(distance(pos_player->m_position, pos->m_position) <= 8*32.f) {
		                        std::string thread = (*(manager_threads->m_create))(*(script->m_attack), entity, player, this); 
		     
		                        m_threads[entity] = thread;
			        		}
		        		} else {
			            	std::vector<ComponentPtr> component_anim_entity;
							if(System<Attack>::findComponent<AnimatedComponent>(entity, component_anim_entity)) {
					    		AnimatedComponentPtr anim = std::static_pointer_cast<AnimatedComponent>(component_anim_entity[0]); 
					    		Interaction::playAnimation(entity, "stance", anim->m_animation_key.second);
					    	}
			            }
		            }
		        }
	        }
	    }
		
		void attack(EntityPtr entity, EntityPtr focus) {
			PositionComponentPtr pos_focus = nullptr;
			PositionComponentPtr pos_entity = nullptr;
			std::vector<ComponentPtr> component_pos_focus;
			std::vector<ComponentPtr> component_pos_entity;
			if(System<Attack>::findComponent<PositionComponent>(entity, component_pos_entity)) {
	    		pos_entity = std::static_pointer_cast<PositionComponent>(component_pos_entity[0]); 
	    	}
	    	if(System<Attack>::findComponent<PositionComponent>(focus, component_pos_focus)) {
	    		pos_focus = std::static_pointer_cast<PositionComponent>(component_pos_focus[0]); 
	    	}

		    // Add a Movement Component to the player
		    entity->deleteComponent<MovementComponent>();
            MovementComponentPtr movement = std::make_shared<MovementComponent>();
            movement->m_goal = pos_focus->m_position;
            entity->addComponent<MovementComponent>(movement);
                                        
            std::vector<ComponentPtr> damage_component;
            if(distance(pos_focus->m_position, pos_entity->m_position) <= 32.f) {
            	entity->deleteComponent<MovementComponent>();

            	Interaction::playAnimation(entity, "attack", Movement::getDirection(pos_focus->m_position - pos_entity->m_position));

	            if(!System<Interaction>::findComponent<DamagePerSecondsComponent>(focus, damage_component)) {
	                DamagePerSecondsComponentPtr damage = std::make_shared<DamagePerSecondsComponent>();
	                damage->m_attacker = entity;
	                damage->m_time = Time::clock.getElapsedTime();
	                damage->m_start = true;

	                focus->addComponent<DamagePerSecondsComponent>(damage);
	            }
        	}
	    }

	    void deplace(EntityPtr entity, const sf::Vector2f& goal) {
	    	MovementComponentPtr movement = std::make_shared<MovementComponent>();
            movement->m_goal = goal;
            entity->addComponent<MovementComponent>(movement);
	    }

	private:
		std::map<EntityPtr, std::string>	m_threads;
};