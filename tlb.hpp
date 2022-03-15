#ifndef TLB
#define TLB

#include <map>

class tlb
{
    public:
        tlb(int vpnNumBits);
        std::map<unsigned int, unsigned int> vpn2pfn;
        unsigned int vpnMask;
        void setVpnMask(int vpnNumBits);
};

#endif
