// cb/build/x86asm/scan.hpp -- the standard scanner (asm/stdscan.c) and the reading of numbers (nasmlib/readnum.c).
//
// The scanner turns one line of source, a token at a time, into the `TokenVal`s the expression evaluator, the parser and
// some output formats read: an operator (its own character, or a TokenType for a two- or three-character one), a number
// (`TokenType::NUM`, or `ERRNUM` when malformed), a floating-point constant (`FLOAT`, the text kept for the float
// converter), a quoted string (`STR`, with its bytes unquoted in `text` and their count in `inttwo`; `ERRSTR` if it is not
// closed), `$` (`HERE`), `$$` (`BASE`), a keyword (register, instruction, prefix, size, `SEG`, ... see tokhash.hpp), a
// braced decorator ({rn-sae}, {1to8}, {k1}, {dfv=cf,zf}), or an identifier (`ID`).
//
// A scanner reads a buffer the caller keeps alive; `reset` points it at a new one. It has no global state: each user of the
// standard scanner (the parser, the directives, an output format's directive) makes its own, or shares one.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string_view>
#include <vector>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

class Assembly;

// Convert a number as the source writes it to its value, by NASM's rules: `0x1F`, `$1F` (when the pass allows $ hex),
// `1Fh`, `0h1F`, `0b101`/`101b`/`0y`/`y`, `0o17`/`17o`/`0q`/`q`, `0d99`/`99d`/`t`, decimal, with `_` as a separator, and an
// optional leading `-` (readnum). `error` is set when the text is not a number. A number that does not fit in 64 bits warns
// (numeric-overflow) and wraps. This is the function `Assembly::hooks.readnum` should be set to.
std::int64_t readnum(Assembly& as, std::string_view str, bool& error);

// What a scanner has read of its buffer: where it is, and the tokens pushed back (stdscan_state).
struct ScanState {
    std::size_t pos = 0;
    std::vector<TokenVal> pushback;   // a stack: the last is returned first
};

class Scanner {
public:
    explicit Scanner(Assembly& assembly) : as_(assembly) {}

    // Start scanning `buffer` from its beginning, dropping any pushed-back tokens (stdscan_reset). The caller keeps the
    // buffer alive, and unchanged, for as long as tokens from it (their `start`) are used.
    void reset(std::string_view buffer);

    // Read the next token into `tv` and return its type: the TokenType value, or the character for a one-character
    // operator, 0 (EOS) at the end of the line or at a `;` comment (stdscan). A token pushed back is returned first.
    int scan(TokenVal& tv);

    // The scanner as the callable the evaluator (EvalScanFn) and the preprocessor's host (PpScanFn) take: it must outlive it.
    std::function<TokenType(TokenVal&)> as_function() {
        return [this](TokenVal& tv) { return static_cast<TokenType>(scan(tv)); };
    }

    // Return `tv` to be read again, before anything else (stdscan_pushback).
    void pushback(const TokenVal& tv) { state_.pushback.push_back(tv); }

    // The unread rest of the buffer (stdscan_tell).
    std::string_view tell() const { return buffer_.substr(state_.pos <= buffer_.size() ? state_.pos : buffer_.size()); }

    // Save and restore the position (stdscan_get, stdscan_set).
    ScanState get() const { return state_; }
    void set(const ScanState& state) { state_ = state; }

private:
    int token(TokenVal& tv);
    int symbol(TokenVal& tv);
    int parse_braces(TokenVal& tv);
    // The character at `i`, or NUL past the end (the buffer is a C string in NASM).
    char at(std::size_t i) const { return i < buffer_.size() ? buffer_[i] : '\0'; }
    std::size_t skip_spaces(std::size_t i) const;

    Assembly& as_;
    std::string_view buffer_;
    ScanState state_;
};

}  // namespace cb::build::x86asm
