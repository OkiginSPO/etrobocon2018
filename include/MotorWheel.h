#ifndef MOTORWHEEL_H
#define MOTORWHEEL_H

#include "PortSettings.h"

class MotorWheel
{
public:
    explicit MotorWheel(motor_port_t _motor_port);
    ~MotorWheel();
    void Initialize();
    void Run(int pwm);
    void Stop(bool _brake);
    void UpdateDistance();
    float GetDistanceDelta() const;
    float GetDistanceTotal() const;
    
private:
    motor_port_t motor_port;
    float lastAngle;
    float distanceDelta;
    float distanceTotal;
    int currentPWM;
};

#endif /* MOTORWHEEL_H */
