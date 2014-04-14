# SPI Flash Programmer

A simple but effective SPI Flash device programmer for the Tiva C Launchpad.

This project allows a Tiva C Launchpad to be made into a SPI Flash programmer capable of taking data from either an internal built-in blob (config.bin) or over the Tiva C Launchpad's debug UART on Port A.

Data programed from the UART is the so-called "USB" path as the data originates from the USB virtual serial port provided by the ICDI interface of the Launchpad board.

## firmware build

The firmware can be built in one of three modes:
* Both USB and built-in blob enabled
* Just USB enabled (NOCONFIG=1)
* Just built-in blob enabled (NOUSB=1)

The build system checks for an errors when NOCONFIG=1 and NOUSB=1 as this would be a pointless configuration.
The build depends on the presence of a suitable ARM toolchain - arm-none-eabi - a flavour of GCC.

Simple build instructions to get running immediately:
```Bash
cd firmware
make NOCONFIG=1
```

## flashprog build

The programming software depends souly on libusb-1.0 which can be installed easily from repository on most Linux distros, and from the libusb-win32 sourceforge project on Windows.

Building the software is as simple as running make in the flashprog directory.

