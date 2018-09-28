mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += 

APPL_CXXOBJS += ArmMotorController.o \
  ColorSensorController.o \
  GyroSensorController.o \
  MotorWheel.o \
  PIDController.o \
  PIDParam.o \
  PortSettings.o \
  Scenario.o \
  ScenarioLcourse.o \
  ScenarioRcourse.o \
  Scene.o \
  SonarSensorController.o \
  TailMotorController.o \
  TouchSensorController.o \
  Utility.o \
  WalkerParam.o \
  Walker_.o \
  Localization.o \
  TestLineTrace.o \
  LCDController.o \
  Logger.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
    $(mkfile_path)../include

INCLUDES += \
    -I$(mkfile_path)../include
