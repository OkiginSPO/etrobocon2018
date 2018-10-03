#include "Direction.h"
#include "common.h"

static float direction = 0.0; //現在の方位
static float coefficient = 1.0;// 走行体の個体差等によって変化する角度係数。 現在未確定

// coefficient
// 1.0 -> 回りが弱い
// 0.95 -> 回りが弱い
// 0.92 -> 回りが若干強い
// 0.9 -> 回りが若干強い
// 0.8 -> 明らかに回りすぎ

 /* 初期化 */
void Direction::init(){
    direction = 90.0;
}

 /* 方位を取得(右旋回が正転) */
float Direction::getDirection(){
    return direction;
}

/* 方位を更新 */
void Direction::update(){
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
//    direction += (360.0 / (2.0 * distance.PI * BROCK_ZONE_TREAD)) * (distance.getDistance4msLeft() - distance.getDistance4msRight());
      direction += ((360.0 / (2.0 * distance.PI * BROCK_ZONE_TREAD)) * (distance.getDistance4msLeft() - distance.getDistance4msRight())) * coefficient;

}

/* 方位を設定 */
void Direction::setDirection(float set_dir){
    direction = set_dir;
}
