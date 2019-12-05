BOARD_TAG = uno
ARDUINO_LIBS = Wire SPI SD
LOCAL_CPP_SRCS += $(wildcard *.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/*.cpp)
LOCAL_CPP_SRCS += $(wildcard libraries/*/*.cpp)
include /usr/share/arduino/Arduino.mk
DEVICE_PATH = /dev/ttyS3