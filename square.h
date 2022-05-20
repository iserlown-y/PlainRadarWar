#ifndef Square_h
#define Square_h
#include "Coord.h"
class Square
{
  public:
    int m_x;
    int m_y;
    int m_x2;
    int m_y2;
    int m_h;
    int m_w;
  public:
    Square(int x, int y, int w, int h);
    Square(Coord *ps, Coord *pe);
    void drawRect();
    void drawInRect();
    void centerPoint();
    void fillInRect();
    void fillCircle();
    void drawX();
  private:
    Coord *getCenter();
    int getRadius();   
};
#endif
