#ifndef LOGGER_H
#define LOGGER_H

typedef struct {
    int index;
    int vattery;
    int r_wheel_enc;
    int l_wheel_enc;
    int r_wheel_pwm;
    int l_wheel_pwm;
    int blightness;
    float proportional;
    float integral;
    float diff;
    float distance;
    int total_pid;
    int sysutm;
} LOGPARAM;

class Logger
{
    public:
        static LOGPARAM* GetLog(void);
    
    private:
        static LOGPARAM *log;
};

#endif /* LOGGER_H */

