include ../build.mk
include $(SETUP_BUILD)

include $(CONFIG_ROM)

LOCAL_RES_DIR := ./res
include $(BUILD_RESOURCES)

LOCAL_ROM_NAME := vdp_test
LOCAL_SRC_FILES := vdp_test.c
include $(BUILD_ROM_32K)
