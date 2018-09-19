#include <t_stddef.h>

#include "Scenario.h"

Scenario::Scenario(int _totalStep)
    : totalStep(_totalStep)
    , currentStep(0)
{
    scenes = new Scene*[totalStep];
}

Scenario::~Scenario()
{
    for (int i = 0; i < totalStep; i++) {
        delete scenes[i];
    }
    delete[] scenes;
}

Scene* Scenario::GetCurrentScene(void)
{
    return scenes[currentStep];
}

bool Scenario::UpdateStep(void) {
    
    if(currentStep >= (totalStep - 1)) {
        return false;
    }
    
    currentStep++;
    
    return true;
}
