#include "common.h"
#include "test.h"

// コンストラクタ

Test::Test() :
sl(walker.getCountL(), walker.getCountR(), false) {
}

//

void Test::start() {
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

    /* LCD画面表示 */
    //ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    //ev3_lcd_draw_string("EV3way-ET sample_cpp", 0, CALIB_FONT_HEIGHT*1);
    init_f("test");
    msg_f("start", 2);
    blockZone.start();

    //  int32_t distance_total = 0;
    //  char msg[32];
    //
    //  while (1) {
    //    sprintf(msg, "distance:%d", (int)distance_total);
    //    msg_f(msg, 3);
    //
    //    sprintf(msg, "positionX:%d", (int)sl.getPointX());
    //    msg_f(msg, 5);
    //
    //    // 現在の走行距離を取得する。
    //    distance_total = distance.getDistanceTotal(walker.getCountL(), walker.getCountR());
    //
    //    // 走行距離をディスプレイに表示する。
    //    sprintf(msg, "distance:%d", (int)distance_total);
    //    msg_f(msg, 3);
    //
    //    // PID値を調整？
    //    sl.update(walker.getCountL(), walker.getCountR());
    //    sprintf(msg, "1distance:%d", (int)distance_total);
    //    msg_f(msg, 3);
    //
    //    // PWM値を10で、0(直進)する。
    //    walker.run(10, 0);
    //    sprintf(msg, "2distance:%d, R:%d, L:%d", (int)distance_total, (int)walker.getCountR(), (int)walker.getCountL());
    //    msg_f(msg, 3);
    //
    //    // 現在の走行距離が1000(100cm)だった場合は、ループを抜ける。
    //    if (distance_total > 1000) {
    //      break;
    //    }
    //
    //    tslp_tsk(4);
    //  }
    //
    //  walker.stop();

}
