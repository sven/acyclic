# ACyCLIC - Auto Completition CommandLine InterfaCe


## Project

  * [ACyCLIC at GitHub](https://github.com/sven/acyclic)


## Ports

  * Linux (eg. Ubuntu 16.10)
  * Atmega8 (eg. myAVR Board MK1)
  * Atmega328p (eg. Arduino Uno, Arduino Nano)


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
   8429    1288     296   10013    271d build/acyclic_linux.elf
   2778     526      98    3402     d4a build/acyclic_atmega8.elf
   2878     526      98    3502     dae build/acyclic_atmega328p.elf

History enabled
---------------
   text    data     bss     dec     hex filename
  10299    1296     320   11915    2e8b build/acyclic_linux.elf
   3592     526     108    4226    1082 build/acyclic_atmega8.elf
   3694     526     108    4328    10e8 build/acyclic_atmega328p.elf
```


## Author

  * [Sven Bachmann](mailto:dev@mcbachmann.de)


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
