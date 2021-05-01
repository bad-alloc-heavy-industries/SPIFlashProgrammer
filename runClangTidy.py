#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
from argparse import ArgumentParser
from pathlib import Path
from subprocess import run
from concurrent.futures import ThreadPoolExecutor
from sys import exit

parser = ArgumentParser(
	description = 'Light-weight wrapper around clang-tidy to enable `ninja clang-tidy` to function properly',
	allow_abbrev = False
)
parser.add_argument('-s', required = True, type = str, metavar = 'sourcePath',
	dest = 'sourcePath', help = 'Path to the source directory to run clang-tidy in')
parser.add_argument('-p', required = True, type = str, metavar = 'buildPath',
	dest = 'buildPath', help = 'Path to the build directory containing a compile_commands.json')
args = parser.parse_args()

def globFiles():
	srcDir = Path(args.sourcePath)
	paths = set(('common', 'firmware', 'software'))
	suffixes = set(('c','C', 'cc', 'cpp', 'cxx', 'CC', 'h', 'H', 'hh', 'hpp', 'hxx', 'HH'))
	for path in paths:
		for suffix in suffixes:
			yield srcDir.glob('{}/**/*.{}'.format(path, suffix))

def gatherFiles():
	for fileGlob in globFiles():
		for file in fileGlob:
			yield file

extraArgs = []

futures = []
returncode = 0
with ThreadPoolExecutor() as pool:
	for file in gatherFiles():
		futures.append(pool.submit(run, ['clang-tidy'] + extraArgs + ['-p', args.buildPath, file]))
	returncode = max((future.result().returncode for future in futures))
exit(returncode)
