#pragma once

namespace engine {

	// Forward deklaraatiot SFMLRenderer luokalle ja Scene luokalle, jotta niit voidaan kytt metodien deklaraatiossa.
	class SFMLRenderer;
	class Scene;

	class GameObject {
	public:
		GameObject() {
			// Alkuarvot:
			m_position.x = 0;
			m_position.y = 0;
			m_size.x = 1;
			m_size.y = 1;
			m_rotation = 0;
		}

		virtual ~GameObject() {}

		virtual void update(Scene&, float deltaTime) = 0;
		virtual void render(SFMLRenderer&) = 0;

		void setPosition(const sf::Vector2f& newPosition) {
			m_position = newPosition;
		}

		sf::Vector2f getPosition() const {
			return m_position;
		}

		sf::Vector2f getSize() const {
			return m_size;
		}

		float getRotation() const {
			return m_rotation;
		}

		void setTextureId(int textureId) {
			m_textureId = textureId;
		}

		int getTextureId() const {
			return m_textureId;
		}

		void move(const sf::Vector2f& delta) {
			auto pos = getPosition();
			setPosition(sf::Vector2f(pos.x + delta.x, pos.y + delta.y));
		}

	private:
		sf::Vector2f		m_position;
		sf::Vector2f		m_size;
		float				m_rotation;
		int					m_textureId;
	};
}
