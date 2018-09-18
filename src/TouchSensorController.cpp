#include "TouchSensorController.h"

TouchSensorController::TouchSensorController(sensor_port_t _sensor_port)
    : sensor_port(_sensor_port)
{
    
}

TouchSensorController::~TouchSensorController()
{
    
}

void TouchSensorController::Initialize(void)
{
    ev3_sensor_config(sensor_port, TOUCH_SENSOR);
}

bool TouchSensorController::IsPressed(void)
{
    bool isPressed = ev3_touch_sensor_is_pressed(sensor_port);
    
    return isPressed;
}

