-include $(SDKROOT)/build/config.mk

CFLAGS += -D__RTTHREAD_OS__

A7_CPU_FLAGS = -mcpu=cortex-a7
A7_VFP_FLAGS = -mfloat-abi=hard -mfpu=neon-vfpv4

ARMV6_CPU_FLAGS = -mcpu=arm1176jzf-s
ARMV6_VFP_FLAGS = -mfloat-abi=softfp -mfpu=vfp

ifeq ($(CHIP_ID), FH8626V300)
	ARM_CORTEXA7_VFP ?= 0
else
	ARM_CORTEXA7_VFP ?= 1
endif
ifeq ($(DEBUG),1)
CFLAGS += -g
endif

LIBS = -lvmm_rpc_arm_rtt -ldbi_rtt  -ldsprpc_leader_rtt -lisprpc_leader_rtt -lrpc_customer


#kernel include
INCS = -I./
INCS += -I$(SDKROOT)/kernel/include/

#driver include src
INCS = -I./
INCS += -I$(SDKROOT)/lib/inc
INCS += -I$(SDKROOT)/lib/inc/advapi
INCS += -I$(SDKROOT)/lib/inc/mpp
INCS += -I$(SDKROOT)/lib/inc/types
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc/bsp
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc/dsp
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc/isp
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc/isp_ext
INCS += -I$(SDKROOT)/lib/$(CHIP_ID_LOWER)/inc/vicap
INCS += -Ilibpes/include
INCS += -Ilibrecord/include
INCS += -Ilibdbi_over_tcp/include

CFLAGS += $(INCS)


SRCS += $(wildcard *.c)
SRCS += $(wildcard startup/*.c)
SRCS += $(wildcard libpes/src/*.c)
SRCS += $(wildcard librecord/src/*.c)
SRCS += $(wildcard libdbi_over_tcp/src/*.c)


SAMPNAME := $(shell basename `pwd`)
SAMP_SRCS := $(SRCS)
LOCAL_CFLAGS := $(CFLAGS)
LOCAL_LIBS := $(LIBS)

include $(SDKROOT)/build/apps.mk

all:

menuconfig:
	mconf Kconfig
	@python $(shell pwd)/script/menu_anno.py ../../.config ../../config.h $(CHIP_ID)
	@rm -f .config.old .config.cmd

clean:
	rm -f $(BIN)
	find . -name \*.o -exec rm -rf {} \;
	find . -name \*.d -exec rm -rf {} \;
.PHONY: clean
