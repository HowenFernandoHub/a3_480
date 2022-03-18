#ifndef TLB
#define TLB

#include <map>
#include <vector>
#include "math.h"
#define MEMORY_SPACE_SIZE 32


class tlb
{
    public:
        tlb(int vpnNumBits, int capacity);
        std::map<unsigned int, unsigned int> vpn2pfn;   // k: vpn, v: pfn
        std::vector<unsigned int> queue;    // always be size 10
        int capacity;
        unsigned int vpnMask;
        void setVpnMask(int vpnNumBits);
        bool hasMapping(unsigned int vpn);
        void insertMapping(unsigned int vpn, unsigned int frameNum);
};

#endif
