#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tiles.h"

extern const sf::Vector2f GameWindowSize;
extern const float WallWidth;

class Ball
{
public:
    enum Direction {Up, Down};
    Ball(const std::string& filename,
         sf::Vector2f pos = sf::Vector2f(GameWindowSize.x / 2.0f, 0.95f * GameWindowSize.y),
         Direction dir = Up,
         float ang = 0.0f,
         float sp = 0.05f);
    virtual ~Ball();
    sf::Sprite getBall()
    {
        return ball;
    }
    float getAngle() const { return angle; }
    void setDirection(Direction dir) { direction = dir; }
    void setAngle( float ang) { angle = ang; }
    void move();
    void moveToStartPosition() { position = sf::Vector2f(GameWindowSize.x / 2.0f, 0.95f * GameWindowSize.y); }
    bool hitTheWall();
    bool hitATile(Tiles& tiles);
    sf::Vector2f getPosition() const
    {
        return position;
    }
    sf::Vector2f size() const
    {
        return sf::Vector2f(ball.getLocalBounds().width * ball.getScale().x, ball.getLocalBounds().height * ball.getScale().y);
    }
    float bottomOfBall() const { return position.y + size().y / 2.0f; }
    float topOfBall() const { return position.y - size().y / 2.0f; }
    float rightSideOfBall() const { return position.x + size().x / 2.0f; }
    float leftSideOfBall() const { return position.x - size().x / 2.0f; }
    bool isCloseToATile(const Tile& tile) const;
    bool hitBottomOfTile(const Tile& tile) const;
    bool hitTopOfTile(const Tile& tile) const;
    bool hitLeftSideOfTile(const Tile& tile) const;
    bool hitRightSideOfTile(const Tile& tile) const;
protected:

private:
    sf::Texture texture;
    sf::Sprite ball;
    sf::Vector2f position;
    Direction direction;
    float angle;
    float speed;
};

#endif // BALL_H
