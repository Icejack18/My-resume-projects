#include <stdexcept>
#include <engine/application.h>
#include <engine/sfml_renderer.h>

int main() {

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

    engine::SFMLRenderer renderer("Engine v1 tester", 1000, 800);
    auto mapTextures = renderer.loadTextures({
    "assets/ground.png",
    "assets/wall.png",
    "assets/grass.png" });

    auto characterTexture = renderer.loadTexture( "assets/player.png" );
    while (renderer.isWindowOpen())
    {
        renderer.beginFrame();

        sf::Vector2f camCenter(500 - 24, 400 - 24);
        sf::Vector2f screenSize(1000, 800);
        renderer.setView(camCenter, screenSize);

        renderer.renderMap(map, mapTextures);
        renderer.renderSprite(sf::Vector2f(1, 1), sf::Vector2f(1, 1), 0.0f, characterTexture);

        renderer.endFrame();
    }

    return 0;
}

