// cb-unity: alone
// engine/pcode_ops_arm32.cpp -- see pcode_ops_arm32.hpp. The ARM descriptions name an operation and give it no p-code; its behavior is written here from the ARM ARM (A2.3
// saturation, A8 for each instruction), with the lane work of NEON in engine/pcode_vector.hpp.
#include "engine/pcode_ops_arm32.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace cb::engine::pcode_ops {

namespace {

using Byte = std::uint8_t;

std::uint64_t readUnsigned(const Byte* p, unsigned size) {
    std::uint64_t v = 0;
    for (unsigned k = 0; k < size && k < 8; ++k) v |= static_cast<std::uint64_t>(p[k]) << (8 * k);
    return v;
}
std::int64_t readSigned(const Byte* p, unsigned size) {
    const std::uint64_t v = readUnsigned(p, size);
    if (size >= 8) return static_cast<std::int64_t>(v);
    const unsigned shift = 64 - 8 * size;
    return static_cast<std::int64_t>(v << shift) >> shift;
}
void writeUnsigned(Byte* p, unsigned size, std::uint64_t v) {
    for (unsigned k = 0; k < size && k < 8; ++k) p[k] = static_cast<Byte>(v >> (8 * k));
}

// The range of an n-bit number (n from 1 to 63).
std::int64_t signedHigh(unsigned n) { return (std::int64_t{1} << (n - 1)) - 1; }
std::int64_t signedLow(unsigned n) { return -(std::int64_t{1} << (n - 1)); }

// SignedSatQ and UnsignedSatQ of A2.2.1: the (signed) value clamped to n bits, signed or unsigned; the second operand of the description's helpers is the number of bits.
bool signedSaturate(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const unsigned n = static_cast<unsigned>(readUnsigned(c.in[1], c.inSize[1]));
    if (n == 0 || n > 63) return false;
    const std::int64_t v = readSigned(c.in[0], c.inSize[0]);
    writeUnsigned(c.out, c.outSize, static_cast<std::uint64_t>(std::clamp(v, signedLow(n), signedHigh(n))));
    return true;
}
bool signedDoesSaturate(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const unsigned n = static_cast<unsigned>(readUnsigned(c.in[1], c.inSize[1]));
    if (n == 0 || n > 63) return false;
    const std::int64_t v = readSigned(c.in[0], c.inSize[0]);
    c.out[0] = v < signedLow(n) || v > signedHigh(n) ? 1 : 0;
    return true;
}
bool unsignedSaturate(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const unsigned n = static_cast<unsigned>(readUnsigned(c.in[1], c.inSize[1]));
    if (n > 63) return false;
    const std::int64_t v = readSigned(c.in[0], c.inSize[0]);
    const std::int64_t high = (std::int64_t{1} << n) - 1;
    writeUnsigned(c.out, c.outSize, static_cast<std::uint64_t>(std::clamp<std::int64_t>(v, 0, high)));
    return true;
}
bool unsignedDoesSaturate(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const unsigned n = static_cast<unsigned>(readUnsigned(c.in[1], c.inSize[1]));
    if (n > 63) return false;
    const std::int64_t v = readSigned(c.in[0], c.inSize[0]);
    c.out[0] = v < 0 || v > (std::int64_t{1} << n) - 1 ? 1 : 0;
    return true;
}
bool absolute(Emu&, Call& c, void*) {
    if (c.nIn < 1 || c.outSize == 0) return false;
    const std::int64_t v = readSigned(c.in[0], c.inSize[0]);
    writeUnsigned(c.out, c.outSize, static_cast<std::uint64_t>(v < 0 ? -v : v));
    return true;
}
bool reverseBitOrder(Emu&, Call& c, void*) {   // RBIT
    if (c.nIn < 1 || c.outSize != c.inSize[0]) return false;
    for (unsigned k = 0; k < c.outSize; ++k) {
        Byte b = c.in[0][c.outSize - 1 - k], r = 0;
        for (unsigned bit = 0; bit < 8; ++bit) r = static_cast<Byte>(r | ((b >> bit) & 1) << (7 - bit));
        c.out[k] = r;
    }
    return true;
}

// The VFP's conversions between integers and floats (VCVT): a double or a single by the size field of the instruction's word (bits 9:8 are 11 for a double).
bool vectorToFloat(Emu&, Call& c, void* context, bool is_signed) {
    const Arm32Context& ctx = *static_cast<Arm32Context*>(context);
    const std::uint32_t source = static_cast<std::uint32_t>(readUnsigned(c.in[0], c.inSize[0]));
    const double v = is_signed ? static_cast<double>(static_cast<std::int32_t>(source)) : static_cast<double>(source);
    if ((ctx.word >> 8 & 3) == 3) writeUnsigned(c.out, c.outSize, std::bit_cast<std::uint64_t>(v));
    else writeUnsigned(c.out, c.outSize, std::bit_cast<std::uint32_t>(static_cast<float>(v)));
    return true;
}
bool vectorSignedToFloat(Emu& e, Call& c, void* x) { return vectorToFloat(e, c, x, true); }
bool vectorUnsignedToFloat(Emu& e, Call& c, void* x) { return vectorToFloat(e, c, x, false); }
bool vectorFromFloat(Emu&, Call& c, void* context, bool is_signed) {
    const Arm32Context& ctx = *static_cast<Arm32Context*>(context);
    const std::uint64_t bits = readUnsigned(c.in[0], c.inSize[0]);
    const double v = (ctx.word >> 8 & 3) == 3 ? std::bit_cast<double>(bits) : static_cast<double>(std::bit_cast<float>(static_cast<std::uint32_t>(bits)));
    std::uint32_t result;
    if (is_signed) result = std::isnan(v) ? 0u : static_cast<std::uint32_t>(v >= 2147483647.0 ? INT32_MAX : v <= -2147483648.0 ? INT32_MIN : static_cast<std::int32_t>(v));
    else result = std::isnan(v) || v <= 0 ? 0u : v >= 4294967295.0 ? 0xFFFFFFFFu : static_cast<std::uint32_t>(v);
    writeUnsigned(c.out, c.outSize, result);
    return true;
}
bool vectorFloatToSigned(Emu& e, Call& c, void* x) { return vectorFromFloat(e, c, x, true); }
bool vectorFloatToUnsigned(Emu& e, Call& c, void* x) { return vectorFromFloat(e, c, x, false); }

// LDREX, STREX: one processor runs, so the exclusive monitor always holds.
bool hasExclusiveAccess(Emu&, Call& c, void*) {
    if (c.outSize == 0) return false;
    c.out[0] = 1;
    return true;
}

}  // namespace

const Table& arm32_table() {
    static const Table table{
        {"SignedSaturate", signedSaturate},         {"SignedDoesSaturate", signedDoesSaturate},   {"UnsignedSaturate", unsignedSaturate},
        {"UnsignedDoesSaturate", unsignedDoesSaturate}, {"Absolute", absolute},                   {"ReverseBitOrder", reverseBitOrder},
        {"VectorSignedToFloat", vectorSignedToFloat}, {"VectorUnsignedToFloat", vectorUnsignedToFloat}, {"VectorFloatToSigned", vectorFloatToSigned},
        {"VectorFloatToUnsigned", vectorFloatToUnsigned}, {"hasExclusiveAccess", hasExclusiveAccess},
    };
    return table;
}

Binding bind_arm32(Emu& emu, Arm32Context* context) { return bind(emu, arm32_table(), context); }

std::vector<std::string> arm32_unserved(Emu& emu) {
    std::vector<std::string> out;
    for (std::size_t i = 0, n = emu.userOpCount(); i < n; ++i) {
        const std::string& name = emu.userOpName(i);
        if (arm32_table().find(name)) continue;
        if (name.rfind("coprocessor", 0) == 0 || name.rfind("software", 0) == 0 || name.rfind("coproc_", 0) == 0) continue;
        out.push_back(name);
    }
    return out;
}

}  // namespace cb::engine::pcode_ops
