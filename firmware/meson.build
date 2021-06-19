# SPDX-License-Identifier: BSD-3-Clause

substrate = include_directories('../deps/substrate')

dragonTI = dependency(
	'dragonTI',
	required: true,
	version: '>=0.0.1',
	fallback: ['dragonTI', 'dragonTI_dep'],
	default_options: [
		'chip=tm4c123gh6pm',
	]
)

firmwareSrc = [
	'startup.cxx', 'spiFlashProgrammer.cxx', 'led.cxx', 'spi.cxx',
	'osc.cxx', 'usb.cxx', 'usb/descriptors.cxx', 'usb/device.cxx',
	'usb/flashProto.cxx'
]

firmwareArgs = targetCXX.get_supported_arguments(
	'-Wvla',
	'-Wimplicit-fallthrough',
	'-Wconversion',
	'-Wstack-usage=4096'
)

firmware = executable(
	'SPIFlashProgrammer',
	firmwareSrc,
	include_directories: [commonInclude, substrate],
	dependencies: [dragonTI],
	cpp_args: firmwareArgs,
	#link_args: ['-T', '@0@/tm4c123gh6pm/memoryLayout.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)

gdb = find_program('gdb')
run_target(
	'program',
	command: [
		gdb,
		'--nx',
		'--batch',
		'-ex', 'tar ext /dev/ttyBmpGdb',
		'-ex', 'mon s',
		'-ex', 'att 1',
		'-ex', 'load',
		'-ex', 'kill',
		firmware
	]
)

objdump = find_program('objdump')
run_target(
	'disasm',
	command: [objdump, '-dC', firmware]
)

size = find_program('get_fw_size.py', dirs: '@0@/../common/scripts'.format(meson.current_source_dir()))
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
