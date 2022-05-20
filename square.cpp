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

// □
void Square::drawRect()
{
  arduboy.drawRect(m_x, m_y, m_w, m_h, 1);
}

// □（内側）
void Square::drawInRect()
{
  arduboy.drawRect(m_x + 1, m_y + 1, m_w - 1, m_h - 1, 1);
}

// ■
void Square::fillInRect()
{
  arduboy.fillRect(m_x + 1, m_y + 1, m_w - 1, m_h - 1, 1);
}

// ・
void Square::centerPoint()
{
  Coord *p = getCenter();
  arduboy.drawPixel(p->m_x, p->m_y, 1);
  delete p;
}

// ●
void Square::fillCircle()
{
  Coord *p = getCenter();
  int r0 = getRadius();
  arduboy.fillCircle(p->m_x, p->m_y, r0-1, 1);
  delete p;
}

// ×
void Square::drawX()
{
  arduboy.drawLine(m_x, m_y, m_x2, m_y2, 1);
  arduboy.drawLine(m_x, m_y2, m_x2, m_y, 1);
}
