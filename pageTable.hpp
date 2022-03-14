

#ifndef PAGETABLE
#define PAGETABLE

#include "level.hpp"
#include "tracereader.h"
#include "math.h"

#define MEMORY_SPACE_SIZE 32



class PageTable
{
    public:
        PageTable(int, int*);
        Level* rootLevel;
        int levelCount;
        unsigned int currFrameNum;    // for the pageInsert function to know what frameNum to use
        unsigned int *maskArr;        // make sure to use new
        unsigned int *shiftArr;       // make sure to use new
        int *entryCountArr;
        unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
        void pageInsert(Level* lvlPtr, unsigned int virtualAddress);
        Map* pageLookup(unsigned int);
    private:
        void fillMaskArr(unsigned int*, int*, int);
        void shiftMaskArr(unsigned int*, int*, int);
        unsigned int reverseBits(unsigned int);
        void fillShiftArr(unsigned int*, int*, int);
        void fillEntryCountArr(int*, int*, int);
        
};




#endif

