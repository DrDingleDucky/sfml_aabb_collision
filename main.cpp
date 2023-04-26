#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player {
   public:
    Player(sf::Color color, float playerWidth, float playerHeight) {
        playerRect.setSize(sf::Vector2f(playerWidth, playerHeight));
        playerRect.setFillColor(color);
    }
    void horizontalMovement(sf::Keyboard::Key key, bool isPressed) {
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        //     playerDirection.x = -1;
        // } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        //     playerDirection.x = 1;
        // } else {
        //     playerDirection.x = 0;
        // }
        if (isPressed) {
            if (key == sf::Keyboard::A) {
                playerDirection.x = -1;
            } else if (key == sf::Keyboard::D) {
                playerDirection.x = 1;
            }
        } else if (playerDirection.x != 0 &&
                   ((key == sf::Keyboard::A && playerDirection.x == -1) ||
                    (key == sf::Keyboard::D && playerDirection.x == 1))) {
            playerDirection.x = 0;
        }
    }
    void verticalMovement(sf::Keyboard::Key key, bool isPressed) {
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        //     playerDirection.y = -1;
        // } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //     playerDirection.y = 1;
        // } else {
        //     playerDirection.y = 0;
        // }
        if (isPressed) {
            if (key == sf::Keyboard::W) {
                playerDirection.y = -1;
            } else if (key == sf::Keyboard::S) {
                playerDirection.y = 1;
            }
        } else if (playerDirection.y != 0 &&
                   ((key == sf::Keyboard::W && playerDirection.y == -1) ||
                    (key == sf::Keyboard::S && playerDirection.y == 1))) {
            playerDirection.y = 0;
        }
    }

    void update(float deltaTime) {
        playerRect.move(playerDirection * deltaTime * 300.f);
    }
    void draw(sf::RenderWindow &window) { window.draw(playerRect); }

   private:
    sf::RectangleShape playerRect;
    sf::Vector2f playerDirection;
};

int main() {
    int windowWidth = 1200;
    int windowHeight = 816;
    std::string windowTitle = "Game";
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                            windowTitle, sf::Style::Close);
    window.setPosition(sf::Vector2i(100, 100));
    sf::Event event;

    sf::Clock clock;
    float deltaTime;

    Player player(sf::Color::White, 48, 48);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                player.verticalMovement(event.key.code, true);
                player.horizontalMovement(event.key.code, true);
            }
            if (event.type == sf::Event::KeyReleased) {
                player.verticalMovement(event.key.code, false);
                player.horizontalMovement(event.key.code, false);
            }
        }
        // update
        deltaTime = clock.restart().asSeconds();
        player.update(deltaTime);

        // render
        window.clear(sf::Color(64, 64, 64, 255));

        player.draw(window);

        window.display();
    }
    return 0;
}
