#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"

struct PositionComponent : public Component {
	sf::Vector2f m_position;
};

using PositionComponentPtr = std::shared_ptr<PositionComponent>;