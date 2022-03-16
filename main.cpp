#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include "pageTable.hpp"
#include "output_mode_helpers.h"

#define MEMORY_SPACE_SIZE 32
#define DEFAULT_CACHE_SIZE 0
#define DEFAULT_OUTPUT_MODE 'summary'


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

void readAdresses(FILE* traceFile, p2AddrTr trace, PageTable* pTable, tlb* cache)
{
    unsigned int virtAddr = 0;
    unsigned int vpn = 0;
    unsigned int frameNum = 0;
    unsigned int physAddr = 0;
    Map* frame;

    // read all virtual addresses and insert into tree if not already present
    while (!feof(traceFile)) {
        if(NextAddress(traceFile, &trace))     // traceFile: File handle from fOpen
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
                frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
                if (frame == NULL) {
                    // go here if VPN not already in tree
                    printf("Page not yet in pTable!\n");
                    pTable->pageInsert(pTable->rootLevel, virtAddr);
                    frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
                }
                frameNum = frame->getFrameNum();
                cache->insertMapping(vpn, frameNum);    // update cache
            }
            physAddr = pTable->appendOffset(frameNum, virtAddr);
            printf("PAddress: %x\n\n", physAddr);
        }
    }

}


FILE* readTraceFile(int argc, char *argv[])
{
    FILE* traceFile;

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
    int cFlag = DEFAULT_CACHE_SIZE;  // cache capacity (default 0 = no TLB)
    char *oFlag;

    /**
     * 
     * NEXT TO DO
     *  - IMPLEMENT TLB
     *  - CACHE REPLACEMENT (Nathan)
     *  - OUTPUT MODES (Junior)
     * 
     * 
     * IMPLEMENT LEVEL (Junior) COMPLETE
     * 
     * 
     * IMPLEMENT PAGE TABLE (Nathan does .cpp) COMPLETE!
     * 
     * IMPLEMENT MAP COMPLETE
     * 
     * INITIALIZE PAGE TABLE LVL 0 COMPLETE
     * 
     */


    processCmdLnArgs(argc, argv, &nFlag, &cFlag, &oFlag);

    int numLevels = (argc - 1) - optind;
    int bitsInLevel[numLevels];
    int totNumBits = 0;
    for (int i = 0; i < numLevels; i++) {
        bitsInLevel[i] = atoi(argv[optind + i + 1]);
        totNumBits += bitsInLevel[i];
    }

    FILE* traceFile = readTraceFile(argc, argv);
    p2AddrTr trace;

    PageTable pTable(numLevels, bitsInLevel);
    tlb* cache = new tlb(totNumBits, cFlag);    

    readAdresses(traceFile, trace, &pTable, cache);

    // for (int i = 0; i < numLevels; i++) {
    //     printf("Bits Level %d: %d\n", i, bitsInLevel[i]);
    //     printf("Entry Count %d: %d\n", i, pTable.entryCountArr[i]);
    //     printf("Mask %d: %0x\n", i, pTable.maskArr[i]);
    //     printf("Shift %d: %d\n\n", i, pTable.shiftArr[i]);
    // }


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

    // deal with output mode
    // switch (*oFlag) {
    //     case 'bitmasks':
    //         report_bitmasks(numLevels, pTable.maskArr);
    //         break;
    //     case 'virtual2physical':
    //         // report_virtual2physical();
    //         break;
    //     case 'v2p_tlb_pt':
    //         break;
    //     case 'vpn2pfn':
    //         break;
    //     case 'offset':
    //         break;
    //     case 'summary':
    //         break;
    //     default:
    //         std::cout << "Invalid Output Mode" << std::endl;
    //         exit(EXIT_FAILURE);
    // }

    

    
}