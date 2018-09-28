#include "LCDController.h"

void LCDController::Initialize(void)
{
    ev3_lcd_set_font(fontSize);
    
    ev3_font_get_size(fontSize, (int32_t *)&fontWidth, (int32_t *)&fontHeight);
}

void LCDController::WriteMessage(const char *msg)
{   
    if (GetLineAreaHeight() >= EV3_LCD_HEIGHT) {
        Clear();
    }
    
    ev3_lcd_draw_string(msg, 0, GetLineAreaHeight());
    
    lineNum++;
}

void LCDController::Clear()
{
    int linePosY = 0;
    
    for(int i = 0; i < lineNum; i++) {
        linePosY = fontHeight * i;
        
        ev3_lcd_draw_string((char *)"", 0, linePosY);
    }
    
    lineNum = 0;
    
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, lcdColor);
}

int LCDController::GetLineAreaHeight()
{
    int lineAreaHeight = lineNum * fontHeight;
    return lineAreaHeight;
}

int LCDController::fontWidth = 0;
int LCDController::fontHeight = 0;
int LCDController::lineNum = 0;