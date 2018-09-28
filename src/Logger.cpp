#include "Logger.h"

LOGPARAM* Logger::GetLog(void)
{
    return log;
}

LOGPARAM* Logger::log = new LOGPARAM();
