#ifndef SCENARIO_H
#define SCENARIO_H

#include "Scene.h"

class Scenario
{
    public:
        Scenario();
        virtual ~Scenario();
        Scene* GetCurrentScene(void);
        bool UpdateStep(void);
        
    private:
        int currentStep;
        
    protected:
        Scene *scenes[];
};

#endif /* SCENARIO_H */

