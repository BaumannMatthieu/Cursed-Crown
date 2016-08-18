#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"
#include "../Core/Resource.h"

struct SpriteComponent : public Component {
	std::string								m_name;

	Resource<sf::Texture>					m_resource;

	sf::VertexArray*						m_vertex_array;
	
	unsigned int							m_width_sprite;
	unsigned int							m_height_sprite;

	unsigned int							m_width_texture;
	unsigned int							m_height_texture;

	sf::Vector2f							m_origin;
};

using SpriteComponentPtr = std::shared_ptr<SpriteComponent>;