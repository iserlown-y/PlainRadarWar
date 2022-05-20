#include "globals.h"
#include "sea.h"

/*
 * 自軍海域グリッド
 */
Sea::Sea(Square *r) : Grid(r)
{
  m_dir = 0;
}

bool Sea::moveCur(short stype)
{
  Grid::moveCur();
  if(arduboy.justPressed(B_BUTTON))
  { 
    Globals::effect(1); 
    m_dir = m_dir == 0 ? 1: 0;  // 縦横切替
  }
  int len = Globals::stype2len(stype);
  bool bEna = isEnableSet(len, m_cur, m_dir);
  if(bEna)
  {
    Square *rs = getCellSquare(m_cur);
    int w = ((m_dir == 0) ? len : 1) * C_CELL_WIDTH; 
    int h = ((m_dir == 0) ? 1 : len) * C_CELL_WIDTH;
    arduboy.drawRect(rs->m_x + 1, rs->m_y + 1, w - 1, h - 1, 1);
    delete rs;
  }
  return bEna;
}

bool Sea::setShip(short stype)
{
  bool bres = false;
  if(arduboy.justPressed(A_BUTTON))
  { bres = Grid::setShip(stype, m_cur, m_dir); }
  return bres;
}

void Sea::draw(int phase)
{
  Grid::draw();
  if(phase == P_SETTING)
  {
    Square *r = getCellSquare(m_cur);
    r->fillInRect();
    delete r;
  }
  drawShips();
}


// 敵軍の攻撃
void Sea::shoot()
{
  // 命中点周辺を攻撃
  if(!concentratedShoot())
  {
    // 索敵攻撃
    serchingShoot();
  }
}

// 命中点の周りを攻める
bool Sea::concentratedShoot()
{
  Coord *p;
  Coord p2;
  int swcnt = SW_RETRY;
  bool bshoot = false;
  int dir = -1;
  // 未沈没の命中点を探す
  while(true)
  {
    p = selectDamage(&dir);
    if(p != NULL)
    {
      if(dir >= 0){ break; }
      if(swcnt-- >= 0)   // 推奨方向が無い場合SW_RETRY回までやりなおす
      {
        delete p;
        continue;
      }
    }
    break;
  }

  if(p != NULL)
  {
    if(dir == -1)
    {
      dir = random(4);
    }
    while(!bshoot)
    {
      Cell *c2;
      p2.copy(p);
      switch(dir)
      {
        case 0:
          p2.next(0);
          break;
        case 1:
          p2.back(0);
          break;
        case 2:
          p2.next(1);
          break;
        case 3:
          p2.back(1);
          break;
      }
      c2 = getCell(&p2);
      dir = random(4);
      if(c2 != NULL && !c2->m_shooted)
      {
        Grid::shoot(&p2, true);
        bshoot = true;
      }
    }
    delete p;
  }
  return bshoot;
}

// 未沈没被命中点検索（推奨方向付き）
// pdir:推奨隣接点方向
Coord *Sea::selectDamage(int *pdir)
{
  Coord *p;
  Coord p2;
  bool hits[] = {false, false, false, false}; // right, left, down. up
  *pdir = -1;
  // 未沈没の命中点を探す
  p = findDamage(hits);
  if(p != NULL)
  {
    int dirs[4];
    int indx = 0;
    for(int i=0; i<4; i++)
    {
      int dir = chkDir(p, hits, i);
      if(dir >= 0)
      {
        dirs[indx++] = dir;
      }
    }
    if(indx > 0)
    {
      *pdir = dirs[random(indx)];
    }
  }
  return p;
}

// 被弾点が連続している場合にその方向の未着弾点方向を返す
// return 0:right 1:left 2:down 3:up -1:nothing
int Sea::chkDir(Coord *p, bool *hits, int indx)
{
  const int revs[] = {1,0,3,2};
  const int edges[] = {1, C_X_NUM, 1, C_Y_NUM};
  int dir = -1;
  if(hits[indx])
  {
    int indx_r = revs[indx];
    int edge = edges[indx];
    int pval = (indx > 1) ? p->m_y : p->m_x;
  
    if(!hits[indx_r] && pval != edge)
    {// 未着弾確認
      Coord pn(p);
      pn.moveBy4Dir(indx_r);
      Cell *c = getCell(&pn);
      if(!c->m_shooted)
      {
        dir = indx_r;
      }
    }
  }
  return dir;
}

// 残存最長艦艇が隠れている可能性のある点を乱射
// 左右・上下片方のみの場合はCNT_MAXまで据え置いて選択
void Sea::serchingShoot()
{ 
  Coord ps[CNT_MAX];
  int cnt = 0;
  bool bshoot = false;
  while(!bshoot)
  {
    int x0 = random(C_X_NUM) + 1;
    int y0 = random(C_Y_NUM) + 1;
    Coord p0(x0, y0);
    Cell *c0 = getCell(&p0);
    if(!c0->m_shooted)
    {
      switch(chkPosibleCell(&p0))
      {
        case 2:
          Grid::shoot(&p0, true);
          bshoot = true;
          break;
        case 1:
          ps[cnt++] = p0;
          if(cnt >= CNT_MAX)
          {
            Grid::shoot(&ps[random(CNT_MAX)], true);
            bshoot = true;
          }
          break;
      }
    }
  }
}

// 残存最長艦艇が隠れている可能性のあるセルか
// return 0:ない 1:片方向あり 2:両方向あり
int Sea::chkPosibleCell(Coord *p)
{
  int lvl = 0;
  byte stype = getSurvivorLongestShip();
  int len = Globals::stype2len(stype);
  if(isPosible(p, 0, len)){ lvl++; }
  if(isPosible(p, 1, len)){ lvl++; }
  return lvl;
}

// 指定長の連続未着弾点に含まれるか
bool Sea::isPosible(Coord *p, int dir, int len)
{
  bool bres = false;
  Coord p2(0,0);
  int cnt = 1;
  for(int nb=0; nb<2; nb++)
  {
    p2.copy(p);
    while(!bres)
    {
      if(nb==0){ p2.next(dir); }else{ p2.back(dir); }
      Cell *c = getCell(&p2);
      if(c != NULL && !c->m_shooted){ cnt++; }else{ break; }
      if(cnt >= len){ bres = true; break; }
    }
  }
  return bres;
}

// 未沈没の最大艦艇種取得
byte Sea::getSurvivorLongestShip()
{
  byte stype = 0;
  for (int x=1; x<=C_X_NUM; x++)
  {
    for (int y=1; y<=C_Y_NUM; y++)
    {
      Coord p(x, y);
      Cell *c = getCell(&p);
      if (c->isSafe()) 
      {
        if (c->m_stype > stype)
        {
          stype = c->m_stype;
          if(stype == C_SHIP_NUM){ break; }
        }
      }
    }
    if(stype == C_SHIP_NUM){ break; }
  }
  return stype;
}

// 未沈没で近隣に未着弾点のある命中点を探す
// 最大PIC_MAX個の候補から１つ選択する
// hits:周囲の被弾状態通知先
Coord *Sea::findDamage(bool *hits)
{
  Coord ps[PIC_MAX];
  bool hitsAry[PIC_MAX][4];
  int pic = 0;
  Coord *p = new Coord(1, 1);
  while(p->m_x <= C_X_NUM)
  {
    p->m_y = 1;
    while(p->m_y <= C_Y_NUM)
    {
      Cell *c = getCell(p);
      if(c->isHit())
      {
        if(!isSunken(p))
        {
          bool bfind = false;
          bfind |= chkNearCell(0, p, 1, 0, &hitsAry[pic][0]);
          bfind |= chkNearCell(0, p, -1, 0, &hitsAry[pic][1]);
          bfind |= chkNearCell(0, p, 0, 1, &hitsAry[pic][2]);
          bfind |= chkNearCell(0, p, 0, -1, &hitsAry[pic][3]);
          if(bfind)
          {
            ps[pic++] = *p;
            if(pic >= PIC_MAX){ break; }
          }
        }
      }
      p->next(1);
    }
    if(pic >= PIC_MAX){ break; }
    p->next(0);
  }
  if(pic == 0)
  {
    delete p;
    p = NULL;
  }
  else
  {
    int indx = random(pic);
    p->copy(&ps[indx]);
    for(int i=0; i<4; i++)
    {
      hits[i] = hitsAry[indx][i];
    }
  }
  return p;
}
