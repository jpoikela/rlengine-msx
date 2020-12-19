#
# Common definitions
export TOP := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/../
export BUILD_SYSTEM := $(TOP)/build
export RLE_TEST  = $(TOP)test
export RLE_ROMS = $(TOP)roms
export RLE_TOOLS = $(TOP)tools

export hide:= @

# Output tree
export BUILD_OUT = $(TOP)out
export BUILD_OUT_BIN = $(BUILD_OUT)/bin
export BUILD_OUT_TOOLS = $(BUILD_OUT)/tools

# Set default target
.PHONY: rle
DEFAULT_GOAL := all
$(DEFAULT_GOAL):

export MAKEFLAGS :=

# warning 59: function must return value, in low level asm functions,
#             we directly set hl with return value without using local vars
# warning 196: pointer target lost const qualifier
#             we need to store ROM data as const, but is is assigned as non const
#
export ENGINE_LDFLAGS := -rc
export ENGINE_ASFLAGS := -plosff

export ARCH := $(shell uname)

ifeq ($(ARCH), Darwin)
export SDCC_ROOT := $(TOP)/prebuilts/darwin/sdcc_3.8.5
else
export SDCC_ROOT := $(TOP)/prebuilts/x86_64/sdcc_3.8.5
endif
export CROSS_CC := $(SDCC_ROOT)/bin/sdcc
export CROSS_AS := $(SDCC_ROOT)/bin/sdasz80
export CROSS_LD := $(SDCC_ROOT)/bin/sdldz80
export CROSS_AR := $(SDCC_ROOT)/bin/sdar
export CROSS_LD := $(SDCC_ROOT)/bin/sdldz80
export SDCC_LIB := $(SDCC_ROOT)/share/lib/z80
export SDCC_INCLUDE := $(SDCC_ROOT)/share/include


ifeq ($(BUILD_TYPE),release)
	export ENGINE_CFLAGS  := -mz80 --std-c99 --opt-code-speed --max-allocs-per-node 100000 --fno-omit-frame-pointer --disable-warning 59 --disable-warning 196 -I $(TOP)/engine/include -I $(SDCC_INCLUDE)
	export ENGINE_CFLAGS_BANKED := -mz80 --model-large --std-c99 --opt-code-speed --max-allocs-per-node 100000 --fno-omit-frame-pointer --disable-warning 59 --disable-warning 196 -I $(TOP)/engine/include -I $(SDCC_INCLUDE)
else
	export ENGINE_CFLAGS  := -DNDEBUG -mz80 --std-c99 --opt-code-speed --fno-omit-frame-pointer --disable-warning 59 --disable-warning 196 -I $(TOP)/engine/include -I $(SDCC_INCLUDE)
	export ENGINE_CFLAGS_BANKED := -DNDEBUG -mz80 --model-large --std-c99 --opt-code-speed --fno-omit-frame-pointer --disable-warning 59 --disable-warning 196 -I $(TOP)/engine/include -I $(SDCC_INCLUDE)
endif

export HOSTCC	:= gcc
export TILED2H  := $(RLE_TOOLS)/map2header.py

# Build Commands
#
export CONFIG_ROM := $(BUILD_SYSTEM)/config_rom.mk
export BUILD_ROM := $(BUILD_SYSTEM)/build_rom.mk
export BUILD_ROM_32K := $(BUILD_SYSTEM)/build_32k_rom.mk
export BUILD_ROM_48K := $(BUILD_SYSTEM)/build_48k_rom.mk
export BUILD_ROM_ASCII8 := $(BUILD_SYSTEM)/build_ascii8_rom.mk
export BUILD_ROM_ASCII8_BANKSWITCH := $(BUILD_SYSTEM)/build_ascii8_rom_bankswitch.mk
export BUILD_RESOURCES := $(BUILD_SYSTEM)/build_resources.mk

# Create output tree
.PHONY: outdirs
outdirs:
	@mkdir -p $(BUILD_OUT)
	@mkdir -p $(BUILD_OUT_BIN)
	@mkdir -p $(BUILD_OUT_TOOLS)
all: outdirs

# Build tests
#
.PHONY: test
test:
	$(MAKE) -C $(RLE_TEST) all
rle: #test

# Build ROMS
#
.PHONY: roms
roms:
	$(MAKE) -C $(RLE_ROMS) all
rle: #roms

clean:
	rm -Rf $(TOP)/out
	$(MAKE) -C $(RLE_TEST) $(MAKEFLAGS) clean
