#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "MotorWheel.h"

class Localization
{
    public:
        explicit Localization(MotorWheel *_leftWheel, MotorWheel *_rightWheel);
        ~Localization();
        float GetDistance(void);
        void UpdateDistance(void);
        void Reset(void);
        
    private:
        MotorWheel *leftWheel;
        MotorWheel *rightWheel;
};

#endif /* LOCALIZATION_H */
