#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../Core/Entity.h"
#include "../Core/Player.h"

#include "../Core/CaracteristicsComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/DeathComponent.h"

#include "../Core/Clock.h"
#include "../Core/Maths.h"
#include "../Core/Log.h"

#include "../Core/System.h"

#include "../Core/InteractionSystem.h"

class Death {
    public:
        void run(std::vector<EntityPtr>& entitys) {
            for(auto entity : entitys) {
                std::vector<ComponentPtr> components;
                if(!System<Death>::findComponent<DeathComponent>(entity, components)) {
                    if(System<Death>::findComponent<CaracteristicsComponent, AnimatedComponent>(entity, components)) {
                        CaracteristicsComponentPtr caract = std::static_pointer_cast<CaracteristicsComponent>(components[0]);
                        AnimatedComponentPtr animated = std::static_pointer_cast<AnimatedComponent>(components[1]);
                        
                        if(caract->m_life < 0) {
                            Interaction::playAnimation(entity, "death", animated->m_animation_key.second);
                            
                            entity->addComponent<DeathComponent>(std::make_shared<DeathComponent>());
                        }
                    }
                }
            }
        }

        static bool dead(EntityPtr entity) {
            std::vector<ComponentPtr> components;
            return System<Death>::findComponent<DeathComponent>(entity, components);
        }
};