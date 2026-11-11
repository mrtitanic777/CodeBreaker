// cb/build/asm_text.hpp -- what CodeBreaker's own source assemblers (Z80, 6502, PowerPC) share: the error types
// an assembly can fail with, the label table, the comment and operand-list scanners, and the interface a CPU's
// instruction syntax implements for the directive layer above it.
//
// The assemblers read the dialects the source writers produce (WLA-DX, ca65, sjasmplus, Zilog syntax). Each is a
// directive layer (labels, banks, data, includes) over a CPU's encoder; the encoder is an `Encoder`, so the
// directive layer is tested on its own and the instruction tables stay with the decompiler that owns them.
//
// Source text is bytes: an assembler never decodes it. A string literal in a data directive becomes exactly the
// bytes it holds between the quotes (a Latin-1 file is its own byte values; a UTF-8 file stays UTF-8).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::build {

// name -> value. Looked up only, never iterated into an output, so a hash map.
using Labels = std::unordered_map<std::string, std::int64_t>;

// The source does not assemble. `line` is the 1-based line of the source (0 when no single line is at fault);
// the message names the line's text and the reason.
class AsmError : public ParseError {
public:
    AsmError(std::uint32_t line, std::string_view text, const std::string& reason)
        : ParseError("", line, 0, text.empty() ? reason : std::string(text) + " (" + reason + ")") {}
    explicit AsmError(const std::string& reason) : ParseError("", 0, 0, reason) {}
    CB_ERROR_BOILERPLATE(AsmError)
};

// A name an expression or instruction uses has no value (yet). The first pass of an assembler expects this for
// labels defined further on; the last pass reports it. Carried by the encoder's Result.
class UndefinedSymbolError : public ParseError {
public:
    explicit UndefinedSymbolError(std::string name)
        : ParseError("", 0, 0, "undefined symbol '" + name + "'"), name_(std::move(name)) {}
    const std::string& name() const { return name_; }
    CB_ERROR_BOILERPLATE(UndefinedSymbolError)

private:
    std::string name_;
};

// One CPU's instruction syntax, as the directive layer of a source assembler needs it.
class Encoder {
public:
    virtual ~Encoder() = default;
    // The value of an expression (numbers, labels, + - * / & | ^ << >> and parentheses, `<x` `>x`) at address
    // `pc`. UndefinedSymbolError for a name that has no value.
    virtual Result<std::int64_t> value(std::string_view expr, const Labels& labels, std::int64_t pc) const = 0;
    // The bytes of one instruction line at address `pc`. UndefinedSymbolError for a label it needs and lacks.
    virtual Result<Bytes> assemble(std::string_view line, std::int64_t pc, const Labels& labels) const = 0;
};

// The part of `line` before a `;` that is not inside a double-quoted string. Not trimmed.
std::string_view strip_comment(std::string_view line);

// The comma-separated items of a data directive, each trimmed; a string keeps its commas. With `track_parens`
// a comma inside parentheses does not separate either (Z80 operands: `LD A,(IX+1)`). An empty last item is
// dropped, as `.byte 1,2,` means two bytes.
std::vector<std::string_view> split_items(std::string_view text, bool track_parens);

// A number as the dialects write it: decimal, 0x1F, 0b101, 0o17, with an optional sign. Nullopt for anything
// else, or a value past 64 bits.
std::optional<std::int64_t> parse_number(std::string_view text);

// True when `error` is an UndefinedSymbolError (the retryable failure of an encoder).
inline bool is_undefined(const Error& error) { return error.kind() == "UndefinedSymbolError"; }

}  // namespace cb::build
