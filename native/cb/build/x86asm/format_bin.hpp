// cb/build/x86asm/format_bin.hpp -- the `bin` output format (NASM's output/outbin.c): flat binary, Intel hex (`ith`) and
// Motorola S-records (`srec`), with sections (start=, vstart=, follows=, align=, nobits), ORG and the `[map ...]` map file.
//
// The factory is `make_bin_format` (format.hpp); this header only says so, for a reader who looks for the format by its
// file name.
#pragma once

#include "cb/build/x86asm/format.hpp"
