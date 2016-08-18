#pragma once

#include <memory>
#include <vector>

#include "../Core/Entity.h"

#include "../Core/PlayerComponent.h"
#include "../Core/MovementComponent.h"
#include "../Core/InteractionComponent.h"

#include "../Core/Log.h"
#include "../Core/System.h"
#include "../Core/Maths.h"

class Input {
    public:
    void run(std::vector<EntityPtr>& entitys) {
        EntityPtr          entity_player = nullptr;
        PlayerComponentPtr player = nullptr;

        MapComponentPtr map = nullptr;
        for(auto entity : entitys) {
            std::vector<ComponentPtr> components;
            if(System<Input>::findComponent<PlayerComponent>(entity, components)) {
                player = std::static_pointer_cast<PlayerComponent>(components[0]);
                entity_player = entity;
            }

            if(System<Input>::findComponent<MapComponent>(entity, components)) {
                map = std::static_pointer_cast<MapComponent>(components[0]);
            }
        }

        //Mouse clic manager
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)||sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            // Retriving the position from the mouse clic
            sf::Vector2f view(player->m_view.getCenter() - player->m_view.getSize()*0.5f);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f map_pos(map->m_origin_x, map->m_origin_y);
                sf::Vector2f origin(map_pos - view);

                // Get the position in the isometric world
                sf::Vector2f pos = getMousePosition(origin);

                // Add a Movement Component to the player
                MovementComponentPtr movement = std::make_shared<MovementComponent>();
                movement->m_goal = pos;
                entity_player->addComponent<MovementComponent>(movement);
            } else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                // Add a Movement Component to the player
                InteractionComponentPtr interaction = std::make_shared<InteractionComponent>();
                interaction->m_clic = view + sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
                entity_player->addComponent<InteractionComponent>(interaction);
            }
        }

        /*sf::Vector3f direction(0.f, 0.f, 0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            direction += sf::Vector3f(-1.f, 0.f, 0.f);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            direction += sf::Vector3f(1.f, 0.f, 0.f);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            direction += sf::Vector3f(0.f, -1.f, 0.f);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            direction += sf::Vector3f(0.f, 1.f, 0.f);
        }

        if(direction != sf::Vector3f(0.f, 0.f, 0.f)) {
            normalize(direction);

            MovementComponentPtr movement = std::make_shared<MovementComponent>();
            movement->m_direction = isometricProjection(direction);

            entity->addComponent<MovementComponent>(movement);
        }*/
    }

    /*static sf::Vector2f isometricProjection(const sf::Vector3f& vect) {
        return sf::Vector2f(0.71*(vect.x - vect.y),
                            0.41*(vect.x + vect.y) - 0.82*vect.z);
    }*/

    static sf::Vector2f getMousePosition(const sf::Vector2f& origin) {
        sf::Vector2f pos(sf::Mouse::getPosition());

        return iso(pos, origin);
    }    
};
