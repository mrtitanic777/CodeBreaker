// codebreaker/invert/early_c_forward.hpp -- SHIM over cb/invert/early_c_forward.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer (invert/run, which names the module as a tool).
//
// The Python's helpers that only the model used (the hex-string byte builder `bs`, the operand dicts, the action emitters) are not
// carried over: they are cb::invert::early_c_forward's internals. What the old callers reach is kept: the model, the FUNC-BEGIN
// reader and the script.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/invert/early_c_forward.hpp"
#include "codebreaker/invert/quad_align.hpp"
#include "py.hpp"

namespace codebreaker::invert::early_c_forward {

using QuadRecord = codebreaker::invert::quad_align::QuadRecord;

inline int64_t parse_funcbegin(const py::bytes& pl) {
    return cb::compat::guard([&] { return int64_t(cb::invert::early_c_forward::parse_funcbegin(cb::compat::from_py(pl))); });
}

// The function's bytes; raises (as the Python did) for a function the model cannot handle.
inline py::bytes model(const py::list<QuadRecord>& func) {
    return cb::compat::guard([&] {
        cb::invert::quad_align::QuadFunction function;
        for (const QuadRecord& r : func) function.push_back({static_cast<std::uint8_t>(std::get<0>(r)), cb::compat::from_py(std::get<1>(r))});
        return cb::compat::to_py(cb::ByteView(cb::invert::early_c_forward::model(function).value()));
    });
}

// The script: `python early_c_forward.py <quads> <obj>`; `argv` is sys.argv (the program name first).
inline void main(const py::list<py::str>& argv) {
    const py::str quads = argv[1], object = argv[2];   // (IndexError for a missing argument)
    cb::compat::guard([&] {
        std::string text = cb::invert::early_c_forward::format_checks(cb::invert::early_c_forward::check(
            cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(quads))), cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(object)))));
        text.pop_back();   // (print adds the last newline)
        py::print(cb::compat::to_py(text));
    });
}

}  // namespace codebreaker::invert::early_c_forward
