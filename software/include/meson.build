# SPDX-License-Identifier: BSD-3-Clause
config = configuration_data()
config.set('AUTOGEN_HEADER', '/* THIS FILE IS AUTOGENERATED, DO NOT EDIT */')
config.set('VERSION', '@VERSION@')
config.set('COMPILER', hostCXX.get_id())
config.set('COMPILER_VERSION', hostCXX.version())
config.set('TARGET_SYS', build_machine.system())
config.set('TARGET_ARCH', build_machine.cpu())

versionHeaderSrc = configure_file(
        configuration: config,
        input: 'version.hxx.in',
        output: 'version.hxx.in'
)

versionHeader = vcs_tag(
        input: versionHeaderSrc,
        output: 'version.hxx',
        replace_string: '@VERSION@',
        fallback: 'v@0@'.format(meson.project_version())
)
