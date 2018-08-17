#include "Grid.h"
#include "math.h"

static float GRID_SIZE = 250.0;//450.0; //座標のマス幅（100mm）

//// 角度の算出に利用
static float GRID_X = 250;// 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
static float GRID_Y = 250;// 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0


static float grid_distance = 0.0; //現在座標から目標座標までの距離
static float grid_direction = 0.0;//現在座標から目標座標の方位



/* 初期化関数 */
void Grid::init() {
    grid_distance = 0.0;
    grid_direction = 0.0;
}

/* 座標aから座標bまでの移動距離を設定する関数 */
void Grid::setDistance(int aX, int aY, int bX, int bY) {
    grid_distance = sqrt( pow((float)(bX-aX),2) + pow((float)(bY-aY),2) ) *  GRID_SIZE;
      // 長方形に対応
//      grid_distance = sqrt( pow((float)(bX-aX) * GRID_X ,2) + pow((float)(bY-aY) * GRID_Y,2) );
}

/* 座標aから座標bまでの移動距離を取得する関数 */
float Grid::getDistance() {
    return grid_distance;
}

/* 目標座標の方位を設定する関数 */
void Grid::setDirection(int aX, int aY, int bX, int bY) {
    float targetDir = 0.0;// 目標方位

    //　座標aから座標bへの方位（ラジアン）を取得
    targetDir = atan2((float)(bY-aY), (float)(bX-aX));
//    targetDir = atan2((float)(bY-aY) * GRID_Y, (float)(bX-aX) * GRID_X);
    //ラジアンから度に変換
    targetDir = targetDir * 180.0 / 3.14159265359;

    grid_direction = targetDir;
}

/* 目標座標の方位を取得する関数 */
float Grid::getDirection() {
    return grid_direction;
}
