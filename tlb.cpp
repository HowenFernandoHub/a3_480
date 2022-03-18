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
    // tlb not at capacity, safe to insert
    if (vpn2pfn.size() < capacity) {
        vpn2pfn[vpn] = frameNum;
    }
    else {
        vpn2pfn.erase(recentPages.front());     // erase least recently used
        vpn2pfn[vpn] = frameNum;
    }
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
    
    if (recentPages.size() >= MAX_QUEUE_SIZE) {
        recentPages.pop_front();
    }
}