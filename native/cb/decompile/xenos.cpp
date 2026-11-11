#include "cb/decompile/xenos.hpp"

#include <format>

#include "cb/core/text.hpp"

namespace cb::decompile::xenos {

namespace {

constexpr std::array<std::string_view, 32> kVector = {
    "add", "mul", "max", "max", "seq", "sgt", "sge", "sne", "frc", "trunc", "floor", "mad", "cndeq",
    "cndge", "cndgt", "dp4", "dp3", "dp2add", "cube", "max4", "setp_eq_push", "setp_ne_push", "setp_gt_push",
    "setp_ge_push", "kille", "killgt", "killge", "killne", "dst", "mova", "", ""};

// how many sources each vector opcode reads
constexpr std::array<std::uint8_t, 32> kVectorSources = {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 2,
                                                         2, 3, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0};

constexpr std::array<std::string_view, 64> kScalar = [] {
    std::array<std::string_view, 64> t{};
    const std::pair<int, std::string_view> names[] = {
        {0x00, "adds"}, {0x01, "adds_prev"}, {0x02, "muls"}, {0x03, "muls_prev"}, {0x04, "muls_prev2"},
        {0x05, "maxs"}, {0x06, "mins"}, {0x07, "seqs"}, {0x08, "sgts"}, {0x09, "sges"}, {0x0a, "snes"},
        {0x0b, "frcs"}, {0x0c, "truncs"}, {0x0d, "floors"}, {0x0e, "exp"}, {0x0f, "logc"}, {0x10, "log"},
        {0x11, "rcpc"}, {0x12, "rcpf"}, {0x13, "rcp"}, {0x14, "rsqc"}, {0x15, "rsqf"}, {0x16, "rsq"},
        {0x17, "mova"}, {0x18, "movaf"}, {0x1a, "subs_prev"}, {0x1b, "setp_eq"}, {0x1c, "setp_ne"},
        {0x1d, "setp_gt"}, {0x1e, "setp_ge"}, {0x1f, "setp_inv"}, {0x20, "setp_pop"}, {0x21, "setp_clr"},
        {0x22, "setp_rst"}, {0x23, "kills_eq"}, {0x24, "kills_gt"}, {0x25, "kills_ge"}, {0x26, "kills_ne"},
        {0x27, "kills_one"}, {0x28, "sqrt"}, {0x2a, "mulsc0"}, {0x2b, "mulsc1"}, {0x2c, "addsc0"},
        {0x2d, "addsc1"}, {0x2e, "subsc0"}, {0x2f, "subsc1"}, {0x30, "sin"}, {0x31, "cos"}};
    for (const auto& [op, name] : names) t[static_cast<std::size_t>(op)] = name;
    return t;
}();

constexpr std::array<std::string_view, 13> kControl = {"nop",  "exec",    "exece",  "cexec", "cexece",
                                                       "cexecp", "cexecpe", "loop",   "endloop", "call",
                                                       "ret",  "jmp",     "alloc"};

constexpr std::string_view kChannels = "xyzw";

}  // namespace

std::string_view vector_name(std::uint32_t opcode) { return opcode < kVector.size() ? kVector[opcode] : ""; }

std::uint32_t vector_sources(std::uint32_t opcode) { return opcode < kVectorSources.size() ? kVectorSources[opcode] : 3; }

std::string_view scalar_name(std::uint32_t opcode) { return opcode < kScalar.size() ? kScalar[opcode] : ""; }

std::string_view control_name(std::uint32_t opcode) { return opcode < kControl.size() ? kControl[opcode] : ""; }

std::string mask_text(std::uint32_t mask) {
    std::string out;
    for (std::uint32_t i = 0; i < 4; ++i) {
        if (mask & (1u << i)) out += kChannels[i];
    }
    return out;
}

std::string swizzle_text(std::uint32_t swizzle) {
    std::string out;
    for (std::uint32_t i = 0; i < 4; ++i) out += kChannels[(i + ((swizzle >> (2 * i)) & 3)) & 3];
    return out;
}

std::string register_text(bool selected, std::uint32_t number) {
    std::string name = selected ? std::format("r{}", number & 0x3f) : std::format("c{}", number);
    if (number & 0x80) name += "_abs";
    if (number & 0x40) name += "[aL]";
    return name;
}

std::vector<std::string> alu(std::uint32_t d0, std::uint32_t d1, std::uint32_t d2) {
    const std::uint32_t vop = (d2 >> 24) & 0x1f, sop = (d0 >> 26) & 0x3f;
    const std::uint32_t vdst = d0 & 0x3f, sdst = (d0 >> 8) & 0x3f;
    const std::uint32_t vmask = (d0 >> 16) & 0xf, smask = (d0 >> 20) & 0xf;
    const bool is_export = (d0 >> 15) & 1, vsat = (d0 >> 24) & 1, ssat = (d0 >> 25) & 1;

    // sources A, B, C: register number (d2), swizzle (d1), constant-or-temporary select (d2), negate (d1)
    struct Field {
        std::uint32_t reg_shift, swizzle_shift, select_bit, negate_bit;
    };
    static constexpr Field kFields[3] = {{16, 16, 31, 26}, {8, 8, 30, 25}, {0, 0, 29, 24}};
    auto source = [&](std::size_t i) {
        const Field& f = kFields[i];
        const std::uint32_t number = (d2 >> f.reg_shift) & 0xff, swizzle = (d1 >> f.swizzle_shift) & 0xff;
        const bool select = (d2 >> f.select_bit) & 1, negate = (d1 >> f.negate_bit) & 1;
        return std::string(negate ? "-" : "") + register_text(select, number) + "." + swizzle_text(swizzle);
    };

    std::vector<std::string> out;
    if (is_export || vmask) {
        const std::uint32_t count = vector_sources(vop);
        const std::string dst = is_export ? "oC0" : std::format("r{}.{}", vdst, mask_text(vmask));
        // max of a source with itself is how the compiler writes a move, and how it lists it
        if ((vop == 2 || vop == 3) && count >= 2 && source(0) == source(1)) {
            out.push_back(std::format("mov{} {}, {}", vsat ? "_sat" : "", dst, source(0)));
        } else {
            std::string name(vector_name(vop));
            if (name.empty()) name = std::format("vop{:02x}", vop);
            std::string sources;
            for (std::uint32_t i = 0; i < count; ++i) sources += (i ? ", " : "") + source(i);
            out.push_back(std::format("{}{} {}, {}", name, vsat ? "_sat" : "", dst, sources));
        }
    }
    if (smask) {   // the scalar operation reads source C
        std::string name(scalar_name(sop));
        if (name.empty()) name = std::format("sop{:02x}", sop);
        out.push_back(std::format("{}{} r{}.{}, {}", name, ssat ? "_sat" : "", sdst, mask_text(smask), source(2)));
    }
    return out;
}

std::vector<std::string> control(std::uint32_t d0, std::uint32_t d1, std::uint32_t d2) {
    // v = d0 | d1 << 32 | d2 << 64 is 96 bits: its two 48-bit halves, built from the 32-bit words
    const std::uint64_t halves[2] = {d0 | (static_cast<std::uint64_t>(d1 & 0xffff) << 32),
                                     (d1 >> 16) | (static_cast<std::uint64_t>(d2) << 16)};
    std::vector<std::string> out;
    for (std::uint64_t half : halves) {
        const auto op = static_cast<std::uint32_t>((half >> 44) & 0xf);
        std::string_view name = control_name(op);
        out.push_back(name.empty() ? std::format("cf{:x}", op) : std::string(name));
    }
    return out;
}

std::vector<std::string> disassemble(std::span<const std::uint32_t> dwords) {
    std::vector<std::string> out;
    if (dwords.size() >= 3) {
        out.push_back("CF: " + text::join(control(dwords[0], dwords[1], dwords[2]), " / "));
    }
    for (std::size_t i = 3; i + 2 < dwords.size(); i += 3) {
        if (!dwords[i] && !dwords[i + 1] && !dwords[i + 2]) break;
        for (std::string& line : alu(dwords[i], dwords[i + 1], dwords[i + 2])) out.push_back(std::move(line));
    }
    return out;
}

}  // namespace cb::decompile::xenos
