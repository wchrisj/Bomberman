BOARD_TAG = uno
LOCAL_CPP_SRCS += $(wildcard *.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/*.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/IRlib/*.cpp)
include /usr/share/arduino/Arduino.mk
