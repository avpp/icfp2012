#ifndef POINT_H
#define POINT_H


struct Point
{
   int x, y;
   Point()
   {
       x = 0;
       y = 0;
   }
   Point (int X, int Y)
   {
       x=X;
       y=Y;
   }
};

#endif // POINT_H
