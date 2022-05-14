#ifndef Sea_h
#define Sea_h
#include "grid.h"
#include "square.h"

#define SW_RETRY 8
#define CNT_MAX 10
#define PIC_MAX 6

class Sea : public Grid
{
public:
  Sea(Square *r);
  bool moveCur(short stype);
  bool setShip(short stype);
  void draw(int phase);
  void shoot();
private:
  int m_dir;
  byte getSurvivorLongestShip();
  bool isPosible(Coord *p, int dir, int len);
  bool concentratedShoot();
  void serchingShoot();
  Coord *selectDamage(int *pdir);
  Coord *findDamage(bool *hits);
  int chkPosibleCell(Coord *p);
  int chkDir(Coord *p, bool *hits, int indx);
};
#endif
