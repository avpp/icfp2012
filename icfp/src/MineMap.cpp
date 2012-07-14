#include "MineMap.h"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <queue>

int getOneChar()
{
    return getchar();
}

int MineMap::META_DEFAULT[MineMap::META_SIZE] = {0, 0, 10};
const char* MineMap::META_NAME[MineMap::META_SIZE] = {"Water", "Flooding", "Waterproof"};

MineMap::MineMap() : m_width(m_Size.x), m_height(m_Size.y),
m_Water(m_metadata[0]), m_Flooding(m_metadata[1]), m_Waterproof(m_metadata[2])
{

    //ctor
    m_Map = NULL;
    m_Size.x = 0;
    m_Size.y = 0;
    m_Lift = m_Size;
    m_Robot = m_Size;
    for (int i = 0; i < META_SIZE; i++)
        m_metadata[i] = META_DEFAULT[i];
}

MineMap::~MineMap()
{
    if (m_height > 0)
    {
        if (m_width > 0)
            for (int i = 0; i < m_height; i++)
                delete [] m_Map[i];
        delete m_Map;
    }

    //dtor
}

void MineMap::ReadMap()
{
    using namespace std;
    queue<char> firstLine;
    char ch;
    while (((ch = getOneChar()) != EOF) && (ch != '\n'))
        firstLine.push(ch);
    int &width = m_width;
    int &height = m_height;
    width = firstLine.size();
    height = 1;
    queue<char*> map;
    char *tmp = new char[width];
    for (int i = 0; i < width; i++)
    {
        tmp[i] = firstLine.front();
        firstLine.pop();
    }
    map.push(tmp);
    ch = getOneChar();
    while ((ch != EOF) && (ch != '\n'))
    {
        tmp = new char [width];
        for (int i = 0; i < width; i++, ch = getOneChar())
        {
            tmp[i] = ch;
            switch (ch)
            {
                case 'R' : m_Robot = Point(i, height); break;
                case 'L' : m_Lift = Point(i, height); break;
                case '\\' : m_Lambdas.push_back(Point(i, height)); break;
            }
        }
        ch = getOneChar();
        map.push(tmp);
        height++;
    }

    m_Map = new char* [height];
    for (int i = 0; i < height; i++)
    {
        m_Map[i] = map.front();
        map.pop();
    }

    int result;
    char* buffer = new char[100];
    buffer[99] = 0;
    int value = 0;
    do
    {
        result = scanf("%99s %d", buffer, &value);
        if (result > 0)
        {
            for (int i = 0; i < META_SIZE; i++)
            {
                if (!strcmp(META_NAME[i], buffer))
                {
                    m_metadata[i] = value;
                    break;
                }
            }
        }

    } while (result > 0);
    delete [] buffer;
}

void MineMap::PrintMap()
{
    int water_start = m_height - m_Flooding;
    printf("Map %d*%d:\n", m_width, m_height);
    for (int i = 0; i < m_height; i++)
    {
        printf("  ");
        if (i >= water_start)
            printf("~");
        else
            printf(" ");
        for (int j = 0; j < m_width; j++)
            printf("%c", m_Map[i][j]);
        if (i >= water_start)
            printf("~");
        printf("\n");
    }
    printf("metadata:\n");
    for (int i = 0; i < META_SIZE; i++)
    {
        printf("  %s = %d\n", META_NAME[i], m_metadata[i]);
    }
    printf("another information:\n");
    printf("  Robot at [%d, %d]\n", m_Robot.x, m_Robot.y);
    printf("  Lift at [%d, %d]\n", m_Lift.x, m_Lift.y);
    printf("  Lambdas at:\n");
    if (m_Lambdas.size() == 0)
        printf("    no lambdas.\n");
    for (unsigned int i = 0; i < m_Lambdas.size(); i++)
        printf("    [%d, %d]\n", m_Lambdas.at(i).x, m_Lambdas.at(i).y);
    printf("It is all\n");
}

bool charInString(char c, char* str)
{
    int i = 0;
    while (str[i])
        if (c == str[i])
            return true;
        else
            i++;
    return false;
}

/*
ѕолучение списка точек около curPoint
добавл€ютс€ только те, которые удовлетвор€ют функции func, если она не NULL
и те, которые вход€т в строчку mask, если include = true или не вход€т иначе
например
GetListOfPoint(list, Point(2,2), NULL, "*#", true);
вернЄт все камни и стены около 2,2
GetListOfPoint(list, Point(2,2), NULL, "*#", false);
вернЄт все не камни и не стены
по идеи можно и так:
GetListOfPoint(list, Point(2,2), NULL, (char*)(MapValue[]){rock, wall}, true);
*/
void MineMap::GetListOfPoint(list<Point>& outList, Point curPoint, TCheckFunction func, char * mask, bool include)
{
    Point lt = Point((curPoint.x == 0)?curPoint.x:curPoint.x-1, (curPoint.y == 0)?curPoint.y:curPoint.y-1);
    Point br = Point((curPoint.x == m_width - 1)?curPoint.x:curPoint.x+1, (curPoint.y == m_height - 1)?curPoint.y:curPoint.y+1);
    for (int i = lt.y; i <= br.y; i++)
        for (int j = lt.x; j < br.x; j++)
        {
            char ch = m_Map[lt.y][lt.x];
            bool inMask = charInString(ch, mask);
            bool MaskCheck = (include)?inMask:!inMask;
            if (MaskCheck && (func == NULL || func(m_Map, Point(j,i))))
            {
                outList.push_back(Point(j, i));
            }
        }
}

bool MineMap::MoveRobot(Direction direction)
{
    bool result = false;

    return result;
}
