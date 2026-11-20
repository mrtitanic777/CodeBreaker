// Tests of the standard scanner (scan.hpp), readnum, and the keyword table (tokhash.hpp). The numbers' values and the error
// and warning texts are those of nasm.exe.
#include "cb/build/x86asm/scan.hpp"

#include <string>
#include <vector>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/tokhash.hpp"
#include "cb/core/testing.hpp"

namespace {
using namespace cb::build::x86asm;

constexpr int st_T(TokenType t) { return token_value(t); }

struct StTok {
    int type = 0;
    std::int64_t integer = 0;
    std::int64_t inttwo = 0;
    std::string text;
    int flag = 0;
    std::string start;
};

// Every token of a line, up to and including the end of the line.
std::vector<StTok> st_scan_all(Scanner& sc, std::string_view line) {
    sc.reset(line);
    std::vector<StTok> out;
    for (int n = 0; n < 200; ++n) {
        TokenVal tv;
        StTok t;
        t.type = sc.scan(tv);
        t.integer = tv.integer;
        t.inttwo = tv.inttwo;
        t.text = tv.text;
        t.flag = tv.flag;
        t.start = std::string(tv.start);
        out.push_back(t);
        if (t.type == st_T(TokenType::EOS) || t.type == st_T(TokenType::INVALID)) break;
    }
    return out;
}

// The types of the tokens of a line, as one string for a quick comparison: numbers and identifiers by name.
std::string st_shape(Scanner& sc, std::string_view line) {
    std::string out;
    for (const StTok& t : st_scan_all(sc, line)) {
        if (!out.empty()) out += ' ';
        switch (static_cast<TokenType>(t.type)) {
        case TokenType::NUM: out += "NUM"; break;
        case TokenType::ERRNUM: out += "ERRNUM"; break;
        case TokenType::FLOAT: out += "FLOAT"; break;
        case TokenType::STR: out += "STR"; break;
        case TokenType::ERRSTR: out += "ERRSTR"; break;
        case TokenType::ID: out += "ID"; break;
        case TokenType::HERE: out += "HERE"; break;
        case TokenType::BASE: out += "BASE"; break;
        case TokenType::EOS: out += "EOS"; break;
        case TokenType::REG: out += "REG"; break;
        case TokenType::INSN: out += "INSN"; break;
        case TokenType::SHL: out += "SHL"; break;
        case TokenType::SHR: out += "SHR"; break;
        case TokenType::SAR: out += "SAR"; break;
        case TokenType::SDIV: out += "SDIV"; break;
        case TokenType::SMOD: out += "SMOD"; break;
        case TokenType::GE: out += "GE"; break;
        case TokenType::LE: out += "LE"; break;
        case TokenType::NE: out += "NE"; break;
        case TokenType::LEG: out += "LEG"; break;
        case TokenType::DBL_AND: out += "DBLAND"; break;
        case TokenType::DBL_OR: out += "DBLOR"; break;
        case TokenType::DBL_XOR: out += "DBLXOR"; break;
        default:
            if (t.type > 0 && t.type < 256) out += static_cast<char>(t.type);
            else out += "?" + std::to_string(t.type);
        }
    }
    return out;
}
}  // namespace

CB_TEST("x86asm.scan.numbers_in_every_form", [] {
    // nasm: dq 0x1F, 0X1f, 1Fh, 0h1F, 0b101, 101b, 0y11, 11y, 0o17, 17o, 0q17, 17q, 0d99, 99d, 99t, 1_000, 0xdead_beef,
    // 0b1111_0000
    struct N {
        const char* text;
        std::int64_t value;
    };
    static const N nums[] = {
        {"0x1F", 0x1f}, {"0X1f", 0x1f}, {"1Fh", 0x1f}, {"0h1F", 0x1f}, {"0b101", 5}, {"101b", 5}, {"0y11", 3}, {"11y", 3},
        {"0o17", 15}, {"17o", 15}, {"0q17", 15}, {"17q", 15}, {"0d99", 99}, {"99d", 99}, {"99t", 99}, {"1_000", 1000},
        {"0xdead_beef", 0xdeadbeefLL}, {"0b1111_0000", 240}, {"0", 0}, {"007", 7}, {"1h", 1}, {"0x0", 0}, {"123456789", 123456789},
        {"0xffffffffffffffff", -1}, {"18446744073709551615", -1}, {"9223372036854775807", INT64_MAX},
        {"9223372036854775808", INT64_MIN}, {"0xFFFFFFFF", 0xffffffffLL}, {"0abch", 0xabc},
    };
    Assembly as;
    Scanner sc(as);
    for (const N& n : nums) {
        const auto toks = st_scan_all(sc, n.text);
        CB_CHECK_EQ(toks.size(), std::size_t{2});
        CB_CHECK_EQ(toks[0].type, st_T(TokenType::NUM));
        if (toks[0].integer != n.value) CB_CHECK_EQ(std::string(n.text) + "=" + std::to_string(toks[0].integer), std::string(n.text) + "=" + std::to_string(n.value));
        CB_CHECK_EQ(toks[0].start, std::string(n.text));
    }
    CB_CHECK(as.diagnostics().empty());
});

CB_TEST("x86asm.scan.dollar_hex", [] {
    // Without the option, `$1F` is the identifier 1F escaped by a dollar sign.
    Assembly as;
    Scanner sc(as);
    as.globl.dollarhex = false;
    auto toks = st_scan_all(sc, "$1F");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::ID));
    CB_CHECK_EQ(toks[0].text, "1F");
    // nasm: dq $1F -> 0x1f and "$ prefix for hexadecimal is deprecated".
    as.globl.dollarhex = true;
    as.src.set(1, "t.asm");
    toks = st_scan_all(sc, "$1F + $");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::NUM));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{0x1f});
    CB_CHECK_EQ(toks[2].type, st_T(TokenType::HERE));
    CB_CHECK_EQ(as.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:1: warning: $ prefix for hexadecimal is deprecated [-w+number-deprecated-hex]");
});

CB_TEST("x86asm.scan.number_errors_and_overflow", [] {
    Assembly as;
    Scanner sc(as);
    CB_CHECK_EQ(st_shape(sc, "12abc"), "ERRNUM EOS");
    CB_CHECK_EQ(st_shape(sc, "0b102"), "ERRNUM EOS");
    CB_CHECK_EQ(st_shape(sc, "1__"), "NUM EOS");
    // nasm: dq 18446744073709551616 -> a warning, and the value wraps (to 0).
    as.src.set(1, "t.asm");
    auto toks = st_scan_all(sc, "18446744073709551616");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::NUM));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{0});
    CB_CHECK_EQ(as.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:1: warning: numeric constant 18446744073709551616 does not fit in 64 bits [-w+number-overflow]");
    as.clear_diagnostics();
    // nasm: dq 0x1ffffffffffffffff
    toks = st_scan_all(sc, "0x1ffffffffffffffff");
    CB_CHECK_EQ(toks[0].integer, std::int64_t{-1});
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:1: warning: numeric constant 0x1ffffffffffffffff does not fit in 64 bits [-w+number-overflow]");
});

CB_TEST("x86asm.scan.readnum", [] {
    Assembly as;
    bool err = true;
    CB_CHECK_EQ(readnum(as, "0x10", err), std::int64_t{16});
    CB_CHECK(!err);
    CB_CHECK_EQ(readnum(as, "  -0x10", err), std::int64_t{-16});   // a leading minus (from %assign) and spaces are accepted
    CB_CHECK(!err);
    CB_CHECK_EQ(readnum(as, "1_0b", err), std::int64_t{2});
    readnum(as, "", err);
    CB_CHECK(err);
    readnum(as, "xyz!", err);
    CB_CHECK(err);
    readnum(as, "0x", err);   // `x` as a suffix of a one-digit number: hexadecimal 0
    CB_CHECK(!err);
    as.globl.dollarhex = true;
    CB_CHECK_EQ(readnum(as, "$ff", err), std::int64_t{255});
    CB_CHECK(!err);
});

CB_TEST("x86asm.scan.floats", [] {
    Assembly as;
    Scanner sc(as);
    struct F {
        const char* text;
        bool is_float;
    };
    static const F fs[] = {{"1.5", true},   {"1e10", true},     {"1e+5", true},   {"1E-5", true},     {"0x1.8p3", true}, {"0x1p-2", true},
                           {"1.e3", true},  {"12.", true},      {"0.5e1", true},  {"1p3", true},      {"1_0.5_0", true}, {"1e10h", false},
                           {"1ex", false},  {"0xe+1", false},   {"0b1.1p1", true}};
    for (const F& f : fs) {
        const auto toks = st_scan_all(sc, f.text);
        if (f.is_float) {
            CB_CHECK_EQ(toks[0].type, st_T(TokenType::FLOAT));
            CB_CHECK_EQ(toks[0].text, std::string(f.text));
        } else {
            CB_CHECK_EQ(toks[0].type, st_T(TokenType::NUM));
        }
    }
    // `0xe+1` is the number 0xe, then + and 1 (the sign belongs to a float or to an exponent of a prefixed hex number only).
    CB_CHECK_EQ(st_shape(sc, "0xe+1"), "NUM + NUM EOS");
    CB_CHECK_EQ(st_shape(sc, "1e+1"), "FLOAT EOS");
    CB_CHECK_EQ(st_scan_all(sc, "1e10h")[0].integer, std::int64_t{0x1e10});
    CB_CHECK_EQ(st_shape(sc, ".5"), "ID EOS");   // nasm: dd .5 -> "symbol `.5' not defined"
});

CB_TEST("x86asm.scan.operators", [] {
    Assembly as;
    Scanner sc(as);
    CB_CHECK_EQ(st_shape(sc, ">>> >> <<< << // %% == = <> != <=> <= >= && ^^ || < > & | ^"), "SAR SHR SHL SHL SDIV SMOD = = NE NE LEG LE GE DBLAND DBLXOR DBLOR < > & | ^ EOS");
    CB_CHECK_EQ(st_shape(sc, "+-*/%()[],?:!~#"), "+ - * / % ( ) [ ] , ? : ! ~ # EOS");
    CB_CHECK_EQ(st_shape(sc, "a>>b"), "ID SHR ID EOS");
    CB_CHECK_EQ(st_shape(sc, "1<=>2"), "NUM LEG NUM EOS");
    CB_CHECK_EQ(st_shape(sc, "<>=>"), "NE = > EOS");
    const auto toks = st_scan_all(sc, "==");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::EQ));
    CB_CHECK_EQ(st_T(TokenType::EQ), int{'='});   // a single = is the same token
});

CB_TEST("x86asm.scan.strings", [] {
    Assembly as;
    Scanner sc(as);
    // nasm: db 'ab''cd' is an error: a quote mark does not double; the scanner gives two strings.
    auto toks = st_scan_all(sc, "'ab''cd'");
    CB_CHECK_EQ(toks.size(), std::size_t{3});
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::STR));
    CB_CHECK_EQ(toks[0].text, "ab");
    CB_CHECK_EQ(toks[0].inttwo, std::int64_t{2});
    CB_CHECK_EQ(toks[1].text, "cd");
    CB_CHECK_EQ(toks[0].start, "'ab'");
    // Backquoted escapes.
    toks = st_scan_all(sc, "`a\\n\\x41\\0z`, 1");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::STR));
    CB_CHECK_EQ(toks[0].text, std::string("a\nA\0z", 5));
    CB_CHECK_EQ(toks[0].inttwo, std::int64_t{5});
    CB_CHECK_EQ(toks[1].type, int{','});
    CB_CHECK_EQ(toks[2].type, st_T(TokenType::NUM));
    // An unterminated string.
    toks = st_scan_all(sc, "\"abc");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::ERRSTR));
    CB_CHECK_EQ(toks[0].text, "abc");
    CB_CHECK_EQ(toks[1].type, st_T(TokenType::EOS));
    toks = st_scan_all(sc, "`abc\\`");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::ERRSTR));
    // An empty string, and a semicolon inside a string is not a comment.
    toks = st_scan_all(sc, "'' ';' ; real comment");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::STR));
    CB_CHECK_EQ(toks[0].inttwo, std::int64_t{0});
    CB_CHECK_EQ(toks[1].text, ";");
    CB_CHECK_EQ(toks[2].type, st_T(TokenType::EOS));
});

CB_TEST("x86asm.scan.identifiers_and_here", [] {
    Assembly as;
    Scanner sc(as);
    auto toks = st_scan_all(sc, ".loop ..@12.x foo$bar a.b ?x @@y _z #q");
    // `#q` is a character and then an identifier.
    CB_CHECK_EQ(toks[0].text, ".loop");
    CB_CHECK_EQ(toks[1].text, "..@12.x");
    CB_CHECK_EQ(toks[2].text, "foo$bar");
    CB_CHECK_EQ(toks[3].text, "a.b");
    CB_CHECK_EQ(toks[4].text, "?x");
    CB_CHECK_EQ(toks[5].text, "@@y");
    CB_CHECK_EQ(toks[6].text, "_z");
    CB_CHECK_EQ(toks[7].type, int{'#'});
    CB_CHECK_EQ(toks[8].text, "q");
    // `$` is the current location, `$$` the segment base, `$name` an identifier escaped from the keywords.
    CB_CHECK_EQ(st_shape(sc, "$ $$ $foo $mov $$x $-$$"), "HERE BASE ID ID ID HERE - BASE EOS");
    toks = st_scan_all(sc, "$mov $$x");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::ID));
    CB_CHECK_EQ(toks[0].text, "mov");
    CB_CHECK_EQ(toks[0].start, "$mov");
    CB_CHECK_EQ(toks[1].text, "$x");
    // An identifier is cut at IDLEN_MAX-1 characters.
    const std::string longid(5000, 'q');
    toks = st_scan_all(sc, longid);
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::ID));
    CB_CHECK_EQ(toks[0].text.size(), std::size_t{4095});
    CB_CHECK_EQ(toks[0].start.size(), std::size_t{5000});
    // UTF-8 letters are identifier characters.
    CB_CHECK_EQ(st_shape(sc, "caf\xc3\xa9"), "ID EOS");
    // Comments.
    CB_CHECK_EQ(st_shape(sc, "a ; b c"), "ID EOS");
    CB_CHECK_EQ(st_shape(sc, "   "), "EOS");
    CB_CHECK_EQ(st_shape(sc, ""), "EOS");
});

CB_TEST("x86asm.scan.keywords", [] {
    Assembly as;
    Scanner sc(as);
    auto toks = st_scan_all(sc, "mov EAX, byte [seg]");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::INSN));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{I_MOV});
    CB_CHECK_EQ(toks[1].type, st_T(TokenType::REG));
    CB_CHECK_EQ(toks[1].integer, std::int64_t{R_EAX});
    CB_CHECK_EQ(toks[1].text, "EAX");   // the text keeps its case
    CB_CHECK_EQ(toks[3].type, st_T(TokenType::SIZE));
    CB_CHECK_EQ(toks[3].integer, std::int64_t{S_BYTE});
    CB_CHECK_EQ(toks[3].inttwo, std::int64_t{1});
    CB_CHECK_EQ(toks[5].type, st_T(TokenType::SEG));
    toks = st_scan_all(sc, "lock WRT times STRICT near __?float32?__ __?UTF16?__ __?ilog2c?__ __?infinity?__ dup");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::PREFIX));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{P_LOCK});
    CB_CHECK_EQ(toks[0].inttwo, std::int64_t{PPS_LOCK});
    CB_CHECK_EQ(toks[1].type, st_T(TokenType::WRT));
    CB_CHECK_EQ(toks[2].type, st_T(TokenType::TIMES));
    CB_CHECK_EQ(toks[3].type, st_T(TokenType::SPECIAL));
    CB_CHECK_EQ(toks[3].integer, std::int64_t{S_STRICT});
    CB_CHECK_EQ(toks[4].type, st_T(TokenType::SPECIAL));
    CB_CHECK_EQ(toks[5].type, st_T(TokenType::FLOATIZE));
    CB_CHECK_EQ(toks[5].integer, std::int64_t{FLOAT_32});
    CB_CHECK_EQ(toks[6].type, st_T(TokenType::STRFUNC));
    CB_CHECK_EQ(toks[6].integer, std::int64_t{STRFUNC_UTF16});
    CB_CHECK_EQ(toks[7].type, st_T(TokenType::IFUNC));
    CB_CHECK_EQ(toks[7].integer, std::int64_t{IFUNC_ILOG2C});
    CB_CHECK_EQ(toks[8].type, st_T(TokenType::FLOAT));
    CB_CHECK_EQ(toks[9].type, st_T(TokenType::ID));
    CB_CHECK_EQ(toks[9].flag, int{TFLAG_DUP});
    // A keyword that is only valid inside braces is an ordinary identifier outside them.
    toks = st_scan_all(sc, "z sae rex evex nf");
    for (int i = 0; i < 5; ++i) CB_CHECK_EQ(toks[static_cast<std::size_t>(i)].type, st_T(TokenType::ID));
    // The opmask registers are registers, k0 too.
    CB_CHECK_EQ(st_shape(sc, "k0 k1 k7 k8"), "REG REG REG ID EOS");
    // A word longer than the longest keyword is never one.
    CB_CHECK_EQ(st_shape(sc, "movmovmovmovmovmovmov"), "ID EOS");
});

CB_TEST("x86asm.scan.ptr_warning", [] {
    Assembly as;
    Scanner sc(as);
    as.src.set(1, "t.asm");
    CB_CHECK_EQ(st_shape(sc, "ptr"), "ID EOS");
    CB_CHECK(as.set_warning_status("+ptr"));
    st_scan_all(sc, "dword ptr");
    bool found = false;
    for (const Diagnostic& d : as.diagnostics()) found |= d.text == "t.asm:1: warning: `ptr' is not a NASM keyword [-w+ptr]";
    CB_CHECK(found);
});

CB_TEST("x86asm.scan.masm_register_names", [] {
    // --masm: 64-bit registers and dr8+/cr9+ are ordinary identifiers (labels); the rest stay registers.
    Assembly nasm_mode;
    Scanner n(nasm_mode);
    CB_CHECK_EQ(st_shape(n, "rax r8 dr8 cr9 eax dr7 cr8 cr0"), "REG REG REG REG REG REG REG REG EOS");

    Assembly masm;
    masm.masm_mode = true;
    Scanner m(masm);
    CB_CHECK_EQ(st_shape(m, "rax r8 dr8 cr9 eax dr7 cr8 cr0 dr31 cr31"), "ID ID ID ID REG REG REG REG ID ID EOS");
    const auto toks = st_scan_all(m, "rax");
    CB_CHECK_EQ(toks[0].text, "rax");
});

CB_TEST("x86asm.scan.braces", [] {
    Assembly as;
    Scanner sc(as);
    as.src.set(1, "t.asm");
    auto toks = st_scan_all(sc, "{rn-sae} {1to8} {z} { sae } {k1}");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::DECORATOR));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{BRC_RN});
    CB_CHECK_EQ(toks[0].text, "{rn-sae}");
    CB_CHECK_EQ(toks[1].type, st_T(TokenType::DECORATOR));
    CB_CHECK_EQ(toks[1].integer, std::int64_t{BRC_1TO8});
    CB_CHECK_EQ(toks[2].integer, std::int64_t{BRC_Z});
    CB_CHECK_EQ(toks[3].integer, std::int64_t{BRC_SAE});
    CB_CHECK_EQ(toks[3].text, "{ sae }");   // the text is the whole thing, braces and spaces and all
    CB_CHECK_EQ(toks[4].type, st_T(TokenType::OPMASK));   // an opmask register within braces
    CB_CHECK_EQ(toks[4].integer, std::int64_t{R_K1});
    // {dfv=...}: the OR of the flags.
    toks = st_scan_all(sc, "{dfv=cf,zf,of}");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::BRCCONST));
    CB_CHECK_EQ(toks[0].integer, std::int64_t{DFV_CF | DFV_ZF | DFV_OF});
    CB_CHECK_EQ(toks[0].inttwo, std::int64_t{IF_DFV});
    CB_CHECK_EQ(toks[0].text, "{dfv=cf,zf,of}");
    // Errors.
    toks = st_scan_all(sc, "{bogus}");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::INVALID));
    CB_CHECK_EQ(as.diagnostics().back().text, "t.asm:1: error: `{bogus}' is not a valid brace token");
    toks = st_scan_all(sc, "{rn-sae");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::INVALID));
    CB_CHECK_EQ(as.diagnostics().back().text, "t.asm:1: error: unterminated braces at end of line");
    toks = st_scan_all(sc, "{rn-sae,z}");
    CB_CHECK_EQ(toks[0].type, st_T(TokenType::INVALID));
    CB_CHECK_EQ(as.diagnostics().back().text, "t.asm:1: error: invalid character `,' in brace sequence");
});

CB_TEST("x86asm.scan.pushback_and_state", [] {
    Assembly as;
    Scanner sc(as);
    sc.reset("a, b");
    TokenVal a, comma, b;
    sc.scan(a);
    const ScanState saved = sc.get();
    sc.scan(comma);
    CB_CHECK(comma.type == TokenType::COMMA);
    CB_CHECK_EQ(std::string(sc.tell()), " b");
    sc.pushback(comma);
    TokenVal again;
    CB_CHECK_EQ(sc.scan(again), int{','});
    CB_CHECK_EQ(sc.scan(b), st_T(TokenType::ID));
    CB_CHECK_EQ(b.text, "b");
    CB_CHECK_EQ(sc.scan(b), st_T(TokenType::EOS));
    sc.set(saved);   // back to just after `a`
    CB_CHECK_EQ(sc.scan(again), int{','});
    // reset drops pushed-back tokens.
    sc.pushback(a);
    sc.reset("x");
    CB_CHECK_EQ(sc.scan(again), st_T(TokenType::ID));
    CB_CHECK_EQ(again.text, "x");
});

CB_TEST("x86asm.tokhash.table", [] {
    TokenVal tv;
    CB_CHECK_EQ(token_hash("MoV", tv), st_T(TokenType::INSN));
    CB_CHECK_EQ(tv.integer, std::int64_t{I_MOV});
    CB_CHECK_EQ(token_hash("xmm17", tv), st_T(TokenType::REG));
    CB_CHECK_EQ(token_hash("r15", tv), st_T(TokenType::REG));
    CB_CHECK_EQ(token_hash("?", tv), st_T(TokenType::QMARK));
    CB_CHECK_EQ(token_hash("dfv=pf", tv), st_T(TokenType::BRCCONST));
    CB_CHECK_EQ(tv.integer, std::int64_t{DFV_CF});
    CB_CHECK(tv.flag & TFLAG_ORBIT);
    CB_CHECK_EQ(token_hash("rn-sae", tv), st_T(TokenType::DECORATOR));
    CB_CHECK_EQ(token_hash("__?masm_ptr?__", tv), st_T(TokenType::MASM_PTR));
    CB_CHECK_EQ(token_hash("__?masm_flat?__", tv), st_T(TokenType::MASM_FLAT));
    CB_CHECK_EQ(token_hash("<<<", tv), st_T(TokenType::SHL));
    CB_CHECK_EQ(token_hash("notakeyword", tv), st_T(TokenType::ID));
    CB_CHECK_EQ(tv.integer, std::int64_t{0});
    CB_CHECK_EQ(tv.flag, 0);
    CB_CHECK_EQ(token_hash("", tv), st_T(TokenType::ID));
    // Every instruction and register name is found, and finds itself.
    for (int i = 0; i <= I_HINT_NOP63; ++i) {
        CB_CHECK_EQ(token_hash(kInsnNames[static_cast<std::size_t>(i)], tv), st_T(TokenType::INSN));
        if (tv.integer != i) CB_CHECK_EQ(std::string(kInsnNames[static_cast<std::size_t>(i)]) + "=" + std::to_string(tv.integer), std::string(kInsnNames[static_cast<std::size_t>(i)]) + "=" + std::to_string(i));
    }
    for (int r = EXPR_REG_START; r < REG_ENUM_LIMIT; ++r) {
        CB_CHECK_EQ(token_hash(register_name(r), tv), st_T(TokenType::REG));
        CB_CHECK_EQ(tv.integer, std::int64_t{r});
    }
});
