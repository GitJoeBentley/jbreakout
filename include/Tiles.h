#ifndef TILES_H
#define TILES_H

#include <string>

#include "Tile.h"

class Tiles
{
    public:
        Tiles();
        virtual ~Tiles();
        static const unsigned Rows = 6;
        static const unsigned Cols = 15;
        const Tile& getTile(unsigned row, unsigned col) const { return array[row][col]; }
        Tile& getTile(unsigned row, unsigned col) { return array[row][col]; }
        static float getTopEdge() { return TopEdge - 8.0f; }
        static float getBottomEdge() { return TopEdge + Rows * 16; } // fix this
    private:
        static const float TopEdge;
        Tile array[Rows][Cols];
};

#endif // TILES_H
