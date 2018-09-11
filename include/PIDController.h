#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include "PIDParam.h"

class PIDController
{
    public:
        PIDController(float _dt);
        ~PIDController();
        void SetGain(const PIDParam *param);
        void Reset(void);
        float GetOperationAmount(float measured_val, float target_val);
        
    private:
        const float delta_t;
        float kp, ki, kd;
        float integral;
        float diff[2];
};

#endif /* PIDCONTROLLER_H */
