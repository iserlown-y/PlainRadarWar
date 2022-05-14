#ifndef Globals_h
#define Globals_h

#include <Arduboy2.h>
#include <ArduboyTones.h>

extern Arduboy2 arduboy;
extern ArduboyTones sound;

//#define DEBUG

#define C_X_NUM 10
#define C_Y_NUM C_X_NUM
#define C_CELL_WIDTH 6
#define C_SHIP_NUM 5
#define GRID_SIZE (C_CELL_WIDTH * C_X_NUM) + 1

// 点滅速度
#define F_SKIP 2

// HIT点滅フレーム数
#define F_NUM F_SKIP * 6

// 結果表示位置
#define M_TOP 20
#define M_LEFT 35

// ガイド表示位置
#define G_TOP 10
#define G_LEFT 2

// フェーズ
#define P_TITLE 1
#define P_SETTING 2
#define P_PLAYING 3
#define P_RESULT 4

class Globals
{
  public:
    static int stype2len(byte stype);
    static void effect(short id);
};

#endif
