# SPDX-License-Identifier: BSD-3-Clause

firmwareSrc = [
	'startup.cxx', 'spiFlashProgrammer.cxx', 'led.cxx', 'spi.cxx',
	'osc.cxx'
]

firmware = executable(
	'SPIFlashProgrammer',
	firmwareSrc,
	link_args: ['-T', '@0@/TM4C123GH6PM.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)
