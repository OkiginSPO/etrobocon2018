#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "Scene.h"

class ScenarioManager
{
    public:
        ScenarioManager(int _totalStep);
        ~ScenarioManager();
        void CreateRcourse(void);
        void CreateLcourse(void);
        Scene* GetCurrentScene(void);
        bool UpdateStep(void);
        
    private:
        const int totalStep;
        int currentStep;
        
    protected:
        Scene **scenes;
};

#endif /* SCENARIOMANAGER_H */

