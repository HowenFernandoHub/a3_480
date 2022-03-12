#include "pageTable.hpp"

// constructor
PageTable::PageTable(int numLevels, int bitsInLevel[])
{
    levelCount = numLevels;
    entryCountArr = new int[numLevels];
    maskArr = new unsigned long int[numLevels];
    shiftArr = new int[numLevels];
    fillEntryCountArr(entryCountArr, bitsInLevel, levelCount);
    fillMaskArr(maskArr, bitsInLevel, numLevels);
    fillShiftArr(shiftArr, bitsInLevel, numLevels);
    Level zero(0, this);    // 'this' is pointer to this PageTable
}

void PageTable::fillEntryCountArr(int entryCountArr[], int bitsInLvl[], int numLvls)
{
    for (int i = 0; i < numLvls; i++) {
        entryCountArr[i] = pow(2, bitsInLvl[i]);
    }
}

void PageTable::fillMaskArr(unsigned long int maskArr[], int bitsInLvl[], int numLevels)
{
    unsigned long int mask;
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

unsigned long int PageTable::reverseBits(unsigned long int num)
{
    int count = ((__SIZEOF_LONG__ * 8) -1); 
    unsigned long int tmp = num;         //  Assign num to the tmp 
	     
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

void PageTable::shiftMaskArr(unsigned long int mask[], int bitsInLvl[], int numLevels)
{
    // shift mask arr
    int shiftAmmount = 0;
    for (int i = 1; i < numLevels; i++) {       // starts at 1 bc first mask doesn't need to shift
        shiftAmmount += bitsInLvl[i - 1];
        mask[i] = mask[i] >> shiftAmmount;
    }
}

void PageTable::fillShiftArr(int shiftArr[], int bitsInLvl[], int numLevels)
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
    unsigned int page;
    page = page & mask;
    page = page >> shift;
    return page;
}

void PageTable::pageInsert(PageTable *pagetable, unsigned int virtualAddress, unsigned int frame)
{
    // FIXME: Finish implementing insert page
}




