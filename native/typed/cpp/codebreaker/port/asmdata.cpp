// codebreaker/port/asmdata.cpp -- SHIM over native/cb/port/asmdata (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. COMPAT(py): deleted with the last py:: consumer (port/__main__).
//
// The module is the data of an assembler module as C: its code is in native/cb/port/asmdata.cpp. What is here is
// conversion: a py::str is a text of code points read as Latin-1 (one byte each), and the old CTypes (the C program's
// types, laid out by asm16) is handed to the new code as the records it reads from it.

#include "cb/compat/py_bridge.hpp"
#include "cb/port/asmdata.hpp"
#include "codebreaker/port/asmdata.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::asmdata {

namespace newp = ::cb::port;
namespace cc = ::cb::compat;

py::list<py::str> typed_notes;

namespace {

std::string U(const py::str& s) { return pre_ansi::p5_shim::bytes_of(s); }
py::str P(std::string_view s) { return pre_ansi::p5_shim::text_of(s); }

}  // namespace

std::optional<py::str> typed(const py::str& text, const asm16::CTypes& ct) {
    newp::asm16::CTypes types;
    types.old_layouts = asm16::to_new(ct.old);
    for (const auto& kv : ct.vars.items())
        types.vars.insert_or_assign(U(std::get<0>(kv)), newp::asm16::VarType{U(std::get<0>(std::get<1>(kv))), U(std::get<1>(std::get<1>(kv)))});
    const std::optional<newp::asmdata::TypedData> made = cc::guard([&] { return newp::asmdata::typed(U(text), types); });
    if (!made) return std::nullopt;
    typed_notes = py::list<py::str>();
    for (const std::string& n : made->notes) typed_notes.append(P(n));
    return P(made->text);
}

std::optional<py::str> messages(const py::str& text) {
    const std::optional<std::string> made = cc::guard([&] { return newp::asmdata::messages(U(text)); });
    if (!made) return std::nullopt;
    return P(*made);
}

}  // namespace codebreaker::port::asmdata
