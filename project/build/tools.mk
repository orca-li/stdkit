# C Compiler Utils
CC := clang

CC_INCLUDES += -I"."

ifeq ($(PROJECT), tests)
ifeq ($(BUILD), debug)
CC_FLAGS += -O0
CC_FLAGS += -ggdb
else
CC_FLAGS += -O3
endif

CC_ASAN += -fsanitize=address,undefined,leak
CC_MSAN += -fsanitize=memory,undefined
CC_CMOCKA += -lcmocka
CC_GTEST += -lgtest
endif

CC_DEFINES += -D"STDKIT_MAJOR"=$(VERSION_MAJOR)
CC_DEFINES += -D"STDKIT_MINOR"=$(VERSION_MINOR)
CC_DEFINES += -D"STDKIT_PATCH"=$(VERSION_PATCH)

CC_FLAGS += -Wall
CC_FLAGS += -Wextra
CC_FLAGS += -Werror
CC_FLAGS += -Wpedantic
CC_FLAGS += -Wshadow
CC_FLAGS += -Wfloat-equal
CC_FLAGS += -Wformat=2
CC_FLAGS += -Wunused-parameter

СС_FLAGS_DEBUG += -O0
СС_FLAGS_DEBUG += -ggdb

CC_FLAGS_RELEASE += -O3

# Archiver
AR := ar
AR_FLAGS += rcs

# LD Utils
LD := ld

CC_TOOL := $(CC) $(CC_INCLUDES) $(CC_DEFINES) $(CC_FLAGS)
# Directory Utils
MD = mkdir
MD_FLAGS += -pv

RM := rm
RM_FLAGS += -rf

# Other Utils
SLEEP := sleep 1
ECHO := echo
LN := ln
CP := cp
READ := read

# Path
BUILD_DIR := build
BUILD_BIN_DIR := $(BUILD_DIR)/bin
BUILD_OBJ_DIR := $(BUILD_DIR)/obj

TOKENFILE := $(BUILD_DIR)/Token.txt

OUTPUT := stdkit

CC_OBJECTS := $(patsubst %.c,$(BUILD_OBJ_DIR)/%.o,$(CC_SOURCES))

FIND_ALL_OBJECTS += $(shell find $(BUILD_OBJ_DIR) -name "*.o.$(VERSION)")

INSTALL_PREFIX ?= /usr/local
INSTALL_LIBDIR := $(INSTALL_PREFIX)/lib
LIB_FULLNAME := lib$(OUTPUT).so.$(VERSION)
LIB_SONAME := lib$(OUTPUT).so.$(VERSION_MAJOR)
LIB_LINK := lib$(OUTPUT).so
CONFIRM_INSTALL := "Are you sure you want to install lib$(OUTPUT) to $(INSTALL_LIBDIR)? (y/n)"

INSTALL_INCLUDE_DIR ?= /usr/include/stdkit# или другой путь
CONFIRM_INSTALL_HEADER := "Are you sure you want to install headers to $(INSTALL_INCLUDE_DIR)? (y/n)"