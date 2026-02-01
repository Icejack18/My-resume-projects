#include <engine/application.h>
#include <cmath>
#include <chrono>
#include <thread>

namespace  collision_check {

    float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        return std::sqrt(dx * dx + dy + dy);
    }

    bool isMapCollision(const std::vector< std::vector<int> >& map, const sf::Vector2f& position) {
        int x = int(position.x);
        int y = int(position.y);
        if (x < 0 || y < 0) {
            return true; // T�rm�ys "negatiivisella puolella"
        }
        if (y >= map.size()) {
            return true; // T�rm�ys mapin alareunaan
        }

        if (x >= map[y].size()) {
            return true; // T�rm�ys mapin oikeaan reunaan
        }

        int tileId = map[y][x];
        return tileId > 0;
    }

    bool isWallCollision(const std::vector< std::vector<int> >& map,
        const sf::Vector2f& position, const sf::Vector2f& size) {
        sf::Vector2f topLeft = position;
        sf::Vector2f topRight = sf::Vector2f(position.x + size.x, position.y);
        sf::Vector2f sideLeft = sf::Vector2f(position.x, position.y + size.y*0.95);
        sf::Vector2f sideRight = sf::Vector2f(position.x + size.x, position.y + size.y*0.95);
        sf::Vector2f bottomLeft = sf::Vector2f(position.x, position.y + size.y);
        sf::Vector2f bottomRight = sf::Vector2f(position.x + size.x, position.y + size.y);

        return (isMapCollision(map, bottomLeft) && isMapCollision(map, sideLeft))
            || (isMapCollision(map, bottomRight) && isMapCollision(map, sideRight))
            || isMapCollision(map, topLeft)
            || isMapCollision(map, topRight);
    }

    bool isGrassCollision(const std::vector< std::vector<int> >& map,
        const sf::Vector2f& position, const sf::Vector2f& size) {
        sf::Vector2f bottomLeft = sf::Vector2f(position.x, position.y + size.y);
        sf::Vector2f bottomRight = sf::Vector2f(position.x + size.x, position.y + size.y);

        return isMapCollision(map, bottomLeft)
            || isMapCollision(map, bottomRight);
    }

    bool isSphereSphereCollision(const sf::Vector2f& pos1, float r1, const sf::Vector2f& pos2, float r2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        float d = std::sqrt(dx * dx + dy * dy); // Herra Pyhagoras!

        return d < (r1 + r2);
    }
}



class Player : public engine::GameObject {
public:
    typedef std::vector< std::vector<int> > Map;

    Player()
        : GameObject() {
    }
    
    virtual void update(engine::Scene&, float deltaTime) {
        auto newPos = getPosition();
        auto size = sf::Vector2f(0.95, 0.95);

        // Liikuta peliobjektia:
        newPos.x = walking(deltaTime, size);
        newPos.y = verticalMovement(deltaTime, size);
        
        setPosition(newPos);
    }

    float verticalMovement(float deltaTime, sf::Vector2f size) {
        auto newPos = getPosition();

        velocityY += gravity;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !falling){
            velocityY = jump_speed;
            falling = true;
        }
        newPos.y += velocityY * deltaTime;

        if (collision_check::isGrassCollision(*pMap, newPos, size)) {
            falling = false;
            velocityY = 0.0f;
            return getPosition().y;
        }
        else if (collision_check::isWallCollision(*pMap, newPos, size)) {
            falling = true;
            velocityY = 0.0f;
            return getPosition().y;
        }
        else falling = true;
        return newPos.y;
    }

    float walking(float deltaTime, sf::Vector2f size) {
        auto newPos = getPosition();
        auto oldPos = getPosition();
        float dx = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);

        newPos.x += dx * deltaTime * speed;
        if (collision_check::isWallCollision(*pMap, newPos, size)) {
            return oldPos.x;
        }
        else return newPos.x;
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

    int getScore() const{
        return score;
    }

    int& scoreRef() { return score; }

private:
    float speed = 4.5f;
    float gravity = 0.45f;
    float jump_speed = -13.5f;
    float velocityY = 0.0f;
    bool falling = false;
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

class Enemy : public engine::GameObject {
public:
    virtual void update(engine::Scene&, float deltaTime) {
        const float SPEED = 2.0f;

        // Tarkista m_nextWaypointIndex, jotta on m_waypoints vektorin rajojen sisällä.
        if (m_nextWaypointIndex >= 0 && m_nextWaypointIndex < m_waypoints.size()) {
            // Tarkista ollaanko kuin kaukana seuraavasta reittipisteestä
            float distance = collision_check::distance(getPosition(), m_waypoints[m_nextWaypointIndex]);
            if (distance < 0.05f) {
                // Jos lähempänä, kuin 0.05, niin siirry seuraavaan waypointtiin
                m_nextWaypointIndex = (m_nextWaypointIndex + 1) % m_waypoints.size(); // Pidä huoli, että: 0 <= m_nextWaypointIndex <  m_waypoints.size()
            }

            auto toPosition = m_waypoints[m_nextWaypointIndex];
            auto currentPosition = getPosition();
            // d = loppupiste - alkupiste = vektori, joka osoittaa currentPositionista toPositioniin
            auto d = substract(toPosition, currentPosition);
            // Normalisoi d (tee vektorista 1:n mittainen), niin saadaan suunta, jonne liikkua
            auto direction = divide(d, magnitude(d));
            // Kerro suunta vauhdilla, niin saadaan oikeaan suuntaan nopeusvektori, minkä mukaan liikkua
            sf::Vector2f velocity = multiply(SPEED, direction);
            // Liiku deltaTimen verran nopeudella velocity
            move(multiply(deltaTime, velocity));
        }
        else {
            // m_nextWaypointIndex on m_waypoints vektorin rajojen sisällä. Tässä tilanteessa ei liikettä.
        }
    }
    virtual void render(engine::SFMLRenderer& renderer) {
        renderer.renderSprite(getPosition(), getSize(), getRotation(), m_texture);
    }

    void setTexture(sf::Texture texture) {
        m_texture = texture;
    }

    void setWaypoints(const std::vector<sf::Vector2f> waypoints) {
        m_waypoints = waypoints;
        if (m_waypoints.size() > 1) {
            // Aseta nykyinen paikka ensimmäiseen reittipisteeseen
            setPosition(m_waypoints[0]);
            // Aseta reittipisteet
            // Seuraava reittipiste, jonne mennä, löytyy indeksistä 1
            m_nextWaypointIndex = 1;
        }
        else {
            // Clear waypoints
            m_nextWaypointIndex = -1;
        }
    }

private:

    // Vektorien vähennyslasku:
    sf::Vector2f substract(const sf::Vector2f& a, const sf::Vector2f& b) {
        return sf::Vector2f(a.x - b.x, a.y - b.y);
    }

    // Vektorin kertolasku (skalaarilla):
    sf::Vector2f multiply(float multiplier, const sf::Vector2f& vec) {
        return sf::Vector2f(multiplier * vec.x, multiplier * vec.y);
    }

    // Vektorin jakolasku (skalaarilla):
    sf::Vector2f divide(const sf::Vector2f& vec, float divisor) {
        return sf::Vector2f(vec.x / divisor, vec.y / divisor);
    }

    // Vektorin pituus
    float magnitude(const sf::Vector2f& vec) {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    std::vector<sf::Vector2f>	m_waypoints;
    int							m_nextWaypointIndex = -1;
    sf::Texture m_texture;

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
        player->setTextureId(0);
        player->setPosition({ 1,22 });
        // Aseta playerille mapin pointteri, jotta tt toimii
        player->setMap(&map);

        // Spawnaa vihollikset
        auto enemy1 = getScene().spawn<Enemy>();
        enemy1->setTextureId(3);
        enemy1->setWaypoints({ sf::Vector2f(8,22), sf::Vector2f(14,22) });

        auto enemy2 = getScene().spawn<Enemy>();
        enemy2->setTextureId(3);
        enemy2->setWaypoints({ sf::Vector2f(26,22), sf::Vector2f(30,22) });

        auto enemy3 = getScene().spawn<Enemy>();
        enemy3->setTextureId(3);
        enemy3->setWaypoints({ sf::Vector2f(22,17), sf::Vector2f(19,17) });
        enemy3->setPosition({ 22,17 });

        // Spawnaa gems
        auto item1 = getScene().spawn<Item>();
        item1->setTextureId(1);
        item1->setPosition({ 14,1 });

        auto item2 = getScene().spawn<Item>();
        item2->setTextureId(1);
        item2->setPosition({ 14,9 });

        auto item3 = getScene().spawn<Item>();
        item3->setTextureId(1);
        item3->setPosition({ 14,20 });

        auto item4 = getScene().spawn<Item>();
        item4->setTextureId(1);
        item4->setPosition({ 14,19 });

        auto item5 = getScene().spawn<Item>();
        item5->setTextureId(1);
        item5->setPosition({ 30,20 });

        auto item6 = getScene().spawn<Item>();
        item6->setTextureId(1);
        item6->setPosition({ 30,21 });

        auto item7 = getScene().spawn<Item>();
        item7->setTextureId(1);
        item7->setPosition({ 19,15 });

        auto item8 = getScene().spawn<Item>();
        item8->setTextureId(1);
        item8->setPosition({ 19,16 });

        auto item9 = getScene().spawn<Item>();
        item9->setTextureId(1);
        item9->setPosition({ 26,6 });

        auto item10 = getScene().spawn<Item>();
        item10->setTextureId(1);
        item10->setPosition({ 28,1 });

        // Spawn goal using textureId 2
        auto goal = getScene().spawn<Goal>();
        goal->setTextureId(2);
        goal->setPosition({ 30,2 });

    }

    virtual void update(float deltaTime) {
        // Taerkista loppu:
        if (false == isGameRunning()) {
                int res = (600 - timeRef()) * getGems();
                getRenderer().drawResult(res);
                getRenderer().endFrame();
                std::this_thread::sleep_for(std::chrono::seconds(5));
                getRenderer().closeWindow();
            return;
        }


        // TODO: P�ivit� peliobjektit ja peli
        for (auto go : getScene().getGameObjects()) {
            go->update(getScene(), deltaTime);
        }

        // Tarkista t�rm�ykset kaikkien peliobjektien kesken:
        for (size_t i = 0; i < getScene().getGameObjects().size(); ++i) {
            auto go1 = getScene().getGameObjects()[i];
            for (size_t j = i + 1; j < getScene().getGameObjects().size(); ++j) {
                auto go2 = getScene().getGameObjects()[j];
                float r1 = 0.5f;
                float r2 = 0.5f;
                sf::Vector2f hs1 = { 0.5f, 0.5f };
                sf::Vector2f hs2 = { 0.5f, 0.5f };
                if (collision_check::isSphereSphereCollision(go1->getPosition(), r1, go2->getPosition(), r2)) {

                    // Kokeile dynamic castilla, ett� onko peliobjekti 1 "Player"-tyyppi?
                    auto player = dynamic_cast<Player*>(go1.get());

                    if (player != nullptr) {
                        // Jos on, niin tarkista ett� onko toinen peliobjekti Item tyyppinen
                        auto item = dynamic_cast<Item*>(go2.get());
                        if (item != nullptr) {
                            // Jos on, niin poista itemi skenest� ja kasvata pelaajan pisteit�.
                            getScene().destroyGameObject(go2);
                            player->setScore(player->getScore() + 1);
                            setGems(player->getScore());
                        }

                        // Tarkista toinen objekti, onko maali, jos on, niin peli loppuu
                        auto goal = dynamic_cast<Goal*>(go2.get());
                        if (goal) {
                            printf("Player reached goal!\n");
                            gameRunning = false;
                        }

                        // Tarkista toinen objekti, onko maali, jos on, niin peli loppuu
                        auto enemy = dynamic_cast<Enemy*>(go2.get());
                        if (enemy) {
                            printf("Player was captured by enemy\n");
                            player->setPosition({ 1,22 });
                            player->setScore(player->getScore() - 1);
                            setGems(player->getScore());
                        }
                    }

                    // Kokeile dynamic castilla, ett� onko peliobjekti 1 "Item"-tyyppi?
                    auto item = dynamic_cast<Item*>(go1.get());
                    if (item != nullptr) {
                        // Jos on, niin tarkista ett� onko toinen peliobjekti Player tyyppinen
                        auto player = dynamic_cast<Player*>(go2.get());
                        if (player != nullptr) {
                            // Jos on, niin poista itemi skenest� ja kasvata pelaajan pisteit?.
                            getScene().destroyGameObject(go1);
                            player->setScore(player->getScore() + 1);
                            setGems(player->getScore());
                        }
                    }

                    // Maali vs player t�rm�ys
                    auto goal = dynamic_cast<Goal*>(go1.get());
                    if (goal != nullptr) {
                        // Jos on, niin tarkista ett� onko toinen peliobjekti Player tyyppinen
                        auto player = dynamic_cast<Player*>(go2.get());
                        if (player != nullptr) {
                            printf("Player reached goal!\n");
                            gameRunning = false;
                        }
                    }

                    // Vihollinen vs player t�rm�ys
                    auto enemy = dynamic_cast<Enemy*>(go1.get());
                    if (enemy != nullptr) {
                        // Jos on, niin tarkista ett� onko toinen peliobjekti Player tyyppinen
                        auto player = dynamic_cast<Player*>(go2.get());
                        if (player != nullptr) {
                            printf("Player was captured by enemy\n");
                            player->setPosition({ 1,22 });
                            player->setScore(player->getScore() - 1);
                            setGems(player->getScore());
                        }
                    }

                }
            }
        }

    }

    virtual void render(engine::SFMLRenderer& renderer) {
        // Aseta kameran paikka ja ruudun koko
        int screenX = 48 * 32;
        int screenY = 48 * 24;
        sf::Vector2f camCenter(screenX/2 - 24, screenY/2 - 24);
        sf::Vector2f screenSize(screenX, screenY);
        renderer.setView(camCenter, screenSize);

        

        // Rendaa mappi ensin:
        renderer.renderMap(map, mapTextures);

        // Rendaa sitten kaikki skenen peliobjektit (jotta ne n?kyy mapin p��ll�)
        // (see: https://en.wikipedia.org/wiki/Painter%27s_algorithm)

        // Hae peliobjektit skenest� ja k�y ne kaikki l�pi for loopissa:
        auto gameObjects = getScene().getGameObjects();
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            auto gameObject = gameObjects[i];
            // Ota tekstuuri characterTextures vektorista peliobjektin tekstuuri, tekstuuri id:n mukaan:
            auto texture = characterTextures[gameObject->getTextureId()];
            // Rendaa peliobjekti rendererin avulla peliobjektin j�senmuuttujien m��r��mien arvojen mukaan.
            renderer.renderSprite(gameObject->getPosition(), gameObject->getSize(), gameObject->getRotation(), texture);
        }
    }


    // Tai n�in void setMap(const auto& newMap)
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
    // Map is 32 x 24 (0,0 is top left)
    std::vector< std::vector<int> > map = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 0, 2, 0, 2, 2, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1 },

        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },

        { 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1 },

        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1 },

        { 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1 },
        { 1, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },

        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
    };


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
        "assets/enemy.png",
        });
    
    for (const auto gameObj : app.getScene().getGameObjects()) {
        auto player = dynamic_cast<Player*>(gameObj.get());
        if (player != nullptr) {
            app.run(player->scoreRef());
        }
    }
    printf("Game over!\n");
    return 0;
}
