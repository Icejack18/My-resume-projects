#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <chrono>
#include <thread>

sf::Texture loadTexture(const std::string& textureFilename) {
    sf::Texture texture;
    if (texture.loadFromFile(textureFilename) == false) {
        printf("Failed to load texture: %s\n", textureFilename.c_str());
        throw std::runtime_error("Failed to load texture: " + textureFilename);
    }
    return texture;
}

void drawSprite(sf::RenderWindow& window, 
    const sf::Vector2f& position, 
    const sf::Vector2f& size, 
    float rotationInRadians, 
    const sf::Texture& player) {

    static const sf::Vector2f tileSize(48.0f, 48.0f);

    sf::Sprite sprite(player);
    sprite.setScale(size);
    sprite.setOrigin(sf::Vector2f(tileSize.x * 0.5f, tileSize.y * 0.5f));
    sprite.setPosition(sf::Vector2f(position.x * tileSize.x, position.y * tileSize.y));

    sprite.setRotation(sf::Angle(sf::radians(rotationInRadians)));
    window.draw(sprite);
}

void renderMap(sf::RenderWindow& window,
    const std::vector<std::vector<int>>& map,
    const sf::Texture& ground,
    const sf::Texture& grass,
    const sf::Texture& wall) {
    for (size_t y = 0; y < map.size(); y++) { // Width
        for (size_t x = 0; x < map[0].size(); x++) { // Length
            if (map.at(y).at(x) == 1) {
                drawSprite(window, sf::Vector2f(x, y), sf::Vector2f(1, 1), 0.0f, ground);
            }
            else if (map.at(y).at(x) == 2) {
                drawSprite(window, sf::Vector2f(x, y), sf::Vector2f(1, 1), 0.0f, wall);
            }
            else if (map.at(y).at(x) == 0) {
                drawSprite(window, sf::Vector2f(x, y), sf::Vector2f(1, 1), 0.0f, grass);
            }
        }
    }
}


std::vector<sf::Texture> loadTextures(const std::vector<std::string>& fileNames) {
    std::vector<sf::Texture> result;
    for (const auto& fileName : fileNames) {
        auto texture = loadTexture(fileName);
        result.push_back(texture);
    }
    
    return result;
}

int main() {
    const auto size = sf::Vector2u(800, 600);
    sf::RenderWindow window(sf::VideoMode(size), "Eka SFML Ikkuna");
    int red = 150, green = 70, blue = 80;
    float rotation = 0.0f;

    int playerX = 1, playerY = 1;

    /*
    std::vector<std::string> allTextures = {
    "assets/player.png",
    "assets/ground.png",
    "assets/wall.png",
    "assets/grass.png"
    };
    loadTextures(allTextures);
    */

    auto player = loadTexture("assets/player.png");
    auto ground = loadTexture("assets/ground.png");
    auto wall = loadTexture("assets/wall.png");
    auto grass = loadTexture("assets/grass.png");

    std::vector<std::vector<int>> map = {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };
    // run the program as long as the window is open
    while (window.isOpen())
    {
        rotation += 0.0001f;
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // TODO: Käsittele käyttäjän syöte (peliohjain, näppis, hiiri)

        // TODO: Update game logic

        // TODO: Render game object to screen
        // Begin current frame
        window.clear(sf::Color(red, green, blue));

        // Liikuta "kamera"
        sf::Vector2f camCenter(400-24, 300-24);
        sf::Vector2f screenSize = window.getView().getSize();
        sf::View view(camCenter, screenSize);
        window.setView(view);

        
        renderMap(window, map, ground, grass, wall);
        drawSprite(window, sf::Vector2f(playerX, playerY), sf::Vector2f(1, 1), rotation, player);
        window.display();

        // Players movement
        // TODO: Add keyboard reading 
        // move left
        // playerX--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            playerX--;

        }

        // move right
        // playerX++;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            playerX++;

        }
        
        // move up
        // playerY--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            playerY--;

        }
         
        // move down
        // playerY++;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            playerY++;
            
        }

    }

    return 0;
}

