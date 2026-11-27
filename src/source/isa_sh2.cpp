// source/isa_sh2.cpp -- the SH-2 as the source writers and the cartridge formats use it (isa_sh2.hpp).

#include "source/isa_sh2.hpp"

#include "decompile/sh2.hpp"

namespace cb::source::isa {

namespace {

namespace dz = decompile::z80;

std::optional<formats::DecodedInsn> decode_sh(decompile::sh2::Variant variant, ByteView mem, std::uint32_t pc) {
    if (pc + 2 > mem.size()) return std::nullopt;
    const std::optional<decompile::sh2::Insn> insn = decompile::sh2::decode(mem, pc, pc, variant);
    if (!insn) return std::nullopt;
    formats::DecodedInsn out;
    out.size = 2;
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

const formats::Decoder& sh2_decoder() {
    static const formats::Decoder decoder = [](ByteView mem, std::uint32_t pc) { return decode_sh(decompile::sh2::Variant::Sh2, mem, pc); };
    return decoder;
}

const formats::Decoder& sh4_decoder() {
    static const formats::Decoder decoder = [](ByteView mem, std::uint32_t pc) { return decode_sh(decompile::sh2::Variant::Sh4, mem, pc); };
    return decoder;
}

}  // namespace cb::source::isa
