#include "TestLineTrace.h"

TestLineTrace::TestLineTrace()
{
    scenarioR = new ScenarioRcourse();
    walker_ = new Walker_(scenarioR);
}

TestLineTrace::~TestLineTrace()
{
    delete scenarioR;
    delete walker_;
}

void TestLineTrace::Initialize(void)
{
    walker_->Initialize();
}

void TestLineTrace::WaitForStart(void)
{
    walker_->WaitForStart();
}

bool TestLineTrace::Terminated(void)
{
    return walker_->Terminated();
}

void TestLineTrace::TestRun(void)
{
//    walker_->LineTrace();
    walker_->RunOnOff();
}
