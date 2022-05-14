#ifndef Grid_h
#define Grid_h

#include "globals.h"
#include "square.h"
#include "cell.h"

class Grid
{
public:
  Grid(Square *r);
  ~Grid();
  void clear();
  void moveCur();
  bool isAnnihilate();

protected:
  Coord *m_cur; // カーソル座標
  byte m_frame; // ヒット時点滅表示用カウンタ
  Coord *m_shoot; // 攻撃座標
  Cell m_cells[C_X_NUM][C_Y_NUM];

protected:
  void draw();
  bool shoot(Coord *p, bool bcpu = false);
  bool setShip(short stype, Coord *p, int dir);
  Cell *getCell(Coord *p);
  bool isSunken(Coord *p);
  Square *getCellSquare(Coord *p);
  bool isEnableSet(int len, Coord *p, int dir);
  void drawShips();
  bool chkNearCell(byte stype, Coord *p, int dx, int dy, bool *bHit = NULL);
  
private:
  void init();
  Square *square;
  void paintCell(Coord *p);
  void paintMiss(Square *r);
  void paintHit(Square *r, bool isKill);
  void paintFrame(Square *r, byte frame);
  //void drawDotLine(int x_s, int y_s, int len, bool is_v, int color);
  void drawShipCell(Coord *p);
  int get_pos_x(int col);
  int get_pos_y(int row);
};
#endif
