#include "TailMotorController.h"

TailMotorController::TailMotorController(motor_port_t _motor_port)
    : motor_port(_motor_port)
{
    
}

TailMotorController::~TailMotorController()
{

}

void TailMotorController::Initialize(void)
{
    ev3_motor_config(motor_port, LARGE_MOTOR);
}
