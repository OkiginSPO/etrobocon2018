#include "ColorSensorController.h"

ColorSensorController::ColorSensorController(sensor_port_t _port)
    : sensor_port(_port)
{

}

ColorSensorController::~ColorSensorController()
{
    
}

int ColorSensorController::GetBrightness()
{
    // LPF入れたほうがいいかも？
    int brightness = ev3_color_sensor_get_reflect(sensor_port);
    
    return brightness;
}

rgb_raw_t ColorSensorController::GetRawRGB()
{
    
}

