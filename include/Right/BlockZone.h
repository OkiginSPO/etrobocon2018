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
#include "PIDController.h"

struct HSV {
    int hue;
    int saturation;
    int value;
};

struct GRID_XY {
    int32_t gridX;
    int32_t gridY;
    int order;
};

typedef enum {
    BEFORE,
    INITIALIZE,
    BLOCKZONE,
    AFTER
} BLOCKZONE_STATE;

class BlockZone {
public:
    // コンストラクタ
    BlockZone();

    // Bluetooth通信で行動データ取得
    void prepareMoveData(FILE* bt);

    // メイン処理
    bool MoveBlockZone();

    void MoveBlockZoneInit();
    
    // 初期位置まで移動する{2,0}
    bool MovingStartPosition();
    
    // 処理状況ステート取得
    BLOCKZONE_STATE GetBlockZoneState();
    // struct GRID_XY {
    //     int32_t gridX;
    //     int32_t gridY;
    //     int order;
    // };

    //    struct GRID_XY grid_xy[70];
private:
    // Bluetooth bluetooth;
    Walker walker;
    ColorSensor colorSensor;
    SelfLocalization sl;
    Clock clock;
    Distance distance;
    Direction direction;
    Grid grid;
    BasicWalker basicWalker;
    //    PIDController pidController;
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

    // 目的地までの経路がライン上かチェック
    bool IsMoveLines(int8_t x, int8_t y, int8_t target_x, int8_t target_y);


    bool IsMove90Turn(int8_t before_x, int8_t before_y, int8_t x, int8_t y, int8_t target_x, int8_t target_y);
    int32_t pid_sample(int32_t sensor_val, int32_t target_val);
    int32_t limit_math(double_t num);
    void PidReset(int32_t p, int32_t i, int32_t d);

};

#endif

