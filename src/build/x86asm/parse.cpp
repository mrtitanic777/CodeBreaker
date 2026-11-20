// build/x86asm/parse.cpp -- the source line parser: NASM's asm/parser.c with CodeBreaker's MASM-mode additions.
//
// The structure and the order of the tests are NASM's (the encoder's choices depend on exactly which flags the parser sets
// on an operand). NASM's gotos (restart_parse, mref_more, fail) are kept as gotos where the flow is a state machine.
#include "build/x86asm/parse.hpp"

#include <cstring>

#include "build/x86asm/expr_util.hpp"
#include "build/x86asm/nctype.hpp"
#include "build/x86asm/strfunc.hpp"

namespace cb::build::x86asm {

namespace drvns_parse {

// Token types as plain ints, to compare with what the scanner returns (a character or a TokenType value).
constexpr int T_EOS = 0;
constexpr int T_ID = token_value(TokenType::ID);
constexpr int T_NUM = token_value(TokenType::NUM);
constexpr int T_STR = token_value(TokenType::STR);
constexpr int T_FLOAT = token_value(TokenType::FLOAT);
constexpr int T_INSN = token_value(TokenType::INSN);
constexpr int T_REG = token_value(TokenType::REG);
constexpr int T_SPECIAL = token_value(TokenType::SPECIAL);
constexpr int T_SIZE = token_value(TokenType::SIZE);
constexpr int T_PREFIX = token_value(TokenType::PREFIX);
constexpr int T_TIMES = token_value(TokenType::TIMES);
constexpr int T_STRFUNC = token_value(TokenType::STRFUNC);
constexpr int T_BRCCONST = token_value(TokenType::BRCCONST);
constexpr int T_OPMASK = token_value(TokenType::OPMASK);
constexpr int T_DECORATOR = token_value(TokenType::DECORATOR);
constexpr int T_MASM_PTR = token_value(TokenType::MASM_PTR);
constexpr int T_MASM_FLAT = token_value(TokenType::MASM_FLAT);
constexpr int T_QMARK = '?';

// Case-blind equality with a lower-case word (nasm_stricmp(text, word) == 0).
bool parse_ieq(std::string_view text, std::string_view word) {
    if (text.size() != word.size()) return false;
    for (std::size_t i = 0; i < text.size(); ++i)
        if (nasm_tolower(text[i]) != word[i]) return false;
    return true;
}

// The operand-size bits of a MASM data-type size in bytes (masm_size_bits).
OpFlags masm_size_bits(int size) {
    switch (size) {
    case 1: return opf::BITS8;
    case 2: return opf::BITS16;
    case 4: return opf::BITS32;
    case 8: return opf::BITS64;
    case 10: return opf::BITS80;
    case 16: return opf::BITS128;
    default: return 0;
    }
}

// The element size of a DB-type opcode, for MASM's data-label typing (masm_dataop_size).
int masm_dataop_size(Opcode opcode) {
    switch (opcode) {
    case I_DB: return 1;
    case I_DW: return 2;
    case I_DD: return 4;
    case I_DQ: return 8;
    case I_DT: return 10;
    case I_DO: return 16;
    default: return 0;
    }
}

// The x86 string instructions. MASM lets these be written with explicit "documentation" operands (e.g. `stosd dword ptr
// es:[edi], eax'); NASM's mnemonics take none (esi/edi/eax/dx are implicit). parse_line strips such operands under --masm.
bool masm_is_string_op(Opcode op) {
    switch (op) {
    case I_MOVSB: case I_MOVSW: case I_MOVSD: case I_MOVSQ:
    case I_STOSB: case I_STOSW: case I_STOSD: case I_STOSQ:
    case I_LODSB: case I_LODSW: case I_LODSD: case I_LODSQ:
    case I_SCASB: case I_SCASW: case I_SCASD: case I_SCASQ:
    case I_CMPSB: case I_CMPSW: case I_CMPSD: case I_CMPSQ:
    case I_INSB: case I_INSW: case I_INSD:
    case I_OUTSB: case I_OUTSW: case I_OUTSD:
        return true;
    default:
        return false;
    }
}

bool is_far_load(Opcode op) { return op == I_LDS || op == I_LES || op == I_LFS || op == I_LGS || op == I_LSS; }

void init_operand(Operand& op, unsigned opidx) {
    op = Operand{};
    op.basereg = -1;
    op.indexreg = -1;
    op.segment = NO_SEG;
    op.wrt = NO_SEG;
    op.opidx = static_cast<std::uint8_t>(opidx);
}

}  // namespace drvns_parse
using namespace drvns_parse;

// Human-readable description of a token, intended for error messages.
std::string Parser::tokstr(const TokenVal& tok) const {
    if (tok.type == TokenType::EOS) return "end of line";
    if (!tok.start.empty()) return "`" + std::string(tok.start) + "'";
    return "invalid token";
}

void Parser::process_size_override(Insn& result, Operand& op) {
    if (as_.tasm_compatible_mode) {
        switch (tokval_.integer) {
        // For TASM compatibility a size override inside the brackets changes the size of the operand, not the address
        // type of the operand as it does in standard NASM syntax. Hence `mov eax,[DWORD val]` is valid syntax in TASM
        // compatibility mode. Note that you lose the ability to override the default address type for the instruction,
        // but we never use anything but 32-bit flat model addressing in our code.
        case S_BYTE: op.type |= opf::BITS8; break;
        case S_WORD: op.type |= opf::BITS16; break;
        case S_DWORD:
        case S_LONG: op.type |= opf::BITS32; break;
        case S_QWORD: op.type |= opf::BITS64; break;
        case S_TWORD: op.type |= opf::BITS80; break;
        case S_OWORD: op.type |= opf::BITS128; break;
        default: as_.nonfatal("invalid operand size specification"); break;
        }
    } else {
        // Standard NASM compatible syntax.
        switch (tokval_.integer) {
        case S_NOSPLIT: op.eaflags |= EAF_TIMESTWO; break;
        case S_REL: op.eaflags |= EAF_REL; break;
        case S_ABS: op.eaflags |= EAF_ABS; break;
        case S_BYTE:
            op.disp_size = 8;
            op.eaflags |= EAF_BYTEOFFS;
            break;
        case P_A16:
        case P_A32:
        case P_A64:
            if (result.prefixes[PPS_ASIZE] && result.prefixes[PPS_ASIZE] != tokval_.integer)
                as_.nonfatal("conflicting address size specifications");
            else
                result.prefixes[PPS_ASIZE] = static_cast<int>(tokval_.integer);
            break;
        case S_WORD:
            op.disp_size = 16;
            op.eaflags |= EAF_WORDOFFS;
            break;
        case S_DWORD:
        case S_LONG:
            op.disp_size = 32;
            op.eaflags |= EAF_WORDOFFS;
            break;
        case S_QWORD:
            op.disp_size = 64;
            op.eaflags |= EAF_WORDOFFS;
            break;
        default: as_.nonfatal("invalid size specification in effective address"); break;
        }
    }
}

// Braced keywords are parsed here. Opmask and zeroing decorators can be placed in any order, e.g. zmm1 {k2}{z} or zmm2
// {z}{k3}; decorator(s) are placed at the end of an operand.
bool Parser::parse_decorators(DecoFlags& decoflags) {
    int i = token_value(tokval_.type);

    while (true) {
        if (i == T_OPMASK) {
            if (decoflags & deco::OPMASK_MASK) {
                as_.nonfatal("opmask k{} is already set", static_cast<std::uint64_t>(decoflags & deco::OPMASK_MASK));
                decoflags = static_cast<DecoFlags>(decoflags & ~deco::OPMASK_MASK);
            }
            decoflags = static_cast<DecoFlags>(decoflags | deco::val_opmask(static_cast<OpFlags>(kRegVals[static_cast<std::size_t>(tokval_.integer)])));
        } else if (i == T_DECORATOR) {
            const int j = static_cast<int>(tokval_.integer);
            switch (j) {
            case BRC_Z: decoflags = static_cast<DecoFlags>(decoflags | deco::Z_MASK); break;
            case BRC_1TO2:
            case BRC_1TO4:
            case BRC_1TO8:
            case BRC_1TO16:
            case BRC_1TO32:
                decoflags = static_cast<DecoFlags>(decoflags | deco::BRDCAST_MASK | deco::val_brnum(static_cast<OpFlags>(j - BRC_1TO2)));
                break;
            default: as_.nonfatal("{{{}}} is not an expected decorator", tokval_.text); break;
            }
        } else if (i == ',' || i == T_EOS) {
            return false;
        } else {
            as_.nonfatal("only a series of valid decorators expected");
            return true;
        }
        i = scan_.scan(tokval_);
    }
}

int Parser::parse_mref(Operand& op, const Expr& e) {
    int b = op.basereg;   // basereg
    int i = op.indexreg;  // indexreg
    int s = op.scale;     // scale
    std::int64_t o = op.offset;

    for (std::size_t n = 0; eu::type_at(e, n); ++n) {
        const ExprTerm& t = e[n];
        if (!t.value) continue;   // operand multiplied by zero

        if (t.type <= EXPR_REG_END) {
            const OpFlags flags = kRegFlags[static_cast<std::size_t>(t.type)];
            const bool is_gpr = is_class(opf::REG_CLASS_GPR, flags);

            if (is_gpr && t.value == 1 && b == -1) {
                b = t.type;   // it can be basereg
            } else if (i == -1) {
                i = t.type;   // must be index register
                s = static_cast<int>(t.value);
            } else {
                if (b == -1)
                    as_.nonfatal("invalid effective address: two index registers");
                else if (!is_gpr)
                    as_.nonfatal("invalid effective address: impossible register");
                else
                    as_.nonfatal("invalid effective address: too many registers");
                return -1;
            }
        } else if (t.type == EXPR_UNKNOWN) {
            op.opflags |= OPFLAG_UNKNOWN;
        } else if (t.type == EXPR_SIMPLE) {
            o += t.value;
        } else if (t.type == EXPR_WRT) {
            op.wrt = static_cast<std::int32_t>(t.value);
        } else if (t.type >= EXPR_SEGBASE) {
            if (t.value == 1) {
                if (op.segment != NO_SEG) {
                    as_.nonfatal("invalid effective address: multiple base segments");
                    return -1;
                }
                op.segment = t.type - EXPR_SEGBASE;
            } else if (t.value == -1 && t.type == as_.location.segment + EXPR_SEGBASE && !(op.opflags & OPFLAG_RELATIVE)) {
                op.opflags |= OPFLAG_RELATIVE;
            } else {
                as_.nonfatal("invalid effective address: impossible segment base multiplier");
                return -1;
            }
        } else {
            as_.nonfatal("invalid effective address: bad subexpression type");
            return -1;
        }
    }

    op.basereg = b;
    op.indexreg = i;
    op.scale = s;
    op.offset = o;
    return 0;
}

void Parser::mref_set_optype(Operand& op) {
    const int b = op.basereg;
    const int i = op.indexreg;
    const int s = op.scale;

    // It is memory, but it can match any r/m operand.
    op.type |= opf::MEMORY_ANY;

    if (!(i == -1 || s > 0)) as_.assert_failed("i == -1 || s > 0");

    if (!(op.eaflags & (EAF_FS | EAF_GS))) op.eaflags |= EAF_NOTFSGS;

    if (b != -1) {
        const OpFlags bclass = kRegFlags[static_cast<std::size_t>(b)];
        op.type &= bclass | ~opf::RN_L16;
    } else if (i == -1) {
        OpFlags flag = opf::MEM_OFFS;
        if (as_.globl.bits == 64) {
            if (op.eaflags & EAF_ABS) {
                // Do nothing.
            } else if (op.eaflags & EAF_REL) {
                flag = opf::IP_REL;
            } else {
                if (as_.globl.rel & op.eaflags) flag = opf::IP_REL;
                if (!(as_.globl.reldef & op.eaflags)) {
                    if (as_.pass_count() != pass_last_seen_) {
                        as_.warn(WARN_IMPLICIT_ABS_DEPRECATED, "implicit DEFAULT ABS is deprecated");
                        pass_last_seen_ = as_.pass_count();
                    }
                }
            }
        }
        op.type |= flag;
    }

    if (i != -1) {
        const OpFlags iclass = kRegFlags[static_cast<std::size_t>(i)];
        op.type &= iclass | ~opf::RN_L16;

        if (is_class(opf::XMMREG, iclass))
            op.type |= opf::XMEM;
        else if (is_class(opf::YMMREG, iclass))
            op.type |= opf::YMEM;
        else if (is_class(opf::ZMMREG, iclass))
            op.type |= opf::ZMEM;
    }

    const OpFlags size = op.type & SIZE_MASK;
    if (!size || size == opf::BITS16) op.type |= opf::RM_SEL;
}

bool Parser::end_expression_next() {
    const ScanState save = scan_.get();
    TokenVal tv;
    const int i = scan_.scan(tv);
    scan_.set(save);
    return i == ',' || i == ';' || i == ')' || !i;
}

// Parse an extended expression, used by db et al. "elem" is the element size; initially comes from the specific opcode
// (e.g. db == 1) but can be overridden. Returns the number of operands, or -1.
int Parser::parse_eops(std::vector<ExtOp>& result, bool critical, int elem) {
    result.clear();
    int oper_num = 0;
    bool do_subexpr = false;
    ExtOp eop;
    bool have_eop = false;

    // End of string is obvious; ) ends a sub-expression list e.g. DUP.
    for (int i = token_value(tokval_.type); i != T_EOS; i = scan_.scan(tokval_)) {
        char endparen = ')';   // is a right paren the end of list?

        if (i == ')') break;

        if (!have_eop) {
            eop = ExtOp{};
            eop.dup = 1;
            eop.elem = elem;
            have_eop = true;
            do_subexpr = false;
        }
        int sign = +1;
        bool skip = false;
        // How a number or float is to be read after the keyword tests below.
        enum class Kind { Done, Float, Expression } kind = Kind::Done;

        // MASM (--masm): `dw OFFSET label' / `dd OFFSET label' -- OFFSET is a no-op in a data initializer (a bare label is
        // already its address), so skip a leading OFFSET keyword and let the label parse as the value. The regular operand
        // parser handles OFFSET separately.
        if (as_.masm_mode && i == T_ID && parse_ieq(tokval_.text, "offset")) i = scan_.scan(tokval_);

        if (i == T_QMARK) {
            eop.type = EOT_DB_RESERVE;
            skip = true;
        } else if (do_subexpr && i == '(') {
            scan_.scan(tokval_);   // skip paren
            std::vector<ExtOp> sub;
            if (parse_eops(sub, critical, eop.elem) < 0) return -1;

            if (sub.empty()) {
                // Subexpression is empty.
                eop.type = EOT_NOTHING;
            } else if (sub.size() == 1) {
                // Subexpression is a single element, flatten.
                ExtOp single = std::move(sub.front());
                if (!single.elem) single.elem = eop.elem;
                single.dup *= eop.dup;
                eop = std::move(single);
            } else {
                eop.subexpr = std::move(sub);
                eop.type = EOT_EXTOP;
            }

            // We should have ended on a closing paren.
            if (token_value(tokval_.type) != 0x29) {
                as_.nonfatal("expected `)' after subexpression, got {}", tokstr(tokval_));
                return -1;
            }
            endparen = 0;   // this time the paren is not the end
            skip = true;
        } else if (i == '%') {
            // %(expression_list)
            do_subexpr = true;
            continue;
        } else if (i == T_SIZE) {
            // Element size override.
            eop.elem = static_cast<int>(tokval_.inttwo);
            do_subexpr = true;
            continue;
        } else if (i == T_STR && end_expression_next()) {
            // end_expression_next() is to distinguish this from a string used as part of an expression...
            eop.type = EOT_DB_STRING;
            eop.string = tokval_.text;
            skip = true;
        } else if (i == T_STRFUNC) {
            bool parens = false;
            const std::string funcname = tokval_.text;
            const StrFunc func = static_cast<StrFunc>(tokval_.integer);

            i = scan_.scan(tokval_);
            if (i == '(') {
                parens = true;
                endparen = 0;
                i = scan_.scan(tokval_);
            }
            if (i != T_STR) {
                as_.nonfatal("{} must be followed by a string constant, got {}", funcname, tokstr(tokval_));
                eop.type = EOT_NOTHING;
            } else {
                eop.type = EOT_DB_STRING_FREE;
                if (auto transformed = string_transform(tokval_.text, func)) {
                    eop.string = std::move(*transformed);
                } else {
                    as_.nonfatal("invalid input string to {}", funcname);
                    eop.type = EOT_NOTHING;
                }
            }
            if (parens && i && i != ')') {
                i = scan_.scan(tokval_);
                if (i != ')') as_.nonfatal("unterminated {} function", funcname);
            }
            skip = i != ',';
        } else if (i == '-' || i == '+') {
            const ScanState save = scan_.get();
            TokenVal tmptok;

            sign = (i == '-') ? -1 : 1;
            if (scan_.scan(tmptok) != T_FLOAT) {
                scan_.set(save);
                kind = Kind::Expression;
            } else {
                tokval_ = tmptok;
                kind = Kind::Float;
            }
        } else if (i == T_FLOAT) {
            kind = Kind::Float;
        } else {
            kind = Kind::Expression;   // anything else, assume it is an expression
        }

        if (kind == Kind::Float) {
            eop.type = EOT_DB_FLOAT;

            const Floatize fmt = Floats::float_deffmt(eop.elem);
            std::size_t len;
            if (fmt == FLOAT_ERR) {
                as_.nonfatal("no {}-bit floating-point format supported", eop.elem << 3);
                len = 0;
            } else if (eop.elem < 1) {
                // fix suggested by Pedro Gimeno... original line was: eop->type = EOT_NOTHING;
                as_.nonfatal("floating-point constant encountered in unknown instruction");
                len = 0;
            } else {
                len = static_cast<std::size_t>(eop.elem);
                FloatBytes bytes{};
                if (floats_.float_const(tokval_.text, sign, bytes, fmt))
                    eop.string.assign(reinterpret_cast<const char*>(bytes.data()), len);
                else
                    len = 0;
            }
            if (!len) {
                eop.string.clear();
                eop.type = EOT_NOTHING;
            }
            skip = true;
        } else if (kind == Kind::Expression) {
            std::optional<Expr> value = host_.evaluate(scan_, tokval_, nullptr, critical ? 1 : 0, nullptr);
            i = token_value(tokval_.type);
            if (!value) return -1;   // error in evaluator
            if (tokval_.flag & TFLAG_DUP) {
                // Expression followed by DUP.
                if (!eu::is_simple(*value)) {
                    as_.nonfatal("non-constant argument supplied to DUP");
                    return -1;
                } else if (eu::value_at(*value, 0) < 0) {
                    as_.nonfatal("negative argument supplied to DUP");
                    return -1;
                }
                eop.dup *= static_cast<std::size_t>(eu::value_at(*value, 0));
                do_subexpr = true;
                continue;
            }

            // value_to_extop: convert an expression vector returned from evaluate() into an extop.
            eop.type = EOT_DB_NUMBER;
            eop.offset = 0;
            eop.segment = eop.wrt = NO_SEG;
            eop.relative = false;
            bool bad = false;
            for (std::size_t n = 0; eu::type_at(*value, n); ++n) {
                const ExprTerm& t = (*value)[n];
                if (!t.value) continue;                   // zero term, safe to ignore
                if (t.type <= EXPR_REG_END) { bad = true; break; }   // false if a register is present
                if (t.type == EXPR_UNKNOWN) break;                   // something we can't resolve yet
                if (t.type == EXPR_SIMPLE) {
                    eop.offset += t.value;                // simple number expression
                    continue;
                }
                if (eop.wrt == NO_SEG && !eop.relative && t.type == EXPR_WRT) {
                    eop.wrt = static_cast<std::int32_t>(t.value);   // WRT term
                    continue;
                }
                if (!eop.relative && t.type == EXPR_SEGBASE + as_.location.segment && t.value == -1) {
                    eop.relative = true;                  // expression of the form: foo - $
                    continue;
                }
                if (eop.segment == NO_SEG && t.type >= EXPR_SEGBASE && t.value == 1) {
                    eop.segment = t.type - EXPR_SEGBASE;
                    continue;
                }
                bad = true;   // otherwise, badness
                break;
            }
            if (bad) as_.nonfatal("expression is not simple or relocatable");
            skip = false;
        }

        if (eop.dup == 0 || eop.type == EOT_NOTHING) {
            // dropped
        } else if (eop.type == EOT_DB_RESERVE && !result.empty() && result.back().type == EOT_DB_RESERVE && result.back().elem == eop.elem) {
            // Coalesce multiple EOT_DB_RESERVE.
            result.back().dup += eop.dup;
        } else {
            // Add this eop to the end of the chain.
            result.push_back(std::move(eop));
        }

        oper_num++;
        have_eop = false;   // done with this operand

        if (skip) {
            // Consume the (last) token if that didn't happen yet.
            i = scan_.scan(tokval_);
        }

        // We're about to call stdscan(), which will eat the comma that we're currently sitting on between arguments.
        // However, we'd better check first that it _is_ a comma.
        if (i == T_EOS || i == endparen) break;   // already at end?
        if (i != ',') {
            as_.nonfatal("comma expected after operand, got {}", tokstr(tokval_));
            return -1;
        }
    }

    return oper_num;
}

// Return true if not a prefix token.
bool Parser::add_prefix(Insn& result) {
    int slot;

    switch (token_value(tokval_.type)) {
    case T_SPECIAL:
        if (tokval_.integer == S_STRICT) {
            result.opt |= OPTIM_STRICT_INSTR;
            return true;
        }
        return false;
    case T_PREFIX:
        slot = static_cast<int>(tokval_.inttwo);
        break;
    case T_REG:
        slot = PPS_SEG;
        if (!is_sreg(static_cast<int>(tokval_.integer))) return false;
        break;
    default:
        return false;
    }

    if (result.prefixes[static_cast<std::size_t>(slot)]) {
        if (result.prefixes[static_cast<std::size_t>(slot)] == tokval_.integer)
            as_.warn(WARN_OTHER, "instruction has redundant prefixes");
        else
            as_.nonfatal("instruction has conflicting prefixes");
    }
    result.prefixes[static_cast<std::size_t>(slot)] = static_cast<int>(tokval_.integer);

    return true;
}

// Set value-specific immediate flags.
OpFlags Parser::set_imm_flags(Operand& op, int opt) {
    const bool strict = (op.type & opf::STRICT_) || (opt & OPTIM_STRICT_OPER);
    const std::int64_t n = op.offset;

    if (!(op.type & opf::IMMEDIATE)) return op.type;

    if (op.opflags & OPFLAG_UNKNOWN) {
        // Be optimistic in pass 1.
        if (!strict || !(op.type & SIZE_MASK)) op.type |= opf::UNITY | opf::FOURBITS;
        if (!strict) op.type |= opf::SBYTEDWORD | opf::SBYTEWORD | opf::UDWORD | opf::SDWORD;
        op.type |= opf::IMM_KNOWN;   // unknowable in pass 1
        return op.type;
    }

    if (!(op.opflags & OPFLAG_SIMPLE)) return op.type;

    op.type |= opf::IMM_KNOWN;

    if (!strict || !(op.type & SIZE_MASK)) {
        if (n == 1) op.type |= opf::UNITY;

        // Allow FOURBITS matching for negative values, so things like ~0 work.
        if (n >= -16 && n <= 15) op.type |= opf::FOURBITS;
    }

    if (strict) return op.type;

    if (static_cast<std::int32_t>(n) == static_cast<std::int8_t>(n)) op.type |= opf::SBYTEDWORD;
    if (static_cast<std::int16_t>(n) == static_cast<std::int8_t>(n)) op.type |= opf::SBYTEWORD;
    if (static_cast<std::uint64_t>(n) == static_cast<std::uint32_t>(n)) op.type |= opf::UDWORD;
    if (static_cast<std::int64_t>(n) == static_cast<std::int32_t>(n)) op.type |= opf::SDWORD;

    return op.type;
}

Insn& Parser::parse_line(std::string_view buffer, Insn& result, int bits) {
    bool insn_is_label = false;
    EvalHints hints;
    int opnum;
    bool critical;
    bool first;
    bool colonless_label;
    bool recover;
    bool far_jmp_ok;
    bool have_prefixes;
    int i;

restart_parse:
    first = true;
    colonless_label = false;

    scan_.reset(buffer);
    i = scan_.scan(tokval_);

    result = Insn{};
    result.times = 1;            // no TIMES either yet
    result.opcode = I_none;      // no opcode
    result.loc = as_.location;   // current assembly position
    result.bits = static_cast<std::uint8_t>(bits);   // current assembly mode
    result.opt = as_.optimizing;   // optimization flags

    // Ignore blank lines.
    if (i == T_EOS) goto fail;

    if (i == T_ID || (insn_is_label && i == T_INSN)) {
        // there's a label here
        const TokenVal label = tokval_;
        first = false;
        result.label = tokval_.text;
        i = scan_.scan(tokval_);
        colonless_label = i != ':';
        if (i == ':') {   // skip over the optional colon
            i = scan_.scan(tokval_);
        } else if (i == 0) {
            as_.warn(WARN_LABEL_ORPHAN, "label `{}' alone on a line without a colon might be in error", label.start);
        }
        if (i != T_INSN || tokval_.integer != I_EQU) {
            // FIXME (NASM): location.segment could be NO_SEG, in which case it is possible we should be passing
            // 'absolute.segment'. Look into this. Work out whether that is *really* what we should be doing. Generally fix
            // things. I think this is right as it is, but am still not certain.
            host_.define_label(result.label, as_.in_absolute ? as_.absolute.segment : as_.location.segment, as_.location.offset, true);
        }
    }

    have_prefixes = false;

    // Process things that go before the opcode.
    while (i) {
        if (i == T_TIMES) {
            // TIMES is a very special prefix.
            i = scan_.scan(tokval_);
            std::optional<Expr> value = host_.evaluate(scan_, tokval_, nullptr, as_.pass_stable() ? 1 : 0, nullptr);
            i = token_value(tokval_.type);
            if (!value) goto fail;   // error in evaluator
            if (!eu::is_simple(*value)) {
                as_.nonfatal("non-constant argument supplied to TIMES");
                result.times = 1;
            } else {
                result.times = static_cast<std::int32_t>(eu::value_at(*value, 0));
                // negative values handled in assemble.c: process_insn()
            }
        } else {
            if (!add_prefix(result)) break;
            have_prefixes = true;
            i = scan_.scan(tokval_);
        }

        first = false;
    }

    if (i != T_INSN) {
        if (!i) {
            if (have_prefixes) {
                // Instruction prefixes are present, but no actual instruction. This is allowed: at this point we invent a
                // notional instruction of RESB 0. Note that this can be combined with TIMES, so do not clear *result!
                result.opcode = I_RESB;
                result.operands = 1;
                result.oprs[0].type = opf::IMM_NORMAL;
                result.oprs[0].opflags = OPFLAG_SIMPLE;
                result.oprs[0].offset = 0;
                result.oprs[0].segment = result.oprs[0].wrt = NO_SEG;
                set_imm_flags(result.oprs[0], result.opt);
            }
        } else if (!first) {
            // What was meant to be an instruction may very well have been mistaken for a label here, so print out both,
            // unless it is unambiguous.
            as_.nonfatal("instruction expected, found `{}{}{}'", colonless_label ? result.label : std::string(), colonless_label ? " " : "",
                         tokval_.start);
        } else if (result.label.empty()) {
            as_.nonfatal("label, instruction or prefix expected at start of line, found `{}'", tokval_.start);
        }
        return result;
    }

    result.opcode = static_cast<Opcode>(tokval_.integer);

    // INCBIN cannot be satisfied with incorrectly evaluated operands, since the correct values _must_ be known on the
    // first pass. Hence, even in pass one, we set the `critical' flag on calling evaluate(), so that it will bomb out on
    // undefined symbols.
    critical = as_.pass_final() || result.opcode == I_INCBIN;

    if (opcode_is_db(result.opcode) || result.opcode == I_INCBIN) {
        i = scan_.scan(tokval_);

        if (first && i == ':') {
            // Really a label.
            insn_is_label = true;
            goto restart_parse;
        }
        first = false;
        const int oper_num = parse_eops(result.eops, critical, db_bytes(result.opcode));
        if (oper_num < 0) goto fail;

        if (result.opcode == I_INCBIN) {
            // Correct syntax for INCBIN is that there should be one string operand, followed by one or two numeric
            // operands.
            const auto& e = result.eops;
            if (e.empty() || e[0].type != EOT_DB_STRING)
                as_.nonfatal("`incbin' expects a file name");
            else if (e.size() > 1 && e[1].type != EOT_DB_NUMBER)
                as_.nonfatal("`incbin': second parameter is non-numeric");
            else if (e.size() > 2 && e[2].type != EOT_DB_NUMBER)
                as_.nonfatal("`incbin': third parameter is non-numeric");
            else if (e.size() > 3)
                as_.nonfatal("`incbin': more than three parameters");
            else
                return result;
            // If we reach here, one of the above errors happened. Throw the instruction away.
            goto fail;
        } else {
            // DB et al.
            result.operands = static_cast<std::uint8_t>(oper_num);
            if (oper_num == 0) as_.warn(WARN_DB_EMPTY, "no operand for data declaration");
            // MASM: a labelled data declaration types the label with its element size, so a later bare reference to it
            // means its contents.
            if (as_.masm_mode && !result.label.empty()) {
                const int dsz = masm_dataop_size(result.opcode);
                if (dsz) as_.masm_type_note(result.label, dsz);
            }
        }
        return result;
    }

    // Now we begin to parse the operands. There may be up to MAX_OPERANDS of these, separated by commas, and terminated by
    // a zero token.
    far_jmp_ok = result.opcode == I_JMP || result.opcode == I_CALL;

    // Initialize operand structures.
    for (opnum = 0; opnum < MAX_OPERANDS; opnum++) init_operand(result.oprs[static_cast<std::size_t>(opnum)], static_cast<unsigned>(opnum));

    for (opnum = 0; opnum < MAX_OPERANDS; opnum++) {
        Operand* op = &result.oprs[static_cast<std::size_t>(opnum)];
        std::optional<Expr> value;       // used most of the time
        bool mref = false;               // is this going to be a memory ref?
        int bracket = 0;                 // is it a [] mref, or a "naked" mref?
        bool mib;                        // compound (mib) mref?
        int setsize = 0;
        DecoFlags brace_flags = 0;       // flags for decorators in braces
        // A far-pointer load (LDS/LES/LFS/LGS/LSS) sizes its memory operand by the destination register, not by an
        // explicit `dword ptr'/`fword ptr' cast: MASM writes `les bx, dword ptr [m]', which NASM rejects as an
        // operand-size mismatch. Under --masm, swallow such a size on these opcodes so the load is left unsized (see also
        // the bare-label case).
        const bool masm_farload = as_.masm_mode && is_far_load(result.opcode);

        i = scan_.scan(tokval_);
        if (first && i == ':') {
            insn_is_label = true;
            goto restart_parse;
        }

        first = false;
        if (opnum == 0) {
            // Allow braced prefix tokens like {evex} after the opcode mnemonic proper, but before the first operand. This
            // is currently not allowed for non-braced prefix tokens.
            while ((tokval_.flag & TFLAG_BRC) && add_prefix(result)) i = scan_.scan(tokval_);
        }

        if (i == T_EOS) break;

        op->type = 0;   // so far, no override

        // Naked special immediate token. Terminates the expression without requiring a post-comma.
        if (i == T_BRCCONST) {
            op->type = opf::IMMEDIATE;   // but not IMM_NORMAL!
            op->opflags = OPFLAG_SIMPLE;
            op->offset = tokval_.integer;
            op->segment = NO_SEG;
            op->wrt = NO_SEG;
            op->iflag = static_cast<int>(tokval_.inttwo);
            set_imm_flags(*op, result.opt);
            i = scan_.scan(tokval_);
            if (i != ',') scan_.pushback(tokval_);
            continue;   // next operand
        }

        // size specifiers
        while (i == T_SPECIAL || i == T_SIZE) {
            switch (tokval_.integer) {
            case S_BYTE:
                if (!setsize) {   // we want to use only the first
                    result.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX;
                    op->type |= opf::BITS8;
                }
                setsize = 1;
                break;
            case S_WORD:
                if (!setsize) op->type |= opf::BITS16;
                setsize = 1;
                break;
            case S_DWORD:
            case S_LONG:
                if (!setsize && !masm_farload) op->type |= opf::BITS32;
                setsize = 1;
                break;
            case S_QWORD:
                if (!setsize) op->type |= opf::BITS64;
                setsize = 1;
                break;
            case S_TWORD:
                if (!setsize) op->type |= opf::BITS80;
                setsize = 1;
                break;
            case S_OWORD:
                if (!setsize) op->type |= opf::BITS128;
                setsize = 1;
                break;
            case S_YWORD:
                if (!setsize) op->type |= opf::BITS256;
                setsize = 1;
                break;
            case S_ZWORD:
                if (!setsize) op->type |= opf::BITS512;
                setsize = 1;
                break;
            case S_TO: op->type |= opf::TO; break;
            case S_STRICT: op->type |= opf::STRICT_; break;
            case S_FAR: op->type |= opf::FAR_; break;
            case S_NEAR:
                // This is not legacy behavior, even if it perhaps should be.
                result.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX;
                op->type |= opf::NEAR_;
                break;
            case S_SHORT:
                result.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX;
                op->type |= opf::SHORT;
                break;
            case S_ABS: op->type |= opf::ABS_; break;
            default: as_.nonfatal("invalid operand size specification");
            }
            i = scan_.scan(tokval_);
        }

        // MASM data-label semantics (--masm): a bare data label used as an operand means its CONTENTS -- a memory
        // reference [label] sized by the label's declared type. `OFFSET label' keeps NASM's bare-label (address) semantics.
        if (as_.masm_mode && !mref && i == T_ID) {
            if (parse_ieq(tokval_.text, "offset")) {
                i = scan_.scan(tokval_);   // consume OFFSET; label stays an address
            } else {
                const int msz = as_.masm_type_query(tokval_.text);
                if (msz) {
                    mref = true;   // -> [label]
                    // LES/LDS/LFS/LGS/LSS load a far pointer whose operand size follows the destination register (16/32),
                    // not the pointer's byte width, so a `dword'/`fword' size from the label's type conflicts: `lds dx,
                    // myptr' would become `lds dx, dword [myptr]' = invalid operand sizes. Leave the far-pointer load
                    // unsized and let the register decide.
                    const bool farload = is_far_load(result.opcode);
                    if (!setsize && !farload) op->type |= masm_size_bits(msz);
                }
            }
        }

        if (i == '[' || i == '&') {
            // memory reference
            mref = true;
            bracket += (i == '[');
            i = scan_.scan(tokval_);
        } else if (i == T_MASM_PTR) {
            // `<size> PTR <operand>'. A following `[' or a bare data label is a (sized) memory reference; but MASM also
            // writes `WORD PTR <const>' for a sized IMMEDIATE (`mov [m], WORD PTR la_freefixedsize', where the constant
            // already carries the operation size). So under --masm a number or a non-data-label identifier after PTR stays
            // an immediate (mref false); everything else is memory as before.
            i = scan_.scan(tokval_);
            if (as_.masm_mode && (i == T_NUM || (i == T_ID && as_.masm_type_query(tokval_.text) == 0))) {
                // sized immediate: leave mref false, i holds the constant
            } else {
                mref = true;   // mref_more handles `[', size overrides, etc.
            }
        }

    mref_more:
        if (mref) {
            bool done = false;
            bool nofw = false;

            while (!done) {
                if (i == T_SPECIAL || i == T_SIZE || i == T_PREFIX) {
                    process_size_override(result, *op);
                } else if (i == '[') {
                    bracket++;
                } else if (i == ',') {
                    scan_.pushback(tokval_);   // rewind the comma
                    tokval_.type = TokenType::NUM;
                    tokval_.integer = 0;
                    done = nofw = true;
                } else if (i == T_MASM_FLAT) {
                    i = scan_.scan(tokval_);
                    if (i != ':') {
                        as_.nonfatal("unknown use of FLAT in MASM emulation");
                        nofw = true;
                    }
                    done = true;
                } else {
                    done = nofw = true;
                }

                if (!nofw) i = scan_.scan(tokval_);
            }
        }

        value = host_.evaluate(scan_, tokval_, &op->opflags, critical ? 1 : 0, &hints);
        i = token_value(tokval_.type);
        if (!value) goto fail;   // error in evaluator

        if (i == '[' && !bracket) {
            // displacement[regs] syntax
            mref = true;
            parse_mref(*op, *value);   // process what we have so far
            goto mref_more;
        }

        if (i == ':') {
            const bool ok_reg = is_register(eu::type_at(*value, 0)) && eu::value_at(*value, 0) == 1 && !eu::type_at(*value, 1);

            if (!mref && ok_reg && !is_sreg(eu::type_at(*value, 0))) {
                // Register pair syntax; this terminates the expression as if it had ended in a comma, but sets the COLON
                // flag on the operand further down.
            } else if (mref || (ok_reg && is_sreg(eu::type_at(*value, 0)))) {
                // segment override?
                mref = true;

                // Process the segment override.
                if (!ok_reg || !is_sreg(eu::type_at(*value, 0))) {
                    as_.nonfatal("invalid segment override");
                } else if (result.prefixes[PPS_SEG]) {
                    as_.nonfatal("instruction has conflicting segment overrides");
                } else {
                    result.prefixes[PPS_SEG] = (*value)[0].type;
                    switch ((*value)[0].type) {
                    case R_FS: op->eaflags |= EAF_FS; break;
                    case R_GS: op->eaflags |= EAF_GS; break;
                    default: break;
                    }
                }

                i = scan_.scan(tokval_);   // then skip the colon
                goto mref_more;
            }
        }

        mib = false;
        if (mref && bracket && i == ',') {
            // [seg:base+offset,index*scale] syntax (mib)
            Operand o2;   // index operand

            if (parse_mref(*op, *value)) goto fail;

            i = scan_.scan(tokval_);   // eat comma
            value = host_.evaluate(scan_, tokval_, &op->opflags, critical ? 1 : 0, &hints);
            i = token_value(tokval_.type);
            if (!value) goto fail;

            init_operand(o2, 0);
            if (parse_mref(o2, *value)) goto fail;
            if (o2.basereg != -1 && o2.indexreg == -1) {
                o2.indexreg = o2.basereg;
                o2.scale = 1;
                o2.basereg = -1;
            }

            if (op->indexreg != -1 || o2.basereg != -1 || o2.offset != 0 || o2.segment != NO_SEG || o2.wrt != NO_SEG) {
                as_.nonfatal("invalid mib expression");
                goto fail;
            }

            op->indexreg = o2.indexreg;
            op->scale = o2.scale;

            if (op->basereg != -1) {
                op->hintbase = op->basereg;
                op->hinttype = EAH_MAKEBASE;
            } else if (op->indexreg != -1) {
                op->hintbase = op->indexreg;
                op->hinttype = EAH_NOTBASE;
            } else {
                op->hintbase = -1;
                op->hinttype = EAH_NOHINT;
            }

            mib = true;
        }

        recover = false;
        if (mref) {
            // MASM (--masm): a param/local bound to a memory operand `[bp-o]' may itself be bracketed in the source (`mov
            // bx,[wLen]'), which expands to `[[bp-o]]'. Collapse the redundant inner bracket(s) so the whole thing parses as
            // one mref; a fold operator after an inner close (`[nBytes+2]' -> `[[bp-o]+2]') re-enters the mref
            // accumulator, exactly as the trailing-`[idx]' fold does below.
            if (as_.masm_mode) {
                while (bracket > 1 && i == ']') {
                    bracket--;
                    i = scan_.scan(tokval_);
                    if (i == '[' || i == '+' || i == '-') {
                        if (parse_mref(*op, *value)) goto fail;
                        goto mref_more;
                    }
                }
            }
            if (bracket == 1) {
                if (i == ']') {
                    bracket--;
                    i = scan_.scan(tokval_);
                    // MASM (--masm) folds a trailing `[idx]' or `+/- disp' into the SAME memory operand: a param/local
                    // expands to a bound `[bp-o]', so `Limit[2]' is `[bp-o][2]' and `Limit+2' is `[bp-o]+2', both meaning
                    // `[bp-o+2]'. parse_mref seeds from op's current base/index/offset and accumulates, so fold the
                    // expression parsed so far and re-enter to parse the rest.
                    if (as_.masm_mode && (i == '[' || i == '+' || i == '-')) {
                        if (parse_mref(*op, *value)) goto fail;
                        goto mref_more;
                    }
                } else {
                    as_.nonfatal("expecting ] at end of memory operand");
                    recover = true;
                }
            } else if (bracket == 0) {
                // Do nothing.
            } else if (bracket > 0) {
                as_.nonfatal("excess brackets in memory operand");
                recover = true;
            } else if (bracket < 0) {
                as_.nonfatal("unmatched ] in memory operand");
                recover = true;
            }

            if (i == T_DECORATOR || i == T_OPMASK) {
                // parse opmask (and zeroing) after an operand
                recover = parse_decorators(brace_flags);
                i = token_value(tokval_.type);
            }
            if (!recover && i != 0 && i != ',') {
                as_.nonfatal("comma, decorator or end of line expected, got `{}'", tokval_.start);
                recover = true;
            }
        } else {   // immediate operand
            if (i != 0 && i != ',' && i != ':' && i != T_DECORATOR && i != T_OPMASK) {
                as_.nonfatal("comma, colon, decorator or end of line expected after operand");
                recover = true;
            } else if (i == ':') {
                op->type |= opf::COLON;
            } else if (i == T_DECORATOR || i == T_OPMASK) {
                // parse opmask (and zeroing) after an operand
                recover = parse_decorators(brace_flags);
            }
        }
        if (recover) {
            do {   // error recovery
                i = scan_.scan(tokval_);
            } while (i != 0 && i != ',');
        }

        // now convert the exprs returned from evaluate() into operand descriptions...
        op->decoflags = static_cast<DecoFlags>(op->decoflags | brace_flags);

        if (mref) {   // it's a memory reference
            // A mib reference was fully parsed already.
            if (!mib) {
                if (parse_mref(*op, *value)) goto fail;
                op->hintbase = static_cast<int>(hints.base);
                op->hinttype = static_cast<EvalHint>(hints.type);
            }
            mref_set_optype(*op);
            // MASM (--masm): an explicit `ds:' override on a memory operand whose default segment is already DS is
            // redundant, and MASM (like every mainstream x86 assembler but NASM) emits no prefix byte for it. NASM would
            // emit a 3Eh prefix, which both diverges from MASM's bytes and, accumulated over a run of such operands,
            // pushes `short' jumps out of rel8 range. Drop it -- but only when the default segment is really DS: a stack
            // register in the addressing mode (BP/SP/EBP/ESP) makes the default SS, so the `ds:' is meaningful and must
            // stay. ESP is only ever a base, but NASM may hold it as the index pending the SIB base/index swap, so check
            // both slots.
            if (as_.masm_mode && result.prefixes[PPS_SEG] == R_DS) {
                const int b = op->basereg, x = op->indexreg;
                if (b != R_BP && b != R_SP && b != R_EBP && b != R_ESP && x != R_BP && x != R_SP && x != R_EBP && x != R_ESP)
                    result.prefixes[PPS_SEG] = 0;
            }
        } else if ((op->type & opf::FAR_) && !far_jmp_ok) {
            as_.nonfatal("invalid use of FAR operand specifier");
            recover = true;
        } else {   // it's not a memory reference
            const int eclass = eu::expr_class(*value, as_.location.segment);

            if (!(eclass & ~(EC_RELOC | EC_UNKNOWN))) {
                // It is an immediate.
                bool size_was_specified = false;
                op->offset = eu::reloc_value(*value);
                op->segment = eu::reloc_seg(*value);
                op->wrt = eu::reloc_wrt(*value);
                if (eclass & EC_SELFREL) op->opflags |= OPFLAG_RELATIVE;
                if (!(eclass & ~EC_SIMPLE)) op->opflags |= OPFLAG_SIMPLE;
                if (eclass & EC_UNKNOWN) op->opflags |= OPFLAG_UNKNOWN;

                op->type |= opf::IMM_NORMAL;
                set_imm_flags(*op, result.opt);

                // Catch a missing size specifier when dealing with a label - which might result in non-intentional
                // handling of only part of the label's address instead of the whole value.
                for (int j = 0; j <= opnum; j++) size_was_specified |= !!(result.oprs[static_cast<std::size_t>(j)].xsize & SIZE_MASK);

                // Raise a 'label operation missing size specifier' error when: 1. No size specifier was mentioned up to
                // this point. 2. One of the operands is a memory reference. 3. The current operand is a label and not a
                // simple immediate.
                if (!size_was_specified && (op->opflags & OPFLAG_FORWARD)) {
                    for (int j = 0; j <= opnum; j++) {
                        if ((result.oprs[static_cast<std::size_t>(j)].type & OPTYPE_MASK) == opf::MEMORY)
                            as_.fatal("size wasn't specified, some data may be omitted");
                    }
                }

                // Special hack: if the previous operand was a colon immediate operand with an explicit size, and this one
                // does not have an explicit size, move the size specifier to this operand. This handles the case: "jmp
                // dword foo:bar" (really being "jmp foo:dword bar".)
                if (opnum > 0 && is_class(opf::IMM_NORMAL | opf::COLON, result.oprs[static_cast<std::size_t>(opnum) - 1].type)) {
                    Operand& prev = result.oprs[static_cast<std::size_t>(opnum) - 1];
                    const OpFlags nsize = op->type & SIZE_MASK;
                    const OpFlags osize = prev.type & SIZE_MASK;
                    if (osize && !nsize) {
                        op->type ^= osize;
                        prev.type ^= osize;
                    }
                }
            } else if (eu::type_at(*value, 0) == EXPR_RDSAE) {
                // it's not an operand but a rounding or SAE decorator. put the decorator information in the (opflag_t) type
                // field of previous operand.
                opnum--;
                op--;
                switch ((*value)[0].value) {
                case BRC_RN:
                case BRC_RU:
                case BRC_RD:
                case BRC_RZ:
                case BRC_SAE:
                    op->decoflags = static_cast<DecoFlags>(op->decoflags | ((*value)[0].value == BRC_SAE ? deco::SAE : deco::ER));
                    result.evex_rm = static_cast<int>((*value)[0].value);
                    break;
                default: as_.nonfatal("invalid decorator"); break;
                }
            } else {   // it's a register
                std::uint64_t regset_size = 0;

                if (eu::type_at(*value, 0) >= EXPR_SIMPLE || eu::value_at(*value, 0) != 1) {
                    as_.nonfatal("invalid operand type");
                    goto fail;
                }

                // We do not allow any kind of expression, except for reg+value in which case it is a register set.
                for (std::size_t k = 1; eu::type_at(*value, k); k++) {
                    if (!(*value)[k].value) continue;

                    if ((*value)[k].type == EXPR_SIMPLE && !regset_size) {
                        regset_size = static_cast<std::uint64_t>((*value)[k].value) + 1;
                        continue;
                    }
                    as_.nonfatal("invalid operand type");
                    goto fail;
                }

                if ((regset_size & (regset_size - 1)) || regset_size >= (std::uint64_t{1} << REGSET_BITS)) {
                    as_.nonfatalf(ERR_PASS2, "invalid register set size");
                    regset_size = 0;
                }

                // Clear overrides, except TO which applies to FPU regs and colon which is used in register pair syntax.
                OpFlags rs;
                if (op->type & ~(opf::TO | opf::COLON)) {
                    // we want to produce a warning iff the specified size is different from the register size
                    rs = op->type & (SIZE_MASK & ~opf::NEAR_);
                } else {
                    rs = 0;
                }

                // Make sure we're not out of nasm_reg_flags, still probably this should be fixed when we're defining the
                // label.
                //
                // An easy trigger is
                //      e equ 0x80000000:0
                //      pshufw word e-0
                if ((*value)[0].type < EXPR_REG_START || (*value)[0].type > EXPR_REG_END) {
                    as_.nonfatal("invalid operand type");
                    goto fail;
                }

                const OpFlags regflags = kRegFlags[static_cast<std::size_t>((*value)[0].type)];
                op->type &= opf::TO | opf::COLON;
                op->type |= opf::REGISTER;
                op->type |= regflags;
                op->type |= (regset_size >> 1) << REGSET_SHIFT;
                op->decoflags = static_cast<DecoFlags>(op->decoflags | brace_flags);
                op->basereg = (*value)[0].type;

                if (rs) {
                    const OpFlags opsize = regflags & (SIZE_MASK & ~opf::NEAR_);
                    if (!opsize) {
                        op->type |= rs;   // for non-size-specific registers, permit size override
                    } else if (opsize != rs) {
                        as_.warn(WARN_REGSIZE, "invalid register size specification ignored");
                    }
                }
            }
        }

        // remember the position of operand having broadcasting/ER mode
        if (op->decoflags & (deco::BRDCAST_MASK | deco::ER | deco::SAE)) {
            result.evex_brerop = opnum;
            op->bcast = true;
            op->xsize = op->decoflags & deco::BRSIZE_MASK;
        } else {
            op->bcast = false;
            op->xsize = op->type & SIZE_MASK;
        }
    }

    result.operands = static_cast<std::uint8_t>(opnum);   // set operand count

    // MASM lets the string instructions carry documentation operands, e.g. `stosd dword ptr es:[edi], eax' or `movsd
    // es:[edi], [esi]'. NASM's string mnemonics take none, so under --masm accept and drop them and encode the implicit
    // form. MOVSD/CMPSD are also SSE mnemonics, so only strip when no operand is an XMM register. (A non-default *source*
    // segment override would need a prefix, but the disassembly corpus emits those as `db' -- every mnemonic-form string
    // op there uses the default segment.)
    if (as_.masm_mode && opnum > 0 && masm_is_string_op(result.opcode)) {
        bool sse = false;
        for (int k = 0; k < opnum; k++) sse |= is_class(opf::XMMREG, result.oprs[static_cast<std::size_t>(k)].type);
        if (!sse) {
            result.operands = 0;
            // The operands documented the implied segments (ES for the destination, DS for the source), which emit no
            // prefix. Drop the segment override captured from e.g. `es:[edi]' so we encode the bare string op. (A
            // non-default source override is a `db' in the corpus, never a mnemonic form, so this never discards a real
            // one.)
            result.prefixes[PPS_SEG] = P_none;
        }
    }

    // MASM emits a redundant operand-size prefix on a segment-register move to or from memory in 32-bit code (`mov word
    // ptr [m], cs' -> 66 8c ..): the value moved is 16-bit, so ML prefixes it, whereas NASM drops the prefix because 8c/8e
    // are inherently 16-bit and treats `word ptr' as redundant. Force o16 under --masm so the bytes match. (Register moves
    // take their size from the GPR and already agree, so only the memory forms need this.)
    if (as_.masm_mode && bits == 32 && result.opcode == I_MOV && result.operands == 2 && result.prefixes[PPS_OSIZE] == P_none &&
        ((is_class(opf::REG_SREG, result.oprs[0].type) && is_class(opf::MEMORY, result.oprs[1].type)) ||
         (is_class(opf::MEMORY, result.oprs[0].type) && is_class(opf::REG_SREG, result.oprs[1].type)))) {
        result.prefixes[PPS_OSIZE] = P_O16;
    }

    // MASM sizes a `mov [mem], OFFSET x' store of a relocatable address by the segment's word size; NASM leaves the memory
    // operand unsized, defaulting the fixup to a byte relocation the obj backend rejects. Under --masm, size an unsized
    // memory operand to word (16-bit) / dword (32-bit) when the other operand is a relocatable immediate (a symbol address,
    // segment set).
    if (as_.masm_mode && result.operands == 2) {
        Operand* mem = nullptr;
        Operand* imm = nullptr;
        for (std::size_t k = 0; k < 2; k++) {
            if (is_class(opf::MEMORY, result.oprs[k].type))
                mem = &result.oprs[k];
            else if (is_class(opf::IMMEDIATE, result.oprs[k].type))
                imm = &result.oprs[k];
        }
        if (mem && imm && !(mem->type & SIZE_MASK) && imm->segment != NO_SEG) mem->type |= (bits >= 32) ? opf::BITS32 : opf::BITS16;
    }

    return result;

fail:
    result.opcode = I_none;
    return result;
}

}  // namespace cb::build::x86asm
