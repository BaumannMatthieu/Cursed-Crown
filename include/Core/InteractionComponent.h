#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"

struct InteractionComponent : public Component {
	sf::Vector2f	m_clic;
};

using InteractionComponentPtr = std::shared_ptr<InteractionComponent>;