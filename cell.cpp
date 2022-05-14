#include "globals.h"
#include "cell.h"

/*
 * Gridのセル
 */
Cell::Cell()
{
  init();
}
Cell::Cell(byte stype)
{
  init(stype);
}

void Cell::init(byte stype)
{
  m_stype = stype;
  m_shooted = false;
}

bool Cell::isHit()
{
  return (m_shooted && m_stype >= 1);
}

bool Cell::isSafe()
{
  return (!m_shooted && m_stype >= 1);
}
