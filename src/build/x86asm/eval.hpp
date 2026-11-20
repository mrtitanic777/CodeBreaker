// build/x86asm/eval.hpp -- the expression evaluator (asm/eval.c) and the expression library (asm/exprlib.c).
//
// An expression is a vector of terms, each a kind of thing and a multiple of it: a register, an unknown (forward) reference,
// a plain number (EXPR_SIMPLE), the segment base of a segment (EXPR_SEGBASE + segment number), a WRT segment, an AVX-512
// rounding decorator. The evaluator reads tokens from a scanner and builds that vector with NASM's rules for what may be
// added, subtracted, multiplied and compared: `label2 - label1` in the same segment is a number, `label - $` is
// self-relative, `seg:offset` far-absolute values keep their segment, and only a number may be divided or shifted or
// bitwise combined. The terms are kept in ascending order of type, as NASM keeps them.
//
// The functions of exprlib.c (is_simple, is_reloc, reloc_value, ...) classify and take apart an evaluated expression; they
// are free functions here. Those that depend on the current segment take it as an argument (or take the assembly).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>

#include "build/x86asm/types.hpp"

namespace cb::build::x86asm {

class Assembly;
class Floats;
class LabelTable;

// ---- exprlib.c ----

// True if the expression is a simple scalar (or a far-absolute, which counts).
bool is_simple(const Expr& vect);
// True if it is a simple scalar and NOT a far-absolute.
bool is_really_simple(const Expr& vect);
// What the expression's terms make of it (expr_classes). `location_segment` is the segment being assembled into: the
// only segment base an expression may subtract (making it self-relative).
ExprClass expr_class(const Expr& vect, std::int32_t location_segment);
// True if it is relocatable: a simple scalar plus at most one segment base, possibly minus the current segment base, plus
// possibly a WRT.
bool is_reloc(const Expr& vect, std::int32_t location_segment);
// True if it has an `unknown' (forward reference) part.
bool is_unknown(const Expr& vect);
// True if it is nothing but an unknown part.
bool is_just_unknown(const Expr& vect);
// The scalar part of a relocatable vector (simple scalars qualify): 0 if there is none.
std::int64_t reloc_value(const Expr& vect);
// The segment number of a relocatable vector, or NO_SEG for a simple scalar.
std::int32_t reloc_seg(const Expr& vect);
// The WRT segment number of a relocatable vector, or NO_SEG if there is no WRT part.
std::int32_t reloc_wrt(const Expr& vect);
// True if it contains a subtraction of the current segment (the location).
bool is_self_relative(const Expr& vect, std::int32_t location_segment);

// The same, taking the segment being assembled into from the assembly (`location.segment`).
ExprClass expr_class(const Assembly& as, const Expr& vect);
bool is_reloc(const Assembly& as, const Expr& vect);
bool is_self_relative(const Assembly& as, const Expr& vect);

// ---- eval.c ----

// The scanner function handed to the evaluator (NASM's `scanner`, with its private data captured): read the next token into
// the token value and return its type (or character).
using EvalScanFn = std::function<TokenType(TokenVal&)>;

// The evaluator. It holds no state between calls to `evaluate`; it reads the assembly (the pass, the location, the limits,
// the MASM modes), the label table, and the floating-point converter. Many may exist at once.
class Evaluator {
public:
    Evaluator(Assembly& assembly, LabelTable& labels, const Floats& floats) : as_(assembly), labels_(labels), floats_(floats) {}

    // What the output format answers for `SEG x`, the segment base value of a segment (`ofmt->segbase`); unset: itself.
    std::function<std::int32_t(std::int32_t segment)> segbase;

    // Evaluate the expression the scanner `sc` supplies. `tv` holds the current token: if its type is TokenType::INVALID the
    // evaluator reads the first token itself, else it starts with the one in `tv`. On return `tv.type` is the token that
    // ended the expression. `fwref` (may be null) gets OPFLAG_FORWARD / OPFLAG_EXTERN set when the expression refers to a
    // not-yet-defined or an external symbol. `critical` means an undefined symbol is an error ("symbol `x' not defined", or
    // "... before use" in the first pass). `hints` (may be null) is told which register is likely the base.
    //
    // Returns nullopt after reporting the error, as NASM returns NULL.
    std::optional<Expr> evaluate(const EvalScanFn& sc, TokenVal& tv, int* fwref, bool critical, EvalHints* hints);

private:
    friend class EvalRun;
    Assembly& as_;
    LabelTable& labels_;
    const Floats& floats_;
};

}  // namespace cb::build::x86asm
