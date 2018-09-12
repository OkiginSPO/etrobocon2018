#ifndef TOUCHSENSORCONTROLLER_H
#define TOUCHSENSORCONTROLLER_H

#include "PortSettings.h"

class TouchSensorController
{
    public:
        TouchSensorController(sensor_port_t _sensor_port);
        ~TouchSensorController();
        void Initialize(void);
        bool IsPressed(void);
        
    private:
        sensor_port_t sensor_port;
};

#endif /* TOUCHSENSORCONTROLLER_H */

