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
#include "Bluetooth.h"

struct HSV {
    int hue;
    int saturation;
    int value;
};

class BlockZone {
public:
    // コンストラクタ
    BlockZone();

    // Bluetooth通信で行動データ取得
    void prepareMoveData(FILE* bt);

    //
    void start();

private:
    Walker walker;
    ColorSensor colorSensor;
    SelfLocalization sl;
    Clock clock;
    Distance distance;
    Direction direction;
    Grid grid;
    BasicWalker basicWalker;
    int32_t distance_total;
    int32_t remnant_X;
    int32_t remnant_Y;

    bool isDestinationArrival;
    
    colorid_t GetColorForRgb(rgb_raw_t rgb);
    
    HSV GetHsv(int r, int g, int b);
    
    colorid_t GetColorForHsv(HSV hsv);
    
    // 目的地がサークルかチェック
    bool IsGoToCircle(int8_t x, int8_t y);
    
    // 目的地がラインかチェック
    bool IsGoToLine(int8_t x, int8_t y, int8_t target_x, int8_t target_y);

    struct GRID_XY {
        int32_t gridX;
        int32_t gridY;
        int order;
    };

    Bluetooth bluetooth;
};

#endif

