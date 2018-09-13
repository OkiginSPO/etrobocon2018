#include "Scene.h"

Scene::Scene(float p, float i, float d, int _forward, float _distance)
    : pid(p, i, d)
    , forward(_forward)
    , distance(_distance)
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

float Scene::GetForward(void) const {
    return forward;
}

float Scene::GetDistance(void) const {
    return distance;
}

