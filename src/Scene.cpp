#include "Scene.h"

Scene::Scene()
    : pid(new PIDParam(0.0F, 0.0F, 0.0F))
    , forward (0)
    , distance(0.0F)
{
    
}

Scene::Scene(float p, float i, float d, int _forward, float _distance, GOAL_TYPE _goalType, colorid_t _colorID)
    : pid(new PIDParam(p, i, d))
    , forward(_forward)
    , distance(_distance)
    , goalType(_goalType)
    , colorID(_colorID)
{
    
}

Scene::~Scene()
{
    delete pid;
}

PIDParam* Scene::GetPIDParam(void) const
{
    return pid;
}

float Scene::GetForward(void) const
{
    return forward;
}

float Scene::GetDistance(void) const
{
    return distance;
}

