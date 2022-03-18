#include "tlb.hpp"


tlb::tlb(int vpnNumBits, int capacity)
{
    this->capacity = capacity;
    setVpnMask(vpnNumBits);
}


void tlb::setVpnMask(int vpnNumBits)
{
    this->vpnMask = 0;
    for (int i = 0; i <= vpnNumBits; i++) {
        vpnMask += pow(2, MEMORY_SPACE_SIZE - i);
    }
}

bool tlb::hasMapping(unsigned int vpn)
{
    if (vpn2pfn.find(vpn) != vpn2pfn.end()) {
        return true;
    }
    return false;
}

void tlb::insertMapping(unsigned int vpn, unsigned int frameNum)
{
    // tlb not at capacity, safe to insert
    if (vpn2pfn.size() < capacity) {
        vpn2pfn[vpn] = frameNum;
    }
    else {
        // FIXME: finish what to do when at capacity
        // NATHAN does
    }
}