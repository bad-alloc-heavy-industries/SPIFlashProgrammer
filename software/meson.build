# SPDX-License-Identifier: BSD-3-Clause

substrate = subproject(
	'substrate',
	default_options: [
		'build_tests=false',
		'cpp_std=c++17',
	]
).get_variable('substrate_native_dep')

libusb = dependency('libusb-1.0', version: '>=1.0.21', native: true)

subdir('include')

flashprogSrc = [
	'flashprog.cxx', 'args.cxx', 'args/tokenizer.cxx', 'args/types.cxx',
	'progress.cxx', versionHeader
]

executable(
	'flashprog',
	flashprogSrc,
	include_directories: [include_directories('include'), commonInclude],
	dependencies: [libusb, substrate],
	gnu_symbol_visibility: 'inlineshidden',
	build_by_default: true,
	install: false,
	native: true
)
