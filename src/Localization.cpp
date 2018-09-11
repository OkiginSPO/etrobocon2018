#include "Localization.h"

Localization::Localization(const MotorWheel* _leftWheel, const MotorWheel* _rightWheel)
    : leftWheel(_leftWheel)
    , rightWheel(_rightWheel)
{
    
}

Localization::~Localization()
{
}

float Localization::GetDistance(void)
{
    float distanceL = leftWheel->GetDistanceTotal();
    float distanceR = rightWheel->GetDistanceTotal();
    float distance = (distanceL + distanceR) / 2.0F;
    
    return distance;
}