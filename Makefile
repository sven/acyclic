CC = /usr/bin/avr-gcc
OC = /usr/bin/avr-objcopy
OS = /usr/bin/avr-size

CFLAGS = \
    -Wall \
    -Wextra \
    -Werror \
    -ggdb \
    -Isrc \
    -Wno-missing-field-initializers \
    -ffunction-sections \
    -fdata-sections


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
	@if [ ! -e .no_test ]; then \
	    ./tests/acyclic_testsuite; \
	 fi
	@echo
	@echo
	@echo "*** Tests: successful ***"
	@size build/acyclic


atmega8:
	@mkdir -p build
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic.o src/acyclic.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic_stdio.o src/acyclic_stdio.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic_platform.o ports/atmega8/acyclic_platform.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Iports/atmega8 -c -o build/acyclic_cmds.o tests/acyclic_cmds.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega8 -DF_CPU=3686400 -Wl,-Map,build/acyclic_atmega8.map -Wl,--gc-sections -Wl,--relax -o build/acyclic_atmega8.elf build/acyclic.o build/acyclic_stdio.o build/acyclic_cmds.o build/acyclic_platform.o
	$(OC) -O ihex -R .eeprom build/acyclic_atmega8.elf build/acyclic_atmega8.hex
	$(OS) build/acyclic_atmega8.elf


atmega328p:
	@mkdir -p build
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic.o src/acyclic.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic_stdio.o src/acyclic_stdio.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic_platform.o ports/atmega328p/acyclic_platform.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Iports/atmega328p -c -o build/acyclic_cmds.o tests/acyclic_cmds.c
	$(CC) $(CFLAGS_ATMEGA) -mmcu=atmega328p -DF_CPU=16000000L -Wl,-Map,build/acyclic_atmega328p.map -Wl,--gc-sections -Wl,--relax -o build/acyclic_atmega328p.elf build/acyclic.o build/acyclic_stdio.o build/acyclic_cmds.o build/acyclic_platform.o
	$(OC) -O ihex -R .eeprom build/acyclic_atmega328p.elf build/acyclic_atmega328p.hex
	$(OS) build/acyclic_atmega328p.elf


gdb:
	gdb ./build/acyclic


flash_atmega8:
	sudo avrdude -p ATmega8 -c usbasp -U lfuse:w:0xff:m -U hfuse:w:0xc9:m -U build/acyclic_atmega8.hex


flash_atmega328p:
	sudo avrdude -v -pm328p -c arduino -P/dev/ttyUSB0 -b115200 -D -U flash:w:build/acyclic_atmega328p.hex:i


flash_arduino_nano:
	sudo avrdude -v -pm328p -c arduino -P/dev/ttyUSB0 -b57600 -D -U flash:w:build/acyclic_atmega328p.hex:i


reset_atmega8:
	sudo avrdude -p ATmega8 -c usbasp


clean:
	rm -f build/*


size_all:
	@touch .no_test
	@sed 's/\(define ACYCLIC_HISTORY .*\)[0-9]/\10/' -i src/acyclic.h
	@make -f Makefile clean linux atmega8 atmega328p >/dev/null 2>&1
	@mv build/acyclic build/acyclic_linux.elf
	@echo "History disabled"
	@echo "----------------"
	@size build/acyclic_linux.elf build/acyclic_atmega8.elf build/acyclic_atmega328p.elf
	@sed 's/\(define ACYCLIC_HISTORY .*\)[0-9]/\11/' -i src/acyclic.h
	@make -f Makefile clean linux atmega8 atmega328p >/dev/null 2>&1
	@mv build/acyclic build/acyclic_linux.elf
	@echo
	@echo "History enabled"
	@echo "---------------"
	@size build/acyclic_linux.elf build/acyclic_atmega8.elf build/acyclic_atmega328p.elf
	@rm -f .no_test
