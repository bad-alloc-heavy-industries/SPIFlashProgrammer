# SPDX-License-Identifier: BSD-3-Clause

firmwareSrc = [
	'startup.cxx', 'spiFlashProgrammer.cxx', 'led.cxx', 'spi.cxx',
	'osc.cxx', 'usb.cxx', 'usb/descriptors.cxx'
]

firmware = executable(
	'SPIFlashProgrammer',
	firmwareSrc,
	link_args: ['-T', '@0@/tm4c123gh6pm/memoryLayout.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)
