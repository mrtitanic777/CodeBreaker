// cb-unity: alone (file-local names that other files in a batch also use)
// cb/source/isa_r4300.cpp -- the R4300i for the source writers (isa_r4300.hpp).

#include "cb/source/isa_r4300.hpp"

#include "cb/decompile/mips.hpp"

namespace cb::source::isa {

namespace {

namespace dm = decompile::mips;
namespace dz = decompile::z80;

std::optional<formats::DecodedInsn> decode_r4300(ByteView mem, std::uint32_t pc) {
    const std::optional<dm::Insn> insn = dm::decode(mem, pc, pc, dm::Variant::R4300);
    if (!insn) return std::nullopt;
    formats::DecodedInsn out;
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

class R4300Syntax final : public build::Encoder {
public:
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return dm::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return dm::assemble(line, static_cast<std::uint32_t>(pc), labels, dm::Variant::R4300);
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
};

}  // namespace

const formats::Decoder& r4300_decoder() {
    static const formats::Decoder decoder = decode_r4300;
    return decoder;
}

const build::Encoder& r4300_encoder() {
    static const R4300Syntax syntax;
    return syntax;
}

}  // namespace cb::source::isa
