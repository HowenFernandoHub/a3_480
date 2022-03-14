
#ifndef LEVEL
#define LEVEL

#include <stdlib.h>
#include "Map.hpp"



class Level
{
    public:
        Level();
        Level(int, class PageTable*);
        Level** nextLevel;        // FIXME: use new in constructor
        Map** mapPtr;
        int currDepth;
        class PageTable* pTable;
        void setNextLevel();
        void setNextLevelNull();
        void setMapPtr();
        void setCurrDepth(int);

};




#endif