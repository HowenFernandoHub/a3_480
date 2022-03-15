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
}


void Level::setNextLevel()
{
    nextLevel = new Level*[pTable->entryCountArr[currDepth]];
}

void Level::setNextLevelNull()
{
    for (int i = 0; i < pTable->entryCountArr[currDepth]; i++) {
        nextLevel[i] = NULL;
    }
}

void Level::setMapPtr()
{
    mapPtr = new Map[pTable->entryCountArr[currDepth]];
}

void Level::setCurrDepth(int newDepth)
{
    currDepth = newDepth;
}