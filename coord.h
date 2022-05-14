#ifndef Coord_h
#define Coord_h

class Coord
{
  public:
    Coord();
    Coord(int x, int y);
    Coord(Coord *p);
    void clear(int val = 0);
    bool equal(Coord *p);
    void copy(Coord *p);
    void next(int dir);
    void back(int dir);
    void moveBy4Dir(int dir);
  public:
    int m_x;
    int m_y;
};
#endif
