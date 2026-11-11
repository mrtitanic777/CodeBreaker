#include "cb/decompile/vmx128.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <format>
#include <map>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::vmx128 {

namespace {

constexpr std::array<Row, 77> kRows = {{
    {"lvewx128", 0x10000083, 0xFC0007F3, Form::Load, "loadVectorElementWordIndexed"},
    {"lvlx128", 0x10000403, 0xFC0007F3, Form::Load, "loadVectorLeftIndexed"},
    {"lvlxl128", 0x10000603, 0xFC0007F3, Form::Load, "loadVectorLeftIndexedLRU"},
    {"lvrx128", 0x10000443, 0xFC0007F3, Form::Load, "loadVectorRightIndexed"},
    {"lvrxl128", 0x10000643, 0xFC0007F3, Form::Load, "loadVectorRightIndexedLRU"},
    {"lvsl128", 0x10000003, 0xFC0007F3, Form::Load, "loadVectorForShiftLeft"},
    {"lvsr128", 0x10000043, 0xFC0007F3, Form::Load, "loadVectorForShiftRight"},
    {"lvx128", 0x100000C3, 0xFC0007F3, Form::Load, "=load"},
    {"lvxl128", 0x100002C3, 0xFC0007F3, Form::Load, "=load"},
    {"stvewx128", 0x10000183, 0xFC0007F3, Form::Store, "storeVectorElementWordIndexed"},
    {"stvlx128", 0x10000503, 0xFC0007F3, Form::Store, "storeVectorLeftIndexed"},
    {"stvlxl128", 0x10000703, 0xFC0007F3, Form::Store, "storeVectorLeftIndexedLRU"},
    {"stvrx128", 0x10000543, 0xFC0007F3, Form::Store, "storeVectorRightIndexed"},
    {"stvrxl128", 0x10000743, 0xFC0007F3, Form::Store, "storeVectorRightIndexedLRU"},
    {"stvx128", 0x100001C3, 0xFC0007F3, Form::Store, "=store"},
    {"stvxl128", 0x100003C3, 0xFC0007F3, Form::Store, "=store"},
    {"vsldoi128", 0x10000010, 0xFC000010, Form::Sldoi, "=sldoi"},
    {"vaddfp128", 0x14000010, 0xFC0003D0, Form::Abc, "vectorAddFloatingPoint"},
    {"vand128", 0x14000210, 0xFC0003D0, Form::Abc, "=vA & vB"},
    {"vandc128", 0x14000250, 0xFC0003D0, Form::Abc, "=vA & ~vB"},
    {"vmaddcfp128", 0x14000110, 0xFC0003D0, Form::Madd, "vectorMultiplyAddFloatingPointToD"},
    {"vmaddfp128", 0x140000D0, 0xFC0003D0, Form::Madd, "vectorMultiplyAddFloatingPoint"},
    {"vmsum3fp128", 0x14000190, 0xFC0003D0, Form::Abc, "vectorMultiplySum3FloatingPoint"},
    {"vmsum4fp128", 0x140001D0, 0xFC0003D0, Form::Abc, "vectorMultiplySum4FloatingPoint"},
    {"vmulfp128", 0x14000090, 0xFC0003D0, Form::Abc, "vectorMultiplyFloatingPoint"},
    {"vnmsubfp128", 0x14000150, 0xFC0003D0, Form::Madd, "vectorNegativeMultiplySubtractFloatingPoint"},
    {"vnor128", 0x14000290, 0xFC0003D0, Form::Abc, "=~(vA | vB)"},
    {"vor128", 0x140002D0, 0xFC0003D0, Form::Abc, "=vA | vB"},
    {"vperm128", 0x14000000, 0xFC000210, Form::Perm, "vectorPermute"},
    {"vpkshss128", 0x14000200, 0xFC0003D0, Form::Abc, "vectorPackSignedHalfWordSignedSaturate"},
    {"vpkshus128", 0x14000240, 0xFC0003D0, Form::Abc, "vectorPackSignedHalfWordUnsignedSaturate"},
    {"vpkswss128", 0x14000280, 0xFC0003D0, Form::Abc, "vectorPackSignedWordSignedSaturate"},
    {"vpkswus128", 0x140002C0, 0xFC0003D0, Form::Abc, "vectorPackSignedWordUnsignedSaturate"},
    {"vpkuhum128", 0x14000300, 0xFC0003D0, Form::Abc, "vectorPackUnsignedHalfWordUnsignedModulo"},
    {"vpkuhus128", 0x14000340, 0xFC0003D0, Form::Abc, "vectorPackUnsignedHalfWordUnsignedSaturate"},
    {"vpkuwum128", 0x14000380, 0xFC0003D0, Form::Abc, "vectorPackUnsignedWordUnsignedModulo"},
    {"vpkuwus128", 0x140003C0, 0xFC0003D0, Form::Abc, "vectorPackUnsignedWordUnsignedSaturate"},
    {"vsel128", 0x14000350, 0xFC0003D0, Form::Abc, "=(vA & ~vD) | (vB & vD)"},
    {"vslo128", 0x14000390, 0xFC0003D0, Form::Abc, "vectorShiftLeftByOctet"},
    {"vsro128", 0x140003D0, 0xFC0003D0, Form::Abc, "vectorShiftRightByOctet"},
    {"vsubfp128", 0x14000050, 0xFC0003D0, Form::Abc, "vectorSubtractFloatingPoint"},
    {"vxor128", 0x14000310, 0xFC0003D0, Form::Abc, "=vA ^ vB"},
    {"vcfpsxws128", 0x18000230, 0xFC0007F0, Form::Bimm, "vectorConvertToSignedFixedPointWordSaturate"},
    {"vcfpuxws128", 0x18000270, 0xFC0007F0, Form::Bimm, "vectorConvertToUnsignedFixedPointWordSaturate"},
    {"vcmpbfp128", 0x18000180, 0xFC000390, Form::Cmp, "vectorCompareBoundsFloatingPoint"},
    {"vcmpeqfp128", 0x18000000, 0xFC000390, Form::Cmp, "vectorCompareEqualToFloatingPoint"},
    {"vcmpequw128", 0x18000200, 0xFC000390, Form::Cmp, "vectorCompareEqualToUnsignedWord"},
    {"vcmpgefp128", 0x18000080, 0xFC000390, Form::Cmp, "vectorCompareGreaterThanOrEqualToFloatingPoint"},
    {"vcmpgtfp128", 0x18000100, 0xFC000390, Form::Cmp, "vectorCompareGreaterThanFloatingPoint"},
    {"vcsxwfp128", 0x180002B0, 0xFC0007F0, Form::Bimm, "vectorConvertFromSignedFixedPointWord"},
    {"vcuxwfp128", 0x180002F0, 0xFC0007F0, Form::Bimm, "vectorConvertFromUnsignedFixedPointWord"},
    {"vexptefp128", 0x180006B0, 0xFC1F07F0, Form::B, "vector2RaisedToTheExponentEstimateFloatingPoint"},
    {"vlogefp128", 0x180006F0, 0xFC1F07F0, Form::B, "vectorLog2EstimateFloatingPoint"},
    {"vmaxfp128", 0x18000280, 0xFC0003D0, Form::Abc, "vectorMaximumFloatingPoint"},
    {"vminfp128", 0x180002C0, 0xFC0003D0, Form::Abc, "vectorMinimumFloatingPoint"},
    {"vmrghw128", 0x18000300, 0xFC0003D0, Form::Abc, "vectorMergeHighWord"},
    {"vmrglw128", 0x18000340, 0xFC0003D0, Form::Abc, "vectorMergeLowWord"},
    {"vpermwi128", 0x18000210, 0xFC000630, Form::Permwi, "vectorPermuteWordImmediate"},
    {"vpkd3d128", 0x18000610, 0xFC000730, Form::D3d, "vectorPackD3D"},
    {"vrefp128", 0x18000630, 0xFC1F07F0, Form::B, "vectorReciprocalEstimateFloatingPoint"},
    {"vrfim128", 0x18000330, 0xFC1F07F0, Form::B, "vectorRoundToFloatingPointIntegerTowardMinusInfinity"},
    {"vrfin128", 0x18000370, 0xFC1F07F0, Form::B, "vectorRoundToFloatingPointIntegerNearest"},
    {"vrfip128", 0x180003B0, 0xFC1F07F0, Form::B, "vectorRoundToFloatingPointIntegerTowardPluInfinity"},
    {"vrfiz128", 0x180003F0, 0xFC1F07F0, Form::B, "vectorRoundToFloatingPointIntegerTowardZero"},
    {"vrlimi128", 0x18000710, 0xFC000730, Form::Rlimi, "vectorRotateLeftImmediateAndMaskInsert"},
    {"vrlw128", 0x18000050, 0xFC0003D0, Form::Abc, "vectorRotateLeftIntegerWord"},
    {"vrsqrtefp128", 0x18000670, 0xFC1F07F0, Form::B, "vectorReciprocalSquareRootEstimateFloatingPoint"},
    {"vslw128", 0x180000D0, 0xFC0003D0, Form::Abc, "vectorShiftLeftIntegerWord"},
    {"vspltisw128", 0x18000770, 0xFC0007F0, Form::Simm, "=splat"},
    {"vspltw128", 0x18000730, 0xFC0007F0, Form::Bimm, "vectorSplatWord"},
    {"vsraw128", 0x18000150, 0xFC0003D0, Form::Abc, "vectorShiftRightAlgebraicWord"},
    {"vsrw128", 0x180001D0, 0xFC0003D0, Form::Abc, "vectorShiftRightWord"},
    {"vupkd3d128", 0x180007F0, 0xFC0007F0, Form::Bimm, "vectorUnpackD3D"},
    {"vupkhsb128", 0x18000380, 0xFC1F07F0, Form::B, "vectorUnpackHighSignedByte"},
    {"vupkhsh128", 0x180007A0, 0xFC1F07F0, Form::B, "vectorUnpackHighSignedHalfWord"},
    {"vupklsb128", 0x180003C0, 0xFC1F07F0, Form::B, "vectorUnpackLowSignedByte"},
    {"vupklsh128", 0x180007E0, 0xFC1F07F0, Form::B, "vectorUnpackLowSignedHalfWord"},
}};

constexpr std::array<X31Row, 8> kX31 = {{
    {"lvlx", 519, Form::Load, "loadVectorLeftIndexed"},
    {"lvlxl", 775, Form::Load, "loadVectorLeftIndexedLRU"},
    {"lvrx", 551, Form::Load, "loadVectorRightIndexed"},
    {"lvrxl", 807, Form::Load, "loadVectorRightIndexedLRU"},
    {"stvlx", 647, Form::Store, "storeVectorLeftIndexed"},
    {"stvlxl", 903, Form::Store, "storeVectorLeftIndexedLRU"},
    {"stvrx", 679, Form::Store, "storeVectorRightIndexed"},
    {"stvrxl", 935, Form::Store, "storeVectorRightIndexedLRU"},
}};

// Most specific (widest mask) first, so a broad row never shadows a narrow one; rows of equal width keep the table's order.
const std::vector<const Row*>& rows_by_specificity() {
    static const std::vector<const Row*> sorted = [] {
        std::vector<const Row*> v;
        for (const Row& r : kRows) v.push_back(&r);
        std::stable_sort(v.begin(), v.end(), [](const Row* a, const Row* b) { return std::popcount(a->mask) > std::popcount(b->mask); });
        return v;
    }();
    return sorted;
}

// "r3", or "0" for r0 (rA 0 is the literal zero in an address).
std::string base_register(std::uint32_t ra) { return ra ? std::format("r{}", ra) : "0"; }

std::string operands(Form form, std::uint32_t w) {
    const std::uint32_t d = vd128(w), a = va128(w), b = vb128(w), c = vc128(w);
    const std::uint32_t ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    switch (form) {
        case Form::Load:
        case Form::Store: return std::format("v{},{},r{}", d, base_register(ra), rb);
        case Form::Sldoi: return std::format("v{},v{},v{},{}", d, a, b, (w >> 6) & 0xF);
        case Form::Perm: return std::format("v{},v{},v{},v{}", d, a, b, c);
        case Form::Madd: return std::format("v{},v{},v{},v{}", d, a, d, b);
        case Form::Simm: {
            const std::int32_t s = static_cast<std::int32_t>((w >> 16) & 31);
            return std::format("v{},{}", d, s >= 16 ? s - 32 : s);
        }
        case Form::Bimm: return std::format("v{},v{},{}", d, b, (w >> 16) & 31);
        case Form::Permwi: return std::format("v{},v{},{}", d, b, (((w >> 6) & 7) << 5) | ((w >> 16) & 31));
        case Form::D3d: return std::format("v{},v{},{},{},{}", d, b, (w >> 18) & 7, (w >> 16) & 3, (w >> 6) & 3);
        case Form::Rlimi: return std::format("v{},v{},{},{}", d, b, (w >> 16) & 31, (w >> 6) & 3);
        case Form::B: return std::format("v{},v{}", d, b);
        case Form::Abc:
        case Form::Cmp: break;
    }
    return std::format("v{},v{},v{}", d, a, b);
}

}  // namespace

std::span<const Row> rows() { return kRows; }
std::span<const X31Row> x31_rows() { return kX31; }

std::optional<Decoded> decode(std::uint32_t w) {
    if (w >> 26 == 31 && !(w & 1)) {
        const std::uint32_t xo = (w >> 1) & 0x3FF;
        for (const X31Row& x : kX31) {
            if (x.extended_opcode == xo) {
                // (these name AltiVec's 32 registers: the destination is the plain five-bit field)
                return Decoded{std::string(x.mnemonic), std::format("v{},{},r{}", (w >> 21) & 31, base_register((w >> 16) & 31), (w >> 11) & 31)};
            }
        }
    }
    if ((w >> 26) != 4 && (w >> 26) != 5 && (w >> 26) != 6) return std::nullopt;
    for (const Row* r : rows_by_specificity()) {
        if ((w & r->mask) == r->pattern) {
            std::string mnemonic(r->mnemonic);
            if (r->form == Form::Cmp && (w & 0x40)) mnemonic += '.';
            return Decoded{std::move(mnemonic), operands(r->form, w)};
        }
    }
    return std::nullopt;
}

std::optional<std::string> text(std::uint32_t word) {
    const std::optional<Decoded> d = decode(word);
    if (!d) return std::nullopt;
    return d->mnemonic + " " + d->operands;
}

Coverage coverage(std::span<const std::uint32_t> code_words) {
    Coverage c;
    for (std::uint32_t w : code_words) {
        const std::uint32_t primary = w >> 26;
        if (primary == 4 || primary == 5 || primary == 6) {
            ++c.vector_words;
            if (decode(w)) ++c.vmx128;
        }
    }
    c.other = c.vector_words - c.vmx128;
    return c;
}

// ---- the SLEIGH --------------------------------------------------------------------------------------------------
// Registers: v0..v31 are AltiVec's (the top 32 VSX registers, vs32..vs63); v32..v127 are new, at offset 0x5000 (free in the
// PowerPC register space: VSX ends at 0x4400, the context at 0x6000). A register field's high bits pick one of four banks
// of 32; each bank is a subtable constructor.

namespace {

using BitRange = std::pair<int, int>;   // (lo, hi) as the SLEIGH token field counts them

const std::array<std::pair<std::string_view, BitRange>, 19> kFields = {{
    {"vDlo", {21, 25}}, {"vDhi", {2, 3}}, {"vAlo", {16, 20}}, {"vAb5", {5, 5}}, {"vAb6", {10, 10}},
    {"vBlo", {11, 15}}, {"vBhi", {0, 1}}, {"vC3", {6, 8}}, {"x128OP", {26, 31}}, {"x128SHB", {6, 9}},
    {"x128IMM", {16, 20}}, {"x128SIMM", {16, 20}}, {"x128PERMHI", {6, 8}}, {"x128D3D", {18, 20}},
    {"x128PACK", {16, 17}}, {"x128Z", {6, 7}}, {"x128RC", {6, 6}}, {"x128RA", {16, 20}}, {"x128RB", {11, 15}},
}};

BitRange field_range(std::string_view name) {
    for (const auto& [n, r] : kFields) {
        if (n == name) return r;
    }
    CB_ASSERT(false, "unknown SLEIGH field");
    return {};
}

std::string bank(int base) {
    std::string out;
    for (int i = 0; i < 32; ++i) out += std::format("{}{}{}", i ? " " : "", base == 0 ? "vs" : "v", base == 0 ? 32 + i : base + i);
    return out;
}

// Contiguous runs (lo, hi) of the set bits.
std::vector<BitRange> runs(std::uint32_t bits) {
    std::vector<BitRange> out;
    std::optional<int> lo;
    for (int i = 0; i < 33; ++i) {
        const bool on = i < 32 && ((bits >> i) & 1);
        if (on && !lo) {
            lo = i;
        } else if (!on && lo) {
            out.emplace_back(*lo, i - 1);
            lo.reset();
        }
    }
    return out;
}

// `vA & vB` -> `vA128 & vB128`: the register names of an inline expression, as whole words.
std::string with_128(std::string_view expr) {
    const auto is_word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };
    std::string out;
    for (std::size_t i = 0; i < expr.size(); ++i) {
        out += expr[i];
        if (expr[i] == 'v' && (i == 0 || !is_word(expr[i - 1])) && i + 1 < expr.size() && (expr[i + 1] == 'D' || expr[i + 1] == 'A' || expr[i + 1] == 'B') &&
            (i + 2 == expr.size() || !is_word(expr[i + 2]))) {
            out += expr[i + 1];
            out += "128";
            ++i;
        }
    }
    return out;
}

// What one row's constructor shows and does.
struct Constructor {
    std::string display;                 // the operands as the mnemonic line shows them
    std::vector<std::string> pattern;    // the operand constraints of the `is` part
    std::vector<std::string> pcode;      // the p-code lines
};

Constructor constructor_of(Form form, std::string_view sem) {
    const std::string ea = "\tea:$(REGISTER_SIZE) = x128RA0 + x128RB;";
    const std::vector<std::string> memory = {"vD128", "x128RA0", "x128RB"};
    switch (form) {
        case Form::Load:
            return {"vD128,x128RA0,x128RB", memory,
                    {ea, sem == "=load" ? "\tvD128 = *[ram]:16 (ea & 0xfffffffffffffff0);" : std::format("\tvD128 = {}(ea);", sem)}};
        case Form::Store:
            return {"vD128,x128RA0,x128RB", memory,
                    {ea, sem == "=store" ? "\t*[ram]:16 (ea & 0xfffffffffffffff0) = vD128;" : std::format("\t{}(ea, vD128);", sem)}};
        case Form::Sldoi:
            return {"vD128,vA128,vB128,x128SHB",
                    {"vD128", "vA128", "vB128", "x128SHB"},
                    {"\ttmp:32 = (zext(vA128) << 128) | zext(vB128);", "\ttmp = tmp << (x128SHB * 8);", "\tvD128 = tmp[128,128];"}};
        case Form::Simm:
            return {"vD128,x128SIMM",
                    {"vD128", "x128SIMM"},
                    {"\tw4:4 = sext(x128SIMM:1);", "\tw:16 = zext(w4);", "\tvD128 = w | (w << 32) | (w << 64) | (w << 96);"}};
        case Form::Perm:
            return {"vD128,vA128,vB128,vC128", {"vD128", "vA128", "vB128", "vC128"}, {std::format("\tvD128 = {}(vA128, vB128, vC128);", sem)}};
        case Form::Madd:
            return {"vD128,vA128,vDs128,vB128", {"vD128", "vA128", "vDs128", "vB128"}, {std::format("\tvD128 = {}(vA128, vDs128, vB128);", sem)}};
        case Form::Bimm:
        case Form::Permwi:
        case Form::Rlimi:
        case Form::D3d: {
            const std::string imm = form == Form::Permwi ? "x128PERM" : form == Form::D3d ? "x128D3D" : "x128IMM";
            std::vector<std::string> extra;
            if (form == Form::Rlimi) extra = {"x128Z"};
            if (form == Form::D3d) extra = {"x128PACK", "x128Z"};
            std::vector<std::string> pattern = {"vD128", "vB128", imm};
            pattern.insert(pattern.end(), extra.begin(), extra.end());
            std::string display = "vD128,vB128," + imm;
            for (const std::string& e : extra) display += "," + e;
            std::string args = "vB128";
            for (std::size_t i = 2; i < pattern.size(); ++i) args += ", " + pattern[i] + ":1";
            const std::string source = form == Form::Rlimi ? "vD128, " : "";
            return {display, pattern, {std::format("\tvD128 = {}({}{});", sem, source, args)}};
        }
        case Form::B: return {"vD128,vB128", {"vD128", "vB128"}, {std::format("\tvD128 = {}(vB128);", sem)}};
        case Form::Abc:
        case Form::Cmp: break;
    }
    // three registers
    if (sem.starts_with('=')) return {"vD128,vA128,vB128", {"vD128", "vA128", "vB128"}, {std::format("\tvD128 = {};", with_128(sem.substr(1)))}};
    return {"vD128,vA128,vB128", {"vD128", "vA128", "vB128"}, {std::format("\tvD128 = {}(vA128, vB128);", sem)}};
}

}  // namespace

std::string sleigh(const std::set<std::string>& declared) {
    std::set<BitRange> fixed;
    for (const Row& r : kRows) {
        for (const BitRange& run : runs(r.mask & 0x03FFFFFF)) fixed.insert(run);
    }
    // the token's fields, then listed by (position, name)
    std::map<std::pair<BitRange, std::string>, bool> fields;
    const auto add = [&](const std::string& name, BitRange range) {
        for (auto it = fields.begin(); it != fields.end(); ++it) {
            if (it->first.second == name) {
                fields.erase(it);
                break;
            }
        }
        fields[{range, name}] = true;
    };
    for (const auto& [name, range] : kFields) add(std::string(name), range);
    for (const BitRange& run : fixed) add(std::format("x128_{}_{}", run.first, run.second), run);
    static constexpr std::array<std::string_view, 3> kRegisterNames = {"vD128", "vA128", "vB128"};
    std::vector<std::pair<std::string_view, int>> pieces;
    for (std::string_view reg : kRegisterNames) {
        for (int k = 0; k < 4; ++k) pieces.emplace_back(reg, k);
    }
    for (const auto& [reg, k] : pieces) {   // each bank: a register field and a name field
        const BitRange base = field_range(std::format("{}lo", reg.substr(0, 2)));
        add(std::format("{}r{}", reg, k), base);
        add(std::format("{}n{}", reg, k), base);
    }
    add("vC128r", field_range("vC3"));
    add("vC128n", field_range("vC3"));

    std::vector<std::string> L = {"# VMX128: the Xbox 360 (Xenon) 128-register AltiVec extension. GENERATED by",
                                  "# `python -m codebreaker.decompile.vmx128 --sleigh` from its table -- edit that, not this.",
                                  ""};
    std::string all_registers;
    for (int i = 32; i < 128; ++i) all_registers += std::format("{}v{}", i > 32 ? " " : "", i);
    L.push_back(std::format("define register offset=0x5000 size=16 [ {} ];", all_registers));
    L.push_back("");
    L.push_back("define token vmx128(32)");
    for (const auto& [key, unused] : fields) {
        (void)unused;
        L.push_back(std::format("\t{}=({},{}){}", key.second, key.first.first, key.first.second, key.second == "x128SIMM" ? " signed" : ""));
    }
    L.push_back(";");
    L.push_back("");
    for (const auto& [reg, k] : pieces) {
        L.push_back(std::format("attach variables [ {}r{} ] [ {} ];", reg, k, bank(32 * k)));
        std::string names;
        for (int i = 0; i < 32; ++i) names += std::format("{}v{}", i ? " " : "", 32 * k + i);
        L.push_back(std::format("attach names [ {}n{} ] [ {} ];", reg, k, names));
    }
    std::string gpr, vs8, v8;
    for (int i = 0; i < 32; ++i) gpr += std::format("{}r{}", i ? " " : "", i);
    for (int i = 0; i < 8; ++i) {
        vs8 += std::format("{}vs{}", i ? " " : "", 32 + i);
        v8 += std::format("{}v{}", i ? " " : "", i);
    }
    L.push_back(std::format("attach variables [ x128RA x128RB ] [ {} ];", gpr));
    L.push_back(std::format("attach variables [ vC128r ] [ {} ];", vs8));
    L.push_back(std::format("attach names [ vC128n ] [ {} ];", v8));
    L.push_back("");
    const auto select = [](std::string_view reg, int k) {
        if (reg == "vD128") return std::format("vDhi={}", k);
        if (reg == "vA128") return std::format("vAb6={} & vAb5={}", k >> 1, k & 1);
        return std::format("vBhi={}", k);
    };
    for (const auto& [reg, k] : pieces) {
        L.push_back(std::format("{0}: {0}n{1} is {2} & {0}n{1} & {0}r{1} {{ export {0}r{1}; }}", reg, k, select(reg, k)));
        if (reg == "vD128") {   // (the multiply-adds show vD again, as a source)
            L.push_back(std::format("vDs128: {0}n{1} is {2} & {0}n{1} & {0}r{1} {{ export {0}r{1}; }}", reg, k, select(reg, k)));
        }
    }
    L.push_back("vC128: vC128n is vC128n & vC128r { export vC128r; }");
    L.push_back("x128PERM: val is x128PERMHI & x128IMM [ val = (x128PERMHI << 5) | x128IMM; ] { export *[const]:1 val; }");   // (vpermwi128's 8-bit control, in two pieces)
    L.push_back("x128RA0: x128RA is x128RA { export x128RA; }");   // (rA, or 0 when it names r0)
    L.push_back("x128RA0: 0 is x128RA=0 { export 0:$(REGISTER_SIZE); }");
    L.push_back("");
    std::set<std::string> ops;
    for (const Row& r : kRows) {
        if (!r.semantics.starts_with('=')) ops.insert(std::string(r.semantics));
    }
    for (const X31Row& x : kX31) ops.insert(std::string(x.operation));
    ops.insert("vectorCompareSetCR6");
    for (const std::string& op : ops) {
        if (!declared.contains(op)) L.push_back(std::format("define pcodeop {};", op));
    }
    L.push_back("");
    for (const X31Row& x : kX31) {   // (AltiVec's own fields and register names)
        const std::string_view reg = x.form == Form::Load ? "vrD" : "vrS";
        L.push_back(std::format(":{0} {1},RA_OR_ZERO,B is OP=31 & {1} & RA_OR_ZERO & B & XOP_1_10={2} & Rc=0", x.mnemonic, reg, x.extended_opcode));
        L.push_back("{");
        L.push_back("\tbuild RA_OR_ZERO;");
        L.push_back("\tea:$(REGISTER_SIZE) = RA_OR_ZERO + B;");
        L.push_back(std::format("\t{};", x.form == Form::Load ? std::format("{} = {}(ea)", reg, x.operation) : std::format("{}(ea, {})", x.operation, reg)));
        L.push_back("}");
        L.push_back("");
    }
    for (const Row& r : kRows) {
        std::vector<std::string> cons = {std::format("x128OP={}", r.pattern >> 26)};
        for (const BitRange& run : runs(r.mask & 0x03FFFFFF)) {
            cons.push_back(std::format("x128_{}_{}={}", run.first, run.second, (r.pattern >> run.first) & ((1u << (run.second - run.first + 1)) - 1)));
        }
        const std::array<int, 2> record_bits = {0, 1};
        for (std::size_t i = 0; i < (r.form == Form::Cmp ? 2u : 1u); ++i) {
            std::vector<std::string> c = cons;
            const bool rc = r.form == Form::Cmp && record_bits[i] == 1;
            if (r.form == Form::Cmp) c.push_back(std::format("x128RC={}", record_bits[i]));
            const std::string name = std::string(r.mnemonic) + (rc ? "." : "");
            Constructor k = constructor_of(r.form, r.semantics);
            L.push_back(std::format(":{} {} is {} & {}", name, k.display, text::join(c, " & "), text::join(k.pattern, " & ")));
            if (rc) k.pcode.push_back("\tcr6 = vectorCompareSetCR6(vD128);");
            L.push_back("{");
            for (const std::string& line : k.pcode) L.push_back(line);
            L.push_back("}");
            L.push_back("");
        }
    }
    return text::join(L, "\n");
}

std::set<std::string> declared_operations(std::string_view sinc_text) {
    std::set<std::string> out;
    constexpr std::string_view kPrefix = "define pcodeop ";
    for (std::string_view line : text::lines(sinc_text)) {
        if (!line.starts_with(kPrefix) || !line.ends_with(';')) continue;
        const std::string_view name = line.substr(kPrefix.size(), line.size() - kPrefix.size() - 1);
        if (!name.empty() && std::ranges::all_of(name, [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; })) out.emplace(name);
    }
    return out;
}

std::size_t write_sleigh_file(const std::filesystem::path& out) {
    std::set<std::string> declared;
    const std::filesystem::path folder = std::filesystem::absolute(out).parent_path();
    std::error_code ec;
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder, ec)) {
        if (entry.path().extension() == ".sinc" && entry.path().filename() != "vmx128.sinc") files.push_back(entry.path());
    }
    for (const std::filesystem::path& f : files) {
        const std::set<std::string> ops = declared_operations(fs::read_text(f));
        declared.insert(ops.begin(), ops.end());
    }
    fs::write_text(out, sleigh(declared) + "\n");
    return kRows.size();
}

}  // namespace cb::decompile::vmx128
