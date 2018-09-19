mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += 

APPL_CXXOBJS += ArmMotorController.o \
  ColorSensorController.o \
  Distance.o \
  GyroSensorController.o \
  MotorWheel.o \
  PIDController.o \
  PIDParam.o \
  PortSettings.o \
  Scenario.o \
  ScenarioLcourse.o \
  ScenarioRcourse.o \
  Scene.o \
  SelfLocalization.o \
  SonarSensorController.o \
  TailMotorController.o \
  TouchSensorController.o \
  Utility.o \
  WalkerParam.o \
  Walker_.o \
  common.o \
  test.o \
  Localization.o \
  walker.o \
  SelfLocalization.o \
  Distance.o \
  BasicWalker.o \
  LineTracerWalker.o \
  Pid.o \
  ParallelParking.o \
  ParpendicularParking.o \
  Parking.o \
  SpeedControl.o \
  TurnControl.o \
  Direction.o \
  BlockZone.o \
  Grid.o \
  TestLineTrace.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
    $(mkfile_path)../include

INCLUDES += \
    -I$(mkfile_path)../include
