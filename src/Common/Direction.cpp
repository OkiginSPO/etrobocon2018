#include "Direction.h"
#include "common.h"

static float direction = 0.0; //現在の方位

 /* 初期化 */
void Direction::init(){
    direction = 0.0;
}

 /* 方位を取得(右旋回が正転) */
float Direction::getDirection(){
    return direction;
}

/* 方位を更新 */
void Direction::update(){
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
    direction += (360.0 / (2.0 * distance.PI * TREAD)) * (distance.getDistance4msLeft() - distance.getDistance4msRight());
}

/* 方位を設定 */
void Direction::setDirection(float set_dir){
    direction = set_dir;
}
