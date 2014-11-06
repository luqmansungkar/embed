.PHONY: all build clean program avrdude
all: build

PROJECT             := embed
MMCU                := atmega32
CPU_FREQ            := 12000000

PRGORAMMER_TTY      := /dev/ttyUSB0


SOURCE              := polling.c


include makeinclude/template.mk
include makeinclude/avrdude.mk

build: $(PROJECT).elf

clean:
	rm -rf *.o *.elf *.hex *.eep *.lss

program: $(PROJECT).hex
	$(AVRDUDE) -Uflash:w:"$<":i

avrdude:
	$(AVRDUDE)
