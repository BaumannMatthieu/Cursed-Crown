#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"
#include "../Core/Entity.h"


struct DamagePerSecondsComponent : public Component {
	EntityPtr	m_attacker;
	sf::Time	m_time;
	bool 		m_start;
};

using DamagePerSecondsComponentPtr = std::shared_ptr<DamagePerSecondsComponent>;