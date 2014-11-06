CPU_FREQ                ?= 12000000
MMCU                    ?= atmega32a

FLAG_OPTIMIZATION       ?= -O1 -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums
FLAG_DEBUG              ?= -g3 -gdwarf-2 -DDEBUG
FLAG_WARNING            ?= -Wall
FLAG_STANDARD           ?= -std=gnu99
FLAG_LINKER             ?= -Wl,--gc-sections
FLAG_SYMBOL             := -DF_CPU=$(CPU_FREQ)UL
FLAG_MCU                := -mmcu=$(MMCU)
FLAG_ALL                := $(FLAG_OPTIMIZATION)\
                           $(FLAG_DEBUG)\
                           $(FLAG_WARNING)\
                           $(FLAG_STANDARD)\
                           $(FLAG_MCU)\
                           $(FLAG_SYMBOL)\
                           $(FLAG_LINKER)\
                           $(FLAG_OTHER)

%.o: %.c
	avr-gcc $(FLAG_ALL) -c -o "$@" "$<"

%.hex: %.elf
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex "$<" "$@"

%.eep: %.elf
	avr-objcopy -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$<" "$@"

%.lss: %.elf
	avr-objdump -h -S "$<" > "$@"

$(PROJECT).elf: $(SOURCE) $(OBJECT)
	avr-gcc $(FLAG_ALL) -o $(PROJECT).elf $(SOURCE) $(OBJECT)
	avr-size "$@"
