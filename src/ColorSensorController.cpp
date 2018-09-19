#include "ColorSensorController.h"

ColorSensorController::ColorSensorController(sensor_port_t _port)
    : sensor_port(_port)
{
    ev3_sensor_config(sensor_port, COLOR_SENSOR);
}

ColorSensorController::~ColorSensorController()
{
    
}

int ColorSensorController::GetBrightness(void)
{
    // LPF入れたほうがいいかも？
    int brightness = ev3_color_sensor_get_reflect(sensor_port);
    
    return brightness;
}

rgb_raw_t ColorSensorController::GetRawRGB(void)
{
    rgb_raw_t rgb_raw;
    ev3_color_sensor_get_rgb_raw(sensor_port, &rgb_raw);
    
    return rgb_raw;
}

