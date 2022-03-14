#include "pageTable.hpp"

// constructor
PageTable::PageTable(int numLevels, int bitsInLevel[])
{
    levelCount = numLevels;
    entryCountArr = new int[numLevels];
    maskArr = new unsigned int[numLevels];
    shiftArr = new unsigned int[numLevels];
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
    // FIXME: Finish implementing insert page
    unsigned int mask = maskArr[lvlPtr->currDepth];
    unsigned int shift = shiftArr[lvlPtr->currDepth];
    unsigned int pageNum = virtualAddressToPageNum(virtualAddress, mask, shift);

    printf("PageNum%d: %0x\n", lvlPtr->currDepth, pageNum);


    // go here if lvlPtr is a leaf node
    if (lvlPtr->currDepth == levelCount - 1) {
        printf("Depth: %d\n", lvlPtr->currDepth);
        lvlPtr->setMapPtr();
        lvlPtr->mapPtr[pageNum].setFrame(currFrameNum);
        lvlPtr->mapPtr[pageNum].setValid();
        printf("Frame Num: %d\n", currFrameNum);
        currFrameNum++;
    } 
    // go here if lvlPtr is interior node
    else {
        // go here if pageNum at this level has already been set
        if (lvlPtr->nextLevel[pageNum] != NULL) {
            printf("Depth: %d\n", lvlPtr->currDepth);
            printf("Already had a pageNum set: %0x\n", pageNum);
            pageInsert(lvlPtr->nextLevel[pageNum], virtualAddress);
        }
        else {
            printf("Depth: %d\n", lvlPtr->currDepth);
            Level* newLevel = new Level(lvlPtr->currDepth + 1, this);
            newLevel->setNextLevel();
            newLevel->setNextLevelNull();
            lvlPtr->nextLevel[pageNum] = newLevel;
            pageInsert(newLevel, virtualAddress);
        }
    }
    
}

Map* PageTable::pageLookup(unsigned int virtAdd)
{
    // FIXME: Implement pageLookup
    // unsigned int vpnArr[levelCount];
    // for (int i = 0; i < levelCount; i++) {
    //     vpnArr[i] = virtualAddressToPageNum(virtAdd, maskArr[i], shiftArr[i]);
    // }
}




