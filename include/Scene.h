#ifndef SCENE_H
#define SCENE_H

#include "ev3api.h"
#include "PIDParam.h"

enum GOAL_TYPE
{
    DISTANCE = 0,
    COLOR = 1
};

class Scene
{
    public:
        Scene();
        explicit Scene(float p, float i, float d, int _forward, float _distance, GOAL_TYPE _goalType, colorid_t _colorID);
        ~Scene();
        PIDParam* GetPIDParam(void) const;
        float GetForward(void) const;
        float GetDistance(void) const;
        
    private:
        PIDParam *pid;
        int forward;
        float distance;
        enum GOAL_TYPE goalType;
        colorid_t colorID;
};

#endif /* SCENE_H */

