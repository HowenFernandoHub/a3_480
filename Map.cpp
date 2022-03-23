/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: Map.cpp
 */



#include "Map.hpp"

// default constructor
Map::Map()
{
    frameNum = 0;       // default set to 0 until updated
    valid = false;      // default set to false until updated
}

/**
 * @brief - sets private variable frameNum. Used in PageTable::pageInsert()
 * @param frameNumber - to be assigned to this->frameNum
 */
void Map::setFrameNum(int frameNumber)
{
    this->frameNum = frameNumber;
}

/**
 * @brief - sets valid to true. Used in PageTable::pageInsert()
 */
void Map::setValid()
{
    this->valid = true;
}

/**
 * @brief - returns private variable this->valid
 */
bool Map::isValid()
{
    return this->valid;
}

/**
 * @brief - returns private variable frameNum
 */
unsigned int Map::getFrameNum()
{
    return frameNum;
}