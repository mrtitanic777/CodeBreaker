// cb-unity: alone
// cb/decompile/arm_thumb2.cpp -- ARMv7-A Thumb: the 32-bit instructions' lengths, branches and text, and the 16-bit instructions Thumb-1 lacks (arm_ext.hpp).

#include <bit>
#include <cctype>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/arm_ext.hpp"

namespace cb::decompile::arm::t2 {

namespace {

constexpr std::uint8_t kAlways = 14;

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const unsigned shift = 32 - bits;
    return static_cast<std::int32_t>(v << shift) >> shift;
}

void fill_raw(Insn& i) {
    for (unsigned k = 0; k < i.length; ++k) i.raw[k] = static_cast<std::uint8_t>(i.word >> (8 * k));
}

struct Row16 {
    const char* name;
    std::uint16_t mask, value;
};
constexpr Row16 kHints[] = {{"nop.n", 0xFFFF, 0xBF00}, {"yield", 0xFFFF, 0xBF10}, {"wfe", 0xFFFF, 0xBF20}, {"wfi", 0xFFFF, 0xBF30}, {"sev", 0xFFFF, 0xBF40}};
constexpr Row16 kExtends[] = {{"sxth", 0xFFC0, 0xB200}, {"sxtb", 0xFFC0, 0xB240}, {"uxth", 0xFFC0, 0xB280}, {"uxtb", 0xFFC0, 0xB2C0},
                              {"rev", 0xFFC0, 0xBA00},  {"rev16", 0xFFC0, 0xBA40}, {"revsh", 0xFFC0, 0xBAC0}};

std::string target_name(std::uint32_t target, const AddressLabels& labels) {
    const auto it = labels.find(target & ~1u);
    if (it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", target & ~1u);
}

// Capstone's text of the 32-bit instruction at `pc`.
std::string capstone_text(const Insn& insn) {
    namespace cs = cb::disasm::capstone;
    thread_local csh handle = 0;
    try {
        const cs::Api& api = cs::api();
        if (handle == 0) cs::check(api.cs_open(CS_ARCH_ARM, static_cast<cs_mode>(CS_MODE_THUMB), &handle), "cs_open");
        cs_insn* out = nullptr;
        const std::size_t count = api.cs_disasm(handle, insn.raw.data(), insn.length, insn.pc, 1, &out);
        std::string text;
        if (count == 1) {
            text = out[0].mnemonic;
            if (out[0].op_str[0] != 0) text += std::string(" ") + out[0].op_str;
        }
        if (out) api.cs_free(out, count);
        if (!text.empty()) return text;
    } catch (const Error&) {
    }
    return std::format("udf.w 0x{:08X}", insn.word);
}

std::optional<unsigned> condition_number(std::string_view s) {
    for (unsigned c = 0; c < 14; ++c)
        if (condition_suffix(c) == s) return c;
    return std::nullopt;
}

}  // namespace

bool is_32bit(std::uint16_t first) { return first >= 0xE800; }

unsigned it_condition(std::uint8_t first_condition, std::uint8_t mask, unsigned index) {
    if (index == 0) return first_condition;
    const unsigned bit = (mask >> (4 - index)) & 1;
    return bit == (first_condition & 1u) ? first_condition : first_condition ^ 1u;
}

std::optional<Insn> decode(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged, Arch arch) {
    if (arch < Arch::V6K) return std::nullopt;
    Insn i;
    i.pc = pc_tagged & ~1u;
    i.isa = Isa::Thumb;
    i.cond = kAlways;
    if (!is_32bit(a)) {
        i.length = 2;
        i.word = a;
        i.op = Op::Ext;
        for (unsigned k = 0; k < std::size(kExtends); ++k)
            if ((a & kExtends[k].mask) == kExtends[k].value) {
                i.ext = static_cast<std::int16_t>(kExtend + k);
                i.rd = a & 7;
                i.rm = (a >> 3) & 7;
                fill_raw(i);
                return i;
            }
        if (arch < Arch::V7A) return std::nullopt;
        if ((a & 0xFF00) == 0xBF00) {
            const unsigned mask = a & 15, first = (a >> 4) & 15;
            if (mask == 0) {
                if (first > 4) return std::nullopt;
                i.ext = static_cast<std::int16_t>(kHint + first);
            } else {
                if (first == 15 || (first == 14 && (mask & 1) == 0 && false)) return std::nullopt;
                i.ext = kIt;
                i.rs = static_cast<std::uint8_t>(first);
                i.list = static_cast<std::uint16_t>(mask);
            }
            fill_raw(i);
            return i;
        }
        if ((a & 0xF500) == 0xB100) {   // CBZ, CBNZ
            i.ext = (a & 0x0800) ? kCbnz : kCbz;
            i.rn = a & 7;
            i.target = (i.pc + 4 + (((a >> 9) & 1u) << 6 | ((a >> 3) & 31u) << 1)) | 1u;
            i.flow = Flow::Branch;
            fill_raw(i);
            return i;
        }
        return std::nullopt;
    }
    if (arch < Arch::V7A) return std::nullopt;
    i.length = 4;
    i.word = static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b) << 16;
    fill_raw(i);
    if ((a & 0xF800) == 0xF000 && (b & 0x8000)) {
        const unsigned b14 = (b >> 14) & 1, b12 = (b >> 12) & 1, s = (a >> 10) & 1, j1 = (b >> 13) & 1, j2 = (b >> 11) & 1;
        if (b14 == 0 && b12 == 0) {   // B<cond>.W
            const unsigned cond = (a >> 6) & 15;
            if (cond < 14) {
                const std::uint32_t imm = s << 20 | j2 << 19 | j1 << 18 | (static_cast<std::uint32_t>(a) & 0x3F) << 12 | (static_cast<std::uint32_t>(b) & 0x7FF) << 1;
                i.op = Op::B;
                i.cond = static_cast<std::uint8_t>(cond);
                i.target = (i.pc + 4 + static_cast<std::uint32_t>(sext(imm, 21))) | 1u;
                i.flow = Flow::Branch;
                return i;
            }
        } else if (b12 == 1 || (b14 == 1 && (b & 1) == 0)) {   // B.W, BL, BLX
            const unsigned i1 = (~(j1 ^ s)) & 1, i2 = (~(j2 ^ s)) & 1;
            const std::uint32_t imm = s << 24 | i1 << 23 | i2 << 22 | (static_cast<std::uint32_t>(a) & 0x3FF) << 12 | (static_cast<std::uint32_t>(b) & 0x7FF) << 1;
            const std::uint32_t offset = static_cast<std::uint32_t>(sext(imm, 25));
            if (b12 == 1) {
                i.op = b14 ? Op::Bl : Op::B;
                i.target = (i.pc + 4 + offset) | 1u;
                i.flow = b14 ? Flow::Call : Flow::Jump;
            } else {
                i.op = Op::Blx;
                i.target = ((i.pc + 4) & ~3u) + offset;
                i.target = *i.target & ~3u;
                i.flow = Flow::Call;
            }
            return i;
        }
    }
    i.op = Op::Ext;
    if ((a >> 12) == 0xE && ((a >> 8) & 0xF) >= 0xC) {   // the coprocessor space: the VFP's encodings are the ARM ones with condition 1110
        const std::uint32_t w = static_cast<std::uint32_t>(a) << 16 | b;
        const int row = ext::find_arm(w, arch);
        if (row >= 0) {
            i.ext = static_cast<std::int16_t>(row);
            return i;
        }
    }
    i.ext = kOpaque;
    return i;
}

std::string text(const Insn& i, const AddressLabels& labels) {
    const int row = i.ext;
    if (row == kIt) {
        const unsigned first = i.rs, mask = i.list;
        const unsigned count = 4 - static_cast<unsigned>(std::countr_zero(mask));
        std::string name = "it";
        for (unsigned k = 1; k < count; ++k) name += ((mask >> (4 - k)) & 1) == (first & 1u) ? 't' : 'e';
        return std::format("{} {}", name, condition_suffix(first));
    }
    if (row == kCbz || row == kCbnz) return std::format("{} {}, {}", row == kCbz ? "cbz" : "cbnz", register_name(i.rn), target_name(*i.target, labels));
    if (row >= kHint && row < kHint + static_cast<int>(std::size(kHints))) return kHints[row - kHint].name;
    if (row >= kExtend && row < kExtend + static_cast<int>(std::size(kExtends)))
        return std::format("{} {}, {}", kExtends[row - kExtend].name, register_name(i.rd), register_name(i.rm));
    if (row >= 0 && row < 1000) {
        const std::uint32_t w = (i.word << 16) | (i.word >> 16);
        return ext::text(row, w, i.pc, labels);
    }
    return capstone_text(i);
}

std::optional<Bytes> assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels) {
    line = text::trim(line);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string mnemonic = text::lower(line.substr(0, split));
    const std::string_view rest = text::ltrim(line.substr(split));
    const auto bytes16 = [](unsigned h) { return Bytes{static_cast<std::uint8_t>(h), static_cast<std::uint8_t>(h >> 8)}; };
    for (unsigned k = 0; k < std::size(kHints); ++k)
        if (mnemonic == kHints[k].name && rest.empty()) return bytes16(kHints[k].value);
    if (mnemonic.starts_with("it") && mnemonic.size() <= 5 && mnemonic.find_first_not_of("te", 2) == std::string::npos) {
        const auto first = condition_number(text::lower(rest));
        if (!first) throw ParseError("", 0, 0, std::string(line) + " (a condition is expected)");
        const unsigned count = static_cast<unsigned>(mnemonic.size() - 2) + 1;
        unsigned mask = 1u << (4 - count);
        for (unsigned k = 1; k < count; ++k) {
            const bool then = mnemonic[1 + k] == 't';
            mask |= ((then ? *first : *first ^ 1u) & 1u) << (4 - k);
        }
        return bytes16(0xBF00 | *first << 4 | mask);
    }
    if (mnemonic == "cbz" || mnemonic == "cbnz") {
        const std::size_t comma = rest.find(',');
        if (comma == std::string_view::npos) throw ParseError("", 0, 0, std::string(line) + " (a register and a target are expected)");
        const auto reg = register_number(rest.substr(0, comma));
        if (!reg || *reg > 7) throw ParseError("", 0, 0, std::string(line) + " (cbz takes a low register)");
        const std::int64_t target = value(text::trim(rest.substr(comma + 1)), labels, pc);
        const std::int64_t distance = (target & ~1LL) - (static_cast<std::int64_t>(pc & ~1u) + 4);
        if (distance < 0 || distance > 126 || (distance & 1)) throw ParseError("", 0, 0, std::string(line) + " (the target is out of reach: cbz branches forward up to 126 bytes)");
        const unsigned d = static_cast<unsigned>(distance);
        return bytes16(0xB100 | (mnemonic == "cbnz" ? 0x0800u : 0u) | ((d >> 6) & 1) << 9 | ((d >> 1) & 31) << 3 | *reg);
    }
    for (unsigned k = 0; k < std::size(kExtends); ++k) {
        if (mnemonic != kExtends[k].name) continue;
        const std::size_t comma = rest.find(',');
        const auto rd = comma == std::string_view::npos ? std::nullopt : register_number(rest.substr(0, comma));
        const auto rm = comma == std::string_view::npos ? std::nullopt : register_number(rest.substr(comma + 1));
        if (!rd || !rm || *rd > 7 || *rm > 7) return std::nullopt;   // (the 32-bit forms are not written)
        return bytes16(kExtends[k].value | *rm << 3 | *rd);
    }
    // the VFP in Thumb: the ARM row's word with condition 1110, the halfwords in order
    if (const std::optional<std::uint32_t> w = ext::assemble(line, pc, labels, Arch::V7A); w && (*w >> 24) >= 0xEC && (*w >> 28) == 0xE) {
        const std::uint32_t first = *w >> 16, second = *w & 0xFFFF;
        return Bytes{static_cast<std::uint8_t>(first), static_cast<std::uint8_t>(first >> 8), static_cast<std::uint8_t>(second), static_cast<std::uint8_t>(second >> 8)};
    }
    return std::nullopt;
}

}  // namespace cb::decompile::arm::t2
