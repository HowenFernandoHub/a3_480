
#ifndef LEVEL
#define LEVEL

#include <stdlib.h>



class Level
{
    public:
        Level(int, class PageTable*);
        Level* nextLevel;        // FIXME: use new in constructor
        int currDepth;
        class PageTable* pTable;
        




};




#endif