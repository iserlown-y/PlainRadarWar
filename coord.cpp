#include "globals.h"
#include "Coord.h"

/*
 * 座標管理
 */

Coord::Coord()
{
  clear(0);
}
Coord::Coord(int x, int y)
{
  m_x = x;
  m_y = y;
}
Coord::Coord(Coord *p)
{
  copy(p);
}

void Coord::clear(int val)
{
  m_x = m_y = val;
}

bool Coord::equal(Coord *p)
{
  return ((p->m_x == m_x) && (p->m_y == m_y));
}

void Coord::copy(Coord *p)
{
  m_x = p->m_x;
  m_y = p->m_y;
}

// dir 0:x 1:y
void Coord::next(int dir)
{
  switch(dir)
  {
    case 0:
      m_x++;
      break;
    case 1:
      m_y++;
      break;
  }
}

// dir 0:x 1:y
void Coord::back(int dir)
{
  switch(dir)
  {
    case 0:
      m_x--;
      break;
    case 1:
      m_y--;
      break;
  }
}

// dir 0:right 1:left 2:down 3:up
void Coord::moveBy4Dir(int dir)
{
  switch(dir)
  {
    case 0:
      next(0); break;
    case 1:
      back(0); break;
    case 2:
      next(1); break;
    case 3:
      back(1); break;
  }
}
