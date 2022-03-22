#include "pageTable.hpp"

Level::Level()
{
    currDepth = 0;
    pTable = NULL;
}

Level::Level(int depth, PageTable* tablePtr)
{
    currDepth = depth;
    pTable = tablePtr;
    setNextLevel();
    setNextLevelNull();     // set zeroeth levels netLevel[] to all nulls
    mapPtr = nullptr;
}


void Level::setNextLevel()
{
    nextLevel = new Level*[pTable->entryCountArr[currDepth]];
}

void Level::setNextLevelNull()
{
    for (int i = 0; i < pTable->entryCountArr[currDepth]; i++) {
        nextLevel[i] = nullptr;
    }
}

void Level::setMapPtr()
{
    this->mapPtr = new Map[pTable->entryCountArr[currDepth]];
}

void Level::setCurrDepth(int newDepth)
{
    this->currDepth = newDepth;
}