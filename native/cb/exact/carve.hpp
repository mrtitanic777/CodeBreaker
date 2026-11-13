// cb/exact/carve.hpp -- where the program's own functions begin and end.
//
//     codebreaker exact PROJECT carve [--write] [-v]
//
//     std::vector<carve::Function> fs = carve::functions(*project);      // {address, length, why}
//
// The program's own code is what lies before the library part (exact/library.hpp), less the kit's objects linked first. In it a
// function begins
//
//   - where the compiler's prologue stands (the toolchain profile's "carve": "prologues", byte patterns as hex regular
//     expressions; one whose group 1 is a near call's displacement counts only when the call goes to the routine
//     "prologue_call" names -- C 3.00's __chkstk)
//   - where a frame is set up (push bp / mov bp,sp) right after a return or a pad: a function compiled without the stack probe
//     (-Gs), or by hand
//   - where a near call of the code goes (the code disassembled from each start to the next)
//
// and ends where the next begins (or the program's own code ends). A start the project knows better can be added or taken away:
// project "starts_add" / "starts_drop" (image offsets).
//
// src/FUNCS (--write): one start a line, the length and why after it.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/exact/library.hpp"
#include "cb/exact/project.hpp"

namespace cb::exact::carve {

// The part of the image the program's own functions are in: [lo, hi), less the ranges that are not its own (the kit's objects and
// the library modules among its code), ascending.
struct Region {
    std::int64_t lo = 0, hi = 0;
    std::vector<std::pair<std::int64_t, std::int64_t>> skip;
    bool inside(std::int64_t address) const;
};

// `map`: the library stage's answer for the project (none: asked for).
Region region(project::Project& project, const library::Map* map = nullptr);

struct Function {
    std::uint32_t address = 0, length = 0;
    std::string why;                       // what began it: "prologue", "frame after a return", "called from 01a2c"
    friend bool operator==(const Function&, const Function&) = default;
};

// The program's own functions in address order. FormatError for a start (project "starts_add") past where the program's own code ends.
std::vector<Function> functions(project::Project& project, const library::Map* map = nullptr);

// A pattern of hex bytes ("55 8b ec (?:33c0|b8....) e8 (....)") as a regular expression over bytes (cb::Regex): two hex digits a
// byte (written \xHH, so that a byte that is a metacharacter is only itself), '..' any byte, and the regex's own ( ) | ? * + : left
// as they are.
std::string hex_pattern(std::string_view pattern);

// What exact/project asks of carve (Services::carve): the functions of a project, as it knows them.
std::function<std::vector<project::CodeFunction>(project::Project&)> service();

// `exact PROJECT carve [--write] [-v]`: the functions (with -v) and a summary go to stdout; --write puts them in src/FUNCS.
int run(project::Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::carve
