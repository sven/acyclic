CC = /usr/bin/avr-gcc
OC = /usr/bin/avr-objcopy
OS = /usr/bin/avr-size

CFLAGS = \
    -Wall \
    -Wextra \
    -Werror \
    -ggdb \
    -Isrc

CFLAGS_CLANG = \
    $(CFLAGS) \
    -Weverything \
    -Wno-documentation-unknown-command \
    -Wno-padded \
    -Wno-disabled-macro-expansion \
    -fcolor-diagnostics

CFLAGS_ATMEGA = \
    $(CFLAGS) \
    -Os \
    -fno-fat-lto-objects \
    -mcall-prologues


SHELL=/bin/bash -o pipefail


linux:
	@mkdir -p build
	@echo "*************"
	@echo "* Compiling *"
	@echo "*************"
	@clang $(CFLAGS_CLANG) -Iports/linux -o build/acyclic src/acyclic.c tests/acyclic_cmds.c ports/linux/acyclic_platform.c
	@echo "Done."
	@echo
	@echo "*********************"
	@echo "* Running testsuite *"
	@echo "*********************"
	@./tests/acyclic_testsuite
	@echo
	@echo
	@echo "*** Tests: successful ***"
	@size build/acyclic


atmega8:
	@mkdir -p build
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic.o src/acyclic.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic_platform.o ports/atmega8/acyclic_platform.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic_cmds.o tests/acyclic_cmds.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Wl,-Map,build/acyclic_atmega8.map -Wl,--gc-sections -Wl,--relax -o build/acyclic.elf build/acyclic.o build/acyclic_cmds.o build/acyclic_platform.o
	$(OC) -O ihex -R .eeprom build/acyclic.elf build/acyclic.hex
	$(OS) build/acyclic.elf


atmega328p:
	@mkdir -p build
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic.o src/acyclic.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic_platform.o ports/atmega328p/acyclic_platform.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic_cmds.o tests/acyclic_cmds.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Wl,-Map,build/acyclic_atmega328p.map -Wl,--gc-sections -Wl,--relax -o build/acyclic.elf build/acyclic.o build/acyclic_cmds.o build/acyclic_platform.o
	$(OC) -O ihex -R .eeprom build/acyclic.elf build/acyclic.hex
	$(OS) build/acyclic.elf


gdb:
	gdb ./build/acyclic


flash_atmega8:
	sudo avrdude -p ATmega8 -c usbasp -U lfuse:w:0xff:m -U hfuse:w:0xc9:m -U build/acyclic.hex


flash_atmega328p:
	sudo avrdude -v -pm328p -c arduino -P/dev/ttyUSB0 -b115200 -D -U flash:w:build/acyclic.hex:i


flash_arduino_nano:
	sudo avrdude -v -pm328p -c arduino -P/dev/ttyUSB0 -b57600 -D -U flash:w:build/acyclic.hex:i


reset_atmega8:
	sudo avrdude -p ATmega8 -c usbasp


clean:
	rm -f build/*
