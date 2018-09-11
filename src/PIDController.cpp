#include "PIDController.h"

PIDController::PIDController(float _dt):
    delta_t(_dt),
    kp(0.0F), ki(0.0F), kd(0.0F),
    integral(0.0F),
    diff{0.0F, 0.0F}
{
    
}

PIDController::~PIDController()
{
    
}

PIDController::SetGain(const PIDParam *param)
{
    kp = param->P;
    ki = param->I;
    kd = param->D;
}

PIDController::Reset()
{
    diff[0] = 0.0;
    diff[1] = 0.0;
    integral = 0.0;
}

float PIDController::GetOperationAmount(float measured_val, float target_val)
{
    float p, i, d = 0;
    
    diff[0] = diff[1];
    diff[1] = target_val - measured_val;
    
    integral += (diff[1] + diff[0]) / 2.0 * delta_t;
    
    p = kp * diff[1];
    i = ki * integral;
    d = kd * ((diff[1] - diff[0]) / delta_t);
    
    /*
     * D項は値が大きくなりすぎる傾向があるため、LPFを入れる
     * http://hi-ctrl.hatenablog.com/entry/2018/02/25/020939
    d = d / (1.0 + (0.125 * Kd * (float)(diff[1] - diff[0]) / delta_t));
    */
    
    return (p + i + d);
}
