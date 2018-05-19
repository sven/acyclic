# ACyCLIC - Auto Completition CommandLine InterfaCe


## Project

  * [ACyCLIC at GitHub](https://github.com/sven/acyclic)


## Ports

  * Linux (eg. Ubuntu 16.10)
  * Atmega8 (eg. myAVR Board MK1)
  * Atmega328p (eg. Arduino Uno, Arduino Nano)
  * [PINKIE](https://github.com/sven/pinkie)


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
$> make size_all # commit: f385339b158ecee0c3d454100e7fbdc45201721a

History disabled
----------------
   text    data     bss     dec     hex filename
   8301    1280     296    9877    2695 build/acyclic_linux.elf
   2738     526      98    3362     d22 build/acyclic_atmega8.elf
   2840     526      98    3464     d88 build/acyclic_atmega328p.elf

History enabled
---------------
   text    data     bss     dec     hex filename
  10231    1288     320   11839    2e3f build/acyclic_linux.elf
   3584     526     108    4218    107a build/acyclic_atmega8.elf
   3686     526     108    4320    10e0 build/acyclic_atmega328p.elf
```


## Author

  * [Sven Bachmann](mailto:dev@mcbachmann.de)


## Contributors

  * [port GmbH](http://www.port.de)


## MIT License

    Copyright (c) 2016-2018, Sven Bachmann <dev@mcbachmann.de>
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
