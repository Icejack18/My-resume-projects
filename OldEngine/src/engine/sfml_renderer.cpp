#include <engine/application.h>

namespace engine {


	void SFMLRenderer::renderSprite(
		const sf::Vector2f& position, const sf::Vector2f& size,
		float rotationInRadians, const sf::Texture& texture) {

		// Skaalauskerroin pelimaailman koodrinaatiston ja 
		// kuvaruutukoordonaatiston välillä: 1 pelimaailman unit vastaa 48 piskeli ruudulla
		//static const sf::Vector2f tileSize(48.0f, 48.0f);

		sf::Sprite sprite(texture);
		sprite.setScale(size);
		const auto scaledPosition = sf::Vector2f(position.x * tileSize.x, position.y * tileSize.y);
		sprite.setOrigin(sf::Vector2f(tileSize.x * 0.5f, tileSize.y * 0.5f));
		sprite.setPosition(scaledPosition);

		sprite.setRotation(sf::radians(rotationInRadians));
		m_window.draw(sprite);
	}

	void SFMLRenderer::renderMap(const std::vector< std::vector<int> >& map,
		const std::vector<sf::Texture>& textures) {
		for (size_t y = 0; y < map.size(); ++y) {
			for (size_t x = 0; x < map[y].size(); ++x) {
				int tileId = map[y][x];
				renderSprite(sf::Vector2f(x, y), sf::Vector2f(1, 1), 0.0f, textures[tileId]);
			}
		}
	}

}






