#ifndef GYROSENSORCONTROLLER_H
#define GYROSENSORCONTROLLER_H

#include "PortSettings.h"

class GyroSensorController
{
    public:
        GyroSensorController(sensor_port_t _sensor_port);
        ~GyroSensorController();
        void Initialize(void);
    
    private:
        sensor_port_t sensor_port;
};

#endif /* GYROSENSORCONTROLLER_H */

