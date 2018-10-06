# app.cppがあるディレクトリの絶対パス
app_path := $(dir $(lastword $(MAKEFILE_LIST)))

# src配下の全ディレクトリをコンパイル対象にする。
APPL_DIR += $(shell find $(app_path)src -type d)

#include配下の全ディレクトリをコンパイル対象にする。
INCLUDES += $(foreach dir,$(shell find $(app_path)include -type d),-I$(dir))

#test配下のディレクトリの指定するファイルをコンパイル対象にする。
APPL_DIR += $(app_path)test
INCLUDES += -I$(app_path)test

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
  Logger.o \
  Bluetooth.o \
  SelfLocalization.o \
  Distance.o \
  Direction.o \
  Grid.o \
  walker.o \
  common.o \
  BasicWalker.o \
  BlockZone.o \
  TurnControl.o \
  Pid.o \
  SpeedControl.o \
  PerpendicularParking.o 

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif
