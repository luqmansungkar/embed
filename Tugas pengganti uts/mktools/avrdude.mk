PRGORAMMER_TTY     ?= /dev/ttyUSB0
MMCU               ?= atmega32

ifeq ($(MMCU), atmega32a)
    MMCU           := atmega32
endif

AVRDUDE            := avrdude -c avr910 -p $(MMCU) -P $(PRGORAMMER_TTY) -b 115200
