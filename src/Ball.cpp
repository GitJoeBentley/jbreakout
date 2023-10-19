#include "Ball.h"
#include <cmath>
#include <iostream>

Ball::Ball(const std::string& filename, sf::Vector2f pos, Direction dir, float ang, float sp)
    : position(pos), direction(dir), angle(ang), speed(sp )
{
    sf::Texture ballTexture;
    texture.loadFromFile(filename);
    ball.setTexture(texture);
    ball.setOrigin(ball.getLocalBounds().width/2.0f,ball.getLocalBounds().height / 2.0f);
    ball.setPosition(pos);
    angle = static_cast<float>(rand()%80-40);
    // angle = 0.5;  // for debugging
    speed *= 1.25  ;
}

Ball::~Ball()
{ }

void Ball::move()
{
    const float RPD = 0.0174533f;   // Radians Per Degree

    if (direction == Up)
    {
        if (angle > 0.0f)
        {
            position.x = position.x + speed * std::sin(RPD*angle);
            position.y = position.y - speed * std::cos(RPD*angle);
        }
        else
        {
            position.x = position.x + speed * std::sin(RPD*angle);
            position.y = position.y - speed * std::cos(RPD*angle);
        }
        // std::cout << std::sin(RPD*angle) << "   " << std::cos(RPD*angle) << "   " <<  position.x << ',' << position.y << std::endl;

    }
    else  // ball direction is down
    {
        if (angle > 0.0f)
        {
            position.x = position.x + speed * std::sin(RPD*angle);
            position.y = position.y + speed * std::cos(RPD*angle);
        }
        else
        {
            position.x = position.x + speed * std::sin(RPD*angle);
            position.y = position.y + speed * std::cos(RPD*angle);
        }
        // std::cout << std::sin(RPD*angle) << "   " << std::cos(RPD*angle) << "   " <<  position.x << ',' << position.y << std::endl;
    }
    ball.setPosition(position);
}

bool Ball::hitTheWall()
{
    // Hit the top wall
    if (topOfBall() <= WallWidth)
    {
        direction = Down;
        return true;
    }
    // Hit the right wall
    if (rightSideOfBall() >= GameWindowSize.x)
    {
        angle = -angle;
        return true;
    }
    // Hit the left wall
    if (leftSideOfBall() <= WallWidth)
    {
        angle = -angle;
        return true;
    }
    return false;
}

bool Ball::hitATile(Tiles& tiles)
{
    if (position.y > Tiles::getBottomEdge() || position.y < Tiles::getTopEdge() - 8.0f)
        return false;
    for (int row = Tiles::Rows - 1; row >= 0 ; row--)
    {
        for (auto col = 0U; col < Tiles::Cols; col++)
        {
            // std::cout << row << ' ' << col << std::endl;
            if (tiles.getTile(row,col).isVisible() && isCloseToATile(tiles.getTile(row,col)))
            {
                if (hitBottomOfTile(tiles.getTile(row,col)))  // ball hit bottom of tile
                {
                    direction = Down;
                    tiles.getTile(row,col).makeInvisible();
                    return true;
                }
                else if (hitTopOfTile(tiles.getTile(row,col)))  // ball hit top of tile
                {
                    direction = Up;
                    tiles.getTile(row,col).makeInvisible();
                    return true;
                }
                else if (hitLeftSideOfTile(tiles.getTile(row,col)))  // ball hit left side of tile
                {
                    angle = -angle;
                    tiles.getTile(row,col).makeInvisible();
                    return true;
                }
                else if (hitRightSideOfTile(tiles.getTile(row,col)))  // ball hit right side of tile
                {
                    angle = -angle;
                    tiles.getTile(row,col).makeInvisible();
                    return true;
                }
                else
                {
                    // no collision
                }
            }
        }
    }
    return false;
}

bool Ball::isCloseToATile(const Tile& tile) const
{
    if (bottomOfBall() < tile.getTopOfTile())
        return false;
    if (topOfBall() > tile.getBottomOfTile())
        return false;
    if (rightSideOfBall() < tile.getLeftSideOfTile())
        return false;
    if (leftSideOfBall() > tile.getRightSideOfTile())
        return false;
    return true;
}

bool Ball::hitBottomOfTile(const Tile& tile) const
{
    //std::cout << position.x << ' ' << tile.getLocation().x << "   " << fabs(position.x - tile.getLocation().x) << "  " << size().x/2.0f + tile.size().x/2.0f << std::endl;
    if(fabs(position.x - tile.getLocation().x) > (size().x/2.0f + tile.size().x/2.0f))
        return false;
    //std::cout << topOfBall() << ' ' << tile.getBottomOfTile() << ' ' << fabs(topOfBall() - tile.getBottomOfTile()) << std::endl;
    if (fabs(topOfBall() - tile.getBottomOfTile()) > 1.482f)
        return false;
    if (rightSideOfBall() < tile.getLeftSideOfTile())
        return false;
    if (leftSideOfBall() > tile.getRightSideOfTile())
        return false;
    return true;
}

bool Ball::hitTopOfTile(const Tile& tile) const
{
    if (fabs(bottomOfBall() - tile.getTopOfTile()) > 1.0f)
        return false;
    if (rightSideOfBall() < tile.getLeftSideOfTile())
        return false;
    if (leftSideOfBall() > tile.getRightSideOfTile())
        return false;
    return true;
}

bool Ball::hitLeftSideOfTile(const Tile& tile) const
{
    if (fabs(rightSideOfBall() - tile.getLeftSideOfTile()) > 1.0f)
        return false;
    if (topOfBall() > tile.getBottomOfTile())
        return false;
    if (bottomOfBall() < tile.getTopOfTile())
        return false;
    return true;
}

bool Ball::hitRightSideOfTile(const Tile& tile) const
{
    if (fabs(leftSideOfBall() - tile.getRightSideOfTile()) > 1.0f)
        return false;
    if (topOfBall() > tile.getBottomOfTile())
        return false;
    if (bottomOfBall() < tile.getTopOfTile())
        return false;
    return true;
}
