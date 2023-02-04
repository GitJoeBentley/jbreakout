#ifndef PADDLE_H
#define PADDLE_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Ball.h"

extern const sf::Vector2f GameWindowSize;
extern const float WallWidth;


class Paddle
{
public:
    enum Direction {Right, Left};
    Paddle(const std::string& filename, sf::Vector2f pos = sf::Vector2f(GameWindowSize.x / 2.0f, 0.98f * GameWindowSize.y));
    virtual ~Paddle();
    sf::Sprite getPaddle() const
    {
        return paddle;
    }
    sf::Vector2f getPosition() const
    {
        return position;
    }
    void move(Direction dir);
    void moveToStartPosition();
    bool hitTheBall(Ball&);
    sf::Vector2f size() const { return sf::Vector2f(paddle.getLocalBounds().width * paddle.getScale().x, paddle.getLocalBounds().height * paddle.getScale().y); }

private:
    sf::Texture texture;
    sf::Sprite paddle;
    sf::Vector2f position;
    float paddleSpeed;
};

#endif // PADDLE_H
