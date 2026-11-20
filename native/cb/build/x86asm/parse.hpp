// cb/build/x86asm/parse.hpp -- the source line parser (NASM's asm/parser.c, with CodeBreaker's MASM-mode parts).
//
// `parse_line` turns one line of preprocessed source into an `Insn` (types.hpp) for the encoder: the label, the prefixes
// and TIMES count, the opcode, and the operands -- registers, immediates (with the flags the encoder matches on), memory
// references with their base, index, scale and segment override -- or, for DB/DW/..., the extended operands (strings,
// floats, numbers, DUP). An error is reported through the `Assembly` and the result's opcode is left I_none.
//
// The parser reads tokens through a `Scanner` (scan.hpp) and evaluates expressions through the `ParseHost` callbacks (the
// evaluator and the label manager are other modules).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/floats.hpp"
#include "cb/build/x86asm/scan.hpp"
#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

// What the parser asks of the modules around it.
struct ParseHost {
    // evaluate(stdscan, NULL, &tokval, opflags, critical, hints): evaluate the expression that starts at the current token
    // of `tokval`, reading further tokens from `scanner`. On return `tokval` holds the token that ended it. `opflags`
    // (may be null) receives OPFLAG_FORWARD/OPFLAG_EXTERN; `hints` (may be null) the base-register hints. nullopt on an
    // error (already reported).
    std::function<std::optional<Expr>(Scanner& scanner, TokenVal& tokval, int* opflags, int critical, EvalHints* hints)> evaluate;
    // define_label(name, segment, offset, is_norm).
    std::function<void(std::string_view name, std::int32_t segment, std::int64_t offset, bool is_norm)> define_label;
};

class Parser {
public:
    Parser(Assembly& assembly, Scanner& scanner, Floats& floats, ParseHost host)
        : as_(assembly), scan_(scanner), floats_(floats), host_(std::move(host)) {}

    // Parse `buffer` (which the caller keeps alive and unchanged until the Insn is no longer used) at bits mode `bits`.
    // Returns `result`; on an error or a blank line `result.opcode` is I_none.
    Insn& parse_line(std::string_view buffer, Insn& result, int bits);

private:
    // The tokens and operands of the line being parsed live in the member `tokval_`, as NASM's file static did.
    std::string tokstr(const TokenVal& tok) const;
    void process_size_override(Insn& result, Operand& op);
    bool parse_decorators(DecoFlags& decoflags);
    int parse_mref(Operand& op, const Expr& e);
    void mref_set_optype(Operand& op);
    int parse_eops(std::vector<ExtOp>& result, bool critical, int elem);
    bool end_expression_next();
    bool add_prefix(Insn& result);
    OpFlags set_imm_flags(Operand& op, int opt);

    Assembly& as_;
    Scanner& scan_;
    Floats& floats_;
    ParseHost host_;
    TokenVal tokval_;
    std::int64_t pass_last_seen_ = 0;   // the last pass the implicit-DEFAULT-ABS warning was given in
};

}  // namespace cb::build::x86asm
