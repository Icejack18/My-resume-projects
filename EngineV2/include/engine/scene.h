#pragma once
#include <engine/game_object.h>
#include <vector>
#include <memory>

namespace engine {

	class Scene {
	public:

		Scene() {
		}

		const std::vector< std::shared_ptr<GameObject> >& getGameObjects() const {
			return m_gameObjects;
		}

		template<typename GameObjectType>
		auto spawn() {
			std::shared_ptr<GameObjectType> newGameObject = std::make_shared<GameObjectType>();
			m_gameObjects.push_back(newGameObject);
			return newGameObject;
		}

		void destroyGameObject(std::shared_ptr<GameObject> gameObject) {
			auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
			if (it == m_gameObjects.end()) {
				// Peliobjektia ei lytynyt!
				throw std::runtime_error("Game object not found in Scene game objects!");
			}
			// Muutoin, poista elementti vektorista
			m_gameObjects.erase(it);
		}


		void clear() {
			m_gameObjects.clear();
		}

	private:
		std::vector< std::shared_ptr<GameObject> > m_gameObjects;

	};

}