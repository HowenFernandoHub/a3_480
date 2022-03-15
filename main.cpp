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
     * NEXT TO DO
     *  - IMPLEMENT TLB
     * 
     * 
     * IMPLEMENT LEVEL (Junior) COMPLETE
     * 
     * 
     * IMPLEMENT PAGE TABLE (Nathan does .cpp) COMPLETE!
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
     * IMPLEMENT MAP COMPLETE
     * 
     * INITIALIZE PAGE TABLE LVL 0 COMPLETE
     *  - all nulls?
     */


    processCmdLnArgs(argc, argv, &nFlag, &cFlag, &oFlag);
    int numLevels = (argc - 1) - optind;
    int bitsInLevel[numLevels];
    int totNumBits = 0;
    for (int i = 0; i < numLevels; i++) {
        bitsInLevel[i] = atoi(argv[optind + i + 1]);
        totNumBits += bitsInLevel[i];
    }

    PageTable pTable(numLevels, bitsInLevel);
    tlb* cache = new tlb(totNumBits, cFlag);

    // for (int i = 0; i < numLevels; i++) {
    //     printf("Bits Level %d: %d\n", i, bitsInLevel[i]);
    //     printf("Entry Count %d: %d\n", i, pTable.entryCountArr[i]);
    //     printf("Mask %d: %0x\n", i, pTable.maskArr[i]);
    //     printf("Shift %d: %d\n\n", i, pTable.shiftArr[i]);
    // }

    sleep(2);      // just so I can check the mask and shift arr vals before printing addresses
    // exit(0);

    // this might all move to readTraceFile() method
    FILE* traceF = readTraceFile(argc, argv);
    p2AddrTr trace;
    unsigned int virtAddr = 0;
    unsigned int vpn = 0;
    unsigned int frameNum = 0;
    unsigned int physAddr = 0;
    Map* frame;

    /********* TEST OF PAGE_INSERT AND PAGE_LOOKUP *********/
    // unsigned int addArr [] = {0xfefffec2, 0xfe0123c2, 0xfefffec2, 0xfe0123c2};

    // for (int i = 0; i < 4; i ++) {
    //     virtAddr = addArr[i];
    //     printf("VAddress: %0x\n", virtAddr);
    //     vpn = virtAddr & cache->vpnMask;
    //     // go here if TLB hit
    //     if (cache->hasMapping(vpn)) {
    //         printf("Page in TLB!\n");
    //         frameNum = cache->vpn2pfn[vpn];
    //     }
    //     // go here if TLB MISS
    //     else {
    //         frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
    //         if (frame == NULL) {
    //             // go here if VPN not already in tree
    //             printf("Page not yet in pTable!\n");
    //             pTable.pageInsert(pTable.rootLevel, virtAddr);
    //             frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
    //         }
    //         frameNum = frame->getFrameNum();
    //         cache->vpn2pfn[vpn] = frameNum;     // update cache
    //     }
    //     physAddr = pTable.appendOffset(frameNum, virtAddr);
    //     printf("PAddress: %x\n\n", physAddr);
    // }

    // read all virtual addresses and insert into tree if not already present
    while (!feof(traceF)) {
        if(NextAddress(traceF, &trace))     // traceF: File handle from fOpen
        {
            virtAddr = trace.addr;
            printf("VAddress: %0x\n", virtAddr);
            vpn = virtAddr & cache->vpnMask;
            // go here if TLB hit
            if (cache->hasMapping(vpn)) {
                printf("Page in TLB!\n");
                frameNum = cache->vpn2pfn[vpn];
            }
            // go here if TLB MISS
            else {
                frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
                if (frame == NULL) {
                    // go here if VPN not already in tree
                    printf("Page not yet in pTable!\n");
                    pTable.pageInsert(pTable.rootLevel, virtAddr);
                    frame = pTable.pageLookup(pTable.rootLevel, virtAddr);
                }
                frameNum = frame->getFrameNum();
                cache->vpn2pfn[vpn] = frameNum;     // update cache
            }
            physAddr = pTable.appendOffset(frameNum, virtAddr);
            printf("PAddress: %x\n\n", physAddr);
        }
    }

    
}