.PHONY: all build clean program avrdude
all: build

PROJECT             := uts
MMCU                := atmega32
CPU_FREQ            := 12000000

PRGORAMMER_TTY      := /dev/ttyUSB0

SOURCE              :=  $(addprefix src/, \
                            yeah.o \
                            i2c/i2c_master.o \
                            lcd/lcd8bit.o \
                        )

AVRFLAG_OTHER       := -Isrc -Isrc/lcd -Isrc/i2c -Isrc/conf

include mktools/template.mk
include mktools/avrdude.mk

build: $(PROJECT).elf

clean:
	@echo "Remove: *.o *.elf *.hex *.eep *.lss src/*.o src/lcd/*.o src/i2c/*.o"
	@rm -rf *.o *.elf *.hex *.eep *.lss src/*.o src/lcd/*.o src/i2c/*.o


program: $(PROJECT).hex
	$(AVRDUDE) -Uflash:w:"$<":i

avrdude:
	$(AVRDUDE)
