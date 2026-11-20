// cb/build/x86asm/preproc_needs.hpp -- what the preprocessor needs from the rest of the engine, as callbacks.
//
// The preprocessor (preproc.hpp) is written against NASM's C prototypes of the modules that are other agents' work: the
// scanner's identifier classifier (nasm_token_hash), the expression evaluator (evaluate), the file reader, the
// listing. Each is a std::function in `PpHost` so the preprocessor builds and is tested before they exist; the driver
// fills the callbacks in from the real modules. An unset callback does the nothing-at-all thing, except the ones marked.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

// The list-file event types the preprocessor reports (listing.h).
enum PpListType : int { LIST_READ, LIST_MACRO, LIST_INCLUDE };

// A function that hands the evaluator the next token (the `scanner` argument of NASM's evaluate()).
using PpScanFn = std::function<TokenType(TokenVal&)>;

struct PpHost {
    // nasm_token_hash(text, &tokval): classify an identifier (an instruction, register, size ... or a plain ID), filling
    // the token value as the scanner does; returns the token type. Used to recognize a size name in %arg/%local.
    std::function<TokenType(std::string_view text, TokenVal& tokval)> token_hash;

    // evaluate(scan, &tokval, NULL, critical, NULL): evaluate an expression from tokens. `tokval.type` is left as the
    // token that ended the expression (EOS when all was consumed). nullopt on an error (already reported).
    std::function<std::optional<Expr>(const PpScanFn& scan, TokenVal& tokval, int critical)> evaluate;

    // directive_valid(name): is this an assembler directive (for %ifdirective)? Unset: false.
    std::function<bool(std::string_view name)> directive_valid;

    // Read a file by the name the source gave (after the -I prefix is applied): its contents, or nullopt if it cannot
    // be opened. The main input file comes through here too. The driver resolves names against its in-memory files.
    std::function<std::optional<std::string>(const std::string& path)> read_file;

    // nasm_realpath: the full form of a path (for %realpath). Unset: the path itself.
    std::function<std::string(const std::string& path)> realpath;

    // The environment (getenv): the variable's value, or nullopt. Unset: no variables.
    std::function<std::optional<std::string>(const std::string& name)> getenv;

    // The listing (lfmt): a line (`lineno` -1 for none), going into and out of an include or macro, list_option('x').
    std::function<void(PpListType type, std::int32_t lineno, std::string_view text)> list_line;
    std::function<void(PpListType type)> list_uplevel;
    std::function<void(PpListType type)> list_downlevel;
    std::function<bool(char option)> list_option;

    // True on the final pass when the debug format wants the include / macro events: not used (no debug format).

    // readnum(text, &error): a number as the source writes it. Unset: the assembly's own (decimal and 0x hex).
    std::function<std::int64_t(std::string_view text, bool& error)> readnum;
};

// The tests the preprocessor makes of an evaluated expression (NASM's is_simple, is_really_simple, reloc_value). An
// expression ends at the first term of type 0 (NASM's terminator); terms of value 0 are insignificant.
namespace pp_detail {
inline std::size_t first_term(const Expr& e) {
    std::size_t i = 0;
    while (i < e.size() && e[i].type && !e[i].value) ++i;
    return i;
}
inline bool is_end(const Expr& e, std::size_t i) { return i >= e.size() || !e[i].type; }
}  // namespace pp_detail

// A simple scalar, or a far-absolute one.
inline bool pp_is_simple(const Expr& e) {
    std::size_t i = pp_detail::first_term(e);
    if (pp_detail::is_end(e, i)) return true;
    if (e[i].type != EXPR_SIMPLE) return false;
    do { ++i; } while (i < e.size() && e[i].type && !e[i].value);
    if (!pp_detail::is_end(e, i) && e[i].type < EXPR_SEGBASE + SEG_ABS) return false;
    return true;
}
// A simple scalar and not a far-absolute.
inline bool pp_is_really_simple(const Expr& e) {
    std::size_t i = pp_detail::first_term(e);
    if (pp_detail::is_end(e, i)) return true;
    if (e[i].type != EXPR_SIMPLE) return false;
    do { ++i; } while (i < e.size() && e[i].type && !e[i].value);
    return pp_detail::is_end(e, i);
}
// The value of a simple expression (0 for anything else).
inline std::int64_t pp_reloc_value(const Expr& e) {
    const std::size_t i = pp_detail::first_term(e);
    if (pp_detail::is_end(e, i)) return 0;
    return e[i].type == EXPR_SIMPLE ? e[i].value : 0;
}
// The first term's value (`evalresult->value`): the number a simple expression stands for.
inline std::int64_t pp_first_value(const Expr& e) { return e.empty() ? 0 : e[0].value; }

}  // namespace cb::build::x86asm
