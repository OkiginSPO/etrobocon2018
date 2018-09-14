/**
 * 
 */
#ifndef LOCATION_H_
#define LOCATION_H_

class Location {
  public:
    // コンストラクタ
    Location();

    // 現在地のx座標
    static float x:
    // 現在地のy座標
    static float y:
    // 目的地のx座標
    float destination_x;
    // 目的地のy座標
    float destination_y;
    // 現在EV3RTが向いている角度
    static int angle;
    // スタートからの総走行距離
    static int distance;
};

#endif
