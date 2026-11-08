// codebreaker/emulate/pcode.hpp -- interface of codebreaker.emulate.pcode (typed C++; implementation pending).
//
// pcode.py -- an instruction's p-code, from the decompiler's own SLEIGH specification.
//
//     lifter = pcode::lifter("x86-16-real");   // (or "x86-32-flat", "ppc-be-64"); null when the lifting program is
//                                              // not there, or has not that mode
//     ins = lifter->lift(code_bytes, offset);  // Lifted: .length, .ops, ...; null if it does not decode
//
// An op is (OPCODE, out, inputs): a varnode is (space, offset, size) with space "r" register, "u" unique, "c" constant,
// "m" ram; out is None for none; a user operation's first input is its name (a str, "segment"). A 16-bit instruction
// is lifted at its offset in its segment.  The program is found as CB_PCODECHECK names it, else beside the decompiler
// (the setting decompiler_bin); one built before the lift mode existed is not used (lifter() is null).
//
// Typing notes:
//   * Varnode is a struct; an op's input is a Varnode, a text (a user operation's name, "@name" in the line) or
//     None ("-"): `Input`.  An op is `Op`.
//   * The Lifter owns a pcodecheck subprocess (py::subprocess); its private state (process, memo, lock) is in the
//     .cpp.  lifter() hands out the process-wide Lifter for a mode as a shared_ptr (null: the Python's None).
//     lift() answers the SAME Lifted for the same (code, offset): shared_ptr<const Lifted>, null when it does not decode.
//   * Constructing a Lifter raises py::OSError ("not a lifting pcodecheck") when the program does not answer `regs`.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::emulate::pcode {

// (space, offset, size)
struct Varnode {
    py::str space;      // "r" | "u" | "c" | "m"
    int64_t offset = 0;
    int64_t size = 0;
};
// One input of an op: a varnode, a user operation's name, or None.
using Input = std::variant<py::none_t, Varnode, py::str>;
// (OPCODE, out, inputs)
struct Op {
    py::str opcode;
    std::optional<Varnode> out;
    py::list<Input> inputs;
};

// One instruction's p-code, and what the shadow needs to know of it without running it: the register bytes it reads and
// writes, whether it reaches memory, a user operation or a branch (anything that must be followed op by op).
struct Lifted {
    int64_t length = 0;
    py::list<Op> ops;
    py::set<int64_t> reads;
    py::set<int64_t> writes;
    bool full = false;

    Lifted(int64_t length, const py::list<Op>& ops);   // works out reads / writes / full from the ops
};

class Lifter {
  public:
    py::str mode;
    bool window = false;                                // mode == "x86-16-real"
    py::dict<py::str, std::tuple<int64_t, int64_t>> regs;   // name -> (offset, size)

    Lifter(const py::str& exe, const py::str& sla, const py::str& mode);
    ~Lifter();
    // pcodecheck's line for the instruction at the start of `code` at `offset` (the C++ shadow parses it, and keeps it
    // for the process: emulate/native_shadow.py).
    py::str line(const py::bytes& code, int64_t offset);
    // The p-code of the instruction at the start of `code` (16 bytes are enough for x86), at `offset`; null when it
    // does not decode.
    std::shared_ptr<const Lifted> lift(const py::bytes& code, int64_t offset);

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// the specification each lift mode reads (native/decompiler/languages)
extern const py::dict<py::str, py::str> SLA;

// CB_PCODECHECK, else decompiler_bin/pcodecheck.exe
py::str _exe();

// The process's lifter for `mode`, started on first use; null when there is none to start.
std::shared_ptr<Lifter> lifter(const py::str& mode = py::str("x86-16-real"));

// What the lifted p-code depends on (for keys of anything kept from it): the program and the specification
// (base/cache.py tool).
std::tuple<py::str, py::str> stamp(const py::str& mode = py::str("x86-16-real"));

}  // namespace codebreaker::emulate::pcode
