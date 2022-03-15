#include "tlb.hpp"


tlb::tlb(int vpnNumBits)
{
    setVpnMask(vpnNumBits);
}


void tlb::setVpnMask(int vpnNumBits)
{
    this->vpnMask = 0;
    for (int i = 0; i <= vpnNumBits; i++) {
        vpnMask += pow(2, MEMORY_SPACE_SIZE - i);
    }
}