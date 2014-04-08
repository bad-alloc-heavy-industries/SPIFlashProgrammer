# SPI Flash Programmer

A simple but effective SPI Flash device programmer for the Tiva C Launchpad.

This project allows a Tiva C Launchpad to be made into a SPI Flash programmer capable of taking data from either an internal built-in blob (config.bin) or over the Tiva C Launchpad's debug UART on Port A.

Data programed from the UART is the so-called "USB" path as the data originates from the USB virtual COM port provided by the device.
