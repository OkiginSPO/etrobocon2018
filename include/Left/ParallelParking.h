/**
 * @file ParallelParking.h
 * @brief 縦列駐車クラス
 */
#ifndef _PARALLELPARKING__
#define _PARALLELPARKING__

#include "BasicWalker.h"
#include "LineTracerWalker.h"
#include "Walker.h"
#include "ColorSensor.h"
#include "common.h"

/*! @class ParallelParking ParallelParking.h "ParallelParking.h"
 *  @brief 駐車クラス
 */
class ParallelParking {
public:
    /**
     * コンストラクタ
     * カラーセンサを初期化する。
     */
    ParallelParking();

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
