#include "pageTable.hpp"


// constructor
PageTable::PageTable(unsigned int numLevels, unsigned int bitsInLevel[])
{
    unsigned int totNumBits = 0;
    addressCount = 0;

    for (int i = 0; i < numLevels; i++) {
        totNumBits += bitsInLevel[i];
    }

    vpnNumBits = totNumBits;
    pageSizeBytes = (MEMORY_SPACE_SIZE - vpnNumBits) / 8;

    levelCount = numLevels;
    entryCountArr = new unsigned int[numLevels];
    maskArr = new unsigned int[numLevels];
    shiftArr = new unsigned int[numLevels];
    setOffsetMask(totNumBits);
    setOffsetShift(totNumBits);
    currFrameNum = 0;
    fillEntryCountArr(entryCountArr, bitsInLevel, levelCount);
    fillMaskArr(maskArr, bitsInLevel, numLevels);
    fillShiftArr(shiftArr, bitsInLevel, numLevels);
    rootLevel = new Level(0, this);        // 'this' is pointer to this PageTable
}

void PageTable::fillEntryCountArr(unsigned int entryCountArr[], unsigned int bitsInLvl[], unsigned int numLvls)
{
    for (int i = 0; i < numLvls; i++) {
        entryCountArr[i] = pow(2, bitsInLvl[i]);
    }
}

void PageTable::fillMaskArr(unsigned int maskArr[], unsigned int bitsInLvl[], unsigned int numLevels)
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

void PageTable::shiftMaskArr(unsigned int mask[], unsigned int bitsInLvl[], unsigned int numLevels)
{
    // shift mask arr
    int shiftAmmount = 0;
    for (int i = 1; i < numLevels; i++) {       // starts at 1 bc first mask doesn't need to shift
        shiftAmmount += bitsInLvl[i - 1];
        mask[i] = mask[i] >> shiftAmmount;
    }
}

void PageTable::fillShiftArr(unsigned int shiftArr[], unsigned int bitsInLvl[], unsigned int numLevels)
{
    int shift = MEMORY_SPACE_SIZE;
    for (int i = 0; i < numLevels; i++) {
        shift = shift - bitsInLvl[i];
        shiftArr[i] = shift;
    }
}

void PageTable::setOffsetMask(unsigned int vpnNumBits)
{
    this->offsetMask = 0;
    unsigned int numBitsOffset = MEMORY_SPACE_SIZE - vpnNumBits;

    for (int i = 0; i < numBitsOffset; i++) {
        this->offsetMask += pow(2, i);
    }
}

void PageTable::setOffsetShift(unsigned int vpnNumBits)
{
    this->offsetShift = MEMORY_SPACE_SIZE - vpnNumBits;
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

    // go here if lvlPtr is a leaf node
    if (lvlPtr->currDepth == levelCount - 1) {
        // go here if mapPtr array hasn't been instantiated
        if (lvlPtr->mapPtr == nullptr) {
            lvlPtr->setMapPtr();    // instantiate mapPtr
        }
        lvlPtr->mapPtr[pageNum].setFrameNum(currFrameNum);
        lvlPtr->mapPtr[pageNum].setValid();
        currFrameNum++;
    } 
    // go here if lvlPtr is interior node
    else {
        // go here if pageNum at this level has already been set
        if (lvlPtr->nextLevel[pageNum] != nullptr) {
            pageInsert(lvlPtr->nextLevel[pageNum], virtualAddress);
        }
        else {
            Level* newLevel = new Level(lvlPtr->currDepth + 1, this);
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
        // // go here if mapPtr not set
        if (lvlPtr->mapPtr == nullptr) {
            return NULL;
        }
        // go here if map invalid
        if (!lvlPtr->mapPtr[pageNum].valid) {
            return NULL;
        }
        // returns this if page hit
        return &(lvlPtr->mapPtr[pageNum]);
    } 
    // go here if lvlPtr is interior node
    if (lvlPtr->nextLevel[pageNum] == NULL) {
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





