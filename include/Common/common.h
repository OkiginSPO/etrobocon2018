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
#define GYRO_OFFSET           0  /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE          40  /* 白色の光センサ値 */
#define LIGHT_BLACK           0  /* 黒色の光センサ値 */
#define SONAR_ALERT_DISTANCE 30  /* 超音波センサによる障害物検知距離[cm] */
#define TAIL_ANGLE_STAND_UP  93  /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
#define P_GAIN             2.5F  /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */
#define TIRE_DIAMETER 101.0  //タイヤ直径 (10.1cm)
#define TREAD 164.0  // 車体トレッド幅(164.0mm)
//#define DEVICE_NAME     "ET0"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
//#define PASS_KEY        "1234" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

extern void init_f(const char *str);
extern void msg_f(const char *str, int32_t line);

#endif
