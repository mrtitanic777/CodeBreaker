// cb/build/x86asm/expr_util.hpp -- the tests the parser, the directives and the output formats make of an evaluated
// expression (NASM's asm/exprlib.c: is_simple, is_really_simple, expr_class, is_reloc, reloc_value, reloc_seg, reloc_wrt).
//
// An `Expr` is a vector of terms; NASM ends the array with a term of type 0, so here the first term of type 0 (if any) ends
// it too; terms of value 0 are insignificant. These live in namespace `eu` so they never collide with the expression
// evaluator's own copies.
#pragma once

#include <cstdint>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm::eu {

// The type of term `i`, 0 past the end (the terminator).
inline std::int32_t type_at(const Expr& e, std::size_t i) { return i < e.size() ? e[i].type : 0; }
inline std::int64_t value_at(const Expr& e, std::size_t i) { return i < e.size() ? e[i].value : 0; }

// The index of the first significant term (type != 0 and value != 0), or the end.
inline std::size_t first(const Expr& e) {
    std::size_t i = 0;
    while (type_at(e, i) && !value_at(e, i)) ++i;
    return i;
}

inline bool is_simple(const Expr& e) {
    std::size_t i = first(e);
    if (!type_at(e, i)) return true;
    if (type_at(e, i) != EXPR_SIMPLE) return false;
    do {
        ++i;
    } while (type_at(e, i) && !value_at(e, i));
    if (type_at(e, i) && type_at(e, i) < EXPR_SEGBASE + SEG_ABS) return false;
    return true;
}

inline bool is_really_simple(const Expr& e) {
    std::size_t i = first(e);
    if (!type_at(e, i)) return true;
    if (type_at(e, i) != EXPR_SIMPLE) return false;
    do {
        ++i;
    } while (type_at(e, i) && !value_at(e, i));
    return !type_at(e, i);
}

// What an expression turned out to be; `segment` is the segment of the current location (a self-relative reference
// subtracts it).
inline int expr_class(const Expr& e, std::int32_t segment) {
    int cls = EC_ZERO;
    for (std::size_t i = 0; type_at(e, i); ++i) {
        const std::int32_t type = e[i].type;
        const std::int64_t value = e[i].value;
        if (!value) {
            // value-0 term
        } else if (type < EXPR_UNKNOWN) {
            if ((cls & EC_REGISTER) || value != 1)
                cls |= EC_REGEXPR;
            else
                cls |= EC_REGISTER;
        } else if (type == EXPR_UNKNOWN) {
            cls |= EC_UNKNOWN;
        } else if (type == EXPR_SIMPLE) {
            cls |= EC_CONST;   // pure number term
        } else if (type == EXPR_WRT) {
            cls |= EC_WRT;
        } else if (type < EXPR_SEGBASE) {
            cls |= EC_COMPLEX;
        } else if (type >= EXPR_SEGBASE + SEG_ABS) {
            // it is an absolute segment
            if (cls & (EC_SEG | EC_SEGABS)) cls |= EC_COMPLEX;
            cls |= EC_SEGABS;
        } else {
            // it is a segment
            if (value == 1) {
                if (cls & (EC_SEG | EC_SEGABS)) cls |= EC_COMPLEX;
                cls |= EC_SEG;
            } else if (value == -1) {
                // can only subtract current segment, and only once
                if (type != segment + EXPR_SEGBASE || (cls & EC_SELFREL)) cls |= EC_COMPLEX;
                cls |= EC_SELFREL;
            } else {
                cls |= EC_COMPLEX;   // non-simple segment arithmetic
            }
        }
    }
    return cls;
}

inline bool is_reloc(const Expr& e, std::int32_t segment) { return !(expr_class(e, segment) & ~EC_RELOC); }

inline std::int64_t reloc_value(const Expr& e) {
    const std::size_t i = first(e);
    if (!type_at(e, i)) return 0;
    return type_at(e, i) == EXPR_SIMPLE ? e[i].value : 0;
}

inline std::int32_t reloc_seg(const Expr& e) {
    for (std::size_t i = 0; type_at(e, i); ++i)
        if (e[i].type >= EXPR_SEGBASE && e[i].value == 1) return e[i].type - EXPR_SEGBASE;
    return NO_SEG;
}

inline std::int32_t reloc_wrt(const Expr& e) {
    std::size_t i = 0;
    while (type_at(e, i) && type_at(e, i) < EXPR_WRT) ++i;
    if (type_at(e, i) == EXPR_WRT) return static_cast<std::int32_t>(e[i].value);
    return NO_SEG;
}

}  // namespace cb::build::x86asm::eu
