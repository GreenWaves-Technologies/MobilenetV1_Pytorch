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

MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_BiasReLULinear_BasicKernels.c  
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_Pooling_BasicKernels.c
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_Conv_BasicKernels.c                  
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_Conv_DP_BasicKernels.c         
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_SoftMax.c
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_Conv_DW_BasicKernels.c         
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_MatAlgebra.c                   
MOBILENET_KER_PATH += $(TILER_GENERATOR_PATH)/CNN/CNN_Conv_DW_DP_BasicKernels.c  

TILER_APP_SRCS = MobileNetModel.c $(MOBILENET_GEN_PATH)/CNN_Generators.c $(MOBILENET_GEN_PATH)/CNN_Generator_Util.c
TILER_EXE = GenMobilenet

TILER_USER_KERNELS = MN_Kernels.c 

APP_SRCS = main.c $(TILER_USER_KERNELS) $(MOBILENET_KER_PATH)
APP_SRCS += utils/src/dnn_utils.c
APP_SRCS += utils/src/ili9341.c utils/src/font.c 
APP_INC += $(TILER_INC) $(CNN_AT_PATH)
RM=rm -f


APP_CFLAGS += -O3 

APP_CFLAGS += -mno-memcpy -fno-tree-loop-distribute-patterns  -fdata-sections -ffunction-sections 
APP_LDFLAGS += -flto -Wl,--gc-sections 

APP_CFLAGS += -w -Wno-maybe-uninitialized -Wno-unused-but-set-variable
APP_CFLAGS += -I$(TILER_INC) -I$(MOBILENET_GEN_PATH) -Iutils/inc


io=host

ifeq ($(ALREADY_FLASHED),)
	READFS_FILES = $(realpath MN_L3_Flash_Const.bin) $(realpath binFiles/L0_INPUT.bin)
	PLPBRIDGE_FLAGS += -f 
endif


# Build the code generator
$(TILER_EXE): 
	gcc -o GenMobilenet -Imbnets -I$(MOBILENET_GEN_PATH) -I$(TILER_INC) -I$(MOBILENET_GEN_PATH)/include $(TILER_APP_SRCS) $(TILER_LIB)


# Run the code generator and generated user kernels
$(TILER_USER_KERNELS): $(TILER_EXE)
	./GenMobilenet

model: $(TILER_USER_KERNELS)

all:: model

clean::
	$(RM) GenMobilenet MN_Kernels.* MobilenetKernels.* GenMobilenetGraph

.PHONY: model clean

include $(GAP_SDK_HOME)/tools/rules/pmsis_rules.mk

