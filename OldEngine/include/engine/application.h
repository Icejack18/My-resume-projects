#pragma once
#include <engine/sfml_renderer.h>
#include <engine/scene.h>

class Player;

namespace engine {

	/// <summary>
	/// Application luokka, jota kytetn pelisovelluksien kantaluokana.
	/// </summary>
	class Application {
	public:

		/// <summary>
		/// Application luokan konstruktori. Sislt sovelluksen rendeijn ja skene graafin.
		/// </summary>
		/// <param name="windowTitle">Ikkunan otsikkoteksti</param>
		/// <param name="windowSizeX">Ikkunan x koko</param>
		/// <param name="windowSizeY">Ikkunan y koko</param>
		Application(const std::string& windowTitle, int windowSizeX, int windowSizeY)
			: m_renderer(windowTitle, windowSizeX, windowSizeY) {
		}

		/// <summary>
		/// Getteri renderijn hakemiseksi.
		/// </summary>
		/// <returns>Palauttaa viittauksen SFMLRenderer-olioon</returns>
		SFMLRenderer& getRenderer() {
			return m_renderer;
		}

		// = 0 metodin lopussa tarkoittaa sit, ett metodia ei ole toteutuettu tss luokassa.
		// update ja render metodit pit totetuttaa aliluokassa/johdetussa luokassa eli ovat Ns. puhtaasti virtuaalisia metodeja.

		virtual void update(float deltaTime) = 0;
		virtual void render(SFMLRenderer& renderer) = 0;


		void run(int &score) {
			engine::SFMLRenderer& renderer = getRenderer();

			// deltaTime = 1/FPS
			// <=> FPS = 1/deltaTime
			float deltaTime = 1.0f / 60.0f;

			sf::Clock clock;
			sf::Clock play_timer;
			
			// Sovelluksen plooppi:
			play_timer.restart();
			std::string stime;
			while (renderer.isWindowOpen()) {
				sf::Time time = clock.restart();
				deltaTime = time.asSeconds();

				ftime = play_timer.getElapsedTime().asSeconds();
				stime = std::to_string(ftime);
				stime.erase(stime.find_first_of('.') + 3, std::string::npos);

				// Update:
				update(deltaTime);

				renderer.beginFrame();
				renderer.clearScreen();

				// Render scene:
				render(renderer);
				renderer.drawGems(std::to_string(score));
				renderer.drawTime(stime);
				stime.clear();
				renderer.endFrame();
			}

			//printf("Your time is %.2f\nYour score is %d\n", ftime, score);
			//printf("Calculated points: %d\n", result);
		}

		Scene& getScene() {
			return m_scene;
		}

		float& timeRef() { return ftime; }
		void setGems(int gems) { this->gems = gems; }
		int getGems() { return gems; }

	private:
		SFMLRenderer m_renderer;
		Scene m_scene;
		float ftime = 0.0f;
		int gems = 0;

	};


} // End - namespace engine