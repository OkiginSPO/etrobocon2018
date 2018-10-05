#include "Grid.h"
#include "math.h"

//static float GRID_SIZE = 200.0; //450.0; //座標のマス幅（100mm）

// 角度の算出に利用(両方200のときは正方形と仮定してのこと。本来は長方形であるべき)
// TODO: pid制御のテスト用にGRIDXYの値をいじってます
static float GRID_X = 225.0; // 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
static float GRID_Y = 200.0; // 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0

static float grid_distance = 0.0; //現在座標から目標座標までの距離
static float grid_direction = 0.0; //現在座標から目標座標の方位

/* 初期化関数 */
void Grid::init() {
    grid_distance = 0.0;
    grid_direction = 90.0;
}

/* 座標aから座標bまでの移動距離を設定する関数 */
void Grid::setDistance(int aX, int aY, int bX, int bY) {
    //       長方形に対応
    grid_distance = sqrt(pow((float)(bY - aY) * GRID_Y ,2) + pow((float)(bX - aX) * GRID_X,2));
//    grid_distance = sqrt(pow((float) (bY - aY), 2) + pow((float) (bX - aX), 2)) * GRID_SIZE;
}

/* 座標aから座標bまでの移動距離を取得する関数 */
float Grid::getDistance() {
    return grid_distance;
}

/* 目標座標の方位を設定する関数 */
void Grid::setDirection(int aX, int aY, int bX, int bY) {
    grid_direction = CalcDirection(aX, aY, bX, bY);
}

float Grid::CalcDirection(int aX, int aY, int bX, int bY) {
    float targetDir = 0.0;// 目標方位

    //　座標aから座標bへの方位（ラジアン）を取得
    targetDir = atan2((float)(bY-aY) * GRID_Y, (float)(bX-aX) * GRID_X);
//    targetDir = atan2((float)(bY-aY), (float)(bX-aX));
    //ラジアンから度に変換
    targetDir = targetDir * 180.0 /  3.14159265359;

    return targetDir;
}

void Grid::setDirection(float direction) {
    grid_direction = direction;
}

/* 目標座標の方位を取得する関数 */
float Grid::getDirection() {
    return grid_direction;
}
