#ifndef SONARSENSORCONTROLLER_H
#define SONARSENSORCONTROLLER_H

#include "PortSettings.h"


class SonarSensorController
{
    public:
        SonarSensorController(sensor_port_t _sensor_port);
        ~SonarSensorController();
        void Initialize(void);
    
    private:
        sensor_port_t sensor_port;
};

#endif /* SONARSENSORCONTROLLER_H */

