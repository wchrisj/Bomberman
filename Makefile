BOARD_TAG = uno
ARDUINO_LIBS = Wire
LOCAL_CPP_SRCS += $(wildcard *.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/*.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/*/*.cpp)
include /usr/share/arduino/Arduino.mk
DEVICE_PATH = /dev/ttyS3
MONITOR_PORT = /dev/ttyS3