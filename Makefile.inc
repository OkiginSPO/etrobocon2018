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
APPL_CXXOBJS += test.o \
  bluetoothTest.o \
  Bluetooth.o \
  common.o \
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
  Grid.o



SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif
