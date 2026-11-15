// cb/source/isa.cpp -- the processors of the console and home-computer lanes, as the source writers use them (isa.hpp).

#include "cb/source/isa.hpp"

#include "cb/decompile/arm.hpp"
#include "cb/decompile/m6502.hpp"
#include "cb/decompile/m68k.hpp"
#include "cb/decompile/mips.hpp"
#include "cb/decompile/sm83.hpp"
#include "cb/decompile/z80.hpp"

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

std::optional<DecodedInsn> decode_6502(ByteView mem, std::uint32_t pc) {
    const std::optional<decompile::m6502::Insn> insn = decompile::m6502::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = insn->size;
    if (insn->target) out.target = *insn->target;
    const std::string_view m = insn->mnemonic;
    if (m == "JSR") out.flow = formats::Flow::Call;
    else if (m == "JMP") out.flow = insn->mode == decompile::m6502::Mode::Ind ? formats::Flow::Indirect : formats::Flow::Jump;
    else if (m == "RTS" || m == "RTI") out.flow = formats::Flow::Return;
    else if (m == "BRK") out.flow = formats::Flow::Halt;
    else if (decompile::m6502::is_branch(m)) out.flow = formats::Flow::Branch;
    return out;
}

// The Z80: a HALT resumes after an interrupt, so it falls through.
std::optional<DecodedInsn> decode_z80(ByteView mem, std::uint32_t pc) {
    const std::optional<dz::Insn> insn = dz::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = insn->size;
    if (insn->target) out.target = *insn->target;
    out.flow = flow_of(insn->flow);
    return out;
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

// The 68000: a TRAP is a branch to its handler (formats/code_scan: not a function start); a halt ends the flow.
std::optional<DecodedInsn> decode_68k(ByteView mem, std::uint32_t pc) {
    const std::optional<decompile::m68k::Insn> insn = decompile::m68k::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = insn->length;
    if (insn->target) out.target = *insn->target;
    out.flow = insn->flow == dz::Flow::Halt ? formats::Flow::Halt : flow_of(insn->flow);
    if (insn->mnemonic == "TRAP" && insn->flow == dz::Flow::Call) out.flow = formats::Flow::Branch;
    return out;
}

// The R3000A: `pc` is the address the code runs at, so a jump's target is in the same 256 MB region. A jump, a return and an indirect jump
// end the flow after their delay slot (which a code scan does not follow: it does not need to); a JALR is a call with no known target.
std::optional<DecodedInsn> decode_mips(ByteView mem, std::uint32_t pc) {
    const std::optional<decompile::mips::Insn> insn = decompile::mips::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out;
    out.size = 4;
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

// ---- encoders ---------------------------------------------------------------------------------------------------

// The failure the directive layer expects of a name that has no value yet.
Failure unknown(const dz::UnknownLabel& e) { return Failure(build::UndefinedSymbolError(e.name())); }

class Z80Syntax final : public build::Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return dz::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return dz::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
};

class M68kSyntax final : public build::Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return decompile::m68k::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return decompile::m68k::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
};

class MipsSyntax final : public build::Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return decompile::mips::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return decompile::mips::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
};

// The ARM7TDMI's: the state an instruction is assembled in is the lowest bit of its address (decompile/arm.hpp).
class ArmSyntax final : public build::Encoder {
public:
    explicit ArmSyntax(decompile::arm::Arch arch = decompile::arm::Arch::V4T) : arch_(arch) {}
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return decompile::arm::value(expr, labels, static_cast<std::uint32_t>(pc) & ~1u);
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return decompile::arm::assemble(line, static_cast<std::uint32_t>(pc), labels, arch_);
        } catch (const dz::UnknownLabel& e) {
            return unknown(e);
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }

private:
    decompile::arm::Arch arch_;
};

// The 6502's assembler says an unknown name as a ParseError ("unknown label or number NAME (line)"): the name is read out of it.
class M6502Syntax final : public build::m6502_asm::Encoder6502 {
public:
    Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const build::Labels& labels) const override {
        constexpr std::string_view kUnknown = "unknown label or number ";
        try {
            return decompile::m6502::assemble(line, pc, labels);
        } catch (const ParseError& e) {
            const std::string_view said = e.what();
            const std::size_t at = said.find(kUnknown);
            if (at == std::string_view::npos) return Failure(e.clone());
            const std::string_view name = said.substr(at + kUnknown.size());
            return Failure(build::UndefinedSymbolError(std::string(name.substr(0, name.find(" (")))));
        }
    }
    bool is_branch(std::string_view mnemonic) const override { return decompile::m6502::is_branch(mnemonic); }
};

}  // namespace

const formats::Decoder& m6502_decoder() {
    static const formats::Decoder decoder = decode_6502;
    return decoder;
}
const formats::Decoder& z80_decoder() {
    static const formats::Decoder decoder = decode_z80;
    return decoder;
}
const formats::Decoder& sm83_decoder() {
    static const formats::Decoder decoder = decode_sm83;
    return decoder;
}
const formats::Decoder& mips_decoder() {
    static const formats::Decoder decoder = decode_mips;
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
const formats::Decoder& m68k_decoder() {
    static const formats::Decoder decoder = decode_68k;
    return decoder;
}

const build::Encoder& z80_encoder() {
    static const Z80Syntax syntax;
    return syntax;
}
const build::Encoder& mips_encoder() {
    static const MipsSyntax syntax;
    return syntax;
}
const build::Encoder& arm_encoder() {
    static const ArmSyntax syntax;
    return syntax;
}
const build::Encoder& arm9_encoder() {
    static const ArmSyntax syntax(decompile::arm::Arch::V5TE);
    return syntax;
}
const build::Encoder& m68k_encoder() {
    static const M68kSyntax syntax;
    return syntax;
}
const build::m6502_asm::Encoder6502& m6502_encoder() {
    static const M6502Syntax syntax;
    return syntax;
}

}  // namespace cb::source::isa
