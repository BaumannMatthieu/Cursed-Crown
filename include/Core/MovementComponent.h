#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"

struct MovementComponent : public Component {
	sf::Vector2f m_goal;

	enum Direction {
		N, 
		NE, 
		E, 
		SE, 
		S, 
		SW, 
		W, 
		NW
	};

	using Direction = enum Direction;

	sf::Vector2f m_d1;
	sf::Vector2f m_d2;

	Direction m_dir1, m_dir2;
};

using MovementComponentPtr = std::shared_ptr<MovementComponent>;