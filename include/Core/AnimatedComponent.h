#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"
#include "../Core/MovementComponent.h"
#include "../Core/Resource.h"

struct Animation {
	std::string						m_name;

	MovementComponent::Direction	m_direction;

	unsigned int					m_frames;
	unsigned int					m_first;

	unsigned int					m_frame_id;
	bool							m_started;

	bool							m_repeat;
	bool							m_reverse;

	std::vector<sf::VertexArray> 	m_vertex_arrays;

	unsigned int 					m_time_begin;
};

using AnimationPtr = std::shared_ptr<Animation>;

struct AnimatedComponent : public Component {
	std::string																			m_name;

	Resource<sf::Texture>																m_resource;
	unsigned int 																		m_animation_time;

	std::map<std::pair<std::string, MovementComponent::Direction>, AnimationPtr>		m_animations;
	std::pair<std::string, MovementComponent::Direction>								m_animation_key;
};

using AnimatedComponentPtr = std::shared_ptr<AnimatedComponent>;