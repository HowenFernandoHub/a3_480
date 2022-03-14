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
    // IDEA:
    //  - size of nextLevel determined by entryCount??? Ask Manan
    
    // prompt says "SET TO NULL. IS THIS OK?"
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
    // initialize mapPtr
    mapPtr = new Map[pTable->entryCountArr[currDepth]];
}

void Level::setCurrDepth(int newDepth)
{
    currDepth = newDepth;
}


void Level::pageInsert(Level* lvlPtr, unsigned int address, unsigned int frameNum)
{
    unsigned int mask = pTable->maskArr[lvlPtr->currDepth];
    unsigned int shift = pTable->shiftArr[lvlPtr->currDepth];
    unsigned int pageNum = pTable->virtualAddressToPageNum(address, mask, shift);

    if (lvlPtr->currDepth == pTable->levelCount)
    {
        // set valid and store frame num
    } else {

    }
}