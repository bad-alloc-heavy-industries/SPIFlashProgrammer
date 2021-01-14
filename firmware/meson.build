# SPDX-License-Identifier: BSD-3-Clause

firmwareSrc = [
	'startup.cxx', 'spiFlashProgrammer.cxx', 'led.cxx', 'spi.cxx',
	'osc.cxx', 'usb.cxx', 'usb/descriptors.cxx', 'usb/device.cxx'
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

size = find_program('get_fw_size.py', dirs: '@0@/../scripts'.format(meson.current_source_dir()))
run_target(
	'size',
	command: [
		size,
		'--flash-size=262144', # 256KiB of Flash
		'--ram-size=32768', # 32KiB of RAM
		'--size-prog=@0@'.format(find_program('size').path()),
		firmware
	]
)
