#include "BlockZone.h"
#include "string"
#include "vector"
#include "stdlib.h"
//#include "stdio.h"

// 構造体に格納されている目標座標の数
static int32_t GRID_NUM = 13;

/**
 * Global variables
 */
typedef enum {
    TURN, // 目標座標の方位へ向くまで旋回
    MOVE, // 目標座標に到達するまで前進
    END // 構造体に格納されている座標を移動しきったら停止
} RUN_STATE;


// 角度の算出に利用
//static float GRID_X = 250; // 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
//static float GRID_Y = 250; // 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0
// コンストラクタ

BlockZone::BlockZone() :
colorSensor(color_sensor),
sl(walker.getCountL(), walker.getCountR(), false) {
}

//

void BlockZone::start() {
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
    static RUN_STATE state = TURN;

    /*
     * ロボコンが落ちる箇所発覚。std::string rootIn/put~~~に文字列を代入しようとすると落ちる（たぶんmemory outof なんちゃら）。
     * 初期化が間違えている可能性あり。とりあえず放置/
     */
    /* {0,0}スタート。({0,0}は記述しない)*/
    // [0,0]地点にいるロボットは[1,0]方向を向いている
    // [0,0]→[2,0]が本来の一辺の移動である。このとき[1,0]は、その二点の中間に置かれた仮想点。
    // [6,0][6,1][6,2][6,3][6,4][6,5][6,6]
    // [5,0][5,1][5,2][5,3][5,4][5,5][5,6]
    // [4,0][4,1][4,2][4,3][4,4][4,5][4,6]
    // [3,0][3,1][3,2][3,3][3,4][3,5][3,6]
    // [2,0][2,1][2,2][2,3][2,4][2,5][2,6]
    // [1,0][1,1][1,2][1,3][1,4][1,5][1,6]
    // [0,0][0,1][0,2][0,3][0,4][0,5][0,6]
    
    
    // [0,6][1,6][2,6][3,6][4,6][5,6][6,6]
    // [0,5][1,5][2,5][3,5][4,5][5,5][6,5]
    // [0,4][1,4][2,4][3,4][4,4][5,4][6,4]
    // [0,3][1,3][2,3][3,3][4,3][5,3][6,3]
    // [0,2][1,2][2,2][3,2][4,2][5,2][6,2]
    // [0,1][1,1][2,1][3,1][4,1][5,1][6,1]
    // [0,0][1,0][2,0][3,0][4,0][5,0][6,0]
    
    GRID_XY target_grid[GRID_NUM] = {
        {1, 0, 0},
        {2, 0, 0}, // 配列の指示が間違えてrう！！
        {2, 1, 0},
        {2, 2, 0},
        {1, 2, 0},
        {0, 2, 0},
        {1, 2, 99},
        {1, 3, 0},
        {1, 4, 0},
        {2, 4, 0},
        {1, 4, 99},
        {0, 5, 0},
        {0, 6, 0}
    };

    
    
//    
//    GRID_XY target_grid[GRID_NUM] = {
//        {0, 1, 0},
//        {0, 2, 0}, // 配列の指示が間違えてrう！！
//        {1, 2, 0},
//        {2, 2, 0},
//        {2, 1, 0},
//        {2, 0, 0},
//        {2, 1, 99},
//        {3, 1, 0},
//        {4, 1, 0},
//        {4, 2, 0},
//        {4, 1, 99},
//        {5, 0, 0},
//        {6, 0, 0}
//    };

    // 構造体配列の要素数を定義の時点で固定にしていない状況で
    // 要素数を取得する必要が出た場合、
    // 1.「sizeof 構造体変数名」で構造体配列全体のサイズを、
    // 2.「sizeof struct タグ名」で1要素あたりのサイズを取得し、
    // 1を2で割ることで要素数を算出すればいいと。  @izumi(検証中)
    // GRID_NUM = ((sizeof target_grid) / (sizeof (struct GRID_XY)));

    char msg[128];
    msg_f("start:brockZone", 3);
    bool comment_out = true;
    walker.reset();
    int cur_gridX = 0; // 現在位置座標のX値
    int cur_gridY = 0; // 現在位置座標のY値
    float target_dis = 0.0; // 現在位置座標から目標座標までの距離
    float target_dir = 0.0; // 現在位置座標から目標座標までの方位
    float cur_dis = 0.0; // 距離計の現在地
    float cur_dir = 0.0; // 方位計の現在地
    int grid_count = 0; // 目標座標構造体への参照カウンタ
    bool isDestinationArrival = false;
    rgb_raw_t rgb;
    HSV hsv = {0, 0, 0};
    colorid_t color;

    // 計測器初期化
    distance.init();
    direction.init();

    // 目標座標までの方位、距離を格納
    grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    target_dis = grid.getDistance();
    target_dir = grid.getDirection();

    while (1) {

        /* 計測器更新 */
        distance.update();
        direction.update();

        // 計測器の現在地を格納
        cur_dis = distance.getDistance();
        cur_dir = direction.getDirection();

        switch (state) {
                // ッターン！
            case TURN:
                msg_f("start:brockZone2", 5);
                sprintf(msg, "cur_dir:%d", (int) cur_dir);
                msg_f(msg, 6);
                sprintf(msg, "target_dir:%d", (int) target_dir);
                msg_f(msg, 7);

                // 指定方位の一定範囲内に収まったら，移動開始
                if (((target_dir - 1.0) < cur_dir) && (cur_dir < (target_dir + 1.0))) {
                    state = MOVE;
                    break;
                }

                // 指定方位まで旋回する(10ではなく1単位で旋回しても、同様にずれるため速度は関係ないと思われる)
                if (cur_dir > target_dir) {
                    // 右回転
                    walker.run(0, -10);
                } else {
                    // 左回転
                    walker.run(0, 10);
                }

                break;
            case MOVE:

                sprintf(msg, "color:%d", (int) colorSensor.getColorNumber());
                msg_f(msg, 1);

                colorSensor.getRawColor(rgb);
                sprintf(msg, "R:%3d G:%3d B:%3d", rgb.r, rgb.g, rgb.b);
                msg_f(msg, 2);

                /** 通常ルート：直進する **/
                if (target_grid[grid_count].order == 0) {

                    // センサによる停止判定(前進時のみ使用)
                    if (cur_dis > (target_dis * 0.8)) {
                        if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                            // 白色、黒色以外を検知したらサークルを検知したと判定する

                            if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
                                isDestinationArrival = true;
                            }
                        } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                            // 黒色を検知したらラインを検知したと判定する
                            if (color == COLOR_BLACK) {
                                isDestinationArrival = true;
                            }
                        } else {
                            if (cur_dis > target_dis) {
                                isDestinationArrival = true;
                            }
                        }
                    }


                    // 指定位置までたどり着いたら状態遷移
                    if (isDestinationArrival || ((cur_dis) > target_dis)) {
                        if (!(grid_count < (GRID_NUM - 1))) {
                            state = END;
                            break;
                        }
                        ev3_speaker_play_tone(NOTE_G5, 40);

                        // 現在位置座標を更新
                        cur_gridX = target_grid[grid_count].gridX;
                        cur_gridY = target_grid[grid_count].gridY;

                        // 計測器のリセット
                        distance.init();
                        direction.setDirection(target_dir);

                        // 次の座標までの方位、距離を格納する
                        grid_count++;
                        /** 通常ルート:次の進行方向へ回転するため、現在座標情報・目標座標を格納する **/

                        grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                        grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                        target_dis = grid.getDistance();
                        target_dir = grid.getDirection();

                        // 再度、次座標への旋回を開始
                        if (target_grid[grid_count].order == 99) {
                            // バック時は回転せず、すぐに後退へ遷移する
                            state = MOVE;
                        } else {
                            state = TURN;
                        }
                        isDestinationArrival = false;
                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
                        state = END;
                    }

                    // 直進
                    walker.run(10, 0);
                } else if (target_grid[grid_count].order == 99) {
                    /** 後退時 **/

                    // 指定位置までたどり着いたら状態遷移(distance各値は絶対値で判断する)
                    if ((fabs(cur_dis) > fabs(target_dis)) && (grid_count < (GRID_NUM - 1))) {

                        // 現在位置座標を更新
                        cur_gridX = target_grid[grid_count].gridX;
                        cur_gridY = target_grid[grid_count].gridY;

                        // 計測器のリセット
                        distance.init();
                        // 現在の車体の向きをセットする。※バック時は車体の向きが違うため、ここで調整を行っています
                        direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));

                        grid_count++;
                        //次の進行方向へ回転するため、現在座標情報・目標座標を格納する
                        grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                        grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                        target_dis = grid.getDistance();
                        target_dir = grid.getDirection();

                        // 再度、次座標への旋回を開始
                        state = TURN;
                        isDestinationArrival = false;
                        break;
                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
                        state = END;
                        break;
                    }

                    // バック
                    walker.run(-10, 0);
                }

                break;
            case END:
                // モータを停止
                walker.stop();
                break;
            default:
                break;
        }
    }
}

HSV BlockZone::GetHsv(int r, int g, int b) {
    HSV hsv = {0, 0, 0};
    int _max = 0;
    int _min = 0;

    _max = max(3, r, g, b);
    _min = min(3, r, g, b);

    if (_max == r) {
        hsv.hue = 60 * ((float) (g - b) / (float) (_max - _min));
    }

    if (_max == g) {
        hsv.hue = 60 * ((float) (b - r) / (float) (_max - _min)) + 120;
    }

    if (_max == b) {
        hsv.hue = 60 * ((float) (r - g) / (float) (_max - _min)) + 240;
    }

    if ((r == g) && (r == b)) {
        hsv.hue = 0;
    }

    if (hsv.hue < 0) {
        hsv.hue += 360;
    }

    hsv.saturation = ((float) (_max - _min) / (float) _max) * 100;

    hsv.value = ((float) _max / (float) 255) * 100;

    return hsv;
}

colorid_t BlockZone::GetColorForHsv(HSV _hsv) {
    if ((_hsv.hue < 20 || _hsv.hue > 320) && _hsv.saturation >= 50) {
        return COLOR_RED;
    }

    if (_hsv.hue > 20 && _hsv.hue < 80 && _hsv.saturation >= 50) {
        return COLOR_YELLOW;
    }

    if (_hsv.hue > 80 && _hsv.hue < 140 && _hsv.saturation >= 50) {
        return COLOR_GREEN;
    }

    if (_hsv.hue > 200 && _hsv.hue < 260 && _hsv.saturation >= 50) {
        return COLOR_BLUE;
    }

    if (_hsv.saturation < 10 && _hsv.value > 90) {
        return COLOR_WHITE;
    }

    if (_hsv.value < 20) {
        return COLOR_BLACK;
    }

    return COLOR_NONE;
}

colorid_t BlockZone::GetColorForRgb(rgb_raw_t rgb) {
    colorid_t color;
    if ((rgb.r > 90) && (rgb.g > 90) && (rgb.b > 90)) {
        color = COLOR_WHITE;
    } else if ((rgb.r < 30) && (rgb.g < 30) && (rgb.b < 30)) {
        color = COLOR_BLACK;
    } else {
        color = COLOR_NONE;
    }
    return color;
}

bool BlockZone::IsGoToCircle(int8_t x, int8_t y) {
    return (x % 2 == 0) && (y % 2 == 0);
}

bool BlockZone::IsGoToLine(int8_t x, int8_t y, int8_t target_x, int8_t target_y) {
    return (x != target_x) && (y != target_y);
}



//
////
//
//#include "BlockZone.h"
//#include "string"
//#include "vector"
//#include "stdlib.h"
////#include "stdio.h"
//
//// 構造体に格納されている目標座標の数
//static int32_t GRID_NUM = 5;
//
///**
// * Global variables
// */
//typedef enum {
//    TURN, // 目標座標の方位へ向くまで旋回
//    RETURN, // バック時のずれから回復するためのリセット回転
//    MOVE, // 目標座標に到達するまで前進
//    END // 構造体に格納されている座標を移動しきったら停止
//} RUN_STATE;
//
////目標座標情報の構造体
//
//
//// 角度の算出に利用
//static float GRID_X = 250; // 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
//static float GRID_Y = 250; // 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0
//// コンストラクタ
//
//BlockZone::BlockZone() :
//colorSensor(color_sensor),
//sl(walker.getCountL(), walker.getCountR(), false) {
//}
//
////
//
//void BlockZone::start() {
//    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
//    static RUN_STATE state = TURN;
//
//    /*
//     * ロボコンが落ちる箇所発覚。std::string rootIn/put~~~に文字列を代入しようとすると落ちる（たぶんmemory outof なんちゃら）。
//     * 初期化が間違えている可能性あり。とりあえず放置/
//     */
//
//    struct GRID_XY target_grid[GRID_NUM] = {
//        {0, 1, 0},
//        {0, 0, 99},
//        {1, 1, 0},
//        {0, 2, 0},
//        {0, 3, 0}
//    };
//    //    struct GRID_XY target_grid[GRID_NUM] = {
//    //        {1, 0, 0},
//    //        {1, 1, 0},
//    //        {1, 0, 99},
//    //        {0, 0, 0},
//    //        {0, 1, 0}
//    //    };
//    //    int32_t grid_count = 0;
//
//    // 構造体配列の要素数を定義の時点で固定にしていない状況で
//    // 要素数を取得する必要が出た場合、
//    // 1.「sizeof 構造体変数名」で構造体配列全体のサイズを、
//    // 2.「sizeof struct タグ名」で1要素あたりのサイズを取得し、
//    // 1を2で割ることで要素数を算出すればいいと。  @izumi(検証中)
//    GRID_NUM = ((sizeof target_grid) / (sizeof (struct GRID_XY)));
//
//    char msg[128];
//    msg_f("start:brockZone", 3);
//
//    walker.reset();
//
//    int loop_count = 0; // 通算の支持回数
//    int counter_of_back = 99; // 前回のバックからの指示数をカウントする(2より大きい適当な自然数を初期値とする。※カウントアップしていって２のときに角度調整を行うため)
//    int cur_gridX = 0; // 現在位置座標のX値
//    int cur_gridY = 0; // 現在位置座標のY値
//    float target_dis = 0.0; // 現在位置座標から目標座標までの距離
//    float target_dir = 0.0; // 現在位置座標から目標座標までの方位
//    float cur_dis = 0.0; // 距離計の現在地
//    float cur_dir = 0.0; // 方位計の現在地
//    int grid_count = 0; // 目標座標構造体への参照カウンタ
//    bool isDestinationArrival = false;
//    rgb_raw_t rgb;
//    HSV hsv = {0, 0, 0};
//    colorid_t color;
//
//    // 計測器初期化
//    distance.init();
//    direction.init();
//
//    // 目標座標までの方位、距離を格納
//    grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//    grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//    target_dis = grid.getDistance();
//    target_dir = grid.getDirection();
//
//    // 初期化終了のタイミングで一声
//    ev3_speaker_play_tone(NOTE_C6, 40);
//
//    while (1) {
//
//        /* 計測器更新 */
//        distance.update();
//        direction.update();
//
//        // 計測器の現在地を格納
//        cur_dis = distance.getDistance();
//        cur_dir = direction.getDirection();
//
//        sprintf(msg, "color:%d", (int) colorSensor.getColorNumber());
//        msg_f(msg, 1);
//
//        colorSensor.getRawColor(rgb);
//        sprintf(msg, "R:%3d G:%3d B:%3d", rgb.r, rgb.g, rgb.b);
//        msg_f(msg, 2);
//
//        msg_f("start:brockZone2", 5);
//        sprintf(msg, "cur_dir:%d", (int) cur_dir);
//        msg_f(msg, 6);
//        sprintf(msg, "target_dir:%d", (int) target_dir);
//        msg_f(msg, 7);
//
//        switch (state) {
//            case TURN:
//                // 指定方位の一定範囲内に収まったら、移動開始
//                if ((cur_dir > (target_dir - 1.0)) && (cur_dir < (target_dir + 1.0))) {
//                    state = MOVE;
//                    break;
//                }
//
//                // 指定方位まで旋回する
//                if (cur_dir < target_dir) {
//                    walker.run(0, -10);
//                } else {
//                    walker.run(0, 10);
//                }
//
//                break;
//            case MOVE:
//
//                hsv = GetHsv(rgb.r, rgb.g, rgb.b);
//                color = GetColorForHsv(hsv);
//
//                if (target_grid[grid_count].order == 0) {
//
//                    // センサによる停止判定(前進時のみ使用)
//                    if (cur_dis > (target_dis * 0.8)) {
//                        if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
//                            // 白色、黒色以外を検知したらサークルを検知したと判定する
//
//                            if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
//                                isDestinationArrival = true;
//                            }
//                        } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
//                            // 黒色を検知したらラインを検知したと判定する
//                            if (color == COLOR_BLACK) {
//                                isDestinationArrival = true;
//                            }
//                        } else {
//                            if (cur_dis > target_dis) {
//                                isDestinationArrival = true;
//                            }
//                        }
//                    }
//
//                    // 回転角から計算した停止（センサが反応しなかった場合等：乗数はノリ。センサによる探索→目的地通過→回転角から計算した停止となるような補助機能的存在）
//                    if ((isDestinationArrival) || (cur_dis > (target_dis * 1.05))) {
//
//                        // カウントアップ
//                        loop_count += 1;
//                        grid_count++;
//
//                        // 現在位置座標を更新
//                        cur_gridX = target_grid[grid_count].gridX;
//                        cur_gridY = target_grid[grid_count].gridY;
//
//                        if (grid_count < (GRID_NUM - 1)) {
//
//                            // 次の座標までの方位、距離を格納する
//                            if (target_grid[grid_count].order == 0) {
//
//                                // 前進パターン
//                                distance.init();
//                                direction.setDirection(target_dir);
//                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                                grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                                target_dis = grid.getDistance();
//                                target_dir = grid.getDirection();
//                            } else if (target_grid[grid_count].order == 99) {
//
//                                // 後退パターン
//                                distance.init();
//                                direction.setDirection(target_dir); // 現在角度の設定
//                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                                grid.setDirection(grid.CalcDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY)); // Directionは0
//                                target_dis = grid.getDistance();
//                                target_dir = grid.getDirection();
//                            } else if (target_grid[grid_count].order == 0 && counter_of_back == 1) { // target_grid[grid_count].order == 0 と記載しなくても大丈夫なはずだけど、念のため記載します.
//                                // こここない
//                                //                                // 前進パターン（バック後）
//                                //                                target_dir -= 180; // 180で初期化されたものをもとに戻す
//                                //                                distance.init();
//                                //                                direction.setDirection(target_dir); // 現在角度の設定
//                                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                                //                                grid.setDirection(grid.CalcDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY)); // Directionは0
//                                //                                target_dis = grid.getDistance();
//                                //                                target_dir = grid.getDirection();
//
//                            }
//
//                            state = TURN;
//                        } else {
//                            state = END;
//                        }
//
//                    }
//                    //                            if (target_grid[grid_count].order == 99) {
//                    //
//                    //                                // 計測器のリセット
//                    //                                distance.init();
//                    //                                direction.setDirection(0);
//                    //
//                    //                                // 次の進行がバックなら回転値を変更する
//                    //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                    //                                grid.setDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY); // Directionは0
//                    //                                target_dis = grid.getDistance();
//                    //                                target_dir = grid.getDirection();
//                    //
//                    //                            } else if (counter_of_back == 1) {
//                    //                                // バックの次の走行が終了した(角度補正)
//                    //                                ev3_speaker_play_tone(NOTE_FS6, 40);
//                    //
//                    //                                // 計測器のリセット
//                    //                                distance.init();
//                    //                                direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                    //                                // direction.setDirection(180 - target_dir);
//                    //
//                    //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                    //                                float tmp_dir = 180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                    //                                grid.setDirection(tmp_dir); //XY両方座標が変わるとき（斜め移動）は90度割り当てる？
//                    //                                target_dis = grid.getDistance();
//                    //                                target_dir = grid.getDirection();
//                    //
//                    //                            } else {
//                    //
//                    //                                // 計測器のリセット
//                    //                                distance.init();
//                    //
//                    //                                if (2 <= loop_count) {
//                    //                                    direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                    //                                } else {
//                    //                                    direction.setDirection(target_dir);
//                    //                                }
//                    //
//                    //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                    //                                grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                    //                                target_dis = grid.getDistance();
//                    //                                target_dir = grid.getDirection();
//                    //                            }
//                    //
//                    //                            // 到着フラグを未達へ変更
//                    //                            isDestinationArrival = false;
//                    //                            counter_of_back += 1; // カウントアップ
//                    //
//                    //                            // 再度、次座標への旋回を開始
//                    //                            state = TURN;
//                    //                        } else {
//                    //                            state = END;
//                    //                        }
//                } else if (target_grid[grid_count].order == 99) {
//                    /** 特殊ルート：後退する **/
//                    walker.run(-10, 0);
//                    loop_count += 1;
//
//                    if ((fabs(cur_dis) > fabs(target_dis)) && (grid_count < (GRID_NUM - 1))) {
//
//                        ev3_speaker_play_tone(NOTE_C4, 40);
//
//                        // 現在位置座標を更新
//                        cur_gridX = target_grid[grid_count].gridX;
//                        cur_gridY = target_grid[grid_count].gridY;
//
//                        // 計測器のリセット
//                        distance.init();
//                        /**この辺りを工夫することで、どの角度でもバック後に正しい向きに行く気がする**/
//                        direction.setDirection(180 - target_dir);
//                        // direction.setDirection(target_dir);
//
//                        grid_count++;
//                        grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                        // grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//                        //                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//
//                        target_dis = grid.getDistance();
//                        target_dir = grid.getDirection();
//
//                        // バックしたよ宣言（次のMOVE終了の際には、角度を調整する必要がある）
//                        // 初期化
//                        counter_of_back = 1;
//
//                        // 旋回を開始
//                        state = TURN;
//                        //                        state = TURN;
//                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
//                        state = END;
//                    }
//                }
//
//                //
//                //                if (target_grid[grid_count].order == 0) {
//                //
//                //                    // 前方進行
//                //                    walker.run(10, 0);
//                //
//                //                    // センサによる停止を行う
//                //                    if (cur_dis > (target_dis * 0.8)) {
//                //                        if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
//                //                            // 白色、黒色以外を検知したらサークルを検知したと判定する
//                //
//                //                            if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
//                //                                isDestinationArrival = true;
//                //                            }
//                //                        } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
//                //                            // 黒色を検知したらラインを検知したと判定する
//                //                            if (color == COLOR_BLACK) {
//                //                                isDestinationArrival = true;
//                //                            }
//                //                        } else {
//                //                            if (cur_dis > target_dis) {
//                //                                isDestinationArrival = true;
//                //                            }
//                //                        }
//                //                    }
//                //                    
//                //
//                //                    // 回転角から計算した停止（センサが反応しなかった場合等：乗数はノリ。センサによる探索→目的地通過→回転角から計算した停止となるような補助機能的存在）
//                //                    if ((isDestinationArrival) || (cur_dis > (target_dis * 1.15))) {
//                //
//                //                        loop_count += 1;
//                //
//                //                        if (grid_count < (GRID_NUM - 1)) {
//                //                            // 現在位置座標を更新
//                //                            cur_gridX = target_grid[grid_count].gridX;
//                //                            cur_gridY = target_grid[grid_count].gridY;
//                //
//                //                            // 次の座標までの方位、距離を格納する
//                //                            grid_count++;
//                //
//                //                            if (target_grid[grid_count].order == 99) {
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //                                direction.setDirection(0);
//                //
//                //                                // 次の進行がバックなら回転値を変更する
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY); // Directionは0
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //
//                //                            } else if (counter_of_back == 1) {
//                //                                // バックの次の走行が終了した(角度補正)
//                //                                ev3_speaker_play_tone(NOTE_FS6, 40);
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //                                direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                //                                // direction.setDirection(180 - target_dir);
//                //
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)); //XY両方座標が変わるとき（斜め移動）は90度割り当てる？
//                //
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //
//                //                            } else {
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //
//                //                                if (2 <= loop_count) {
//                //                                    direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                //                                } else {
//                //                                    direction.setDirection(target_dir);
//                //                                }
//                //
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //                            }
//                //
//                //                            // 到着フラグを未達へ変更
//                //                            isDestinationArrival = false;
//                //                            counter_of_back += 1; // カウントアップ
//                //
//                //                            // 再度、次座標への旋回を開始
//                //                            state = TURN;
//                //                        } else {
//                //                            state = END;
//                //                        }
//                //                    }
//                //                } else if (target_grid[grid_count].order == 99) {
//                //                    /** 特殊ルート：後退する **/
//                //                    walker.run(-10, 0);
//                //                    loop_count += 1;
//                //
//                //                    if ((fabs(cur_dis) > fabs(target_dis)) && (grid_count < (GRID_NUM - 1))) {
//                //
//                //                        ev3_speaker_play_tone(NOTE_C4, 40);
//                //
//                //                        // 現在位置座標を更新
//                //                        cur_gridX = target_grid[grid_count].gridX;
//                //                        cur_gridY = target_grid[grid_count].gridY;
//                //
//                //                        // 計測器のリセット
//                //                        distance.init();
//                //                        /**この辺りを工夫することで、どの角度でもバック後に正しい向きに行く気がする**/
//                //                        direction.setDirection(180 - target_dir);
//                //                        // direction.setDirection(target_dir);
//                //
//                //                        grid_count++;
//                //                        grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                        // grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//                //                        //                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//                //
//                //                        target_dis = grid.getDistance();
//                //                        target_dir = grid.getDirection();
//                //
//                //                        // バックしたよ宣言（次のMOVE終了の際には、角度を調整する必要がある）
//                //                        // 初期化
//                //                        counter_of_back = 1;
//                //
//                //                        // 旋回を開始
//                //                        state = TURN;
//                //                        //                        state = TURN;
//                //                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
//                //                        state = END;
//                //                    }   
//                //                    
//                //                    
//                //
//                //                    // 回転角から計算した停止（センサが反応しなかった場合等：乗数はノリ。センサによる探索→目的地通過→回転角から計算した停止となるような補助機能的存在）
//                //                    if ((cur_dis > (target_dis * 1.15)) || (isDestinationArrival)) {
//                //
//                //                        loop_count += 1;
//                //
//                //                        if (grid_count < (GRID_NUM - 1)) {
//                //                            // 現在位置座標を更新
//                //                            cur_gridX = target_grid[grid_count].gridX;
//                //                            cur_gridY = target_grid[grid_count].gridY;
//                //
//                //                            // 次の座標までの方位、距離を格納する
//                //                            grid_count++;
//                //
//                //                            if (target_grid[grid_count].order == 99) {
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //                                direction.setDirection(0);
//                //
//                //                                // 次の進行がバックなら回転値を変更する
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY); // Directionは0
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //
//                //                            } else if (counter_of_back == 1) {
//                //                                // バックの次の走行が終了した(角度補正)
//                //                                ev3_speaker_play_tone(NOTE_FS6, 40);
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //                                direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                //                                // direction.setDirection(180 - target_dir);
//                //
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)); //XY両方座標が変わるとき（斜め移動）は90度割り当てる？
//                //
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //
//                //                            } else {
//                //
//                //                                // 計測器のリセット
//                //                                distance.init();
//                //
//                //                                    direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));
//                //                                } else {
//                //                                    direction.setDirection(target_dir);
//                //                                }
//                //
//                //                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                                target_dis = grid.getDistance();
//                //                                target_dir = grid.getDirection();
//                //                            }
//                //
//                //                            // 到着フラグを未達へ変更
//                //                            isDestinationArrival = false;
//                //                            counter_of_back += 1; // カウントアップ
//                //
//                //                            // 再度、次座標への旋回を開始
//                //                            state = TURN;
//                //                        } else {
//                //                            state = END;
//                //                        }
//                //                    }
//                //                } else if (target_grid[grid_count].order == 99) {
//                //                    /** 特殊ルート：後退する **/
//                //                    walker.run(-10, 0);
//                //                    loop_count += 1;
//                //
//                //                    if ((fabs(cur_dis) > fabs(target_dis)) && (grid_count < (GRID_NUM - 1))) {
//                //
//                //                        ev3_speaker_play_tone(NOTE_C4, 40);
//                //
//                //                        // 現在位置座標を更新
//                //                        cur_gridX = target_grid[grid_count].gridX;
//                //                        cur_gridY = target_grid[grid_count].gridY;
//                //
//                //                        // 計測器のリセット
//                //                        distance.init();
//                //                        /**この辺りを工夫することで、どの角度でもバック後に正しい向きに行く気がする**/
//                //                        direction.setDirection(180 - target_dir);
//                //                        // direction.setDirection(target_dir);
//                //
//                //                        grid_count++;
//                //                        grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                        // grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
//                //                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//                //                        //                        grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
//                //
//                //                        target_dis = grid.getDistance();
//                //                        target_dir = grid.getDirection();
//                //
//                //                        // バックしたよ宣言（次のMOVE終了の際には、角度を調整する必要がある）
//                //                        // 初期化
//                //                        counter_of_back = 1;
//                //
//                //                        // 旋回を開始
//                //                        state = TURN;
//                //                        //                        state = TURN;
//                //                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
//                //                        state = END;
//                //                    }
//
//        }
//        break;
//        case END:
//        // モータを停止
//        walker.stop();
//        break;
//        default:
//        break;
//    }
//}
//}
//
//HSV BlockZone::GetHsv(int r, int g, int b) {
//    HSV hsv = {0, 0, 0};
//    int _max = 0;
//    int _min = 0;
//
//    _max = max(3, r, g, b);
//    _min = min(3, r, g, b);
//
//    if (_max == r) {
//        hsv.hue = 60 * ((float) (g - b) / (float) (_max - _min));
//    }
//
//    if (_max == g) {
//        hsv.hue = 60 * ((float) (b - r) / (float) (_max - _min)) + 120;
//    }
//
//    if (_max == b) {
//        hsv.hue = 60 * ((float) (r - g) / (float) (_max - _min)) + 240;
//    }
//
//    if ((r == g) && (r == b)) {
//        hsv.hue = 0;
//    }
//
//    if (hsv.hue < 0) {
//        hsv.hue += 360;
//    }
//
//    hsv.saturation = ((float) (_max - _min) / (float) _max) * 100;
//
//    hsv.value = ((float) _max / (float) 255) * 100;
//
//    return hsv;
//}
//
//colorid_t BlockZone::GetColorForHsv(HSV _hsv) {
//    if ((_hsv.hue < 20 || _hsv.hue > 320) && _hsv.saturation >= 50) {
//        return COLOR_RED;
//    }
//
//    if (_hsv.hue > 20 && _hsv.hue < 80 && _hsv.saturation >= 50) {
//        return COLOR_YELLOW;
//    }
//
//    if (_hsv.hue > 80 && _hsv.hue < 140 && _hsv.saturation >= 50) {
//        return COLOR_GREEN;
//    }
//
//    if (_hsv.hue > 200 && _hsv.hue < 260 && _hsv.saturation >= 50) {
//        return COLOR_BLUE;
//    }
//
//    if (_hsv.saturation < 10 && _hsv.value > 90) {
//        return COLOR_WHITE;
//    }
//
//    if (_hsv.value < 20) {
//        return COLOR_BLACK;
//    }
//
//    return COLOR_NONE;
//}
//
//colorid_t BlockZone::GetColorForRgb(rgb_raw_t rgb) {
//    colorid_t color;
//    if ((rgb.r > 90) && (rgb.g > 90) && (rgb.b > 90)) {
//        color = COLOR_WHITE;
//    } else if ((rgb.r < 30) && (rgb.g < 30) && (rgb.b < 30)) {
//        color = COLOR_BLACK;
//    } else {
//        color = COLOR_NONE;
//    }
//    return color;
//}
//
//bool BlockZone::IsGoToCircle(int8_t x, int8_t y) {
//    return (x % 2 == 0) && (y % 2 == 0);
//}
//
//bool BlockZone::IsGoToLine(int8_t x, int8_t y, int8_t target_x, int8_t target_y) {
//    return (x != target_x) && (y != target_y);
//}
//
//
////    std::string rootInputByBlueTooth = "0010,0020,0021,0022,0023,0024,9923,0022,0032,0042,0043,0044,9954,0063,0064,0065,0066,9956,0046,0036,0025,0016,0026,36,46,9936,0026,9800 ";
////    std::string rootInputByBlueTooth("0010,0020,0021,0022,0023,0024,9923,0022,0032,0042,0043,0044,9954,0063,0064,0065,0066,9956,0046,0036,0025,0016,0026,36,46,9936,0026,9800 ");
//
////        std::string rootInputByBlueTooth ;
////        rootInputByBlueTooth = "1234";
////    rootInputByBlueTooth = "0010,0020,0021,0022,0023,0024,9923,0022,0032,0042,0043,0044,9954,0063,0064,0065,0066,9956,0046,0036,0025,0016,0026,36,46,9936,0026,9800 ";
////    int num = 2;
////    std::vector<GRID_XY> target_grid;
////
////    // 命令文字列を配列型(vector)形式に組み立てる
////    for (int i = 0; i < rootInputByBlueTooth.length(); i = i + 5) {
////        // 頭の２桁を一時格納
////        int prefix1 = atoi(rootInputByBlueTooth.substr(i - 1, 1).c_str());
////        int prefix2 = atoi(rootInputByBlueTooth.substr(i, 1).c_str());
////
////        int gridY = atoi(rootInputByBlueTooth.substr(i + 1, 1).c_str());
////        int gridX = atoi(rootInputByBlueTooth.substr(i + 2, 1).c_str());
////        int order = (prefix1 * 10) + prefix2; // 10の位と1の位を足し合わせる
////
////        target_grid[i] = {gridY, gridX, order};
////    }
///* {0,0}スタート。({0,0}は記述しない)*/
//// [0,0]地点にいるロボットは[1,0]方向を向いている
//// [0,0]→[2,0]が本来の一辺の移動である。このとき[1,0]は、その二点の中間に置かれた仮想点。
//// [6,0][6,1][6,2][6,3][6,4][6,5][6,6]
//// [5,0][5,1][5,2][5,3][5,4][5,5][5,6]
//// [4,0][4,1][4,2][4,3][4,4][4,5][4,6]
//// [3,0][3,1][3,2][3,3][3,4][3,5][3,6]
//// [2,0][2,1][2,2][2,3][2,4][2,5][2,6]
//// [1,0][1,1][1,2][1,3][1,4][1,5][1,6]
//// [0,0][0,1][0,2][0,3][0,4][0,5][0,6]
////    GRID_XY target_grid[GRID_NUM] = {
////        {1, 0, 0}, // 直進
////        {2, 0, 0}, // 直進
////        {2, 1, 0}, // 右90度
////        {2, 2, 0}, // 直進
////        {2, 1, 99}, // 右180度//99
////        {2, 0, 0}, // 直進
////        {1, 0, 0}, // 左90度
////        {0, 0, 0}, // 直進
////        {0, 1, 0}, // 左90度
////        {0, 0, 0}
////    };
//
//
//
//
//
//
//
//
////            case TURN:
////
////                // 指定方位の一定範囲内に収まったら，移動開始
////                if ((cur_dir > (target_dir - 1.0)) && (cur_dir < (target_dir + 1.0))) {
////                    state = MOVE;
////                    break;
////                }
////
////                // 指定方位まで旋回する
////                if (cur_dir < target_dir) {
////                    // 右回転
////                    walker.run(0, -10);
////                } else {
////                    // 左回転
////                    walker.run(0, 10);
////                }
////
////                break;
////            case MOVE:
////
////                sprintf(msg, "color:%d", (int) colorSensor.getColorNumber());
////                msg_f(msg, 1);
////
////                colorSensor.getRawColor(rgb);
////                sprintf(msg, "R:%3d G:%3d B:%3d", rgb.r, rgb.g, rgb.b);
////                msg_f(msg, 2);
////
////                hsv = GetHsv(rgb.r, rgb.g, rgb.b);
////                color = GetColorForHsv(hsv);
////
////                if (target_grid[grid_count].order == 0) {
////                    /** 通常ルート：直進する **/
////                    walker.run(10, 0);
////
////                    // センサによる位置補正（検知）
////                    if (cur_dis > (target_dis * 0.8)) {
////                        if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
////                            // 白色、黒色以外を検知したらサークルを検知したと判定する
////                            //                        if (colorSensor.getColorNumber() != COLOR_NONE) {
////                            //                            isDestinationArrival = true;
////                            //                        }
////
////                            if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
////                                isDestinationArrival = true;
////                            }
////                        } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
////                            // 黒色を検知したらラインを検知したと判定する
////                            //                        if (colorSensor.getColorNumber() != COLOR_NONE) {
////                            //                            isDestinationArrival = true;
////                            //                        }
////                            if (color == COLOR_BLACK) {
////                                isDestinationArrival = true;
////                            }
////                        } else {
////                            if (cur_dis > target_dis) {
////                                isDestinationArrival = true;
////                            }
////                        }
////                    }
////
////
////                    if (isDestinationArrival) {
////                        if ((grid_count < (GRID_NUM - 1))) {
////                            //                    if (((cur_dis) > target_dis) && (grid_count < (GRID_NUM - 1))) {
////                            // 現在位置座標を更新
////                            cur_gridX = target_grid[grid_count].gridX;
////                            cur_gridY = target_grid[grid_count].gridY;
////
////                            // 計測器のリセット
////                            distance.init();
////                            direction.setDirection(target_dir);
////
////                            // 次の座標までの方位、距離を格納する
////                            grid_count++;
////                            if (target_grid[grid_count].order == 0) {
////                                /** 直進時(通常パターン) **/
////                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
////                                grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
////                                target_dis = grid.getDistance();
////                                target_dir = grid.getDirection();
////                            } else if (target_grid[grid_count].order == 99) {
////                                /** 後退時（パラメータに補正をかける） **/
////                                direction.setDirection(0);
////                                grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
////                                grid.setDirection(cur_gridX, cur_gridY, cur_gridX, cur_gridY);
////                                target_dis = grid.getDistance();
////                                target_dir = grid.getDirection();
////                            }
////
////                            // 到着フラグを未達へ変更
////                            isDestinationArrival = false;
////
////                            // 次座標への旋回を開始
////                            state = TURN;
////                        } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
////                            state = END;
////                        }
////                    } else if (target_grid[grid_count].order == 99) {
////                        /** 特殊ルート：後退する **/
////                        walker.run(-10, 0);
////
////                        if ((fabs(cur_dis) > fabs(target_dis)) && (grid_count < (GRID_NUM - 1))) {
////                            // 現在位置座標を更新
////                            cur_gridX = target_grid[grid_count].gridX;
////                            cur_gridY = target_grid[grid_count].gridY;
////
////                            // 計測器のリセット
////                            distance.init();
////                            direction.setDirection(180 - target_dir);
////
////                            // 次の座標までの方位、距離を格納する
////                            grid_count++;
////                            /** 99ルート:次の進行方向へ回転するため、現在座標情報・目標座標を格納する **/
////
////                            grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
////                            grid.setDirection(180 - grid.CalcDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY));
////                            target_dis = grid.getDistance();
////                            target_dir = grid.getDirection();
////
////                            // 旋回を開始
////                            state = TURN;
////                        } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
////                            state = END;
////                        }
////                    }
////                }
