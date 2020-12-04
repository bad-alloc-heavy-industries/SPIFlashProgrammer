# SPI Flash Programmer

After a complete rewrite and re-hash of the original ideal having had it proove itself, a PCB and tightly integrated yet simple to use firmware is the name of the game.

SPIFlashProgrammer is a small but capable board housing a Texas Instruments Tiva-C series ARM microprocessor, two Flash devices for use in a production environment to allow one board to program up to two different target devices on a production line, and USB for power and connecting to a host machine.

When connected to a host, the USB connectivity and `flashprog` utility come together to form a sleak programming experience, allowing the tool to be used in a "set and forget" manner for test and debug of bitstreams and firmware.

## Flash chip support

The firmware targets 25 series memory devices such as the M25P16, W25Q80BV, and AT25SF641.

## Build and installation

Building requires a small number of prerequsites and dependencies:

* [Meson](https://meson.build)
* [Ninja](https://ninja-build.org)
* A modern arm-none-eabi GCC such as the [GNU ARM Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
	or any distro or custom build GCC capable of targeting ARMv7 via C++17

To build the firmware and host software, run

```Bash
meson build --cross-file=cross-files/arm-none-eabi.meson
ninja -C build
```

You may then use the results in-place, or install the host software with `ninja -C build install`.

Programming the firmware onto a newly minted board can be achived via any JTAG adapter that can talk TI ICDI.
We use the awesome [Black Magic Probe](https://github.com/blacksphere/blackmagic) with the toolchain GDB.
