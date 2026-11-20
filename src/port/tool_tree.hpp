// port/tool_tree.hpp -- the source tree the port reads: its C, its assembler modules, and the recovery's lists beside them.
//
// SOURCE_DIR is read, never written: its C (`*.c`, `*.h`, in it or its `src/`) and its assembler modules (`*.asm`, in it, its
// `asm/` or its `src/`). Beside them the recovery may have left two lists that say where the old linker put the variables:
//
//     SYMBOLS   `name D address`        a variable's place in the data segment
//     BSS       `range from to`, `hole at length`, `name offset`    the variables that began without a value, by DS offset
//
// Text is read as the old tools wrote it, one byte a character (Latin-1), with line ends made `\n`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "core/ordered_map.hpp"
#include "port/c_source.hpp"

namespace cb::port::tool {

using Files = pre_ansi::Files;

struct SourceTree {
    Files c;                                                   // the C files by name, in name order (src/ over the folder)
    OrderedMap<std::string, std::string> assembler;            // the assembler modules by name
    std::map<std::string, std::filesystem::path> assembler_paths;
};

// Reads the tree. A name met in two folders is the later folder's (src/ over the folder), in the earlier one's place.
SourceTree read_tree(const std::filesystem::path& source_dir);

// The text with CRLF and lone CR made LF (how a text file is read).
std::string normalize_newlines(std::string_view text);

// {C name: its place in the data segment} of the SYMBOLS list (name, `D`, hex address; a leading `_` is not part of the C name).
// Empty when the tree has none.
OrderedMap<std::string, std::uint64_t> read_symbols(const std::filesystem::path& source_dir);

// {C name: its bytes} of the variables the BSS list names: each runs to the next one's offset, to the end of the range, or to a
// hole that begins first.
OrderedMap<std::string, std::uint64_t> read_bss(const std::filesystem::path& source_dir);

// [(old address in the data segment, name)] of the program's variables from a SYMBOLS file beside an assembler module, sorted.
std::vector<std::pair<std::uint32_t, std::string>> data_places(const std::filesystem::path& assembler_module);

}  // namespace cb::port::tool
