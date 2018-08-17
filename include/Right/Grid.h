/* 
 * File:   Grid.h
 * Author: a.Izumi
 *
 * Created on 2018/07/24, 15:21
 */

#ifndef GRID_H
#define	GRID_H

#include "math.h"

class Grid {
public:
    /* 初期化関数 */
void init();
/* 座標aから座標bまでの移動距離を設定する関数 */
void setDistance(int aX, int aY, int bX, int bY);
/* 座標aから座標bまでの移動距離を取得する関数 */
float getDistance();

/* 目標座標の方位を設定する関数 */
void setDirection(int aX, int aY, int bX, int bY);
/* 目標座標の方位を取得する関数 */
float getDirection();

private:
    
};

#endif	/* GRID_H */

