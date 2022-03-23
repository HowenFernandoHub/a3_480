/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: tlb.hpp
 */


#ifndef TLB
#define TLB

#include <map>
#include <deque>
#include "math.h"
#define MEMORY_SPACE_SIZE 32
#define MAX_QUEUE_SIZE 10


class tlb
{
    public:
        tlb(int vpnNumBits, int capacity);
        std::map<unsigned int, unsigned int> vpn2pfn;   // k: vpn, v: pfn
        std::deque<unsigned int> recentPages;    // always be size 10
        int capacity;
        bool usingTlb();
        unsigned int vpnMask;
        void setVpnMask(int vpnNumBits);
        bool hasMapping(unsigned int vpn);
        void insertMapping(unsigned int vpn, unsigned int frameNum);
        void updateQueue(unsigned int recentVpn);
        bool queueContains(unsigned int vpn);
        void eraseVpnFromQueue(unsigned int vpn);
};

#endif
