#include "BlockZone.h"
#include "string"
#include "stdlib.h"
#include "TurnControl.h"
#include "PerpendicularParking.h"
#include "MotorWheel.h"

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

static BLOCKZONE_STATE blockzone_state = BEFORE;
static CIRCLE_POSITION circle_position = NO_DETECTION;
static CIRCLE_POSITION before_circle_position = NO_DETECTION;
static RUN_STATE state = TURN;
static RUN_STATE before_state = TURN;
static colorid_t color;

//                [現実]                  //                [理想]
// [6,0][6,1][6,2][6,3][6,4][6,5][6,6]   // [0,6][1,6][2,6][3,6][4,6][5,6][6,6]
// [5,0][5,1][5,2][5,3][5,4][5,5][5,6]   // [0,5][1,5][2,5][3,5][4,5][5,5][6,5]
// [4,0][4,1][4,2][4,3][4,4][4,5][4,6]   // [0,4][1,4][2,4][3,4][4,4][5,4][6,4]
// [3,0][3,1][3,2][3,3][3,4][3,5][3,6]   // [0,3][1,3][2,3][3,3][4,3][5,3][6,3]
// [2,0][2,1][2,2][2,3][2,4][2,5][2,6]   // [0,2][1,2][2,2][3,2][4,2][5,2][6,2]
// [1,0][1,1][1,2][1,3][1,4][1,5][1,6]   // [0,1][1,1][2,1][3,1][4,1][5,1][6,1]
// [0,0][0,1][0,2][0,3][0,4][0,5][0,6]   // [0,0][1,0][2,0][3,0][4,0][5,0][6,0]
//static GRID_XY target_grid[100] = {
//    //    {2, 0, 0},
//    {2, 1, 0},
//    {2, 2, 0},
//    {2, 3, 0},
//    {2, 4, 0},
//    {2, 3, 99},
//    {1, 4, 0},
//    {2, 5, 0},
//    {2, 6, 98}
//};
// static GRID_XY target_grid[100] = {
//     //    {2, 0, 0},
//     {2, 1, 0},
//     {2, 2, 0},
//     {2, 1, 99},
//     {2, 0, 0},
//     {3, 0, 0},
//     {4, 0, 0},
//     {4, 1, 0},
//     {4, 2, 0},
//     {4, 3, 0},
//     {4, 4, 0},
//     {4, 5, 0},
//     {4, 6, 0},
//     {4, 5, 99},
//     {3, 6, 0},
//     {2, 6, 98}
// };

static char msg[128];
static int cur_gridX = 2; // 現在位置座標のX値 // 20スタート
static int cur_gridY = 0; // 現在位置座標のY値
static float target_dis = 0.0; // 現在位置座標から目標座標までの距離
static float target_dir = 0.0; // 現在位置座標から目標座標までの方位
static float cur_dis = 0.0; // 距離計の現在地
static float cur_dir = 0.0; // 方位計の現在地
static int grid_count = 0; // 目標座標構造体への参照カウンタ
static bool isDestinationArrival = false;
static int right_drive = -1; // プラス：右エッジ　マイナス：左エッジ 初期値左エッジ
static int right_turn = -1; // プラス：右ターン　マイナス：左ターン　初期値左ターン(意味はない)
static int speed = 10; // 前進時のpwm値
static int pidTurnValue = 0;
static rgb_raw_t rgb;
static HSV hsv = {0, 0, 0};
static int8_t turn;

static TurnControl *turnControl;

// Pid走行関連
static double_t DELTA_T = 0.004;
static double_t KP = 0.38; //1;
static double_t KI = 0; //0;
static double_t KD = 0; //0;
static double_t diff[2];
static double_t integral;

void BlockZone::PidReset(int32_t p, int32_t i, int32_t d) {
    KP = p;
    KI = i;
    KD = d;
    diff[0] = 0.0;
    diff[1] = 0.0;
    integral = 0.0;
}

int32_t BlockZone::pid_sample(int32_t sensor_val, int32_t target_val) {
    int32_t p, i, d;
    // TODO: PID値をある程度決定するまっすぐすすみたい！！！！！！！！！！！！！！
    // p = 0.6, i = 0, d = 0.008 →ほぼまっすぐ！！
    double kp = 0.6; //0.4;
    double ki = 0.0; //0;
    double kd = 0.008; //0;
    double delta_t = 0.004;
    // 右側ライントレース
    diff[0] = diff[1];
    diff[1] = sensor_val - target_val;
    integral += (diff[1] + diff[0]) / 2.0 * delta_t;
    p = kp * diff[1];
    i = ki * integral;
    d = kd * (diff[1] - diff[0]) / delta_t;
    return limit_math(p + i + d);
}

int32_t BlockZone::limit_math(double_t num) {
    if (num < -100) {
        return -100;
    } else if (100 < num) {
        return 100;
    } else {
        return (int) num;
    }
}

// コンストラクタ

BlockZone::BlockZone() :
colorSensor(color_sensor),
sl(walker.getCountL(), walker.getCountR(), false) {
}

///BlockZone::start()以前に呼び出されてダイクストラ情報をBluetoothより取得する

void BlockZone::prepareMoveData(FILE* bt) {
    Bluetooth bluetooth;
    bluetooth.fetchDijkstraData(bt, target_grid);
}

BLOCKZONE_STATE BlockZone::GetBlockZoneState() {
    return blockzone_state;
}

// BEFORE

bool BlockZone::MovingStartPosition() {
    // TODO: スタートはOK
    int my_turn = -pid_sample(colorSensor.getBrightness(), 20);
    //     赤サークル検知までライントレース
    colorSensor.getRawColor(rgb);
    hsv = GetHsv(rgb.r, rgb.g, rgb.b);
    color = GetColorForHsv(hsv);
    if (color == COLOR_RED) {
        switch (circle_position) {
            case NO_DETECTION:
                circle_position = OUTER;
                break;
            case MIDDLE:
                // 2回目にサークルの色を検知したら目的地到着とする
                // 赤検知で終了
                circle_position = NO_DETECTION;
                walker.stop();
                blockzone_state = INITIALIZE;
                break;
            default:
                break;
        }
    } else {
        if (circle_position == OUTER) {
            circle_position = MIDDLE;
        }
    }

    // 左側ライントレース(マイナス値)
    if (circle_position == NO_DETECTION) {
        walker.run(speed, my_turn);
    } else {
        if (before_circle_position != circle_position) {
            ev3_speaker_play_tone(NOTE_AS4, 1);
            walker.stop();
            before_circle_position = circle_position;
        }
        walker.run(10, 0);
    }
    return false;
}

// INITIALIZE

void BlockZone::MoveBlockZoneInit() {
    circle_position = NO_DETECTION;
    PidReset(0.4, 0, 0.68);
    // ブロック並べエリアから使用する
    cur_gridX = 2; // 現在位置座標のX値 // 20スタート
    cur_gridY = 0; // 現在位置座標のY値
    target_dis = 0.0; // 現在位置座標から目標座標までの距離
    target_dir = 0.0; // 現在位置座標から目標座標までの方位
    cur_dis = 0.0; // 距離計の現在地
    cur_dir = 0.0; // 方位計の現在地
    grid_count = 0; // 目標座標構造体への参照カウンタ
    isDestinationArrival = false;
    right_drive = -1; // プラス：右側　マイナス：左側
    circle_position = NO_DETECTION;
    hsv = {0, 0, 0};
    speed = 10; // 前進時のpwm値
    state = TURN;
    turn = 0;
    pidTurnValue = 0;
    walker.reset();
    // 目標座標までの方位、距離を格納
    grid.setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);

    target_dis = grid.getDistance();
    target_dir = grid.getDirection();

    // 計測器初期化
    distance.init();
    direction.init();

    /* 初期化完了通知 */
    //    ev3_led_set_color(LED_ORANGE);
    blockzone_state = BLOCKZONE;
}

// BLOCKZONE

bool BlockZone::MoveBlockZone() {
    /* 計測器更新 */
    distance.update();
    direction.update();

    // 計測器の現在地を格納
    cur_dis = distance.getDistance();
    cur_dir = direction.getDirection();

    pidTurnValue = pid_sample(colorSensor.getBrightness(), 20) * right_drive;
    sprintf(msg, "state:%d", (int) state);
    msg_f(msg, 3);
    switch (state) {
            // ッターン！
        case TURN:


            if ((abs(target_dir - cur_dir) < 2.0)) {
                // 90度ターンの際には、黒ライン検知でもターン終了判定を行う
                if ((2 <= grid_count)
                        && IsMove90Turn(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)
                        && (color == COLOR_BLACK)) {
                    state = MOVE;
                    turnControl = new TurnControl();

                    // pidリセット
                    //                    PidReset(0.4, 0, 0.68);
                    break;
                } else if (abs(target_dir - cur_dir) < 1.0) {
                    state = MOVE;
                    turnControl = new TurnControl();

                    // pidリセット
                    //                    PidReset(0.4, 0, 0.68);
                    break;
                }
            }


            if (target_grid[grid_count].order == 99) {
                // バック時はエッジ側に回転する
                if (cur_dir <= target_dir) {
                    // 右回り
                    walker.run(0, 10);
                } else {
                    // 左回り
                    walker.run(0, -10);
                }
            } else {
                if (target_dir <= cur_dir) {
                    // 右回り
                    walker.run(0, 10);
                } else {
                    // 左回り
                    walker.run(0, -10);
                }
            }

            //            if ((2 <= grid_count)
            //                    && IsOneEigty(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY, target_grid[grid_count - 1].order, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
            //                // 180度回転の場合は、必ず黒線検知でのみMOVEへ移行する    
            //                if ((fabs(cur_dir - target_dir) < 40) &&(color == COLOR_BLACK)) {
            //                    state = MOVE;
            //                    turnControl = new TurnControl();
            //                    break;
            //                }
            //            } else {
            //                ev3_speaker_play_tone(NOTE_AS4, 2);
            //                if ((2 <= grid_count)
            //                        && (IsMove90Turn(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY))
            //                        && (color = COLOR_BLACK)) {
            //                    // 90度回転の場合は、黒線検知でもMOVEへ移行できる(回転過剰を防ぐ))
            //                    state = MOVE;
            //                    turnControl = new TurnControl();
            //                    break;
            //                } else {
            //                    // 通常時は、角度がきたらMOVEへへ移行する
            //                    if (fabs(target_dir - cur_dir) < 1.0) {
            //                        state = MOVE;
            //                        turnControl = new TurnControl();
            //                        // TODO: PIDリセットするかを検証する
            //                        break;
            //                    }
            //                }
            //            }
            //            if ((2 <= grid_count)
            //                    && IsOneEigty(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY, target_grid[grid_count - 1].order, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
            //                walker.run(0, (0 < right_turn) ? -10 : 10);
            //            } else {
            //                if (cur_dir >= target_dir) {
            //                    walker.run(0, 10);
            //                } else {
            //                    walker.run(0, -10);
            //                }
            //            }

            break;
        case MOVE:
            /** 通常ルート：直進する **/
            if (target_grid[grid_count].order == 0 || target_grid[grid_count].order == 98) {

                // センサによる停止判定(前進時のみ使用)
                if (IsGoToCircle(target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                    // 白色、黒色以外を検知したらサークルを検知したと判定する

                    if (color == COLOR_RED || color == COLOR_GREEN || color == COLOR_YELLOW || color == COLOR_BLUE) {
                        switch (circle_position) {
                            case NO_DETECTION:
                                // 次の移動が後退(order=99)の場合は、ブロックを置いて逃げるため1つ目の検知で終了判定へ進む
                                if ((target_grid[grid_count].order != 98) && (target_grid[grid_count + 1].order == 99)) {
                                    // TODO: 後退距離調整
                                    isDestinationArrival = true;
                                } else {
                                    circle_position = OUTER;
                                }
                                break;
                            case MIDDLE:
                                // 2回目にサークルの色を検知したら目的地到着とする
                                circle_position = NO_DETECTION;
                                isDestinationArrival = true;
                                break;
                            default:
                                break;
                        }
                    } else if (circle_position == OUTER) {
                        // ここを通らないようにする（１つ目のライン検知でOK
                        circle_position = MIDDLE;
                    }
                } else if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)
                        && ((target_dis * 0.4) <= cur_dis)) {
                    // 黒色を検知したらラインを検知したと判定する
                    if (color == COLOR_BLACK) {
                        isDestinationArrival = true;
                    }
                } else {
                    // サークルから黒ラインに沿って仮想点に移動している際等
                    // TODO:このif-elseの中ではなく、包括的に距離で停止処理を組み込んだ方がいいと思う。その際は移動させる必要があります
                    if (cur_dis >= target_dis) {
                        isDestinationArrival = true;
                    }
                }
                //                }

                // 指定位置までたどり着いたら状態遷移
                if (isDestinationArrival) {
                    ev3_speaker_play_tone(NOTE_A6, 1);

                    // 最後の移動が完了したらENDへ移行
                    if (target_grid[grid_count].order == 98) {
                        ev3_speaker_play_tone(NOTE_FS4, 1);
                        cur_gridX = target_grid[grid_count].gridX;
                        cur_gridY = target_grid[grid_count].gridY;

                        // 計測器のリセット
                        distance.init();
                        direction.setDirection(target_dir);

                        // 次の座標までの方位、距離を格納する
                        grid_count++;

                        /** 通常ルート:次の進行方向へ回転するため、現在座標情報・目標座標を格納する **/
                        GRID_XY last_target = {2, 7, 0};
                        grid.setDistance(cur_gridX, cur_gridY, last_target.gridX, last_target.gridY);
                        grid.setDirection(cur_gridX, cur_gridY, last_target.gridX, last_target.gridY);
                        target_dis = grid.getDistance();
                        target_dir = grid.getDirection();

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

                    int beforeDir = target_dir;

                    grid.setDistance(grid.CalcDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY) - 25.0);
                    grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                    target_dis = grid.getDistance();
                    target_dir = grid.getDirection();


                    // ライントレースのエッジと初めの回転向きを決定する
                    if ((fabs(beforeDir) - fabs(target_dir)) < 4) {
                        // 直進時->エッジそのまま
                        // 後退時->エッジ反転
                        right_drive = (target_grid[grid_count].order == 99) ? -right_drive : right_drive;
                        right_turn = right_drive;
                    } else if ((2 <= grid_count)
                            &&(IsOneEigty(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY, target_grid[grid_count - 1].order, target_grid[grid_count].gridX, target_grid[grid_count].gridY))) {
                        // 180度回転のときはエッジを反転する(回転角度が大きくなりすぎるため)
                        right_drive = right_drive;
                        right_turn = right_drive;
                    } else {
                        if (beforeDir < target_dir) {
                            right_drive = -1;
                            right_turn = right_drive;
                        } else {
                            right_drive = 1;
                            right_turn = right_drive;
                        }
                    }

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
                }

                // 直進
                // 前判定なので、進む条件はここに持ってきた@izumi
                if (IsGoToLine(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY)) {
                    // 斜め移動は直進走行
                    walker.run(speed, 0);
                } else {
                    if (circle_position == NO_DETECTION) {
                        turn = pidTurnValue;
                    } else {
                        if (before_circle_position != circle_position) {
                            ev3_speaker_play_tone(NOTE_AS4, 1);
                            walker.stop();
                            before_circle_position = circle_position;
                        }
                        turn = 0;
                    }
                    walker.run(speed, turn);
                }

            } else if (target_grid[grid_count].order == 99) {
                /** 後退時 **/
                //                ev3_speaker_play_tone(NOTE_F4, 1);
                // 指定位置までたどり着いたら状態遷移(distance各値は絶対値で判断する)
                if ((fabs(cur_dis) > fabs(target_dis))) {

                    // 現在位置座標を更新
                    cur_gridX = target_grid[grid_count].gridX;
                    cur_gridY = target_grid[grid_count].gridY;

                    // 計測器のリセット
                    distance.init();
                    // 現在の車体の向きをセットする。※バック時は車体の向きが違うため、ここで調整を行っています
                    direction.setDirection(grid.CalcDirection(target_grid[grid_count - 2].gridX, target_grid[grid_count - 2].gridY, target_grid[grid_count - 1].gridX, target_grid[grid_count - 1].gridY));

                    grid_count++;

                    //次の進行方向へ回転するため、現在座標情報・目標座標を格納する
                    grid.setDistance(grid.CalcDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY) - 25.0);
                    grid.setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                    target_dis = grid.getDistance();
                    target_dir = grid.getDirection();

                    // 再度、次座標への旋回を開始
                    state = TURN;
                    isDestinationArrival = false;
                    break;
                }

                // バック
                walker.run(-10, 0);
            }

            break;
        case END:
            // 直角駐車場のスタート位置へ移動する
            if (fabs(target_dir - cur_dir) < 1.0) {
                blockzone_state = AFTER;
                //                return false;
            } else {
                // 回転
                if (cur_dir >= target_dir) {
                    walker.run(0, 10);
                } else {
                    walker.run(0, -10);
                }
            }

            break;
        default:
            break;
    }

    colorSensor.getRawColor(rgb);
    hsv = GetHsv(rgb.r, rgb.g, rgb.b);
    color = GetColorForHsv(hsv);

    // 処理継続

    return true;
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

/**
 * サークルへの移動判定 例：{2,1} -> {2,2}
 * @param x 目標座標x
 * @param y 目標座標y
 * @return サークルのある座標ならtrue
 */
bool BlockZone::IsGoToCircle(int8_t x, int8_t y) {

    return (x % 2 == 0) && (y % 2 == 0);
}

/**
 * 斜め移動判定 例：{2,1} -> {1,2}
 * @param x 現在座標X
 * @param y 現在座標y
 * @param target_x 目標座標x
 * @param target_y 目標座標y
 * @return 斜め移動判定であればtrue
 */
bool BlockZone::IsGoToLine(int8_t x, int8_t y, int8_t target_x, int8_t target_y) {

    return (x != target_x) && (y != target_y);
}

// ライン上を動きます

bool BlockZone::IsOneTheLine(int8_t x, int8_t y, int8_t target_x, int8_t target_y) {

    return (x != target_x) ^ (y != target_y); // 排他的論理和
}

bool BlockZone::IsMove90Turn(int8_t before_x, int8_t before_y, int8_t x, int8_t y, int8_t target_x, int8_t target_y) {

    return ((x != target_x) ^ (y != target_y))&& ((x != before_x) ^ (y != before_y));
}

// 180度回転

bool BlockZone::IsOneEigty(int8_t beforeTo2_x, int8_t beforeTo2_y, int8_t before_x, int8_t before_y, int8_t before_order, int8_t target_x, int8_t target_y) {
    if (before_order == 99) {
        if ((beforeTo2_x != target_x) ^ (beforeTo2_y != target_y)) {
            return (beforeTo2_x == before_x == target_x) || (beforeTo2_y == before_y == target_y);
        }
    }
    return false;
}
