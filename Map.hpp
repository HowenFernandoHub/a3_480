/**
 * Author: Howen Anthony Fernando, Nathan Schwedock
 * RedIDs: 822914112, 822910056
 * Class: CS480, Spring 2022
 * Assignment #3: Demand Paging with TLB
 * Filename: Map.cpp
 */


#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();  // default constructor
        void setFrameNum(int frameNumber);  // sets frameNum of Map
        void setValid();    // sets valid = true
        bool isValid();     // returns valid used in PageTable::paegLookup()
        unsigned int getFrameNum();       // returns frameNum
    private:
        unsigned int frameNum;      // physical frameNum mapped to vpn
        bool valid;     // true if Map has been initialized and given a frameNum


};

#endif