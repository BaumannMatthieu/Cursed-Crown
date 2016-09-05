#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../Core/Entity.h"
#include "../Core/Player.h"

#include "../Core/CaracteristicsComponent.h"
#include "../Core/DamagePerSecondsComponent.h"

#include "../Core/Clock.h"
#include "../Core/Maths.h"
#include "../Core/Log.h"

#include "../Core/System.h"
#include "../Core/MovementSystem.h"

class Damage {
    public:
        void run(std::vector<EntityPtr>& entitys) {
            for(auto entity : entitys) {
                std::vector<ComponentPtr> components;
                
                if(System<Damage>::findComponent<DamagePerSecondsComponent, CaracteristicsComponent>(entity, components)) {
                    DamagePerSecondsComponentPtr damage = std::static_pointer_cast<DamagePerSecondsComponent>(components[0]);
                    CaracteristicsComponentPtr caract = std::static_pointer_cast<CaracteristicsComponent>(components[1]);

                    if(damage->m_start) {
                        caract->m_life = caract->m_life - 25;
                        damage->m_start = false;
                    }

                    sf::Time cooldown = Time::clock.getElapsedTime() - damage->m_time;
                
                    if(cooldown.asMilliseconds() > 1000) {
                        entity->deleteComponent<DamagePerSecondsComponent>();
                    }
                }
            }
        }
};
