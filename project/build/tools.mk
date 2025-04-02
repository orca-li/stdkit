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

# LD Utils
LD := ld

CC_TOOL := $(CC) $(CC_INCLUDES) $(CC_DEFINES) $(CC_FLAGS) $(CC_SOURCES)

# Directory Utils
MD = mkdir
MD_FLAGS += -pv

RM := rm
RM_FLAGS += -rf

# Other Utils
SLEEP := sleep 1
ECHO := echo

# Path
BUILD := build

TOKENFILE := $(BUILD)/Token.txt

OUTPUT := stdkit-v$(VERSION)