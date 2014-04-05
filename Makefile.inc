# Makefile include for SPI Flash Programmer. Set V to something other than 0 for more detailed build output.

ifeq ($(MAKELEVEL), 0)

# Silence the tools
ifeq ("$(origin V)", "command line")
	BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
	BUILD_VERBOSE = 0
endif

MAKEFLAGS += -w

ifeq ($(BUILD_VERBOSE), 0)
	quiet=quiet_
	Q=@
else
	quiet=
	Q=
endif
ifneq ($(findstring s,$(MAKEFLAGS)),)
	quiet=silent_
endif

export quiet Q BUILD_VERBOSE

endif

quiet_cmd_cc = " CC    $@"
cmd_cc = $(GCC) $(2)
quiet_cmd_cxx = " CXX   $@"
cmd_cxx = $(CC) $(2)
quiet_cmd_ccld = " CCLD  $@"
cmd_ccld = $(CC) $(2)
quiet_cmd_ld = " LD    $@"
cmd_ld = $(LD) $(2)
quiet_cmd_strip = " STRIP $@"
cmd_strip = $(STRIP) $(1)
quiet_cmd_ar = " AR    $(2)"
cmd_ar = $(AR) $(2) $(3)
quiet_cmd_ranlib = " RANLIB $(2)"
cmd_ranlib = $(RANLIB) $(2)
quiet_cmd_ln = " LN    $(3) => $(2)"
cmd_ln = $(LN) $(2) $(3)
quiet_cmd_rm = " CLEAN $(2)"
cmd_rm = $(RM) $(3)
quiet_cmd_objcopy = " BIN   $@"
cmd_objcopy = $(OBJCOPY) $(2)

define run-cmd
	@echo $($(quiet)cmd_$(1))
	@$(cmd_$(1))
endef

define debug-strip
	$(Q)if [ $(DEBUG) -eq 0 ]; then \
		echo $($(quiet)cmd_strip); \
		$(cmd_strip); \
	fi
endef

# Set up build engine variables
ARM_PREFIX = arm-none-eabi-
GCC ?= $(ARM_PREFIX)gcc
ifeq ($(strip $(DEBUG)), 1)
	OPTIM_FLAGS = -ggdb
	GCC_FLAGS = 
else
	GCC_VER = $(shell $(ARM_PREFIX)gcc -dumpversion | cut -d . -f 1)
	ifeq ($(shell if [ $(GCC_VER) -ge 4 ]; then echo 1; else echo 0; fi), 1)
		GCC_FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
	else
		GCC_FLAGS = 
	endif
	OPTIM_FLAGS = -O2
	DEBUG = 0
endif
CC = $(GCC)
CXX = $(GCC) $(GCC_FLAGS)
LD = $(ARM_PREFIX)ld
CHMOD = chmod 0755
STRIP = $(ARM_PREFIX)strip -x
RM = rm -f
AR = $(ARM_PREFIX)ar cr
RANLIB = $(ARM_PREFIX)ranlib
OBJCOPY = $(ARM_PREFIX)objcopy
LN = ln -sfT
INSTALL = install
INSTALL_FILE = $(INSTALL) -m644
INSTALL_BIN = $(INSTALL) -m755
INSTALL_DIR = $(INSTALL) -m755 -d

export GCC DEBUG

