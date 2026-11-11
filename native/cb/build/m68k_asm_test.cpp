// Tests of the 68000 assembler's directive layer, over a miniature instruction encoder (the real one is the
// decompiler's and arrives with that module): NOP, RTS, BRA.S / BRA.W label, JMP label.L.
//
// DIVERGES from the Python:
//   * `NAME equ <expression naming a label defined later>` resolves on the last pass (the Python refused it on the
//   first, with
//     a KeyError that became an AsmError, though the same expression is fine in `dc`).
//   * `dcb` with a negative count, an `org` or `dcb` that would make an image of more than 64 MiB, and a negative
//   address are
//     AsmErrors (the Python moved the location counter backwards, tried to allocate, or wrapped).
//   * A "string" in dc.b is the bytes between its quotes (the source is bytes), not the Latin-1 encoding of a decoded
//   str.
#include "cb/build/m68k_asm.hpp"

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// (a container is compared with ==; the failure message of CB_CHECK_EQ cannot render one)
#define CHECK_SAME(a, ...) CB_CHECK((a) == (__VA_ARGS__))

namespace {

using namespace cb;
using namespace cb::build;

// Numbers ($1F, 0x1F, decimal), labels, + and - from left to right.
class MiniEncoder : public Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const Labels& labels, std::int64_t) const override {
        expr = text::trim(expr);
        if (expr.empty())
            return ParseError("", 0, 0, "empty expression");
        std::int64_t total = 0;
        char sign = '+';
        std::size_t start = 0;
        for (std::size_t i = 0; i <= expr.size(); ++i) {
            if (i < expr.size() && !((expr[i] == '+' || expr[i] == '-') && i > start))
                continue;
            auto term = atom(text::trim(expr.substr(start, i - start)), labels);
            if (!term.ok())
                return term;
            total += sign == '+' ? term.value() : -term.value();
            if (i < expr.size())
                sign = expr[i];
            start = i + 1;
        }
        return total;
    }

    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const Labels& labels) const override {
        const std::string up = text::upper(line);
        if (up == "NOP")
            return Bytes{0x4E, 0x71};
        if (up == "RTS")
            return Bytes{0x4E, 0x75};
        if (up.starts_with("BRA.S ")) {
            auto v = value(line.substr(6), labels, pc);
            if (!v.ok())
                return Failure(v.shared_error());
            const std::int64_t disp = v.value() - (pc + 2);
            if (disp == 0 || disp < -128 || disp > 127)
                return ParseError("", 0, 0, "short branch out of range");
            return Bytes{0x60, static_cast<std::uint8_t>(disp & 0xFF)};
        }
        if (up.starts_with("BRA.W ")) {
            auto v = value(line.substr(6), labels, pc);
            if (!v.ok())
                return Failure(v.shared_error());
            const std::int64_t disp = v.value() - (pc + 2);
            return Bytes{0x60, 0x00, static_cast<std::uint8_t>((disp >> 8) & 0xFF),
                         static_cast<std::uint8_t>(disp & 0xFF)};
        }
        if (up.starts_with("JMP ") && up.ends_with(".L")) {
            auto v = value(line.substr(4, line.size() - 6), labels, pc);
            if (!v.ok())
                return Failure(v.shared_error());
            const auto a = static_cast<std::uint32_t>(v.value());
            return Bytes{0x4E,
                         0xF9,
                         static_cast<std::uint8_t>(a >> 24),
                         static_cast<std::uint8_t>(a >> 16),
                         static_cast<std::uint8_t>(a >> 8),
                         static_cast<std::uint8_t>(a)};
        }
        return ParseError("", 0, 0, "unknown instruction");
    }

private:
    static Result<std::int64_t> atom(std::string_view t, const Labels& labels) {
        if (t.empty())
            return ParseError("", 0, 0, "empty operand");
        if (t[0] == '$') {
            if (auto v = text::parse_int<std::int64_t>(t.substr(1), 16))
                return *v;
            return ParseError("", 0, 0, "bad hex number");
        }
        if (t[0] >= '0' && t[0] <= '9') {
            if (auto v = parse_number(t))
                return *v;
            return ParseError("", 0, 0, "bad number");
        }
        if (auto it = labels.find(std::string(t)); it != labels.end())
            return it->second;
        return UndefinedSymbolError(std::string(t));
    }
};

const MiniEncoder kIsa;
const m68k_asm::Assembler kAsm(kIsa);

Bytes b(std::initializer_list<int> v) {
    Bytes out;
    for (int x : v)
        out.push_back(static_cast<std::uint8_t>(x));
    return out;
}

} // namespace

CB_TEST("build.m68k_asm.data_is_big_endian", [] {
    const Bytes image = kAsm.build("  dc.b 1,2,$FF\n"
                                   "  dc.w $1234, 5\n"
                                   "  dc.l $DEADBEEF\n"
                                   "  dc.l $1FFFFFFFF\n"); // too wide for a long: the high bits are dropped
    CHECK_SAME(image, b({1, 2, 0xFF, 0x12, 0x34, 0, 5, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF}));
});

CB_TEST("build.m68k_asm.strings_labels_and_comments", [] {
    const Bytes image = kAsm.build("* a whole-line comment\n"
                                   "start:  dc.b \"A;B,C\",0   ; the string keeps its ; and ,\n"
                                   "end:    dc.w end-start\n");
    // "A;B,C" 0 -> 41 3B 42 2C 43 00 (6 bytes), end = 6, so the word is 0006
    CHECK_SAME(image, b({0x41, 0x3B, 0x42, 0x2C, 0x43, 0, 0, 6}));
});

CB_TEST("build.m68k_asm.org_equ_dcb_even", [] {
    const Bytes image = kAsm.build("BASE equ $10\n"
                                   "  org BASE\n"
                                   "  dc.b 7\n"
                                   "  even\n"
                                   "  dcb.w 2,$ABCD\n"
                                   "  dcb.b 3,1\n");
    // zeros up to $10, the byte, a pad byte (even), two words, three bytes
    Bytes want(0x10, 0);
    for (int x : {7, 0, 0xAB, 0xCD, 0xAB, 0xCD, 1, 1, 1})
        want.push_back(static_cast<std::uint8_t>(x));
    CHECK_SAME(image, want);
});

CB_TEST("build.m68k_asm.gap_between_orgs_is_zero", [] {
    const Bytes image = kAsm.build("dc.b 1\norg 4\ndc.b 2\n");
    CHECK_SAME(image, b({1, 0, 0, 0, 2}));
});

CB_TEST("build.m68k_asm.forward_labels_and_branch_sizes", [] {
    // a short branch forward over a NOP, a word branch back, a JMP to a label defined later
    const Bytes image = kAsm.build("top:   nop\n"
                                   "       bra.s skip\n"
                                   "       nop\n"
                                   "skip:  bra.w top\n"
                                   "       jmp end.l\n"
                                   "end:   rts\n");
    // nop(0-1) bra.s(2-3, to 6: disp 2) nop(4-5) bra.w(6-9, to 0: disp -8) jmp(10-15) rts(16)
    CHECK_SAME(image,
               b({0x4E, 0x71, 0x60, 0x02, 0x4E, 0x71, 0x60, 0x00, 0xFF, 0xF8, 0x4E, 0xF9, 0, 0, 0, 16, 0x4E, 0x75}));
});

CB_TEST("build.m68k_asm.forward_equ_resolves", [] {
    // (DIVERGES from the Python, which refused a constant whose value names a label defined later)
    const Bytes image = kAsm.build("size equ endlabel-startlabel\nstartlabel: dc.b 1,2,3\nendlabel: dc.b size\n");
    CHECK_SAME(image, b({1, 2, 3, 3}));
});

CB_TEST("build.m68k_asm.errors_name_the_line", [] {
    bool threw = false;
    try {
        (void)kAsm.build("nop\n\nfrobnicate d0\n");
    } catch (const AsmError& e) {
        threw = true;
        const std::string what = e.what();
        CB_CHECK(what.find("frobnicate d0") != std::string::npos);
        CB_CHECK_EQ(e.line(), 3u);
    }
    CB_CHECK(threw);
    CB_CHECK_THROWS_AS(AsmError, kAsm.build("dc.w nowhere\n"));        // undefined on the last pass
    CB_CHECK_THROWS_AS(AsmError, kAsm.build("dc.b 1\n bra.s 1000\n")); // a branch the encoder refuses
    CB_CHECK_THROWS_AS(AsmError, kAsm.build("dcb.b -1,0\n"));
    CB_CHECK_THROWS_AS(AsmError, kAsm.build("org $7FFFFFFF\ndc.b 1\n")); // outside any image
    CB_CHECK_THROWS_AS(AsmError, kAsm.build("org -2\ndc.b 1\n"));
});

CB_TEST("build.m68k_asm.size_guess_uses_a_near_address", [] {
    // BRA.S to an undefined name is sized with the name at pc+4: a short branch is 2 bytes
    const auto r = m68k_asm::size_guess(kIsa, "bra.s later", 0x100, {});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().size(), 2u);
    const auto bad = m68k_asm::size_guess(kIsa, "frobnicate", 0, {});
    CB_CHECK(!bad.ok());
});

CB_TEST("build.m68k_asm.strip_comments", [] {
    CB_CHECK_EQ(std::string(m68k_asm::strip("  * all comment")), "");
    CB_CHECK_EQ(std::string(m68k_asm::strip("nop ; rest  ")), "nop");
    CB_CHECK_EQ(std::string(m68k_asm::strip("dc.b \";\" ; x")), "dc.b \";\"");
});
