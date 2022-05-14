#ifndef Cell_h
#define Cell_h

class Cell
{
  public:
    Cell();
    Cell(byte stype);
    void init(byte stype = 0);
    bool isHit();
    bool isSafe();
  public:
    byte m_stype;
    bool m_shooted;
};
#endif
