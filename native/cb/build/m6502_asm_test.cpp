// Tests of the 6502 assembler's directive layer and linker configuration, over a miniature encoder:
// NOP, RTS, LDA #n, STA nnnn, JMP nnnn, BNE label.
#include "cb/build/m6502_asm.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build;
using namespace cb::build::m6502_asm;

class MiniEncoder : public Encoder6502 {
public:
    Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const Labels& labels) const override {
        const std::string up = text::upper(line);
        if (up == "NOP") return Bytes{0xEA};
        if (up == "RTS") return Bytes{0x60};
        if (up.starts_with("LDA #")) {
            auto v = operand(line.substr(5), labels);
            if (!v.ok()) return Failure(v.shared_error());
            return Bytes{0xA9, static_cast<std::uint8_t>(v.value())};
        }
        struct Absolute {
            std::string_view prefix;
            std::uint8_t op;
        };
        for (const Absolute& a : {Absolute{"STA ", 0x8D}, Absolute{"JMP ", 0x4C}}) {
            if (!up.starts_with(a.prefix)) continue;
            auto v = operand(line.substr(a.prefix.size()), labels);
            if (!v.ok()) return Failure(v.shared_error());
            return Bytes{a.op, static_cast<std::uint8_t>(v.value() & 0xFF), static_cast<std::uint8_t>(v.value() >> 8)};
        }
        if (up.starts_with("BNE ")) {
            auto v = operand(line.substr(4), labels);
            if (!v.ok()) return Failure(v.shared_error());
            const std::int64_t rel = v.value() - (static_cast<std::int64_t>(pc) + 2);
            if (rel < -128 || rel > 127) return ParseError("", 0, 0, "branch out of range");
            return Bytes{0xD0, static_cast<std::uint8_t>(rel & 0xFF)};
        }
        return ParseError("", 0, 0, "unknown instruction");
    }
    bool is_branch(std::string_view mnemonic) const override { return mnemonic == "BNE" || mnemonic == "BEQ"; }

private:
    static Result<std::int64_t> operand(std::string_view t, const Labels& labels) {
        t = text::trim(t);
        if (!t.empty() && t[0] == '$') {
            if (auto v = text::parse_int<std::int64_t>(t.substr(1), 16)) return *v;
            return ParseError("", 0, 0, "bad number");
        }
        if (!t.empty() && t[0] >= '0' && t[0] <= '9') return text::parse_int<std::int64_t>(t).value_or(0);
        if (auto it = labels.find(std::string(t)); it != labels.end()) return it->second;
        return UndefinedSymbolError(std::string(t));
    }
};

const MiniEncoder kIsa;
const Assembler kAsm(kIsa);

const char* kConfig =
    "# an NROM-ish layout\n"
    "MEMORY {\n"
    "    HEADER: start = $0000, size = 4, fill = yes;   # the iNES header\n"
    "    PRG:    start = $8000, size = $20, fill = yes;\n"
    "    RAM:    start = $0200, size = $100;\n"
    "}\n"
    "SEGMENTS {\n"
    "    HEADER: load = HEADER, type = ro;\n"
    "    CODE:   load = PRG,    type = ro;\n"
    "    DATA:   load = PRG,    type = ro;\n"
    "}\n";

}  // namespace

CB_TEST("build.m6502_asm.config", [] {
    const LinkConfig c = parse_config(kConfig);
    CB_CHECK_EQ(c.areas.size(), 3u);
    CB_CHECK_EQ(c.areas[0].name, "HEADER");
    CB_CHECK_EQ(c.areas[0].size, 4u);
    CB_CHECK(c.areas[0].fill);
    CB_CHECK_EQ(c.areas[1].start, 0x8000u);
    CB_CHECK_EQ(c.areas[1].size, 0x20u);
    CB_CHECK(!c.areas[2].fill);
    CB_CHECK_EQ(c.segment_area.at("CODE"), "PRG");
    CB_CHECK_EQ(c.segment_area.at("DATA"), "PRG");
    CB_CHECK_EQ(c.segment_area.size(), 3u);
    CB_CHECK_THROWS_AS(AsmError, (void)parse_config("MEMORY { A: start = zzz, size = 1; }"));
    CB_CHECK_THROWS_AS(AsmError, (void)parse_config("SEGMENTS { CODE: type = ro; }"));
    CB_CHECK(parse_config("").areas.empty());
});

CB_TEST("build.m6502_asm.links_segments_into_areas", [] {
    const std::string source =
        ".segment \"HEADER\"\n"
        "    .byte \"NES\", $1A\n"
        ".segment \"CODE\"\n"
        "SCREEN = $2000\n"
        "reset:  LDA #$80      ; a comment\n"
        "        STA SCREEN\n"
        "loop:   NOP\n"
        "        BNE loop\n"
        "        JMP later\n"
        ".segment \"DATA\"\n"
        "later:  RTS\n"
        "table:  .word reset, later, $BEEF\n";
    const Bytes image = kAsm.build(source, kConfig);
    CB_CHECK_EQ(image.size(), 4u + 0x20u);
    CB_CHECK_EQ(image[0], 'N');
    CB_CHECK_EQ(image[3], 0x1A);
    // PRG begins at file offset 4, CPU address $8000
    const std::size_t p = 4;
    CB_CHECK_EQ(image[p + 0], 0xA9);   // LDA #$80
    CB_CHECK_EQ(image[p + 1], 0x80);
    CB_CHECK_EQ(image[p + 2], 0x8D);   // STA $2000
    CB_CHECK_EQ(image[p + 3], 0x00);
    CB_CHECK_EQ(image[p + 4], 0x20);
    CB_CHECK_EQ(image[p + 5], 0xEA);   // loop = $8005
    CB_CHECK_EQ(image[p + 6], 0xD0);   // BNE loop: from $8008 back to $8005
    CB_CHECK_EQ(image[p + 7], 0xFD);
    CB_CHECK_EQ(image[p + 8], 0x4C);   // JMP later: later = $800B
    CB_CHECK_EQ(image[p + 9], 0x0B);
    CB_CHECK_EQ(image[p + 10], 0x80);
    CB_CHECK_EQ(image[p + 11], 0x60);
    CB_CHECK_EQ(image[p + 12], 0x00);  // .word reset: $8000
    CB_CHECK_EQ(image[p + 13], 0x80);
    CB_CHECK_EQ(image[p + 14], 0x0B);
    CB_CHECK_EQ(image[p + 16], 0xEF);  // $BEEF
    CB_CHECK_EQ(image[p + 17], 0xBE);
    CB_CHECK_EQ(image[p + 18], 0x00);  // filled with zeros to the area's size
});

CB_TEST("build.m6502_asm.incbin_and_unfilled_areas", [] {
    fs::ScopedTempDir dir("m6502");
    fs::write_file(dir.path() / "gfx.chr", Bytes{1, 2, 3});
    const std::string config = "MEMORY { ROM: start = $8000, size = 8; } SEGMENTS { CODE: load = ROM; }";
    const Bytes image = kAsm.build(".segment \"CODE\"\n.incbin \"gfx.chr\"\n.byte 9\n", config, dir.path());
    CB_CHECK_EQ(image.size(), 4u);   // fill is off: just what was written
    CB_CHECK_EQ(image[2], 3);
    CB_CHECK_EQ(image[3], 9);
    CB_CHECK_THROWS_AS(NotFoundError, (void)kAsm.build(".segment \"CODE\"\n.incbin \"none.chr\"\n", config, dir.path()));
});

CB_TEST("build.m6502_asm.errors_name_the_line", [] {
    auto line_of = [](std::string_view source) {
        try {
            (void)kAsm.build(source, kConfig);
        } catch (const AsmError& e) {
            return e.line();
        }
        return 0u;
    };
    CB_CHECK_EQ(line_of("NOP\n"), 1u);                                        // code outside any segment
    CB_CHECK_EQ(line_of("lab: .byte 1\n"), 1u);                               // a label outside any segment
    CB_CHECK_EQ(line_of(".segment \"CODE\"\nNOP\n.segment \"ZEROPAGE\"\n"), 3u);   // not in the configuration
    CB_CHECK_EQ(line_of(".segment \"CODE\"\n.bogus 1\n"), 2u);
    CB_CHECK_EQ(line_of(".segment \"CODE\"\n.byte\n"), 2u);                   // no operands
    CB_CHECK_EQ(line_of(".segment \"CODE\"\n.byte lab\n"), 2u);               // not a number
    CB_CHECK_EQ(line_of(".segment \"CODE\"\nJMP nowhere\n"), 2u);             // never defined
    CB_CHECK_EQ(line_of(".segment \"CODE\"\n.word nowhere\n"), 2u);
    CB_CHECK_EQ(line_of(".segment \"CODE\"\nFLY\n"), 2u);
    // 0x21 bytes into a 0x20-byte area: the error is about the area, not one line
    const std::string big = ".segment \"CODE\"\n.byte " + std::string("1,2,3,4,5,6,7,8,9,10,11,12,16,15,14,13,") +
                            "1,2,3,4,5,6,7,8,9,10,11,12,16,15,14,13,1\n";
    CB_CHECK_THROWS_AS(AsmError, (void)kAsm.build(big, kConfig));
});

CB_TEST("build.m6502_asm.branches_are_sized_before_their_labels_exist", [] {
    // a forward BNE to a label farther on: two bytes on the first pass, the real offset on the second
    const std::string config = "MEMORY { R: start = $8000, size = 8, fill = yes; } SEGMENTS { CODE: load = R; }";
    const Bytes image = kAsm.build(".segment \"CODE\"\nBNE far\nNOP\nfar: RTS\n", config);
    CB_CHECK_EQ(image[0], 0xD0);
    CB_CHECK_EQ(image[1], 0x01);
    CB_CHECK_EQ(image[3], 0x60);
});
