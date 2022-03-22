#include "Map.hpp"


Map::Map()
{
    frameNum = 0;
    valid = false;
}

void Map::setFrameNum(int frameNumber)
{
    this->frameNum = frameNumber;
}

void Map::setValid()
{
    this->valid = true;
}

unsigned int Map::getFrameNum()
{
    return frameNum;
}