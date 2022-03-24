/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: level.cpp
 */


#include "pageTable.hpp"

// default constructor needed for nextLevel[]
Level::Level()
{
    currDepth = 0;
    pTable = NULL;
}

// constructor sets depth, pTable, nextLevel and mapPtr
Level::Level(int depth, PageTable* tablePtr)
{
    currDepth = depth;
    pTable = tablePtr;
    setNextLevel();
    setNextLevelNull();     // set zeroeth levels netLevel[] to all nulls
    mapPtr = nullptr;       // initialize to nullptr to avoid segFault.
}

// assigns nextLevel to an array of Level*
void Level::setNextLevel()
{
    // size of nextLevel = num possible levels at the currDepth
    nextLevel = new Level*[pTable->entryCountArr[currDepth]];
}

// ensures that all elements of nextLevel are set to Null
void Level::setNextLevelNull()
{
    for (int i = 0; i < pTable->entryCountArr[currDepth]; i++) {
        nextLevel[i] = nullptr;
    }
}

// use new operator to assign mapPtr a Map arr
void Level::setMapPtr()
{
    // size of mapPtr = num possible levels based on numBits in level
    this->mapPtr = new Map[pTable->entryCountArr[currDepth]];
}