# Copyright 2019 GreenWaves Technologies, SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


APP = Mobilenet

BOARD_NAME?=gapuino


TILER_PATH = $(GAP_SDK_HOME)/tools/autotiler_v3

TILER_LIB = $(TILER_PATH)/lib/libtile.a
TILER_INC = $(TILER_PATH)/include
TILER_GENERATOR_PATH = $(TILER_PATH)/generators

MOBILENET_GEN_PATH = $(TILER_GENERATOR_PATH)/CNN
MOBILENET_KER_PATH = $(wildcard $(TILER_GENERATOR_PATH)/CNN/CNN_*.c)

APP_SRCS = main.c MN_Kernels.c $(MOBILENET_KER_PATH)
APP_SRCS += utils/src/dnn_utils.c
APP_SRCS += utils/src/ili9341.c utils/src/font.c 
APP_INC += $(TILER_INC) $(CNN_AT_PATH)
RM=rm -f


APP_CFLAGS += -O3 $(USE_HARDWARE_CE)

APP_CFLAGS += -mno-memcpy -fno-tree-loop-distribute-patterns  -fdata-sections -ffunction-sections 
APP_LDFLAGS +=  -flto -Wl,--gc-sections 

APP_CFLAGS += -w -Wno-maybe-uninitialized -Wno-unused-but-set-variable
APP_CFLAGS += -I$(TILER_INC) -I$(MOBILENET_GEN_PATH) -Iutils/inc


ifeq ($(ALREADY_FLASHED),)
		# this is for the board
		READFS_FILES = $(realpath MN_L3_Flash_Const.bin) $(realpath binFiles/L0_INPUT.bin)
		PLPBRIDGE_FLAGS += -f 
endif

export GAP_USE_OPENOCD=1
io=host


# The double colon allows us to force this to occur before the imported all target
# Link model generation to all step
all:: #model

# Build the code generator
GenTileBuild:
	gcc -o GenMobilenet $(USE_HARDWARE_CE) -Imbnets -I$(MOBILENET_GEN_PATH) -I$(TILER_INC) -I$(MOBILENET_GEN_PATH)/include MobileNetModel.c $(MOBILENET_GEN_PATH)/CNN_Generators.c $(TILER_LIB)


# Run the code generator
GenTile: GenTileBuild
	./GenMobilenet

model: GenTile

clean::
	$(RM) GenMobilenet MobilenetKernels.* GenMobilenetGraph

.PHONY: model clean

include $(GAP_SDK_HOME)/tools/rules/pmsis_rules.mk

