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




class PageTable
{
    public:
        PageTable(unsigned int, unsigned int*, int);
        Level* rootLevel;
        unsigned int levelCount;
        unsigned int addressCount;
        unsigned int numBytes;
        unsigned int frameCount;
        unsigned int vpnNumBits;
        unsigned int pageSizeBytes;
        unsigned int countPageTableHits;
        unsigned int countTlbHits;
        unsigned int currFrameNum;      // for the pageInsert function to know what frameNum to use
        unsigned int *maskArr;
        unsigned int *shiftArr;
        unsigned int *entryCountArr;
        unsigned int getOffset(unsigned int virtAddr);
        unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
        void pageInsert(Level* lvlPtr, unsigned int virtualAddress);
        Map* pageLookup(Level* lvlPtr, unsigned int virtualAddress);
    
        unsigned int offsetMask;        // to append onto PFN
        unsigned int offsetShift;
        void fillMaskArr(unsigned int*, unsigned int*, unsigned int);
        void shiftMaskArr(unsigned int*, unsigned int*, unsigned int);
        unsigned int reverseBits(unsigned int);
        void fillShiftArr(unsigned int*, unsigned int*, unsigned int);
        void fillEntryCountArr(unsigned int*, unsigned int*, unsigned int);
        void setOffsetMask(unsigned int);
        void setOffsetShift(unsigned int);
        unsigned int appendOffset(unsigned int frameNum, unsigned int virtualAddress);
        
};




#endif

