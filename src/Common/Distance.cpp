/**
 * @file Distance.cpp
 * @brief Distanceクラスの関数を定義<br>
 * @author Futa HIRAKOBA
 */

#include "Distance.h"
#include "common.h"

int32_t Distance::getDistanceTotal(int32_t countL, int32_t countR){
  leftMotorDegTotal = countL;
  rightMotorDegTotal = countR;
  return (leftMotorDegTotal + rightMotorDegTotal) / 2;
}

int32_t Distance::getDistanceCurrent(int32_t countL, int32_t countR){
int32_t leftMotorDeg = countL - leftMotorDegOld;
int32_t rightMotorDeg = countR - rightMotorDegOld;

  return (leftMotorDeg + rightMotorDeg) / 2;
}

void Distance::resetDistance(int32_t countL, int32_t countR){
  getDistanceTotal(countL, countR);
  leftMotorDegOld = leftMotorDegTotal;
  rightMotorDegOld = rightMotorDegTotal;
}


/* 個々から以下は和泉 */

// static float TIRE_DIAMETER = 97.0; // タイヤ直径(97mm)
static float distance = 0.0;     //走行距離
static float distance4msL = 0.0; //左タイヤの4ms間の距離
static float distance4msR = 0.0; //右タイヤの4ms間の距離
static float pre_angleL, pre_angleR; // 左右モータ回転角度の過去値

/* 初期化関数 */
void Distance::init() {
    //各変数の値の初期化
    distance = 0.0;
    distance4msR = 0.0;
    distance4msL = 0.0;
    //モータ角度の過去値に現在値を代入
    pre_angleL = walker.getCountL(); //ev3_motor_get_counts(left_motor);
    pre_angleR = walker.getCountR(); // ev3_motor_get_counts(right_motor);
}

/* 距離更新（4ms間の移動距離を毎回加算している） */
void Distance::update(){
    float cur_angleL = walker.getCountL();//ev3_motor_get_counts(left_motor); //左モータ回転角度の現在値
    float cur_angleR = walker.getCountR();//ev3_motor_get_counts(right_motor);//右モータ回転角度の現在値
    float distance4ms = 0.0;        //4msの距離

    // 4ms間の走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値　- モータ角度現在値)
    distance4msL = ((PI * BROCK_ZONE_TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL);  // 4ms間の左モータ距離
    distance4msR = ((PI * BROCK_ZONE_TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR);  // 4ms間の右モータ距離
    distance4ms = (distance4msL + distance4msR) / 2.0; //左右タイヤの走行距離を足して割る
    distance += distance4ms;

    //モータの回転角度の過去値を更新
    pre_angleL = cur_angleL;
    pre_angleR = cur_angleR;
}

/* 走行距離を取得 */
float Distance::getDistance(){
    return distance;
}

/* 右タイヤの4ms間の距離を取得 */
float Distance::getDistance4msRight(){
    return distance4msR;
}

/* 左タイヤの4ms間の距離を取得 */
float Distance::getDistance4msLeft(){
    return distance4msL;
}
