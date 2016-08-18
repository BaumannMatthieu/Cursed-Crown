#pragma once

#include <SFML/Graphics.hpp>

using TileID = size_t;

class Map {
	public:
		Map(size_t width, size_t height) : m_tiles(width, std::vector<TileID>(height, 0)) {

		}

		void draw(sf::RenderWindow& window) const {

		}

	private:
		std::vector<std::vector<TileID>>	m_tiles;
		sf::Texture							m_tex;

};