#include "BlockZone.h"
#include "string"
#include "vector"
#include "stdlib.h"
#include "TurnControl.h"

// 構造体に格納されている目標座標の数
static int32_t GRID_NUM = 9;

/**
 * Global variables
 */
typedef enum {
    TURN, // 目標座標の方位へ向くまで旋回
    MOVE, // 目標座標に到達するまで前進
    END // 構造体に格納されている座標を移動しきったら停止
} RUN_STATE;

typedef enum {
    NO_DETECTION, //見検知
    OUTER, // サークルの外側
    MIDDLE, // 内側の白い円
    INNER // サークルの内側の円
} CIRCLE_POSITION;

// 角度の算出に利用
//static float GRID_X = 250; // 開発部の床が50cm  225.0; // 横(45cm) その中間点をとって22.5
//static float GRID_Y = 250; // 開発部の床が50cm   200.0; // 縦(40cm) その中間点をとって20.0
// コンストラクタ

BlockZone::BlockZone() :
colorSensor(color_sensor),
sl(walker.getCountL(), walker.getCountR(), false) {
}

void BlockZone::prepareMoveData(FILE* bt) {
    bluetooth.fetchDijkstraData(bt);
}

void BlockZone::start() {
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
    static RUN_STATE state = TURN;
    TurnControl turnControl;
    int8_t turn;

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
        {2, 0, 0},
        {2, 1, 0}, // 配列の指示が間違えてrう！！
        {2, 2, 0},
        {2, 3, 0},
        {2, 4, 0},
        {2, 3, 99},
        {1, 4, 0},
        {2, 5, 0},
        {2, 6, 0}
    };

    char msg[128];
    msg_f("start:brockZone", 3);
    bool comment_out = true;
    walker.reset();
    int8_t speed = 10; //20; // 前進時のpwm値
    int cur_gridX = 0; // 現在位置座標のX値 // 20スタート
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
    static CIRCLE_POSITION circle_position = NO_DETECTION;

    // 計測器初期化
    distance.init();
    direction.init();

    // 初期車体向きを設定する
//        direction.setDirection(90.0);

    // 目標座標までの方位、距離を格納
    //    grid.setDistance(2,0,2,0);//cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    //    grid.setDirection(0,0,2,0);//cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
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
                    walker.run(0, 10);
                } else {
                    // 左回転
                    walker.run(0, -10);
                }

                break;
            case MOVE:

                /** 通常ルート：直進する **/
                if (target_grid[grid_count].order == 0) {

                    // センサによる停止判定(前進時のみ使用)
                    if ((cur_dis > (target_dis * 0.8))) {
                        if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                            // 白色、黒色以外を検知したらサークルを検知したと判定する

                            if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
                                switch (circle_position) {
                                    case NO_DETECTION:
                                        circle_position = OUTER;
                                        break;
                                    case MIDDLE:
                                        // 2回目にサークルの色を検知したら目的地到着とする
                                        circle_position = NO_DETECTION;
                                        isDestinationArrival = true;
                                        break;
                                    default:
                                        break;
                                }
                            } else {

                                if (circle_position == OUTER) {
                                    circle_position = MIDDLE;
                                }
                            }
                        } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                            ev3_speaker_play_tone(NOTE_B5, 200);
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
                    if (isDestinationArrival) {
                        ev3_speaker_play_tone(NOTE_A6, 200);
                        if (!(grid_count < (GRID_NUM - 1))) {
                            state = END;
                            break;
                        }

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
                        // 抜ける
                        break;
                    } else if ((cur_dis > target_dis) && (grid_count >= (GRID_NUM - 1))) {
                        state = END;
                    }

                    // 直進
                    //                    walker.run(10, 0);
                    // 前判定なので、進む条件はここに持ってきた@izumi
                    if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                        // 斜め移動は直進走行
                        walker.run(speed, 0);
                    } else {

                        switch (circle_position) {
                            case OUTER:
                            case MIDDLE:
                                turn = 0;
                                break;
                            default:
                                if (cur_dis > (target_dis * 0.8)) {
                                    // サークルの色検知とライントレースが干渉するため、目的距離の80%進んだ段階でライントレースを止める
                                    turn = 0;
                                } else {
                                    // ライン上の移動はライントレース
                                    turn = turnControl.calculateTurnForPid(speed, colorSensor.getBrightness());
                                }
                                break;
                        }

                        walker.run(speed, turn);
                    }

                    colorSensor.getRawColor(rgb);
                    hsv = GetHsv(rgb.r, rgb.g, rgb.b);
                    color = GetColorForHsv(hsv);

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
//    return (x != target_x) ^ (y != target_y); //排他的論理和
}
