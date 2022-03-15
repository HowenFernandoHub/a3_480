#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();
        void setFrame(int frameNum);
        void setValid();     
        bool valid;   
    private:
        int frame;


};

#endif