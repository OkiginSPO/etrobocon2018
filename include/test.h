#ifndef TEST_H_
#define TEST_H_

#include "common.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "walker.h"
#include "SelfLocalization.h"
#include "Distance.h"

using namespace ev3api;

class Test
{
  public:
    // コンストラクタ
    Test();

    //
    void start();

  private:
    Walker walker;
    SelfLocalization sl;
    Clock clock;
    Distance distance;
    int32_t distance_total;
};

#endif

