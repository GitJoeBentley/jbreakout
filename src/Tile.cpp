#include "Tile.h"


Tile::Tile()
    : sprite(TileTexture), visible(true), location(sf::Vector2f(0.0f,0.0f))
{
    sprite.setOrigin(sprite.getLocalBounds().width/2.0f,sprite.getLocalBounds().height / 2.0f);
    sprite.setColor(sf::Color(255,0,0));
}

Tile::~Tile()
{
    //dtor
}
