PROJECT ?= tests

ifeq ($(PROJECT), main)
include project/list/main/main.mk
endif

ifeq ($(PROJECT), tests)
include project/list/tests/tests.mk
endif