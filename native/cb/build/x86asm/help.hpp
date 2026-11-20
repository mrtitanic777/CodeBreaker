// cb/build/x86asm/help.hpp -- the text of `nasm -h` (the help function of asm/nasm.c).
#pragma once

#include <string>
#include <string_view>

namespace cb::build::x86asm {

// The help NASM prints for `-h what` (what: "", "opt", "all", "topics", "-f", "-w", "-O", "--limit", ...). `progname` is the name
// in the usage line; `current_format` is the shortname of the selected output format (marked [default] in the -f list).
std::string nasm_help(std::string_view what, std::string_view progname, std::string_view current_format);

}  // namespace cb::build::x86asm
