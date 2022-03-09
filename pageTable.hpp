

#ifndef PAGETABLE
#define PAGETABLE

#include "level.hpp"


class PageTable
{
    public:
        PageTable(int);
        int levelCount;
        char bitMask[0];        // FIXME: Me dumb can't figure out
        char bitShift[0];
        int entryCount[0];




};




#endif

