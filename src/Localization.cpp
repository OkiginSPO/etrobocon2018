#include "Localization.h"

Localization::Localization(MotorWheel* _leftWheel, MotorWheel* _rightWheel)
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

void Localization::UpdateDistance(void)
{
    leftWheel->UpdateDistance();
    rightWheel->UpdateDistance();
}

void Localization::Reset(void)
{
    leftWheel->Initialize();
    rightWheel->Initialize();
}

