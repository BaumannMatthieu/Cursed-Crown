#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"
#include "../Core/Resource.h"

struct Tileset {
	std::string				m_name;
	unsigned int			m_firstgid;

	unsigned int 			m_tilewidth;
	unsigned int			m_tileheight;

	Resource<sf::Texture>	m_resource;
	unsigned int			m_image_width;
	unsigned int 			m_image_height;
};

using Tileset = struct Tileset;

struct Layer {
	std::string						m_name;
	std::string						m_type;
	
	unsigned int					m_x;
	unsigned int					m_y;

	unsigned int					m_width;
	unsigned int					m_height;

	bool							m_visible;
	float							m_opacity;

	unsigned int					m_tileset_id;

	std::string						m_encoding;
	
	std::vector<unsigned int>		m_data;
	std::vector<sf::VertexArray>	m_vertexarrays;
};

using Layer = struct Layer;

struct Object {
	std::string				m_name;
	std::string				m_type;

	std::string				m_shape;

	unsigned int			m_x;
	unsigned int			m_y;

	unsigned int			m_width;
	unsigned int			m_height;

	unsigned int			m_gid;
	bool					m_visible;
};

using Object = struct Object;

struct ObjectGroup {
	std::string				m_name;

	bool 					m_visible;
	float 					m_opacity;

	std::vector<Object>		m_objects;
};

using ObjectGroup = struct ObjectGroup;

struct MapComponent : public Component {
	std::string					m_version;
	std::string					m_luaversion;
	std::string 				m_orientation;

	unsigned int 				m_width;
	unsigned int				m_height;

	unsigned int				m_tilewidth;
	unsigned int 				m_tileheight;

	unsigned int				m_origin_x;
	unsigned int				m_origin_y;

	std::vector<Tileset>		m_tilesets;
	std::vector<Layer>			m_layers;
	std::vector<ObjectGroup>	m_objectsgroup;
};

using MapComponentPtr = std::shared_ptr<MapComponent>;

using ObjectPtr = std::shared_ptr<Object>;