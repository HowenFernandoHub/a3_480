

#ifndef PAGETABLE
#define PAGETABLE

#include "level.hpp"
#include "tlb.hpp"
#include "tracereader.h"




class PageTable
{
    public:
        PageTable(int, int*);
        Level* rootLevel;
        tlb* cache;
        int levelCount;
        unsigned int currFrameNum;      // for the pageInsert function to know what frameNum to use
        unsigned int *maskArr;
        unsigned int *shiftArr;
        int *entryCountArr;
        unsigned int getOffset(unsigned int virtAddr);
        unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
        void pageInsert(Level* lvlPtr, unsigned int virtualAddress);
        Map* pageLookup(Level* lvlPtr, unsigned int virtualAddress);
    
        unsigned int offsetMask;        // to append onto PFN
        unsigned int offsetShift;
        void fillMaskArr(unsigned int*, int*, int);
        void shiftMaskArr(unsigned int*, int*, int);
        unsigned int reverseBits(unsigned int);
        void fillShiftArr(unsigned int*, int*, int);
        void fillEntryCountArr(int*, int*, int);
        void setOffsetMask(int*);
        void setOffsetShift(int*);
        unsigned int appendOffset(unsigned int frameNum, unsigned int virtualAddress);
        
};




#endif

