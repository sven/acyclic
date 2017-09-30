# ACyCLIC - Auto Completition CommandLine InterfaCe


## Author

  * [Sven Bachmann](mailto:dev@mcbachmann.de)
  * [ACyCLIC at GitHub][https://github.com/sven/acyclic]


## Ports

  * Linux (z.B. Ubuntu 16.10)
  * Atmega8 (z.B. myAVR Board MK1)
  * Atmega328p (z.B. Arduino Uno, Arduino Nano)


## Quickstart

  * Install necessary compilers, tools and libs: gcc or avr-gcc
  * Compile your port

```
    # Linux (runs a little test script after compilation)
    make linux
    ./build/acyclic

    # Atmega8
    make atmega8
    make flash_atmega8

    # Atmega328p
    make atmega328p
    make flash_arduino_nano
```


## Footprint

A small overview of the current footprint when compiling the application that
is used for the testcases is shown here.

```
$> make size_all

History disabled
----------------
   text    data     bss     dec     hex filename
   9242    1304     112   10658    29a2 build/acyclic_linux.elf
   2154     486      98    2738     ab2 build/acyclic_atmega8.elf
   2248     486      98    2832     b10 build/acyclic_atmega328p.elf

History enabled
---------------
   text    data     bss     dec     hex filename
  11176    1312     112   12600    3138 build/acyclic_linux.elf
   3022     486     108    3616     e20 build/acyclic_atmega8.elf
   3116     486     108    3710     e7e build/acyclic_atmega328p.elf
```


## Contributors

  * [port GmbH](http://www.port.de)


## MIT License

    Copyright (c) 2016-2017, Sven Bachmann <dev@mcbachmann.de>
    ACyCLIC at GitHub: https://github.com/sven/acyclic
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
