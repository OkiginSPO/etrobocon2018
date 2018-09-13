#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "MotorWheel.h"

class Localization
{
    public:
        explicit Localization(const MotorWheel *_leftWheel, const MotorWheel *_rightWheel);
        ~Localization();
        float GetDistance(void);
        
    private:
        const MotorWheel *leftWheel;
        const MotorWheel *rightWheel;
};

#endif /* LOCALIZATION_H */
