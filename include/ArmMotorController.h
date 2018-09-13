#ifndef ARMMOTORCONTROLLER_H
#define ARMMOTORCONTROLLER_H

#include "PortSettings.h"

class ArmMotorController
{
    public:
        ArmMotorController(motor_port_t _motor_port);
        ~ArmMotorController();
        void Initialize(void);
        
    private:
        motor_port_t motor_port;
};

#endif /* ARMMOTORCONTROLLER_H */
