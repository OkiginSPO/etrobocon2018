/**
 * EV3RT の走行を管理するクラス
 */
#ifndef WALKER_H_
#define WALKER_H_

#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

class Walker {
  public:

    // コンストラクタ
    Walker();

    // 初期化
    void init();
    // 停止
    void terminate();
    // リセット
    void reset();
    // 走る
    void run(int8_t pwm, int8_t turn);
    // 止まる
    void stop();
    // ブレーキ
    void setBrakeMotor(bool brake);
    // 車輪のオフセット値:左
    int32_t getCountL();
    // 車輪のオフセット値:右
    int32_t getCountR();
    // 指定した車輪の回転角度分だけ進む
    void moveAngle(int8_t pwm, int angle);
    // 車体を指定した角度回転させる
    void angleChange(int angle, int rotation);

  private:
    Motor leftWheel;
    Motor rightWheel;
    Clock clock;
    bool isRight = true; // 右: 1, 左: -1

};

#endif
