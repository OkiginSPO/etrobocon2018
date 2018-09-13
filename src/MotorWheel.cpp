#include "WalkerParam.h"
#include "MotorWheel.h"

MotorWheel::MotorWheel(motor_port_t _motor_port)
    : motor_port(_motor_port)
    , lastAngle(0.0F)
    , distanceDelta(0.0F)
    , distanceTotal(0.0F)
    , currentPWM(0)
{
    ev3_motor_config(motor_port, LARGE_MOTOR);
}

MotorWheel::~MotorWheel()
{
    
}

void MotorWheel::Initialize(void)
{
    distanceDelta = 0.0;
    distanceTotal = 0.0;
    lastAngle = ev3_motor_get_counts(motor_port);
}

void MotorWheel::Run(int pwm)
{
    if(currentPWM == pwm) {
        return;
    }
    
    ev3_motor_set_power(motor_port, pwm);
    
    currentPWM = pwm;
}

void MotorWheel::Stop(bool _brake)
{
    ev3_motor_stop(motor_port, _brake);
}

void MotorWheel::UpdateDistance(void)
{
    float currentAngle = ev3_motor_get_counts(motor_port);
    distanceDelta = ((PI * WHEEL_DIAMETER) / 360.0) * (currentAngle - lastAngle);
    distanceTotal += distanceDelta;
    
    lastAngle = currentAngle;
}

float MotorWheel::GetDistanceDelta(void) const
{
    return distanceDelta;
}

float MotorWheel::GetDistanceTotal(void) const
{
    return distanceTotal;
}
