// cb/invert/early_c_forward.hpp -- the forward model of the MC2/LC2 code generator: the quad records of a function to the
// exact 8086 bytes the compiler made of them, with no compiler involved. The first brick of the compiler-inverse engine:
// a model that predicts byte for byte is evidence that the code-generation templates are understood, and the quad/object
// pairs of quad_align are the ground truth it is checked against.
//
// The model covers integer arithmetic (16 and 32 bit), relationals feeding an `if`, assignment, globals, pointers and
// struct fields, `if` and `while`, and `x = x +/- 1` as inc/dec. An operand is a constant, a variable (a global, or a
// parameter living at [bp+N]), a dereferenced pointer parameter with a field offset, or the previous result in ax.
// Anything else is a shape the model does not know, and model() says so (UnsupportedError) instead of guessing.
//
//     const auto quads = quad_align::parse_quads(quad_file);
//     Result<Bytes> predicted = early_c_forward::model(quads[0]);
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/invert/quad_align.hpp"

namespace cb::invert::early_c_forward {

// One operand of a quad record's payload.
struct Operand {
    enum class Kind {
        Const,   // an immediate: `value`
        Node,    // a variable: `id`; a parameter when id > the function's own id, else a global; `type` 4 = long
        Deref,   // a pointer parameter `id` read or written through, at field offset `field`
        Temp,    // the result of the previous quad, already in ax
    };
    Kind kind = Kind::Temp;
    std::uint16_t value = 0;   // Const
    std::uint16_t id = 0;      // Node, Deref
    std::uint8_t type = 0;     // Node
    std::uint8_t field = 0;    // Deref
};

// The operands a payload holds, up to the first byte that does not start one (the rest is the quad's own trailer). A
// constant cut short by the end of the payload is a FormatError.
std::vector<Operand> parse_operands(ByteView payload);

// The id of the function a FUNC-BEGIN payload names (FormatError if it is too short).
std::uint16_t parse_funcbegin(ByteView payload);

// The code the compiler made of one function (its quads, FUNC-BEGIN first): prologue, body, epilogue, with the short
// jumps resolved. Calls and global addresses are left as the zero placeholders the object's relocations fill in.
// UnsupportedError for a shape the model does not cover (a long relational, a store to a constant, a jump out of the
// rel8 range, ...); FormatError for quads that are malformed (a missing operand, an unmatched label).
Result<Bytes> model(const quad_align::QuadFunction& function);

// How one object function fared.
struct Check {
    std::string name;
    Bytes original;                   // what the compiler emitted
    std::optional<Bytes> predicted;   // absent when the model could not handle it, or the quad file had no such function
    bool exact() const { return predicted && equal_bytes(*predicted, original); }
};

// Every function of the object that looks like compiled code, modelled from the quad function at the same position.
std::vector<Check> check(ByteView quad_file, ByteView object);

// The tool's output for those checks: a line per function (the predicted and original bytes under a failure), then
// "MC2 FORWARD MODEL: n/m BYTE-EXACT".
std::string format_checks(const std::vector<Check>& checks);

// `early_c_forward <quads> <object>`: the report on stdout. Returns the exit code (0; UsageError for a missing argument).
int command(std::span<const std::string> args);

}  // namespace cb::invert::early_c_forward
