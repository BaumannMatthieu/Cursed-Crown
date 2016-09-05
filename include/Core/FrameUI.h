#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../Core/Resource.h"
#include "../Core/Log.h"
#include "../Core/Player.h"
#include "../Core/System.h"


namespace UI {

class Frame {
	public:
		Frame(unsigned int width, unsigned int height) : m_width(width), m_height(height), m_title_message("Hello World") {
			m_frame.setSize(sf::Vector2f(width, height));
			m_frame.setFillColor(sf::Color(100, 100, 100));


			std::vector<ComponentPtr> components;
			if(System<Frame>::findComponent<PlayerComponent>(player, components)) {
				m_player_component = std::static_pointer_cast<PlayerComponent>(components[0]); 
			}

			//m_frame.setPosition(player_component->m_view.getCenter());
			m_frame.setPosition(m_player_component->m_view.getCenter()
								+ sf::Vector2f(0.f, m_player_component->m_view.getSize().y/2.f - m_height));

			if(!m_font.load("data/font/sansation.ttf")) {
				LOG("Error font frame has not been found");
			}

		    /*m_title.setFont(m_font.get());                                                 
		    m_title.setCharacterSize(18);                                          
		    m_title.setPosition(m_frame.getPosition() + sf::Vector2f(10.f, 10.f));                                     
		    m_title.setColor(sf::Color::White);
		    m_title.setString(m_title_message);*/
		}

		~Frame() {

		}

		void update() {
				m_frame.setPosition(m_player_component->m_view.getCenter()
						+ sf::Vector2f(0.f, m_player_component->m_view.getSize().y/2.f - m_height));
		}

		void draw(sf::RenderWindow& window) {
			window.draw(m_frame);
			//window.draw(m_title);
		}

	private:
		unsigned int		m_width;
		unsigned int		m_height;

		Resource<sf::Image>	m_image;

		std::string			m_title_message;

		sf::RectangleShape	m_frame;

		sf::Text 			m_title;  
		Resource<sf::Font>  m_font;

		PlayerComponentPtr  m_player_component;
};

using FramePtr = std::shared_ptr<Frame>;

}