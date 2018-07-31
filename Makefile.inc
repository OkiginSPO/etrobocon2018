# app.cppがあるディレクトリの絶対パス
app_path := $(dir $(lastword $(MAKEFILE_LIST)))

# src配下の全ディレクトリをコンパイル対象にする。
APPL_DIR += $(shell find $(app_path)src -type d)
APPL_DIR += $(app_path)test

#include配下の全ディレクトリをコンパイル対象にする。
INCLUDES += $(foreach dir,$(shell find $(app_path)include -type d),-I$(dir))
INCLUDES += -I$(app_path)test

APPL_COBJS += 
APPL_CXXOBJS += test.o \
  common.o \
  walker.o \
  SelfLocalization.o \
  Distance.o \
  BasicWalker.o \
  LineTracerWalker.o \
  Parking.o \
  Pid.o \
  SpeedControl.o \
  TurnControl.o


SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif
