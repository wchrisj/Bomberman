BOARD_TAG = uno
ARDUINO_LIBS = Wire
LOCAL_C_SRCS    ?= $(shell find . -type f -name '*.c' | tr '\n' ' ')
LOCAL_CPP_SRCS  ?= $(shell find . -type f -name '*.cpp' | tr '\n' ' ')
include /usr/share/arduino/Arduino.mk
DEVICE_PATH = /dev/ttyS3
MONITOR_PORT = /dev/ttyS3