

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
        long unsigned int *maskArr;        // make sure to use new
        int *shiftArr;       // make sure to use new
        int *entryCountArr;
        unsigned int virtualAddressToPageNum(unsigned int, unsigned int, unsigned int);
        void pageInsert(PageTable*, unsigned int, unsigned int);
    private:
        void fillMaskArr(unsigned long int*, int*, int);
        void shiftMaskArr(unsigned long int*, int*, int);
        unsigned long int reverseBits(unsigned long int);
        void fillShiftArr(int*, int*, int);
        void fillEntryCountArr(int*, int*, int);
        
};




#endif

