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
    ./acyclic

    # Atmega8
    make atmega8
    make flash_atmega8

    # Atmega328p
    make atmega328p
    make flash_arduino_nano
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
