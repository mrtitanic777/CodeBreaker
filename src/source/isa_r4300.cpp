// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_r4300.cpp -- the R4300i for the source writers (isa_r4300.hpp).

#include "source/isa_r4300.hpp"

#include "decompile/mips.hpp"

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

}  // namespace

const formats::Decoder& r4300_decoder() {
    static const formats::Decoder decoder = decode_r4300;
    return decoder;
}

}  // namespace cb::source::isa
