#include "SonarSensorController.h"

SonarSensorController::SonarSensorController(sensor_port_t _sensor_port)
    : sensor_port(_sensor_port)
{
    ev3_sensor_config(sensor_port, ULTRASONIC_SENSOR);
}

SonarSensorController::~SonarSensorController()
{

}

void SonarSensorController::Initialize(void)
{

}

