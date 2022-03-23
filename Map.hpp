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
        Map();
        void setFrameNum(int frameNumber);
        void setValid();
        unsigned int getFrameNum();     
        bool valid;   
    private:
        unsigned int frameNum;


};

#endif