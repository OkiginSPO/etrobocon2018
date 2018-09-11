/**
 * @file PerpendicularParking.h
 * @brief 直角駐車クラス
 * @author Keisuke MORI
 */
#ifndef __PERPENDICULARPARKING__
#define __PERPENDICULARPARKING__

#include "BasicWalker.h"
#include "LineTracerWalker.h"
#include "Walker.h"
#include "ColorSensor.h"
#include "common.h"

/*! @class PerpendicularParking PerpendicularParking.h "PerpendicularParking.h"
 *  @brief 縦列駐車クラス
 */
class PerpendicularParking {
public:
    /**
     * コンストラクタ
     * カラーセンサを初期化する。
     */
    PerpendicularParking();

    /**
     * 縦列駐車を実行する。
     * 実行開始時にBasicWalkerクラスのインスタンスを生成し、
     * goStraightメンバ関数とspinメンバ関数に依存する。
     */
    void runParking();

private:
    ColorSensor colorSensor;
};


#endif
