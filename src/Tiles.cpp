#include "Tiles.h"

extern const float WallWidth;

const float Tiles::TopEdge = 100.0f;

Tiles::Tiles()
{
    float tileWidth = array[0][0].size().x;
    float tileHeight = array[0][0].size().y;
    sf::Vector2f location;
     uint8_t redcolor = 0; uint8_t greencolor = 0; uint8_t bluecolor = UINT8_MAX;
    for (auto row = 0U; row < Rows; row++)
    {
        redcolor += UINT8_MAX / Rows;
        for (auto col = 0U; col < Cols; col++)
        {
            bluecolor -= static_cast<uint8_t>(1);
             location = sf::Vector2f(WallWidth+tileWidth/2+col*tileWidth,TopEdge+row*tileHeight);
            array[row][col].setLocation(location);
            array[row][col].getSprite().setColor(sf::Color(redcolor,greencolor,bluecolor));
            // std::cout << "tile " << array[row][col].getLocation().x << ',' << array[row][col].getLocation().y << std::endl;
        }
        array[row][0].makeInvisible();
        array[row][Cols-1].makeInvisible();
    }
}

Tiles::~Tiles()
{

}
