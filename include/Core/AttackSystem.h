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
#include "../Core/Player.h"
#include "../Core/MovementComponent.h"
#include "../Core/ManagerThreads.h"

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
		        		PositionComponentPtr pos = std::static_pointer_cast<PositionComponent>(components_entity[0]); 
		        		ScriptComponentPtr script = std::static_pointer_cast<ScriptComponent>(components_entity[1]);

		        		if(distance(pos_player->m_position, pos->m_position) <= 8*32.f) {
	                        std::string thread = (*(manager_threads->m_create))(*(script->m_attack), entity, this); 
	     
	                        m_threads[entity] = thread;
		        		}
		            }
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