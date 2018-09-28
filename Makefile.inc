mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += 

APPL_CXXOBJS += MotorWheel.o \
  ColorSensorController.o \
  PIDController.o \
  TouchSensorController.o \
  Scenario.o \
  Scene.o \
  PIDParam.o \
  Pid.o \
  SpeedControl.o \
  TestLineTrace.o \
  PortSettings.o \
  Utility.o \
  WalkerParam.o \
  Localization.o \
  TestLineTrace.o \
  LCDController.o \
  Logger.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif
