#ifndef COMMON_H_
#define COMMON_H_

#include "ev3api.h"
#include "Port.h"

/**
 * センサー、モーターの接続を定義します
 */
static const ePortS
    touch_sensor    = PORT_1,
    sonar_sensor    = PORT_2,
    color_sensor    = PORT_3,
    gyro_sensor     = PORT_4;

static const ePortM
    left_motor      = PORT_C,
    right_motor     = PORT_B,
    tail_motor      = PORT_A;

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define TIRE_DIAMETER 101.0  //タイヤ直径 (10.1cm)
#define TREAD 164.0  // 車体トレッド幅(164.0mm)

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

extern void init_f(const char *str);
extern void msg_f(const char *str, int32_t line);

extern int max(int len, ...);
extern int min(int len, ...);

#endif
