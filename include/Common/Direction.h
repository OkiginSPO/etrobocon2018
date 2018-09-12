/* 
 * File:   Direction.h
 * Author: a.Izumi
 *
 * Created on 2018/07/24, 16:47
 */

#ifndef DIRECTION_H
#define DIRECTION_H

#include "Distance.h"

/**
 * タイヤの進んだ距離（回転角）に関するクラス
 */
class Direction {
public:
    /* 初期化 */
    void init();

    /* 方位を取得(右旋回が正転) */
    float getDirection();

    // 方位を更新
    void update();
    
    /* 方位を設定*/
    void setDirection(float set_dir);
private:
    /* PI値を使用するためにinclude */
    Distance distance;
};

#endif	/* DIRECTION_H */

