#pragma once

#include <memory>

#include "../Core/Component.h"

struct PlayerComponent : public Component {
	sf::RenderWindow*		m_window;
	sf::View				m_view;
};

using PlayerComponentPtr = std::shared_ptr<PlayerComponent>; 
