// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_ops_mips.cpp -- the PlayStation adapter's own semantics for the GTE (coprocessor 2): its registers (what MFC2 MTC2 CFC2 CTC2 LWC2 SWC2 do to them: SXYP, IRGB,
// ORGB, LZCS, FLAG), and the commands NCLIP AVSZ3 AVSZ4 SQR OP GPF GPL MVMVA with the saturation flags. The description names the coprocessor operations (getCopReg,
// setCopReg, copFunction) and gives them no meaning; the adapter binds them here. The other commands (RTPS RTPT NCDS ...) are not run: the run ends "not run" as the lane's own core's does.
#include "engine/pcode_ops_mips.hpp"

#include <algorithm>
#include <array>
#include <bit>

namespace cb::engine::pcode_ops_mips {

namespace {


std::int32_t sext(std::uint32_t v, unsigned bits) {
    const unsigned shift = 32 - bits;
    return static_cast<std::int32_t>(v << shift) >> shift;
}

// ---- the GTE's registers (the commands are not run) ------------------------------------------------------------------

std::uint32_t gte_read(const Gte& cpu, unsigned n) {
    const auto& d = cpu.data;
    switch (n) {
        case 1: case 3: case 5: case 8: case 9: case 10: case 11: return static_cast<std::uint32_t>(sext(d[n], 16));   // VZ0 VZ1 VZ2 IR0-IR3
        case 7: case 16: case 17: case 18: case 19: return d[n] & 0xFFFF;                                              // OTZ SZ0-SZ3
        case 15: return d[14];                                                                                          // SXYP reads SXY2
        case 28:
        case 29: {                                                                                                      // IRGB, ORGB: the IR registers as 5-bit colours
            const auto channel = [&](unsigned i) { return static_cast<std::uint32_t>(std::clamp(sext(d[9 + i], 16) / 0x80, 0, 0x1F)); };
            return channel(0) | channel(1) << 5 | channel(2) << 10;
        }
        default: return d[n & 31];
    }
}

void gte_write(Gte& cpu, unsigned n, std::uint32_t v) {
    auto& d = cpu.data;
    switch (n) {
        case 15:   // SXYP: push onto the screen XY FIFO
            d[12] = d[13];
            d[13] = d[14];
            d[14] = v;
            d[15] = v;
            break;
        case 28:   // IRGB: set IR1-IR3 from the 5-bit colours
            d[9] = (v & 0x1F) * 0x80;
            d[10] = ((v >> 5) & 0x1F) * 0x80;
            d[11] = ((v >> 10) & 0x1F) * 0x80;
            d[28] = v & 0x7FFF;
            break;
        case 29: break;   // ORGB: read only
        case 30: {        // LZCS: LZCR is the count of leading equal bits
            d[30] = v;
            d[31] = static_cast<std::uint32_t>((static_cast<std::int32_t>(v) < 0) ? std::countl_one(v) : std::countl_zero(v));
            break;
        }
        case 31: break;   // LZCR: read only
        default: d[n & 31] = v; break;
    }
}

std::uint32_t gte_control_read(const Gte& cpu, unsigned n) {
    const auto& c = cpu.control;
    switch (n) {
        case 4: case 12: case 20: case 26: case 27: case 29: case 30: return static_cast<std::uint32_t>(sext(c[n], 16));   // the 16-bit control registers
        case 31: return (c[31] & 0x7FFFF000u) | ((c[31] & 0x7F87E000u) != 0 ? 0x80000000u : 0);                            // FLAG: bit 31 is the OR of the errors
        default: return c[n & 31];
    }
}



constexpr std::uint32_t kFlagMac1Plus = 1u << 30, kFlagMac2Plus = 1u << 29, kFlagMac3Plus = 1u << 28;
constexpr std::uint32_t kFlagMac1Minus = 1u << 27, kFlagMac2Minus = 1u << 26, kFlagMac3Minus = 1u << 25;
constexpr std::uint32_t kFlagIr1 = 1u << 24, kFlagColour = 1u << 21, kFlagOtz = 1u << 18, kFlagMac0Plus = 1u << 16, kFlagMac0Minus = 1u << 15;

class GteCommand {
public:
    GteCommand(Gte& cpu, std::uint32_t command)
        : d_(cpu.data), c_(cpu.control), shift_(((command >> 19) & 1) * 12), lm_(((command >> 10) & 1) != 0), command_(command) {
    }

    bool run(std::string_view name) {
        static constexpr std::array<std::string_view, 8> kRun = {"nclip", "avsz3", "avsz4", "sqr", "op", "gpf", "gpl", "mvmva"};
        if (std::ranges::find(kRun, name) == kRun.end()) return false;
        c_[31] = 0;
        if (name == "nclip") nclip();
        else if (name == "avsz3") average(c_[29], 17, 3);
        else if (name == "avsz4") average(c_[30], 16, 4);
        else if (name == "sqr") sqr();
        else if (name == "op") cross();
        else if (name == "gpf") interpolate(false);
        else if (name == "gpl") interpolate(true);
        else multiply();
        return true;
    }

private:
    static std::int32_t half(std::uint32_t v, unsigned which) { return static_cast<std::int16_t>(v >> (16 * which)); }
    std::int64_t ir(unsigned i) const { return half(d_[8 + i], 0); }

    // MACn: the 44-bit value checked, then taken after the shift as 32 bits
    void set_mac(unsigned i, std::int64_t v) {
        constexpr std::int64_t kMax = (std::int64_t{1} << 43) - 1, kMin = -(std::int64_t{1} << 43);
        if (v > kMax) c_[31] |= i == 1 ? kFlagMac1Plus : i == 2 ? kFlagMac2Plus : kFlagMac3Plus;
        if (v < kMin) c_[31] |= i == 1 ? kFlagMac1Minus : i == 2 ? kFlagMac2Minus : kFlagMac3Minus;
        d_[24 + i] = static_cast<std::uint32_t>(static_cast<std::int32_t>(v >> shift_));
    }
    // IRn from MACn, saturated to 16 bits (0 and up when lm is set)
    void set_ir(unsigned i) {
        const std::int32_t mac = static_cast<std::int32_t>(d_[24 + i]);
        const std::int32_t lo = lm_ ? 0 : -0x8000;
        std::int32_t v = mac;
        if (v > 0x7FFF || v < lo) {
            c_[31] |= kFlagIr1 >> (i - 1);
            v = v > 0x7FFF ? 0x7FFF : lo;
        }
        d_[8 + i] = static_cast<std::uint32_t>(v);
    }
    void set_mac0(std::int64_t v) {
        if (v > 0x7FFFFFFFll) c_[31] |= kFlagMac0Plus;
        if (v < -0x80000000ll) c_[31] |= kFlagMac0Minus;
        d_[24] = static_cast<std::uint32_t>(v);
    }
    void push_colour() {
        std::uint32_t rgb = d_[6] & 0xFF000000u;
        for (unsigned i = 0; i < 3; ++i) {
            std::int32_t v = static_cast<std::int32_t>(d_[25 + i]) >> 4;
            if (v < 0 || v > 255) {
                c_[31] |= kFlagColour >> i;
                v = v < 0 ? 0 : 255;
            }
            rgb |= static_cast<std::uint32_t>(v) << (8 * i);
        }
        d_[20] = d_[21];
        d_[21] = d_[22];
        d_[22] = rgb;
    }

    void nclip() {
        const std::int64_t sx0 = half(d_[12], 0), sy0 = half(d_[12], 1), sx1 = half(d_[13], 0), sy1 = half(d_[13], 1), sx2 = half(d_[14], 0),
                           sy2 = half(d_[14], 1);
        set_mac0(sx0 * sy1 + sx1 * sy2 + sx2 * sy0 - sx0 * sy2 - sx1 * sy0 - sx2 * sy1);
    }
    void average(std::uint32_t factor, unsigned first, unsigned count) {
        std::int64_t sum = 0;
        for (unsigned i = 0; i < count; ++i) sum += d_[first + i] & 0xFFFF;
        const std::int64_t v = static_cast<std::int64_t>(half(factor, 0)) * sum;
        set_mac0(v);
        std::int64_t otz = v >> 12;
        if (otz < 0 || otz > 0xFFFF) {
            c_[31] |= kFlagOtz;
            otz = otz < 0 ? 0 : 0xFFFF;
        }
        d_[7] = static_cast<std::uint32_t>(otz);
    }
    void sqr() {
        for (unsigned i = 1; i <= 3; ++i) set_mac(i, ir(i) * ir(i));
        for (unsigned i = 1; i <= 3; ++i) set_ir(i);
    }
    void cross() {
        const std::int64_t d1 = half(c_[0], 0), d2 = half(c_[2], 0), d3 = half(c_[4], 0);
        set_mac(1, d2 * ir(3) - d3 * ir(2));
        set_mac(2, d3 * ir(1) - d1 * ir(3));
        set_mac(3, d1 * ir(2) - d2 * ir(1));
        for (unsigned i = 1; i <= 3; ++i) set_ir(i);
    }
    void interpolate(bool add) {
        const std::int64_t ir0 = half(d_[8], 0);
        for (unsigned i = 1; i <= 3; ++i) {
            const std::int64_t base = add ? static_cast<std::int64_t>(static_cast<std::int32_t>(d_[24 + i])) << shift_ : 0;
            set_mac(i, base + ir0 * ir(i));
        }
        for (unsigned i = 1; i <= 3; ++i) set_ir(i);
        push_colour();
    }
    void multiply() {
        const unsigned mx = (command_ >> 17) & 3, v = (command_ >> 15) & 3, cv = (command_ >> 13) & 3;
        const unsigned base = mx == 0 ? 0 : mx == 1 ? 8 : 16;   // RT, the light matrix, the light colour matrix (3: garbage; the RT is used)
        const auto element = [&](unsigned r, unsigned col) {
            const unsigned k = 3 * r + col;
            return static_cast<std::int64_t>(half(c_[base + k / 2], k % 2));
        };
        std::array<std::int64_t, 3> vec{};
        if (v < 3) {
            vec = {half(d_[2 * v], 0), half(d_[2 * v], 1), half(d_[2 * v + 1], 0)};
        } else {
            vec = {ir(1), ir(2), ir(3)};
        }
        const unsigned control = cv == 0 ? 5 : cv == 1 ? 13 : 21;   // TR, BK, FC (3: none)
        for (unsigned i = 0; i < 3; ++i) {
            const std::int64_t t = cv == 3 ? 0 : static_cast<std::int64_t>(static_cast<std::int32_t>(c_[control + i])) << 12;
            set_mac(i + 1, t + element(i, 0) * vec[0] + element(i, 1) * vec[1] + element(i, 2) * vec[2]);
        }
        for (unsigned i = 1; i <= 3; ++i) set_ir(i);
    }

    std::array<std::uint32_t, 32>& d_;
    std::array<std::uint32_t, 32>& c_;
    unsigned shift_;
    bool lm_;
    std::uint32_t command_;
};

}  // namespace
std::uint32_t gte_register(const Gte& gte, unsigned n) { return gte_read(gte, n); }
void gte_set_register(Gte& gte, unsigned n, std::uint32_t value) { gte_write(gte, n, value); }
std::uint32_t gte_control_register(const Gte& gte, unsigned n) { return gte_control_read(gte, n); }
bool gte_run_command(Gte& gte, std::uint32_t command, std::string_view name) { return GteCommand(gte, command).run(name); }

}  // namespace cb::engine::pcode_ops_mips
