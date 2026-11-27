// decompile/mips_rsp.cpp -- the RSP's vector unit and its vector loads and stores (mips_rsp.hpp).

#include "decompile/mips_rsp.hpp"

#include <array>
#include <format>
#include <string>

#include "core/text.hpp"

namespace cb::decompile::mips::rsp {

namespace {

[[noreturn]] void refuse(std::string_view name, const std::string& why) { throw ParseError("", 0, 0, std::string(name) + " (" + why + ")"); }

constexpr std::uint32_t kCop2 = 0x12u << 26;
constexpr std::uint32_t kLwc2 = 0x32u << 26;
constexpr std::uint32_t kSwc2 = 0x3Au << 26;

// The computational instructions, by function (bits 5-0). An empty name: no instruction.
constexpr auto kVector = std::to_array<std::string_view>({
    "vmulf", "vmulu", "vrndp", "vmulq", "vmudl", "vmudm", "vmudn", "vmudh",   // 0x00
    "vmacf", "vmacu", "vrndn", "vmacq", "vmadl", "vmadm", "vmadn", "vmadh",   // 0x08
    "vadd", "vsub", "", "vabs", "vaddc", "vsubc", "", "",                     // 0x10
    "", "", "", "", "", "vsar", "", "",                                       // 0x18
    "vlt", "veq", "vne", "vge", "vcl", "vch", "vcr", "vmrg",                  // 0x20
    "vand", "vnand", "vor", "vnor", "vxor", "vnxor", "", "",                  // 0x28
    "vrcp", "vrcpl", "vrcph", "vmov", "vrsq", "vrsql", "vrsqh", "vnop",       // 0x30
    "", "", "", "", "", "", "", "",                                           // 0x38
});

// The one-lane instructions: vd's lane is in the vs field.
bool one_lane(unsigned function) { return function >= 0x30 && function <= 0x36; }

constexpr auto kLoads = std::to_array<std::string_view>({"lbv", "lsv", "llv", "ldv", "lqv", "lrv", "lpv", "luv", "lhv", "lfv", "lwv", "ltv"});
constexpr auto kStores = std::to_array<std::string_view>({"sbv", "ssv", "slv", "sdv", "sqv", "srv", "spv", "suv", "shv", "sfv", "swv", "stv"});
constexpr auto kControl = std::to_array<std::string_view>({"$vco", "$vcc", "$vce"});

std::string vreg(unsigned n) { return std::format("$v{}", n); }

// vt's element as written after the register: "" for the whole vector, [0q] ... [7].
std::string element_text(unsigned e) {
    static constexpr std::array<std::string_view, 16> kNames = {"", "[e1]", "[0q]", "[1q]", "[0h]", "[1h]", "[2h]", "[3h]",
                                                                "[0]", "[1]", "[2]", "[3]", "[4]", "[5]", "[6]", "[7]"};
    return std::string(kNames[e & 15]);
}

std::optional<unsigned> element_of(std::string_view t) {
    static constexpr std::array<std::string_view, 16> kNames = {"", "e1", "0q", "1q", "0h", "1h", "2h", "3h", "0", "1", "2", "3", "4", "5", "6", "7"};
    for (unsigned i = 0; i < kNames.size(); ++i)
        if (t == kNames[i]) return i;
    return std::nullopt;
}

// "$v12" -> 12
std::optional<unsigned> vreg_of(std::string_view t) {
    t = text::trim(t);
    if (t.size() < 3 || t[0] != '$' || (t[1] != 'v' && t[1] != 'V')) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(2));
    if (!n || *n > 31) return std::nullopt;
    return *n;
}

// "$v3[0q]" -> (3, "0q"); "$v3" -> (3, "")
struct WithElement {
    unsigned reg = 0;
    std::string element;
};
std::optional<WithElement> split_element(std::string_view t) {
    t = text::trim(t);
    std::string_view inside;
    if (const std::size_t open = t.find('['); open != std::string_view::npos) {
        if (t.back() != ']') return std::nullopt;
        inside = text::trim(t.substr(open + 1, t.size() - open - 2));
        t = text::trim(t.substr(0, open));
    }
    const std::optional<unsigned> r = vreg_of(t);
    if (!r) return std::nullopt;
    return WithElement{*r, std::string(inside)};
}

unsigned number_0_15(std::string_view name, const std::string& t) {
    const auto n = text::parse_int<unsigned>(text::trim(t));
    if (!n || *n > 15) refuse(name, "an element is 0 to 15");
    return *n;
}

}  // namespace

unsigned access_size(unsigned opcode) {
    static constexpr std::array<unsigned, 12> kSizes = {1, 2, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16};
    return opcode < kSizes.size() ? kSizes[opcode] : 0;
}

bool decode(std::uint32_t word, Insn& insn) {
    const std::uint32_t opcode = word & 0xFC000000u;
    std::string name;
    if (opcode == kCop2) {
        const unsigned fmt = (word >> 21) & 0x1F;
        if (fmt & 0x10) {   // a computational instruction
            const unsigned function = word & 0x3F;
            if (function >= kVector.size() || kVector[function].empty()) return false;
            name = kVector[function];
        } else {
            if ((word & 0x7F) != 0) return false;
            if (fmt == 0) name = "mfc2";
            else if (fmt == 4) name = "mtc2";
            else if (fmt == 2 || fmt == 6) {
                if (((word >> 7) & 0xF) != 0 || ((word >> 11) & 0x1F) > 2) return false;
                name = fmt == 2 ? "cfc2" : "ctc2";
            } else {
                return false;
            }
        }
    } else if (opcode == kLwc2 || opcode == kSwc2) {
        const unsigned op = (word >> 11) & 0x1F;
        if (op >= kLoads.size()) return false;
        // (the transposed and the packed ones take only even or whole elements; the hardware ignores nothing here, so every element is read back)
        name = opcode == kLwc2 ? kLoads[op] : kStores[op];
    } else {
        return false;
    }
    insn.mnemonic = std::move(name);
    insn.form = Form::Special;
    insn.rsp = true;
    insn.flow = Flow::None;
    insn.delay_slot = false;
    insn.rs = static_cast<std::uint8_t>((word >> 21) & 0x1F);
    insn.rt = static_cast<std::uint8_t>((word >> 16) & 0x1F);
    insn.rd = static_cast<std::uint8_t>((word >> 11) & 0x1F);
    return true;
}

std::string text(const Insn& insn) {
    const std::uint32_t w = insn.word;
    const std::string& name = insn.mnemonic;
    const unsigned vt = (w >> 16) & 0x1F, vs = (w >> 11) & 0x1F, vd = (w >> 6) & 0x1F, e = (w >> 21) & 0xF;
    const std::uint32_t opcode = w & 0xFC000000u;
    if (opcode == kCop2 && ((w >> 25) & 1)) {
        const unsigned function = w & 0x3F;
        if (function == 0x37) {   // vnop
            if ((w & 0x01FFFFC0) == 0) return "vnop";
            return std::format("vnop {}, {}, {}{}", vreg(vd), vreg(vs), vreg(vt), element_text(e));
        }
        if (one_lane(function)) return std::format("{} {}[{}], {}{}", name, vreg(vd), vs, vreg(vt), element_text(e));
        return std::format("{} {}, {}, {}{}", name, vreg(vd), vreg(vs), vreg(vt), element_text(e));
    }
    if (opcode == kCop2) {
        const std::string rt(register_name(vt));
        if (name == "cfc2" || name == "ctc2") return std::format("{} {}, {}", name, rt, kControl[vs]);
        return std::format("{} {}, {}[{}]", name, rt, vreg(vs), (w >> 7) & 0xF);
    }
    // a vector load or store
    const unsigned op = (w >> 11) & 0x1F;
    const int offset = static_cast<int>(static_cast<std::int32_t>(w << 25) >> 25) * static_cast<int>(access_size(op));
    return std::format("{} {}[{}], {}({})", name, vreg(vt), (w >> 7) & 0xF, offset, register_name((w >> 21) & 0x1F));
}

std::optional<std::uint32_t> assemble(std::string_view name, const std::vector<std::string>& ops, std::uint32_t pc, const NameLabels& labels) {
    // a computational instruction
    for (unsigned function = 0; function < kVector.size(); ++function) {
        if (kVector[function].empty() || kVector[function] != name) continue;
        std::uint32_t word = kCop2 | (1u << 25) | function;
        if (function == 0x37 && ops.empty()) return word;   // vnop
        if (one_lane(function)) {
            if (ops.size() != 2) refuse(name, "a lane of vd and vt[e]");
            const std::optional<WithElement> d = split_element(ops[0]), t = split_element(ops[1]);
            if (!d || !t || d->element.empty()) refuse(name, "a lane of vd and vt[e]");
            const auto lane_value = text::parse_int<unsigned>(text::trim(d->element));
            if (!lane_value || *lane_value > 31) refuse(name, "a lane is 0 to 31");
            const unsigned lane = *lane_value;
            const std::optional<unsigned> e = element_of(t->element);
            if (!e) refuse(name, "an element: [0q] [1q] [0h]-[3h] [0]-[7]");
            return word | (*e << 21) | (t->reg << 16) | (lane << 11) | (d->reg << 6);
        }
        if (ops.size() != 3) refuse(name, "vd, vs, vt[e]");
        const std::optional<unsigned> d = vreg_of(ops[0]), s = vreg_of(ops[1]);
        const std::optional<WithElement> t = split_element(ops[2]);
        if (!d || !s || !t) refuse(name, "vd, vs, vt[e]");
        const std::optional<unsigned> e = element_of(t->element);
        if (!e) refuse(name, "an element: [0q] [1q] [0h]-[3h] [0]-[7]");
        return word | (*e << 21) | (t->reg << 16) | (*s << 11) | (*d << 6);
    }
    // the moves
    if (name == "mfc2" || name == "mtc2" || name == "cfc2" || name == "ctc2") {
        if (ops.size() != 2) refuse(name, "rt, and a vector register's element or a control register");
        const std::optional<unsigned> rt = register_number(text::trim(ops[0]));
        if (!rt) refuse(name, "rt is a general register");
        const unsigned fmt = name == "mfc2" ? 0u : name == "mtc2" ? 4u : name == "cfc2" ? 2u : 6u;
        std::uint32_t word = kCop2 | (fmt << 21) | (*rt << 16);
        if (fmt == 2 || fmt == 6) {
            const std::string_view c = text::trim(ops[1]);
            for (unsigned i = 0; i < kControl.size(); ++i)
                if (c == kControl[i]) return word | (i << 11);
            refuse(name, "a control register: $vco $vcc $vce");
        }
        const std::optional<WithElement> v = split_element(ops[1]);
        if (!v) refuse(name, "a vector register and its element");
        return word | (v->reg << 11) | ((v->element.empty() ? 0u : number_0_15(name, v->element)) << 7);
    }
    // the loads and stores
    for (unsigned op = 0; op < kLoads.size(); ++op) {
        if (name != kLoads[op] && name != kStores[op]) continue;
        if (ops.size() != 2) refuse(name, "vt[e], offset(base)");
        const std::optional<WithElement> v = split_element(ops[0]);
        if (!v) refuse(name, "vt[e], offset(base)");
        const unsigned element = v->element.empty() ? 0u : number_0_15(name, v->element);
        const std::string_view m = text::trim(ops[1]);
        const std::size_t open = m.rfind('(');
        if (open == std::string_view::npos || m.back() != ')') refuse(name, "offset(base)");
        const std::optional<unsigned> base = register_number(text::trim(m.substr(open + 1, m.size() - open - 2)));
        if (!base) refuse(name, "the base is a general register");
        const std::string_view off_text = text::trim(m.substr(0, open));
        const std::int64_t offset = off_text.empty() ? 0 : value(off_text, labels, pc);
        const std::int64_t size = access_size(op);
        if (offset % size != 0) refuse(name, std::format("the offset is a multiple of {}", size));
        const std::int64_t scaled = offset / size;
        if (scaled < -64 || scaled > 63) refuse(name, "the offset is out of reach");
        const std::uint32_t opcode = name == kLoads[op] ? kLwc2 : kSwc2;
        return opcode | (*base << 21) | (v->reg << 16) | (op << 11) | (element << 7) | (static_cast<std::uint32_t>(scaled) & 0x7F);
    }
    return std::nullopt;
}

}  // namespace cb::decompile::mips::rsp
