// Tests of the Z80 assembler's directive layer, over a miniature instruction encoder (the real one is the
// decompiler's, and arrives with that module): NOP, HALT, LD A,n, LD HL,nn, JP nn, CALL nn.
#include "cb/build/z80_asm.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build;

// Numbers ($1F, 0x1F, %101, decimal), labels, + and - from left to right, and <x / >x.
class MiniEncoder : public Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const Labels& labels, std::int64_t pc) const override {
        expr = text::trim(expr);
        if (expr.empty()) return ParseError("", 0, 0, "empty expression");
        if (expr[0] == '<' || expr[0] == '>') {
            auto inner = value(expr.substr(1), labels, pc);
            if (!inner.ok()) return inner;
            return expr[0] == '<' ? (inner.value() & 0xFF) : ((inner.value() >> 8) & 0xFF);
        }
        std::int64_t total = 0;
        char sign = '+';
        std::size_t start = 0;
        for (std::size_t i = 0; i <= expr.size(); ++i) {
            if (i < expr.size() && !((expr[i] == '+' || expr[i] == '-') && i > start)) continue;
            auto term = atom(text::trim(expr.substr(start, i - start)), labels, pc);
            if (!term.ok()) return term;
            total += sign == '+' ? term.value() : -term.value();
            if (i < expr.size()) sign = expr[i];
            start = i + 1;
        }
        return total;
    }

    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const Labels& labels) const override {
        const std::string up = text::upper(line);
        if (up == "NOP") return Bytes{0x00};
        if (up == "HALT") return Bytes{0x76};
        auto with_operand = [&](std::string_view prefix, std::uint8_t op, bool word) -> std::optional<Result<Bytes>> {
            if (!up.starts_with(prefix)) return std::nullopt;
            auto v = value(line.substr(prefix.size()), labels, pc);
            if (!v.ok()) return Result<Bytes>(Failure(v.shared_error()));
            Bytes out{op, static_cast<std::uint8_t>(v.value() & 0xFF)};
            if (word) out.push_back(static_cast<std::uint8_t>((v.value() >> 8) & 0xFF));
            return Result<Bytes>(out);
        };
        if (auto r = with_operand("LD A,", 0x3E, false)) return *r;
        if (auto r = with_operand("LD HL,", 0x21, true)) return *r;
        if (auto r = with_operand("JP ", 0xC3, true)) return *r;
        if (auto r = with_operand("CALL ", 0xCD, true)) return *r;
        return ParseError("", 0, 0, "unknown instruction");
    }

private:
    static Result<std::int64_t> atom(std::string_view t, const Labels& labels, std::int64_t) {
        if (t.empty()) return ParseError("", 0, 0, "empty operand");
        if (t[0] == '$') {
            if (auto v = text::parse_int<std::int64_t>(t.substr(1), 16)) return *v;
            return ParseError("", 0, 0, "bad hex number");
        }
        if (t[0] == '%') {
            if (auto v = text::parse_int<std::int64_t>(t.substr(1), 2)) return *v;
            return ParseError("", 0, 0, "bad binary number");
        }
        if (t[0] >= '0' && t[0] <= '9') {
            if (auto v = parse_number(t)) return *v;
            return ParseError("", 0, 0, "bad number");
        }
        if (auto it = labels.find(std::string(t)); it != labels.end()) return it->second;
        return UndefinedSymbolError(std::string(t));
    }
};

const MiniEncoder kIsa;
const z80_asm::Assembler kAsm(kIsa);

}  // namespace

CB_TEST("build.z80_asm.rom_banks_labels_and_forward_references", [] {
    const std::string source =
        "; a two-bank cartridge\n"
        ".MEMORYMAP\n"
        "  SLOTSIZE $4000\n"
        "  DEFAULTSLOT 0\n"
        "  SLOT 0 $0000\n"
        "  SLOT 1 $4000\n"
        ".ENDME\n"
        ".ROMBANKMAP\n"
        "  BANKSTOTAL 2\n"
        "  BANKSIZE $4000\n"
        "  BANKS 2\n"
        ".ENDRO\n"
        ".BANK 0 SLOT 0\n"
        ".ORG 0\n"
        "start:  JP main\n"
        ".ORG $10\n"
        "main:   LD A,5\n"
        "        HALT\n"
        "        CALL table\n"
        ".BANK 1 SLOT 1\n"
        ".ORG 0\n"
        "table:  .DB 1, $02, \"A,B\"   ; a string keeps its comma\n"
        "        .DW table, <table+3, >table\n";
    const Bytes rom = kAsm.build(source);
    CB_CHECK_EQ(rom.size(), 0x8000u);
    CB_CHECK_EQ(rom[0], 0xC3);   // JP main: main is at $0010
    CB_CHECK_EQ(rom[1], 0x10);
    CB_CHECK_EQ(rom[2], 0x00);
    CB_CHECK_EQ(rom[0x10], 0x3E);
    CB_CHECK_EQ(rom[0x11], 5);
    CB_CHECK_EQ(rom[0x12], 0x76);
    CB_CHECK_EQ(rom[0x13], 0xCD);   // CALL table: bank 1 seen at slot 1, so $4000
    CB_CHECK_EQ(rom[0x14], 0x00);
    CB_CHECK_EQ(rom[0x15], 0x40);
    CB_CHECK_EQ(rom[0x16], 0xFF);   // unwritten bytes are $FF
    const std::size_t t = 0x4000;
    CB_CHECK_EQ(rom[t + 0], 1);
    CB_CHECK_EQ(rom[t + 1], 2);
    CB_CHECK_EQ(rom[t + 2], 'A');
    CB_CHECK_EQ(rom[t + 3], ',');
    CB_CHECK_EQ(rom[t + 4], 'B');
    CB_CHECK_EQ(rom[t + 5], 0x00);   // .DW table = $4000, little-endian
    CB_CHECK_EQ(rom[t + 6], 0x40);
    CB_CHECK_EQ(rom[t + 7], 0x03);   // <table+3: low byte of $4000 is 0, + 3
    CB_CHECK_EQ(rom[t + 8], 0x00);
    CB_CHECK_EQ(rom[t + 9], 0x40);   // >table is $40, stored as a word
    CB_CHECK_EQ(rom[t + 10], 0x00);
});

CB_TEST("build.z80_asm.constants_dsb_orga_and_default_size", [] {
    // no memory map: one 16 KB bank, slot 0 at $0000
    const std::string source =
        ".DEFINE BASE $20\n"
        "LIMIT = BASE + 1\n"
        ".ORGA $0100\n"
        "      .DSB 4 $AB\n"
        "here: LD A,LIMIT\n"
        "      .DB here\n";
    const Bytes rom = kAsm.build(source);
    CB_CHECK_EQ(rom.size(), 0x4000u);
    CB_CHECK_EQ(rom[0xFF], 0xFF);
    for (int i = 0; i < 4; ++i) CB_CHECK_EQ(rom[0x100 + i], 0xAB);
    CB_CHECK_EQ(rom[0x104], 0x3E);
    CB_CHECK_EQ(rom[0x105], 0x21);   // LIMIT
    CB_CHECK_EQ(rom[0x106], 0x04);   // here = $0104, low byte
});

CB_TEST("build.z80_asm.incbin_beside_the_source", [] {
    fs::ScopedTempDir dir("z80asm");
    const Bytes payload{9, 8, 7};
    fs::write_file(dir.path() / "blob.bin", payload);
    const Bytes rom = kAsm.build(".ORG 2\n.INCBIN \"blob.bin\"\nHALT\n", dir.path());
    CB_CHECK_EQ(rom[2], 9);
    CB_CHECK_EQ(rom[4], 7);
    CB_CHECK_EQ(rom[5], 0x76);
    fs::write_file(dir.path() / "prog.asm", as_bytes(".ORG 0\n.INCBIN \"blob.bin\"\n"));
    CB_CHECK_EQ(kAsm.build_file(dir.path() / "prog.asm")[1], 8);
    CB_CHECK_THROWS_AS(NotFoundError, (void)kAsm.build(".INCBIN \"nope.bin\"\n", dir.path()));
});

CB_TEST("build.z80_asm.errors_name_the_line", [] {
    auto message = [](std::string_view source) {
        try {
            (void)kAsm.build(source);
        } catch (const AsmError& e) {
            return std::pair{e.line(), std::string(e.what())};
        }
        return std::pair{0u, std::string("no error")};
    };
    auto [line, text] = message("NOP\n\nJP nowhere\n");
    CB_CHECK_EQ(line, 3u);
    CB_CHECK(text.find("JP nowhere") != std::string::npos);
    CB_CHECK(text.find("nowhere") != std::string::npos);
    CB_CHECK_EQ(message(".BANK 9 SLOT 0\n.DB 1\n").first, 2u);               // bytes outside the 16 KB ROM
    CB_CHECK_EQ(message("NOP\n.FROBNICATE 3\n").first, 2u);                  // directive not understood
    CB_CHECK_EQ(message("FLY\n").first, 1u);                                 // not an instruction
    CB_CHECK_EQ(message(".DSB -1 0\n").first, 1u);
    CB_CHECK_THROWS_AS(AsmError, (void)kAsm.build("BANKSTOTAL 100000\nBANKSIZE $4000\n"));   // an absurd ROM size
    // an assignment that is never defined is an error only at the end
    CB_CHECK_EQ(message("X = later\nlater = 1\n").first, 0u);
    CB_CHECK_EQ(message("X = never\n").first, 1u);
});

CB_TEST("build.z80_asm.flat_dialect", [] {
    fs::ScopedTempDir dir("z80flat");
    fs::write_file(dir.path() / "tail.bin", Bytes{0xEE, 0xEF});
    const std::string source =
        "SIZE EQU 3\n"
        "    ORG $8000\n"
        "entry: JP over\n"
        "    DEFB 1,2\n"
        "    DEFW entry\n"
        "    DEFS SIZE,$55\n"
        "    DS 2\n"
        "over: HALT\n"
        "    INCBIN \"tail.bin\"\n"
        "    ORG $8040\n"
        "    DEFM \"hi\"\n";
    const auto flat = kAsm.build_flat(source, dir.path());
    CB_CHECK_EQ(flat.start, 0x8000u);
    // entry: C3 ll hh (3) / 01 02 / 00 80 / 55 55 55 / 00 00 / 76 / EE EF = 15 bytes, a gap to $8040, "hi"
    CB_CHECK_EQ(flat.bytes.size(), 0x42u);
    CB_CHECK_EQ(flat.bytes[0], 0xC3);
    CB_CHECK_EQ(flat.bytes[1], 0x0C);   // over = $800C
    CB_CHECK_EQ(flat.bytes[2], 0x80);
    CB_CHECK_EQ(flat.bytes[3], 1);
    CB_CHECK_EQ(flat.bytes[5], 0x00);
    CB_CHECK_EQ(flat.bytes[6], 0x80);
    CB_CHECK_EQ(flat.bytes[7], 0x55);
    CB_CHECK_EQ(flat.bytes[10], 0);
    CB_CHECK_EQ(flat.bytes[12], 0x76);
    CB_CHECK_EQ(flat.bytes[13], 0xEE);
    CB_CHECK_EQ(flat.bytes[14], 0xEF);
    CB_CHECK_EQ(flat.bytes[15], 0);   // gaps are zero
    CB_CHECK_EQ(flat.bytes[0x40], 'h');
    CB_CHECK_EQ(flat.bytes[0x41], 'i');

    CB_CHECK(kAsm.build_flat("; nothing\n").bytes.empty());
    // chunks written later overwrite earlier ones where they overlap
    const auto over = kAsm.build_flat("ORG 10\nDEFB 1,2,3\nORG 11\nDEFB 9\n");
    CB_CHECK_EQ(over.start, 10u);
    CB_CHECK_EQ(over.bytes.size(), 3u);
    CB_CHECK_EQ(over.bytes[1], 9);
    CB_CHECK_THROWS_AS(AsmError, (void)kAsm.build_flat("DEFW missing\n"));
});

CB_TEST("build.z80_asm.size_guess_gives_each_unknown_name_a_value", [] {
    auto r = z80_asm::size_guess(kIsa, "CALL somewhere", 0x100);
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().size(), 3u);
    CB_CHECK(!z80_asm::size_guess(kIsa, "FLY", 0).ok());
});
