#ifndef MINEMAP_H
#define MINEMAP_H

#include "Point.h"
#include <vector>

using namespace std;

enum Direction {up, down, left, right};

class MineMap
{
    public:
        MineMap();
        virtual ~MineMap();
        char** GetMap() { return m_Map; }
        Point GetSize() { return m_Size; }
        vector<Point> GetLambdas() { return m_Lambdas; }
        Point GetLift() { return m_Lift; }
        Point GetRobot() { return m_Robot; }
        int GetWidth() { return m_width; }
        int GetHeight() { return m_height; }
        void ReadMap();
        void PrintMap();

        bool MoveRobot(Direction direction);
    protected:
    private:
        char** m_Map;
        Point m_Size;
        vector<Point> m_Lambdas;
        Point m_Lift;
        Point m_Robot;
        int& m_width;
        int& m_height;
        int& m_Water;
        int& m_Flooding;
        int& m_Waterproof;

        static const int META_SIZE = 3;
        static const char* META_NAME[META_SIZE];// = {"Water", "Flooding", "Waterproof"};
        static int META_DEFAULT[META_SIZE];// = {0, 0, 10};
        int m_metadata[META_SIZE];

};

#endif // MINEMAP_H
