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
    // these conditional control statements will need testing
    if (currDepth != pTable->levelCount) {
        setNextLevel();
    } else {
        setMapPtr();
    }
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
}

void Level::setCurrDepth(int newDepth) {
    currDepth = newDepth;
}