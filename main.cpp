#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player {
   private:
    sf::RectangleShape playerRect;
    sf::Vector2f playerDirection;
    float playerSpeed;

   public:
    Player(sf::Color color, float playerSpeed, sf::Vector2f playerSize, sf::Vector2f playerPos) 
        : playerSpeed(playerSpeed), playerDirection(0.0f, 0.0f) {
        playerRect.setFillColor(color);
        playerRect.setSize(playerSize);
        playerRect.setPosition(playerPos);
    }

    void horizontalMovement(sf::Keyboard::Key key, bool isPressed) {
        if (isPressed) {
            if (key == sf::Keyboard::A) {
                playerDirection.x = -1.0f;
            } else if (key == sf::Keyboard::D) {
                playerDirection.x = 1.0f;
            }
        } else {
            if ((key == sf::Keyboard::A && playerDirection.x == -1.0f) || (key == sf::Keyboard::D && playerDirection.x == 1.0f)) {
                playerDirection.x = 0.0f;
            }
        }
    }

    void verticalMovement(sf::Keyboard::Key key, bool isPressed) {
        if (isPressed) {
            if (key == sf::Keyboard::W) {
                playerDirection.y = -1.0f;
            } else if (key == sf::Keyboard::S) {
                playerDirection.y = 1.0f;
            }
        } else {
            if ((key == sf::Keyboard::W && playerDirection.y == -1.0f) || (key == sf::Keyboard::S && playerDirection.y == 1.0f)) {
                playerDirection.y = 0.0f;
            }
        }
    }

    void update(float deltaTime) {
        playerRect.move(playerDirection * deltaTime * playerSpeed);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(playerRect);
    }
};

int main() {
    std::string windowTitle = "Game";
    int windowWidth = 1200;
    int windowHeight = 816;

    int windowPositionX = sf::VideoMode::getDesktopMode().width / 2 - windowWidth / 2;
    int windowPositionY = sf::VideoMode::getDesktopMode().height / 2 - windowHeight / 2;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Close);
    window.setPosition(sf::Vector2i(windowPositionX, windowPositionY));
    sf::Event event;

    sf::Clock clock;
    float deltaTime;

    Player player(sf::Color::White, 300.0f, sf::Vector2f(48.0f, 48.0f), sf::Vector2f(48.0f, 48.0f));

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
        deltaTime = clock.restart().asSeconds();
        player.update(deltaTime);

        window.clear(sf::Color(64, 64, 64, 255));

        player.draw(window);

        window.display();
    }
    return 0;
}
