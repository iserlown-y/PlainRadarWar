/*
 * MIT License
 * 
 * Begin license text.
 * 
 * Copyright 2022 Atsuro Ida
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 * End license text.
 * 
 * 
 * Plain RaderWar ver1.0.0
 * Arduboy用のレーダー作戦ゲーム
 */
#include "globals.h"
#include "radar.h"
#include "sea.h"

Square square1(0, 0, GRID_SIZE, GRID_SIZE); // 敵軍領域（レーダー）
Square square2(GRID_SIZE + 4, 0, GRID_SIZE, GRID_SIZE); // 自軍領域
Radar radar(&square1);
Sea sea(&square2);
short phase;
short turn;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(12);
  arduboy.initRandomSeed();
  arduboy.audio.off();
#ifdef DEBUG
  Serial.begin(9600);
#endif
  reStart(P_TITLE);
}

void loop() {
  if (!arduboy.nextFrame()) { return; }
  arduboy.pollButtons();
  arduboy.clear();

  // フェーズ別処理
  bool bchg = false;
  switch(phase)
  {
    case P_TITLE:     // タイトル表示
      if(showTitle()){ phase = P_SETTING; bchg = true; }
      break;
    case P_SETTING:   // 艦艇配置
      if(setting()){ phase = P_PLAYING; bchg = true; }
      break;
    case P_PLAYING:   // 対戦
      if(playing()){ phase = P_RESULT; }
      break;
    case P_RESULT:    // 結果表示
      break;
  }

  if(phase > P_TITLE)
  {// Grid描画
    radar.draw(phase);  // レーダー面（敵）
    sea.draw(phase);    // 海面（自軍）
    if(bchg){ capture(); }
  }

  switch(phase)
  {
    case P_SETTING:
      showSetGuide();
      break;
    case P_RESULT:
      // 結果印字
      if(showResult())
      {// リスタート
        reStart(P_SETTING);
      }
      break;
  }

  arduboy.display();
}

void reStart(short phase_)
{
  phase = phase_;
  turn = 0;
  sea.clear();
  radar.clear();
  radar.setShips();  
}

bool showTitle()
{
  bool bres = false;
  arduboy.setCursor(0,10);
  arduboy.setTextSize(2);
  arduboy.println("Plain");
  arduboy.println("RadarWar");
  arduboy.setTextSize(1);
  arduboy.setCursor(0,48);
  arduboy.println(" Press [A] sound.on");
  arduboy.println(" Press [B] sound.off");

  if(arduboy.justPressed(A_BUTTON))
  {
    arduboy.audio.on();
    bres = true;
  }
  else if(arduboy.justPressed(B_BUTTON))
  {
    arduboy.audio.off();
    bres = true;
  }
  return bres;
}

// 艦船配置
bool setting()
{
  bool bres = false;
  static short stype = C_SHIP_NUM;
  if(sea.moveCur(stype))
  {
    if(sea.setShip(stype))
    {
      Globals::effect(1);
      stype--;
      if(stype <= 0){ bres = true; stype = C_SHIP_NUM; }
    }
  }
  return bres;
}

// 対戦
bool playing()
{
  bool bEnd = false;
  radar.moveCur();
  if(radar.shoot())
  {
    turn++;
    bEnd = radar.isAnnihilate();
    if(!bEnd)
    {
      sea.shoot();
      bEnd = sea.isAnnihilate();
    }
  }
  return bEnd;  
}

// 結果表示
bool showResult()
{
  char msg[9];
  if(arduboy.notPressed(A_BUTTON))
  {
    // 勝敗
    int w = WIDTH - (M_LEFT * 2);
    int h = HEIGHT - (M_TOP * 2);
    arduboy.fillRect(M_LEFT, M_TOP, w, h, 0);
    arduboy.drawRect(M_LEFT + 1, M_TOP + 1, w - 2, h - 2, 1);
    arduboy.setCursor(M_LEFT + 5, M_TOP + 3);
    arduboy.print(radar.isAnnihilate() ? "You win!" : "You lose");
    arduboy.setCursor(M_LEFT + 5, M_TOP + 13);
    sprintf(msg, "Turn:%d", turn);
    arduboy.print(msg);
    // 案内
    arduboy.fillRect(0, HEIGHT - 9, WIDTH, 9, 0);
    arduboy.setCursor(2, HEIGHT - 8);
    arduboy.print("Press [B] to restart.");
  }
  
  return arduboy.pressed(B_BUTTON);
}

void showSetGuide()
{
  arduboy.fillRect(G_LEFT, G_TOP, GRID_SIZE - (G_LEFT * 2), GRID_SIZE - (G_TOP * 2), 0);
  arduboy.setCursor(G_LEFT + 2, G_TOP + 2);
  arduboy.print("Set ships");
  arduboy.setCursor(G_LEFT + 2, G_TOP + 17);
  arduboy.print("[A]:Fix");
  arduboy.setCursor(G_LEFT + 2, G_TOP + 27);
  arduboy.print("[B]:H<->V");
}

void capture()
{
#ifdef DEBUG
  Serial.write(arduboy.getBuffer(), 128*64 / 8);
#endif
}
