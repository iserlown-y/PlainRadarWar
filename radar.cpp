#include "globals.h"
#include "radar.h"

/*
 * 敵軍海域（レーダー）グリッド
 */

Radar::Radar(Square *r) : Grid(r)
{
}

void Radar::draw(int phase)
{
  Grid::draw();
  if(phase == P_PLAYING)
  {// カーソル描画
    Square *r = getCellSquare(m_cur);
    arduboy.drawRect(r->m_x + 1, r->m_y + 1, r->m_w - 1, r->m_h - 1, 1);
    delete r;
  }
  else if(phase == P_RESULT)
  {// 正解艦艇表示
    drawShips();
  }
}

void Radar::setShips()
{
  for (short stype = 1; stype <= C_SHIP_NUM; stype++)
  {
    bool bres = false;
    do
    {
      int x = random(1, C_X_NUM + 1);
      int y = random(1, C_Y_NUM + 1);
      Coord p(x,y);
      int dir = random(0, 2);
      bres = setShip(stype, &p, dir);
    } while (!bres);
  }
}

bool Radar::shoot()
{
  return Grid::shoot(m_cur);
}
