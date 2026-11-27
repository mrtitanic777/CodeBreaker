// Tests of the processors as the source writers use them: how each decoder says control leaves an instruction (the cartridge formats
// follow code by it), and how each encoder tells a name with no value yet from text that is not an instruction (the assemblers' first pass
// depends on it).

#include "source/isa.hpp"

#include "core/testing.hpp"

namespace cb::source::isa {
namespace {

// The 64 KB the decoders read, with `code` at `pc`.
Bytes memory_with(std::uint32_t pc, const Bytes& code) {
    Bytes mem(0x10000, 0);
    std::copy(code.begin(), code.end(), mem.begin() + pc);
    return mem;
}

struct Seen {
    std::uint32_t size = 0;
    formats::Flow flow = formats::Flow::Next;
    std::optional<std::uint32_t> target;
};

std::optional<Seen> decode_with(const formats::Decoder& decoder, std::uint32_t pc, const Bytes& code) {
    const Bytes mem = memory_with(pc, code);
    const std::optional<formats::DecodedInsn> d = decoder(mem, pc);
    if (!d) return std::nullopt;
    return Seen{d->size, d->flow, d->target};
}

#define CB_CHECK_FLOW(decoder, pc, code, want_size, want_flow, want_target)                         \
    do {                                                                                           \
        const std::optional<Seen> seen_ = decode_with(decoder, pc, code);                          \
        CB_CHECK(seen_.has_value());                                                               \
        if (seen_) {                                                                               \
            CB_CHECK_EQ(seen_->size, static_cast<std::uint32_t>(want_size));                       \
            CB_CHECK(seen_->flow == (want_flow));                                                  \
            CB_CHECK(seen_->target == (want_target));                                              \
        }                                                                                          \
    } while (false)

constexpr std::optional<std::uint32_t> kNoTarget = std::nullopt;

CB_TEST("source.isa.the_6502_says_how_control_leaves", [] {
    const formats::Decoder& d = m6502_decoder();
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x20, 0x0C, 0xC0}), 3, formats::Flow::Call, std::optional<std::uint32_t>(0xC00C));           // JSR
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x4C, 0x06, 0xC0}), 3, formats::Flow::Jump, std::optional<std::uint32_t>(0xC006));           // JMP abs
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x6C, 0x00, 0x02}), 3, formats::Flow::Indirect, kNoTarget);                                  // JMP (ind)
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x60}), 1, formats::Flow::Return, kNoTarget);                                                // RTS
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x40}), 1, formats::Flow::Return, kNoTarget);                                                // RTI
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0x00}), 1, formats::Flow::Halt, kNoTarget);                                                  // BRK
    CB_CHECK_FLOW(d, 0xC010, (Bytes{0x10, 0xFB}), 2, formats::Flow::Branch, std::optional<std::uint32_t>(0xC00D));              // BPL back 5
    CB_CHECK_FLOW(d, 0xC000, (Bytes{0xEA}), 1, formats::Flow::Next, kNoTarget);                                                  // NOP
    CB_CHECK(!decode_with(d, 0xC000, Bytes{0x02}).has_value());                                                                  // not an opcode
});

CB_TEST("source.isa.the_z80_says_how_control_leaves", [] {
    const formats::Decoder& d = z80_decoder();
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xCD, 0x34, 0x12}), 3, formats::Flow::Call, std::optional<std::uint32_t>(0x1234));             // CALL nn
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xC4, 0x34, 0x12}), 3, formats::Flow::Call, std::optional<std::uint32_t>(0x1234));             // CALL NZ,nn
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xCF}), 1, formats::Flow::Call, std::optional<std::uint32_t>(0x08));                           // RST $08
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xC3, 0x34, 0x12}), 3, formats::Flow::Jump, std::optional<std::uint32_t>(0x1234));             // JP nn
    CB_CHECK_FLOW(d, 0x100, (Bytes{0x18, 0x10}), 2, formats::Flow::Jump, std::optional<std::uint32_t>(0x112));                    // JR e
    CB_CHECK_FLOW(d, 0x100, (Bytes{0x20, 0x10}), 2, formats::Flow::Branch, std::optional<std::uint32_t>(0x112));                  // JR NZ,e
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xC9}), 1, formats::Flow::Return, kNoTarget);                                                  // RET
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xC0}), 1, formats::Flow::Next, kNoTarget);                                                    // RET NZ falls through
    CB_CHECK_FLOW(d, 0x100, (Bytes{0xE9}), 1, formats::Flow::Indirect, kNoTarget);                                                // JP (HL)
    CB_CHECK_FLOW(d, 0x100, (Bytes{0x76}), 1, formats::Flow::Next, kNoTarget);                                                    // HALT resumes after an interrupt
});

CB_TEST("source.isa.the_sm83_says_how_control_leaves", [] {
    const formats::Decoder& d = sm83_decoder();
    CB_CHECK_FLOW(d, 0x150, (Bytes{0xCD, 0x70, 0x01}), 3, formats::Flow::Call, std::optional<std::uint32_t>(0x170));
    CB_CHECK_FLOW(d, 0x150, (Bytes{0xD9}), 1, formats::Flow::Return, kNoTarget);                                  // RETI
    CB_CHECK_FLOW(d, 0x150, (Bytes{0xC8}), 1, formats::Flow::Next, kNoTarget);                                    // RET Z falls through
    CB_CHECK_FLOW(d, 0x150, (Bytes{0x10, 0x00}), 2, formats::Flow::Halt, kNoTarget);                              // STOP ends the flow
    CB_CHECK_FLOW(d, 0x150, (Bytes{0x20, 0xFA}), 2, formats::Flow::Branch, std::optional<std::uint32_t>(0x14C));   // JR NZ,e
    CB_CHECK(!decode_with(d, 0x150, Bytes{0xD3}).has_value());                                                    // one of the eleven that do not exist
});

CB_TEST("source.isa.the_68000_says_how_control_leaves", [] {
    const formats::Decoder& d = m68k_decoder();
    CB_CHECK_FLOW(d, 0x200, (Bytes{0x4E, 0xB9, 0x00, 0x00, 0x02, 0x20}), 6, formats::Flow::Call, std::optional<std::uint32_t>(0x220));   // JSR (xxx).L
    CB_CHECK_FLOW(d, 0x200, (Bytes{0x4E, 0x75}), 2, formats::Flow::Return, kNoTarget);                                                  // RTS
    CB_CHECK_FLOW(d, 0x200, (Bytes{0x60, 0xFE}), 2, formats::Flow::Jump, std::optional<std::uint32_t>(0x200));                         // BRA.S *
    CB_CHECK_FLOW(d, 0x200, (Bytes{0x66, 0x10}), 2, formats::Flow::Branch, std::optional<std::uint32_t>(0x212));                       // BNE.S
    // a TRAP is a branch to its handler: followed, but its handler is no function start
    const std::optional<Seen> trap = decode_with(d, 0x200, Bytes{0x4E, 0x41});
    CB_CHECK(trap && trap->flow == formats::Flow::Branch);
});

// The R3000A, read at the address it runs at, in the 2 MB the cartridge formats scan: a jump, return and indirect jump end the flow (after
// their delay slot, which a scan does not follow); a JAL is a call, a JALR a call with no known target, a conditional branch a branch.
CB_TEST("source.isa.the_r3000a_says_how_control_leaves", [] {
    const std::uint32_t pc = 0x1000;
    const auto little = [](std::uint32_t w) { return Bytes{static_cast<std::uint8_t>(w), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 24)}; };
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x27BDFFE0), 4, formats::Flow::Next, std::nullopt);                          // addiu $sp, $sp, -32
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x03E00008), 4, formats::Flow::Return, std::nullopt);                        // jr $ra
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x01000008), 4, formats::Flow::Indirect, std::nullopt);                      // jr $t0
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x0C004040), 4, formats::Flow::Call, std::optional<std::uint32_t>(0x10100));  // jal $10100
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x0100F809), 4, formats::Flow::Call, std::nullopt);                          // jalr $t0
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x08004040), 4, formats::Flow::Jump, std::optional<std::uint32_t>(0x10100));  // j $10100
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x10800003), 4, formats::Flow::Branch, std::optional<std::uint32_t>(0x1010));  // beq $a0, $zero, +3
    CB_CHECK_FLOW(mips_decoder(), pc, little(0x1000FFFF), 4, formats::Flow::Jump, std::optional<std::uint32_t>(0x1000));   // b (to itself)
    CB_CHECK(!decode_with(mips_decoder(), pc, little(0xFFFFFFFF)));                                                        // not an instruction
});

CB_TEST("source.isa.pattern_has", [] {
    CB_CHECK(pattern_has("LD (nn),A", "(nn)"));
    CB_CHECK(!pattern_has("LD A,n", "(nn)"));
    CB_CHECK(pattern_has("LDH A,(h)", "LDH"));
    CB_CHECK(pattern_has("anything", ""));
});

}  // namespace
}  // namespace cb::source::isa
