include $(SDKROOT)/build/config.mk

# LIBNAME := board
LIBNAME := $(shell basename `pwd`)

# core
CSRCS := $(wildcard $(SOC_NAME)/*.c)
CSRCS += $(wildcard loader/*.c)
CSRCS += $(wildcard startup/*.c)

CSRCS += $(wildcard $(SOC_NAME)/$(BOARD_NAME)/*.c)

ifeq ($(CONFIG_FH_FAST_BOOT), y)
ifeq ($(CONFIG_CONFIG_CHIP_MC632X), y)
    CSRCS += $(wildcard mc632x/fastboot/*.c)
endif
endif

CSRCS += $(wildcard common/board_info.c) $(wildcard common/calibrate.c) \
$(wildcard common/chkenv.c) $(wildcard common/fh_chipid.c) \
$(wildcard common/fh_pmu.c) $(wildcard common/media_proc.c) \
$(wildcard common/trap.c) $(wildcard common/timer.c)

ifeq ($(CONFIG_CONFIG_ARCH_MC632X), y)
	CSRCS += $(wildcard common/mol_pinctrl.c)
else
	CSRCS += $(wildcard common/pinctrl.c)
endif

include $(SDKROOT)/build/lib.mk
