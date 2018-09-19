#ifndef SCENE_H
#define SCENE_H

#include "PIDParam.h"

class Scene
{
    public:
        Scene();
        explicit Scene(float p, float i, float d, int _forward, float _distance);
        ~Scene();
        PIDParam* GetPIDParam(void) const;
        float GetForward(void) const;
        float GetDistance(void) const;
        
    private:
        PIDParam *pid;
        int forward;
        float distance;
};

#endif /* SCENE_H */

