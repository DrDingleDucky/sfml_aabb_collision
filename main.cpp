#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Tile
{
public:
    sf::RectangleShape rect;
    Tile(sf::Color color, sf::Vector2f size, sf::Vector2f pos)
    {
        this->color = color;
        this->size = size;
        this->pos = pos;
        rect.setFillColor(color);
        rect.setSize(size);
        rect.setPosition(pos);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(rect);
    }

private:
    sf::Color color;
    sf::Vector2f size;
    sf::Vector2f pos;
};

class Player
{
public:
    Player(
        float speed,
        sf::Color color,
        sf::Vector2f size,
        sf::Vector2f pos)
    {
        this->speed = speed;
        this->color = color;
        this->size = size;
        this->pos = pos;
        direction = sf::Vector2f(0.0f, 0.0f);
        rect.setFillColor(color);
        rect.setSize(size);
        rect.setPosition(pos);
    }

    void update(
        sf::RenderWindow &window,
        float deltaTime,
        std::vector<Tile> &tileGroup)
    {
        horizontalMovement(deltaTime);
        horizontalCollisions(tileGroup);
        verticalMovement(deltaTime);
        verticalCollisions(tileGroup);
        camera(window);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(rect);
    }

private:
    float speed;
    sf::Color color;
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::Vector2f direction;
    sf::RectangleShape rect;

    void horizontalMovement(float deltaTime)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            direction.x = 0.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            direction.x = -1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            direction.x = 1.0f;
        }
        else
        {
            direction.x = 0.0f;
        }

        rect.move(sf::Vector2f(direction.x * deltaTime * speed, 0.0f));
    }

    void horizontalCollisions(std::vector<Tile> &tileGroup)
    {
        for (auto &tile : tileGroup)
        {
            if (rect.getGlobalBounds().intersects(tile.rect.getGlobalBounds()))
            {
                if (direction.x > 0.0f)
                {
                    rect.setPosition(sf::Vector2f(
                        tile.rect.getGlobalBounds().left -
                            rect.getSize().x,
                        rect.getPosition().y));
                }
                else if (direction.x < 0.0f)
                {
                    rect.setPosition(sf::Vector2f(
                        tile.rect.getGlobalBounds().left +
                            tile.rect.getSize().x,
                        rect.getPosition().y));
                }
            }
        }
    }

    void verticalMovement(float deltaTime)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            direction.y = 0.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            direction.y = -1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            direction.y = 1.0f;
        }
        else
        {
            direction.y = 0.0f;
        }

        rect.move(sf::Vector2f(0.0f, direction.y * deltaTime * speed));
    }

    void verticalCollisions(std::vector<Tile> &tileGroup)
    {
        for (auto &tile : tileGroup)
        {
            if (rect.getGlobalBounds().intersects(tile.rect.getGlobalBounds()))
            {
                if (direction.y > 0.0f)
                {
                    rect.setPosition(sf::Vector2f(
                        rect.getPosition().x,
                        tile.rect.getGlobalBounds().top -
                            rect.getSize().y));
                }
                else if (direction.y < 0.0f)
                {
                    rect.setPosition(sf::Vector2f(
                        rect.getPosition().x,
                        tile.rect.getGlobalBounds().top +
                            tile.rect.getSize().y));
                }
            }
        }
    }

    void camera(sf::RenderWindow &window)
    {
        if (rect.getPosition().x + rect.getSize().x / 2.0f <
            window.getView().getCenter().x - window.getSize().x / 2.0f)
        {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x -
                    window.getSize().x / 2.0f * 3.0f,
                window.getView().getCenter().y -
                    window.getSize().y / 2.0f,
                window.getSize().x,
                window.getSize().y)));
        }
        else if (rect.getPosition().x + rect.getSize().x / 2.0f >
                 window.getView().getCenter().x +
                     window.getSize().x / 2.0f)
        {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x +
                    window.getSize().x / 2.0f * 3.0f,
                window.getView().getCenter().y +
                    window.getSize().y / 2.0f,
                window.getSize().x,
                window.getSize().y)));
        }
        else if (rect.getPosition().y + rect.getSize().y / 2.0f <
                 window.getView().getCenter().y -
                     window.getSize().y / 2.0f)
        {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x +
                    window.getSize().x / 2.0f,
                window.getView().getCenter().y -
                    window.getSize().y / 2.0f * 3.0f,
                window.getSize().x,
                window.getSize().y)));
        }
        else if (rect.getPosition().y + rect.getSize().y / 2.0f >
                 window.getView().getCenter().y +
                     window.getSize().y / 2.0f)
        {
            window.setView(sf::View(sf::FloatRect(
                window.getView().getCenter().x -
                    window.getSize().x / 2.0f,
                window.getView().getCenter().y +
                    window.getSize().y / 2.0f * 3.0f,
                window.getSize().x,
                window.getSize().y)));
        }
    }
};

void loadLevel(
    std::string map,
    std::vector<Player> &playerGroup,
    std::vector<Tile> &tileGroup)
{
    std::ifstream file(map);
    std::string line;

    float x;
    float y;

    float collom_index = 0.0f;

    while (std::getline(file, line))
    {
        for (float row_index = 0.0f; row_index < line.length(); row_index++)
        {
            x = row_index * 48.0f;
            y = collom_index * 48.0f;

            if (line[row_index] == 't') // t - tile
            {
                tileGroup.push_back(Tile(
                    sf::Color(0, 0, 0),
                    sf::Vector2f(48.0f, 48.0f),
                    sf::Vector2f(x, y)));
            }
            else if (line[row_index] == 'p') // p - player
            {
                playerGroup.push_back(Player(
                    225.0f,                     // player speed
                    sf::Color(255, 255, 255),   // player color
                    sf::Vector2f(48.0f, 48.0f), // player size
                    sf::Vector2f(x, y)));       // player pos
            }
        }
        collom_index++;
    }
}

int main()
{
    std::string winTitle = "sfml aabb collision";
    int winWidth = 1200;
    int winHeight = 816;

    sf::RenderWindow window(
        sf::VideoMode(winWidth, winHeight),
        winTitle,
        sf::Style::Close);
    window.setPosition(sf::Vector2i(
        sf::VideoMode::getDesktopMode().width / 2 - winWidth / 2,
        sf::VideoMode::getDesktopMode().height / 2 - winHeight / 2));

    std::vector<Player> playerGroup;
    std::vector<Tile> tileGroup;

    loadLevel(std::string("map.txt"), playerGroup, tileGroup);

    sf::Clock clock;
    float deltaTime;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        // update
        deltaTime = clock.restart().asSeconds();

        for (auto &player : playerGroup)
        {
            player.update(window, deltaTime, tileGroup);
        }

        // draw
        window.clear(sf::Color(64, 64, 64));

        for (auto &tile : tileGroup)
        {
            tile.draw(window);
        }

        for (auto &player : playerGroup)
        {
            player.draw(window);
        }

        window.display();
    }
    return 0;
}