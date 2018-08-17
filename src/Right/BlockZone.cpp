#include "common.h"
#include "test.h"

// 構造体に格納されている目標座標の数
static int32_t GRID_NUM = 8;

/**
 * Global variables
 */
typedef enum {
    TURN, // 目標座標の方位へ向くまで旋回
    MOVE, // 目標座標に到達するまで前進
    END // 構造体に格納されている座標を移動しきったら停止
} RUN_STATE;

//目標座標情報の構造体

struct GRID_XY {
    int32_t gridY; // y, x
    int32_t gridX;
    int back;
};

// 角度の算出に利用
static float GRID_X = 250; // 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
static float GRID_Y = 250; // 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0
// コンストラクタ

BlockZone::BlockZone() :
sl(walker.getCountL(), walker.getCountR(), false) {
}

//

void BlockZone::start() {
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
    static RUN_STATE state = TURN;
    /* {0,0}スタート。({0,0}は記述しない)*/
    // [0,0]地点にいるロボットは[1,0]方向を向いている
    // [0,0]→[2,0]が本来の一辺の移動である。このとき[1,0]は、その二点の中間に置かれた仮想点。
    // [6,0][6,1][6,2][6,3][6,4][6,5][6,6]
    // [5,0][5,1][5,2][5,3][5,4][5,5][5,6]
    // [4,0][4,1][4,2][4,3][4,4][4,5][4,6]
    // [3,0][3,1][3,2][3,3][3,4][3,5][3,6]
    // [2,0][2,1][2,2][2,3][2,4][2,5][2,6]
    // [1,0][1,1][1,2][1,3][1,4][1,5][1,6]
    // [0,0][0,1][0,2][0,3][0,4][0,5][0,6]
    struct GRID_XY target_grid[GRID_NUM] = {
        {1, 0, 0},
        {2, 0, 0},
        {2, 1, 0},
        {2, 2, 0},
        {1, 2, 0},
        {0, 2, 0},
        {0, 3, 0},
        {0, 4, 0}
    };

    //    int32_t grid_count = 0;
    int32_t angle = 0; // 回転角度
    int32_t distance = 0; // 進む距離

    GRID_XY past = {0, 0, 0}; // 過去  
    GRID_XY present = {0, 0, 0}; // 現在
    GRID_XY future = {0, 0, 0}; // 未来
    char msg[32];
    msg_f("start:brockZone", 3);




    walker.turnRight(180);




    
    /* 下のforループがうまく回ってくれないのでテスト用にオンコード  */
    /* pid走行の為か、進む距離がターン毎にズレていくようです */
    
        // 振りむく角度と向き(90 = 90度)
    basicWalker.spin(basicWalker.SPIN_RIGHT, 0);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)

    distance =MiriMeterToDistance(450);
    basicWalker.goStraight(50, distance);
    basicWalker.reset();
    
    basicWalker.spin(basicWalker.SPIN_RIGHT, 90);
    // walker.angleChange(90, 1);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)

    distance =MiriMeterToDistance(800);
    basicWalker.goStraight(50, distance);
    basicWalker.reset();

    // basicWalker.spin(basicWalker.SPIN_RIGHT, 90);
    walker.angleChange(90, 1);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)

    distance =MiriMeterToDistance(450);
    basicWalker.goStraight(50, distance);
    basicWalker.reset();
    /*
    // 振りむく角度と向き(90 = 90度)
    basicWalker.spin(basicWalker.SPIN_RIGHT, 90);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight(50, 500);
    basicWalker.reset();
    
    
    // 振りむく角度と向き(90 = 90度)
    basicWalker.spin(basicWalker.SPIN_RIGHT, 90);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight(50, 500);
    basicWalker.reset();
    
    
    
    // 振りむく角度と向き(90 = 90度)
    basicWalker.spin(basicWalker.SPIN_LEFT, 90);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight(50, 500);
    basicWalker.reset();
    */
//    for (int32_t grid_count = 1; grid_count < GRID_NUM - 1; grid_count++) {
//
//        // 配列から過去～現在～未来の座標を取得するため。
//        if ((0 < grid_count) && ((grid_count) <= GRID_NUM - 1)) {
//            // 通常処理
//            past = target_grid[grid_count - 1];
//            present = target_grid[grid_count];
//            future = target_grid[grid_count + 1];
//            msg_f("start:brockZone:1-1", 4);
//        } else {
//            // 配列に3つ以上値があるのが前提の処理。それ以下の場合は処理が行われない(どういう動きをするかわからない考えていない)
//            if (0 == grid_count) {
//                // 通常処理
//                past = {0, 0};
//                present = target_grid[grid_count];
//                future = target_grid[grid_count + 1];
//                msg_f("start:brockZone:1-2", 5);
//
//            } else {
//                // 処理終了。配列に、次座標が格納されていない為。
//                msg_f("start:brockZone:1-3", 6);
//
//                break;
//            }
//        }
//
//        // 初期化
//        angle = 0;
//        distance = 0;
//        msg_f("start:brockZone:2", 7);
//
//        sprintf(msg, "IsEquX:%d", (bool) IsEquivalent(past.gridX, present.gridX, future.gridX));
//        msg_f(msg, 4);
//
//
//        sprintf(msg, "IsEquY:%d", (bool) IsEquivalent(past.gridX, present.gridX, future.gridX));
//        msg_f(msg, 6);
//
//        sprintf(msg, "IsSerialX:%d", (bool) IsSerialnumber(past.gridX, present.gridX, future.gridX));
//        msg_f(msg, 8);
//
//        sprintf(msg, "IsSerialY:%d", (bool) IsSerialnumber(past.gridY, present.gridY, future.gridY));
//        msg_f(msg, 10);
//
//        // Xの値が同値かつYが連番なら角度0, (Y方向に進む)
//        if (IsEquivalent(past.gridX, present.gridX, future.gridX) && IsSerialnumber(past.gridY, present.gridY, future.gridY)) {
//            distance = 250;//(target_grid[grid_count].gridY - target_grid[grid_count + 1].gridY) * GRID_Y; // 過去 - 現在値 = 距離(Y方向)
//            angle = 0;
//            msg_f("X同値かつY連番", 7);
//            // 3行セット
//            move(angle, distance);
//            continue;
//        }
//
//        // Yの値が同値かつXが連番なら角度0, (X方向に進む)
//        if (IsEquivalent(past.gridY, present.gridY, future.gridY) && IsSerialnumber(past.gridX, present.gridX, future.gridX)) {
//            distance = 250;//(target_grid[grid_count].gridX - target_grid[grid_count + 1].gridX) * GRID_X; // 過去 - 現在値 = 距離(X方向)
//            angle = 0;
//
//            msg_f("Y同値かつX連番", 7);
//            move(angle, distance);
//            continue;
//        }
//
//        // 以降の処理は、3桁同値が無い前提(上でcontinueを行っているため)
//
//        //過去と未来のX-X, Y-Yの差が1または-1で一致している場合は、角度90度
//         if ((past.gridX - future.gridX) == ((past.gridY - future.gridY))) {
//             distance = 250;// 計算要
//             angle = 90;
//             
//             move(angle, distance);
//             continue;
//         }
//        
//        // 過去のXYと、未来のXYが一致している場合は角度180,
//        if ((past.gridX == future.gridX) && ((past.gridY == future.gridY))) {
//            distance = 250;//計算要
//            angle = 180;
//
//            move(angle, distance);
//            continue;
//        }
//        

//    }
}

int32_t BlockZone::MiriMeterToDistance(int32_t mm) {
  return mm / ((3.14 * TIRE_DIAMETER) / 360);
}

bool BlockZone::IsEquivalent(int32_t a, int32_t b, int32_t c) {
    return (a == b)&&(b == c);
}

bool BlockZone::IsSerialnumber(int32_t a, int32_t b, int32_t c) {
    // 2, 3, 4 : (4-3=1) == (3-2=1)
    // 4, 3, 4 : (4-3=1) != (3-4=-1)
    // 4, 3, 2 : (2-3=-1) == (3-4=-1)
    return (c - b) == (b - a);
}

void BlockZone::move(int32_t angle, int32_t distance) {
    // 振りむく角度と向き(90 = 90度)
    basicWalker.spin(basicWalker.SPIN_RIGHT, angle);
    basicWalker.reset();

    basicWalker.setPidWithoutTarget(1.0, 2.0, 0.02);
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight(50, distance);
    basicWalker.reset();
}
