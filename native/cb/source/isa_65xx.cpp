#include "cb/source/isa_65xx.hpp"

#include "cb/decompile/huc6280.hpp"
#include "cb/decompile/m6502.hpp"
#include "cb/decompile/w65816.hpp"

namespace cb::source::isa {

namespace {

namespace m6502 = decompile::m6502;

std::optional<formats::DecodedInsn> decode_65sc02(ByteView mem, std::uint32_t pc) {
    const std::optional<m6502::Insn> insn = m6502::decode(mem, pc, pc, m6502::Variant::Cmos);
    if (!insn) return std::nullopt;
    formats::DecodedInsn out;
    out.size = insn->size;
    if (insn->target) out.target = *insn->target;
    const std::string_view m = insn->mnemonic;
    if (m == "JSR") out.flow = formats::Flow::Call;
    else if (m == "JMP") out.flow = insn->mode == m6502::Mode::Abs ? formats::Flow::Jump : formats::Flow::Indirect;
    else if (m == "BRA") out.flow = formats::Flow::Jump;
    else if (m == "RTS" || m == "RTI") out.flow = formats::Flow::Return;
    else if (m == "BRK") out.flow = formats::Flow::Halt;
    else if (m6502::is_branch(m)) out.flow = formats::Flow::Branch;
    return out;
}

// An unknown name is a ParseError ("unknown label or number NAME (line)"): the name is read out of it for the directive layer.
class Cmos6502Syntax final : public build::m6502_asm::Encoder6502 {
public:
    Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const build::Labels& labels) const override {
        constexpr std::string_view kUnknown = "unknown label or number ";
        try {
            return m6502::assemble(line, pc, labels, m6502::Variant::Cmos);
        } catch (const ParseError& e) {
            const std::string_view said = e.what();
            const std::size_t at = said.find(kUnknown);
            if (at == std::string_view::npos) return Failure(e.clone());
            const std::string_view name = said.substr(at + kUnknown.size());
            return Failure(build::UndefinedSymbolError(std::string(name.substr(0, name.find(" (")))));
        }
    }
    bool is_branch(std::string_view mnemonic) const override { return m6502::is_branch(mnemonic); }
};

// The HuC6280's assembler says an unknown name as the 65C02's does: a ParseError naming it.
class Huc6280Syntax final : public build::m6502_asm::Encoder6502 {
public:
    Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const build::Labels& labels) const override {
        constexpr std::string_view kUnknown = "unknown label or number ";
        try {
            return decompile::huc6280::assemble(line, pc, labels);
        } catch (const ParseError& e) {
            const std::string_view said = e.what();
            const std::size_t at = said.find(kUnknown);
            if (at == std::string_view::npos) return Failure(e.clone());
            const std::string_view name = said.substr(at + kUnknown.size());
            return Failure(build::UndefinedSymbolError(std::string(name.substr(0, name.find(" (")))));
        }
    }
    bool is_branch(std::string_view mnemonic) const override { return decompile::huc6280::is_branch(mnemonic); }
    std::size_t branch_length(std::string_view mnemonic) const override {
        if (mnemonic.size() == 4 && (mnemonic.starts_with("BBR") || mnemonic.starts_with("BBS"))) return 3;   // a zero-page byte and a displacement
        return is_branch(mnemonic) ? 2 : 0;
    }
};

// The 65C816's: the register widths come from the directive layer (`.a16`, `.i16`).
class W65816Syntax final : public build::m6502_asm::Encoder6502 {
public:
    Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const build::Labels& labels) const override { return assemble_with(line, pc, labels, {}); }
    Result<Bytes> assemble_with(std::string_view line, std::uint32_t pc, const build::Labels& labels, Widths widths) const override {
        constexpr std::string_view kUnknown = "unknown label or number ";
        try {
            return decompile::w65816::assemble(line, pc, labels, widths.a16, widths.i16);
        } catch (const ParseError& e) {
            const std::string_view said = e.what();
            const std::size_t at = said.find(kUnknown);
            if (at == std::string_view::npos) return Failure(e.clone());
            const std::string_view name = said.substr(at + kUnknown.size());
            return Failure(build::UndefinedSymbolError(std::string(name.substr(0, name.find(" (")))));
        }
    }
    bool is_branch(std::string_view mnemonic) const override { return decompile::w65816::is_branch(mnemonic); }
    std::size_t branch_length(std::string_view mnemonic) const override {
        if (mnemonic == "BRL" || mnemonic == "PER") return 3;   // a 16-bit displacement
        return is_branch(mnemonic) ? 2 : 0;
    }
};

}  // namespace

const formats::Decoder& m65sc02_decoder() {
    static const formats::Decoder decoder = decode_65sc02;
    return decoder;
}

const build::m6502_asm::Encoder6502& m65sc02_encoder() {
    static const Cmos6502Syntax syntax;
    return syntax;
}

const build::m6502_asm::Encoder6502& huc6280_encoder() {
    static const Huc6280Syntax syntax;
    return syntax;
}

const build::m6502_asm::Encoder6502& w65816_encoder() {
    static const W65816Syntax syntax;
    return syntax;
}

}  // namespace cb::source::isa
