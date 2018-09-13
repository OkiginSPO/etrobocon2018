#ifndef SCENE_H
#define SCENE_H

#include "PIDParam.h"

class Scene
{
    public:
        explicit Scene(float p, float i, float d, int _forward, float _distance);
        ~Scene();
        PIDParam* GetPIDParam(void);
        float GetForward(void);
        float GetDistance(void);
        
    private:
        const PIDParam *pid;
        const int forward;
        const int distance;
};

#endif /* SCENE_H */

