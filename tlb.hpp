#ifndef TLB
#define TLB

#include <map>
#include "math.h"
#define MEMORY_SPACE_SIZE 32


class tlb
{
    public:
        tlb(int vpnNumBits, int capacity);
        std::map<unsigned int, unsigned int> vpn2pfn;
        int capacity;
        unsigned int vpnMask;
        void setVpnMask(int vpnNumBits);
        bool hasMapping(unsigned int vpn);
        void insertMapping(unsigned int vpn, unsigned int frameNum);
};

#endif
