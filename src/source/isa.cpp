// source/isa.cpp -- the processors of the console and home-computer lanes, as the source writers use them (isa.hpp).

#include "source/isa.hpp"

#include "source/processors.hpp"

#include "decompile/arm.hpp"
#include "decompile/m6502.hpp"
#include "decompile/m68k.hpp"
#include "decompile/mips.hpp"
#include "decompile/sm83.hpp"
#include "decompile/z80.hpp"

namespace cb::source::isa {

namespace {

// ---- decoders ---------------------------------------------------------------------------------------------------

using formats::DecodedInsn;
namespace dz = decompile::z80;

// The Z80 family's categories (shared by the SM83 and the 68000) in the cartridge formats' terms.
formats::Flow flow_of(dz::Flow flow) {
    switch (flow) {
        case dz::Flow::Jump: return formats::Flow::Jump;
        case dz::Flow::Branch: return formats::Flow::Branch;
        case dz::Flow::Call:
        case dz::Flow::ConditionalCall: return formats::Flow::Call;
        case dz::Flow::Return: return formats::Flow::Return;
        case dz::Flow::Indirect: return formats::Flow::Indirect;
        case dz::Flow::Halt:
        case dz::Flow::ConditionalReturn:
        case dz::Flow::None: break;
    }
    return formats::Flow::Next;
}

// The SM83 as the Z80, and a STOP ends the flow.
std::optional<DecodedInsn> decode_sm83(ByteView mem, std::uint32_t pc) {
    const std::optional<decompile::sm83::Insn> insn = decompile::sm83::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = insn->size;
    if (insn->target) out.target = *insn->target;
    out.flow = insn->pattern == "STOP" ? formats::Flow::Halt : flow_of(insn->flow);
    return out;
}

// The ARM7TDMI: the address `pc` carries the state in its lowest bit (odd: Thumb), and so does a target. A jump, a return and an indirect jump
// end the flow (every instruction is conditional, so a conditional one falls through: the decoder says so); a BX through a register is
// `Indirect` and the writer resolves where it goes.
std::optional<DecodedInsn> decode_arm_as(ByteView mem, std::uint32_t pc, decompile::arm::Arch arch) {
    const std::optional<decompile::arm::Insn> insn = decompile::arm::decode(mem, pc & ~1u, pc, arch);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = insn->length;
    if (insn->target) out.target = *insn->target;
    switch (insn->flow) {
        case dz::Flow::Jump: out.flow = formats::Flow::Jump; break;
        case dz::Flow::Branch: out.flow = formats::Flow::Branch; break;
        case dz::Flow::Call: out.flow = formats::Flow::Call; break;
        case dz::Flow::Return: out.flow = formats::Flow::Return; break;
        case dz::Flow::Indirect: out.flow = formats::Flow::Indirect; break;
        default: break;
    }
    return out;
}

std::optional<DecodedInsn> decode_arm(ByteView mem, std::uint32_t pc) { return decode_arm_as(mem, pc, decompile::arm::Arch::V4T); }
// The ARM946E-S (ARMv5TE): the same, with BLX (a call whose target has the other state), CLZ and the rest of the ARMv5TE instructions.
std::optional<DecodedInsn> decode_arm9(ByteView mem, std::uint32_t pc) { return decode_arm_as(mem, pc, decompile::arm::Arch::V5TE); }
// The ARM11 (ARMv6K, the 3DS): ARMv5TE and the ARMv6 and VFP rows.
std::optional<DecodedInsn> decode_arm11(ByteView mem, std::uint32_t pc) { return decode_arm_as(mem, pc, decompile::arm::Arch::V6K); }

}  // namespace

const formats::Decoder& m6502_decoder() {
    static const formats::Decoder decoder = processors::described_decoder(processors::find("6502"));   // (the one decoder: its SLEIGH description)
    return decoder;
}
const formats::Decoder& m65sc02_decoder() {
    static const formats::Decoder decoder = processors::described_decoder(processors::find("65c02"));   // (the one decoder: its SLEIGH description)
    return decoder;
}

const formats::Decoder& z80_decoder() {
    static const formats::Decoder decoder = processors::described_decoder(processors::find("z80"));   // (the one decoder: its SLEIGH description)
    return decoder;
}
const formats::Decoder& sm83_decoder() {
    static const formats::Decoder decoder = decode_sm83;
    return decoder;
}
const formats::Decoder& mips_decoder() {
    static const formats::Decoder decoder = processors::described_decoder(processors::find("mips"));   // (the one decoder: its SLEIGH description)
    return decoder;
}
const formats::Decoder& arm_decoder() {
    static const formats::Decoder decoder = decode_arm;
    return decoder;
}
const formats::Decoder& arm9_decoder() {
    static const formats::Decoder decoder = decode_arm9;
    return decoder;
}
const formats::Decoder& arm11_decoder() {
    static const formats::Decoder decoder = decode_arm11;
    return decoder;
}
const formats::Decoder& m68k_decoder() {
    static const formats::Decoder decoder = processors::described_decoder(processors::find("m68k"));   // (the one decoder: its SLEIGH description)
    return decoder;
}

}  // namespace cb::source::isa
