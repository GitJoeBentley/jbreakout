#include <cmath>
#include "Paddle.h"

Paddle::Paddle(const std::string& filename, sf::Vector2f pos)
    : position(pos), paddleSpeed(9.0f)
{
    texture.loadFromFile(filename);
    paddle.setTexture(texture);
    paddle.setOrigin(paddle.getLocalBounds().width/2.0f,paddle.getLocalBounds().height / 2.0f);
    paddle.setPosition(pos);
}

void Paddle::moveToStartPosition()
{
    position = sf::Vector2f(GameWindowSize.x / 2.0f, 0.98f * GameWindowSize.y);
    paddle.setPosition(position);
}

Paddle::~Paddle()
{
    //dtor
}

void Paddle::move(Direction dir)
{
    if (dir == Right)
        position.x += paddleSpeed;
    if (dir == Left)
        position.x -= paddleSpeed;
    paddle.setPosition(position);
}

bool Paddle::hitTheBall(Ball& ball)
{
    float deltaX = ball.getPosition().x - position.x;
    float deltaY = position.y - ball.getPosition().y;

    float sumOfHalfWidths = size().x/2.0f + ball.size().x/2.0f;
    float sumOfHalfHeights = size().y/2.0f + ball.size().y/2.0f;

    // Collision tolerance tweak
    sumOfHalfHeights *= 1.0f;
    sumOfHalfWidths *= 0.94f;

    float percentCollisionLocationOnPaddle;
    float newAngle;

    if (deltaY >=0.0f && fabs(deltaX) <= sumOfHalfWidths && deltaY <= sumOfHalfHeights)
    {
        percentCollisionLocationOnPaddle = 100.0f * deltaX / ((size().x + ball.size().x) / 2.0f);
        ball.setDirection(Ball::Up);
        if (percentCollisionLocationOnPaddle > 50.0f)  // Hit the right side of the paddle
        {
            newAngle = ball.getAngle() + (percentCollisionLocationOnPaddle - 50.0f);
            if (newAngle > 72.0f)
                newAngle = 72.0f;
            //std::cout << "new angle: " << newAngle << std::endl;
            ball.setAngle(newAngle);
            ball.move();
        }
        if (percentCollisionLocationOnPaddle < -50.0f)  // Hit the left side of the paddle
        {
            newAngle = ball.getAngle() + (percentCollisionLocationOnPaddle + 50.0f);
            if (newAngle < -72.0f)
                newAngle = -7.0f;
            //std::cout << "*new angle: " << newAngle << std::endl;
            ball.setAngle(newAngle);
            ball.move();
        }

        return true;
    }
    return false;
}
