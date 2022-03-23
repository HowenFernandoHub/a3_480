/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: Map.cpp
 */



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