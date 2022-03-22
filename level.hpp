
#ifndef LEVEL
#define LEVEL

#include <stdlib.h>
#include "Map.hpp"
#include <math.h>

class PageTable;

class Level
{
    public:
        Level();
        Level(int, PageTable*);
        Level** nextLevel;        // FIXME: use new in constructor
        Map* mapPtr;
        unsigned int currDepth;
        PageTable* pTable;
        void setNextLevel();
        void setNextLevelNull();
        void setMapPtr();
        void setCurrDepth(int);

};




#endif