// cb/invert/msc6_segment.hpp -- what is read of a Microsoft C 6.0a code segment beside its single functions (cb/invert/msc6.hpp):
//
//     infer_signatures   the parameters of a segment's internal functions, from how they are called: each callee's `retf N` (the exact parameter
//                        bytes), the parameters its body loads far or as a long, and the far pointers pushed (a segment word, then an offset) at
//                        every near call site -- one body alone misreads them (a far pointer only passed through is never `les`'d)
//     thunk              the segment-switch thunks the linker generates (`mov ax, SEG; mov ds, ax; ... call far`), parsed, laid out again byte for
//                        byte, and written as the MASM that makes them
//     shapes             a function that is one known shape, as the C that compiles to it (the forwarding thunk)
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/invert/msc6.hpp"

namespace cb::invert::msc6_segment {

// A framed function of the segment: where it starts, and its parameters from the first (named p0 p1 ...).
struct Signature {
    std::uint32_t offset = 0;
    std::vector<msc6::Param> params;
};

// The functions of `segment` that have parameters, in code order.
std::vector<Signature> infer_signatures(ByteView segment);

enum class ThunkKind { Forwarder, ReturnStub };

struct ThunkPart {
    std::string op;     // "pushd" "pushw" "pushm" "pushr" "pushseg" "popseg" "pushi8" "pushi16" "lcall" "zero" "cdq" "cwd" "addsp" "movax"
    Bytes bytes;
};

struct Thunk {
    Bytes prologue, epilogue, padding;
    int segment = 0;                   // the data segment the thunk loads
    std::optional<int> retf;           // the far return's byte count; none for a plain `retf`
    std::vector<ThunkPart> body;
    ThunkKind kind() const;
    Bytes bytes() const;               // the thunk laid out again
};

// The thunk `function` is, if it is one. `exact`: only when laying it out again gives the function's own bytes.
std::optional<Thunk> thunk(ByteView function, bool exact = true);
// The MASM of a thunk, as the procedure `name`.
std::string thunk_assembly(const Thunk& t, std::string_view name);

struct Shape {
    std::string name;     // "thunk"
    std::string text;
};
// The C that compiles to `function`, when it is one known shape: the candidates, tried in order.
std::vector<Shape> shapes(ByteView function);

}  // namespace cb::invert::msc6_segment
