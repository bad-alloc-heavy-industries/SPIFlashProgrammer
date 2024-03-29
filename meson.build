# SPDX-License-Identifier: BSD-3-Clause
project(
	'SPIFlashProgrammer',
	'cpp',
	default_options: [
		'cpp_std=c++17',
		'build.cpp_std=c++17',
		'warning_level=3',
		'buildtype=release',
		'strip=true',
		'b_ndebug=if-release',
		'b_lto=true'
	],
	version: '2.0.0',
	meson_version: '>= 0.55',
	subproject_dir: 'deps'
)

if not meson.is_cross_build()
	error('SPIFlashProgrammer must be cross-compiled to the target microcontroller and cannot be built for the host machine')
endif

targetCXX = meson.get_compiler('cpp', native: false)
hostCXX = meson.get_compiler('cpp', native: true)
commonInclude = include_directories('common/include')

subdir('firmware')
subdir('software')

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
