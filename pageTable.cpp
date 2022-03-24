/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: main.cpp
 */


#include "pageTable.hpp"

// constructor
PageTable::PageTable(unsigned int numLevels, unsigned int bitsInLevel[], int vpnNumBits)
{    
    addressCount = 0;
    frameCount = 0;
    numBytesSize = 0;
    this->vpnNumBits = vpnNumBits;
    pageSizeBytes = (unsigned int)pow(2, (MEMORY_SPACE_SIZE - vpnNumBits));     // 2^(bits in offset)
    countTlbHits = 0;
    countPageTableHits = 0;

    levelCount = numLevels;
    entryCountArr = new unsigned int[numLevels];
    maskArr = new unsigned int[numLevels];
    shiftArr = new unsigned int[numLevels];
    setOffsetMask(vpnNumBits);
    setOffsetShift(vpnNumBits);
    currFrameNum = 0;
    fillEntryCountArr(entryCountArr, bitsInLevel, levelCount);
    fillMaskArr(maskArr, bitsInLevel, numLevels);
    fillShiftArr(shiftArr, bitsInLevel, numLevels);
    rootLevel = new Level(0, this);        // 'this' is pointer to this PageTable
    numBytesSize += sizeof(Level) /** entryCountArr[0]*/;       // incrementing numBytesSize by size of Level * number of possible Levels in nextLevelArr
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
        for (int j = 0; j <= bitsInLvl[i]; j++) {
            mask += (unsigned int)pow(2, (MEMORY_SPACE_SIZE - j));
        }
        maskArr[i] = mask;
    }
    shiftMaskArr(maskArr, bitsInLvl, numLevels);
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
            numBytesSize += sizeof(Map) * entryCountArr[lvlPtr->currDepth];
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
            numBytesSize += sizeof(Level) * entryCountArr[lvlPtr->currDepth];
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
            return nullptr;
        }
        // go here if map invalid
        if (!lvlPtr->mapPtr[pageNum].isValid()) {
            return nullptr;
        }
        // returns this if page hit
        return &(lvlPtr->mapPtr[pageNum]);
    } 
    // go here if lvlPtr is interior node
    if (lvlPtr->nextLevel[pageNum] == nullptr) {
        return nullptr;
    }

    pageLookup(lvlPtr->nextLevel[pageNum], virtualAddress);     // recursion to next level
    
}


unsigned int PageTable::appendOffset(unsigned int frameNum, unsigned int virtualAddress)
{
    unsigned int physicalAddr = frameNum << offsetShift;
    physicalAddr = physicalAddr | getOffset(virtualAddress);
    return physicalAddr;
}





