// codebreaker/symbols/demangle.hpp -- a decorated linker name as the declaration it was made from.
//
//     declaration("?AddRef@CFoo@@UAGKXZ")   "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
//     short_("?AddRef@CFoo@@UAGKXZ")        "CFoo::AddRef"
//     parse("?AddRef@CFoo@@UAGKXZ")         a Parsed (name, kind, access, static, virtual, far, convention,
//                                           returns, parameters, const)
//     plain("_LZRead@12")                   ("LZRead", "__stdcall", 12)      (also "LZRead@12")
//
// The reading itself is the C++ engine of native/symbols/demangle.cc (namespace cbs); this module is
// its typed front: the answer comes back as a Parsed record instead of a dict.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::symbols::demangle {

// What a decorated C++ name says (the Python dict). A data name carries only name/kind/text.
struct Parsed {
    py::str name;
    py::str base;
    py::str class_;                  // key "class"
    py::str kind;                    // "function", "data" or "name"
    py::str access;
    bool static_ = false;            // key "static"
    bool virtual_ = false;           // key "virtual"
    bool far_ = false;               // key "far"
    py::str convention;
    py::str returns;
    py::list<py::str> parameters;
    py::str const_;                  // key "const"
    py::str text;
};

std::optional<py::str> type_name(const py::str& decorated);
std::optional<Parsed> parse(const py::str& name);
std::optional<py::str> declaration(const py::str& name);
std::optional<py::str> short_(const py::str& name);   // Python `short`
std::tuple<py::str, py::str, std::optional<int64_t>> plain(const py::str& name);
py::str readable(const py::str& name);
std::optional<int64_t> stack_bytes(const py::str& name);
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::demangle
