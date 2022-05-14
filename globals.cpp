#include "globals.h"

/*
 * 共通
 */

// 艦種を長さに
int Globals::stype2len(byte stype)
{
  int len = -1;
  switch(stype)
  {
    case 1: // 駆逐艦
      len = 2;
      break;
    case 2: // 巡洋艦
    case 3: // 潜水艦
      len = 3;
      break;
    case 4: // 戦艦
      len = 4;
      break;
    case 5: // 空母
      len = 5;
      break;
  }
  return len;
}

// 効果音
void Globals::effect(short id)
{
  switch(id)
  {
    case 1: // A,Bクリック
      sound.tone(NOTE_C4,50);
      break;
    case 2: // Hit
      sound.tone(NOTE_C4,100, NOTE_E4,100, NOTE_G4,100);
      break;
    case 3: // 被弾
      sound.tone(NOTE_C0,100, NOTE_E0,100, NOTE_G0,100);
      break;
    default:
      break;
  }
}
