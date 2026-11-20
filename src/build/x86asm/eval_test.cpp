// Tests of the expression evaluator and expression library (eval.hpp). The values are what nasm.exe computes for the same
// expressions in `dq` data (`1<<64`, `-7//2`, `__?ilog2f?__(1000)`, `__?float32?__(1.5)`, `__?utf16?__("ab")`, ...), and the
// error texts are nasm's.
#include "build/x86asm/eval.hpp"

#include <string>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/floats.hpp"
#include "build/x86asm/labels.hpp"
#include "build/x86asm/scan.hpp"
#include "core/testing.hpp"

namespace {
using namespace cb::build::x86asm;

// One assembly with a label table, scanner and evaluator, assembling into segment 2 at offset 0x10.
struct EvFix {
    Assembly as;
    LabelTable labels{as};
    Floats floats{as};
    Evaluator ev{as, labels, floats};
    Scanner sc{as};
    std::int32_t next_seg = 100;

    EvFix() {
        as.src.set(1, "t.asm");
        labels.init();
        labels.hooks.seg_alloc = [this] { const std::int32_t s = next_seg; next_seg += 2; return s; };
        as.location.known = true;
        as.location.segment = 2;
        as.location.offset = 0x10;
    }

    std::optional<Expr> eval(std::string_view text, bool critical = false, int* fwref = nullptr, EvalHints* hints = nullptr) {
        sc.reset(text);
        TokenVal tv;
        tv.type = TokenType::INVALID;
        const EvalScanFn fn = [this](TokenVal& t) { return static_cast<TokenType>(sc.scan(t)); };
        last_end = TokenType::INVALID;
        auto result = ev.evaluate(fn, tv, fwref, critical, hints);
        last_end = tv.type;
        return result;
    }

    TokenType last_end = TokenType::INVALID;

    std::string last_error() const { return as.diagnostics().empty() ? std::string() : as.diagnostics().back().text; }
};

std::string ev_show(const Expr& e) {
    std::string out;
    for (const ExprTerm& t : e) {
        if (!out.empty()) out += ',';
        if (t.type == EXPR_UNKNOWN) out += "unk";
        else if (t.type == EXPR_SIMPLE) out += "num";
        else if (t.type == EXPR_WRT) out += "wrt";
        else if (t.type == EXPR_RDSAE) out += "rdsae";
        else if (t.type >= EXPR_SEGBASE) out += "seg" + std::to_string(t.type - EXPR_SEGBASE);
        else out += "reg" + std::to_string(t.type);
        out += "=" + std::to_string(t.value);
    }
    return out;
}

// The expression as text, or "ERR" if the evaluator gave up.
std::string ev_S(EvFix& f, std::string_view text, bool critical = false) {
    const auto e = f.eval(text, critical);
    return e ? ev_show(*e) : "ERR";
}

// The value of a numeric expression, as the number text ("ERR" for an error).
std::string ev_N(EvFix& f, std::string_view text) {
    const auto e = f.eval(text);
    if (!e) return "ERR";
    if (!is_simple(*e)) return "not simple: " + ev_show(*e);
    return std::to_string(reloc_value(*e));
}

Expr ev_ex(std::initializer_list<ExprTerm> terms) { return Expr(terms); }
}  // namespace

CB_TEST("x86asm.eval.arithmetic", [] {
    EvFix f;
    // nasm: dq 6*7, 100-1, 0xff & 0x0f, 0xf0 | 0x0f, 0xff ^ 0x0f, 7 && 0, (1+2)*3
    CB_CHECK_EQ(ev_N(f, "6*7"), "42");
    CB_CHECK_EQ(ev_N(f, "100-1"), "99");
    CB_CHECK_EQ(ev_N(f, "0xff & 0x0f"), "15");
    CB_CHECK_EQ(ev_N(f, "0xf0 | 0x0f"), "255");
    CB_CHECK_EQ(ev_N(f, "0xff ^ 0x0f"), "240");
    CB_CHECK_EQ(ev_N(f, "(1+2)*3"), "9");
    // nasm: dq 5 - 3 - 1, 2*3+4, 2+3*4, 100/7/2
    CB_CHECK_EQ(ev_N(f, "5 - 3 - 1"), "1");
    CB_CHECK_EQ(ev_N(f, "2*3+4"), "10");
    CB_CHECK_EQ(ev_N(f, "2+3*4"), "14");
    CB_CHECK_EQ(ev_N(f, "100/7/2"), "7");
    // nasm: db 0x7f+1 -> 0x80 ; dq 0x7fffffffffffffff + 1 -> wraps
    CB_CHECK_EQ(ev_N(f, "0x7f+1"), "128");
    CB_CHECK_EQ(ev_N(f, "0x7fffffffffffffff + 1"), std::to_string(INT64_MIN));
    // Unary operators. nasm: dq !5, !0, ~0, -(-3), +4
    CB_CHECK_EQ(ev_N(f, "!5"), "0");
    CB_CHECK_EQ(ev_N(f, "!0"), "1");
    CB_CHECK_EQ(ev_N(f, "~0"), "-1");
    CB_CHECK_EQ(ev_N(f, "-(-3)"), "3");
    CB_CHECK_EQ(ev_N(f, "+4"), "4");
    CB_CHECK_EQ(ev_N(f, "- - 5"), "5");
    // The expression ended at the end of the line.
    f.eval("1+2");
    CB_CHECK(f.last_end == TokenType::EOS);
    f.eval("1+2, 3");
    CB_CHECK(f.last_end == TokenType::COMMA);
});

CB_TEST("x86asm.eval.shifts", [] {
    EvFix f;
    // nasm: dq 1<<64, 1<<63, 1<<65, -1>>1, -1>>>1, -8>>>1, 1<<-1  (a count is taken modulo 64, as the machine does)
    CB_CHECK_EQ(ev_N(f, "1<<64"), "1");
    CB_CHECK_EQ(ev_N(f, "1<<63"), std::to_string(INT64_MIN));
    CB_CHECK_EQ(ev_N(f, "1<<65"), "2");
    CB_CHECK_EQ(ev_N(f, "-1>>1"), std::to_string(INT64_MAX));
    CB_CHECK_EQ(ev_N(f, "-1>>>1"), "-1");
    CB_CHECK_EQ(ev_N(f, "-8>>>1"), "-4");
    CB_CHECK_EQ(ev_N(f, "1<<-1"), std::to_string(INT64_MIN));
    CB_CHECK_EQ(ev_N(f, "1<<<4"), "16");
    CB_CHECK_EQ(ev_N(f, "0x100 >> 4"), "16");
});

CB_TEST("x86asm.eval.division", [] {
    EvFix f;
    // nasm: dq -7/2, -7//2, -7 % 2, -7 %% 2, 5//-2, 5 %% -2   (/ and % are unsigned, // and %% are signed)
    CB_CHECK_EQ(ev_N(f, "-7/2"), std::to_string(0x7ffffffffffffffcLL));
    CB_CHECK_EQ(ev_N(f, "-7//2"), "-3");
    CB_CHECK_EQ(ev_N(f, "-7 % 2"), "1");
    CB_CHECK_EQ(ev_N(f, "-7 %% 2"), "-1");
    CB_CHECK_EQ(ev_N(f, "5//-2"), "-2");
    CB_CHECK_EQ(ev_N(f, "5 %% -2"), "1");
    // nasm: dq 10 / 0, 10 // 0, 10 %% 0 -> "division by zero"
    CB_CHECK_EQ(ev_S(f, "10 / 0"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: division by zero");
    CB_CHECK_EQ(ev_S(f, "10 // 0"), "ERR");
    CB_CHECK_EQ(ev_S(f, "10 %% 0"), "ERR");
    CB_CHECK_EQ(f.as.diagnostics().size(), std::size_t{3});
    // The one signed overflow does not trap here; it wraps.
    CB_CHECK_EQ(ev_N(f, "-9223372036854775808 // -1"), std::to_string(INT64_MIN));
    CB_CHECK_EQ(ev_N(f, "-9223372036854775808 %% -1"), "0");
});

CB_TEST("x86asm.eval.comparisons_and_logic", [] {
    EvFix f;
    // nasm: dq 1<2, 2<=2, 3>2, 1==1, 1!=1, 1<>2   (all 1 but 1!=1)
    CB_CHECK_EQ(ev_N(f, "1<2"), "1");
    CB_CHECK_EQ(ev_N(f, "2<=2"), "1");
    CB_CHECK_EQ(ev_N(f, "3>2"), "1");
    CB_CHECK_EQ(ev_N(f, "1==1"), "1");
    CB_CHECK_EQ(ev_N(f, "1=1"), "1");
    CB_CHECK_EQ(ev_N(f, "1!=1"), "0");
    CB_CHECK_EQ(ev_N(f, "1<>2"), "1");
    CB_CHECK_EQ(ev_N(f, "2>=3"), "0");
    // nasm: dq 1<=>2, 2<=>2, 3<=>2 -> 0, 0, 1: the -1 of "less" is NASM's own marker for unknown, so 1<=>2 is unknown
    // (it comes out as 0 in a data item).
    CB_CHECK_EQ(ev_S(f, "1<=>2"), "unk=1");
    CB_CHECK_EQ(ev_N(f, "2<=>2"), "0");
    CB_CHECK_EQ(ev_N(f, "3<=>2"), "1");
    // nasm: dq 1 ? 2 : 3, 0 ? 2 : 3, 1||0, 0||0, 1&&0, 1&&2, 1^^1, 1^^0
    CB_CHECK_EQ(ev_N(f, "1 ? 2 : 3"), "2");
    CB_CHECK_EQ(ev_N(f, "0 ? 2 : 3"), "3");
    CB_CHECK_EQ(ev_N(f, "1||0"), "1");
    CB_CHECK_EQ(ev_N(f, "0||0"), "0");
    CB_CHECK_EQ(ev_N(f, "1&&0"), "0");
    CB_CHECK_EQ(ev_N(f, "1&&2"), "1");
    CB_CHECK_EQ(ev_N(f, "1^^1"), "0");
    CB_CHECK_EQ(ev_N(f, "1^^0"), "1");
    CB_CHECK_EQ(ev_N(f, "7 && 0"), "0");
    CB_CHECK_EQ(ev_N(f, "1 ? 2 : 0 ? 3 : 4"), "2");
    CB_CHECK_EQ(ev_N(f, "0 ? 2 : 0 ? 3 : 4"), "4");
});

CB_TEST("x86asm.eval.syntax_errors", [] {
    EvFix f;
    // Each text and message is nasm's (assembling `dq <expression>`).
    struct E {
        const char* text;
        const char* message;
    };
    static const E errs[] = {
        {"1 +", "expression syntax error"},
        {"1 - ,", "expression syntax error"},
        {"(1", "expecting `)'"},
        {"1 ? 2", "`?' without matching `:'"},
        {"1 &&& 3", "expression syntax error"},
        {"__?float32?__(abc)", "expecting floating-point number"},
        {"__?float32?__ 1.5", "expecting `('"},
        {"__?float32?__(1.5", "expecting `)'"},
        {"foo wrt", "expression syntax error"},
    };
    for (const E& e : errs) {
        f.as.clear_diagnostics();
        CB_CHECK_EQ(ev_S(f, e.text), "ERR");
        CB_CHECK_EQ(f.last_error(), std::string("t.asm:1: error: ") + e.message);
    }
    // A string function wants a string.
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "__?utf16?__(5)"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: expecting string as argument to __?utf16?__");
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "__?utf16?__(`\\xc3`)"), "ERR");   // not valid UTF-8
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: invalid input string to __?utf16?__");
});

CB_TEST("x86asm.eval.limit", [] {
    EvFix f;
    f.as.nasm_limit[LIMIT_EVAL] = 4;
    CB_CHECK_EQ(ev_S(f, "1+1+1"), "num=3");
    CB_CHECK_EQ(ev_S(f, "1+1+1+1+1+1+1"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: expression too long");
});

CB_TEST("x86asm.eval.ilog2", [] {
    EvFix f;
    // nasm: dq __?ilog2f?__(1000), __?ilog2c?__(1000), __?ilog2e?__(1024), __?ilog2w?__(1000), __?ilog2c?__(1), __?ilog2c?__(0)
    CB_CHECK_EQ(ev_N(f, "__?ilog2f?__(1000)"), "9");
    CB_CHECK_EQ(ev_N(f, "__?ilog2c?__(1000)"), "10");
    CB_CHECK_EQ(ev_N(f, "__?ilog2e?__(1024)"), "10");
    CB_CHECK(f.as.diagnostics().empty());
    CB_CHECK_EQ(ev_N(f, "__?ilog2w?__(1000)"), "9");   // a warning: "ilog2 argument is not a power of two"
    CB_CHECK_EQ(f.last_error(), "t.asm:1: warning: ilog2 argument is not a power of two [-w+other]");
    CB_CHECK_EQ(ev_N(f, "__?ilog2c?__(1)"), "0");
    CB_CHECK_EQ(ev_N(f, "__?ilog2c?__(0)"), "0");
    CB_CHECK_EQ(ev_N(f, "__?ilog2f?__(0)"), "0");
    CB_CHECK_EQ(ev_N(f, "__?ilog2f?__ 256"), "8");     // the argument need not be in parentheses
    // The ilog2e of a non-power is an error, but the value is still computed.
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_N(f, "__?ilog2e?__(1000)"), "9");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: ilog2 argument is not a power of two");
});

CB_TEST("x86asm.eval.floatize", [] {
    EvFix f;
    // nasm: dq __?float32?__(1.5), __?float64?__(-2.0), __?float80m?__(1.0), __?float80e?__(1.0), __?float16?__(1.5),
    // __?bfloat16?__(1.5), __?float128l?__(1.0), __?float128h?__(1.0), __?float8?__(1.5), __?float32?__(-1.5), __?float64?__(0.1)
    CB_CHECK_EQ(ev_N(f, "__?float32?__(1.5)"), std::to_string(0x3fc00000LL));
    CB_CHECK_EQ(ev_N(f, "__?float64?__(-2.0)"), std::to_string(static_cast<std::int64_t>(0xc000000000000000ULL)));
    CB_CHECK_EQ(ev_N(f, "__?float80m?__(1.0)"), std::to_string(static_cast<std::int64_t>(0x8000000000000000ULL)));
    CB_CHECK_EQ(ev_N(f, "__?float80e?__(1.0)"), std::to_string(0x3fffLL));
    CB_CHECK_EQ(ev_N(f, "__?float16?__(1.5)"), std::to_string(0x3e00LL));
    CB_CHECK_EQ(ev_N(f, "__?bfloat16?__(1.5)"), std::to_string(0x3fc0LL));
    CB_CHECK_EQ(ev_N(f, "__?float128l?__(1.0)"), "0");
    CB_CHECK_EQ(ev_N(f, "__?float128h?__(1.0)"), std::to_string(0x3fff000000000000LL));
    CB_CHECK_EQ(ev_N(f, "__?float8?__(1.5)"), std::to_string(0x3cLL));
    CB_CHECK_EQ(ev_N(f, "__?float32?__(-1.5)"), std::to_string(0xbfc00000LL));
    CB_CHECK_EQ(ev_N(f, "__?float64?__(0.1)"), std::to_string(0x3fb999999999999aLL));
    CB_CHECK_EQ(ev_N(f, "__?float32?__(+1.5) + 1"), std::to_string(0x3fc00001LL));
});

CB_TEST("x86asm.eval.string_functions", [] {
    EvFix f;
    // nasm: dq __?utf16?__("ab"), __?utf32le?__("a"), __?utf16?__("abcd")
    CB_CHECK_EQ(ev_N(f, "__?utf16?__(\"ab\")"), std::to_string(0x00620061LL));
    CB_CHECK_EQ(ev_N(f, "__?utf32le?__(\"a\")"), "97");
    CB_CHECK_EQ(ev_N(f, "__?utf16?__(\"abcd\")"), std::to_string(0x0064006300620061LL));
    CB_CHECK_EQ(ev_N(f, "__?utf16?__ \"x\""), "120");
    CB_CHECK_EQ(ev_N(f, "__?utf16?__(\"x\") + 1"), "121");
    // nasm: dw __utf16__("x") -> 78 00
    CB_CHECK_EQ(ev_N(f, "__?utf16le?__('x')"), "120");
    // Strings as numbers: nasm: dq "a", "ab", "abcdefghi"
    CB_CHECK_EQ(ev_N(f, "'a'"), "97");
    CB_CHECK_EQ(ev_N(f, "\"ab\""), std::to_string(0x6261LL));
    CB_CHECK(f.as.diagnostics().empty());
    CB_CHECK_EQ(ev_N(f, "\"abcdefghi\""), std::to_string(0x6867666564636261LL));
    CB_CHECK_EQ(f.last_error(), "t.asm:1: warning: character constant too long [-w+other]");
    CB_CHECK_EQ(ev_N(f, "`\\x41\\n`"), std::to_string(0x0a41LL));
    CB_CHECK_EQ(ev_N(f, "''"), "0");
});

CB_TEST("x86asm.eval.labels_and_location", [] {
    EvFix f;
    f.labels.define_label("start", 2, 0, true);
    f.labels.define_label("lab", 2, 0x20, true);
    f.labels.define_label("lab2", 2, 0x28, true);
    f.labels.define_label("oth", 4, 8, true);
    // A label is its offset plus the base of its segment.
    CB_CHECK_EQ(ev_S(f, "lab"), "num=32,seg2=1");
    CB_CHECK_EQ(ev_S(f, "lab+4"), "num=36,seg2=1");
    CB_CHECK_EQ(ev_S(f, "4+lab"), "num=36,seg2=1");
    CB_CHECK_EQ(ev_S(f, "oth"), "num=8,seg4=1");
    // The difference of two labels of one segment is a number.
    CB_CHECK_EQ(ev_S(f, "lab2-lab"), "num=8");
    CB_CHECK_EQ(ev_S(f, "(lab2-lab)*2"), "num=16");
    CB_CHECK_EQ(ev_N(f, "lab2 - start"), "40");
    // $ is the current location, $$ the start of the segment; nasm: start: dq start, $, $$, $-$$, $ - start
    CB_CHECK_EQ(ev_S(f, "$"), "num=16,seg2=1");
    CB_CHECK_EQ(ev_S(f, "$$"), "num=0,seg2=1");
    CB_CHECK_EQ(ev_S(f, "$-$$"), "num=16");
    CB_CHECK_EQ(ev_S(f, "$ - start"), "num=16");
    CB_CHECK_EQ(ev_S(f, "lab - $"), "num=16");
    CB_CHECK_EQ(ev_S(f, "$$ + 3"), "num=3,seg2=1");
    // A label of another segment makes a difference that is not a number: it is the label less this segment's base.
    CB_CHECK_EQ(ev_S(f, "oth - $"), "num=-8,seg2=-1,seg4=1");
    CB_CHECK_EQ(ev_S(f, "lab - oth"), "num=24,seg2=1,seg4=-1");
    // In an ABSOLUTE section $ and $$ come from it.
    f.as.in_absolute = true;
    f.as.absolute.segment = 6;
    f.as.absolute.offset = 0x44;
    CB_CHECK_EQ(ev_S(f, "$"), "num=68,seg6=1");
    CB_CHECK_EQ(ev_S(f, "$$"), "num=0,seg6=1");
    f.as.in_absolute = false;
    // Multiplying a relocatable keeps the segment term too, as NASM does.
    CB_CHECK_EQ(ev_S(f, "lab*2"), "num=64,seg2=2");
    // Strings and registers in expressions.
    CB_CHECK_EQ(ev_S(f, "'a'+lab"), "num=129,seg2=1");
    CB_CHECK_EQ(ev_S(f, "lab == lab"), "num=1");
    CB_CHECK_EQ(ev_S(f, "lab != lab2"), "num=1");
});

CB_TEST("x86asm.eval.local_labels", [] {
    EvFix f;
    f.labels.define_label("main", 2, 0x40, true);
    f.labels.define_label(".loop", 2, 0x48, true);
    f.labels.define_label("next", 2, 0x50, true);
    f.labels.define_label(".loop", 2, 0x58, true);
    // nasm: .loop belongs to the last non-local label; main.loop is its full name.
    CB_CHECK_EQ(ev_S(f, ".loop"), "num=88,seg2=1");
    CB_CHECK_EQ(ev_S(f, "main.loop"), "num=72,seg2=1");
    CB_CHECK_EQ(ev_S(f, "next.loop"), "num=88,seg2=1");
}  );

CB_TEST("x86asm.eval.undefined_symbols", [] {
    EvFix f;
    // Not critical: an unknown, flagged as a forward reference.
    int fwref = 0;
    auto e = f.eval("1 + nowhere", false, &fwref);
    CB_CHECK_EQ(ev_show(*e), "unk=1");   // an unknown swallows what it is added to
    CB_CHECK_EQ(fwref, OPFLAG_FORWARD);
    CB_CHECK(is_just_unknown(*e));
    // Critical: an error, in the first pass saying "before use".
    CB_CHECK_EQ(ev_S(f, "nowhere", true), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: symbol `nowhere' not defined before use");
    // In later passes it says only "not defined".
    f.as.error_pass_end();
    f.as.begin_pass();
    f.as.error_pass_end();
    f.as.begin_pass();
    f.as.location.known = true;
    f.as.location.segment = 2;
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "nowhere", true), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: symbol `nowhere' not defined");
    // A local label names its scope.
    f.labels.define_label("main", 2, 0x40, true);
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, ".missing", true), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: symbol `main.missing' not defined");
    // Forward references may be compared and combined: unknown stays unknown.
    CB_CHECK_EQ(ev_S(f, "nowhere == 3"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "nowhere * 3"), "unk=3");
    CB_CHECK_EQ(ev_S(f, "nowhere & 3"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "nowhere / 3"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "~nowhere"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "1 ? nowhere : 2"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "nowhere ? 1 : 2"), "unk=1");
    // The segment of an unknown is unknown, and critical makes that an error.
    CB_CHECK_EQ(ev_S(f, "seg nowhere"), "unk=1");
});

CB_TEST("x86asm.eval.externs", [] {
    EvFix f;
    CB_CHECK(f.labels.declare_label("ext", LBL_EXTERN));
    f.labels.define_label("ext", 0, 0, true);   // a pseudo-segment of its own
    int fwref = 0;
    const auto e = f.eval("ext + 4", false, &fwref);
    CB_CHECK_EQ(ev_show(*e), "num=4,seg100=1");
    CB_CHECK_EQ(fwref, OPFLAG_EXTERN);
});

CB_TEST("x86asm.eval.registers_and_hints", [] {
    EvFix f;
    // [eax*2+ebx]: the first register seen is hinted as the base; being multiplied makes the hint "not base".
    EvalHints h;
    auto e = f.eval("eax*2+ebx", false, nullptr, &h);
    CB_CHECK_EQ(ev_show(*e), "reg" + std::to_string(R_EAX) + "=2,reg" + std::to_string(R_EBX) + "=1");
    CB_CHECK_EQ(h.type, int{EAH_NOTBASE});
    e = f.eval("ebx+eax", false, nullptr, &h);
    CB_CHECK_EQ(h.type, int{EAH_MAKEBASE});
    CB_CHECK_EQ(h.base, std::int64_t{R_EBX});
    // The terms are in ascending order of type whatever the order written.
    CB_CHECK_EQ(ev_show(*e), "reg" + std::to_string(R_EAX) + "=1,reg" + std::to_string(R_EBX) + "=1");
    // [eax+eax]: summed.
    e = f.eval("eax+eax", false, nullptr, &h);
    CB_CHECK_EQ(ev_show(*e), "reg" + std::to_string(R_EAX) + "=2");
    CB_CHECK_EQ(h.type, int{EAH_SUMMED});
    // A register with a displacement.
    e = f.eval("esi+4", false, nullptr, &h);
    CB_CHECK_EQ(ev_show(*e), "reg" + std::to_string(R_ESI) + "=1,num=4");
    CB_CHECK_EQ(h.base, std::int64_t{R_ESI});
    // Registers are not numbers.
    CB_CHECK_EQ(ev_N(f, "eax & 1"), "0");   // reported, and the evaluation goes on
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: `&' operator may only be applied to scalar values");
    // eax*ebx cannot be made.
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "eax*ebx"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: unable to multiply two non-scalar objects");
});

CB_TEST("x86asm.eval.non_scalar_errors", [] {
    EvFix f;
    f.labels.define_label("lab", 2, 0x20, true);
    f.labels.define_label("oth", 4, 8, true);
    // Operators that need numbers, given labels. (nasm reports and goes on for most of these.)
    const struct {
        const char* text;
        const char* message;
    } errs[] = {
        {"~lab", "`~' operator may only be applied to scalar values"},
        {"!lab", "`!' operator may only be applied to scalar values"},
        {"__?ilog2f?__(lab)", "function may only be applied to scalar values"},
        {"lab / 2", "division operator may only be applied to scalar values"},
        {"lab * lab", "unable to multiply two non-scalar objects"},
        {"lab < oth", "`<': operands differ by a non-scalar"},
        {"lab <= oth", "`<=': operands differ by a non-scalar"},
        {"lab > oth", "`>': operands differ by a non-scalar"},
        {"lab >= oth", "`>=': operands differ by a non-scalar"},
        {"lab <=> oth", "`<=>': operands differ by a non-scalar"},
        {"lab ? 1 : 2", "the left-hand side of `?' must be a scalar value"},
        {"lab | 1", "`|' operator may only be applied to scalar values"},
        {"lab ^ 1", "`^' operator may only be applied to scalar values"},
        {"lab << 1", "shift operator may only be applied to scalar values"},
        {"lab || 1", "`|' operator may only be applied to scalar values"},
        {"lab && 1", "`&' operator may only be applied to scalar values"},
        {"lab ^^ 1", "`^' operator may only be applied to scalar values"},
    };
    for (const auto& e : errs) {
        f.as.clear_diagnostics();
        f.eval(e.text);
        CB_CHECK_EQ(f.last_error(), std::string("t.asm:1: error: ") + e.message);
    }
    // But equality of two labels of different segments is simply false.
    CB_CHECK_EQ(ev_N(f, "lab == oth"), "0");
    CB_CHECK_EQ(ev_N(f, "lab != oth"), "1");
});

CB_TEST("x86asm.eval.seg_and_wrt", [] {
    EvFix f;
    f.labels.define_label("lab", 2, 0x20, true);
    f.labels.define_label("odd", 3, 0x10, true);
    f.labels.define_label("absl", SEG_ABS | 0x1000, 0x30, true);
    // With a format that has no segment bases (bin), SEG is unknown, and critical makes that an error.
    f.ev.segbase = [](std::int32_t) { return NO_SEG; };
    CB_CHECK_EQ(ev_S(f, "seg lab"), "unk=1");
    CB_CHECK_EQ(ev_S(f, "seg lab", true), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: unable to determine segment base");
    // With one that has (the base of segment s is segment s+1; the evaluator asks for it by that number).
    f.as.clear_diagnostics();
    f.ev.segbase = [](std::int32_t s) { return s; };
    CB_CHECK_EQ(ev_S(f, "seg lab"), "seg3=1");
    // SEG of an absolute-segment label is a number: the segment value.
    CB_CHECK_EQ(ev_S(f, "seg absl"), "num=4096");
    // SEG of a plain number or a register is an error.
    CB_CHECK_EQ(ev_S(f, "seg 5"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: cannot apply SEG to a non-relocatable value");
    CB_CHECK_EQ(ev_S(f, "seg eax"), "ERR");
    // SEG of something already a segment base: an error in NASM.
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "seg odd"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: SEG applied to something which is already a segment base");
    // ... and the base itself in MASM mode.
    f.as.masm_mode = true;
    CB_CHECK_EQ(ev_S(f, "seg odd"), "num=16,seg3=1");
    f.as.masm_mode = false;
    // WRT adds the segment the value is relative to.
    f.labels.define_label("sect", 6, 0, true);
    CB_CHECK_EQ(ev_S(f, "lab wrt sect"), "num=32,wrt=6,seg2=1");
    CB_CHECK_EQ(ev_S(f, "lab wrt 0x10"), "num=32,wrt=" + std::to_string(0x10 | SEG_ABS) + ",seg2=1");
    CB_CHECK_EQ(ev_S(f, "lab wrt nowhere"), "unk=1");
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "lab wrt eax"), "ERR");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: invalid right-hand operand to WRT");
});

CB_TEST("x86asm.eval.masm_mode", [] {
    EvFix f;
    f.as.masm_mode = true;
    f.labels.define_label("Foo", 2, 0x20, true);
    // OFFSET inside an expression is a no-op prefix in --masm mode; elsewhere it is an identifier.
    CB_CHECK_EQ(ev_S(f, "offset Foo + 1"), "num=33,seg2=1");
    CB_CHECK_EQ(ev_S(f, "3 + OFFSET Foo"), "num=35,seg2=1");
    CB_CHECK_EQ(ev_S(f, "FOO"), "num=32,seg2=1");   // names match without regard to case
    // An undefined symbol in a conditional-assembly expression is 0.
    CB_CHECK_EQ(ev_S(f, "nowhere", true), "ERR");
    f.as.masm_if_undef_zero = true;
    CB_CHECK_EQ(ev_S(f, "nowhere", true), "num=0");
    CB_CHECK_EQ(ev_S(f, "nowhere + 5", true), "num=5");
});

CB_TEST("x86asm.eval.preprocess_only", [] {
    EvFix f;
    f.as.location.known = false;
    CB_CHECK_EQ(ev_S(f, "foo"), "unk=1");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: symbol references not supported in preprocess-only mode");
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "$"), "unk=1");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: `$' not supported in preprocess-only mode");
    f.as.clear_diagnostics();
    CB_CHECK_EQ(ev_S(f, "$$"), "unk=1");
    CB_CHECK_EQ(f.last_error(), "t.asm:1: error: `$$' not supported in preprocess-only mode");
    CB_CHECK_EQ(ev_N(f, "1+2"), "3");   // numbers still work
});

CB_TEST("x86asm.eval.decorators_and_pushed_token", [] {
    EvFix f;
    // A decorator token is an expression term of its own kind.
    CB_CHECK_EQ(ev_S(f, "{rn-sae}"), "rdsae=" + std::to_string(BRC_RN));
    // The evaluator starts with the token it is given when its type is not INVALID.
    f.sc.reset(" + 2");
    TokenVal tv;
    tv.type = TokenType::NUM;
    tv.integer = 5;
    const EvalScanFn fn = [&f](TokenVal& t) { return static_cast<TokenType>(f.sc.scan(t)); };
    const auto e = f.ev.evaluate(fn, tv, nullptr, false, nullptr);
    CB_CHECK_EQ(ev_show(*e), "num=7");
});

CB_TEST("x86asm.exprlib.classification", [] {
    const std::int32_t cur = 2;   // the segment being assembled into
    const Expr zero;
    CB_CHECK(is_simple(zero) && is_really_simple(zero));
    CB_CHECK_EQ(static_cast<int>(expr_class(zero, cur)), static_cast<int>(EC_ZERO));
    CB_CHECK_EQ(reloc_value(zero), std::int64_t{0});
    CB_CHECK_EQ(reloc_seg(zero), NO_SEG);
    CB_CHECK_EQ(reloc_wrt(zero), NO_SEG);

    const Expr num = ev_ex({{EXPR_SIMPLE, 5}});
    CB_CHECK(is_simple(num) && is_really_simple(num) && is_reloc(num, cur));
    CB_CHECK_EQ(static_cast<int>(expr_class(num, cur)), static_cast<int>(EC_CONST));
    CB_CHECK_EQ(reloc_value(num), std::int64_t{5});

    // Terms of value 0 do not count.
    const Expr zeros = ev_ex({{EXPR_UNKNOWN, 0}, {EXPR_SIMPLE, 7}, {EXPR_SEGBASE + 2, 0}});
    CB_CHECK(is_simple(zeros) && is_really_simple(zeros));
    CB_CHECK_EQ(reloc_value(zeros), std::int64_t{7});
    CB_CHECK(!is_unknown(zeros));

    const Expr label = ev_ex({{EXPR_SIMPLE, 0x20}, {EXPR_SEGBASE + 2, 1}});
    CB_CHECK(!is_simple(label) && is_reloc(label, cur));
    CB_CHECK_EQ(static_cast<int>(expr_class(label, cur)), static_cast<int>(EC_CONST | EC_SEG));
    CB_CHECK_EQ(reloc_seg(label), std::int32_t{2});
    CB_CHECK_EQ(reloc_value(label), std::int64_t{0x20});
    CB_CHECK(!is_self_relative(label, cur));

    // The current segment subtracted once: self-relative. Another segment subtracted: complex.
    const Expr rel = ev_ex({{EXPR_SIMPLE, 4}, {EXPR_SEGBASE + 2, -1}});
    CB_CHECK(is_self_relative(rel, cur) && is_reloc(rel, cur));
    CB_CHECK(!is_reloc(rel, 4));
    CB_CHECK_EQ(static_cast<int>(expr_class(rel, 4)), static_cast<int>(EC_CONST | EC_COMPLEX | EC_SELFREL));
    // A segment twice (value 2): complex.
    CB_CHECK(!is_reloc(ev_ex({{EXPR_SEGBASE + 2, 2}}), cur));
    // Two segment bases added: complex.
    CB_CHECK(!is_reloc(ev_ex({{EXPR_SEGBASE + 2, 1}, {EXPR_SEGBASE + 4, 1}}), cur));

    // A far-absolute: simple, but not really simple.
    const Expr farabs = ev_ex({{EXPR_SIMPLE, 1}, {EXPR_SEGBASE + (SEG_ABS | 5), 1}});
    CB_CHECK(is_simple(farabs) && !is_really_simple(farabs));
    CB_CHECK_EQ(static_cast<int>(expr_class(farabs, cur)), static_cast<int>(EC_CONST | EC_SEGABS));

    // WRT.
    const Expr wrt = ev_ex({{EXPR_SIMPLE, 1}, {EXPR_WRT, 6}, {EXPR_SEGBASE + 2, 1}});
    CB_CHECK(is_reloc(wrt, cur));
    CB_CHECK_EQ(reloc_wrt(wrt), std::int32_t{6});
    CB_CHECK_EQ(reloc_wrt(label), NO_SEG);
    CB_CHECK_EQ(static_cast<int>(expr_class(wrt, cur)), static_cast<int>(EC_CONST | EC_SEG | EC_WRT));

    // Registers.
    CB_CHECK_EQ(static_cast<int>(expr_class(ev_ex({{R_EAX, 1}}), cur)), static_cast<int>(EC_REGISTER));
    CB_CHECK_EQ(static_cast<int>(expr_class(ev_ex({{R_EAX, 2}}), cur)), static_cast<int>(EC_REGEXPR));
    CB_CHECK_EQ(static_cast<int>(expr_class(ev_ex({{R_EAX, 1}, {R_EBX, 1}}), cur)), static_cast<int>(EC_REGISTER | EC_REGEXPR));
    CB_CHECK(!is_reloc(ev_ex({{R_EAX, 1}}), cur) && !is_simple(ev_ex({{R_EAX, 1}})));

    // Unknowns.
    const Expr unk = ev_ex({{EXPR_UNKNOWN, 1}});
    CB_CHECK(is_unknown(unk) && is_just_unknown(unk) && !is_simple(unk));
    CB_CHECK(is_unknown(ev_ex({{EXPR_UNKNOWN, 1}, {EXPR_SIMPLE, 2}})) && !is_just_unknown(ev_ex({{EXPR_UNKNOWN, 1}, {EXPR_SIMPLE, 2}})));

    // The overloads taking the assembly use its location.
    Assembly as;
    as.location.segment = 2;
    CB_CHECK(is_self_relative(as, rel) && is_reloc(as, label));
    as.location.segment = 4;
    CB_CHECK(is_self_relative(as, rel) && !is_reloc(as, rel));   // subtracting another segment's base is not relocatable
    CB_CHECK_EQ(static_cast<int>(expr_class(as, rel)), static_cast<int>(EC_CONST | EC_COMPLEX | EC_SELFREL));
});
