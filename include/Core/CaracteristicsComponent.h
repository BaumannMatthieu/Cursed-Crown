#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"

struct CaracteristicsComponent : public Component {
    std::string m_name;

    float m_life;
    float m_mana;
    
    float m_defense;
    float m_touch;
    float m_esquive;

    float m_displacement_speed;

    float m_max_weight;
    unsigned int m_gold;

    using Alignment = enum Alignment {
        GOOD,
        CHAOTIC_GOOD,
        NEUTRAL,
        CHAOTIQUE_BAD,
        BAD
    };

    Alignment m_alignment;

    using Caracteristic = enum Caracteristic {
        FORCE,
        DISCRETION,
        SPIRIT,
        CHARISMA,
        CHANCE,
        VOLONTE,
        NUM_CARACTERISTICS  
    };

    std::vector<int> m_caracteristics;
};

using CaracteristicsComponentPtr = std::shared_ptr<CaracteristicsComponent>; 
