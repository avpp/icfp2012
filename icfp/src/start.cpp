#include "MineMap.h"

#include <stdio.h>

int main()
{
    MineMap mm;
    mm.ReadMap();
    mm.PrintMap();

    printf("check list of tramplaines (from main function):\n");
    list <pair <Point, Point> > l = mm.GetTramplainPairs();
    while (l.size())
    {
        Point p1 = l.front().first;
        Point p2 = l.front().second;
        l.pop_front();
        printf("[%d, %d] -> [%d, %d]\n",p1.x, p1.y, p2.x, p2.y);
    }
    return 0;
}
