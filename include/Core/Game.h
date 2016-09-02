#pragma once

#include <SFML/Graphics.hpp>

#include "../Core/InteractionSystem.h"
#include "../Core/AttackSystem.h"
#include "../Core/DamagePerSecondsSystem.h"
#include "../Core/DeathSystem.h"
#include "../Core/DrawSystem.h"
#include "../Core/MovementSystem.h"
#include "../Core/InputSystem.h"
#include "../Core/ZorderSystem.h"

#include "../Core/EntityManager.h"
#include "../Core/ScriptManager.h"

#include "../Core/Player.h"

class Game
{
	public:
		Game() : m_window(sf::VideoMode::getDesktopMode(), "Cursed Crown", sf::Style::Fullscreen)
		{
			EntityPtr squeleton = std::make_shared<Entity>();
			squeleton->addComponent<CaracteristicsComponent>(std::make_shared<CaracteristicsComponent>());
			//squeleton->addComponent<ScriptComponent>(std::make_shared<ScriptComponent>());
			   
			//m_script_manager.loadScript<ScriptCharacter>(squeleton, "include/Core/squeleton.lua");  
			m_entity_manager.add(squeleton);

			player->addComponent<PlayerComponent>(std::make_shared<PlayerComponent>());
			// Add a position component to the player so that he can move
			PositionComponentPtr position = std::make_shared<PositionComponent>();
			position->m_position = sf::Vector2f(16.f, 16.f);
			player->addComponent<PositionComponent>(position);

			// Add an animation component which store the resources of the animation
			//player->addComponent<CaracteristicsComponent>(std::make_shared<CaracteristicsComponent>());
			//player->addComponent<AnimatedComponent>(std::make_shared<AnimatedComponent>());
			//player->addComponent<ScriptComponent>(std::make_shared<ScriptComponent>());
			//player->addComponent<DrawComponent>(std::make_shared<DrawComponent>());
			m_script_manager.loadScript<ScriptEntity>(player, "player");
			m_entity_manager.add(player);

			// Add an orc which is controlled by a script when he Damages.
			EntityPtr orc = std::make_shared<Entity>();
			// Add a position component to the player so that he can move
			PositionComponentPtr position_orc = std::make_shared<PositionComponent>();
			position_orc->m_position = sf::Vector2f(300.f, 300.f);
			orc->addComponent<PositionComponent>(position_orc);
			m_script_manager.loadScript<ScriptEntity>(orc, "orc heavy");
			m_entity_manager.add(orc);

			EntityPtr map = std::make_shared<Entity>();
			map->addComponent<MapComponent>(std::make_shared<MapComponent>());
			map->addComponent<DrawComponent>(std::make_shared<DrawComponent>());

			m_script_manager.loadScript<ScriptMap>(map, "map");
			m_entity_manager.add(map);

			// Add the objects' entities which are stored in the map component. Are concerned :
			//		- all the NPC : enemys and merchants, storys tellers, etc..
			//		- objects like chest, doors, potions...
			//		- map's traps
			Game::addObjectsEntities(map);

			Game::linkWindowToDrawAndPlayerComponent();
			Game::setViewPlayer();

			// Limit the actual framerate to 60 frame per seconds.
			m_window.setFramerateLimit(60);

			// Set up the coroutines manager by loading the wait.lua script
			m_script_manager.loadThreadsScript("thread");
		}

		void addObjectsEntities(EntityPtr map) {
			std::vector<ComponentPtr> components;
			if(System<Game>::findComponent<MapComponent>(map, components)) {
	            MapComponentPtr map = std::static_pointer_cast<MapComponent>(components[0]); 

	            for(unsigned int i = 0; i < map->m_objectsgroup.size(); ++i) {
	            	for(unsigned int j = 0; j < map->m_objectsgroup[i].m_objects.size(); ++j) {
	            		Object& object = map->m_objectsgroup[i].m_objects[j];

	            		std::cout << object.m_name << std::endl;

	            		EntityPtr entity = std::make_shared<Entity>();
	            		PositionComponentPtr position = std::make_shared<PositionComponent>();
	            		position->m_position = sf::Vector2f(object.m_x, object.m_y);
	            		entity->addComponent<PositionComponent>(position);

	            		m_script_manager.loadScript<ScriptEntity>(entity, object.m_name);
	            		m_entity_manager.add(entity);
	            	}
	            }
	        }
		}

		void linkWindowToDrawAndPlayerComponent() {
			std::vector<EntityPtr>& entitys = m_entity_manager.getEntitys();

			for(auto entity : entitys) {
				std::vector<ComponentPtr> components;

	            if(System<Game>::findComponent<DrawComponent>(entity, components)) {
	                DrawComponentPtr draw = std::static_pointer_cast<DrawComponent>(components[0]); 
	    			draw->m_window = &m_window;
	            }
	        }
		}

		void setViewPlayer() {
			std::vector<EntityPtr>& entitys = m_entity_manager.getEntitys();

			PlayerComponentPtr player = nullptr;
			std::vector<MapComponentPtr> maps;

			for(auto entity : entitys) {
				std::vector<ComponentPtr> components;

				if(System<Game>::findComponent<PlayerComponent>(entity, components)) {
					player = std::static_pointer_cast<PlayerComponent>(components[0]); 
				}

				if(System<Game>::findComponent<MapComponent>(entity, components)) {
					maps.push_back(std::static_pointer_cast<MapComponent>(components[0])); 
				}
			}

			player->m_window = &m_window;
	    			
    		unsigned int view_width = sf::VideoMode::getDesktopMode().width;
    		unsigned int view_height = sf::VideoMode::getDesktopMode().height;

    		// First map loaded is the map where the player starts
    		player->m_view = sf::View(sf::FloatRect(maps[0]->m_origin_x + maps[0]->m_tilewidth/2 - view_width/2,
    							maps[0]->m_origin_y - view_height/2,
    							view_width,
    							view_height));
			m_window.setView(player->m_view);
		}
						
		~Game() 
		{

		}

		void run()
		{
			std::vector<EntityPtr>& entitys = m_entity_manager.getEntitys();

		    while (m_window.isOpen())
		    {
				sf::Event event;
				while (m_window.pollEvent(event))
				{
				    if (event.type == sf::Event::Closed)
						m_window.close();
			
					if(event.type == sf::Event::KeyPressed) {
						if(event.key.code == sf::Keyboard::Escape) 
							m_window.close();	
					}
				}

				(*(manager_threads->m_wake_up))(Time::clock.getElapsedTime().asMilliseconds(), NULL);

				m_input_system.run(entitys);

				m_movement_system.run(entitys);
				m_interaction_system.run(entitys);
				m_damage_system.run(entitys);
				m_zorder_system.run(entitys);
				m_attack_system.run(entitys);

				m_death_system.run(entitys);

				m_window.clear();
				m_draw_system.run(entitys);
				m_window.display();
			}
		}

	private:

		sf::RenderWindow	m_window;
		EntityManager		m_entity_manager;

		System<Interaction>	m_interaction_system;
		System<Draw>		m_draw_system;
		System<Movement>	m_movement_system;
		System<Zorder>		m_zorder_system;
		System<Input>		m_input_system;
		System<Attack>		m_attack_system;
		System<Damage>		m_damage_system;
		System<Death>		m_death_system;

		ScriptManager 		m_script_manager;
};

