#include <t_stddef.h>

#include "Scenario.h"

Scenario::Scenario()
    : currentStep(0)
{
    
}

Scenario::~Scenario()
{

}

Scene* Scenario::GetCurrentScene(void)
{
    return scenes[currentStep];
}

bool Scenario::UpdateStep(void) {
    int totalStep = (sizeof(scenes) / sizeof(*scenes));
    
    if(currentStep >= (totalStep - 1)) {
        return false;
    }
    
    currentStep++;
    
    return true;
}
