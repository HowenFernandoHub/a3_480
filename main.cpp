#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "pageTable.hpp"

#define MEMORY_SPACE_SIZE 32


void processCmdLnArgs(int argc, char *argv[], int *nFlag, int *cFlag, char **oFlag)
{
    // check that the minimum # of cmd-line args are given
    if (argc < 3)
    {
        std::cerr << "Error: Not enough command-line args given" << std::endl;
        std::cerr << "  \nExiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    int opt;

    // process optional flags
    // skips over if no optional flags
    while ((opt = getopt (argc, argv, "n:c:o:")) != -1)
    {
        switch (opt)
        {
            case 'n':
                // std::cout << "had p flag" << std::endl;
                // FIXME: Find way to implement DEFAULT
                *nFlag = atoi(optarg);
                break;
            case 'c':
                // std::cout << "had h flag" << std::endl;
                *cFlag = atoi(optarg);
                break;
            case 'o':
                // std::cout << "had n flag" << std::endl;
                *oFlag = optarg;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    // go here if only optional cmd-line args are given but not the mandatory ones
    if (optind > (argc - 2)) {
        std::cerr << "Error:\n  Gave optional cmd line args but not mandatory ones\n";
        std::cerr <<  "  Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // check bit options are viable
    int totBits = 0;
    int bitOption = 0;
    for (int i = optind + 1; i < argc; i++) {
        bitOption = atoi(argv[i]);
        totBits += bitOption;
        if (bitOption < 1) {
            std::cerr << "Level " << i << " page table must be at least 1 bit" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (totBits > 28) {
            std::cerr << "Too many bits used in page tables" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

}


FILE* readTraceFile(int argc, char *argv[])
{
    FILE *traceFile;

    // check that trace file can be opened
    char *traceFname = argv[optind];
    if ((traceFile = fopen(traceFname, "rb")) == NULL) {
        std::cerr << "Unable to open <<" << traceFname << ">>" << std::endl;
        exit(EXIT_FAILURE);
    }

    return traceFile;
}

unsigned long int ReverseTheBits(unsigned long int num)
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


void fillMaskArr(unsigned long int maskArr[], int bitsInLvl[], int numLevels)
{
    unsigned long int mask;
    for (int i = 0; i < numLevels; i++) {
        mask = 0;
        for (int j = 0; j < bitsInLvl[i]; j++) {
            mask += pow(2, j);
        }
        mask = ReverseTheBits(mask);
        maskArr[i] = mask;
    }
}


void fillShiftArr(int shiftArr[], int bitsInLvl[], int  numLevels)
{
    int shift = MEMORY_SPACE_SIZE;
    for (int i = 0; i < numLevels; i++) {
        shift = shift - bitsInLvl[i];
        shiftArr[i] = shift;
    }
}


int main(int argc, char **argv)
{
    int nFlag;
    int cFlag = 0;
    char *oFlag;

    /**
     * 
     * IMPLEMENT LEVEL (Junior)
     * 
     * 
     * IMPLEMENT PAGE TABLE (Nathan does .cpp)
     *  - levelCount
     *  - bitmask[i]
     *  - bitShift[i]
     *  - entryCount[i]
     *  - Level *nextLevel[]
     *  - unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
     *  - PageTable (Has pointer to 0 level page tree)
     *  - Level (struct/class)
     *      - curr depth
     *      - pointer to PageTable to access info
     *  - Map (struct/class)
     * 
     * IMPLEMENT MAP
     * 
     * INITIALIZE PAGE TABLE LVL 0
     *  - all nulls?
     */


    processCmdLnArgs(argc, argv, &nFlag, &cFlag, &oFlag);

    int numLevels = (argc - 1) - optind;

    int bitsInLevel[numLevels];

    for (int i = 0; i < numLevels; i++) {
        bitsInLevel[i] = atoi(argv[optind + i + 1]);
    }

    unsigned long int bitMask[numLevels];
    fillMaskArr(bitMask, bitsInLevel, numLevels);

    for (int i = 0; i < numLevels; i++) {
        printf("%0lx\n", bitMask[i]);
        printf("%0x\n", bitsInLevel[i]);
    }

    int shiftArr[numLevels];
    fillShiftArr(shiftArr, bitsInLevel, numLevels);

    for (int i = 0; i < numLevels; i++) {
        printf("%0d\n", shiftArr[i]);
        printf("%0x\n", bitsInLevel[i]);
    }

    exit(0);


    // this might all move to readTraceFile() method
    FILE* traceF = readTraceFile(argc, argv);
    p2AddrTr trace;
    unsigned long int virtAddr = 0;
    unsigned long int mask = 0xffff0000000;
    unsigned long int page;

    while (!feof(traceF)) {
        if(NextAddress(traceF, &trace))
        {
            virtAddr = trace.addr;
            page = virtAddr & mask;     // bit mask testing
            page = page >> 28;          // bit mask shifting
            printf("%0lx\n", page);
        }
    }

    
}