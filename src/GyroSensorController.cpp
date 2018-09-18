#include "GyroSensorController.h"

GyroSensorController::GyroSensorController(sensor_port_t _sensor_port)
    : sensor_port(_sensor_port)
{

}

GyroSensorController::~GyroSensorController()
{

}

void GyroSensorController::Initialize(void)
{
    ev3_sensor_config(sensor_port, GYRO_SENSOR);
}
