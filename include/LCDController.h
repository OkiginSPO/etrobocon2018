#ifndef LCDCONTROLLER_H
#define LCDCONTROLLER_H

#include "ev3api.h"

class LCDController
{
    public:
        LCDController();
        ~LCDController();
        void Initialize(void);
        void WriteMessage(const char *msg);
        void Clear(void);
        
    private:
        const lcdfont_t fontSize;
        const lcdcolor_t lcdColor;
        int fontWidth;
        int fontHeight;
        int lineNum;
        
        int GetLineAreaHeight(void);
};

#endif /* LCDCONTROLLER_H */

