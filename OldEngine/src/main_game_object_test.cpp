/*
#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
using namespace std;

class GameObject {
public:
	virtual ~GameObject(){}
	virtual void update(float deltaTime) = 0;
};

class Player : public GameObject{
public:
	void update(float deltaTime) override {
		cout << "Player is moving..." << endl;
	}
};

class Enemy : public GameObject{
	int hp = 5;
public:
	Enemy() : hp(5) {}
	void update(float deltaTime) override {
		cout << "Enemy is moving...(HP is " << hp << ")" << endl;
	}
	void damage() {
		hp--;
	}
	int getHP() const { return hp; }
};

class Projectile : public GameObject{
public:
	void update(float deltaTime) override {
		cout << "Projectile is flying..." << endl;
	}
};

class PickableItem : public GameObject {
public:
	void update(float deltaTime) override {
		cout << "Pickable item just stays there" << endl;
	}
};

int main() {
	vector<shared_ptr<GameObject>> objects;
	float deltaTime = 1.0f;
	cout << "Program strated\n";

	while (true) {

		if (GetAsyncKeyState('1') & 0x8000) {
			objects.push_back(make_shared<Player>());
			cout << "Player created\n";
		}
		if (GetAsyncKeyState('2') & 0x8000) {
			objects.push_back(make_shared<Enemy>());
			cout << "Enemy created\n";
		}
		if (GetAsyncKeyState('3') & 0x8000) {
			objects.push_back(make_shared<Projectile>());
			cout << "Projectile created\n";
		}
		if (GetAsyncKeyState('4') & 0x8000) {
			objects.push_back(make_shared<PickableItem>());
			cout << "Pickable item created\n";
		}

		if (GetAsyncKeyState('A') & 0x8000) {
			Enemy* enemy = nullptr;
			Projectile* projectile = nullptr;

			for (auto& obj : objects) {
				if (!enemy) enemy = dynamic_cast<Enemy*>(obj.get());
				if (!projectile) projectile = dynamic_cast<Projectile*>(obj.get());
				if (enemy && projectile) break;
			}

			if (enemy && projectile) {
				enemy->damage();
				cout << "Enemy is hit! HP is " << enemy->getHP() << endl;
				if (enemy->getHP() <= 0) {
					cout << "Enemy destroyed!\n";
					objects.erase(remove_if(objects.begin(), objects.end(), [&](shared_ptr<GameObject>& o) {
						return o.get() == enemy; }),
					objects.end());
					}
				}
			}
		if (GetAsyncKeyState('B') & 0x8000) {
			Player* player = nullptr;
			PickableItem* item = nullptr;

			for (auto& obj : objects) {
				if (!player) player = dynamic_cast<Player*>(obj.get());
				if (!item) item = dynamic_cast<PickableItem*>(obj.get());
				if (player && item) break;
			}

			if (player && item) {
				cout << "Player picked up an item -> item destroyed!\n";
				objects.erase(remove_if(objects.begin(), objects.end(), [&](shared_ptr<GameObject>& o) {
					return o.get() == item; }), objects.end());
			}
		}
		for (auto& obj : objects) {
			obj->update(deltaTime);
		}

		this_thread::sleep_for(chrono::seconds(1));
	}

	return 0;
}
*/