#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Tile {
   public:
    sf::RectangleShape tileRect;
    Tile(sf::Color tileColor, sf::Vector2f tileSize, sf::Vector2f tilePos) {
        tileRect.setFillColor(tileColor);
        tileRect.setSize(tileSize);
        tileRect.setPosition(tilePos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(tileRect);
    }
};

class Player {
   private:
    float playerSpeed;
    Tile tile;

   public:
    sf::Vector2f playerDirection;
    sf::RectangleShape playerRect;
    Player(sf::Color playerColor, float playerSpeed, sf::Vector2f playerSize, sf::Vector2f playerPos, Tile tile)
        : playerSpeed(playerSpeed), playerDirection(0.0f, 0.0f), tile(tile) {
        playerRect.setFillColor(playerColor);
        playerRect.setSize(playerSize);
        playerRect.setPosition(playerPos);
    }

    void horizontalMovement(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            playerDirection.x = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerDirection.x = 1;
        } else {
            playerDirection.x = 0;
        }

        playerRect.move(sf::Vector2f(playerDirection.x * deltaTime * playerSpeed, 0.0f));
    }

    void horizontalCollisions() {
        if (playerRect.getGlobalBounds().intersects(tile.tileRect.getGlobalBounds())) {
            if (playerDirection.x > 0) {
                playerRect.setPosition(sf::Vector2f(tile.tileRect.getGlobalBounds().left - playerRect.getSize().x, playerRect.getPosition().y));
            } else if (playerDirection.x < 0) {
                playerRect.setPosition(sf::Vector2f(tile.tileRect.getGlobalBounds().left + tile.tileRect.getSize().x, playerRect.getPosition().y));
            }
        }
    }

    void verticalMovement(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            playerDirection.y = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            playerDirection.y = 1;
        } else {
            playerDirection.y = 0;
        }

        playerRect.move(sf::Vector2f(0 ,playerDirection.y * deltaTime * playerSpeed));
    }

    void verticalCollisions() {
        if (playerRect.getGlobalBounds().intersects(tile.tileRect.getGlobalBounds())) {
            if (playerDirection.y > 0) {
                playerRect.setPosition(sf::Vector2f(playerRect.getPosition().x, tile.tileRect.getGlobalBounds().top - playerRect.getSize().y));
            } else if (playerDirection.y < 0) {
                playerRect.setPosition(sf::Vector2f(playerRect.getPosition().x, tile.tileRect.getGlobalBounds().top + tile.tileRect.getSize().y));
            }
        }
    }

    void update(float deltaTime) {
        horizontalMovement(deltaTime);
        horizontalCollisions();

        verticalMovement(deltaTime);
        verticalCollisions();
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

    Tile tile(sf::Color::Black, sf::Vector2f(96.0f, 96.0f), sf::Vector2f(384.0f, 384.0f));
    Player player1(sf::Color::White, 100.0f, sf::Vector2f(48.0f, 48.0f), sf::Vector2f(48.0f, 48.0f), tile);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
        deltaTime = clock.restart().asSeconds();
        player1.update(deltaTime);

        window.clear(sf::Color(64, 64, 64, 255));

        player1.draw(window);
        tile.draw(window);

        window.display();
    }
    return 0;
}