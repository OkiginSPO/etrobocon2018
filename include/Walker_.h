#ifndef WALKER__H
#define WALKER__H

#include "MotorWheel.h"
#include "Localization.h"
#include "ColorSensorController.h"
#include "PIDController.h"

class Walker_
{
    public:
        Walker_();
        ~Walker_();
        void Initialize(void);
    
    private:
        const float DELTA_T;
        MotorWheel *leftWheel;
        MotorWheel *rightWheel;
        Localization *localization;
        ColorSensorController *colorSensorController;
        PIDController *pidController;
};

#endif /* WALKER__H */

