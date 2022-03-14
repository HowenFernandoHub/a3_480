#ifndef MAP
#define MAP

class Map 
{
    public:
        Map();
        Map(int frame, bool valid);
        void setFrame(int frameNum);
        void setValid();        
    private:
        int frame;
        bool valid;


};

#endif