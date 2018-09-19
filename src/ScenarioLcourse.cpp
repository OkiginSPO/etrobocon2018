#include "ScenarioLcourse.h"

ScenarioLcourse::ScenarioLcourse()
    : Scenario(5)
{
    scenes[0] = new Scene(0.0F, 0.0F, 0.0F, 0, 0.0F);
    scenes[1] = new Scene(0.0F, 0.0F, 0.0F, 0, 0.0F);
    scenes[2] = new Scene(0.0F, 0.0F, 0.0F, 0, 0.0F);
    scenes[3] = new Scene(0.0F, 0.0F, 0.0F, 0, 0.0F);
    scenes[4] = new Scene(0.0F, 0.0F, 0.0F, 0, 0.0F);
}

ScenarioLcourse::~ScenarioLcourse()
{
    
}

