#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "pageTable.hpp"
#include "output_mode_helpers.h"

#define MEMORY_SPACE_SIZE 32
#define DEFAULT_NUM_ADDRESSES -1
#define DEFAULT_CACHE_SIZE 0
#define DEFAULT_OUTPUT_MODE (char*)"summary"


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
                *nFlag = atoi(optarg);
                break;
            case 'c':
                *cFlag = atoi(optarg);
                // check if cFlag is valid
                if (*cFlag < 0) {
                    std::cerr << "Cache capacity must be a number, greater than or equal to 0" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
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
            std::cerr << "Level " << i - optind - 1 << " page table must be at least 1 bit" << std::endl;
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
    FILE* traceFile;

    // check that trace file can be opened
    char *traceFname = argv[optind];
    if ((traceFile = fopen(traceFname, "rb")) == NULL) {
        std::cerr << "Unable to open <<" << traceFname << ">>" << std::endl;
        exit(EXIT_FAILURE);
    }

    return traceFile;
}

void processNextAddress(FILE* traceFile, p2AddrTr* trace, PageTable* pTable, tlb* cache,
                        bool v2p, bool v2p_tlb, bool vpn2pfn, bool offset)
{
    unsigned int virtAddr = 0;
    unsigned int vpn = 0;
    unsigned int frameNum = 0;
    unsigned int physAddr = 0;
    bool tlbHit = false;
    bool pageTableHit = true;   // default true, conditional check if not
    Map* frame;

    virtAddr = trace->addr;     // assign virtAddr a value

    // go here if tlb has capacity > 0
    if (cache->usingTlb()) {
        
        vpn = virtAddr & cache->vpnMask;
        vpn = vpn >> (MEMORY_SPACE_SIZE - pTable->vpnNumBits);

        cache->updateQueue(vpn);    // update most recently used
        printf("we are using tlb\n");

        // go here if TLB hit
        if (cache->hasMapping(vpn)) {
            frameNum = cache->vpn2pfn[vpn];
            tlbHit = true;
            pTable->countTlbHits++;
        }
        // go here if TLB MISS
        else {
            frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
            if (frame == nullptr) {
                // go here if PageTable MISS
                pageTableHit = false;
                pTable->pageInsert(pTable->rootLevel, virtAddr);
                frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
                frameNum = frame->getFrameNum();
                cache->insertMapping(vpn, frameNum);    // update cache
                pTable->frameCount++;
            }
            else {
                // go here if PageTable HIT
                frameNum = frame->getFrameNum();
                cache->insertMapping(vpn, frameNum);    // update cache
                pTable->countPageTableHits++;
            }
            
        }
    }

    // go here if not using tlb
    else {
        frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
        if (frame == nullptr) {
            // go here if PageTable MISS
            pageTableHit = false;
            pTable->pageInsert(pTable->rootLevel, virtAddr);
            frame = pTable->pageLookup(pTable->rootLevel, virtAddr);
            frameNum = frame->getFrameNum();
            pTable->frameCount++;
        }
        else {
            // go here if PageTable HIT
            frameNum = frame->getFrameNum();
            pTable->countPageTableHits++;
        }
    }
    
    physAddr = pTable->appendOffset(frameNum, virtAddr);

    if (v2p) {  // virtual2PhysicalMode
        report_virtual2physical(virtAddr, physAddr);
    }
    else if (v2p_tlb) {
        report_v2pUsingTLB_PTwalk(virtAddr, physAddr, tlbHit, pageTableHit);
    }
    else if (vpn2pfn) {
        unsigned int pages[pTable->levelCount];
        for (int i = 0; i < pTable->levelCount; i++) {
            pages[i] = pTable->virtualAddressToPageNum(virtAddr, pTable->maskArr[i], pTable->shiftArr[i]);
        }
        report_pagemap(pTable->levelCount, pages, frameNum);
    }
    else if (offset) {
        hexnum(pTable->getOffset(virtAddr));
    }

    std::map<unsigned int, unsigned int>::iterator iter;
    // printf("CACHE: \n");
    for (iter = cache->vpn2pfn.begin(); iter != cache->vpn2pfn.end(); iter++) {
        // std::cout << iter->first << " => " << iter->second << '\n';
    }   

    // printf("QUEUE: \n");
    for (int i = 0; i < cache->recentPages.size(); i++) {
        // std::cout << cache->recentPages[i];
    }
}


void readAddresses(FILE* traceFile, p2AddrTr *trace, PageTable* pTable, tlb* cache, int numAddresses,
                    bool v2p, bool v2p_tlb, bool vpn2pfn, bool offset)
{
    // read all virtual addresses and insert into tree if not already present
    if (numAddresses < 0) {
        while (!feof(traceFile)) {
            if(NextAddress(traceFile, trace)) {     // traceFile: File handle from fOpen
                processNextAddress(traceFile, trace, pTable, cache, v2p, v2p_tlb, vpn2pfn, offset);
                pTable->addressCount++;
            }
        }
    } else {
        for (int i = 0; i < numAddresses; i++) {
            if(NextAddress(traceFile, trace)) {     // traceFile: File handle from fOpen
                processNextAddress(traceFile, trace, pTable, cache, v2p, v2p_tlb, vpn2pfn, offset);
                pTable->addressCount++;
            }
        }
    }
    
}


int main(int argc, char **argv)
{
    /**
     * 
     * NEXT TO DO
     *  - IMPLEMENT TLB
     *  - CACHE REPLACEMENT (Nathan)
     *  - OUTPUT MODES (Junior)
     * 
     * IMPLEMENT LEVEL (Junior) COMPLETE
     * 
     * IMPLEMENT PAGE TABLE (Nathan does .cpp) COMPLETE!
     * 
     * IMPLEMENT MAP COMPLETE
     * 
     * INITIALIZE PAGE TABLE LVL 0 COMPLETE
     * 
     */

    int nFlag = DEFAULT_NUM_ADDRESSES;      // how many addresses to read in
    int cFlag = DEFAULT_CACHE_SIZE;         // cache capacity (default 0 = no TLB)
    char *oFlag = DEFAULT_OUTPUT_MODE;      // what type of output to show

    processCmdLnArgs(argc, argv, &nFlag, &cFlag, &oFlag);

    unsigned int numLevels = (argc - 1) - optind;
    unsigned int bitsInLevel[numLevels];
    int vpnNumBits = 0;

    for (int i = 0; i < numLevels; i++) {
        bitsInLevel[i] = atoi(argv[optind + i + 1]);
        vpnNumBits += bitsInLevel[i];
    }

    FILE* traceFile = readTraceFile(argc, argv);
    p2AddrTr trace;

    PageTable pTable(numLevels, bitsInLevel, vpnNumBits);
    tlb* cache = new tlb(vpnNumBits, cFlag);    


    /********* TEST OF PAGE_INSERT AND PAGE_LOOKUP *********/
    // unsigned int addArr [] = {0xfefffec2, 0xfe0123c2, 0xfefffec2, 0xfe0123c2};


    // deal with output mode
    if (strcmp(oFlag, "bitmasks") == 0) {
        report_bitmasks(numLevels, pTable.maskArr);
    } else if (strcmp(oFlag, "virtual2physical") == 0) {
        readAddresses(traceFile, &trace, &pTable, cache, nFlag, true, false, false, false);
    } else if (strcmp(oFlag, "v2p_tlb_pt") == 0) {
        readAddresses(traceFile, &trace, &pTable, cache, nFlag, false, true, false, false);
        // NEED TO FINISH THIS ONE TOO!
    } else if (strcmp(oFlag, "vpn2pfn") == 0) {
        readAddresses(traceFile, &trace, &pTable, cache, nFlag, false, false, true, false);
    } else if (strcmp(oFlag, "offset") == 0) {
        readAddresses(traceFile, &trace, &pTable, cache, nFlag, false, false, false, true);
    } else if (strcmp(oFlag, "summary") == 0) {
        readAddresses(traceFile, &trace, &pTable, cache, nFlag, false, false, false, false);
        report_summary(pTable.pageSizeBytes, pTable.countTlbHits,
                        pTable.countPageTableHits, pTable.addressCount, pTable.frameCount, 200);    // FIXME: FINISH THIS!
    } else {
        std::cout << "Invalid Output Mode" << std::endl;
        exit(EXIT_FAILURE);
    }

}