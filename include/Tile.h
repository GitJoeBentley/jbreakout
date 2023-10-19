#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
//#include "Ball.h"

extern sf::Texture TileTexture;


class Tile
{
public:
    Tile();
//    Tile(const sf::Texture& texture, const sf::Vector2f location, const sf::Color color);
    virtual ~Tile();
    sf::Sprite getSprite() const
    {
        return sprite;
    }
    sf::Sprite& getSprite()
    {
        return sprite;
    }
    sf::Vector2f size() const
    {
        return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
    }
    bool isVisible() const
    {
        return visible;
    }
    void makeInvisible()
    {
        visible = false;
    }
    void setLocation(sf::Vector2f loc)
    {
        location = loc;
        sprite.setPosition(location);
    }
    sf::Vector2f getLocation() const
    {
        return location;
    }
    float getTopOfTile() const
    {
        return location.y - size().y / 2.0f;
    }
    float getBottomOfTile() const
    {
        return location.y + size().y / 2.0f;
    }
    float getLeftSideOfTile() const
    {
        return location.x - size().x / 2.0f;
    }
    float getRightSideOfTile() const
    {
        return location.x + size().x / 2.0f;
    }

private:
    sf::Sprite sprite;
    bool visible;
    sf::Vector2f location;
};

#endif // TILE_H
