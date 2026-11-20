// Tests of the x86asm encoder (encode.cpp): instructions in 16-, 32- and 64-bit modes encoded and compared with the bytes
// the real NASM 3.02 (native/nasm/build/bin/nasm.exe, `-f bin`) produced for the same line.
//
// The cases are in encode_cases.inc (a table of {bits, source line, expected bytes}); the expected bytes were captured from
// nasm.exe by docs/history/x86asm-generators/gen_encode_tests.py, which also lists the lines NASM rejects (expected
// "!", the encoder must then fail too). The parser is another module, so the test carries a small operand reader of its
// own (the NASM operand syntax: registers, numbers, `$`-relative jump targets, memory references with size words, segment
// overrides, `rel`/`abs`/`nosplit`/`byte`/`dword` inside the brackets, AVX-512 decorators) that builds the Insn exactly as
// NASM's parser would (parser.c: set_imm_flags, mref_set_optype, the size words and the optimization flags they set).
#include "cb/build/x86asm/encode.hpp"

#include <cctype>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/testing.hpp"

namespace encode_test_impl {
using namespace cb::build::x86asm;

// ---- a capturing output format ----

class Capture final : public OutputFormat {
public:
    std::vector<std::uint8_t> bytes;
    int records = 0;

    std::string_view fullname() const override { return "capture"; }
    std::string_view shortname() const override { return "bin"; }
    std::string_view extension() const override { return ""; }
    unsigned flags() const override { return 0; }
    int maxbits() const override { return 64; }
    const std::vector<std::string>& stdmac() const override {
        static const std::vector<std::string> none;
        return none;
    }
    void init() override {}
    void output(const OutData& d) override {
        ++records;
        switch (d.type) {
        case OUT_RAWDATA: {
            const auto* p = static_cast<const std::uint8_t*>(d.data);
            bytes.insert(bytes.end(), p, p + d.size);
            break;
        }
        case OUT_ZERODATA:
        case OUT_RESERVE: bytes.insert(bytes.end(), d.size, 0); break;
        default: bytes.insert(bytes.end(), d.size, 0xCC); break;   // a relocation: not a case of these tests
        }
    }
    void symdef(std::string_view, std::int32_t, std::int64_t, int, std::string_view) override {}
    std::int32_t section(std::optional<std::string_view>, int&) override { return 0; }
    void cleanup() override {}
};

// ---- the operand reader ----

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
std::string trim(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return std::string(s);
}

int find_reg(const std::string& name) {
    for (int r = EXPR_REG_START; r < REG_ENUM_LIMIT; ++r) {
        if (name == kRegNames[static_cast<std::size_t>(r - EXPR_REG_START)]) return r;
    }
    return 0;
}

Opcode find_opcode(const std::string& name) {
    for (std::size_t i = 0; i < kInsnNames.size(); ++i) {
        if (name == kInsnNames[i]) return static_cast<Opcode>(i);
    }
    return I_none;
}

// Size and operand-modifier words: the OpFlags bit each sets (0 for those handled by hand).
bool size_word(const std::string& w, OpFlags& bits) {
    if (w == "byte") bits = opf::BITS8;
    else if (w == "word") bits = opf::BITS16;
    else if (w == "dword" || w == "long") bits = opf::BITS32;
    else if (w == "qword") bits = opf::BITS64;
    else if (w == "tword") bits = opf::BITS80;
    else if (w == "oword") bits = opf::BITS128;
    else if (w == "yword") bits = opf::BITS256;
    else if (w == "zword") bits = opf::BITS512;
    else return false;
    return true;
}

struct Reader {
    std::string s;
    std::size_t p = 0;
    bool ok = true;
    bool uses_here = false;   // the expression used `$`
    std::int64_t here = 0;

    void skip() {
        while (p < s.size() && std::isspace(static_cast<unsigned char>(s[p]))) ++p;
    }
    bool eof() {
        skip();
        return p >= s.size();
    }
    std::int64_t primary() {
        skip();
        if (p >= s.size()) {
            ok = false;
            return 0;
        }
        const char c = s[p];
        if (c == '(') {
            ++p;
            const std::int64_t v = expr();
            skip();
            if (p < s.size() && s[p] == ')') ++p;
            else ok = false;
            return v;
        }
        if (c == '-') {
            ++p;
            return -primary();
        }
        if (c == '+') {
            ++p;
            return primary();
        }
        if (c == '~') {
            ++p;
            return ~primary();
        }
        if (c == '$') {
            ++p;
            uses_here = true;
            return here;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            std::size_t q = p;
            while (q < s.size() && std::isalnum(static_cast<unsigned char>(s[q]))) ++q;
            const std::string t = s.substr(p, q - p);
            p = q;
            return static_cast<std::int64_t>(std::strtoull(t.c_str(), nullptr, 0));
        }
        ok = false;
        return 0;
    }
    std::int64_t term() {
        std::int64_t v = primary();
        for (;;) {
            skip();
            if (p < s.size() && s[p] == '*') {
                ++p;
                v *= primary();
            } else {
                return v;
            }
        }
    }
    std::int64_t expr() {
        std::int64_t v = term();
        for (;;) {
            skip();
            if (p < s.size() && s[p] == '+') {
                ++p;
                v += term();
            } else if (p < s.size() && s[p] == '-') {
                ++p;
                v -= term();
            } else {
                return v;
            }
        }
    }
};

struct Builder {
    Insn ins;
    int bits = 16;
    bool ok = true;
    std::string why;

    void fail(std::string w) {
        if (ok) why = std::move(w);
        ok = false;
    }

    // NASM's set_imm_flags (parser.c).
    static OpFlags imm_flags(Operand& op, int opt) {
        const bool strict = (op.type & opf::STRICT_) || (opt & OPTIM_STRICT_OPER);
        const std::int64_t n = op.offset;
        if (!(op.type & opf::IMMEDIATE)) return op.type;
        if (op.opflags & OPFLAG_UNKNOWN) {
            if (!strict || !(op.type & SIZE_MASK)) op.type |= opf::UNITY | opf::FOURBITS;
            if (!strict) op.type |= opf::SBYTEDWORD | opf::SBYTEWORD | opf::UDWORD | opf::SDWORD;
            op.type |= opf::IMM_KNOWN;
            return op.type;
        }
        if (!(op.opflags & OPFLAG_SIMPLE)) return op.type;
        op.type |= opf::IMM_KNOWN;
        if (!strict || !(op.type & SIZE_MASK)) {
            if (n == 1) op.type |= opf::UNITY;
            if (n >= -16 && n <= 15) op.type |= opf::FOURBITS;
        }
        if (strict) return op.type;
        if (static_cast<std::int32_t>(n) == static_cast<std::int8_t>(n)) op.type |= opf::SBYTEDWORD;
        if (static_cast<std::int16_t>(n) == static_cast<std::int8_t>(n)) op.type |= opf::SBYTEWORD;
        if (static_cast<std::uint64_t>(n) == static_cast<std::uint32_t>(n)) op.type |= opf::UDWORD;
        if (static_cast<std::int64_t>(n) == static_cast<std::int32_t>(n)) op.type |= opf::SDWORD;
        return op.type;
    }

    // The memory reference inside the brackets (parser.c: parse_mref and mref_set_optype).
    void memory(Operand& op, std::string_view inside) {
        Reader r;
        r.s = std::string(inside);
        r.here = ins.loc.offset;
        int b = -1, i = -1, sc = 0, hintbase = -1;
        EvalHint hinttype = EAH_NOHINT;
        std::int64_t off = 0;
        bool have_here = false;
        // Leading words: nosplit, rel, abs, byte/word/dword/qword (displacement size), a16/a32/a64, `seg:`.
        for (;;) {
            r.skip();
            std::size_t q = r.p;
            while (q < r.s.size() && std::isalnum(static_cast<unsigned char>(r.s[q]))) ++q;
            const std::string w = lower(r.s.substr(r.p, q - r.p));
            OpFlags sz = 0;
            const int sreg = w.empty() ? 0 : find_reg(w);
            if (sreg && is_sreg(sreg) && q < r.s.size() && r.s[q] == ':') {
                ins.prefixes[PPS_SEG] = sreg;
                if (sreg == R_FS) op.eaflags |= EAF_FS;
                if (sreg == R_GS) op.eaflags |= EAF_GS;
                q++;
            } else if (w == "nosplit") op.eaflags |= EAF_TIMESTWO;
            else if (w == "rel") op.eaflags |= EAF_REL;
            else if (w == "abs") op.eaflags |= EAF_ABS;
            else if (w == "a16" || w == "a32" || w == "a64") ins.prefixes[PPS_ASIZE] = w == "a16" ? P_A16 : w == "a32" ? P_A32 : P_A64;
            else if (size_word(w, sz) && (q >= r.s.size() || r.s[q] == ' ')) {
                op.disp_size = sz == opf::BITS8 ? 8 : sz == opf::BITS16 ? 16 : sz == opf::BITS32 ? 32 : 64;
                op.eaflags |= sz == opf::BITS8 ? EAF_BYTEOFFS : EAF_WORDOFFS;
            } else break;
            r.p = q;
        }
        // The terms: +/- register, register*n, n*register, numbers, `$`.
        int sign = 1;
        bool more = true;
        while (more && !r.eof()) {
            r.skip();
            if (r.s[r.p] == '+') { ++r.p; sign = 1; continue; }
            if (r.s[r.p] == '-') { ++r.p; sign = -1; continue; }
            std::size_t q = r.p;
            while (q < r.s.size() && (std::isalnum(static_cast<unsigned char>(r.s[q])) || r.s[q] == '_')) ++q;
            const std::string w = lower(r.s.substr(r.p, q - r.p));
            const int reg = w.empty() ? 0 : find_reg(w);
            if (reg) {
                r.p = q;
                std::int64_t mult = 1;
                bool starred = false;
                r.skip();
                if (r.p < r.s.size() && r.s[r.p] == '*') {
                    ++r.p;
                    mult = r.primary();
                    starred = true;
                }
                mult *= sign;
                if (hinttype == EAH_NOHINT && hintbase == -1) {
                    hintbase = reg;
                    hinttype = starred ? EAH_NOTBASE : EAH_MAKEBASE;
                } else if (hintbase == reg) {
                    hinttype = EAH_SUMMED;
                }
                const bool is_gpr = is_class(opf::REG_GPR, kRegFlags[static_cast<std::size_t>(reg)]);
                if (is_gpr && mult == 1 && b == -1) b = reg;
                else if (i == -1) { i = reg; sc = static_cast<int>(mult); }
                else { fail("two index registers"); return; }
            } else {
                // A number or number*register.
                Reader n;
                n.s = r.s;
                n.p = r.p;
                n.here = r.here;
                std::int64_t v = n.term();
                r.p = n.p;
                if (!n.ok) { fail("bad memory expression"); return; }
                if (n.uses_here) have_here = true;
                off += sign * v;
            }
            sign = 1;
        }
        op.basereg = b;
        op.indexreg = i;
        op.scale = sc;
        op.offset = off;
        op.hintbase = hintbase;
        op.hinttype = hinttype;
        if (have_here) op.segment = ins.loc.segment;
        // mref_set_optype
        op.type |= opf::MEMORY_ANY;
        if (!(op.eaflags & (EAF_FS | EAF_GS))) op.eaflags |= EAF_NOTFSGS;
        if (b != -1) {
            op.type &= kRegFlags[static_cast<std::size_t>(b)] | ~opf::RN_L16;
        } else if (i == -1) {
            OpFlags flag = opf::MEM_OFFS;
            if (bits == 64) {
                if (op.eaflags & EAF_ABS) {
                } else if (op.eaflags & EAF_REL) {
                    flag = opf::IP_REL;
                }
            }
            op.type |= flag;
        }
        if (i != -1) {
            const OpFlags iclass = kRegFlags[static_cast<std::size_t>(i)];
            op.type &= iclass | ~opf::RN_L16;
            if (is_class(opf::XMMREG, iclass)) op.type |= opf::XMEM;
            else if (is_class(opf::YMMREG, iclass)) op.type |= opf::YMEM;
            else if (is_class(opf::ZMMREG, iclass)) op.type |= opf::ZMEM;
        }
        const OpFlags size = op.type & SIZE_MASK;
        if (!size || size == opf::BITS16) op.type |= opf::RM_SEL;
    }

    // A standalone `{rn-sae}`/`{sae}` after an operand belongs to the previous operand (parser.c: EXPR_RDSAE).
    void rounding_deco(Operand& prev, int prev_index, const std::string& text) {
        const std::string d = lower(trim(text));
        int code = 0;
        bool er = true;
        if (d == "{sae}") { code = BRC_SAE; er = false; }
        else if (d == "{rn-sae}") code = BRC_RN;
        else if (d == "{rd-sae}") code = BRC_RD;
        else if (d == "{ru-sae}") code = BRC_RU;
        else if (d == "{rz-sae}") code = BRC_RZ;
        else { fail("decorator " + d); return; }
        prev.decoflags |= static_cast<DecoFlags>(er ? deco::ER : deco::SAE);
        ins.evex_rm = code;
        ins.evex_brerop = prev_index;
        prev.bcast = true;
        prev.xsize = prev.decoflags & deco::BRSIZE_MASK;
    }

    // One operand's text. `colon` is set when the text ends with a colon (far pair) -- handled by the caller.
    Operand operand(std::string text, int index, bool far_ok) {
        (void)far_ok;
        Operand op;
        op.basereg = -1;
        op.indexreg = -1;
        op.segment = NO_SEG;
        op.wrt = NO_SEG;
        op.opidx = static_cast<std::uint8_t>(index);
        text = trim(text);

        // Decorators at the end: {k1} {z} {1to16} {rn-sae} ...
        DecoFlags deco_flags = 0;
        int rounding = 0;
        while (!text.empty() && text.back() == '}') {
            const std::size_t open = text.rfind('{');
            if (open == std::string::npos) break;
            const std::string d = lower(text.substr(open + 1, text.size() - open - 2));
            if (d == "z") deco_flags |= static_cast<DecoFlags>(deco::Z_MASK);
            else if (find_reg(d) && is_reg_class(opf::REG_CLASS_OPMASK, find_reg(d))) deco_flags |= static_cast<DecoFlags>(deco::val_opmask(static_cast<OpFlags>(kRegVals[static_cast<std::size_t>(find_reg(d))])));
            else if (d.rfind("1to", 0) == 0) {
                const int n = std::atoi(d.c_str() + 3);
                const int code = n == 2 ? 0 : n == 4 ? 1 : n == 8 ? 2 : n == 16 ? 3 : 4;
                deco_flags |= static_cast<DecoFlags>(deco::BRDCAST_MASK | deco::val_brnum(static_cast<OpFlags>(code)));
            } else if (d == "sae") { deco_flags |= static_cast<DecoFlags>(deco::SAE); rounding = BRC_SAE; }
            else if (d == "rn-sae") { deco_flags |= static_cast<DecoFlags>(deco::ER); rounding = BRC_RN; }
            else if (d == "rd-sae") { deco_flags |= static_cast<DecoFlags>(deco::ER); rounding = BRC_RD; }
            else if (d == "ru-sae") { deco_flags |= static_cast<DecoFlags>(deco::ER); rounding = BRC_RU; }
            else if (d == "rz-sae") { deco_flags |= static_cast<DecoFlags>(deco::ER); rounding = BRC_RZ; }
            else { fail("decorator " + d); return op; }
            text = trim(text.substr(0, open));
        }

        // Leading words: sizes and modifiers, a segment override.
        bool setsize = false;
        bool mref = false;
        for (;;) {
            std::size_t q = 0;
            while (q < text.size() && (std::isalnum(static_cast<unsigned char>(text[q])) || text[q] == '_')) ++q;
            const std::string w = lower(text.substr(0, q));
            OpFlags sz = 0;
            const char next = q < text.size() ? text[q] : '\0';
            if (size_word(w, sz) && (next == ' ' || next == '[')) {
                if (!setsize) {
                    op.type |= sz;
                    if (sz == opf::BITS8) ins.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX;
                }
                setsize = true;
            } else if ((w == "short" || w == "near" || w == "far" || w == "strict" || w == "to" || w == "abs") && (next == ' ' || next == '[')) {
                if (w == "short") { ins.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX; op.type |= opf::SHORT; }
                else if (w == "near") { ins.opt |= OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX; op.type |= opf::NEAR_; }
                else if (w == "far") op.type |= opf::FAR_;
                else if (w == "abs") op.type |= opf::ABS_;
                else if (w == "strict") op.type |= opf::STRICT_;
                else op.type |= opf::TO;
            } else {
                const int reg = w.empty() ? 0 : find_reg(w);
                if (reg && is_sreg(reg) && next == ':') {
                    // A segment override: `es:[...]`.
                    ins.prefixes[PPS_SEG] = reg;
                    if (reg == R_FS) op.eaflags |= EAF_FS;
                    if (reg == R_GS) op.eaflags |= EAF_GS;
                    text = trim(text.substr(q + 1));
                    mref = true;
                    break;
                }
                break;
            }
            text = trim(text.substr(q));
        }

        if (!text.empty() && text[0] == '[') {
            const std::size_t close = text.rfind(']');
            if (close == std::string::npos) { fail("no ]"); return op; }
            memory(op, std::string_view(text).substr(1, close - 1));
        } else if (mref) {
            fail("segment override without memory");
        } else {
            const std::string w = lower(text);
            const int reg = find_reg(w);
            if (reg) {
                op.type &= opf::TO | opf::COLON;
                op.type |= opf::REGISTER;
                op.type |= kRegFlags[static_cast<std::size_t>(reg)];
                op.basereg = reg;
            } else {
                Reader r;
                r.s = text;
                r.here = ins.loc.offset;
                const std::int64_t v = r.expr();
                if (!r.ok || !r.eof()) { fail("bad expression `" + text + "'"); return op; }
                op.offset = v;
                if (r.uses_here) {
                    op.segment = ins.loc.segment;
                } else {
                    op.opflags |= OPFLAG_SIMPLE;
                }
                op.type |= opf::IMM_NORMAL;
                imm_flags(op, ins.opt);
            }
        }
        op.decoflags |= deco_flags;
        if (rounding) ins.evex_rm = rounding;
        if (op.decoflags & (deco::BRDCAST_MASK | deco::ER | deco::SAE)) {
            ins.evex_brerop = index;
            op.bcast = true;
            op.xsize = op.decoflags & deco::BRSIZE_MASK;
        } else {
            op.bcast = false;
            op.xsize = op.type & SIZE_MASK;
        }
        return op;
    }
};

// Split on top-level commas (none inside [] or {}).
std::vector<std::string> split_commas(const std::string& s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (char c : s) {
        if (c == '[' || c == '{') ++depth;
        if (c == ']' || c == '}') --depth;
        if (c == ',' && depth == 0) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!trim(cur).empty()) out.push_back(cur);
    return out;
}

// Build the Insn for a source line; `why` says what is wrong when it returns false.
bool build(const std::string& line, int bits, Insn& ins, std::string& why, bool masm = false) {
    Builder b;
    b.bits = bits;
    b.ins.bits = static_cast<std::uint8_t>(bits);
    b.ins.times = 1;
    b.ins.loc.segment = 0;
    b.ins.loc.offset = 0;
    b.ins.loc.known = true;
    b.ins.opt = masm ? OPTIM_LEVEL_1 : OPTIM_DEFAULT;
    b.ins.evex_brerop = -1;
    std::string rest = trim(line);

    // Prefix words.
    for (;;) {
        std::size_t q = 0;
        while (q < rest.size() && (std::isalnum(static_cast<unsigned char>(rest[q])) || rest[q] == '{' || rest[q] == '}')) ++q;
        const std::string w = lower(rest.substr(0, q));
        int pfx = 0, slot = -1;
        if (w == "lock") { pfx = P_LOCK; slot = PPS_LOCK; }
        else if (w == "rep") { pfx = P_REP; slot = PPS_REP; }
        else if (w == "repe") { pfx = P_REPE; slot = PPS_REP; }
        else if (w == "repz") { pfx = P_REPZ; slot = PPS_REP; }
        else if (w == "repne") { pfx = P_REPNE; slot = PPS_REP; }
        else if (w == "repnz") { pfx = P_REPNZ; slot = PPS_REP; }
        else if (w == "xacquire") { pfx = P_XACQUIRE; slot = PPS_REP; }
        else if (w == "xrelease") { pfx = P_XRELEASE; slot = PPS_REP; }
        else if (w == "bnd") { pfx = P_BND; slot = PPS_REP; }
        else if (w == "nobnd") { pfx = P_NOBND; slot = PPS_REP; }
        else if (w == "o16") { pfx = P_O16; slot = PPS_OSIZE; }
        else if (w == "o32") { pfx = P_O32; slot = PPS_OSIZE; }
        else if (w == "o64") { pfx = P_O64; slot = PPS_OSIZE; }
        else if (w == "osp") { pfx = P_OSP; slot = PPS_OSIZE; }
        else if (w == "a16") { pfx = P_A16; slot = PPS_ASIZE; }
        else if (w == "a32") { pfx = P_A32; slot = PPS_ASIZE; }
        else if (w == "a64") { pfx = P_A64; slot = PPS_ASIZE; }
        else if (w == "asp") { pfx = P_ASP; slot = PPS_ASIZE; }
        else if (w == "{rex}") { pfx = P_REX; slot = PPS_REX; }
        else if (w == "{rex2}") { pfx = P_REX2; slot = PPS_REX; }
        else if (w == "{vex}") { pfx = P_VEX; slot = PPS_REX; }
        else if (w == "{vex2}") { pfx = P_VEX2; slot = PPS_REX; }
        else if (w == "{vex3}") { pfx = P_VEX3; slot = PPS_REX; }
        else if (w == "{evex}") { pfx = P_EVEX; slot = PPS_REX; }
        else if (w == "{pt}") { pfx = P_PT; slot = PPS_SEG; }
        else if (w == "{pn}") { pfx = P_PN; slot = PPS_SEG; }
        else {
            const int reg = find_reg(w);
            if (reg && is_sreg(reg) && q < rest.size() && trim(rest.substr(q)).size() && rest.size() > q && rest[q] == ' ') {
                // `es movsb`-style segment prefix word.
                pfx = reg;
                slot = PPS_SEG;
            } else {
                break;
            }
        }
        if (rest.size() == q) break;   // the prefix is the whole line: it is not a prefix
        if (b.ins.prefixes[static_cast<std::size_t>(slot)] && b.ins.prefixes[static_cast<std::size_t>(slot)] != pfx) {
            why = "conflicting prefixes";
            return false;
        }
        b.ins.prefixes[static_cast<std::size_t>(slot)] = pfx;
        rest = trim(rest.substr(q));
    }

    std::size_t q = 0;
    while (q < rest.size() && !std::isspace(static_cast<unsigned char>(rest[q]))) ++q;
    const std::string mnem = lower(rest.substr(0, q));
    b.ins.opcode = find_opcode(mnem);
    if (b.ins.opcode == I_none) {
        why = "unknown instruction " + mnem;
        return false;
    }
    const std::string operands = trim(rest.substr(q));
    int n = 0;
    if (!operands.empty()) {
        for (const std::string& part : split_commas(operands)) {
            if (n > 0 && trim(part).rfind('{', 0) == 0) {
                b.rounding_deco(b.ins.oprs[static_cast<std::size_t>(n - 1)], n - 1, part);
                continue;
            }
            // A far pair `seg:offset` (not a segment override: that is before a `[`).
            const std::size_t colon = part.find(':');
            if (colon != std::string::npos && part.find('[') == std::string::npos && !find_reg(lower(trim(part.substr(0, colon))))) {
                if (n + 2 > MAX_OPERANDS) { why = "too many operands"; return false; }
                Operand first = b.operand(part.substr(0, colon), n, true);
                first.type |= opf::COLON;
                b.ins.oprs[static_cast<std::size_t>(n++)] = first;
                b.ins.oprs[static_cast<std::size_t>(n++)] = b.operand(part.substr(colon + 1), n, true);
                continue;
            }
            if (n >= MAX_OPERANDS) { why = "too many operands"; return false; }
            b.ins.oprs[static_cast<std::size_t>(n)] = b.operand(part, n, true);
            ++n;
        }
    }
    if (!b.ok) {
        why = b.why;
        return false;
    }
    b.ins.operands = static_cast<std::uint8_t>(n);
    ins = b.ins;
    return true;
}

// Hex "8b 40 05" to bytes.
std::vector<std::uint8_t> from_hex(std::string_view h) {
    std::vector<std::uint8_t> out;
    std::size_t i = 0;
    while (i < h.size()) {
        if (h[i] == ' ') { ++i; continue; }
        out.push_back(static_cast<std::uint8_t>(std::strtoul(std::string(h.substr(i, 2)).c_str(), nullptr, 16)));
        i += 2;
    }
    return out;
}

std::string to_hex(const std::vector<std::uint8_t>& v) {
    static const char* digits = "0123456789abcdef";
    std::string out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) out += ' ';
        out += digits[v[i] >> 4];
        out += digits[v[i] & 15];
    }
    return out;
}

// A run of the encoder at the final pass over one line; returns the bytes (and `failed` when it refused the line).
struct Run {
    std::vector<std::uint8_t> bytes;
    bool failed = false;
    std::int64_t size = -2;
    std::string diag;
};

Assembly make_assembly(int bits) {
    Assembly as;
    as.cmd_sb = bits;
    as.error_init();
    for (int i = 0; i < 4; ++i) {   // INIT -> FIRST -> OPT -> STAB -> FINAL
        as.begin_pass();
        if (i < 3) as.error_pass_end();
    }
    as.reset_global_defaults(bits);
    as.location.segment = 0;
    as.location.offset = 0;
    as.location.known = true;
    return as;
}

Run run_line(const std::string& line, int bits, bool masm = false) {
    Run r;
    Insn ins;
    std::string why;
    if (!build(line, bits, ins, why, masm)) {
        r.failed = true;
        r.diag = "reader: " + why;
        return r;
    }
    Assembly as = make_assembly(bits);
    as.masm_mode = masm;
    Capture cap;
    Encoder enc(as, cap);
    Insn sizing = ins;
    r.size = enc.insn_size(sizing);
    if (r.size < 0) {
        r.failed = true;
        for (const auto& d : as.diagnostics()) r.diag += d.text + "; ";
        return r;
    }
    ins.loc = as.location;
    const std::int64_t l = enc.assemble(ins);
    if (as.erropt.worst >= ERR_NONFATAL || l <= 0) r.failed = true;
    r.bytes = cap.bytes;
    for (const auto& d : as.diagnostics()) r.diag += d.text + "; ";
    if (!r.failed && l != r.size) r.diag += "size " + std::to_string(r.size) + " vs assembled " + std::to_string(l) + "; ";
    return r;
}

struct Case {
    int bits;
    const char* text;
    const char* hex;   // "!" when NASM rejects it
};

constexpr Case kMasmCases[] = {
#include "cb/build/x86asm/encode_masm_cases.inc"
};

constexpr Case kCases[] = {
#include "cb/build/x86asm/encode_cases.inc"
};

}  // namespace encode_test_impl
using namespace encode_test_impl;

CB_TEST("x86asm.encode.tables", [] {
    CB_CHECK_EQ(kTemplates.size(), kNumTemplates);
    CB_CHECK_EQ(kInstructions.size(), kNumInstructions);
    // The templates of an opcode are all that opcode's, and the first of NOP is the plain one-byte NOP.
    std::size_t total = 0;
    for (std::size_t op = 0; op < kNumInstructions; ++op) {
        const TemplateList l = templates_of(static_cast<Opcode>(op));
        total += l.ntemp;
        for (const ItemTemplate& t : l) CB_CHECK_EQ(static_cast<int>(t.opcode), static_cast<int>(op));
    }
    CB_CHECK_EQ(total, kNumTemplates);
    const TemplateList nop = templates_of(I_NOP);
    CB_CHECK(nop.ntemp > 0);
    CB_CHECK_EQ(nop.temp[0].operands, 0);
    bool has90 = false;
    for (const std::uint8_t* c = nop.temp[0].code; *c; ++c) has90 |= *c == 0x90;
    CB_CHECK(has90);
});

CB_TEST("x86asm.encode.matches_nasm", [] {
    std::string failures;
    int n = 0;
    for (const Case& c : kCases) {
        const Run r = run_line(c.text, c.bits);
        const std::string want = c.hex;
        std::string got = r.failed ? "!" : to_hex(r.bytes);
        if (got != want) {
            if (++n <= 60) failures += std::to_string(c.bits) + "-bit `" + c.text + "`: expected " + want + ", got " + got + " " + r.diag + "\n";
        }
    }
    if (n > 60) failures += "... and " + std::to_string(n - 60) + " more\n";
    CB_CHECK_EQ(failures, std::string());
});

// With --masm (CodeBreaker's MASM mode): the reg,r/m orientation of a register pair, the accumulator form of a 16-bit
// ALU immediate, and [reg*2] kept as a SIB form.
CB_TEST("x86asm.encode.masm_matches_nasm", [] {
    std::string failures;
    for (const Case& c : kMasmCases) {
        const Run r = run_line(c.text, c.bits, true);
        const std::string want = c.hex;
        const std::string got = r.failed ? "!" : to_hex(r.bytes);
        if (got != want) failures += std::to_string(c.bits) + "-bit `" + c.text + "`: expected " + want + ", got " + got + " " + r.diag + "\n";
    }
    CB_CHECK_EQ(failures, std::string());
});

// Data and reservations go through the same path: DB/DW/DD, a string, TIMES, RESB, and the ABSOLUTE restriction.
CB_TEST("x86asm.encode.data", [] {
    Assembly as = make_assembly(32);
    Capture cap;
    Encoder enc(as, cap);
    Insn ins;
    ins.opcode = I_DB;
    ins.bits = 32;
    ins.times = 1;
    ins.loc.segment = 0;
    ExtOp a;
    a.type = EOT_DB_NUMBER;
    a.elem = 1;
    a.offset = 0x41;
    a.segment = NO_SEG;
    a.wrt = NO_SEG;
    a.dup = 1;
    ExtOp b;
    b.type = EOT_DB_STRING;
    b.elem = 1;
    b.string = "hi";
    b.dup = 2;
    ins.eops = {a, b};
    CB_CHECK_EQ(enc.insn_size(ins), std::int64_t{5});
    CB_CHECK_EQ(enc.assemble(ins), std::int64_t{5});
    CB_CHECK_EQ(to_hex(cap.bytes), std::string("41 68 69 68 69"));

    // DW with a 16-bit number and a string padded to the element size.
    Capture cap2;
    Encoder enc2(as, cap2);
    Insn w = ins;
    w.opcode = I_DW;
    w.eops.clear();
    a.elem = 2;
    a.offset = 0x1234;
    b.elem = 2;
    b.string = "abc";
    b.dup = 1;
    w.eops = {a, b};
    CB_CHECK_EQ(enc2.assemble(w), std::int64_t{2 + 4});
    CB_CHECK_EQ(to_hex(cap2.bytes), std::string("34 12 61 62 63 00"));
});

// A short jump out of range is refused; a forward reference is sized for the long form optimistically (pass 1: short).
CB_TEST("x86asm.encode.jumps", [] {
    // Unknown (forward) target in the first passes: the short form is assumed.
    Insn ins;
    std::string why;
    CB_CHECK(build("jmp $+5", 32, ins, why));
    ins.oprs[0].opflags |= OPFLAG_UNKNOWN;
    Assembly as;
    as.cmd_sb = 32;
    as.error_init();
    as.begin_pass();
    as.reset_global_defaults(32);
    as.location.segment = 0;
    Capture cap;
    Encoder enc(as, cap);
    CB_CHECK_EQ(enc.insn_size(ins), std::int64_t{2});
    // A known target 0x1000 away needs the near form (5 bytes).
    Insn far_ins;
    CB_CHECK(build("jmp $+0x1000", 32, far_ins, why));
    CB_CHECK_EQ(enc.insn_size(far_ins), std::int64_t{5});
    // `short` to the same target is an error at the final pass.
    Insn bad;
    CB_CHECK(build("jmp short $+0x1000", 32, bad, why));
    CB_CHECK(enc.insn_size(bad) < 0 || enc.assemble(bad) <= 0 || as.erropt.worst >= ERR_NONFATAL);
});
