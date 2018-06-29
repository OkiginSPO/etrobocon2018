/**
 *
 */
#ifndef WALKER_H_
#define WALKER_H_

#include "location.h"

class Walker {
  public:
    // コンストラクタ
    Walker();

    // 現在地
    Location current_location;
    // 目的地
    Location destination_location;
    // スタートからの走行距離
    int distance;
};

#endif
