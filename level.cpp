#include "pageTable.hpp"

Level::Level(int depth=0, PageTable* tablePtr=NULL)
{
    currDepth = depth;
    pTable = tablePtr;
}


void Level::setNextLevel()
{

}