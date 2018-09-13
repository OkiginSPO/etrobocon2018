#ifndef LOGGER_H
#define LOGGER_H

#include "Log.h"

class Logger
{
    public:
        Logger();
        ~Logger();
        void SendLog(void);
        
    private:
        Log *log;
};

#endif /* LOGGER_H */

