#include "common.h"
#include "test.h"

// コンストラクタ
Test::Test()
{

}

//
void Test::start()
{
  ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
  /* LCD画面表示 */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("EV3way-ET sample_cpp", 0, CALIB_FONT_HEIGHT*1);
}
