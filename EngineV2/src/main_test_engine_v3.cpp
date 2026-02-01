#include <engine/application.h>
//#include <math.h>
#include <cmath>

namespace  collision_check {

    /// <summary>
    /// Tarkastaa mapin ja pisteen välisen törmäyksen. 
    /// Olettaa, että tileId: <= 0 tilet ovat käveltäviä tilejä.
    /// </summary>
    /// <param name="map">Mappi</param>
    /// <param name="position">Pisteen paikka</param>
    /// <returns>Palauttaa true, jos annettu positio on seinän päällä (tileId > 0)</returns>
    bool isMapCollision(const std::vector< std::vector<int> >& map, const sf::Vector2f& position) {
        int x = int(position.x);
        int y = int(position.y);
        if (x < 0 || y < 0) {
            return true; // Törmäys "negatiivisella puolella"
        }
        if (y >= map.size()) {
            return true; // Törmäys mapin alareunaan
        }

        if (x >= map[y].size()) {
            return true; // Törmäys mapin oikeaan reunaan
        }

        int tileId = map[y][x];
        return tileId > 0;
    }

    /// <summary>
    /// Tarkistaa koollisen peliobjektin törmäyksen mappiin.
    /// </summary>
    /// <param name="map">Mappi</param>
    /// <param name="position">peliobjektin paikka</param>
    /// <param name="size">Peliobjektin koko</param>
    /// <returns>Palauttaa true, jos peliobjekti törmäsi seinään</returns>
    bool isMapCollision(const std::vector< std::vector<int> >& map,
        const sf::Vector2f& position, const sf::Vector2f& size) {
        sf::Vector2f topLeft = position;
        sf::Vector2f topRight = sf::Vector2f(position.x + size.x, position.y);
        sf::Vector2f bottomLeft = sf::Vector2f(position.x, position.y + size.y);
        sf::Vector2f bottomRight = sf::Vector2f(position.x + size.x, position.y + size.y);

        // Palauta true, jos joku piste törmää mappiin.
        return isMapCollision(map, topLeft)
            || isMapCollision(map, topRight)
            || isMapCollision(map, bottomLeft)
            || isMapCollision(map, bottomRight);
    }

    bool isSphereSphereCollision(const sf::Vector2f& pos1, float r1, const sf::Vector2f& pos2, float r2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        float d = std::sqrt(dx * dx + dy * dy); // Herra Pyhagoras!
        // Jos pallojen keskipisteiden etäisyys toisistaan 
        // on pienempi, kuin säteiden summa, niin siinä
        // tapauksessa on törmäys!
        return d < (r1 + r2);
    }

    bool isAABBAABBCollision(const sf::Vector2f& pos1, const sf::Vector2f& halfSize1,
        const sf::Vector2f& pos2, const sf::Vector2f& halfSize2) {
        float dx = std::abs(pos1.x - pos2.x);
        float dy = std::abs(pos1.y - pos2.y);
        float hsx = (halfSize1.x + halfSize2.x);
        float hsy = (halfSize1.y + halfSize2.y);
        return dx < hsx && dy < hsy;
    }
}



class Player : public engine::GameObject {
public:
    typedef std::vector< std::vector<int> > Map;

    Player()
        : GameObject() {
    }

    virtual void update(engine::Scene&, float deltaTime) {
        //printf("Player update called!\n");
        auto newPos = getPosition();

        float dx = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        float dy = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        //printf("dx: %f, dy: %f\n", dx, dy);

        // Liikuta peliobjektia:
        newPos.x += dx * deltaTime;
        newPos.y += dy * deltaTime;

        // Tarkista törmäys mappiin
        sf::Vector2f size(0.95f, 0.95f);
        if (collision_check::isMapCollision(*pMap, newPos, size)) {
            // Törmäys: Palauta edellinen paikka peliobjektin paikaksi, koska se ei törmää mappiin.
            newPos = getPosition();
        }
        else {
            // Ei törmäystä:
        }
        setPosition(newPos);
    }

    virtual void render(engine::SFMLRenderer& renderer) {
        //renderer.renderSprite();
    }

    // Asettaa mapin playerille
    void setMap(Map* map) {
        pMap = map;
    }

    void setScore(int newScore) {
        score = newScore;
        printf("Player new score: %d\n", score);
    }

    int getScore() const {
        return score;
    }

private:
    float speed = 1.0f;
    Map* pMap = nullptr; // Oletuksena mappi on null
    int score = 0;

};

class Item : public engine::GameObject {
public:
    virtual void update(engine::Scene&, float deltaTime) {
    }

    virtual void render(engine::SFMLRenderer&) {
    }

};


class Goal : public engine::GameObject {
public:
    virtual void update(engine::Scene&, float deltaTime) {
    }

    virtual void render(engine::SFMLRenderer&) {
    }

};

class MyGameApp : public engine::Application {
public:
    MyGameApp()
        : Application("Minun peli", 800, 600) {

        // Spawnaa playeri
        auto player = getScene().spawn<Player>();
        // Määritä että player käyttää tekstruuri id:tä numero 0
        player->setTextureId(0);
        //player->setPosition({ 1,0 });
        player->setPosition({ 2,3 });
        // Aseta playerille mapin pointteri, jotta tt toimii
        player->setMap(&map);

        // Spawnaa itemi
        auto item = getScene().spawn<Item>();
        // Määritä että item käyttää tekstruuri id:tä numero 1 (assets/item.png)
        item->setTextureId(1);
        item->setPosition({ 4,6 });

        // Spawn goal using textureId 2
        auto goal = getScene().spawn<Goal>();
        goal->setTextureId(2);
        goal->setPosition({ 5,7 });

    }

    virtual void update(float deltaTime) {
        // Taerkista loppu:
        if (false == isGameRunning()) {
            getRenderer().closeWindow();
            return;
        }

        // TODO: Päivitä peliobjektit ja peli
        for (auto go : getScene().getGameObjects()) {
            go->update(getScene(), deltaTime);
        }

        // Tarkista törmäykset kaikkien peliobjektien kesken:
        for (size_t i = 0; i < getScene().getGameObjects().size(); ++i) {
            auto go1 = getScene().getGameObjects()[i];
            for (size_t j = i + 1; j < getScene().getGameObjects().size(); ++j) {
                auto go2 = getScene().getGameObjects()[j];
                float r1 = 0.5f;
                float r2 = 0.5f;
                sf::Vector2f hs1 = { 0.5f, 0.5f };
                sf::Vector2f hs2 = { 0.5f, 0.5f };
                //              if (collision_check::isAABBAABBCollision(go1->getPosition(), hs1, go2->getPosition(), hs2)) {
                if (collision_check::isSphereSphereCollision(go1->getPosition(), r1, go2->getPosition(), r2)) {
                    // Törmäys!

                    // Kokeile dynamic castilla, että onko peliobjekti 1 "Player"-tyyppi?
                    auto player = dynamic_cast<Player*>(go1.get());
                    if (player != nullptr) {
                        // Jos on, niin tarkista että onko toinen peliobjekti Item tyyppinen
                        auto item = dynamic_cast<Item*>(go2.get());
                        if (item != nullptr) {
                            // Jos on, niin poista itemi skenestä ja kasvata pelaajan pisteitä.
                            getScene().destroyGameObject(go2);
                            player->setScore(player->getScore() + 1);
                        }

                        // Tarkista toinen objekti, onko maali, jos on, niin peli loppuu
                        auto goal = dynamic_cast<Goal*>(go2.get());
                        if (goal) {
                            printf("Player reached goal!\n");
                            gameRunning = false;
                        }

                    }

                    // Kokeile dynamic castilla, että onko peliobjekti 1 "Item"-tyyppi?
                    auto item = dynamic_cast<Item*>(go1.get());
                    if (item != nullptr) {
                        // Jos on, niin tarkista että onko toinen peliobjekti Player tyyppinen
                        auto player = dynamic_cast<Player*>(go2.get());
                        if (player != nullptr) {
                            // Jos on, niin poista itemi skenestä ja kasvata pelaajan pisteit?.
                            getScene().destroyGameObject(go1);
                            player->setScore(player->getScore() + 1);
                        }
                    }

                    // Maali vs player törmäys
                    auto goal = dynamic_cast<Goal*>(go1.get());
                    if (goal != nullptr) {
                        // Jos on, niin tarkista että onko toinen peliobjekti Player tyyppinen
                        auto player = dynamic_cast<Player*>(go2.get());
                        if (player != nullptr) {
                            printf("Player reached goal!\n");
                            gameRunning = false;
                        }
                    }


                }
            }
        }

    }

    virtual void render(engine::SFMLRenderer& renderer) {
        // Aseta kameran paikka ja ruudun koko
        sf::Vector2f camCenter(400 - 24, 300 - 24);
        sf::Vector2f screenSize(800, 600);
        renderer.setView(camCenter, screenSize);

        // Rendaa mappi ensin:
        renderer.renderMap(map, mapTextures);

        // Rendaa sitten kaikki skenen peliobjektit (jotta ne n?kyy mapin päällä)
        // (see: https://en.wikipedia.org/wiki/Painter%27s_algorithm)

        // Hae peliobjektit skenestä ja käy ne kaikki läpi for loopissa:
        auto gameObjects = getScene().getGameObjects();
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            auto gameObject = gameObjects[i];
            // Ota tekstuuri characterTextures vektorista peliobjektin tekstuuri, tekstuuri id:n mukaan:
            auto texture = characterTextures[gameObject->getTextureId()];
            // Rendaa peliobjekti rendererin avulla peliobjektin jäsenmuuttujien määräämien arvojen mukaan.
            renderer.renderSprite(gameObject->getPosition(), gameObject->getSize(), gameObject->getRotation(), texture);
        }
    }


    // Tai näin void setMap(const auto& newMap)
    void setMap(const std::vector< std::vector<int> >& newMap) {
        map = newMap;
    }

    void setMapTextures(const std::vector<std::string>& fileNames) {
        mapTextures = getRenderer().loadTextures(fileNames);
    }

    void setCharacterTextures(const std::vector<std::string>& fileNames) {
        characterTextures = getRenderer().loadTextures(fileNames);
    }

    bool isGameRunning() const {
        return gameRunning;
    }

private:
    std::vector< std::vector<int> > map;
    std::vector< sf::Texture > mapTextures;
    std::vector< sf::Texture > characterTextures;
    bool gameRunning = true;

}; // End - MyGameApp


int main() {
    std::vector< std::vector<int> > map = {
        // x==0, x==1, x==2, x==3
        { 1,        1,    1,   1, 1, 1, 1 }, // Rivi, jossa y==0
        { 1,        0,    0,   0, 0, 0, 1 }, // Rivi, jossa y==1
        { 1,        0,    0,   0, 0, 0, 1 }, // Rivi, jossa y==2
        { 1,        0,    0,   1, 0, 0, 1 }, // Rivi, jossa y==3
        { 1,        0,    0,   1, 0, 0, 1 }, // Rivi, jossa y==4
        { 1,        0,    0,   0, 0, 0, 1 }, // Rivi, jossa y==5, 
        { 1,        0,    0,   0, 0, 0, 1 }, // Rivi, jossa y==6, 
        { 1,        1,    1,   1, 1, 0, 1 }, // Rivi, jossa y==7, jossa seiniä
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
        "assets/item.png",
        "assets/goal.png",
        });

    //app.run();
    printf("Game over!\n");
    for (const auto gameObj : app.getScene().getGameObjects()) {
        auto player = dynamic_cast<Player*>(gameObj.get());
        if (player != nullptr) {
            printf("Player score: %d\n", player->getScore());
        }
    }
    return 0;
}
