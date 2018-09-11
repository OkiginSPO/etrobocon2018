#ifndef COLORSENSORCONTROLLER_H
#define COLORSENSORCONTROLLER_H

#include "PortSettings.h"

class ColorSensorController
{
    public:
        explicit ColorSensorController(sensor_port_t _port);
        ~ColorSensorController();
        int GetBrightness();
        rgb_raw_t GetRawRGB();
        
    private:
        sensor_port_t sensor_port;
};

#endif /* COLORSENSORCONTROLLER_H */
