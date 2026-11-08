// codebreaker/symbols/wine16.hpp -- C++ from codebreaker.symbols.wine16 (typed port).
//
// wine16.py -- the 16-bit Windows modules' exports as Wine's .spec files type them.
//
//     wine16::exports(str("SETUPX"))[7]   -> Export{name="IpFindFirstLine", convention="pascal",
//                                            args=["word", "str", "str", "ptr"], ret16=false, stub=true}
//
// Wine (LGPL; settings: wine16 -- its `dlls` folder, the *.dll16 / *.exe16 / *.drv16 folders read)
// reimplements the 16-bit system modules -- KERNEL, USER, GDI, SETUPX, the drivers -- and declares
// every export in a .spec file: its ordinal, convention, the kind of each argument and whether the
// result is 16 bits (-ret16) or 32. An export Wine leaves unimplemented (`stub`) often carries its
// arguments in a comment (`stub IpFindFirstLine #(word str str ptr)`). Argument kinds, as pushed:
//
//     word, s_word          2 bytes (unsigned, signed)
//     long, segptr, segstr  4 bytes (a number; a far pointer passed as such)
//     ptr, str              4 bytes, a far pointer (Wine converts it to a flat one)
//
// The order is the declaration's: for pascal the first is pushed first.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::wine16 {

// Python's namedtuple Export("name convention args ret16 stub"); args is None when nothing is said
// of the arguments (or a kind is not read here).
struct Export {
    py::str name;
    py::str convention;
    std::optional<py::list<py::str>> args;
    bool ret16 = false;
    bool stub = false;
};

extern const py::dict<py::str, int64_t> WIDTH;
extern const py::set<py::str> POINTER;
extern const py::re::Pattern _LINE;
// module -> {ordinal: Export}
extern py::dict<py::str, py::dict<int64_t, Export>> _cache;

py::str _folder();
py::str _module(const py::str& spec_name);

// {ordinal: Export} of a 16-bit module (KERNEL, USER, SETUPX, ...), or {} when Wine has none
// (the dict is the cached one, shared as Python's is)
py::dict<int64_t, Export> exports(const py::str& module);

py::dict<py::str, py::dict<int64_t, Export>> _read_all();

// {ordinal: Export} of one .spec file's text
py::dict<int64_t, Export> read_spec(const py::str& text);

// the reconstruction's type for an argument kind
py::str c_type(const py::str& kind);

}  // namespace codebreaker::symbols::wine16
