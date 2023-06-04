#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Tile {
public:
    sf::RectangleShape m_rectangle;
    Tile(sf::Color color,
         sf::Vector2f size,
         sf::Vector2f position)
        : m_color(color),
          m_size(size),
          m_startPosition(position) {
        m_rectangle.setFillColor(m_color);
        m_rectangle.setSize(m_size);
        m_rectangle.setPosition(m_startPosition);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(m_rectangle);
    }

private:
    sf::Color m_color;
    sf::Vector2f m_size;
    sf::Vector2f m_startPosition;
};

class Player {
public:
    Player(
        sf::Color color,
        float speed,
        sf::Vector2f size,
        sf::Vector2f position,
        std::vector<Tile> &tileGroup)
        : m_color(color),
          m_speed(speed),
          m_size(size),
          m_position(position),
          m_tileGroup(tileGroup),
          m_direction(0.0f, 0.0f) {
        m_rectangle.setFillColor(m_color);
        m_rectangle.setSize(m_size);
        m_rectangle.setPosition(m_position);
    }

    void update(sf::RenderWindow &window, float deltaTime) {
        horizontalMovement(deltaTime);
        horizontalCollisions();

        verticalMovement(deltaTime);
        verticalCollisions();

        camera(window);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(m_rectangle);
    }

private:
    sf::Color m_color;
    float m_speed;
    sf::Vector2f m_size;
    sf::Vector2f m_position;
    std::vector<Tile> m_tileGroup;

    sf::Vector2f m_direction;
    sf::RectangleShape m_rectangle;

    void horizontalMovement(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_direction.x = 0.0f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_direction.x = -1.0f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_direction.x = 1.0f;
        } else {
            m_direction.x = 0.0f;
        }

        m_rectangle.move(sf::Vector2f(m_direction.x * deltaTime * m_speed, 0.0f));
    }

    void horizontalCollisions() {
        for (auto &tile : m_tileGroup) {
            if (m_rectangle.getGlobalBounds().intersects(tile.m_rectangle.getGlobalBounds())) {
                if (m_direction.x > 0.0f) {
                    m_rectangle.setPosition(sf::Vector2f(
                        tile.m_rectangle.getGlobalBounds().left - m_rectangle.getSize().x,
                        m_rectangle.getPosition().y));
                } else if (m_direction.x < 0.0f) {
                    m_rectangle.setPosition(sf::Vector2f(
                        tile.m_rectangle.getGlobalBounds().left + tile.m_rectangle.getSize().x,
                        m_rectangle.getPosition().y));
                }
            }
        }
    }

    void verticalMovement(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_direction.y = 0.0f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_direction.y = -1.0f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_direction.y = 1.0f;
        } else {
            m_direction.y = 0.0f;
        }

        m_rectangle.move(sf::Vector2f(0.0f, m_direction.y * deltaTime * m_speed));
    }

    void verticalCollisions() {
        for (auto &tile : m_tileGroup) {
            if (m_rectangle.getGlobalBounds().intersects(tile.m_rectangle.getGlobalBounds())) {
                if (m_direction.y > 0.0f) {
                    m_rectangle.setPosition(sf::Vector2f(
                        m_rectangle.getPosition().x,
                        tile.m_rectangle.getGlobalBounds().top - m_rectangle.getSize().y));
                } else if (m_direction.y < 0.0f) {
                    m_rectangle.setPosition(sf::Vector2f(
                        m_rectangle.getPosition().x,
                        tile.m_rectangle.getGlobalBounds().top + tile.m_rectangle.getSize().y));
                }
            }
        }
    }

    void camera(sf::RenderWindow &window) {
        if (m_rectangle.getPosition().x + m_rectangle.getSize().x / 2.0f <
            window.getView().getCenter().x - window.getSize().x / 2.0f) {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x - window.getSize().x / 2.0f * 3.0f,
                window.getView().getCenter().y - window.getSize().y / 2.0f,
                window.getSize().x, window.getSize().y)));
        } else if (m_rectangle.getPosition().x + m_rectangle.getSize().x / 2.0f >
                   window.getView().getCenter().x + window.getSize().x / 2.0f) {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x + window.getSize().x / 2.0f * 3.0f,
                window.getView().getCenter().y + window.getSize().y / 2.0f,
                window.getSize().x, window.getSize().y)));
        } else if (m_rectangle.getPosition().y + m_rectangle.getSize().y / 2.0f <
                   window.getView().getCenter().y - window.getSize().y / 2.0f) {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x + window.getSize().x / 2.0f,
                window.getView().getCenter().y - window.getSize().y / 2.0f * 3.0f,
                window.getSize().x, window.getSize().y)));
        } else if (m_rectangle.getPosition().y + m_rectangle.getSize().y / 2.0f >
                   window.getView().getCenter().y + window.getSize().y / 2.0f) {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x - window.getSize().x / 2.0f,
                window.getView().getCenter().y + window.getSize().y / 2.0f * 3.0f,
                window.getSize().x, window.getSize().y)));
        }
    }
};

void loadLevel(std::string map,
               float &playerPositionX,
               float &playerPositionY,
               std::vector<Tile> &tileGroup) {
    std::ifstream file(map);
    std::string line;

    float x;
    float y;

    if (file.is_open()) {
        float row_index = 0.0f;
        while (std::getline(file, line)) {
            for (float collom_index = 0.0f; collom_index < line.length(); collom_index++) {
                x = collom_index * 48.0f;
                y = row_index * 48.0f;

                if (line[collom_index] == 't') { // t - tile
                    tileGroup.push_back(Tile(
                        sf::Color::Black,
                        sf::Vector2f(48.0f, 48.0f),
                        sf::Vector2f(x, y)));
                } else if (line[collom_index] == 'p') { // p - player
                    playerPositionX = x;
                    playerPositionY = y;
                }
            }
            row_index++;
        }
    } else {
        std::cout << "error: can't open 'map.txt'\n";
        exit(1);
    }
}

int main() {
    std::string windowTitle = "sfml aabb collision";
    int windowWidth = 1200;
    int windowHeight = 816;

    int windowPositionX = sf::VideoMode::getDesktopMode().width / 2 -
                          windowWidth / 2;
    int windowPositionY = sf::VideoMode::getDesktopMode().height / 2 -
                          windowHeight / 2;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                            windowTitle,
                            sf::Style::Close);
    window.setPosition(sf::Vector2i(windowPositionX, windowPositionY));

    sf::Clock clock;
    float deltaTime;

    std::vector<Tile> tileGroup;

    float playerPositionX;
    float playerPositionY;

    loadLevel(std::string("map.txt"),
              playerPositionX,
              playerPositionY,
              tileGroup);

    Player player(
        sf::Color::White,                               // player color
        225.0f,                                         // player speed
        sf::Vector2f(48.0f, 48.0f),                     // player size
        sf::Vector2f(playerPositionX, playerPositionY), // player position
        tileGroup);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();

        player.update(window, deltaTime);

        window.clear(sf::Color(64, 64, 64));

        for (auto &tile : tileGroup) {
            tile.draw(window);
        }

        player.draw(window);

        window.display();
    }
    return 0;
}