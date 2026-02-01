#include <engine/application.h>

class Player : public engine::GameObject {
public:
    Player()
        : GameObject() {
    }

    virtual void update(engine::Scene&, float deltaTime) {
        printf("Player update called!\n");
        auto newPos = getPosition();
#if 1
        if (true == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            newPos.x += speed * deltaTime; // Oikealle kulkeminen
        }
        if (true == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            newPos.x -= speed * deltaTime; // Vasemmalle kulkeminen
        }
        if (true == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            newPos.y -= speed * deltaTime; // Yls kulkeminen
        }
        if (true == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            newPos.y += speed * deltaTime; // Alas kulkeminen
        }
#else
        float dx = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        float dy = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        printf("dx: %f, dy: %f\n", dx, dy);
        newPos.x += dx * deltaTime;
        newPos.y += dy * deltaTime;
#endif
        setPosition(newPos);
    }

    virtual void render(engine::SFMLRenderer& renderer) {
        //renderer.renderSprite();
    }

private:
    float speed = 1.0f;

};



class MyGameApp : public engine::Application {
public:
    MyGameApp()
        : Application("Minun peli", 800, 600) {

        // Spawnaa playeri
        auto player = getScene().spawn<Player>();
        // Mrit ett player kytt tekstruuri id:t numero 0
        player->setTextureId(0);
        //player->setPosition({ 1,0 });
        player->setPosition({ 2,3 });
    }

    virtual void update(float deltaTime) {
        // TODO: Pivit peliobjektit ja peli
        for (auto go : getScene().getGameObjects()) {
            go->update(getScene(), deltaTime);
        }
    }

    virtual void render(engine::SFMLRenderer& renderer) {
        // Aseta kameran paikka ja ruudun koko
        sf::Vector2f camCenter(400 - 24, 300 - 24);
        sf::Vector2f screenSize(800, 600);
        renderer.setView(camCenter, screenSize);

        // Rendaa mappi ensin:
        renderer.renderMap(map, mapTextures);

        // Rendaa sitten kaikki skenen peliobjektit (jotta ne nkyy mapin pll)
        // (see: https://en.wikipedia.org/wiki/Painter%27s_algorithm)

        // Hae peliobjektit skenest ja ky ne kaikki lpi for loopissa:
        auto gameObjects = getScene().getGameObjects();
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            auto gameObject = gameObjects[i];
            // Ota tekstuuri characterTextures vektorista peliobjektin tekstuuri, tekstuuri id:n mukaan:
            auto texture = characterTextures[gameObject->getTextureId()];
            // Rendaa peliobjekti rendererin avulla peliobjektin jsenmuuttujien mrmien arvojen mukaan.
            renderer.renderSprite(gameObject->getPosition(), gameObject->getSize(), gameObject->getRotation(), texture);
        }
    }


    // Tai nin void setMap(const auto& newMap)
    void setMap(const std::vector< std::vector<int> >& newMap) {
        map = newMap;
    }

    void setMapTextures(const std::vector<std::string>& fileNames) {
        mapTextures = getRenderer().loadTextures(fileNames);
    }

    void setCharacterTextures(const std::vector<std::string>& fileNames) {
        characterTextures = getRenderer().loadTextures(fileNames);
    }


private:
    std::vector< std::vector<int> > map;
    std::vector< sf::Texture > mapTextures;
    std::vector< sf::Texture > characterTextures;

};


int main() {
    std::vector< std::vector<int> > map = {
        // x==0, x==1, x==2, x==3
        { 1,        0,    0,   0, 0, 0 }, // Rivi, jossa y==0
        { 1,        0,    0,   0, 0, 1 }, // Rivi, jossa y==1
        { 1,        0,    0,   0, 0, 1 }, // Rivi, jossa y==2
        { 1,        0,    0,   2, 2, 1 }, // Rivi, jossa y==3
        { 1,        0,    0,   2, 2, 1 }, // Rivi, jossa y==4
        { 1,        1,    1,   1, 0, 1 }, // Rivi, jossa y==5, jossa seini
        { 1,        0,    0,   0, 0, 1 }, // Rivi, jossa y==5, jossa seini
    };

    //app.loadMap("assets/test-map.tmx");

    // Tee MyGameApp instanssi
    MyGameApp app;

    app.setMap(map);
    app.setMapTextures({
        "assets/ground.png",
        "assets/wall.png",
        "assets/grass.png",
        });
    app.setCharacterTextures({
        "assets/player.png",
        });

    //app.run();

    return 0;
}
