#include "tlb.hpp"
#include <stack>



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
    // if tlb at capacity need to erase the least recent used address
    if (vpn2pfn.size() >= capacity) {
        vpn2pfn.erase(recentPages.front());     // erase least recently used
        recentPages.pop_front();
    }
    
    vpn2pfn[vpn] = frameNum;
}

bool tlb::queueContains(unsigned int vpn)
{
    for (int i = 0; i < recentPages.size(); i++) {
        if (recentPages[i] == vpn) return true;
    }
    return false;
}

void tlb::eraseVpnFromQueue(unsigned int vpn)
{
    for (int i = 0; i < recentPages.size(); i++) {
        if (recentPages[i] == vpn) {
            recentPages.erase(recentPages.begin() + i);
        }
    }
}

void tlb::updateQueue(unsigned int recentVpn)
{
    if (queueContains(recentVpn)) {     // if queue contains vpn update vpn to most recent
        eraseVpnFromQueue(recentVpn);
    }
    
    recentPages.push_back(recentVpn);
    
    if (recentPages.size() > MAX_QUEUE_SIZE) {
        // printf("recent is maxed");
        if (hasMapping(recentPages.front())) {
            vpn2pfn.erase(recentPages.front());     // erase least recently used
        }
        recentPages.pop_front();
    }
}