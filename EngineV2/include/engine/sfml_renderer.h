#pragma once
#include <stdexcept> //  std::runtime_error
#include <SFML/Graphics.hpp>
#include <string>

namespace engine {

	class SFMLRenderer {

	public:
		typedef std::vector< std::vector<int> > TileMap;
		typedef std::vector<sf::Texture> Textures;

		SFMLRenderer(const std::string& title, int width, int height)
			: m_window(sf::VideoMode(sf::Vector2u(width, height)), title) {
			if (font.openFromFile("assets/EightBitDragon-anqx.ttf")) {
				printf("Font is ready\n");
			}
			// Rajoita FPS 60 frameen sekunnissa
			m_window.setFramerateLimit(60);

		}

		sf::Texture loadTexture(const std::string& textureFilename) {
			sf::Texture texture;
			if (texture.loadFromFile(textureFilename) == false) {
				printf("Failed to load texture: %s\n", textureFilename.c_str());
				throw std::runtime_error("Failed to load texture: " + textureFilename);
			}
			return texture;
		}


		std::vector<sf::Texture> loadTextures(const std::vector<std::string>& fileNames) {
			std::vector<sf::Texture> result;
			for (const auto& fileName : fileNames) {
				auto texture = loadTexture(fileName);
				result.push_back(texture);
			}
			return result;
		}

		bool isWindowOpen() const {
			return m_window.isOpen();
		}

		void beginFrame() {
			// check all the window's events that were triggered since the last iteration of the loop
			while (const std::optional event = m_window.pollEvent())
			{
				// "close requested" event: we close the window
				if (event->is<sf::Event::Closed>()) {
					m_window.close();
				}
			}
		}

		void endFrame() {
			m_window.display(); // Nytt objektit ruudulla
		}

		void clearScreen() {
			m_window.clear(sf::Color(50, 128, 200));
		}

		void setView(const sf::Vector2f& camCenter, const sf::Vector2f& screenSize) {
			sf::View view(camCenter, screenSize);
			m_window.setView(view);
		}

		void renderSprite(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			float rotationInRadians, const sf::Texture& texture);

		void renderMap(const std::vector< std::vector<int> >& map, const std::vector<sf::Texture>& textures);

		void closeWindow() {
			m_window.close();
		}

		void setTileSize(const sf::Vector2f& newTileSize) {
			tileSize = newTileSize;
		}

		void drawGems(const std::string gems) {
			sf::Text text(font);

			text.setCharacterSize(24);
			text.setFillColor(sf::Color::Yellow);
			text.setStyle(sf::Text::Bold);
			text.setPosition(sf::Vector2f(48, 96));

			text.setString("Gems: " + gems);

			m_window.draw(text);
		}

		void drawResult(int result) {
			sf::Text text(font);

			text.setCharacterSize(96);
			text.setFillColor(sf::Color::Yellow);
			text.setStyle(sf::Text::Bold);
			text.setPosition(sf::Vector2f(48*11, 48*11));

			text.setString("Score: " + std::to_string(result));

			m_window.draw(text);
		}

		void drawTime(const std::string time) {
			sf::Text text(font);

			text.setCharacterSize(24);
			text.setFillColor(sf::Color::Yellow);
			text.setStyle(sf::Text::Bold);
			text.setPosition(sf::Vector2f(48, 48));

			text.setString("Time: " + time);

			m_window.draw(text);
		}

	private:
		sf::RenderWindow m_window;
		sf::Vector2f tileSize = sf::Vector2f(48.0f, 48.0f);
		sf::Font font;
	};

} // End - namespace engine