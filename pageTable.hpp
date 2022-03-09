

#ifndef PAGETABLE
#define PAGETABLE

#include "level.hpp"
#include "tracereader.h"


class PageTable
{
    public:
        PageTable(int lvCount = 1);
        Level* rootLevel;
        int levelCount;
        long unsigned int *bitMask;        // make sure to use new
        long unsigned int *bitShift;       // make sure to use new
        int *entryCount;
        unsigned int virtualAddressToPageNum(unsigned int, unsigned int, unsigned int);

};




#endif

