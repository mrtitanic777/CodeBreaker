// cb/exact/bss.hpp -- the program's variables that begin without a value, each at its place (src/BSS).
//
//     codebreaker exact PROJECT bss [--write]
//
//     bss::Proposal p = bss::propose(*project);
//     std::vector<std::string> text = bss::lines(project->profile_parts()->communal, p);
//
// They lie from the end of the file's data (the load image ends there) to the stack's segment. In the sources they were plain
// definitions the compiler hands the linker as communal names, which the linker places in the order of its own table (a hash of the
// names) -- ours are not the original names, so each is given its place outright (exact/link.hpp makes an object of them). What is
// there:
//
//   - the program's own: the data places SYMBOLS learnt from the identical functions, in the range; and the sources' names for
//     data SYMBOLS has no place for (a variable only functions not yet identical use): the engine names data g_XXXX after its
//     place, DS:XXXX
//   - the library's: its modules' own _BSS segments (a "hole" the linker fills with them, found by the words of their code that
//     point into it), and its communal names (stdio's buffers) by the words of the library's code that name them
//
//     src/BSS:  range LO HI / segment NAME [para] / hole ADDR LEN / name ADDR      (hex)
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/exact/library.hpp"
#include "cb/exact/project.hpp"

namespace cb::exact::bss {

// A library module's own _BSS: where it stands in the data segment, how long it is, and the modules whose _BSS run together there.
struct Hole {
    std::int64_t address = 0, length = 0;
    std::vector<std::string> modules;
    friend bool operator==(const Hole&, const Hole&) = default;
};

struct Proposal {
    std::int64_t edata = 0, stack = 0;            // the range: the end of the file's data to the stack's segment (DS offsets)
    std::map<std::string, std::int64_t> names;    // name -> its DS offset (looked up, and listed by place)
    std::vector<Hole> holes;                      // ascending (none for a compiler whose communal names go to a segment of their own)
};

// `map`: the library stage's answer for the project (none: asked for).
Proposal propose(project::Project& project, const library::Map* map = nullptr);

// The text of src/BSS. `communal`: the toolchain profile's segment of communal names, when it has one.
std::vector<std::string> lines(const std::optional<project::Communal>& communal, const Proposal& proposal);

// `exact PROJECT bss [--write]`: the text goes to stdout; --write puts it in src/BSS.
int run(project::Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::bss
