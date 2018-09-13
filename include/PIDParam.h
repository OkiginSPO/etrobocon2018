#ifndef PIDPARAM_H
#define PIDPARAM_H

class PIDParam
{
public:
    float P, I, D;
    
    explicit PIDParam(float _p, float _i, float _d);
    ~PIDParam();
};

#endif /* PIDPARAM_H */
