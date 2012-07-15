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
    for (int i = 0; i < TRAMPOLAINES_AMOUNT; i++)
    {
        m_startTrampolines[i] = m_endTrampolines[i] = NULL;
        m_tramplinesAcces[i] = -1;
    }
    m_tramplanes = NULL;
    m_tramplanesCount = 0;
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

    for (int i = 0; i < TRAMPOLAINES_AMOUNT; i++)
    {
        if (m_startTrampolines[i])
            delete m_startTrampolines[i];
        if (m_endTrampolines[i])
            delete m_endTrampolines[i];
    }
    if (m_tramplanes)
        delete [] m_tramplanes;
    //dtor
}

/*
��������� ���������� � ����������, �������, ����� � ������.
*/
void MineMap::fillMapCache(char ch, int x, int y)
{
    if (ch >= START_TRAMPLAINE_CHAR && ch < START_TRAMPLAINE_CHAR + TRAMPOLAINES_AMOUNT)
    {
        m_startTrampolines[ch - START_TRAMPLAINE_CHAR] = new Point(x, y);
    }
    else if (ch >= START_TRAGET_CHAR && ch < START_TRAGET_CHAR + TRAMPOLAINES_AMOUNT)
    {
        m_endTrampolines[ch - START_TRAGET_CHAR] = new Point(x, y);
    }
    else switch (ch)
    {
        case 'R' : m_Robot = Point(x, y); break;
        case 'L' : m_Lift = Point(x, y); break;
        case '\\' : m_Lambdas.push_back(Point(x, y)); break;
    }
}

/*
������ �����
*/
void MineMap::ReadMap()
{
    using namespace std;

    /* ������ ������ ������� � �����-�������. ���������� ��� ����, ��� �� ������ ������ ������*/
    queue<char> firstLine;
    char ch;
    int cnt = 0;
    while (((ch = getOneChar()) != EOF) && (ch != '\n'))
    {
        firstLine.push(ch);
        fillMapCache(ch, cnt, 0);
        cnt++;
    }
    /* ������ �����-������� �����, ��������� �� �������*/
    int &width = m_width;
    int &height = m_height;
    width = firstLine.size();
    height = 1;
    queue<char*> map;
    /* �������� ���� ������ �������*/
    char *tmp = new char[width];
    for (int i = 0; i < width; i++)
    {
        tmp[i] = firstLine.front();
        firstLine.pop();
    }
    map.push(tmp);
    /* ������ �� ����� ����� ��� ���� �� ���������� �������, ������������ � '\n' */
    ch = getOneChar();
    while ((ch != EOF) && (ch != '\n'))
    {
        /* �������� ��� ������� � ���������� � � �������� ����� (��� ������ ������ ���� ������)*/
        tmp = new char [width];
        for (int i = 0; i < width; i++, ch = getOneChar())
        {
            tmp[i] = ch;
            fillMapCache(ch, i, height);
        }
        /* ������ ��������� ������ */
        ch = getOneChar();
        /* �������� ����� ��������� ������ � �����-����� */
        map.push(tmp);
        height++;
    }

    /* ������������ �� �����-�����(�������) � ���������� ������ (������ �������)*/
    m_Map = new char* [height];
    for (int i = 0; i < height; i++)
    {
        m_Map[i] = map.front();
        map.pop();
    }
    /* ������������, ������� ���� ������� ���������� � ������ ������ � ����������*/
    for (int i = 0; i < TRAMPOLAINES_AMOUNT; i++)
        if (m_startTrampolines[i])
            m_tramplanesCount++;
    if (m_tramplanesCount)
        m_tramplanes = new unsigned int [m_tramplanesCount];
    int trampolaineNum = 0;

    /* ����� ����� ������ �������������� ���������� � �����*/
    int result;
    char* buffer = new char[100];
    buffer[99] = 0;
    int value = 0;
    do
    {
        /* ������ ������� �� ������� ��� ����������� ����������� ���� ����������*/
        result = scanf("%99s", buffer);
        if (result > 0)
        {
            /* ���� ���������� � "Trampoline", ������ ��� �������� � ������� ���������� ��� ����*/
            if (!strcmp("Trampoline", buffer))
            {
                char trmp[5] = "qwer";
                char *start = &trmp[0], *end = &trmp[2];
                /* ������ �� ������� " A targets 1"*/
                /* ������ ������ �������, ��� �� ������� 9 �������� " targets "*/
                result = scanf("%*c%c%*9c%c", start, end);
                /* ���������� � ������ ���������� ���������� ����� � ������� ���� ���������� */
                int startTramp = m_tramplanes[trampolaineNum++] = *start - START_TRAMPLAINE_CHAR;
                /* ������� ���������� � ������� ����� ��������� � ����, ��� ����� � ��������������� */
                /* ������� � ���������� ������ ��������� ������ ���������� ����� ���� */
                m_tramplinesAcces[startTramp] = *end - START_TRAGET_CHAR;
            }
            else
            /* ����� ��� ������ ����, �������� �� Water, Flooding, Waterproof*/
                for (int i = 0; i < META_SIZE; i++)
                {
                    if (!strcmp(META_NAME[i], buffer))
                    {
                        result = scanf("%d", &value);
                        m_metadata[i] = value;
                        break;
                    }
                }
        }
    /* �� ��� ������, ���� �� ��������� ����� ����� */
    } while (result > 0);
    delete [] buffer;
}

/*
������ ������� - ����� �� ����� ����� =)
��� ��������� ������ ������������ ����� (enum PrintStyle)
�� ��������� ��������� �� (PSFull)
*/
void MineMap::PrintMap(int style)
{
    if (style & PSMap)
    {
        /* ������� ����� � ������ */
        int water_start = m_height - m_Flooding;
        printf("Map %d*%d:\n", m_width, m_height);
        printf("  +");
        for (int i = 0; i < m_width; i++)
            printf("-");
        printf("+\n");
        for (int i = 0; i < m_height; i++)
        {
            printf("  ");
            /* ���� ������� ��������, �� ������ ������ "~" */
            if (i >= water_start)
                printf("~");
            else
                printf("|");
            /* ������� ���� ���������� �����*/
            for (int j = 0; j < m_width; j++)
                printf("%c", m_Map[i][j]);
            if (i >= water_start)
                printf("~");
            else
                printf("|");
            printf("\n");
        }
        /* ������ ������� �����*/
        printf("  +");
        for (int i = 0; i < m_width; i++)
            printf("-");
        printf("+\n");
    }
    if (style & PSMeta)
    {
        /* ����� ����������*/
        printf("metadata:\n");
        for (int i = 0; i < META_SIZE; i++)
        {
            printf("  %s = %d\n", META_NAME[i], m_metadata[i]);
        }
    }

    if (style & (PSRobot | PSLift))
    {
        /* ����� ���������� � ��������� ������ � ����� */
        printf("another information:\n");
        if (style & PSRobot)
            printf("  Robot at [%d, %d]\n", m_Robot.x, m_Robot.y);
        if (style & PSLift)
            printf("  Lift at [%d, %d] is %s\n", m_Lift.x, m_Lift.y, (m_Map[m_Lift.y][m_Lift.x] == 'O')?"open":"close");
    }
    if (style & (PSLambda | PSTramplaines))
    {
        /* ����� ���������� � ������������ ���� ����� � ���������� � ��� ������� */
        int i = 0;
        int lambdaCount = m_Lambdas.size();

        if (style & PSLambda)
            printf(" +----------------+");
        else
            printf("                  +");
        if (style & PSTramplaines)
            printf("-------------------------------------------+\n");
        else
            printf("\n");

        if (style & PSLambda)
            printf(" |%3d Lambdas at: |", lambdaCount);
        else
            printf("                  |");
        if (style & PSTramplaines)
            printf("    %3d Tramplanes:                        |\n", m_tramplanesCount);
        else
            printf("\n");

        if (style & PSLambda)
            printf(" +----------------+");
        else
            printf("                  +");
        if (style & PSTramplaines)
            printf("-------------------------------------------+\n");
        else
            printf("\n");

        while (((style & PSLambda) && i < lambdaCount) || ((style & PSTramplaines) && i < m_tramplanesCount))
        {
            if (style & PSLambda)
                printf(" |  ");
            else
                printf("    ");
            if (i < lambdaCount && (style & PSLambda))
                printf("[%3d, %3d]    ", m_Lambdas.at(i).x, m_Lambdas.at(i).y);
            else if (!lambdaCount && !i && (style & PSLambda))
                printf("no lambdas.   ");
            else
                printf("              ");
            printf("|    ");

            if (i < m_tramplanesCount && (style & PSTramplaines))
            {
                int start = m_tramplanes[i];
                int end = m_tramplinesAcces[m_tramplanes[i]];
                printf("from '%c' [%3d, %3d] to '%c' [%3d, %3d]  |\n",
                    START_TRAMPLAINE_CHAR + start, m_startTrampolines[start]->x, m_startTrampolines[start]->y,
                    START_TRAGET_CHAR + end, m_endTrampolines[end]->x, m_endTrampolines[end]->y);
            }
            else if (!m_tramplanesCount && !i && (style & PSTramplaines))
                printf("no tramplaines.                        |\n");
            else
                printf("                                       %c\n", (style & PSTramplaines)?'|':' ');
            i++;
        }

        if (style & PSLambda)
            printf(" +----------------+");
        else
            printf("                  +");
        if (style & PSTramplaines)
            printf("-------------------------------------------+\n");
        else
            printf("\n");
    }
    if (style)
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
��������� ������ ����� ����� curPoint
����������� ������ ��, ������� ������������� ������� func, ���� ��� �� NULL
� ��, ������� ������ � ������� mask, ���� include = true ��� �� ������ �����
��������
GetListOfPoint(list, Point(2,2), NULL, "*#", true);
����� ��� ����� � ����� ����� 2,2
GetListOfPoint(list, Point(2,2), NULL, "*#", false);
����� ��� �� ����� � �� �����
�� ���� ����� � ���:
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

/* ���������� ������ ���������� � ���� ��� �������� - ������� */
list<pair<Point, Point> > MineMap::GetTramplainPairs()
{
    list<pair<Point, Point> > answer;
    for (int i = 0; i < m_tramplanesCount; i++)
    {
        Point start = *m_startTrampolines[m_tramplanes[i]];
        Point end = *m_endTrampolines[m_tramplinesAcces[m_tramplanes[i]]];
        answer.push_back(pair<Point, Point>(start, end));

    }
    return answer;
}

bool MineMap::MoveRobot(Direction direction)
{
    bool result = false;

    return result;
}
