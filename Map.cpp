#include "Map.hpp"


Map::Map()
{
    frame = 0;
    valid = false;
}

void Map::setFrame(int frameNum)
{
    frame = frameNum;
}

void Map::setValid()
{
    if (!valid) valid = true;
}

unsigned int Map::getFrame()
{
    return frame;
}