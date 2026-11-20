// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_mips_le.cpp -- the R5900 and the Allegrex for the source writers (isa_mips_le.hpp).

#include "source/isa_mips_le.hpp"

#include "decompile/mips.hpp"

namespace cb::source::isa {

namespace {

namespace dm = decompile::mips;
namespace dz = decompile::z80;

std::optional<formats::DecodedInsn> decode_as(ByteView mem, std::uint32_t pc, dm::Variant variant) {
    const std::optional<dm::Insn> insn = dm::decode(mem, pc, pc, variant);
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

std::optional<formats::DecodedInsn> decode_r5900(ByteView mem, std::uint32_t pc) { return decode_as(mem, pc, dm::Variant::R5900); }
std::optional<formats::DecodedInsn> decode_allegrex(ByteView mem, std::uint32_t pc) { return decode_as(mem, pc, dm::Variant::Allegrex); }

class Syntax final : public build::Encoder {
public:
    explicit Syntax(dm::Variant variant) : variant_(variant) {}
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
            return dm::assemble(line, static_cast<std::uint32_t>(pc), labels, variant_);
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }

private:
    dm::Variant variant_;
};

}  // namespace

const formats::Decoder& r5900_decoder() {
    static const formats::Decoder decoder = decode_r5900;
    return decoder;
}
const build::Encoder& r5900_encoder() {
    static const Syntax syntax(dm::Variant::R5900);
    return syntax;
}
const formats::Decoder& allegrex_decoder() {
    static const formats::Decoder decoder = decode_allegrex;
    return decoder;
}
const build::Encoder& allegrex_encoder() {
    static const Syntax syntax(dm::Variant::Allegrex);
    return syntax;
}

}  // namespace cb::source::isa
