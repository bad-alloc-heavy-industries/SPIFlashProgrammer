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

