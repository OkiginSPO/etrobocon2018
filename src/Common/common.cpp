#include "common.h"
#include <stdarg.h>

// 初期処理用
void init_f(const char *str) {
  // フォントの設定と0行目の表示
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(str, 0, 0);
}

/**
 * 行単位で引数の文字列を表示
 * @param str 表示する文字列
 * @param line 20ドットごとの行番号（1から5）
 */
void msg_f(const char *str, int32_t line) {
  const int8_t line_height = 15;
  ev3_lcd_fill_rect(0, line * line_height, EV3_LCD_WIDTH, line_height, EV3_LCD_WHITE);
  ev3_lcd_draw_string(str, 0, line * line_height);
}

int max(int len, ...) {
    int i, _max, num;
    va_list ap;
    va_start(ap, len);
    
    _max = va_arg(ap, int);
    
    for(i = 1; i < len; i++) {
        num = va_arg(ap, int);
        if (num > _max) {
            _max = num;
        }
    }
    va_end(ap);
    
    return _max;
}

int min(int len, ...) {
    int i, _min, num;
    va_list ap;
    
    va_start(ap, len);
    
    _min = va_arg(ap, int);
    
    for(i = 1; i < len; i++) {
        num = va_arg(ap, int);
        if (num < _min) {
            _min = num;
        }
    }
    va_end(ap);
    
    return _min;
}
