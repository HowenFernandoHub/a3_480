#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();
        void setFrame(int frameNum);
        void setValid();        
    private:
        int frame;
        bool valid;


};

#endif