// codebreaker/invert/quad_dump.hpp -- SHIM over cb/invert/quad_dump.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer. (What the tool prints when it is given no file is the new usage text.)
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/invert/quad_dump.hpp"

namespace codebreaker::invert::quad_dump {

// the text the tool prints when it is given no file: its description and the opcode map
inline const py::str DOC = cb::compat::to_py(cb::invert::quad_dump::usage());

// opcode -> mnemonic
inline const py::dict<int64_t, py::str> OPC = [] {
    py::dict<int64_t, py::str> out;
    for (int op = 0; op < 256; ++op) {
        const auto name = cb::invert::quad_dump::opcode_name(static_cast<std::uint8_t>(op));
        if (!name.empty()) out.set(op, cb::compat::to_py(name));
    }
    return out;
}();

// The offset where the trailing run of printable ASCII and NUL bytes (the name pool) starts.
inline int64_t namepool_start(const py::bytes& d) {
    return cb::compat::to_py(cb::invert::quad_dump::namepool_start(cb::compat::from_py(d)));
}

// operands are packed little-endian words; show a compact view
inline py::str fmt_payload(const py::bytes& p) {
    const cb::Bytes bytes = cb::compat::from_py(p);
    return cb::compat::to_py(cb::invert::quad_dump::format_payload(bytes));
}

// Dump the quad file at `path` to stdout.
inline void main(const py::str& path) {
    cb::compat::guard([&] {
        const std::string label = cb::compat::from_py(path);
        const cb::Bytes file = cb::fs::read_file(cb::fs::from_text(label));
        std::string text = cb::invert::quad_dump::dump(cb::invert::quad_dump::parse(file), label);
        text.pop_back();   // (print adds the last newline)
        py::print(cb::compat::to_py(text));
    });
}

// The module's `if __name__ == "__main__":` block. `argv` is sys.argv (the program name first).
// -> the process's exit status (1 after the usage text, else 0).
inline int64_t cli(const py::list<py::str>& argv) {
    if (py::len(argv) < 2) {
        py::print(DOC);
        return 1;
    }
    main(argv[1]);
    return 0;
}

}  // namespace codebreaker::invert::quad_dump
