#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"

struct DrawComponent : public Component {
	sf::RenderWindow*	m_window;
};

using DrawComponentPtr = std::shared_ptr<DrawComponent>;