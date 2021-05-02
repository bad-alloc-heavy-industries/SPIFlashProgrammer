# SPDX-License-Identifier: BSD-3-Clause

substrate = dependency(
	'substrate',
	required: true,
	version: '>=0.0.1',
	fallback: ['substrate', 'substrate_dep'],
	native: true
)

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
