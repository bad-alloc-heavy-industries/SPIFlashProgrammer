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

ARM_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -msoft-float -mfloat-abi=softfp -ffunction-sections -fdata-sections
DEFINES = -DPART_LM4F120H5QR -DARM_MATH_CM4 -DTARGET_IS_BLIZZARD_RA1
CFLAGS = -c $(OPTIM_FLAGS) -pedantic -Wall $(ARM_FLAGS) -Iinclude $(DEFINES) -o $@ $<
#LFLAGS = $(ARM_FLAGS) -Wl,--static,--gc-sections,-T,TM4C.ld -o $(ELF) $(O)
LFLAGS = -T TM4C.ld --static --gc-sections -o $(ELF) $(O)
BFLAGS = -O binary $(ELF) $(BIN)

O = SPIFlash.o Startup.o
ELF = SPIFlashProgrammer.elf
BIN = SPIFlashProgrammer.bin

default: all

all: $(BIN)

$(BIN): $(ELF)
	$(call run-cmd,objcopy,$(BFLAGS))

$(ELF): $(O)
	$(call run-cmd,ld,$(LFLAGS))

clean:
	$(call run-cmd,rm,SPI Flash Programmer,$(BIN) $(ELF) $(O))

.c.o:
	$(call run-cmd,cc,$(CFLAGS))

.PHONY: clean all default .c.o

