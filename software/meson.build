substrate = dependency(
	'substrate',
	required: true,
	version: '>=0.0.1',
	fallback: ['substrate', 'substrate_dep'],
	native: true
)

libusb = dependency('libusb-1.0', native: true)

flashprogSrc = [
	'flashprog.cxx'
]

executable(
	'flashprog',
	flashprogSrc,
	include_directories: [include_directories('include')],
	dependencies: [libusb, substrate],
	gnu_symbol_visibility: 'inlineshidden',
	build_by_default: true,
	install: false,
	native: true
)
