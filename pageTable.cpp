#include "pageTable.hpp"


// constructor
PageTable::PageTable(int numLevels, int bitsInLevel[])
{
    int totNumBits = 0;

    for (int i = 0; i < numLevels; i++) {
        totNumBits += bitsInLevel[i];
    }

    cache = new tlb(totNumBits);

    printf("VPN Mask: %x\n", cache->vpnMask);
    levelCount = numLevels;
    entryCountArr = new int[numLevels];
    maskArr = new unsigned int[numLevels];
    shiftArr = new unsigned int[numLevels];
    setOffsetMask(bitsInLevel);
    setOffsetShift(bitsInLevel);
    currFrameNum = 0;
    fillEntryCountArr(entryCountArr, bitsInLevel, levelCount);
    fillMaskArr(maskArr, bitsInLevel, numLevels);
    fillShiftArr(shiftArr, bitsInLevel, numLevels);
    rootLevel = new Level(0, this);        // 'this' is pointer to this PageTable
    rootLevel->setNextLevel();
    rootLevel->setNextLevelNull();    // set zeroeth levels netLevel[] to all nulls
}

void PageTable::fillEntryCountArr(int entryCountArr[], int bitsInLvl[], int numLvls)
{
    for (int i = 0; i < numLvls; i++) {
        entryCountArr[i] = pow(2, bitsInLvl[i]);
    }
}

void PageTable::fillMaskArr(unsigned int maskArr[], int bitsInLvl[], int numLevels)
{
    unsigned int mask;
    for (int i = 0; i < numLevels; i++) {
        mask = 0;
        for (int j = 0; j < bitsInLvl[i]; j++) {
            mask += pow(2, j);
        }
        mask = reverseBits(mask);
        maskArr[i] = mask;
    }
    shiftMaskArr(maskArr, bitsInLvl, numLevels);
}

unsigned int PageTable::reverseBits(unsigned int num)
{
    int count = ((__SIZEOF_INT__ * 8) -1); 
    unsigned int tmp = num;         //  Assign num to the tmp 
	     
    num >>= 1; // shift num because LSB already assigned to tmp
    
    while(num)
    {
       tmp <<= 1;  //shift the tmp because alread have the LSB of num  
       tmp |= num & 1; // putting the set bits of num
       num >>= 1; 
       count--;
    }
    
    tmp <<= count; //when num become zero shift tmp from the remaining counts
    return tmp;
}

void PageTable::shiftMaskArr(unsigned int mask[], int bitsInLvl[], int numLevels)
{
    // shift mask arr
    int shiftAmmount = 0;
    for (int i = 1; i < numLevels; i++) {       // starts at 1 bc first mask doesn't need to shift
        shiftAmmount += bitsInLvl[i - 1];
        mask[i] = mask[i] >> shiftAmmount;
    }
}

void PageTable::fillShiftArr(unsigned int shiftArr[], int bitsInLvl[], int numLevels)
{
    int shift = MEMORY_SPACE_SIZE;
    for (int i = 0; i < numLevels; i++) {
        shift = shift - bitsInLvl[i];
        shiftArr[i] = shift;
    }
}

void PageTable::setOffsetMask(int bitsInLvl[])
{
    this->offsetMask = 0;
    int numBitsOffset = MEMORY_SPACE_SIZE;
    for (int i = 0; i < levelCount; i++) {
        numBitsOffset -= bitsInLvl[i];
    }

    for (int i = 0; i < numBitsOffset; i++) {
        this->offsetMask += pow(2, i);
    }
}

void PageTable::setOffsetShift(int bitsInLvl[])
{
    this->offsetShift = MEMORY_SPACE_SIZE;
    for (int i = 0; i < levelCount; i++) {
        this->offsetShift -= bitsInLvl[i];
    }
}

unsigned int PageTable::getOffset(unsigned int virtAddr)
{
    return virtAddr & offsetMask;
}

unsigned int PageTable::virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
    // FIXME: Needs testing
    unsigned int pageNum;
    pageNum = virtualAddress & mask;
    pageNum = pageNum >> shift;
    return pageNum;
}

void PageTable::pageInsert(Level* lvlPtr, unsigned int virtualAddress)
{
    unsigned int mask = maskArr[lvlPtr->currDepth];
    unsigned int shift = shiftArr[lvlPtr->currDepth];
    unsigned int pageNum = virtualAddressToPageNum(virtualAddress, mask, shift);

    printf("PageNum%d: %0x\n", lvlPtr->currDepth, pageNum);

    // go here if lvlPtr is a leaf node
    if (lvlPtr->currDepth == levelCount - 1) {
        lvlPtr->setMapPtr();
        lvlPtr->mapPtr[pageNum].setFrameNum(currFrameNum);
        lvlPtr->mapPtr[pageNum].setValid();
        printf("Frame Num: %d\n", currFrameNum);
        currFrameNum++;
    } 
    // go here if lvlPtr is interior node
    else {
        // go here if pageNum at this level has already been set
        if (lvlPtr->nextLevel[pageNum] != NULL) {
            printf("Already had a pageNum set: %0x\n", pageNum);
            pageInsert(lvlPtr->nextLevel[pageNum], virtualAddress);
        }
        else {
            Level* newLevel = new Level(lvlPtr->currDepth + 1, this);
            newLevel->setNextLevel();
            newLevel->setNextLevelNull();
            lvlPtr->nextLevel[pageNum] = newLevel;
            pageInsert(newLevel, virtualAddress);
        }
    }
}

Map* PageTable::pageLookup(Level* lvlPtr, unsigned int virtualAddress)
{
    unsigned int mask = maskArr[lvlPtr->currDepth];
    unsigned int shift = shiftArr[lvlPtr->currDepth];
    unsigned int pageNum = virtualAddressToPageNum(virtualAddress, mask, shift);

    // go here if lvlPtr is a leaf node
    if (lvlPtr->currDepth == levelCount - 1) {
        // go here if mapPtr not set
        if (lvlPtr->mapPtr == NULL) {
            printf("MAP NOT FOUND\n");
            return NULL;
        }
        // go here if map invalid
        if (!lvlPtr->mapPtr[pageNum].valid) {
            printf("MAP NOT FOUND\n");
            return NULL;
        }
        // page found!
        printf("MAP FOUND!!!\n");
        return &(lvlPtr->mapPtr[pageNum]);
    } 
    // go here if lvlPtr is interior node
    if (lvlPtr->nextLevel[pageNum] == NULL) {
        printf("PAGE %d NOT FOUND\n", lvlPtr->currDepth);
        return NULL;
    }

    pageLookup(lvlPtr->nextLevel[pageNum], virtualAddress);     // recursion to next level
    
}


unsigned int PageTable::appendOffset(unsigned int frameNum, unsigned int virtualAddress)
{
    unsigned int physicalAddr = frameNum << offsetShift;
    physicalAddr = physicalAddr | getOffset(virtualAddress);
    return physicalAddr;
}





