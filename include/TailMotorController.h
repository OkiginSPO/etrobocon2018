#ifndef TAILMOTORCONTROLLER_H
#define TAILMOTORCONTROLLER_H

#include "PortSettings.h"

class TailMotorController
{
    public:
        TailMotorController(motor_port_t _motor_port);
        ~TailMotorController();
        void Initialize(void);
    
    private:
        motor_port_t motor_port;
};

#endif /* TAILMOTORCONTROLLER_H */

