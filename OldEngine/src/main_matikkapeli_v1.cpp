#include <iostream>
#include <time.h>
#include <stdexcept>
#include <string>
using namespace std;

int main() {
	srand((unsigned int)time(0));
	setlocale(LC_ALL, "");

	int tulos = 0;
	float temp = 0;

	float vastaus_jako = 0;
	int vastaus = 0;
	int pisteet = 0;
	int kierros = 5;

	int luku1 = 0;
	int luku2 = 0;
	int merkki = 0;

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

		default:
			throw runtime_error("Epakelpo operaationumero " + to_string(merkki));
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

	/* Second method to compare
	* if(abs(vastaus - tulos) < 0.01f) 
	* 2 desimals pinpoint (cast every variable as float)
	* 
	*/

	return 0;
}
