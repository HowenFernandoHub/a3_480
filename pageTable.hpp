/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: pageTable.hpp
 */



#ifndef PAGETABLE
#define PAGETABLE

#include "level.hpp"
#include "tlb.hpp"
#include "tracereader.h"

#define MEMORY_SPACE_SIZE 32





class PageTable
{
    public:
        // constructor
        PageTable(unsigned int, unsigned int*, int);

        // ptr to root level
        Level* rootLevel;
        
        // bit arrays and entryCountArr
        unsigned int *maskArr;
        unsigned int *shiftArr;
        unsigned int offsetMask;        // to append onto PFN
        unsigned int offsetShift;
        unsigned int *entryCountArr;

        // pageTable information
        unsigned int levelCount;
        unsigned int addressCount;
        unsigned int numBytesSize;
        unsigned int frameCount;
        unsigned int vpnNumBits;
        unsigned int pageSizeBytes;
        unsigned int currFrameNum;      // for the pageInsert function to know what frameNum to use

        // hit counts
        unsigned int countPageTableHits;
        unsigned int countTlbHits;

        unsigned int getOffset(unsigned int virtAddr);
        unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
        void pageInsert(Level* lvlPtr, unsigned int virtualAddress);
        Map* pageLookup(Level* lvlPtr, unsigned int virtualAddress);
    
        
        void fillMaskArr(unsigned int*, unsigned int*, unsigned int);
        void shiftMaskArr(unsigned int*, unsigned int*, unsigned int);
        void fillShiftArr(unsigned int*, unsigned int*, unsigned int);
        void fillEntryCountArr(unsigned int*, unsigned int*, unsigned int);
        void setOffsetMask(unsigned int);
        void setOffsetShift(unsigned int);
        unsigned int appendOffset(unsigned int frameNum, unsigned int virtualAddress);
        
};




#endif

