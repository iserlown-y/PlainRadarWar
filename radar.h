#ifndef Radar_h
#define Radar_h
#include "grid.h"
#include "square.h"
class Radar : public Grid
{
public:
  Radar(Square *r);
  void draw(int phase);
  bool shoot();
  void setShips();
};
#endif
