#include "Map.hpp"


Map::Map()
{
    frameNum = 0;
    valid = false;
}

void Map::setFrameNum(int frameNum)
{
    this->frameNum = frameNum;
}

void Map::setValid()
{
    if (!valid) valid = true;
}

unsigned int Map::getFrameNum()
{
    return frameNum;
}