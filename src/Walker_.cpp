#include <ev3api.h>
#include "PortSettings.h"
#include "Walker_.h"
#include "Utility.h"

#define COURSE_R
//#define COURSE_L

Walker_::Walker_(Scenario *_scenario)
    : DELTA_T(0.004F)
    , targetBlightness(20)
    , terminated(false)
    , scenario(_scenario)
{

}

Walker_::~Walker_()
{
    delete leftWheel;
    delete rightWheel;
    delete localization;
    delete colorSensorController;
    delete pidController;
    delete touchSensorController;
}

// 走行用のモジュールは分けたほうがいい気がするけどとりあえず
void Walker_::Initialize(void)
{
    leftWheel = new MotorWheel(WHEEL_LEFT_MOTOR);
    rightWheel = new MotorWheel(WHEEL_RIGHT_MOTOR);
    localization = new Localization(leftWheel, rightWheel);
    colorSensorController = new ColorSensorController(PORT_COLOR_SENSOR);
    pidController = new PIDController(DELTA_T);
    touchSensorController = new TouchSensorController(PORT_TOUCH_SENSOR);
}

void Walker_::WaitForStart(void)
{
    while (true) {
        if(touchSensorController->IsPressed()) {
            break;
        }
        tslp_tsk(10);
    }
}

void Walker_::RunOnOff(void)
{
    int pwm = 100 / 6;
    float kp = 0.83F;
    int blightness = colorSensorController->GetBrightness();
    int diff = blightness - targetBlightness;
    
    int operation = diff * kp;
    
    int pwmL = pwm + operation;
    int pwmR = pwm - operation;
    
    localization->UpdateDistance();
    
    float distance = localization->GetDistance();
    float targetDis = 0.0F;
    
#ifdef COURSE_R
    targetDis = 6092.2F * 2.0F;
#endif

#ifdef COURSE_L
    targetDis = 4092.0F * 2.0F;
#endif
    
    if (distance <= targetDis) {
        leftWheel->Run(pwmL);
        rightWheel->Run(pwmR);
    } else {
        leftWheel->Stop(true);
        rightWheel->Stop(true);
        terminated = true;
    }
}

void Walker_::LineTrace(void)
{
    float scenarioDistance;
    RUN_STATE runState = RUN_STATE::AHEAD;
    Scene *currentScene;
    
    localization->UpdateDistance();
    
    float lenc = leftWheel->GetLastAngle();
    float renc = rightWheel->GetLastAngle();
    
    currentScene = scenario->GetCurrentScene();
    scenarioDistance = localization->GetDistance();
    
    if (scenarioDistance >= (currentScene->GetDistance() * 2.0F)) {
        bool updated = true;
        localization->Reset();
        updated = scenario->UpdateStep();
        pidController->Reset();
        ev3_speaker_play_tone(NOTE_C4, 1000);
        
        if(!updated) {
            runState = RUN_STATE::STOP;
        }
    }
    
    pidController->SetGain(currentScene->GetPIDParam());
    
    int blightness = 0;
    float pidValue = 0.0F;
    int operation = 0;
    int pwmR = 0;
    int pwmL = 0;
    
    switch(runState) {
        case RUN_STATE::AHEAD:
            blightness = colorSensorController->GetBrightness();
            pidValue = pidController->GetOperationAmount(blightness, targetBlightness);
            operation = Utility::math_limit((int)pidValue, 0, 100);
            pwmL = (currentScene->GetForward() + operation);
            pwmR = (currentScene->GetForward() - operation);
            
            leftWheel->Run(pwmL);
            rightWheel->Run(pwmR);
            break;
        case RUN_STATE::STOP:
            leftWheel->Stop(true);
            rightWheel->Stop(true);
            terminated = true;
            break;
        default:
            break;
    }
}

bool Walker_::Terminated(void) {
    return terminated;
}

