#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player {
   private:
    sf::RectangleShape playerRect;
    sf::Vector2f playerDirection;
    float playerSpeed;

   public:
    Player(sf::Color playerColor, float playerSpeed, sf::Vector2f playerSize, sf::Vector2f playerPos)
        : playerSpeed(playerSpeed), playerDirection(0.0f, 0.0f) {
        playerRect.setFillColor(playerColor);
        playerRect.setSize(playerSize);
        playerRect.setPosition(playerPos);
    }

    void horizontalCollisions() {
        
    }

    void verticalCollisions() {

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

class Tile {
   private:
    sf::RectangleShape tileRect;

   public:
    Tile(sf::Color tileColor, sf::Vector2f tileSize, sf::Vector2f tilePos) {
        tileRect.setFillColor(tileColor);
        tileRect.setSize(tileSize);
        tileRect.setPosition(tilePos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(tileRect);
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

    Player player1(sf::Color::White, 300.0f, sf::Vector2f(48.0f, 48.0f), sf::Vector2f(48.0f, 48.0f));
    Player player2(sf::Color::White, 300.0f, sf::Vector2f(96.0f, 96.0f), sf::Vector2f(192.0f, 192.0f));

    Tile tile1(sf::Color::Black, sf::Vector2f(96.0f, 96.0f), sf::Vector2f(384.0f, 384.0f));

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                player1.verticalMovement(event.key.code, true);
                player1.horizontalMovement(event.key.code, true);

                player2.verticalMovement(event.key.code, true);
                player2.horizontalMovement(event.key.code, true);
            }
            if (event.type == sf::Event::KeyReleased) {
                player1.verticalMovement(event.key.code, false);
                player1.horizontalMovement(event.key.code, false);

                player2.verticalMovement(event.key.code, false);
                player2.horizontalMovement(event.key.code, false);
            }
        }
        deltaTime = clock.restart().asSeconds();
        player1.update(deltaTime);
        player2.update(deltaTime);

        window.clear(sf::Color(64, 64, 64, 255));

        player1.draw(window);
        player2.draw(window);
        tile1.draw(window);

        window.display();
    }
    return 0;
}
