
#ifndef LEVEL
#define LEVEL

#include <stdlib.h>
#include "Map.hpp"

class PageTable;

class Level
{
    public:
        Level();
        Level(int, PageTable*);
        Level** nextLevel;        // FIXME: use new in constructor
        Map* mapPtr;
        int currDepth;
        PageTable* pTable;
        void setNextLevel();
        void setNextLevelNull();
        void setMapPtr();
        void setCurrDepth(int);

};




#endif