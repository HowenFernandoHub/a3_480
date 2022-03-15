#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();
        void setFrame(int frameNum);
        void setValid();
        unsigned int getFrame();     
        bool valid;   
    private:
        unsigned int frame;


};

#endif