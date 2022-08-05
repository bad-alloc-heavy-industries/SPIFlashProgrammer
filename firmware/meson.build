# SPDX-License-Identifier: BSD-3-Clause

substrate = subproject(
	'substrate',
	required: true,
	version: '>=0.0.1',
	default_options: [
		'build_tests=false',
		'build_library=false'
	]
).get_variable(
	'substrate_dep'
).partial_dependency(
	compile_args: true,
	includes: true
)

dragonTI = dependency(
	'dragonTI',
	required: true,
	version: '>=0.0.1',
	fallback: ['dragonTI', 'dragonTI_dep'],
	default_options: [
		'chip=tm4c123gh6pm',
	]
).partial_dependency(
	compile_args: true,
	includes: true
)

dragonUSB = dependency(
	'dragonUSB',
	required: true,
	version: '>=0.0.1',
	fallback: ['dragonUSB', 'dragonUSB_dep'],
	default_options: [
		'chip=tm4c123gh6pm',
		'interfaces=2',
		'endpoints=1',
		'epBufferSize=64',
		'configDescriptors=1',
		'ifaceDescriptors=2',
		'endpointDescriptors=2',
		'strings=5',
		'dfuFlashBufferSize=128',
		'dfuFlashPageSize=128',
		'drivers=dfu'
	]
)

firmwareSrc = [
	'startup.cxx', 'spiFlashProgrammer.cxx', 'led.cxx', 'spi.cxx',
	'sfdp.cxx', 'flash.cxx', 'osc.cxx', 'timer.cxx',
	'usb/descriptors.cxx', 'usb/flashProto.cxx'
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
	include_directories: [commonInclude],
	dependencies: [substrate, dragonTI, dragonUSB],
	cpp_args: firmwareArgs,
	link_args: ['-T', '@0@/tm4c123gh6pm/memoryLayout.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)

objcopy = find_program('objcopy')
firmwareBin = custom_target(
	'SPIFlashProgrammer.bin',
	input: firmware,
	output: 'SPIFlashProgrammer.bin',
	command: [
		objcopy,
		'-j', '.text',
		'-j', '.data',
		'-j', '.note.gnu.build-id',
		'-O', 'binary',
		'@INPUT@',
		'@OUTPUT@',
	],
	build_by_default: true,
	install: false
)

dfuUtil = find_program('dfu-util')
run_target(
	'program',
	command: [
		dfuUtil,
		'-d', '1209:ab0c,:badb', '-R',
		'-D', firmwareBin
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
