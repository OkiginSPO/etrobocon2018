#include "ScenarioRcourse.h"

ScenarioRcourse::ScenarioRcourse()
    : Scenario(12)
{
    scenes[0] = new Scene(0.4F, 0.0F, 0.68F, 60, 1068.65F);
    scenes[1] = new Scene(0.82F, 0.15F, 0.1F, 30, 889.5F);
    scenes[2] = new Scene(0.65F, 0.0F, 0.008F, 60, 214.5F);
    scenes[3] = new Scene(0.82F, 0.15F, 0.1F, 30, 287.0F);
    scenes[4] = new Scene(0.82F, 0.15F, 0.1F, 30, 297.0F);
    scenes[5] = new Scene(0.65F, 0.0F, 0.008F, 60, 329.65F);
    scenes[6] = new Scene(0.82F, 0.15F, 0.1F, 30, 579.2F);
    scenes[7] = new Scene(0.7F, 0.0F, 0.15F, 50, 1262.0F);
    scenes[8] = new Scene(0.82F, 0.15F, 0.1F, 30, 259.7F);
    scenes[9] = new Scene(0.7F, 0.0F, 0.15F, 50, 206.0F);
    scenes[10] = new Scene(0.82F, 0.15F, 0.1F, 30, 382.2F);
    scenes[11] = new Scene(0.7F, 0.0F, 0.15F, 50, 316.8F);
}
    
ScenarioRcourse::~ScenarioRcourse()
{

}

