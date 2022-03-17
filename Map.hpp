#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();
        void setFrameNum(int frameNum);
        void setValid();
        unsigned int getFrameNum();     
        bool valid;   
    private:
        unsigned int frameNum;


};

#endif