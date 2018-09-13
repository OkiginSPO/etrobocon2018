#include "PortSettings.h"
#include "Walker_.h"

Walker_::Walker_()
    : DELTA_T(0.004F)
{

}

Walker_::~Walker_()
{
    delete leftWheel;
    delete rightWheel;
    delete localization;
    delete colorSensorController;
    delete pidController;
}

// 走行用のモジュールは分けたほうがいい気がするけどとりあえず
void Walker_::Initialize(void)
{
    leftWheel = new MotorWheel(WHEEL_LEFT_MOTOR);
    rightWheel = new MotorWheel(WHEEL_RIGHT_MOTOR);
    localization = new Localization(leftWheel, rightWheel);
    colorSensorController = new ColorSensorController(PORT_COLOR_SENSOR);
    pidController = new PIDController(DELTA_T);
}
