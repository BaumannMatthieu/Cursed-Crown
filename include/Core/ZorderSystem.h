#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "../Core/Entity.h"

#include "../Core/MapComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/DrawComponent.h"

#include "../Core/Log.h"
#include "../Core/System.h"
#include "../Core/Clock.h"

class Zorder {
    private:
        struct Comparator {
            bool operator()(EntityPtr first, EntityPtr second) {
                std::vector<ComponentPtr> components;
                if(System<Draw>::findComponent<PositionComponent, SpriteComponent>(first, components)) {
                    PositionComponentPtr pos_first = std::static_pointer_cast<PositionComponent>(components[0]);

                    if(System<Draw>::findComponent<PositionComponent, SpriteComponent>(second, components)) {
                        PositionComponentPtr pos_second = std::static_pointer_cast<PositionComponent>(components[2]);

                        return (pos_first->m_position.y < pos_second->m_position.y);
                    }
                    return false;
                } 
                return true;
            }
        } Comparator;

    public:
        void run(std::vector<EntityPtr>& entitys) {
            std::sort(entitys.begin(), entitys.end(), Comparator);
        }
};
