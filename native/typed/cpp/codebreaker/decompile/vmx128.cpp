// codebreaker/decompile/vmx128.cpp -- typed C++ of codebreaker.decompile.vmx128

#include "codebreaker/decompile/vmx128.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/formats/xex.hpp"

namespace codebreaker::decompile::vmx128 {

const py::str DOC = R"DOC(VMX128 -- the Xbox 360 (Xenon) 128-register extension of AltiVec.

    python -m codebreaker.decompile.vmx128 <hex-word> ...      decode instruction words
    python -m codebreaker.decompile.vmx128 --coverage <file>   vector decode coverage over .text
    python -m codebreaker.decompile.vmx128 --sleigh <out>      write the SLEIGH (vmx128.sinc)

Stock PowerPC decoders get VMX128 silently wrong: its extra register bits sit where a base
decoder reads opcode-extension bits, so a VMX128 word is usually a legal but different stock
instruction (`lvx128 v1,r3,r4` = 0x102320C3 reads as `vucmprlb 1,3,4`). This table is the one
place VMX128 is described: the native decompiler's SLEIGH (languages/src/ppc/vmx128.sinc, the
PowerPC:BE:64:Xenon language) is generated from it.

Fields (IBM numbering, bit 0 = most significant):
  VD128 = bits 28..29 ++ 6..10   (destination; also a source in the multiply-add forms)
  VA128 = bits 21, 26 ++ 11..15  (its high bits sit inside the extended opcode)
  VB128 = bits 30..31 ++ 16..20
  VC128 = bits 23..25            (3 bits: vperm128's control is v0..v7)

Clean-room from the decode specification (the field layout and the 77-row opcode table),
checked against the Xbox 360 compiler's own /FAcs listings: every row decodes to its mnemonic.)DOC";

int64_t vd128(int64_t w) {
    return (((w >> 2) & 3) << 5) | ((w >> 21) & 31);
}

int64_t va128(int64_t w) {
    return (((w >> 10) & 1) << 6) | (((w >> 5) & 1) << 5) | ((w >> 16) & 31);
}

int64_t vb128(int64_t w) {
    return ((w & 3) << 5) | ((w >> 11) & 31);
}

int64_t vc128(int64_t w) {
    return (w >> 6) & 7;
}

// The 77 rows. `form` is the operand shape; `semantics` is what the SLEIGH does: an expression over
// vD vA vB vC (and EA, IMM) for the operations C can say plainly (starting "="), else the name of the
// user operation the decompiler shows.
const py::list<Row> ROWS = {
    {"lvewx128", 0x10000083, 0xFC0007F3, "load", "loadVectorElementWordIndexed"},
    {"lvlx128", 0x10000403, 0xFC0007F3, "load", "loadVectorLeftIndexed"},
    {"lvlxl128", 0x10000603, 0xFC0007F3, "load", "loadVectorLeftIndexedLRU"},
    {"lvrx128", 0x10000443, 0xFC0007F3, "load", "loadVectorRightIndexed"},
    {"lvrxl128", 0x10000643, 0xFC0007F3, "load", "loadVectorRightIndexedLRU"},
    {"lvsl128", 0x10000003, 0xFC0007F3, "load", "loadVectorForShiftLeft"},
    {"lvsr128", 0x10000043, 0xFC0007F3, "load", "loadVectorForShiftRight"},
    {"lvx128", 0x100000C3, 0xFC0007F3, "load", "=load"},
    {"lvxl128", 0x100002C3, 0xFC0007F3, "load", "=load"},
    {"stvewx128", 0x10000183, 0xFC0007F3, "store", "storeVectorElementWordIndexed"},
    {"stvlx128", 0x10000503, 0xFC0007F3, "store", "storeVectorLeftIndexed"},
    {"stvlxl128", 0x10000703, 0xFC0007F3, "store", "storeVectorLeftIndexedLRU"},
    {"stvrx128", 0x10000543, 0xFC0007F3, "store", "storeVectorRightIndexed"},
    {"stvrxl128", 0x10000743, 0xFC0007F3, "store", "storeVectorRightIndexedLRU"},
    {"stvx128", 0x100001C3, 0xFC0007F3, "store", "=store"},
    {"stvxl128", 0x100003C3, 0xFC0007F3, "store", "=store"},
    {"vsldoi128", 0x10000010, 0xFC000010, "sldoi", "=sldoi"},
    {"vaddfp128", 0x14000010, 0xFC0003D0, "abc", "vectorAddFloatingPoint"},
    {"vand128", 0x14000210, 0xFC0003D0, "abc", "=vA & vB"},
    {"vandc128", 0x14000250, 0xFC0003D0, "abc", "=vA & ~vB"},
    {"vmaddcfp128", 0x14000110, 0xFC0003D0, "madd", "vectorMultiplyAddFloatingPointToD"},
    {"vmaddfp128", 0x140000D0, 0xFC0003D0, "madd", "vectorMultiplyAddFloatingPoint"},
    {"vmsum3fp128", 0x14000190, 0xFC0003D0, "abc", "vectorMultiplySum3FloatingPoint"},
    {"vmsum4fp128", 0x140001D0, 0xFC0003D0, "abc", "vectorMultiplySum4FloatingPoint"},
    {"vmulfp128", 0x14000090, 0xFC0003D0, "abc", "vectorMultiplyFloatingPoint"},
    {"vnmsubfp128", 0x14000150, 0xFC0003D0, "madd", "vectorNegativeMultiplySubtractFloatingPoint"},
    {"vnor128", 0x14000290, 0xFC0003D0, "abc", "=~(vA | vB)"},
    {"vor128", 0x140002D0, 0xFC0003D0, "abc", "=vA | vB"},
    {"vperm128", 0x14000000, 0xFC000210, "perm", "vectorPermute"},
    {"vpkshss128", 0x14000200, 0xFC0003D0, "abc", "vectorPackSignedHalfWordSignedSaturate"},
    {"vpkshus128", 0x14000240, 0xFC0003D0, "abc", "vectorPackSignedHalfWordUnsignedSaturate"},
    {"vpkswss128", 0x14000280, 0xFC0003D0, "abc", "vectorPackSignedWordSignedSaturate"},
    {"vpkswus128", 0x140002C0, 0xFC0003D0, "abc", "vectorPackSignedWordUnsignedSaturate"},
    {"vpkuhum128", 0x14000300, 0xFC0003D0, "abc", "vectorPackUnsignedHalfWordUnsignedModulo"},
    {"vpkuhus128", 0x14000340, 0xFC0003D0, "abc", "vectorPackUnsignedHalfWordUnsignedSaturate"},
    {"vpkuwum128", 0x14000380, 0xFC0003D0, "abc", "vectorPackUnsignedWordUnsignedModulo"},
    {"vpkuwus128", 0x140003C0, 0xFC0003D0, "abc", "vectorPackUnsignedWordUnsignedSaturate"},
    {"vsel128", 0x14000350, 0xFC0003D0, "abc", "=(vA & ~vD) | (vB & vD)"},
    {"vslo128", 0x14000390, 0xFC0003D0, "abc", "vectorShiftLeftByOctet"},
    {"vsro128", 0x140003D0, 0xFC0003D0, "abc", "vectorShiftRightByOctet"},
    {"vsubfp128", 0x14000050, 0xFC0003D0, "abc", "vectorSubtractFloatingPoint"},
    {"vxor128", 0x14000310, 0xFC0003D0, "abc", "=vA ^ vB"},
    {"vcfpsxws128", 0x18000230, 0xFC0007F0, "bimm", "vectorConvertToSignedFixedPointWordSaturate"},
    {"vcfpuxws128", 0x18000270, 0xFC0007F0, "bimm", "vectorConvertToUnsignedFixedPointWordSaturate"},
    {"vcmpbfp128", 0x18000180, 0xFC000390, "cmp", "vectorCompareBoundsFloatingPoint"},
    {"vcmpeqfp128", 0x18000000, 0xFC000390, "cmp", "vectorCompareEqualToFloatingPoint"},
    {"vcmpequw128", 0x18000200, 0xFC000390, "cmp", "vectorCompareEqualToUnsignedWord"},
    {"vcmpgefp128", 0x18000080, 0xFC000390, "cmp", "vectorCompareGreaterThanOrEqualToFloatingPoint"},
    {"vcmpgtfp128", 0x18000100, 0xFC000390, "cmp", "vectorCompareGreaterThanFloatingPoint"},
    {"vcsxwfp128", 0x180002B0, 0xFC0007F0, "bimm", "vectorConvertFromSignedFixedPointWord"},
    {"vcuxwfp128", 0x180002F0, 0xFC0007F0, "bimm", "vectorConvertFromUnsignedFixedPointWord"},
    {"vexptefp128", 0x180006B0, 0xFC1F07F0, "b", "vector2RaisedToTheExponentEstimateFloatingPoint"},
    {"vlogefp128", 0x180006F0, 0xFC1F07F0, "b", "vectorLog2EstimateFloatingPoint"},
    {"vmaxfp128", 0x18000280, 0xFC0003D0, "abc", "vectorMaximumFloatingPoint"},
    {"vminfp128", 0x180002C0, 0xFC0003D0, "abc", "vectorMinimumFloatingPoint"},
    {"vmrghw128", 0x18000300, 0xFC0003D0, "abc", "vectorMergeHighWord"},
    {"vmrglw128", 0x18000340, 0xFC0003D0, "abc", "vectorMergeLowWord"},
    {"vpermwi128", 0x18000210, 0xFC000630, "permwi", "vectorPermuteWordImmediate"},
    {"vpkd3d128", 0x18000610, 0xFC000730, "d3d", "vectorPackD3D"},
    {"vrefp128", 0x18000630, 0xFC1F07F0, "b", "vectorReciprocalEstimateFloatingPoint"},
    {"vrfim128", 0x18000330, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardMinusInfinity"},
    {"vrfin128", 0x18000370, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerNearest"},
    {"vrfip128", 0x180003B0, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardPluInfinity"},
    {"vrfiz128", 0x180003F0, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardZero"},
    {"vrlimi128", 0x18000710, 0xFC000730, "rlimi", "vectorRotateLeftImmediateAndMaskInsert"},
    {"vrlw128", 0x18000050, 0xFC0003D0, "abc", "vectorRotateLeftIntegerWord"},
    {"vrsqrtefp128", 0x18000670, 0xFC1F07F0, "b", "vectorReciprocalSquareRootEstimateFloatingPoint"},
    {"vslw128", 0x180000D0, 0xFC0003D0, "abc", "vectorShiftLeftIntegerWord"},
    {"vspltisw128", 0x18000770, 0xFC0007F0, "simm", "=splat"},
    {"vspltw128", 0x18000730, 0xFC0007F0, "bimm", "vectorSplatWord"},
    {"vsraw128", 0x18000150, 0xFC0003D0, "abc", "vectorShiftRightAlgebraicWord"},
    {"vsrw128", 0x180001D0, 0xFC0003D0, "abc", "vectorShiftRightWord"},
    {"vupkd3d128", 0x180007F0, 0xFC0007F0, "bimm", "vectorUnpackD3D"},
    {"vupkhsb128", 0x18000380, 0xFC1F07F0, "b", "vectorUnpackHighSignedByte"},
    {"vupkhsh128", 0x180007A0, 0xFC1F07F0, "b", "vectorUnpackHighSignedHalfWord"},
    {"vupklsb128", 0x180003C0, 0xFC1F07F0, "b", "vectorUnpackLowSignedByte"},
    {"vupklsh128", 0x180007E0, 0xFC1F07F0, "b", "vectorUnpackLowSignedHalfWord"},
};

// The Xenon's (and Cell's) unaligned vector loads and stores on the 32 AltiVec registers, in
// primary opcode 31 -- not VMX128, but missing from stock decoders alike.
const py::list<X31Row> X31 = {
    {"lvlx", 519, "load", "loadVectorLeftIndexed"},    {"lvlxl", 775, "load", "loadVectorLeftIndexedLRU"},
    {"lvrx", 551, "load", "loadVectorRightIndexed"},   {"lvrxl", 807, "load", "loadVectorRightIndexedLRU"},
    {"stvlx", 647, "store", "storeVectorLeftIndexed"}, {"stvlxl", 903, "store", "storeVectorLeftIndexedLRU"},
    {"stvrx", 679, "store", "storeVectorRightIndexed"}, {"stvrxl", 935, "store", "storeVectorRightIndexedLRU"}};

namespace {

// {extended opcode: (mnemonic, form)}
const py::dict<int64_t, std::tuple<py::str, py::str>>& x31_by_xo() {
    static const py::dict<int64_t, std::tuple<py::str, py::str>> m = [] {
        py::dict<int64_t, std::tuple<py::str, py::str>> d;
        for (const X31Row& r : X31) {
            d[std::get<1>(r)] = std::make_tuple(std::get<0>(r), std::get<2>(r));
        }
        return d;
    }();
    return m;
}

int64_t popcount(int64_t v) {
    int64_t n = 0;
    for (uint64_t u = static_cast<uint64_t>(v); u != 0; u >>= 1) {
        n += static_cast<int64_t>(u & 1);
    }
    return n;
}

// most specific (widest mask) first, so a broad row never shadows a narrow one
const std::vector<Row>& by_specificity() {
    static const std::vector<Row> rows = [] {
        std::vector<Row> v;
        for (const Row& r : ROWS) {
            v.push_back(r);
        }
        std::stable_sort(v.begin(), v.end(),
                         [](const Row& a, const Row& b) { return popcount(std::get<2>(a)) > popcount(std::get<2>(b)); });
        return v;
    }();
    return rows;
}

}  // namespace

std::optional<std::tuple<py::str, py::str>> decode(int64_t word) {
    int64_t w = word & 0xFFFFFFFF;
    if ((w >> 26) == 31 && (w & 1) == 0 && x31_by_xo().contains((w >> 1) & 0x3FF)) {
        int64_t ra = (w >> 16) & 31, rb = (w >> 11) & 31;
        const auto& hit = x31_by_xo()[(w >> 1) & 0x3FF];
        return std::make_tuple(std::get<0>(hit), py::percent("v%d,%s,r%d", (w >> 21) & 31,
                                                              ra != 0 ? py::percent("r%d", ra) : py::str("0"), rb));
    }
    int64_t op = w >> 26;
    if (op != 4 && op != 5 && op != 6) {
        return std::nullopt;
    }
    for (const Row& row : by_specificity()) {
        const py::str& mnem = std::get<0>(row);
        int64_t pat = std::get<1>(row), mask = std::get<2>(row);
        const py::str& form = std::get<3>(row);
        if ((w & mask) == pat) {
            py::str m = mnem;
            if (form == "cmp" && (w & 0x40) != 0) {
                m += ".";
            }
            return std::make_tuple(m, _operands(form, w));
        }
    }
    return std::nullopt;
}

py::str _operands(const py::str& form, int64_t w) {
    int64_t d = vd128(w), a = va128(w), b = vb128(w), c = vc128(w);
    int64_t ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    if (form == "load" || form == "store") {   // (rA 0 is the literal zero, written 0)
        return py::percent("v%d,%s,r%d", d, ra != 0 ? py::percent("r%d", ra) : py::str("0"), rb);
    }
    if (form == "sldoi") {
        return py::percent("v%d,v%d,v%d,%d", d, a, b, (w >> 6) & 0xF);
    }
    if (form == "perm") {
        return py::percent("v%d,v%d,v%d,v%d", d, a, b, c);
    }
    if (form == "madd") {
        return py::percent("v%d,v%d,v%d,v%d", d, a, d, b);
    }
    if (form == "simm") {
        int64_t s = (w >> 16) & 31;
        return py::percent("v%d,%d", d, s >= 16 ? s - 32 : s);
    }
    if (form == "bimm") {
        return py::percent("v%d,v%d,%d", d, b, (w >> 16) & 31);
    }
    if (form == "permwi") {
        return py::percent("v%d,v%d,%d", d, b, (((w >> 6) & 7) << 5) | ((w >> 16) & 31));
    }
    if (form == "d3d") {
        return py::percent("v%d,v%d,%d,%d,%d", d, b, (w >> 18) & 7, (w >> 16) & 3, (w >> 6) & 3);
    }
    if (form == "rlimi") {
        return py::percent("v%d,v%d,%d,%d", d, b, (w >> 16) & 31, (w >> 6) & 3);
    }
    if (form == "b") {
        return py::percent("v%d,v%d", d, b);
    }
    return py::percent("v%d,v%d,v%d", d, a, b);
}

std::optional<py::str> text(int64_t word) {
    auto r = decode(word);
    if (!r) {
        return std::nullopt;
    }
    return py::percent("%s %s", std::get<0>(*r), std::get<1>(*r));
}

py::dict<py::str, int64_t> coverage(const formats::powerpc_image::Image& img) {
    int64_t total = 0, vmx = 0;
    for (int64_t va = std::get<0>(img.code); va < std::get<1>(img.code); va += 4) {
        std::optional<int64_t> w = img.word(va);
        if (w.has_value() && ((*w >> 26) == 4 || (*w >> 26) == 5 || (*w >> 26) == 6)) {
            total += 1;
            vmx += decode(*w).has_value() ? 1 : 0;
        }
    }
    py::dict<py::str, int64_t> out;
    out[py::str("vector words")] = total;
    out[py::str("vmx128")] = vmx;
    out[py::str("other")] = total - vmx;
    return out;
}

// ---- the SLEIGH -----------------------------------------------------------------------------------
// Registers: v0..v31 are AltiVec's (the top 32 VSX registers, vs32..vs63); v32..v127 are new, at
// offset 0x5000 (free in the PowerPC register space: VSX ends at 0x4400, the context at 0x6000).
// A register field's high bits pick one of four banks of 32; each bank is a subtable constructor.
namespace {

using Field = std::tuple<int64_t, int64_t>;   // (lo, hi): a bit range

const std::map<std::string, Field>& base_fields() {
    static const std::map<std::string, Field> f = {
        {"vDlo", {21, 25}},     {"vDhi", {2, 3}},       {"vAlo", {16, 20}},     {"vAb5", {5, 5}},
        {"vAb6", {10, 10}},     {"vBlo", {11, 15}},     {"vBhi", {0, 1}},       {"vC3", {6, 8}},
        {"x128OP", {26, 31}},   {"x128SHB", {6, 9}},    {"x128IMM", {16, 20}},  {"x128SIMM", {16, 20}},
        {"x128PERMHI", {6, 8}}, {"x128D3D", {18, 20}},  {"x128PACK", {16, 17}}, {"x128Z", {6, 7}},
        {"x128RC", {6, 6}},     {"x128RA", {16, 20}},   {"x128RB", {11, 15}}};
    return f;
}

py::str bank(int64_t base) {
    py::list<py::str> names;
    for (int64_t i = 0; i < 32; ++i) {
        names.append(base == 0 ? py::percent("vs%d", 32 + i) : py::percent("v%d", base + i));
    }
    return py::join(py::str(" "), names);
}

// the names "<prefix>first .. <prefix>first+n-1", space separated
py::str numbered(const char* prefix, int64_t first, int64_t n) {
    py::list<py::str> names;
    for (int64_t i = 0; i < n; ++i) {
        names.append(py::percent("%s%d", py::str(prefix), first + i));
    }
    return py::join(py::str(" "), names);
}

}  // namespace

py::list<std::tuple<int64_t, int64_t>> _runs(int64_t bits) {
    py::list<std::tuple<int64_t, int64_t>> out;
    std::optional<int64_t> lo;
    for (int64_t i = 0; i < 33; ++i) {
        bool on = i < 32 && ((bits >> i) & 1) != 0;
        if (on && !lo.has_value()) {
            lo = i;
        } else if (!on && lo.has_value()) {
            out.append(std::make_tuple(*lo, i - 1));
            lo.reset();
        }
    }
    return out;
}

py::str sleigh(const py::set<py::str>& declared) {
    // the fixed bit runs of the masks
    std::set<Field> fixed;
    for (const Row& r : ROWS) {
        for (const Field& f : _runs(std::get<2>(r) & 0x03FFFFFF)) {
            fixed.insert(f);
        }
    }
    std::map<std::string, Field> fields = base_fields();
    for (const Field& f : fixed) {
        fields[py::percent("x128_%d_%d", std::get<0>(f), std::get<1>(f)).utf8()] = f;
    }
    const char* regs[3] = {"vD128", "vA128", "vB128"};
    std::vector<std::pair<std::string, int64_t>> pieces;
    for (const char* reg : regs) {
        for (int64_t k = 0; k < 4; ++k) {
            pieces.push_back({reg, k});
        }
    }
    for (const auto& pc : pieces) {   // each bank: a register field and a name field
        const std::string& reg = pc.first;
        int64_t k = pc.second;
        Field lo = fields[reg.substr(0, 2) + "lo"];
        fields[py::percent("%sr%d", py::str(reg), k).utf8()] = lo;
        fields[py::percent("%sn%d", py::str(reg), k).utf8()] = lo;
    }
    fields["vC128r"] = fields["vC3"];
    fields["vC128n"] = fields["vC3"];

    py::list<py::str> L;
    L.append(py::str("# VMX128: the Xbox 360 (Xenon) 128-register AltiVec extension. GENERATED by"));
    L.append(py::str("# `python -m codebreaker.decompile.vmx128 --sleigh` from its table -- edit that, not this."));
    L.append(py::str(""));
    L.append(py::percent("define register offset=0x5000 size=16 [ %s ];", numbered("v", 32, 96)));
    L.append(py::str(""));
    L.append(py::str("define token vmx128(32)"));
    std::vector<std::pair<std::string, Field>> sorted_fields(fields.begin(), fields.end());
    std::sort(sorted_fields.begin(), sorted_fields.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first < b.first;
    });
    for (const auto& nf : sorted_fields) {
        const char* sign = nf.first == "x128SIMM" ? " signed" : "";
        L.append(py::percent("\t%s=(%d,%d)%s", py::str(nf.first), std::get<0>(nf.second), std::get<1>(nf.second),
                             py::str(sign)));
    }
    L.append(py::str(";"));
    L.append(py::str(""));
    for (const auto& pc : pieces) {
        py::str reg(pc.first);
        int64_t k = pc.second;
        L.append(py::percent("attach variables [ %sr%d ] [ %s ];", reg, k, bank(32 * k)));
        L.append(py::percent("attach names [ %sn%d ] [ %s ];", reg, k, numbered("v", 32 * k, 32)));
    }
    L.append(py::percent("attach variables [ x128RA x128RB ] [ %s ];", numbered("r", 0, 32)));
    L.append(py::percent("attach variables [ vC128r ] [ %s ];", numbered("vs", 32, 8)));
    L.append(py::percent("attach names [ vC128n ] [ %s ];", numbered("v", 0, 8)));
    L.append(py::str(""));
    auto sel = [](const std::string& reg, int64_t k) -> py::str {
        if (reg == "vD128") {
            return py::percent("vDhi=%d", k);
        }
        if (reg == "vA128") {
            return py::percent("vAb6=%d & vAb5=%d", k >> 1, k & 1);
        }
        return py::percent("vBhi=%d", k);
    };
    for (const auto& pc : pieces) {
        py::str r(pc.first);
        int64_t k = pc.second;
        L.append(py::percent("%s: %sn%d is %s & %sn%d & %sr%d { export %sr%d; }", r, r, k, sel(pc.first, k), r, k, r, k, r, k));
        if (pc.first == "vD128") {   // (the multiply-adds show vD again, as a source)
            L.append(py::percent("vDs128: %sn%d is %s & %sn%d & %sr%d { export %sr%d; }", r, k, sel(pc.first, k), r, k, r, k, r, k));
        }
    }
    L.append(py::str("vC128: vC128n is vC128n & vC128r { export vC128r; }"));
    L.append(py::str("x128PERM: val is x128PERMHI & x128IMM [ val = (x128PERMHI << 5) | x128IMM; ] "
                     "{ export *[const]:1 val; }"));   // (vpermwi128's 8-bit control, in two pieces)
    L.append(py::str("x128RA0: x128RA is x128RA { export x128RA; }"));   // (rA, or 0 when it names r0)
    L.append(py::str("x128RA0: 0 is x128RA=0 { export 0:$(REGISTER_SIZE); }"));
    L.append(py::str(""));
    std::set<std::string> ops;
    for (const Row& r : ROWS) {
        if (!std::get<4>(r).startswith(py::str("="))) {
            ops.insert(std::get<4>(r).utf8());
        }
    }
    for (const X31Row& r : X31) {
        ops.insert(std::get<3>(r).utf8());
    }
    ops.insert("vectorCompareSetCR6");
    for (const std::string& op : ops) {
        if (!declared.contains(py::str(op))) {
            L.append(py::percent("define pcodeop %s;", py::str(op)));
        }
    }
    L.append(py::str(""));
    for (const X31Row& r : X31) {   // (AltiVec's own fields and register names)
        const py::str& mnem = std::get<0>(r);
        int64_t xo = std::get<1>(r);
        const py::str& form = std::get<2>(r);
        const py::str& sem = std::get<3>(r);
        py::str reg = form == "load" ? py::str("vrD") : py::str("vrS");
        L.append(py::percent(":%s %s,RA_OR_ZERO,B is OP=31 & %s & RA_OR_ZERO & B & XOP_1_10=%d & Rc=0", mnem, reg, reg, xo));
        L.append(py::str("{"));
        L.append(py::str("\tbuild RA_OR_ZERO;"));
        L.append(py::str("\tea:$(REGISTER_SIZE) = RA_OR_ZERO + B;"));
        L.append(py::str("\t") +
                 (form == "load" ? py::percent("%s = %s(ea)", reg, sem) : py::percent("%s(ea, %s)", sem, reg)) + ";");
        L.append(py::str("}"));
        L.append(py::str(""));
    }
    for (const Row& row : ROWS) {
        const py::str& mnem = std::get<0>(row);
        int64_t pat = std::get<1>(row), mask = std::get<2>(row);
        const py::str& form = std::get<3>(row);
        const py::str& sem = std::get<4>(row);
        py::list<py::str> cons;
        cons.append(py::percent("x128OP=%d", pat >> 26));
        for (const Field& f : _runs(mask & 0x03FFFFFF)) {
            int64_t lo = std::get<0>(f), hi = std::get<1>(f);
            cons.append(py::percent("x128_%d_%d=%d", lo, hi, (pat >> lo) & ((int64_t(1) << (hi - lo + 1)) - 1)));
        }
        std::vector<int64_t> rcs;
        if (form == "cmp") {
            rcs = {0, 1};
        } else {
            rcs = {-1};   // (None)
        }
        for (int64_t rc : rcs) {
            py::list<py::str> c = py::list<py::str>::from(cons);
            if (rc >= 0) {
                c.append(py::percent("x128RC=%d", rc));
            }
            py::str name = mnem + (rc == 1 ? "." : "");
            auto con = _constructor(form, sem);
            const py::str& ops_text = std::get<0>(con);
            const py::list<py::str>& operands = std::get<1>(con);
            py::list<py::str> body = py::list<py::str>::from(std::get<2>(con));
            L.append(py::percent(":%s %s is %s & %s", name, ops_text, py::join(py::str(" & "), c),
                                 py::join(py::str(" & "), operands)));
            if (rc == 1) {
                body.append(py::str("\tcr6 = vectorCompareSetCR6(vD128);"));
            }
            L.append(py::str("{"));
            for (const py::str& b : body) {
                L.append(b);
            }
            L.append(py::str("}"));
            L.append(py::str(""));
        }
    }
    return py::join(py::str("\n"), L);
}

namespace {

// re.sub(r"\bv([DAB])\b", r"v\g<1>128", s)
py::str widen_registers(const py::str& s) {
    return py::re::sub(py::str(R"(\bv([DAB])\b)"), py::str(R"(v\g<1>128)"), s);
}

py::list<py::str> strs(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) {
        out.append(py::str(x));
    }
    return out;
}

py::list<py::str> one(const py::str& x) {
    py::list<py::str> out;
    out.append(x);
    return out;
}

py::list<py::str> plus(const py::list<py::str>& a, const py::list<py::str>& b) {
    py::list<py::str> out = py::list<py::str>::from(a);
    for (const py::str& x : b) {
        out.append(x);
    }
    return out;
}

}  // namespace

std::tuple<py::str, py::list<py::str>, py::list<py::str>> _constructor(const py::str& form, const py::str& sem) {
    using R = std::tuple<py::str, py::list<py::str>, py::list<py::str>>;
    py::list<py::str> ea = strs({"\tea:$(REGISTER_SIZE) = x128RA0 + x128RB;"});
    py::str lsx("vD128,x128RA0,x128RB");
    py::list<py::str> lsops = strs({"vD128", "x128RA0", "x128RB"});
    if (form == "load") {
        if (sem == "=load") {
            return R(lsx, lsops, plus(ea, strs({"\tvD128 = *[ram]:16 (ea & 0xfffffffffffffff0);"})));
        }
        return R(lsx, lsops, plus(ea, one(py::percent("\tvD128 = %s(ea);", sem))));
    }
    if (form == "store") {
        if (sem == "=store") {
            return R(lsx, lsops, plus(ea, strs({"\t*[ram]:16 (ea & 0xfffffffffffffff0) = vD128;"})));
        }
        return R(lsx, lsops, plus(ea, one(py::percent("\t%s(ea, vD128);", sem))));
    }
    if (form == "sldoi") {
        return R(py::str("vD128,vA128,vB128,x128SHB"), strs({"vD128", "vA128", "vB128", "x128SHB"}),
                 strs({"\ttmp:32 = (zext(vA128) << 128) | zext(vB128);", "\ttmp = tmp << (x128SHB * 8);",
                       "\tvD128 = tmp[128,128];"}));
    }
    if (form == "simm") {
        return R(py::str("vD128,x128SIMM"), strs({"vD128", "x128SIMM"}),
                 strs({"\tw4:4 = sext(x128SIMM:1);", "\tw:16 = zext(w4);",
                       "\tvD128 = w | (w << 32) | (w << 64) | (w << 96);"}));
    }
    if (form == "perm") {
        return R(py::str("vD128,vA128,vB128,vC128"), strs({"vD128", "vA128", "vB128", "vC128"}),
                 one(py::percent("\tvD128 = %s(vA128, vB128, vC128);", sem)));
    }
    if (form == "madd") {
        return R(py::str("vD128,vA128,vDs128,vB128"), strs({"vD128", "vA128", "vDs128", "vB128"}),
                 one(py::percent("\tvD128 = %s(vA128, vDs128, vB128);", sem)));
    }
    if (form == "bimm" || form == "permwi" || form == "rlimi" || form == "d3d") {
        py::str imm = form == "permwi" ? py::str("x128PERM") : form == "d3d" ? py::str("x128D3D") : py::str("x128IMM");
        py::str extra = form == "rlimi" ? py::str(",x128Z") : form == "d3d" ? py::str(",x128PACK,x128Z") : py::str("");
        py::list<py::str> operands = strs({"vD128", "vB128"});
        operands.append(imm);
        for (const py::str& e : py::split(extra, py::str(","))) {
            if (e != "") {
                operands.append(e);
            }
        }
        py::list<py::str> args;
        args.append(py::str("vB128"));
        for (int64_t i = 2; i < py::len(operands); ++i) {
            args.append(operands[i] + ":1");
        }
        py::str src = form == "rlimi" ? py::str("vD128, ") : py::str("");
        return R(py::percent("vD128,vB128,%s%s", imm, extra), operands,
                 one(py::percent("\tvD128 = %s(%s%s);", sem, src, py::join(py::str(", "), args))));
    }
    if (form == "b") {
        return R(py::str("vD128,vB128"), strs({"vD128", "vB128"}), one(py::percent("\tvD128 = %s(vB128);", sem)));
    }
    // three registers ("abc", "cmp")
    if (sem.startswith(py::str("="))) {
        return R(py::str("vD128,vA128,vB128"), strs({"vD128", "vA128", "vB128"}),
                 one(py::percent("\tvD128 = %s;", widen_registers(sem.slice_from(1)))));
    }
    return R(py::str("vD128,vA128,vB128"), strs({"vD128", "vA128", "vB128"}),
             one(py::percent("\tvD128 = %s(vA128, vB128);", sem)));
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
        py::print(py::split(DOC, py::str("\n\n"))[0]);
        return 0;
    }
    if (argv[0] == "--sleigh") {
        if (py::len(argv) < 2) {
            py::print(py::str("usage: --sleigh <out.sinc>"));
            return 2;
        }
        py::set<py::str> declared;
        py::str dir = py::os::path::dirname(py::os::path::abspath(argv[1]));
        py::list<py::str> names = py::os::listdir(dir);
        std::vector<py::str> sorted_names;
        for (const py::str& n : names) {
            sorted_names.push_back(n);
        }
        std::sort(sorted_names.begin(), sorted_names.end());
        for (const py::str& n : sorted_names) {
            if (!n.lower().endswith(py::str(".sinc"))) {
                continue;
            }
            if (n != "vmx128.sinc") {
                py::str body = py::open(py::os::path::join(dir, n)).read_text();
                declared.update(py::re::Pattern(py::str("^define pcodeop (\\w+);"), py::re::MULTILINE).findall(body));
            }
        }
        {
            py::file f = py::open(argv[1], py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
            f.write(sleigh(declared) + "\n");
            f.close();
        }
        py::print(py::percent("wrote %s (%d rows)", argv[1], py::len(ROWS)));
        return 0;
    }
    if (argv[0] == "--coverage") {
        py::str path = argv[1].lower().endswith(py::str(".xex")) ? formats::xex::basefile(argv[1]) : argv[1];
        py::ref<formats::powerpc_image::Image> img = formats::powerpc_image::read(path);
        for (const char* k : {"vector words", "vmx128", "other"}) {
            py::print(py::percent("  %-13s %d", py::str(k), coverage(*img)[py::str(k)]));
        }
        return 0;
    }
    for (const py::str& tok : argv) {
        int64_t w = py::int_parse(tok, 16);
        auto t = text(w);
        py::print(py::percent("0x%08X  %s", w, t ? *t : py::str("(not VMX128)")));
    }
    return 0;
}

}  // namespace codebreaker::decompile::vmx128
