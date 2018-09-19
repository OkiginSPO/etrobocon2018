#ifndef TESTLINETRACE_H
#define TESTLINETRACE_H

#include "../include/ScenarioRcourse.h"
#include "../include/Walker_.h"

class TestLineTrace
{
    public:
        TestLineTrace();
        ~TestLineTrace();
        void Initialize(void);
        void WaitForStart(void);
        bool Terminated(void);
        void TestRun(void);
    
    private:
        ScenarioRcourse *scenarioR;
        Walker_ *walker_;
};

#endif /* TESTLINETRACE_H */

