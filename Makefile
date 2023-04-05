# See LICENSE file for license details.
# © 2023 Philipp Lindner <vredez at yahoo dot com>

CXX ?= g++
CXXFLAGS ?= -O3
PREFIX ?= /usr/local/bin/
STRIP = strip

src_file := cpustatus.cpp
target := $(basename $(src_file))
installed := $(PREFIX)$(target)

$(target): $(src_file)
	$(CXX) $(CXXFLAGS) "$<" -o "$@"

$(installed): $(target)
	install -s --strip-program=$(STRIP) -m 755 "$<" "$@"

.PHONY: install
install: $(installed)

.PHONY: uninstall
uninstall:
	rm -rf "$(installed)"

.PHONY: clean
clean:
	rm -f "$(target)"
