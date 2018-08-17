#ifndef BLOCKZONE_H_
#define BLOCKZONE_H_

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
#include "Direction.h"
#include "Grid.h"
#include "BasicWalker.h"


class BlockZone
{
  public:
    // コンストラクタ
    BlockZone();

    //
    void start();

  private:
    Walker walker;
    SelfLocalization sl;
    Clock clock;
    Distance distance;
    Direction direction;
    Grid grid;
    BasicWalker basicWalker;
    int32_t distance_total;
    int32_t remnant_X;
    int32_t remnant_Y;
    
    /* 連番チェック*/
    bool IsSerialnumber(int32_t a, int32_t b, int32_t c);
    
    /* 同値チェック */
    bool IsEquivalent(int32_t a, int32_t b, int32_t c);
    
    /* 動く */
    void move(int32_t angle, int32_t distance);

    /* mm から distance(回転数) */
    int32_t MiriMeterToDistance(int32_t mm);
};

#endif

