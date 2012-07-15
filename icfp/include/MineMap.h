#ifndef MINEMAP_H
#define MINEMAP_H

#include "Point.h"
#include <vector>
#include <list>
#include <map>
#include <stdio.h>

using namespace std;


typedef bool (*TCheckFunction)(char** map, Point curPoint);

enum MapValue {rock = '*', lambda = '\\', wall = '#', earth = '.',
               robot = 'R', liftClose = 'L', liftOpen = 'O', empty = ' ',
               beard = 'W', razor = '!'};
enum Direction {up, down, left, right};
enum PrintStyle {PSMap = 1<<0, PSMeta = 1<<1, PSRobot = 1<<2, PSLift = 1<<3, PSShort = ((1<<4) - 1),
                 PSLambda = 1<<4, PSTramplaines = 1<<5, PSBeards = 1<<6, PSRazors = 1<<7,
                 PSFull = ((1<<8) - 1) };

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
        void PrintMap(int style = PSFull);

        void GetListOfPoint(list<Point>& outList, Point curPoint, TCheckFunction func, char * mask = "\0", bool include=true);

        list<pair<Point, Point> > GetTramplainPairs();
        bool MoveRobot(Direction direction);

        void openFileForMap(char *fileName);
    protected:
    private:

        void fillMapCache(char ch, int x, int y);

        char** m_Map;
        Point m_Size;
        vector<Point> m_Lambdas;
        vector<Point> m_Beards;
        vector<Point> m_Razors;
        Point m_Lift;
        Point m_Robot;
        int& m_width;
        int& m_height;
        int& m_Water;
        int& m_Flooding;
        int& m_Waterproof;
        int& m_Growth;
        int& m_RazorAmount;

        static const int TRAMPOLAINES_AMOUNT = 9;
        static const char START_TRAMPLAINE_CHAR = 'A';
        static const char START_TRAGET_CHAR = '1';
        Point* m_startTrampolines[TRAMPOLAINES_AMOUNT];
        Point* m_endTrampolines[TRAMPOLAINES_AMOUNT];
        unsigned int m_tramplinesAcces[TRAMPOLAINES_AMOUNT];
        unsigned int* m_tramplanes;
        unsigned int m_tramplanesCount;

        static const int META_SIZE = 5;
        static const char* META_NAME[META_SIZE];
        static int META_DEFAULT[META_SIZE];
        int m_metadata[META_SIZE];

        FILE *m_inputFile;
        int getOneChar();

};

#endif // MINEMAP_H
