#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
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


int main(int argc, char **argv)
{
    int nFlag;      // how many addresses to read in
    int cFlag = 0;  // cache capacity (default 0 = no TLB)
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

    PageTable pTable(numLevels, bitsInLevel);

    // printf("Offset Mask: %0x\n", pTable.offsetMask);
    for (int i = 0; i < numLevels; i++) {
        printf("Bits Level %d: %d\n", i, bitsInLevel[i]);
        printf("Entry Count %d: %d\n", i, pTable.entryCountArr[i]);
        printf("Mask %d: %0x\n", i, pTable.maskArr[i]);
        printf("Shift %d: %d\n", i, pTable.shiftArr[i]);
    }

    sleep(5);      // just so I can check the mask and shift arr vals before printing addresses


    // this might all move to readTraceFile() method
    FILE* traceF = readTraceFile(argc, argv);
    p2AddrTr trace;
    unsigned int virtAddr = 0;
    unsigned int physFrameNum = 0;
    unsigned int physAddr = 0;
    Map* frame;

    /********* TEST OF PAGE_INSERT AND PAGE_LOOKUP *********/
    virtAddr = 0xfefffec2;
    printf("VAddress: %x\n", virtAddr);
    pTable.pageInsert(pTable.rootLevel, virtAddr);
    frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
    physFrameNum = frame->getFrame();
    physFrameNum = physFrameNum << pTable.offsetShift;
    physAddr = physFrameNum | pTable.getOffset(virtAddr);
    printf("PAddress: %x\n\n", physAddr);

    virtAddr = 0xfe0123c2;
    printf("Address: %x\n", virtAddr);
    pTable.pageInsert(pTable.rootLevel, virtAddr);
    frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
    physFrameNum = frame->getFrame();
    physFrameNum = physFrameNum << pTable.offsetShift;
    physAddr = physFrameNum | pTable.getOffset(virtAddr);
    printf("PAddress: %x\n\n", physAddr);


    virtAddr = 0xfe0123c2;
    printf("Address: %x\n", virtAddr);
    pTable.pageLookup(pTable.rootLevel, virtAddr);
    printf("Offset: %x\n", pTable.getOffset(virtAddr));


    virtAddr = 0xfeffffc2;
    printf("Address: %x\n", virtAddr);
    pTable.pageLookup(pTable.rootLevel, virtAddr);

    virtAddr = 0xfef3ffc2;
    printf("Address: %x\n", virtAddr);
    pTable.pageLookup(pTable.rootLevel, virtAddr);
    /********************************************/

    while (!feof(traceF)) {
        if(NextAddress(traceF, &trace))     // traceF: File handle from fOpen
        {
            // virtAddr = trace.addr;
            // printf("Address: %0x\n", virtAddr);
            // Map* frameNum = pTable.pageLookup(pTable.rootLevel, virtAddr);
            // if (frameNum == NULL) {
            //     // go here if pageNum not found
            //     pTable.pageInsert(pTable.rootLevel, virtAddr);
            // }
        }
    }

    
}