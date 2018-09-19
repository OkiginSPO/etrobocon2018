#ifndef SCENARIO_H
#define SCENARIO_H

#include "Scene.h"

class Scenario
{
    public:
        Scenario(int _totalStep);
        virtual ~Scenario();
        Scene* GetCurrentScene(void);
        bool UpdateStep(void);
        
    private:
        const int totalStep;
        int currentStep;
        
    protected:
        Scene **scenes;
};

#endif /* SCENARIO_H */

