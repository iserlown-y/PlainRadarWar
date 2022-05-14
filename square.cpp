#include "globals.h"
#include "square.h"

/*
 * 矩形領域管理
 */
Square::Square(int x, int y, int w, int h)
{
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
  m_x2 = x + w;
  m_y2 = y + h;
}

Square::Square(Coord *ps, Coord *pe)
{
  m_x = ps->m_x;
  m_y = ps->m_y;
  m_x2 = pe->m_x;
  m_y2 = pe->m_y;
  m_w = m_x2 - m_x;
  m_h = m_y2 - m_y;
}

Coord *Square::getCenter()
{
  int x0 = m_x + (m_w / 2);
  int y0 = m_y + (m_h / 2);
  Coord *p = new Coord(x0, y0);

  return p;
}

int Square::getRadius()
{
  int r0 = (m_h > m_w) ? m_w : m_h;
  return r0 / 2;
}
