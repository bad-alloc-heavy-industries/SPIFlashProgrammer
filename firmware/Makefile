# This file is part of SPI Flash Programmer (SPIFP)
# Copyright © 2014 Rachel Mant (dx-mon@users.sourceforge.net)
#
# SPIFP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SPIFP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include Makefile.inc

CFLAGS_EXTRA = -ffunction-sections -fdata-sections
CFLAGS = -c $(OPTIM_FLAGS) -Wall -Wextra -Wshadow $(ARM_FLAGS) $(CFLAGS_EXTRA) -Iinclude -I.. $(DEFINES) -o $@ $<
#LFLAGS = $(ARM_FLAGS) -Wl,--static,--gc-sections,-T,$(LSCRIPT) -o $(ELF) $(O)
LFLAGS = -T $(LSCRIPT) --static --gc-sections -o $(ELF) $(O)
BFLAGS = -O binary $(ELF) $(BIN)

O_EXTRA = SPI.o UART.o GPIO.o Startup.o
O_TIVAC = $(patsubst %, TivaC/%, $(O_EXTRA))
O_LPC4370 = $(patsubst %, LPC4370/%, $(O_EXTRA))

ifneq ($(NOCONFIG), 1)
PREREQ = bin2src/bin2src
O += config.o
endif

O += SPIFlash.o
ELF = SPIFlashProgrammer.elf
BIN = SPIFlashProgrammer.bin

ifeq ($(TARGET), TivaC)
O += $(O_TIVAC)
LSCRIPT = TivaC/TM4C.ld
ARM_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -msoft-float -mfloat-abi=softfp
DEFINES = -DPART_LM4F120H5QR -DARM_MATH_CM4 -DTARGET_IS_BLIZZARD_RA1
else ifeq ($(TARGET), LPC4370)
O += $(O_LPC4370)
O_EXTRA += USB.o
LSCRIPT = LPC4370/LPC4370.ld
ARM_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mhard-float -mfloat-abi=hard
DEFINES =
else ifeq ($(MAKECMDGOALS),clean)
O += $(O_TIVAC) $(O_LPC4370) LPC4370/USB.o
else
$(error Invalid build configuration detected, must see a valid TARGET)
endif

ifeq ($(NOCONFIG), 1)
DEFINES += -DNOCONFIG
endif

ifeq ($(NOUSB), 1)
DEFINES += -DNOUSB
endif

default: all

all: $(BIN)

$(BIN): $(ELF)
	$(call run-cmd,objcopy,$(BFLAGS))

ifneq ($(and $(findstring $(NOCONIFIG), 1), $(findstring $(NOUSB), 1)),)
$(ELF):
	@echo "Error, cannot ask me to build with neither a config file nor USB support"
	@exit 1
else
$(ELF): $(PREREQ) $(O)
	$(call run-cmd,ld,$(LFLAGS))
endif

bin2src/bin2src:
	@(cd bin2src && $(MAKE))

clean:
	$(call run-cmd,rm,firmware,$(BIN) $(ELF) $(O) config.c)

.c.o:
	$(call run-cmd,cc,$(CFLAGS))

%.c: %.bin
	$(call run-cmd,bin2src)

.PHONY: clean all default .c.o bin2src/bin2src
config.o: config.c
config.c: config.bin

