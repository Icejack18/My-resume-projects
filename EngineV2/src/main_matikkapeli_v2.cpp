#include <iostream>
#include <time.h>
using namespace std;

class Game
{
public:
	Game(int k);
	~Game();
	void start();

private:
	int kierros;
};

Game::Game(int k)
{
	kierros = k;
}

Game::~Game()
{
}

void Game::start()
{
	srand(time(0));
	setlocale(LC_ALL, "");

	int tulos = 0;
	float temp = 0;

	int luku1 = 0;
	int luku2 = 0;
	int merkki = 0;

	float vastaus_jako = 0;
	int vastaus = 0;
	int pisteet = 0;

	for (int i = 0; i < kierros; i++) {

		luku1 = rand() % 10 + 1;
		luku2 = rand() % 10 + 1;
		merkki = rand() % 4 + 1;

		switch (merkki)
		{
		case 1:
			tulos = luku1 + luku2;
			cout << luku1 << " + " << luku2 << " = ";
			break;

		case 2:
			tulos = luku1 - luku2;
			cout << luku1 << " - " << luku2 << " = ";
			break;

		case 3:
			tulos = luku1 * luku2;
			cout << luku1 << " * " << luku2 << " = ";
			break;

		case 4:
			temp = (float)luku1 / (float)luku2;
			tulos = temp * 10;
			cout << "Vastaus on 1 merkki pilkun jälkeen. Älä pyörista (1.666 -> 1.6 oikein vastaus)" << endl;
			cout << luku1 << " / " << luku2 << " = ";
			break;
		}
		if (merkki != 4)
			cin >> vastaus;
		else {
			cin >> vastaus_jako;
			vastaus = vastaus_jako * 10;
		}

		if (vastaus == tulos) {
			cout << "Oikein!" << endl;
			pisteet++;
		}
		else cout << "Väärin :(" << endl;
	}

	cout << "Sinun pisteet: " << pisteet << "/" << kierros << endl;
	cout << "Game Over!\n";
}

int main() {
	Game game(5);
	game.start();

	return 0;
}