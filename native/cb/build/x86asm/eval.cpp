// cb/build/x86asm/eval.cpp -- the expression evaluator and the expression library; see eval.hpp. A port of asm/eval.c and
// asm/exprlib.c, with the --masm behaviours (OFFSET inside an expression, undefined symbols as 0 in IF, SEG of a segment).
#include "cb/build/x86asm/eval.hpp"

#include <bit>
#include <cstdint>
#include <string>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/floats.hpp"
#include "cb/build/x86asm/labels.hpp"
#include "cb/build/x86asm/nctype.hpp"
#include "cb/build/x86asm/strfunc.hpp"

namespace cb::build::x86asm {

// ---- exprlib.c ----

namespace {

// The end of an expression: a missing term or one of type 0 (NASM's terminator).
bool eval_at_end(const Expr& v, std::size_t i) { return i >= v.size() || v[i].type == 0; }

// Skip the terms whose value is zero: they are insignificant.
std::size_t eval_skip_zero(const Expr& v, std::size_t i) {
    while (!eval_at_end(v, i) && !v[i].value) ++i;
    return i;
}

}  // namespace

bool is_simple(const Expr& vect) {
    std::size_t i = eval_skip_zero(vect, 0);
    if (eval_at_end(vect, i)) return true;
    if (vect[i].type != EXPR_SIMPLE) return false;
    i = eval_skip_zero(vect, i + 1);
    if (!eval_at_end(vect, i) && vect[i].type < EXPR_SEGBASE + SEG_ABS) return false;
    return true;
}

bool is_really_simple(const Expr& vect) {
    std::size_t i = eval_skip_zero(vect, 0);
    if (eval_at_end(vect, i)) return true;
    if (vect[i].type != EXPR_SIMPLE) return false;
    i = eval_skip_zero(vect, i + 1);
    return eval_at_end(vect, i);
}

ExprClass expr_class(const Expr& vect, std::int32_t location_segment) {
    int cls = EC_ZERO;

    for (std::size_t i = 0; !eval_at_end(vect, i); ++i) {
        const ExprTerm& t = vect[i];
        if (!t.value) {
            // A value-0 term.
        } else if (t.type < EXPR_UNKNOWN) {
            if ((cls & EC_REGISTER) || t.value != 1) cls |= EC_REGEXPR;
            else cls |= EC_REGISTER;
        } else if (t.type == EXPR_UNKNOWN) {
            cls |= EC_UNKNOWN;
        } else if (t.type == EXPR_SIMPLE) {
            cls |= EC_CONST;   // a pure number term
        } else if (t.type == EXPR_WRT) {
            cls |= EC_WRT;
        } else if (t.type < EXPR_SEGBASE) {
            cls |= EC_COMPLEX;
        } else if (t.type >= EXPR_SEGBASE + SEG_ABS) {
            // It is an absolute segment.
            if (cls & (EC_SEG | EC_SEGABS)) cls |= EC_COMPLEX;
            cls |= EC_SEGABS;
        } else {
            // It is a segment.
            if (t.value == 1) {
                if (cls & (EC_SEG | EC_SEGABS)) cls |= EC_COMPLEX;
                cls |= EC_SEG;
            } else if (t.value == -1) {
                // Can only subtract the current segment, and only once.
                if (t.type != location_segment + EXPR_SEGBASE || (cls & EC_SELFREL)) cls |= EC_COMPLEX;
                cls |= EC_SELFREL;
            } else {
                cls |= EC_COMPLEX;   // non-simple segment arithmetic
            }
        }
    }
    return static_cast<ExprClass>(cls);
}

bool is_reloc(const Expr& vect, std::int32_t location_segment) { return !(expr_class(vect, location_segment) & ~EC_RELOC); }

bool is_unknown(const Expr& vect) { return (expr_class(vect, 0) & EC_UNKNOWN) != 0; }

bool is_just_unknown(const Expr& vect) { return expr_class(vect, 0) == EC_UNKNOWN; }

std::int64_t reloc_value(const Expr& vect) {
    const std::size_t i = eval_skip_zero(vect, 0);
    if (eval_at_end(vect, i)) return 0;
    return vect[i].type == EXPR_SIMPLE ? vect[i].value : 0;
}

std::int32_t reloc_seg(const Expr& vect) {
    for (std::size_t i = 0; !eval_at_end(vect, i); ++i) {
        if (vect[i].type >= EXPR_SEGBASE && vect[i].value == 1) return vect[i].type - EXPR_SEGBASE;
    }
    return NO_SEG;
}

std::int32_t reloc_wrt(const Expr& vect) {
    std::size_t i = 0;
    while (!eval_at_end(vect, i) && vect[i].type < EXPR_WRT) ++i;
    if (!eval_at_end(vect, i) && vect[i].type == EXPR_WRT) return static_cast<std::int32_t>(vect[i].value);
    return NO_SEG;
}

bool is_self_relative(const Expr& vect, std::int32_t location_segment) { return (expr_class(vect, location_segment) & EC_SELFREL) != 0; }

ExprClass expr_class(const Assembly& as, const Expr& vect) { return expr_class(vect, as.location.segment); }
bool is_reloc(const Assembly& as, const Expr& vect) { return is_reloc(vect, as.location.segment); }
bool is_self_relative(const Assembly& as, const Expr& vect) { return is_self_relative(vect, as.location.segment); }

// ---- eval.c ----

// One evaluation: the parser state NASM keeps in file statics (the scanner, the current token, the flags) is this object's.
class EvalRun {
public:
    EvalRun(Evaluator& ev, const EvalScanFn& sc, TokenVal& tv, int* opflags, bool critical, EvalHints* hint)
        : ev_(ev), as_(ev.as_), scan_fn_(sc), tokval_(tv), opflags_(opflags), critical_(critical), hint_(hint) {}

    std::optional<Expr> run();

private:
    using Opt = std::optional<Expr>;

    // The wrapper around the scanner: read the next token.
    void scan() { tt_ = token_value(scan_fn_(tokval_)); }

    // Add two vector datatypes. There is bizarre behaviour on far-absolute segment types: they are preserved during addition
    // _only_ if one of the segments is a truly pure scalar.
    Expr add_vectors(const Expr& p, const Expr& q);
    // Multiply a vector by a scalar, stripping the far-absolute segment part if present. Explicit treatment of UNKNOWN is not
    // required, since it silently does the right thing anyway. If `affect_hints` is set, the hint type also changes to
    // NOTBASE if a MAKEBASE hint points at a register being multiplied, so [eax*1+ebx] hints EBX rather than EAX as the base.
    Expr scalar_mult(Expr vect, std::int64_t scalar, bool affect_hints);
    static Expr scalarvect(std::int64_t scalar) { return Expr{{EXPR_SIMPLE, scalar}}; }
    static Expr unknown_expr() { return Expr{{EXPR_UNKNOWN, 1}}; }
    Opt segment_part(Expr e);

    // The grammar parsed:
    //
    // expr  : bexpr [ WRT expr6 ]
    // bexpr : cexpr
    // cexpr : rexp0 [ {?} bexpr {:} cexpr ]
    // rexp0 : rexp1 [ {||} rexp1...]
    // rexp1 : rexp2 [ {^^} rexp2...]
    // rexp2 : rexp3 [ {&&} rexp3...]
    // rexp3 : expr0 [ {=,==,<>,!=,<,>,<=,>=,<=>} expr0... ]
    // expr0 : expr1 [ {|} expr1...]
    // expr1 : expr2 [ {^} expr2...]
    // expr2 : expr3 [ {&} expr3...]
    // expr3 : expr4 [ {<<,>>,<<<,>>>} expr4...]
    // expr4 : expr5 [ {+,-} expr5...]
    // expr5 : expr6 [ {*,/,%,//,%%} expr6...]
    // expr6 : { ~,+,-,IFUNC,SEG } expr6 | (bexpr) | symbol | $ | number
    Opt bexpr() { return cexpr(); }
    Opt cexpr();
    Opt rexp0();
    Opt rexp1();
    Opt rexp2();
    Opt rexp3();
    Opt expr0();
    Opt expr1();
    Opt expr2();
    Opt expr3();
    Opt expr4();
    Opt expr5();
    Opt expr6();
    Opt eval_floatize(Floatize type);
    Opt eval_strfunc(StrFunc type, const std::string& name);
    std::int64_t eval_ifunc(std::int64_t val, IFunc func);

    // The scalar operands of a binary operator: each must be a number or an unknown, else report `what`.
    static bool scalar_or_unknown(const Expr& e) { return is_simple(e) || is_just_unknown(e); }
    void need_scalars(const Expr& e, const Expr& f, const char* op) {
        if (!scalar_or_unknown(e) || !scalar_or_unknown(f)) as_.nonfatal("`{}' operator may only be applied to scalar values", op);
    }

    Evaluator& ev_;
    Assembly& as_;
    const EvalScanFn& scan_fn_;
    TokenVal& tokval_;   // the current token
    int tt_ = 0;         // the type of the current token
    int* opflags_;
    bool critical_;
    EvalHints* hint_;
    std::int64_t deadman_ = 0;
};

Expr EvalRun::add_vectors(const Expr& p, const Expr& q) {
    const bool preserve = is_really_simple(p) || is_really_simple(q);
    Expr out;
    std::size_t pi = 0, qi = 0;

    while (!eval_at_end(p, pi) && !eval_at_end(q, qi) && p[pi].type < EXPR_SEGBASE + SEG_ABS && q[qi].type < EXPR_SEGBASE + SEG_ABS) {
        std::int32_t lasttype;
        if (p[pi].type > q[qi].type) {
            out.push_back(q[qi]);
            lasttype = q[qi++].type;
        } else if (p[pi].type < q[qi].type) {
            out.push_back(p[pi]);
            lasttype = p[pi++].type;
        } else {   // *p and *q have the same type
            const std::int64_t sum = static_cast<std::int64_t>(static_cast<std::uint64_t>(p[pi].value) + static_cast<std::uint64_t>(q[qi].value));
            if (sum) {
                out.push_back({p[pi].type, sum});
                if (hint_) hint_->type = EAH_SUMMED;
            }
            lasttype = p[pi].type;
            pi++, qi++;
        }
        if (lasttype == EXPR_UNKNOWN) return out;
    }
    while (!eval_at_end(p, pi) && (preserve || p[pi].type < EXPR_SEGBASE + SEG_ABS)) out.push_back(p[pi++]);
    while (!eval_at_end(q, qi) && (preserve || q[qi].type < EXPR_SEGBASE + SEG_ABS)) out.push_back(q[qi++]);
    return out;
}

Expr EvalRun::scalar_mult(Expr vect, std::int64_t scalar, bool affect_hints) {
    std::size_t i = 0;
    while (!eval_at_end(vect, i) && vect[i].type < EXPR_SEGBASE + SEG_ABS) {
        vect[i].value = static_cast<std::int64_t>(static_cast<std::uint64_t>(scalar) * static_cast<std::uint64_t>(vect[i].value));
        if (hint_ && hint_->type == EAH_MAKEBASE && vect[i].type == hint_->base && affect_hints) hint_->type = EAH_NOTBASE;
        ++i;
    }
    vect.resize(i);   // the terminator goes here: any far-absolute segment part is dropped
    return vect;
}

// The SEG operator: the segment part of a relocatable value. Returns nullopt, as usual, if an error occurs, after reporting it.
EvalRun::Opt EvalRun::segment_part(Expr e) {
    if (is_unknown(e)) return unknown_expr();

    if (!is_reloc(e, as_.location.segment)) {
        as_.nonfatal("cannot apply SEG to a non-relocatable value");
        return std::nullopt;
    }

    const std::int32_t seg = reloc_seg(e);
    if (seg == NO_SEG) {
        as_.nonfatal("cannot apply SEG to a non-relocatable value");
        return std::nullopt;
    }
    if (seg & SEG_ABS) return scalarvect(seg & ~SEG_ABS);
    if (seg & 1) {
        // The operand is already a segment base. MASM allows `SEG segname' (and `SEG group') to name a segment's own base value
        // -- it just yields that base -- so pass the relocatable through unchanged.
        if (as_.masm_mode) return e;
        as_.nonfatal("SEG applied to something which is already a segment base");
        return std::nullopt;
    }
    const std::int32_t base = ev_.segbase ? ev_.segbase(seg + 1) : seg + 1;
    return Expr{{base == NO_SEG ? EXPR_UNKNOWN : EXPR_SEGBASE + base, 1}};
}

EvalRun::Opt EvalRun::cexpr() {
    Opt e = rexp0();
    if (!e) return std::nullopt;

    if (tt_ == token_value(TokenType::QMARK)) {
        scan();
        Opt f = bexpr();
        if (!f) return std::nullopt;

        if (tt_ != ':') {
            as_.nonfatal("`?' without matching `:'");
            return std::nullopt;
        }

        scan();
        Opt g = cexpr();
        if (!g) return std::nullopt;

        if (is_simple(*e)) {
            e = reloc_value(*e) ? std::move(f) : std::move(g);
        } else if (is_just_unknown(*e)) {
            e = unknown_expr();
        } else {
            as_.nonfatal("the left-hand side of `?' must be a scalar value");
        }
    }
    return e;
}

EvalRun::Opt EvalRun::rexp0() {
    Opt e = rexp1();
    if (!e) return std::nullopt;

    while (tt_ == token_value(TokenType::DBL_OR)) {
        scan();
        Opt f = rexp1();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "|");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(static_cast<std::int64_t>(reloc_value(*e) || reloc_value(*f)));
    }
    return e;
}

EvalRun::Opt EvalRun::rexp1() {
    Opt e = rexp2();
    if (!e) return std::nullopt;

    while (tt_ == token_value(TokenType::DBL_XOR)) {
        scan();
        Opt f = rexp2();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "^");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(static_cast<std::int64_t>(!reloc_value(*e) ^ !reloc_value(*f)));
    }
    return e;
}

EvalRun::Opt EvalRun::rexp2() {
    Opt e = rexp3();
    if (!e) return std::nullopt;

    while (tt_ == token_value(TokenType::DBL_AND)) {
        scan();
        Opt f = rexp3();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "&");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(static_cast<std::int64_t>(reloc_value(*e) && reloc_value(*f)));
    }
    return e;
}

EvalRun::Opt EvalRun::rexp3() {
    Opt e = expr0();
    if (!e) return std::nullopt;

    while (tt_ == token_value(TokenType::EQ) || tt_ == token_value(TokenType::LT) || tt_ == token_value(TokenType::GT) ||
           tt_ == token_value(TokenType::NE) || tt_ == token_value(TokenType::LE) || tt_ == token_value(TokenType::GE) ||
           tt_ == token_value(TokenType::LEG)) {
        const int tto = tt_;
        scan();
        Opt f = expr0();
        if (!f) return std::nullopt;

        e = add_vectors(*e, scalar_mult(std::move(*f), -1, false));

        std::int64_t v;
        if (tto == token_value(TokenType::EQ) || tto == token_value(TokenType::NE)) {
            if (is_unknown(*e)) v = -1;   // means unknown
            else if (!is_really_simple(*e) || reloc_value(*e) != 0) v = (tto == token_value(TokenType::NE));   // unequal, so true if NE
            else v = (tto == token_value(TokenType::EQ));   // equal, so true if EQ
        } else {
            if (is_unknown(*e)) {
                v = -1;   // means unknown
            } else if (!is_really_simple(*e)) {
                as_.nonfatal("`{}': operands differ by a non-scalar",
                             tto == token_value(TokenType::LE) ? "<=" :
                             tto == token_value(TokenType::LT) ? "<" :
                             tto == token_value(TokenType::GE) ? ">=" :
                             tto == token_value(TokenType::GT) ? ">" :
                             tto == token_value(TokenType::LEG) ? "<=>" : "<internal error>");
                v = 0;   // must set it to _something_
            } else {
                const std::int64_t vv = reloc_value(*e);
                if (tto == token_value(TokenType::LEG)) v = (vv < 0) ? -1 : (vv > 0) ? 1 : 0;
                else if (vv == 0) v = (tto == token_value(TokenType::LE) || tto == token_value(TokenType::GE));
                else if (vv > 0) v = (tto == token_value(TokenType::GE) || tto == token_value(TokenType::GT));
                else v = (tto == token_value(TokenType::LE) || tto == token_value(TokenType::LT));   // vv < 0
            }
        }

        if (v == -1) e = unknown_expr();
        else e = scalarvect(v);
    }
    return e;
}

EvalRun::Opt EvalRun::expr0() {
    Opt e = expr1();
    if (!e) return std::nullopt;

    while (tt_ == '|') {
        scan();
        Opt f = expr1();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "|");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(reloc_value(*e) | reloc_value(*f));
    }
    return e;
}

EvalRun::Opt EvalRun::expr1() {
    Opt e = expr2();
    if (!e) return std::nullopt;

    while (tt_ == '^') {
        scan();
        Opt f = expr2();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "^");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(reloc_value(*e) ^ reloc_value(*f));
    }
    return e;
}

EvalRun::Opt EvalRun::expr2() {
    Opt e = expr3();
    if (!e) return std::nullopt;

    while (tt_ == '&') {
        scan();
        Opt f = expr3();
        if (!f) return std::nullopt;
        need_scalars(*e, *f, "&");
        if (is_just_unknown(*e) || is_just_unknown(*f)) e = unknown_expr();
        else e = scalarvect(reloc_value(*e) & reloc_value(*f));
    }
    return e;
}

EvalRun::Opt EvalRun::expr3() {
    Opt e = expr4();
    if (!e) return std::nullopt;

    while (tt_ == token_value(TokenType::SHL) || tt_ == token_value(TokenType::SHR) || tt_ == token_value(TokenType::SAR)) {
        const int tto = tt_;
        scan();
        Opt f = expr4();
        if (!f) return std::nullopt;
        if (!scalar_or_unknown(*e) || !scalar_or_unknown(*f)) {
            as_.nonfatal("shift operator may only be applied to scalar values");
        } else if (is_just_unknown(*e) || is_just_unknown(*f)) {
            e = unknown_expr();
        } else {
            // C leaves a shift by 64 or more undefined; the machine this was written for takes the count modulo the width.
            const std::int64_t a = reloc_value(*e);
            const unsigned n = static_cast<unsigned>(reloc_value(*f)) & 63u;
            if (tto == token_value(TokenType::SHL)) e = scalarvect(static_cast<std::int64_t>(static_cast<std::uint64_t>(a) << n));
            else if (tto == token_value(TokenType::SHR)) e = scalarvect(static_cast<std::int64_t>(static_cast<std::uint64_t>(a) >> n));
            else e = scalarvect(a >> n);
        }
    }
    return e;
}

EvalRun::Opt EvalRun::expr4() {
    Opt e = expr5();
    if (!e) return std::nullopt;
    while (tt_ == '+' || tt_ == '-') {
        const int tto = tt_;
        scan();
        Opt f = expr5();
        if (!f) return std::nullopt;
        if (tto == '+') e = add_vectors(*e, *f);
        else e = add_vectors(*e, scalar_mult(std::move(*f), -1, false));
    }
    return e;
}

EvalRun::Opt EvalRun::expr5() {
    Opt e = expr6();
    if (!e) return std::nullopt;
    while (tt_ == '*' || tt_ == '/' || tt_ == '%' || tt_ == token_value(TokenType::SDIV) || tt_ == token_value(TokenType::SMOD)) {
        const int tto = tt_;
        scan();
        Opt f = expr6();
        if (!f) return std::nullopt;
        if (tto != '*' && (!scalar_or_unknown(*e) || !scalar_or_unknown(*f))) {
            as_.nonfatal("division operator may only be applied to scalar values");
            return std::nullopt;
        }
        if (tto != '*' && !is_just_unknown(*f) && reloc_value(*f) == 0) {
            as_.nonfatal("division by zero");
            return std::nullopt;
        }
        const bool unknown = is_just_unknown(*e) || is_just_unknown(*f);
        if (tto == '*') {
            if (is_simple(*e)) e = scalar_mult(std::move(*f), reloc_value(*e), true);
            else if (is_simple(*f)) e = scalar_mult(std::move(*e), reloc_value(*f), true);
            else if (is_just_unknown(*e) && is_just_unknown(*f)) e = unknown_expr();
            else {
                as_.nonfatal("unable to multiply two non-scalar objects");
                return std::nullopt;
            }
        } else if (unknown) {
            e = unknown_expr();
        } else {
            const std::int64_t a = reloc_value(*e);
            const std::int64_t b = reloc_value(*f);
            if (tto == '/') {
                e = scalarvect(static_cast<std::int64_t>(static_cast<std::uint64_t>(a) / static_cast<std::uint64_t>(b)));
            } else if (tto == '%') {
                e = scalarvect(static_cast<std::int64_t>(static_cast<std::uint64_t>(a) % static_cast<std::uint64_t>(b)));
            } else if (b == -1) {
                // INT64_MIN / -1 traps on the machine; the answer a C compiler's target gives is the wrapped one.
                e = scalarvect(tto == token_value(TokenType::SDIV) ? static_cast<std::int64_t>(0 - static_cast<std::uint64_t>(a)) : 0);
            } else if (tto == token_value(TokenType::SDIV)) {
                e = scalarvect(a / b);
            } else {
                e = scalarvect(a % b);
            }
        }
    }
    return e;
}

EvalRun::Opt EvalRun::eval_floatize(Floatize type) {
    const IeeeFormat& fmt = kFpFormats[static_cast<std::size_t>(type)];
    int sign = 1;

    scan();
    if (tt_ != '(') {
        as_.nonfatal("expecting `('");
        return std::nullopt;
    }
    scan();
    if (tt_ == '-' || tt_ == '+') {
        sign = (tt_ == '-') ? -1 : 1;
        scan();
    }
    if (tt_ != token_value(TokenType::FLOAT)) {
        as_.nonfatal("expecting floating-point number");
        return std::nullopt;
    }
    FloatBytes result{};
    if (!ev_.floats_.float_const(tokval_.text, sign, result, type)) return std::nullopt;
    scan();
    if (tt_ != ')') {
        as_.nonfatal("expecting `)'");
        return std::nullopt;
    }

    // The bytes of the format from the offset on, at most 64 bits of them, as an integer.
    std::size_t len = static_cast<std::size_t>(fmt.bytes - fmt.offset);
    if (len > 8) len = 8;
    std::uint64_t val = 0;
    for (std::size_t i = len; i; --i) val = (val << 8) + result[static_cast<std::size_t>(fmt.offset) + i - 1];

    scan();
    return scalarvect(static_cast<std::int64_t>(val));
}

EvalRun::Opt EvalRun::eval_strfunc(StrFunc type, const std::string& name) {
    bool parens = false;
    scan();
    if (tt_ == '(') {
        parens = true;
        scan();
    }
    if (tt_ != token_value(TokenType::STR)) {
        as_.nonfatal("expecting string as argument to {}", name);
        return std::nullopt;
    }
    const std::string_view input = std::string_view(tokval_.text).substr(0, static_cast<std::size_t>(tokval_.inttwo));
    const std::optional<std::string> transformed = string_transform(input, type);
    if (!transformed) {
        as_.nonfatal("invalid input string to {}", name);
        return std::nullopt;
    }

    bool rn_warn = false;
    const std::int64_t val = readstrnum(*transformed, rn_warn);
    if (parens) {
        scan();
        if (tt_ != ')') {
            as_.nonfatal("expecting `)'");
            return std::nullopt;
        }
    }

    if (rn_warn) as_.warn(WARN_OTHER, "character constant too long");

    scan();
    return scalarvect(val);
}

std::int64_t EvalRun::eval_ifunc(std::int64_t val, IFunc func) {
    const auto uval = static_cast<std::uint64_t>(val);
    // ilog2 of a value; 0 for 0 (ilog2_64).
    const auto ilog2 = [](std::uint64_t v) { return static_cast<std::int64_t>(std::bit_width(v | 1) - 1); };
    switch (func) {
    case IFUNC_ILOG2E:
    case IFUNC_ILOG2W:
        if (!(uval && (uval & (uval - 1)) == 0)) {
            as_.error(func == IFUNC_ILOG2E ? ERR_NONFATAL : (ERR_WARNING | WARN_OTHER), "ilog2 argument is not a power of two");
        }
        return ilog2(uval);
    case IFUNC_ILOG2F:
        return ilog2(uval);
    case IFUNC_ILOG2C:
        return (uval < 2) ? 0 : ilog2(uval - 1) + 1;
    }
    as_.panic("invalid IFUNC token {}", static_cast<int>(func));
}

EvalRun::Opt EvalRun::expr6() {
    if (++deadman_ > as_.nasm_limit[LIMIT_EVAL]) {
        as_.nonfatal("expression too long");
        return std::nullopt;
    }

    // MASM (--masm): OFFSET is a no-op prefix inside an expression -- a bare label is already its offset -- so `dw N + OFFSET foo'
    // and `dd OFFSET foo' work. The parser strips a leading OFFSET on a whole operand (and gives it its data-label-vs-address
    // meaning there); handling it here as well lets it appear mid-expression (kdata's `(N-1)*size S + dataOffset foo', where
    // the createSeg <seg>OFFSET operator expands to OFFSET).
    if (as_.masm_mode && tt_ == token_value(TokenType::ID) && !tokval_.text.empty()) {
        std::string lower = tokval_.text;
        for (char& c : lower) c = nasm_tolower(c);
        if (lower == "offset") {
            scan();
            return expr6();
        }
    }

    switch (tt_) {
    case '-': {
        scan();
        Opt e = expr6();
        if (!e) return std::nullopt;
        return scalar_mult(std::move(*e), -1, false);
    }

    case '+':
        scan();
        return expr6();

    case '~': {
        scan();
        Opt e = expr6();
        if (!e) return std::nullopt;
        if (is_just_unknown(*e)) return unknown_expr();
        if (!is_simple(*e)) {
            as_.nonfatal("`~' operator may only be applied to scalar values");
            return std::nullopt;
        }
        return scalarvect(~reloc_value(*e));
    }

    case '!': {
        scan();
        Opt e = expr6();
        if (!e) return std::nullopt;
        if (is_just_unknown(*e)) return unknown_expr();
        if (!is_simple(*e)) {
            as_.nonfatal("`!' operator may only be applied to scalar values");
            return std::nullopt;
        }
        return scalarvect(!reloc_value(*e));
    }

    case static_cast<int>(TokenType::IFUNC): {
        const auto func = static_cast<IFunc>(tokval_.integer);
        scan();
        Opt e = expr6();
        if (!e) return std::nullopt;
        if (is_just_unknown(*e)) return unknown_expr();
        if (!is_simple(*e)) {
            as_.nonfatal("function may only be applied to scalar values");
            return std::nullopt;
        }
        return scalarvect(eval_ifunc(reloc_value(*e), func));
    }

    case static_cast<int>(TokenType::SEG): {
        scan();
        Opt e = expr6();
        if (!e) return std::nullopt;
        e = segment_part(std::move(*e));
        if (!e) return std::nullopt;
        if (is_unknown(*e) && critical_) {
            as_.nonfatal("unable to determine segment base");
            return std::nullopt;
        }
        return e;
    }

    case static_cast<int>(TokenType::FLOATIZE):
        return eval_floatize(static_cast<Floatize>(tokval_.integer));

    case static_cast<int>(TokenType::STRFUNC): {
        const std::string name = tokval_.text;
        return eval_strfunc(static_cast<StrFunc>(tokval_.integer), name);
    }

    case '(': {
        scan();
        Opt e = bexpr();
        if (!e) return std::nullopt;
        if (tt_ != ')') {
            as_.nonfatal("expecting `)'");
            return std::nullopt;
        }
        scan();
        return e;
    }

    case static_cast<int>(TokenType::NUM):
    case static_cast<int>(TokenType::BRCCONST):
    case static_cast<int>(TokenType::STR):
    case static_cast<int>(TokenType::REG):
    case static_cast<int>(TokenType::ID):
    case static_cast<int>(TokenType::INSN):   // opcodes that occur here are really labels
    case static_cast<int>(TokenType::HERE):
    case static_cast<int>(TokenType::BASE):
    case static_cast<int>(TokenType::DECORATOR): {
        Expr temp;
        switch (static_cast<TokenType>(tt_)) {
        case TokenType::NUM:
        case TokenType::BRCCONST:
            temp.push_back({EXPR_SIMPLE, tokval_.integer});
            break;
        case TokenType::STR: {
            bool rn_warn = false;
            const std::int64_t tmpval = readstrnum(std::string_view(tokval_.text).substr(0, static_cast<std::size_t>(tokval_.inttwo)), rn_warn);
            if (rn_warn) as_.warn(WARN_OTHER, "character constant too long");
            temp.push_back({EXPR_SIMPLE, tmpval});
            break;
        }
        case TokenType::REG:
            temp.push_back({static_cast<std::int32_t>(tokval_.integer), 1});
            if (hint_ && hint_->type == EAH_NOHINT) {
                hint_->base = tokval_.integer;
                hint_->type = EAH_MAKEBASE;
            }
            break;
        case TokenType::ID:
        case TokenType::INSN:
        case TokenType::HERE:
        case TokenType::BASE: {
            // If !location.known, no symbol, Here or Base references are valid because we are in preprocess-only mode.
            if (!as_.location.known) {
                as_.nonfatal("{} not supported in preprocess-only mode",
                             tt_ == token_value(TokenType::HERE) ? "`$'" : tt_ == token_value(TokenType::BASE) ? "`$$'" : "symbol references");
                temp.push_back({EXPR_UNKNOWN, 1});
                break;
            }

            std::int32_t type = EXPR_SIMPLE;   // might get overridden by UNKNOWN
            std::int32_t label_seg = NO_SEG;
            std::int64_t label_ofs = 0;
            if (tt_ == token_value(TokenType::BASE)) {
                label_seg = as_.in_absolute ? as_.absolute.segment : as_.location.segment;
                label_ofs = 0;
            } else if (tt_ == token_value(TokenType::HERE)) {
                label_seg = as_.in_absolute ? as_.absolute.segment : as_.location.segment;
                label_ofs = as_.in_absolute ? as_.absolute.offset : as_.location.offset;
            } else {
                const LabelType ltype = ev_.labels_.lookup_label(tokval_.text, label_seg, label_ofs);
                if (ltype == LBL_none && as_.masm_if_undef_zero) {
                    // MASM (--masm): an undefined symbol in a conditional-assembly expression (`IF SDEBUG') evaluates to 0.
                    temp.push_back({EXPR_SIMPLE, 0});
                    break;
                }
                if (ltype == LBL_none) {
                    const std::string scope = ev_.labels_.local_scope(tokval_.text);
                    if (critical_) {
                        as_.nonfatal("symbol `{}{}' not defined{}", scope, tokval_.text, as_.pass_first() ? " before use" : "");
                        return std::nullopt;
                    }
                    if (opflags_) *opflags_ |= OPFLAG_FORWARD;
                    type = EXPR_UNKNOWN;
                    label_seg = NO_SEG;
                    label_ofs = 1;
                } else if (is_extern(ltype)) {
                    if (opflags_) *opflags_ |= OPFLAG_EXTERN;
                }
            }
            temp.push_back({type, label_ofs});
            if (label_seg != NO_SEG) temp.push_back({EXPR_SEGBASE + label_seg, 1});
            break;
        }
        case TokenType::DECORATOR:
            temp.push_back({EXPR_RDSAE, tokval_.integer});
            break;
        default:
            break;
        }
        scan();
        return temp;
    }

    default:
        as_.nonfatal("expression syntax error");
        return std::nullopt;
    }
}

std::optional<Expr> EvalRun::run() {
    Opt f;

    tt_ = token_value(tokval_.type);
    if (tt_ == token_value(TokenType::INVALID)) scan();

    Opt e = bexpr();
    if (!e) return std::nullopt;

    if (tt_ == token_value(TokenType::WRT)) {
        scan();   // eat the WRT
        f = expr6();
        if (!f) return std::nullopt;
    }
    e = scalar_mult(std::move(*e), 1, false);   // strip the far-absolute segment part
    if (f) {
        Expr g;
        if (is_just_unknown(*f)) {
            g = unknown_expr();
        } else {
            if (!is_reloc(*f, as_.location.segment)) {
                as_.nonfatal("invalid right-hand operand to WRT");
                return std::nullopt;
            }
            std::int64_t value = reloc_seg(*f);
            if (value == NO_SEG) {
                value = reloc_value(*f) | SEG_ABS;
            } else if (!(value & SEG_ABS) && !(value % 2) && critical_) {
                as_.nonfatal("invalid right-hand operand to WRT");
                return std::nullopt;
            }
            g = Expr{{EXPR_WRT, value}};
        }
        e = add_vectors(*e, g);
    }
    return e;
}

std::optional<Expr> Evaluator::evaluate(const EvalScanFn& sc, TokenVal& tv, int* fwref, bool critical, EvalHints* hints) {
    if (hints) hints->type = EAH_NOHINT;
    EvalRun run(*this, sc, tv, fwref, critical, hints);
    return run.run();
}

}  // namespace cb::build::x86asm
