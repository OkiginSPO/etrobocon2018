#ifndef LCDCONTROLLER_H
#define LCDCONTROLLER_H

#include "ev3api.h"

class LCDController
{
    public:
        static void Initialize(void);
        static void WriteMessage(const char *msg);
        static void Clear(void);
        
    private:
        static const lcdfont_t fontSize = EV3_FONT_SMALL;
        static const lcdcolor_t lcdColor = EV3_LCD_WHITE;
        static int fontWidth;
        static int fontHeight;
        static int lineNum;
        
        static int GetLineAreaHeight(void);
};

#endif /* LCDCONTROLLER_H */

