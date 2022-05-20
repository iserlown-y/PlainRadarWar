#include "globals.h"
#include "grid.h"

/*
 * 戦場グリッド基底クラス
 */
Grid::Grid(Square *r)
{
  square = r;
  m_cur = new Coord(1,1);
  m_shoot = new Coord(0,0);
  clear();
}

Grid::~Grid()
{
  delete m_cur;
  delete m_shoot;
}

void Grid::clear()
{
  m_cur->clear(1);
  m_shoot->clear();
  m_frame = 0;
  init();  
}

void Grid::init()
{
  for(int x=0; x<C_X_NUM; x++)
  {
    for(int y=0; y<C_Y_NUM; y++)
    {
      m_cells[x][y].init();
    }
  }
}

void Grid::draw()
{
  // 枠
  square->drawRect();
  for(int row=1; row<C_Y_NUM; row++)
  {
    int y = get_pos_y(row);
    arduboy.drawLine(square->m_x, y, square->m_x + square->m_w - 1, y, 1);
    //drawDotLine(square->m_x, y, square->m_w, false, 1);
  }
  for(int col=1; col<C_X_NUM; col++)
  {
    int x = get_pos_x(col);
    arduboy.drawLine(x, square->m_y, x, square->m_y + square->m_h - 1, 1);
    //drawDotLine(x, square->m_y, square->m_h, true, 1);
  }
  // 結果
  Coord p;
  for(p.m_y=1; p.m_y<=C_Y_NUM; p.m_y++)
  {
    for(p.m_x=1; p.m_x<=C_X_NUM; p.m_x++)
    {
      paintCell(&p);
    }
  }
}

/*
void Grid::drawDotLine(int x_s, int y_s, int len, bool is_v, int color)
{
  int cnt = 0;
  int x = x_s;
  int y = y_s;
  while(cnt  < len)
  {
    switch((cnt + 3) % 6)
    {
      case 0:
        break;
      default:
        arduboy.drawPixel(x, y, color);
        break;
    }
    if(is_v){ y++; }else{ x++; }
    cnt++;
  }
}
*/

void Grid::drawShips()
{
  Coord p;
  for(p.m_y=1; p.m_y<=C_Y_NUM; p.m_y++)
  {
    for(p.m_x=1; p.m_x<=C_X_NUM; p.m_x++)
    {
      drawShipCell(&p);
    }
  }
}

void Grid::drawShipCell(Coord *p)
{
  Cell *c = getCell(p);
  if(c->m_stype >= 1)
  {
    Square *r = getCellSquare(p);
    int x0, y0, x1, y1;
    // Left
    if(!chkNearCell(c->m_stype, p, -1, 0))
    {
      x0 = x1 = r->m_x + 1;
      y0 = r->m_y + 1;
      y1 = r->m_y2 - 1;
      arduboy.drawLine(x0, y0, x1, y1, 1);
    }
    // Right
    if(!chkNearCell(c->m_stype, p, 1, 0))
    {
      x0 = x1 = r->m_x2 - 1;
      y0 = r->m_y + 1;
      y1 = r->m_y2 - 1;
      arduboy.drawLine(x0, y0, x1, y1, 1);
    }
    // Down
    if(!chkNearCell(c->m_stype, p, 0, 1))
    {
      x0 = r->m_x + 1;
      x1 = r->m_x2 - 1;
      y0 = y1 = r->m_y2 - 1;
      arduboy.drawLine(x0, y0, x1, y1, 1);
    }
    // Up
    if(!chkNearCell(c->m_stype, p, 0, -1))
    {
      x0 = r->m_x + 1;
      x1 = r->m_x2 - 1;
      y0 = y1 = r->m_y + 1;
      arduboy.drawLine(x0, y0, x1, y1, 1);
    }
    delete r;
  }
}

// 近隣セルの艦艇種または未被弾チェック(stype=0)
// pbHit(option):未沈没のHitであるかの通知先
bool Grid::chkNearCell(byte stype, Coord *p, int dx, int dy, bool *pbHit)
{
  bool bres = false;
  Coord p2(p->m_x + dx, p->m_y + dy);
  Cell *c = getCell(&p2);
  if(pbHit != NULL){ *pbHit = false; }
  if(c != NULL)
  { 
    if(stype == 0)
    {
      bres = !c->m_shooted;
    }
    else
    {
      bres = (c->m_stype == stype);
    }
    if(pbHit != NULL && !isSunken(&p2))
    { 
      *pbHit = c->isHit(); 
    }
  }
  return bres;
}

void Grid::moveCur()
{
  if(arduboy.pressed(RIGHT_BUTTON))
  {
    if(m_cur->m_x < C_X_NUM){ m_cur->next(0); }
  }
  else if(arduboy.pressed(LEFT_BUTTON))
  {
    if(m_cur->m_x > 1){ m_cur->back(0); }
  }
  else if(arduboy.pressed(DOWN_BUTTON))
  {
    if(m_cur->m_y < C_Y_NUM){ m_cur->next(1); }
  }
  else if(arduboy.pressed(UP_BUTTON))
  {
    if(m_cur->m_y > 1){ m_cur->back(1); }
  }
}

bool Grid::shoot(Coord *p, bool bcpu)
{
  bool bRet = false;
  short effect_id = bcpu ? 0 : 1;
  if(arduboy.justPressed(A_BUTTON))
  {
    Cell *c = getCell(p);
    bRet = !c->m_shooted;
    if(bRet)
    {
      c->m_shooted = true;
      m_shoot->copy(p);
      m_frame = F_NUM;
      if(c->isHit())
      {
        effect_id = bcpu ? 3 : 2;
      }
      Globals::effect(effect_id);
    }
  }
  return bRet;
}

int Grid::get_pos_x(int col)
{
  return square->m_x + ((square->m_w - 1) * col / C_X_NUM);
}

int Grid::get_pos_y(int row)
{
  return square->m_y + ((square->m_h - 1) * row / C_Y_NUM);
}

Square *Grid::getCellSquare(Coord *p)
{
  Coord ps(get_pos_x(p->m_x - 1), get_pos_y(p->m_y - 1));
  Coord pe(get_pos_x(p->m_x), get_pos_y(p->m_y));
  Square *r = new Square(&ps, &pe);
  return r;
}

// 沈没判定
bool Grid::isSunken(Coord *p)
{
  bool bSunken = false;
  Cell *cell = getCell(p);
  if(cell->m_shooted)
  {
    Coord pw;
    bSunken = true;
    while(pw.m_x <= C_X_NUM)
    {
      pw.next(0);
      pw.m_y = 0;
      while(pw.m_y <= C_Y_NUM)
      {
        pw.next(1);
        Cell *c = getCell(&pw);
        if(c->m_stype == cell->m_stype)
        {
          if(c->m_shooted == false)
          { bSunken = false; break; }
        }
      }
      if(!bSunken){ break; }
    }
  }
  return bSunken;
}

// 全滅判定
bool Grid::isAnnihilate()
{
  bool bAnn = true;
  Coord pw                                                                                                                                                                                                                                                                                                                                                                                               ;
  while(pw.m_x <= C_X_NUM)
  {
    pw.next(0);
    pw.m_y = 0;
    while(pw.m_y <= C_Y_NUM)
    {
      pw.next(1);
      Cell *c = getCell(&pw);
      if(c->isSafe()){ bAnn = false; break; }
    }
    if(!bAnn){ break; }
  }
  return bAnn;
}

void Grid::paintCell(Coord *p)
{
  Square *r = getCellSquare(p);
  Cell *cell = getCell(p);
  if(cell->m_shooted)
  {
    if(cell->m_stype == 0)
    {
      if((m_frame > 0) && p->equal(m_shoot))
      {
        paintFrame(r, F_SKIP);  // 点滅１回
        m_frame = 0;
      }
      else
      {
        paintMiss(r);
      }
    }
    else if((m_frame > 0) && cell->isHit() && p->equal(m_shoot))
    {
      paintFrame(r, m_frame--);
    }
    else
    {
      paintHit(r, isSunken(p));
    }
  }
  delete r;
}

void Grid::paintMiss(Square *r)
{
  // ・
  r->centerPoint();
}

void Grid::paintHit(Square *r, bool isKill)
{
  if(isKill)
  {// ●
    r->fillCircle();
  }
  else
  {// ×
    r->drawX();
  }
}

void Grid::paintFrame(Square *r, byte frame)
{
  if(frame % F_SKIP == 0)
  {// ■
    r->fillInRect();
  }
}

bool Grid::setShip(short stype, Coord *p, int dir)
{
  int len = Globals::stype2len(stype);
  bool bRet = isEnableSet(len, p, dir);
  if(bRet)
  {
    Coord p2(p); 
    for(int i=0; i<len; i++)
    {
      Cell *c = getCell(&p2);
      c->m_stype = stype;
      c->m_shooted = false;
      p2.next(dir);
    }
  }
  return bRet;
}

// セル情報
// returnのCell*はdeleteしてはいけない
Cell *Grid::getCell(Coord *p)
{
  if((p->m_x <= C_X_NUM) && (p->m_x >= 1) && (p->m_y <= C_Y_NUM) && (p->m_y >= 1))
  {
    return &m_cells[p->m_x - 1][p->m_y - 1];
  }
  else
  {
    return NULL;
  }
}

// 艦艇配置可否判定
bool Grid::isEnableSet(int len, Coord *p, int dir)
{
  bool bEna = true;
  Coord p2(p); 
  for(int i=0; i<len; i++)
  {
    Cell *c = getCell(&p2);
    if(c != NULL)
    {
      if(c->m_stype > 0){ bEna = false; break; }  // 他艦艇に重なる
      p2.next(dir);
    }
    else
    {
      bEna = false; break;  // 領域外
    }
  }
  return bEna;
}
