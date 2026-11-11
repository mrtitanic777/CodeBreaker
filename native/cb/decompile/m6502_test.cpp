#include "cb/decompile/m6502.hpp"

#include <format>
#include <map>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// DIVERGES from the Python module (the old harness agreed on all else: 9342 cases, 4138 decodes, 5198 assemblies):
//  - A number is `$hex`, decimal, or `0x` hex. Python's int(s, 0) also took `0b101`, `0o17`, `1_000`, digits of other
//    scripts and rejected leading zeros (`012`); ca65 has none of those spellings except plain decimal, so the first
//    four are errors now and `012` is decimal 12.
//  - decode() takes an unsigned 32-bit address and an unsigned offset: a negative offset (Python indexed from the end)
//    or a negative / past-4G address is not representable.
//  - Error text is not compared; every assembler failure is a cb::ParseError (Python: ValueError).
//  - The tables are iterated in opcode order, not dict insertion order.
namespace m = cb::decompile::m6502;
using cb::Bytes;

namespace {

Bytes hex(std::string_view s) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(s.substr(i, 2)), nullptr, 16)));
    return out;
}

std::string dis(std::string_view bytes, std::uint32_t pc = 0x8000, const m::AddressLabels& labels = {}) {
    Bytes code = hex(bytes);
    auto insn = m::decode(code, 0, pc);
    return insn ? m::text(*insn, labels) : "<none>";
}

std::string to_hex(const Bytes& b) {
    std::string out;
    for (std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

std::string asm_hex(std::string_view line, std::uint32_t pc, const m::NameLabels& labels = {}) {
    return to_hex(m::assemble(line, pc, labels));
}

}  // namespace

CB_TEST("decompile.m6502.table", [] {
    int documented = 0;
    for (const auto& entry : m::opcode_table()) documented += entry.has_value();
    CB_CHECK_EQ(documented, 151);   // the NMOS 6502's documented set
    CB_CHECK_EQ(m::opcode_of("LDA", m::Mode::Imm).value(), 0xA9);
    CB_CHECK_EQ(m::opcode_of("STA", m::Mode::Izy).value(), 0x91);
    CB_CHECK(!m::opcode_of("STA", m::Mode::Imm).has_value());
    CB_CHECK(!m::opcode_of("XXX", m::Mode::Imp).has_value());
    CB_CHECK(m::is_branch("BNE") && !m::is_branch("JMP"));
    CB_CHECK(m::ends_flow("RTS") && m::ends_flow("BRK") && !m::ends_flow("JSR"));
    // the table and opcode_of agree for every documented opcode
    for (int op = 0; op < 256; ++op) {
        const auto& entry = m::opcode_table()[static_cast<std::size_t>(op)];
        if (entry) CB_CHECK_EQ(m::opcode_of(entry->mnemonic, entry->mode).value(), op);
    }
});

CB_TEST("decompile.m6502.decode_forms", [] {
    CB_CHECK_EQ(dis("a901"), "LDA #$01");
    CB_CHECK_EQ(dis("a510"), "LDA $10");
    CB_CHECK_EQ(dis("b510"), "LDA $10,x");
    CB_CHECK_EQ(dis("ad0020"), "LDA $2000");
    CB_CHECK_EQ(dis("bd0002"), "LDA $0200,x");
    CB_CHECK_EQ(dis("b90002"), "LDA $0200,y");
    CB_CHECK_EQ(dis("a110"), "LDA ($10,x)");
    CB_CHECK_EQ(dis("b110"), "LDA ($10),y");
    CB_CHECK_EQ(dis("6c34f2"), "JMP ($F234)");
    CB_CHECK_EQ(dis("0a"), "ASL a");
    CB_CHECK_EQ(dis("ea"), "NOP");
    CB_CHECK_EQ(dis("b610"), "LDX $10,y");
    // a zero-page-sized value written as absolute keeps its width
    CB_CHECK_EQ(dis("ad1000"), "LDA $0010");
    // branches: from the next instruction, signed, wrapping at 16 bits
    CB_CHECK_EQ(dis("d0fe", 0x8000), "BNE $8000");
    CB_CHECK_EQ(dis("f005", 0x8000), "BEQ $8007");
    CB_CHECK_EQ(dis("d0fe", 0x0000), "BNE $0000");
    CB_CHECK_EQ(dis("d0f0", 0x0000), "BNE $FFF2");   // 0 + 2 - 16 wraps below zero
    CB_CHECK_EQ(dis("10ff", 0xFFFE), "BPL $FFFF");
    CB_CHECK_EQ(dis("1003", 0xFFFE), "BPL $0003");   // wraps past the top
    CB_CHECK_EQ(dis("203412", 0x8000), "JSR $1234");
    auto jsr = m::decode(hex("203412"), 0, 0x8000);
    CB_CHECK(jsr && jsr->target == 0x1234 && jsr->size == 3 && jsr->operand == 0x1234);
    auto lda = m::decode(hex("a542"), 0, 0x8000);
    CB_CHECK(lda && !lda->target && lda->size == 2 && lda->at == 0x8000);
});

CB_TEST("decompile.m6502.decode_limits", [] {
    CB_CHECK(!m::decode(hex("02"), 0, 0));                    // undocumented opcode
    CB_CHECK(!m::decode(hex("ad00"), 0, 0));                  // operand cut off by the end
    CB_CHECK(!m::decode(hex("a9"), 0, 0));
    CB_CHECK(!m::decode(Bytes{}, 0, 0));
    CB_CHECK(!m::decode(hex("ea"), 1, 0));                    // offset at the end
    CB_CHECK(m::decode(hex("00ea"), 1, 5).has_value());       // offset into the middle
});

CB_TEST("decompile.m6502.labels", [] {
    m::AddressLabels labels;
    labels[0x10] = "zp_var";
    labels[0x200] = "ram";
    labels[0x8003] = "loop";
    labels[0x42] = "";
    labels[0x00] = "nul";
    CB_CHECK_EQ(dis("a510", 0x8000, labels), "LDA zp_var");
    CB_CHECK_EQ(dis("bd0002", 0x8000, labels), "LDA ram,x");
    CB_CHECK_EQ(dis("d001", 0x8000, labels), "BNE loop");
    // an empty label name is no label
    CB_CHECK_EQ(dis("a542", 0x8000, labels), "LDA $42");
    // a label for a zero-page address used as an absolute operand stays absolute (`a:` forces the wide form)
    CB_CHECK_EQ(dis("ad0000", 0x8000, labels), "LDA a:nul");
    CB_CHECK_EQ(dis("ad1000", 0x8000, labels), "LDA a:zp_var");
});

CB_TEST("decompile.m6502.assemble", [] {
    CB_CHECK_EQ(asm_hex("LDA #$01", 0), "a901");
    CB_CHECK_EQ(asm_hex("  lda   #$01   ; comment", 0), "a901");
    CB_CHECK_EQ(asm_hex("LDA $10", 0), "a510");
    CB_CHECK_EQ(asm_hex("LDA $0010", 0), "ad1000");
    CB_CHECK_EQ(asm_hex("LDA a:$10", 0), "ad1000");
    CB_CHECK_EQ(asm_hex("LDA $10,x", 0), "b510");
    CB_CHECK_EQ(asm_hex("LDA $1000,x", 0), "bd0010");
    CB_CHECK_EQ(asm_hex("LDX $10,y", 0), "b610");
    CB_CHECK_EQ(asm_hex("LDA ($10,x)", 0), "a110");
    CB_CHECK_EQ(asm_hex("LDA ($10),y", 0), "b110");
    CB_CHECK_EQ(asm_hex("JMP ($1234)", 0), "6c3412");
    CB_CHECK_EQ(asm_hex("ASL a", 0), "0a");
    CB_CHECK_EQ(asm_hex("RTS", 0), "60");
    CB_CHECK_EQ(asm_hex("RTS ; done", 0), "60");
    CB_CHECK_EQ(asm_hex("LDA #255", 0), "a9ff");
    CB_CHECK_EQ(asm_hex("LDA #<$1234", 0), "a934");   // the byte-select marks are ignored
    // a decimal or $-less value
    CB_CHECK_EQ(asm_hex("LDA 4096", 0), "ad0010");
    // a label: a small value is zero-page, a large one absolute
    m::NameLabels labels{{"zp", 0x20}, {"big", 0x1234}, {"loop", 0x8010}};
    CB_CHECK_EQ(asm_hex("STA zp", 0, labels), "8520");
    CB_CHECK_EQ(asm_hex("STA big,x", 0, labels), "9d3412");
    CB_CHECK_EQ(asm_hex("STA a:zp", 0, labels), "8d2000");
    // a branch: relative to the next instruction
    CB_CHECK_EQ(asm_hex("BNE loop", 0x8010, labels), "d0fe");
    CB_CHECK_EQ(asm_hex("BEQ loop", 0x800c, labels), "f002");
    CB_CHECK_EQ(asm_hex("BNE $8080", 0x8000), "d07e");
    // an instruction with no zero-page form falls to absolute
    CB_CHECK_EQ(asm_hex("JSR $10", 0), "201000");
});

CB_TEST("decompile.m6502.assemble_errors", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("", 0));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("LD", 0));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("LDAX #1", 0));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("XYZ", 0));                    // unknown mnemonic
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("STA #$01", 0));               // no immediate form
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("LDA nowhere", 0));            // unknown label
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("LDA $zz", 0));                // bad hex
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BNE $9000", 0x8000));        // out of reach
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BNE $7F00", 0x8000));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("JMP ($1234", 0));             // unclosed
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("RTS $10", 0));                // no absolute form
    // the edges of branch reach are fine
    CB_CHECK_NOTHROW((void)m::assemble("BNE $8081", 0x8000));    // +127
    CB_CHECK_NOTHROW((void)m::assemble("BNE $7F82", 0x8000));    // -128
});

// Every documented opcode, with several operands: decode -> text -> assemble gives the bytes back.
CB_TEST("decompile.m6502.roundtrip_every_opcode", [] {
    const std::uint8_t operands[][2] = {{0x00, 0x00}, {0x01, 0x00}, {0x7f, 0x00}, {0x80, 0x00}, {0xff, 0x00},
                                        {0x00, 0x02}, {0x34, 0x12}, {0xff, 0xff}, {0x10, 0x00}};
    int checked = 0;
    for (int op = 0; op < 256; ++op) {
        const auto& entry = m::opcode_table()[static_cast<std::size_t>(op)];
        if (!entry) {
            Bytes code{static_cast<std::uint8_t>(op), 0, 0};
            CB_CHECK(!m::decode(code, 0, 0x8000));
            continue;
        }
        for (const auto& operand : operands) {
            Bytes code{static_cast<std::uint8_t>(op), operand[0], operand[1]};
            auto insn = m::decode(code, 0, 0x8000);
            CB_CHECK(insn.has_value());
            if (!insn) continue;
            CB_CHECK_EQ(insn->size, m::instruction_size(entry->mode));
            code.resize(insn->size);
            // a branch whose target is out of reach of a re-assembly at the same pc cannot occur: it came from one
            CB_CHECK_EQ(asm_hex(m::text(*insn), 0x8000), to_hex(code));
            ++checked;
        }
    }
    CB_CHECK_EQ(checked, 151 * 9);
});

// The real program tests/data/nes/demo.s (hand-written for the NES lane): assembled with labels from a two-pass
// pass of the same assembler, disassembled again, and each line must read back as it was written.
CB_TEST("decompile.m6502.nes_demo_program", [] {
    const std::string source = cb::fs::read_text(cb::testing::data_path("decompile/testdata/m6502_demo.s"));
    struct Line {
        std::string text;
        std::string label;   // a label defined just before this instruction
    };
    std::vector<Line> program;
    std::string pending;
    bool in_bank = false;
    for (std::string_view raw : cb::text::lines(source)) {
        std::string_view line = cb::text::trim(raw);
        if (line.starts_with(".segment")) {
            in_bank = line.find("BANK0") != std::string_view::npos;
            continue;
        }
        if (!in_bank || line.empty() || line.starts_with(";") || line.starts_with(".")) continue;
        if (line.ends_with(":")) {
            pending = std::string(line.substr(0, line.size() - 1));
            continue;
        }
        program.push_back({std::string(line), pending});
        pending.clear();
    }
    CB_CHECK_EQ(program.size(), 59u);   // the instruction lines of demo.s, counted by hand

    // pass 1: sizes (labels not yet known are far; a branch is two bytes)
    m::NameLabels labels;
    std::uint32_t pc = 0xC000;
    for (const Line& l : program) {
        if (!l.label.empty()) labels[l.label] = pc;
        const std::string mnemonic = cb::text::upper(l.text.substr(0, 3));
        if (m::is_branch(mnemonic)) {
            pc += 2;
        } else {
            m::NameLabels far_labels = labels;
            for (const Line& other : program) far_labels.emplace(other.label, 0xFFFF);
            far_labels.emplace(pending, 0xFFFF);
            pc += static_cast<std::uint32_t>(m::assemble(l.text, pc, far_labels).size());
        }
    }
    if (!pending.empty()) labels[pending] = pc;   // `colours:` labels the data after the last instruction
    // pass 2
    Bytes image;
    pc = 0xC000;
    for (const Line& l : program) {
        Bytes b = m::assemble(l.text, pc, labels);
        image.insert(image.end(), b.begin(), b.end());
        pc += static_cast<std::uint32_t>(b.size());
    }
    CB_CHECK_EQ(image.size(), pc - 0xC000);
    CB_CHECK_EQ(image[0], 0x78);   // SEI: the reset vector's first instruction
    // disassemble with the same labels and compare line for line (mnemonics upper case, `a` lower, as written)
    m::AddressLabels by_address;
    for (const auto& [name, address] : labels) by_address[static_cast<std::uint16_t>(address)] = name;
    std::size_t offset = 0, index = 0;
    pc = 0xC000;
    while (offset < image.size()) {
        auto insn = m::decode(image, offset, pc);
        CB_CHECK(insn.has_value());
        if (!insn) break;
        CB_CHECK(index < program.size());
        if (index >= program.size()) break;
        std::string expected = program[index].text;
        // normalise the source's spelling: spaces after commas, upper-case register letters
        std::string got = m::text(*insn, by_address);
        CB_CHECK_EQ(cb::text::lower(got), cb::text::lower(expected));
        offset += insn->size;
        pc += insn->size;
        ++index;
    }
    CB_CHECK_EQ(index, program.size());
    // the multiply routine is a loop of ADC with a backwards branch to `more`
    CB_CHECK(labels.contains("multiply") && labels.contains("more") && labels.at("more") > labels.at("multiply"));
});
