#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player {
   public:
    Player(sf::Color color, float playerSpeed, sf::Vector2f playerSize, sf::Vector2f playerPos) {
        this->playerRect.setFillColor(color);
        this->playerSpeed = playerSpeed;
        this->playerRect.setSize(playerSize);
        this->playerRect.setPosition(playerPos);
        this->playerDirection = sf::Vector2f(0.0f, 0.0f);
    }

    void horizontalMovement(sf::Keyboard::Key key, bool isPressed) {
        if (isPressed) {
            if (key == sf::Keyboard::A) {
                this->playerDirection.x = -1.0f;
            } else if (key == sf::Keyboard::D) {
                this->playerDirection.x = 1.0f;
            }
        } else {
            if ((key == sf::Keyboard::A && this->playerDirection.x == -1.0f) || (key == sf::Keyboard::D && this->playerDirection.x == 1.0f)) {
                this->playerDirection.x = 0.0f;
            }
        }
    }

    void verticalMovement(sf::Keyboard::Key key, bool isPressed) {
        if (isPressed) {
            if (key == sf::Keyboard::W) {
                this->playerDirection.y = -1.0f;
            } else if (key == sf::Keyboard::S) {
                this->playerDirection.y = 1.0f;
            }
        } else {
            if ((key == sf::Keyboard::W && this->playerDirection.y == -1.0f) || (key == sf::Keyboard::S && this->playerDirection.y == 1.0f)) {
                this->playerDirection.y = 0.0f;
            }
        }
    }

    void update(float deltaTime) {
        this->playerRect.move(this->playerDirection * deltaTime * this->playerSpeed);
    }

    void draw(sf::RenderWindow &window) { window.draw(this->playerRect); }

   private:
    sf::RectangleShape playerRect;
    sf::Vector2f playerDirection;
    float playerSpeed;
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

    Player player(sf::Color::White, 300.0f, sf::Vector2f(48.0f, 48.0f), sf::Vector2f(100.0f, 100.0f));

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
