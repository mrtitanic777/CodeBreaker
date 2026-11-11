// codebreaker/invert/segment_thunk.hpp -- C++17 port of codebreaker.invert.segment_thunk.
//
// segment_thunk.py -- the whole-function template for 16-bit segment-switch thunks.
//
// Much of the framed corpus (the printer and device drivers especially) is generated assembly that
// switches DS to a fixed data segment and forwards to a handler, or returns 0:
//
//     <far loadds prologue>          ; mov ax,ds | push ds;pop ax; nop; inc bp; push bp; mov bp,sp; push ds; mov ds,ax
//     mov ax, <seg> ; mov ds, ax     ; switch DS to the fixed data segment
//     <body>                         ; forwarder: push args; call far <target>   stub: xor ax,ax [; cdq]
//     <lea/std epilogue> ; retf <N>
//
// parse() reads that grammar (None the instant a byte is not explained), verify() also re-lays the
// bytes from the parse and demands they are the function's, and emit_asm() writes the MASM.  C++
// (native/invert/msc6_helpers.cc; docs/NATIVE.md); this calls its entry point (cbinv::segment_thunk).
//
// Typing notes:
//   * The parsed thunk (the Python's dict {pro, seg, body [(name, raw)], epi, retf, pad, ...}) is the record
//     Thunk.  `fb` is the function's bytes, `asm_` the MASM text for the name the parse was made with ("thunk"),
//     `kind` "forwarder" or "return-stub"; pro, epi and pad are the prologue, epilogue and padding bytes.
//   * parse and verify answer None as an empty optional.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::segment_thunk {

struct Thunk {
    py::str asm_;
    py::str kind;
    py::bytes pro;
    int64_t seg = 0;
    py::list<std::tuple<py::str, py::bytes>> body;
    py::bytes epi;
    std::optional<int64_t> retf;
    py::bytes pad;
    py::bytes fb;
};

// {pro, seg, body [(name, raw)], epi, retf, pad} or None.
std::optional<Thunk> parse(const py::bytes& fb);

// The parse, when re-laying it gives exactly these bytes; else None.
std::optional<Thunk> verify(const py::bytes& fb);

// The thunk as a 16-bit MASM procedure.
py::str emit_asm(const Thunk& t, const py::str& name = py::str("thunk"));

py::str kind(const Thunk& t);

}  // namespace codebreaker::invert::segment_thunk
