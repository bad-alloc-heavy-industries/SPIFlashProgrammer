# This file is part of SPI Flash Programmer (SPIFP)
# Copyright © 2014 Rachel Mant (dx-mon@users.sourceforge.net)
#
# SPIFP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SPIFP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include Makefile.inc

PKG_CONFIG_PKGS = libusb-1.0
EXTRA_CFLAGS = $(shell pkg-config --cflags $(PKG_CONFIG_PKGS))
CFLAGS = -c $(OPTIM_FLAGS) -I.. $(EXTRA_CFLAGS) -o $@ $<
LIBS = $(shell pkg-config --libs $(PKG_CONFIG_PKGS))
# -lstdc++
LFLAGS = $(O) $(LIBS) -o $(BIN)

O = strUtils.o USB.o flashprog.o
BIN = flashprog

default: all

all: $(BIN)

$(BIN): $(O)
	$(call run-cmd,ccld,$(LFLAGS))
	$(call run-cmd,chmod,$(BIN))
	$(call debug-strip,$(BIN))

clean:
	$(call run-cmd,rm,flashprog,$(BIN) $(O))

.c.o:
	$(call run-cmd,cc,$(CFLAGS))

.cpp.o:
	$(call run-cmd,cxx,$(CFLAGS))

.PHONY: default all clean .c.o .cpp.o
