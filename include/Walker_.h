#ifndef WALKER__H
#define WALKER__H

#include "MotorWheel.h"
#include "Localization.h"
#include "ColorSensorController.h"
#include "PIDController.h"
#include "TouchSensorController.h"
#include "Scenario.h"
#include "Log.h"

enum class RUN_STATE {
    AHEAD,
    STOP
};

class Walker_
{
    public:
        Walker_(Scenario *_scenario);
        ~Walker_();
        void Initialize(void);
        void WaitForStart(void);
        void RunOnOff(void);
        void LineTrace(void);
        bool Terminated(void);
    
    private:
        const float DELTA_T;
        const int targetBlightness;
        bool terminated;
        
        MotorWheel *leftWheel;
        MotorWheel *rightWheel;
        Localization *localization;
        ColorSensorController *colorSensorController;
        PIDController *pidController;
        TouchSensorController *touchSensorController;
        Scenario *scenario;
        Log *log;
};

#endif /* WALKER__H */

