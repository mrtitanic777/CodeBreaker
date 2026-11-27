// emulate/rsp.cpp -- the RSP interpreter (rsp.hpp). The vector unit follows the behaviour the N64 emulation community has published from
// tests on consoles (n64brew's RSP pages, ares's and cen64's sources): the accumulator's lanes, the clamps, the flags and the divide tables.

#include "emulate/rsp.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <format>

#include "core/error.hpp"

namespace cb::emulate::rsp {

namespace {

constexpr std::uint64_t kAccMask = 0xFFFFFFFFFFFFull;

// The accumulator's 48 bits as a signed number, and back.
std::int64_t acc_value(std::uint64_t a) { return static_cast<std::int64_t>(a << 16) >> 16; }
std::uint64_t acc_of(std::int64_t v) { return static_cast<std::uint64_t>(v) & kAccMask; }

std::uint16_t acc_low(std::uint64_t a) { return static_cast<std::uint16_t>(a); }
std::uint16_t acc_mid(std::uint64_t a) { return static_cast<std::uint16_t>(a >> 16); }
std::uint16_t acc_high(std::uint64_t a) { return static_cast<std::uint16_t>(a >> 32); }

// The middle 16 bits clamped to a signed 16-bit value (from the 32 bits above the low lane).
std::uint16_t clamp_signed(std::uint64_t a) {
    const std::int64_t v = acc_value(a) >> 16;
    if (v < -32768) return 0x8000;
    if (v > 32767) return 0x7FFF;
    return static_cast<std::uint16_t>(v);
}
// The middle 16 bits as unsigned: 0 when negative, 0xFFFF when they overflow.
std::uint16_t clamp_unsigned_mid(std::uint64_t a) {
    const std::int64_t v = acc_value(a) >> 16;
    if (v < 0) return 0;
    if (v > 32767) return 0xFFFF;
    return static_cast<std::uint16_t>(v);
}
// The low 16 bits, when the upper 32 are their sign; else 0 or 0xFFFF (VMADL, VMADN).
std::uint16_t clamp_unsigned_low(std::uint64_t a) {
    const std::int64_t v = acc_value(a) >> 16;
    if (v < -32768) return 0;
    if (v > 32767) return 0xFFFF;
    return acc_low(a);
}

std::int16_t s16(std::uint16_t v) { return static_cast<std::int16_t>(v); }

// Which lane of vt a lane reads, for element `e`.
unsigned lane_of(unsigned i, unsigned e) {
    if (e <= 1) return i;
    if (e <= 3) return (i & ~1u) | (e & 1u);
    if (e <= 7) return (i & ~3u) | (e & 3u);
    return e & 7u;
}

std::int32_t reciprocal(std::int32_t input, bool sixteen) {
    if (input == 0) return 0x7FFFFFFF;
    if (sixteen && input == -32768) return static_cast<std::int32_t>(0xFFFF0000u);
    const std::uint32_t mask = static_cast<std::uint32_t>(input >> 31);
    std::uint32_t data = static_cast<std::uint32_t>(input) ^ mask;
    if (input > -32768) data -= mask;
    if (data == 0) return 0x7FFFFFFF;
    const unsigned shift = static_cast<unsigned>(std::countl_zero(data));
    const std::uint32_t index = static_cast<std::uint32_t>(((static_cast<std::uint64_t>(data) << shift) & 0x7FC00000u) >> 22);
    std::uint32_t result = Rsp::reciprocals()[index];
    result = (0x10000u | result) << 14;
    return static_cast<std::int32_t>((result >> (31 - shift)) ^ mask);
}

std::int32_t inverse_root(std::int32_t input, bool sixteen) {
    if (input == 0) return 0x7FFFFFFF;
    if (sixteen && input == -32768) return static_cast<std::int32_t>(0xFFFF0000u);
    const std::uint32_t mask = static_cast<std::uint32_t>(input >> 31);
    std::uint32_t data = static_cast<std::uint32_t>(input) ^ mask;
    if (input > -32768) data -= mask;
    if (data == 0) return 0x7FFFFFFF;
    const unsigned shift = static_cast<unsigned>(std::countl_zero(data));
    std::uint32_t index = static_cast<std::uint32_t>(((static_cast<std::uint64_t>(data) << shift) & 0x7FC00000u) >> 22);
    index = (index & 0x1FE) | (shift & 1);
    std::uint32_t result = Rsp::inverse_roots()[index];
    result = (0x10000u | result) << 14;
    return static_cast<std::int32_t>((result >> ((31 - shift) >> 1)) ^ mask);
}

}  // namespace

// The ROM's entries are 1/x and 1/sqrt(x) for the 512 mantissas x in [1, 2), in 17 bits with the top bit (always 1) left out; 1.0's own
// result does not fit and saturates to 0xFFFF. The inverse square root's odd entries are those of 2x (the exponent's parity).
const std::array<std::uint16_t, 512>& Rsp::reciprocals() {
    static const std::array<std::uint16_t, 512> table = [] {
        std::array<std::uint16_t, 512> t{};
        for (std::uint64_t i = 0; i < 512; ++i) {
            const std::uint64_t v = (((1ull << 34) / (i + 512)) + 1) >> 8;
            t[i] = v >= 0x20000 ? std::uint16_t{0xFFFF} : static_cast<std::uint16_t>(v);
        }
        return t;
    }();
    return table;
}

const std::array<std::uint16_t, 512>& Rsp::inverse_roots() {
    static const std::array<std::uint16_t, 512> table = [] {
        std::array<std::uint16_t, 512> t{};
        for (std::uint64_t i = 0; i < 512; ++i) {
            // the largest v with v^2 * x <= 2^34, x = n / 512 (n = the mantissa's 9 bits, doubled for an odd entry)
            const std::uint64_t n = ((i & ~1ull) + 512) << (i & 1);
            std::uint64_t v = static_cast<std::uint64_t>(std::sqrt(static_cast<double>(1ull << 43) / static_cast<double>(n))) + 2;
            while (v * v * n > (1ull << 43)) --v;   // (exact from a near start)
            t[i] = v >= 0x20000 ? std::uint16_t{0xFFFF} : static_cast<std::uint16_t>(v);
        }
        return t;
    }();
    return table;
}

Rsp::Rsp(ByteSpan sp, ByteSpan rdram) : sp_(sp), rdram_(rdram) {
    if (sp_.size() < 2 * kMemSize) throw UsageError("the RSP needs DMEM and IMEM (8 KB)");
}

std::uint8_t Rsp::byte(unsigned reg, unsigned b) const {
    const std::uint16_t lane = v_[reg & 31][(b & 15) >> 1];
    return (b & 1) ? static_cast<std::uint8_t>(lane) : static_cast<std::uint8_t>(lane >> 8);
}

void Rsp::set_byte(unsigned reg, unsigned b, std::uint8_t value) {
    std::uint16_t& lane = v_[reg & 31][(b & 15) >> 1];
    lane = (b & 1) ? static_cast<std::uint16_t>((lane & 0xFF00) | value) : static_cast<std::uint16_t>((lane & 0x00FF) | (value << 8));
}

std::uint32_t Rsp::load(std::uint32_t a, unsigned size, bool sign) const {
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size; ++i) v = (v << 8) | dmem(a + i);
    if (sign && size < 4) {
        const unsigned bits = 32 - 8 * size;
        v = static_cast<std::uint32_t>(static_cast<std::int32_t>(v << bits) >> bits);
    }
    return v;
}

std::uint32_t Rsp::fetch(std::uint32_t pc) const {
    const std::size_t at = kMemSize + (pc & 0xFFC);
    return static_cast<std::uint32_t>(sp_[at]) << 24 | static_cast<std::uint32_t>(sp_[at + 1]) << 16 | static_cast<std::uint32_t>(sp_[at + 2]) << 8 | sp_[at + 3];
}

void Rsp::store(std::uint32_t a, unsigned size, std::uint32_t v) {
    for (unsigned i = 0; i < size; ++i) set_dmem(a + i, static_cast<std::uint8_t>(v >> (8 * (size - 1 - i))));
}

void Rsp::dma(bool to_rdram, std::uint32_t length) {
    // the length register: bits 0-11 the row length - 1, 12-19 the row count - 1, 20-31 the skip between rows in RDRAM
    const std::uint32_t row = (length & 0xFFF) + 1, count = ((length >> 12) & 0xFF) + 1, skip = length >> 20;
    std::uint32_t mem = mem_addr_ & 0x1FF8, dram = dram_addr_ & 0xFFFFF8;
    const std::uint32_t row8 = (row + 7) & ~7u;
    for (std::uint32_t r = 0; r < count; ++r) {
        for (std::uint32_t i = 0; i < row8; ++i) {
            const std::uint32_t m = (mem & 0x1000) | ((mem + i) & 0xFFF);
            const std::uint32_t d = dram + i;
            if (d >= rdram_.size()) continue;
            if (to_rdram) rdram_[d] = sp_[m];
            else sp_[m] = rdram_[d];
        }
        mem = (mem & 0x1000) | ((mem + row8) & 0xFFF);
        dram += row8 + skip;
    }
    mem_addr_ = mem;
    dram_addr_ = dram;
}

std::uint32_t Rsp::cop0(unsigned reg) const {
    switch (reg & 15) {
        case 0: return mem_addr_;
        case 1: return dram_addr_;
        case 2:
        case 3: return 0xFF8;   // (the lengths read back as done)
        case 4: return status_;
        case 5:
        case 6: return 0;       // DMA full, busy
        case 7: return semaphore_;
        default: return dp_[reg & 7];
    }
}

void Rsp::set_cop0(unsigned reg, std::uint32_t v) {
    switch (reg & 15) {
        case 0: mem_addr_ = v & 0x1FF8; break;
        case 1: dram_addr_ = v & 0xFFFFF8; break;
        case 2: dma(false, v); break;
        case 3: dma(true, v); break;
        case 4: {   // SP_STATUS as written: pairs of clear / set bits
            if (v & 0x0000001) status_ &= ~kHalt;
            if (v & 0x0000002) status_ |= kHalt;
            if (v & 0x0000004) status_ &= ~kBroke;
            if (v & 0x0000020) status_ &= ~kSingleStep;
            if (v & 0x0000040) status_ |= kSingleStep;
            if (v & 0x0000080) status_ &= ~kInterruptOnBreak;
            if (v & 0x0000100) status_ |= kInterruptOnBreak;
            for (unsigned sig = 0; sig < 8; ++sig) {   // the signals, bits 7-14 of the status
                if (v & (0x200u << (2 * sig))) status_ &= ~(0x80u << sig);
                if (v & (0x400u << (2 * sig))) status_ |= 0x80u << sig;
            }
            break;
        }
        case 7: semaphore_ = 0; break;
        case 8: dp_[0] = dp_[2] = v & 0xFFFFF8; break;   // DPC_START (CURRENT follows it)
        case 9:
            dp_[1] = v & 0xFFFFF8;
            if (on_dpc) on_dpc(dp_[2], dp_[1], (dp_[3] & 1) != 0);
            dp_[2] = dp_[1];
            break;
        case 11:   // DPC_STATUS: clear / set XBUS (bits 0, 1), the rest not modelled
            if (v & 1) dp_[3] &= ~1u;
            if (v & 2) dp_[3] |= 1u;
            break;
        default: break;
    }
}

void Rsp::branch(bool taken, std::uint32_t target) {
    if (taken) next_ = target & 0xFFC;
}

RunResult Rsp::run(std::uint32_t pc, std::uint64_t max_steps) {
    pc_ = pc & 0xFFC;
    next_.reset();
    in_delay_ = false;
    status_ &= ~(kHalt | kBroke);
    RunResult result;
    while (result.steps < max_steps) {
        const std::uint32_t word = fetch(pc_);
        const std::uint32_t at = pc_;
        ++result.steps;
        if (!execute(word)) {
            if (status_ & kBroke) {
                result.broke = true;
            } else if (!(status_ & kHalt)) {
                result.fault = std::format("not an RSP instruction 0x{:08X} at 0x{:03X}", word, at);
            } else {
                result.fault = "halted by a write to SP_STATUS";
            }
            return result;
        }
    }
    result.fault = "step limit";
    return result;
}

bool Rsp::step() {
    if (status_ & kHalt) return false;
    return execute(fetch(pc_));
}

bool Rsp::execute(std::uint32_t w) {
    // where the next instruction is: a branch taken in the previous instruction goes after this one (its delay slot)
    const bool delay = next_.has_value();
    const std::uint32_t target = delay ? *next_ : 0;
    next_.reset();
    const std::uint32_t pc = pc_;
    pc_ = (pc_ + 4) & 0xFFC;

    const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sa = (w >> 6) & 31, fn = w & 63;
    const std::uint32_t imm = w & 0xFFFF;
    const std::uint32_t simm = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(imm)));
    std::uint32_t* const d = &r_[rd];
    const std::uint32_t s = r_[rs], t = r_[rt];
    const std::uint32_t branch_to = pc + 4 + (simm << 2);
    bool ok = true;
    switch (op) {
        case 0x00:   // SPECIAL
            switch (fn) {
                case 0x00: *d = t << sa; break;
                case 0x02: *d = t >> sa; break;
                case 0x03: *d = static_cast<std::uint32_t>(static_cast<std::int32_t>(t) >> sa); break;
                case 0x04: *d = t << (s & 31); break;
                case 0x06: *d = t >> (s & 31); break;
                case 0x07: *d = static_cast<std::uint32_t>(static_cast<std::int32_t>(t) >> (s & 31)); break;
                case 0x08: branch(true, s); break;
                case 0x09: *d = (pc + 8) & 0xFFC; branch(true, s); break;
                case 0x0D: status_ |= kHalt | kBroke; ok = false; break;   // BREAK: the task is done
                case 0x20: case 0x21: *d = s + t; break;
                case 0x22: case 0x23: *d = s - t; break;
                case 0x24: *d = s & t; break;
                case 0x25: *d = s | t; break;
                case 0x26: *d = s ^ t; break;
                case 0x27: *d = ~(s | t); break;
                case 0x2A: *d = static_cast<std::int32_t>(s) < static_cast<std::int32_t>(t) ? 1 : 0; break;
                case 0x2B: *d = s < t ? 1 : 0; break;
                default: ok = false; break;
            }
            break;
        case 0x01: {   // REGIMM
            const bool lt = static_cast<std::int32_t>(s) < 0;
            switch (rt) {
                case 0x00: branch(lt, branch_to); break;
                case 0x01: branch(!lt, branch_to); break;
                case 0x10: r_[31] = (pc + 8) & 0xFFC; branch(lt, branch_to); break;
                case 0x11: r_[31] = (pc + 8) & 0xFFC; branch(!lt, branch_to); break;
                default: ok = false; break;
            }
            break;
        }
        case 0x02: branch(true, (w & 0x3FF) << 2); break;
        case 0x03: r_[31] = (pc + 8) & 0xFFC; branch(true, (w & 0x3FF) << 2); break;
        case 0x04: branch(s == t, branch_to); break;
        case 0x05: branch(s != t, branch_to); break;
        case 0x06: branch(static_cast<std::int32_t>(s) <= 0, branch_to); break;
        case 0x07: branch(static_cast<std::int32_t>(s) > 0, branch_to); break;
        case 0x08: case 0x09: r_[rt] = s + simm; break;
        case 0x0A: r_[rt] = static_cast<std::int32_t>(s) < static_cast<std::int32_t>(simm) ? 1 : 0; break;
        case 0x0B: r_[rt] = s < simm ? 1 : 0; break;
        case 0x0C: r_[rt] = s & imm; break;
        case 0x0D: r_[rt] = s | imm; break;
        case 0x0E: r_[rt] = s ^ imm; break;
        case 0x0F: r_[rt] = imm << 16; break;
        case 0x10:   // COP0
            if (rs == 0) r_[rt] = cop0(rd);
            else if (rs == 4) {
                set_cop0(rd, t);
                if (rd == 4 && (status_ & kHalt)) ok = false;   // the RSP halted itself
            } else ok = false;
            break;
        case 0x12:   // COP2
            if (rs & 0x10) {
                vector_op(w);
            } else {
                const unsigned e = (w >> 7) & 15;
                switch (rs) {
                    case 0: r_[rt] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>((byte(rd, e) << 8) | byte(rd, e + 1)))); break;
                    case 4:
                        set_byte(rd, e, static_cast<std::uint8_t>(t >> 8));
                        if (e + 1 < 16) set_byte(rd, e + 1, static_cast<std::uint8_t>(t));
                        break;
                    case 2: {
                        std::uint32_t v = 0;
                        if (rd == 0) v = vco_;
                        else if (rd == 1) v = vcc_;
                        else v = vce_;
                        r_[rt] = rd == 2 ? v : static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v)));
                        break;
                    }
                    case 6:
                        if (rd == 0) vco_ = static_cast<std::uint16_t>(t);
                        else if (rd == 1) vcc_ = static_cast<std::uint16_t>(t);
                        else vce_ = static_cast<std::uint8_t>(t);
                        break;
                    default: ok = false; break;
                }
            }
            break;
        case 0x20: r_[rt] = load(s + simm, 1, true); break;
        case 0x21: r_[rt] = load(s + simm, 2, true); break;
        case 0x23: case 0x27: r_[rt] = load(s + simm, 4, false); break;   // (LWU is LW on 32 bits)
        case 0x24: r_[rt] = load(s + simm, 1, false); break;
        case 0x25: r_[rt] = load(s + simm, 2, false); break;
        case 0x28: store(s + simm, 1, t); break;
        case 0x29: store(s + simm, 2, t); break;
        case 0x2B: store(s + simm, 4, t); break;
        case 0x32: vector_load(w); break;
        case 0x3A: vector_store(w); break;
        default: ok = false; break;
    }
    r_[0] = 0;
    if (delay) pc_ = target;
    if (!ok) pc_ = pc;   // (stopped at the instruction)
    return ok;
}

void Rsp::vector_op(std::uint32_t w) {
    const unsigned e = (w >> 21) & 15, vt = (w >> 16) & 31, vs = (w >> 11) & 31, vd = (w >> 6) & 31, fn = w & 63;
    const Vector a = v_[vs];
    Vector b;
    for (unsigned i = 0; i < 8; ++i) b[i] = v_[vt][lane_of(i, e)];
    Vector out = v_[vd];
    const auto set_acc_low = [&](unsigned i, std::uint16_t v) { acc_[i] = (acc_[i] & ~0xFFFFull) | v; };
    switch (fn) {
        case 0x00: case 0x01: case 0x08: case 0x09: {   // VMULF VMULU VMACF VMACU
            const bool acc = fn >= 0x08, unsign = fn & 1;
            for (unsigned i = 0; i < 8; ++i) {
                const std::int64_t product = static_cast<std::int64_t>(s16(a[i])) * s16(b[i]) * 2;
                acc_[i] = acc ? acc_of(acc_value(acc_[i]) + product) : acc_of(product + 0x8000);
                out[i] = unsign ? clamp_unsigned_mid(acc_[i]) : clamp_signed(acc_[i]);
            }
            break;
        }
        case 0x02: case 0x0A: {   // VRNDP VRNDN
            for (unsigned i = 0; i < 8; ++i) {
                std::int64_t product = s16(b[i]);
                if (vs & 1) product <<= 16;
                std::int64_t value = acc_value(acc_[i]);
                if ((fn == 0x02) ? value >= 0 : value < 0) value += product;
                acc_[i] = acc_of(value);
                out[i] = clamp_signed(acc_[i]);
            }
            break;
        }
        case 0x03: {   // VMULQ
            for (unsigned i = 0; i < 8; ++i) {
                std::int32_t product = static_cast<std::int32_t>(s16(a[i])) * s16(b[i]);
                if (product < 0) product += 31;
                acc_[i] = acc_of(static_cast<std::int64_t>(product) << 16);
                out[i] = static_cast<std::uint16_t>(clamp_signed(acc_of(acc_value(acc_[i]) >> 1)) & ~15u);
            }
            break;
        }
        case 0x0B: {   // VMACQ
            for (unsigned i = 0; i < 8; ++i) {
                std::int32_t product = static_cast<std::int32_t>(acc_value(acc_[i]) >> 16);
                if (product < 0 && !(product & (1 << 5))) product += 32;
                else if (product >= 32 && !(product & (1 << 5))) product -= 32;
                acc_[i] = acc_of((static_cast<std::int64_t>(product) << 16) | (acc_[i] & 0xFFFF));
                out[i] = static_cast<std::uint16_t>(clamp_signed(acc_of(static_cast<std::int64_t>(product >> 1) << 16)) & ~15u);
            }
            break;
        }
        case 0x04: case 0x0C:   // VMUDL VMADL
            for (unsigned i = 0; i < 8; ++i) {
                const std::int64_t product = (static_cast<std::uint32_t>(a[i]) * b[i]) >> 16;
                acc_[i] = fn == 0x04 ? acc_of(product) : acc_of(acc_value(acc_[i]) + product);
                out[i] = fn == 0x04 ? acc_low(acc_[i]) : clamp_unsigned_low(acc_[i]);
            }
            break;
        case 0x05: case 0x0D:   // VMUDM VMADM
            for (unsigned i = 0; i < 8; ++i) {
                const std::int64_t product = static_cast<std::int64_t>(s16(a[i])) * static_cast<std::int64_t>(b[i]);
                acc_[i] = fn == 0x05 ? acc_of(product) : acc_of(acc_value(acc_[i]) + product);
                out[i] = clamp_signed(acc_[i]);
            }
            break;
        case 0x06: case 0x0E:   // VMUDN VMADN
            for (unsigned i = 0; i < 8; ++i) {
                const std::int64_t product = static_cast<std::int64_t>(a[i]) * static_cast<std::int64_t>(s16(b[i]));
                acc_[i] = fn == 0x06 ? acc_of(product) : acc_of(acc_value(acc_[i]) + product);
                out[i] = fn == 0x06 ? acc_low(acc_[i]) : clamp_unsigned_low(acc_[i]);
            }
            break;
        case 0x07: case 0x0F:   // VMUDH VMADH
            for (unsigned i = 0; i < 8; ++i) {
                const std::int64_t product = (static_cast<std::int64_t>(s16(a[i])) * s16(b[i])) << 16;
                acc_[i] = fn == 0x07 ? acc_of(product) : acc_of(acc_value(acc_[i]) + product);
                out[i] = clamp_signed(acc_[i]);
            }
            break;
        case 0x10: case 0x11:   // VADD VSUB (with VCO's carry)
            for (unsigned i = 0; i < 8; ++i) {
                const std::int32_t carry = (vco_ >> i) & 1;
                const std::int32_t r = fn == 0x10 ? s16(a[i]) + s16(b[i]) + carry : s16(a[i]) - s16(b[i]) - carry;
                set_acc_low(i, static_cast<std::uint16_t>(r));
                out[i] = static_cast<std::uint16_t>(std::clamp(r, -32768, 32767));
            }
            vco_ = 0;
            break;
        case 0x13:   // VABS
            for (unsigned i = 0; i < 8; ++i) {
                std::uint16_t r = 0, low = 0;
                if (s16(a[i]) < 0) {
                    low = static_cast<std::uint16_t>(-s16(b[i]));
                    r = b[i] == 0x8000 ? 0x7FFF : low;
                } else if (s16(a[i]) > 0) {
                    r = low = b[i];
                }
                set_acc_low(i, low);
                out[i] = r;
            }
            break;
        case 0x14:   // VADDC
            vco_ = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const std::uint32_t r = static_cast<std::uint32_t>(a[i]) + b[i];
                set_acc_low(i, static_cast<std::uint16_t>(r));
                out[i] = static_cast<std::uint16_t>(r);
                if (r >> 16) vco_ |= 1u << i;
            }
            break;
        case 0x15:   // VSUBC
            vco_ = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const std::int32_t r = static_cast<std::int32_t>(a[i]) - static_cast<std::int32_t>(b[i]);
                set_acc_low(i, static_cast<std::uint16_t>(r));
                out[i] = static_cast<std::uint16_t>(r);
                if (r < 0) vco_ |= 1u << i;
                if (r != 0) vco_ |= 0x100u << i;
            }
            break;
        case 0x1D:   // VSAR
            for (unsigned i = 0; i < 8; ++i) out[i] = e == 8 ? acc_high(acc_[i]) : e == 9 ? acc_mid(acc_[i]) : e == 10 ? acc_low(acc_[i]) : 0;
            break;
        case 0x20: case 0x21: case 0x22: case 0x23: {   // VLT VEQ VNE VGE
            std::uint16_t cc = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const bool ne = (vco_ >> (8 + i)) & 1, carry = (vco_ >> i) & 1;
                const std::int16_t x = s16(a[i]), y = s16(b[i]);
                bool c = false;
                if (fn == 0x20) c = x < y || (x == y && ne && carry);
                else if (fn == 0x21) c = x == y && !ne;
                else if (fn == 0x22) c = x != y || ne;
                else c = x > y || (x == y && !(ne && carry));
                if (c) cc |= 1u << i;
                const std::uint16_t r = c ? a[i] : b[i];
                set_acc_low(i, r);
                out[i] = r;
            }
            vcc_ = cc;
            vco_ = 0;
            break;
        }
        case 0x24: {   // VCL
            std::uint16_t cc = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const bool carry = (vco_ >> i) & 1, ne = (vco_ >> (8 + i)) & 1, ce = (vce_ >> i) & 1;
                bool le = (vcc_ >> i) & 1, ge = (vcc_ >> (8 + i)) & 1;
                std::uint16_t r = 0;
                if (carry) {
                    if (!ne) {
                        const std::uint32_t sum = static_cast<std::uint32_t>(a[i]) + b[i];
                        const bool overflow = sum > 0xFFFF;
                        const bool zero = (sum & 0xFFFF) == 0;
                        le = ce ? (zero || !overflow) : (zero && overflow);
                    }
                    r = le ? static_cast<std::uint16_t>(-s16(b[i])) : a[i];
                } else {
                    if (!ne) ge = a[i] >= b[i];
                    r = ge ? b[i] : a[i];
                }
                if (le) cc |= 1u << i;
                if (ge) cc |= 0x100u << i;
                set_acc_low(i, r);
                out[i] = r;
            }
            vcc_ = cc;
            vco_ = 0;
            vce_ = 0;
            break;
        }
        case 0x25: {   // VCH
            std::uint16_t cc = 0, co = 0;
            std::uint8_t ce = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const std::int32_t x = s16(a[i]), y = s16(b[i]);
                const bool carry = (x ^ y) < 0;
                bool le = false, ge = false, ne = false;
                std::uint16_t r = 0;
                if (carry) {
                    ge = y < 0;
                    le = x + y <= 0;
                    const bool vce = x + y == -1;
                    ne = !(x + y == 0 || vce);
                    if (vce) ce |= static_cast<std::uint8_t>(1u << i);
                    r = le ? static_cast<std::uint16_t>(-y) : a[i];
                } else {
                    le = y < 0;
                    ge = x - y >= 0;
                    ne = x - y != 0;
                    r = ge ? b[i] : a[i];
                }
                if (le) cc |= 1u << i;
                if (ge) cc |= 0x100u << i;
                if (carry) co |= 1u << i;
                if (ne) co |= 0x100u << i;
                set_acc_low(i, r);
                out[i] = r;
            }
            vcc_ = cc;
            vco_ = co;
            vce_ = ce;
            break;
        }
        case 0x26: {   // VCR
            std::uint16_t cc = 0;
            for (unsigned i = 0; i < 8; ++i) {
                const std::int32_t x = s16(a[i]), y = s16(b[i]);
                bool le = false, ge = false;
                std::uint16_t r = 0;
                if ((x ^ y) < 0) {
                    ge = y < 0;
                    le = x + y + 1 <= 0;
                    r = le ? static_cast<std::uint16_t>(~b[i]) : a[i];
                } else {
                    le = y < 0;
                    ge = x - y >= 0;
                    r = ge ? b[i] : a[i];
                }
                if (le) cc |= 1u << i;
                if (ge) cc |= 0x100u << i;
                set_acc_low(i, r);
                out[i] = r;
            }
            vcc_ = cc;
            vco_ = 0;
            vce_ = 0;
            break;
        }
        case 0x27:   // VMRG
            for (unsigned i = 0; i < 8; ++i) {
                const std::uint16_t r = (vcc_ >> i) & 1 ? a[i] : b[i];
                set_acc_low(i, r);
                out[i] = r;
            }
            vco_ = 0;
            break;
        case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D:   // VAND VNAND VOR VNOR VXOR VNXOR
            for (unsigned i = 0; i < 8; ++i) {
                std::uint16_t r = 0;
                switch (fn) {
                    case 0x28: r = a[i] & b[i]; break;
                    case 0x29: r = static_cast<std::uint16_t>(~(a[i] & b[i])); break;
                    case 0x2A: r = a[i] | b[i]; break;
                    case 0x2B: r = static_cast<std::uint16_t>(~(a[i] | b[i])); break;
                    case 0x2C: r = a[i] ^ b[i]; break;
                    default: r = static_cast<std::uint16_t>(~(a[i] ^ b[i])); break;
                }
                set_acc_low(i, r);
                out[i] = r;
            }
            break;
        case 0x30: case 0x31: case 0x32: case 0x34: case 0x35: case 0x36: case 0x33: {   // VRCP VRCPL VRCPH VMOV VRSQ VRSQL VRSQH
            const unsigned de = vs & 7;
            const std::uint16_t input = v_[vt][e & 7];
            for (unsigned i = 0; i < 8; ++i) set_acc_low(i, b[i]);
            if (fn == 0x33) {   // VMOV
                out[de] = b[de];
            } else if (fn == 0x32 || fn == 0x36) {   // VRCPH VRSQH: the high half in, the last result's high half out
                div_in_ = s16(input);
                div_dp_ = true;
                out[de] = static_cast<std::uint16_t>(div_out_);
            } else {
                const bool low = fn == 0x31 || fn == 0x35;
                const bool sixteen = !(low && div_dp_);
                const std::int32_t in = sixteen ? static_cast<std::int32_t>(s16(input))
                                                : static_cast<std::int32_t>((static_cast<std::uint32_t>(static_cast<std::uint16_t>(div_in_)) << 16) | input);
                const std::int32_t result = (fn == 0x30 || fn == 0x31) ? reciprocal(in, sixteen) : inverse_root(in, sixteen);
                div_out_ = static_cast<std::int16_t>(static_cast<std::uint32_t>(result) >> 16);
                div_dp_ = false;
                out[de] = static_cast<std::uint16_t>(result);
            }
            break;
        }
        case 0x37: return;   // VNOP
        default: return;     // (the decoder reads no other function: nothing is changed)
    }
    v_[vd] = out;
}

void Rsp::vector_load(std::uint32_t w) {
    const unsigned base = (w >> 21) & 31, vt = (w >> 16) & 31, op = (w >> 11) & 31, e = (w >> 7) & 15;
    static constexpr std::array<unsigned, 12> kSizes = {1, 2, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16};
    if (op >= kSizes.size()) return;
    const std::int32_t offset = static_cast<std::int32_t>(w << 25) >> 25;
    std::uint32_t addr = r_[base] + static_cast<std::uint32_t>(offset * static_cast<std::int32_t>(kSizes[op]));
    switch (op) {
        case 0: case 1: case 2: case 3:   // LBV LSV LLV LDV
            for (unsigned i = 0; i < kSizes[op] && e + i < 16; ++i) set_byte(vt, e + i, dmem(addr + i));
            break;
        case 4: {   // LQV: to the end of the 16-byte line
            const unsigned n = 16 - (addr & 15);
            for (unsigned i = 0; i < n && e + i < 16; ++i) set_byte(vt, e + i, dmem(addr + i));
            break;
        }
        case 5: {   // LRV: the line's bytes before the address
            const unsigned start = 16 - ((addr & 15) - e);
            addr &= ~15u;
            for (unsigned b = start; b < 16; ++b) set_byte(vt, b & 15, dmem(addr++));
            break;
        }
        case 6: case 7: {   // LPV LUV: bytes into the lanes' high bits (LUV: one bit lower)
            const unsigned index = (addr & 7) - e;
            addr &= ~7u;
            for (unsigned i = 0; i < 8; ++i) {
                const std::uint16_t v = dmem(addr + ((index + i) & 15));
                v_[vt][i] = static_cast<std::uint16_t>(op == 6 ? v << 8 : v << 7);
            }
            break;
        }
        case 8: {   // LHV
            const unsigned index = (addr & 7) - e;
            addr &= ~7u;
            for (unsigned i = 0; i < 8; ++i) v_[vt][i] = static_cast<std::uint16_t>(dmem(addr + ((index + i * 2) & 15)) << 7);
            break;
        }
        case 9: {   // LFV
            const unsigned index = (addr & 7) - e;
            addr &= ~7u;
            Vector tmp{};
            for (unsigned i = 0; i < 4; ++i) {
                tmp[i] = static_cast<std::uint16_t>(dmem(addr + ((index + i * 4) & 15)) << 7);
                tmp[i + 4] = static_cast<std::uint16_t>(dmem(addr + ((index + i * 4 + 8) & 15)) << 7);
            }
            for (unsigned b = e; b < std::min(e + 8u, 16u); ++b) {
                const std::uint16_t lane = tmp[b >> 1];
                set_byte(vt, b, (b & 1) ? static_cast<std::uint8_t>(lane) : static_cast<std::uint8_t>(lane >> 8));
            }
            break;
        }
        case 10:   // LWV
            for (unsigned b = 16 - e; b < e + 16; ++b) {
                set_byte(vt, b & 15, dmem(addr));
                addr += 4;
            }
            break;
        case 11: {   // LTV: one lane into each of eight registers
            const std::uint32_t begin = addr & ~7u;
            addr = begin + ((e + (addr & 8)) & 15);
            const unsigned vbase = vt & ~7u;
            unsigned voff = e >> 1;
            for (unsigned i = 0; i < 8; ++i) {
                set_byte(vbase + voff, i * 2, dmem(addr++));
                if (addr == begin + 16) addr = begin;
                set_byte(vbase + voff, i * 2 + 1, dmem(addr++));
                if (addr == begin + 16) addr = begin;
                voff = (voff + 1) & 7;
            }
            break;
        }
        default: break;
    }
}

void Rsp::vector_store(std::uint32_t w) {
    const unsigned base = (w >> 21) & 31, vt = (w >> 16) & 31, op = (w >> 11) & 31, e = (w >> 7) & 15;
    static constexpr std::array<unsigned, 12> kSizes = {1, 2, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16};
    if (op >= kSizes.size()) return;
    const std::int32_t offset = static_cast<std::int32_t>(w << 25) >> 25;
    std::uint32_t addr = r_[base] + static_cast<std::uint32_t>(offset * static_cast<std::int32_t>(kSizes[op]));
    switch (op) {
        case 0: case 1: case 2: case 3:   // SBV SSV SLV SDV
            for (unsigned i = 0; i < kSizes[op]; ++i) set_dmem(addr + i, byte(vt, e + i));
            break;
        case 4: {   // SQV
            const unsigned n = 16 - (addr & 15);
            for (unsigned i = 0; i < n; ++i) set_dmem(addr + i, byte(vt, e + i));
            break;
        }
        case 5: {   // SRV
            const unsigned end = e + (addr & 15), b16 = 16 - (addr & 15);
            addr &= ~15u;
            for (unsigned b = e; b < end; ++b) set_dmem(addr++, byte(vt, b + b16));
            break;
        }
        case 6: case 7:   // SPV SUV
            for (unsigned b = e; b < e + 8; ++b) {
                const bool high = (b & 15) < 8;
                const bool packed = op == 6 ? high : !high;
                set_dmem(addr++, packed ? byte(vt, (b & 7) << 1) : static_cast<std::uint8_t>(v_[vt][b & 7] >> 7));
            }
            break;
        case 8: {   // SHV
            const unsigned index = addr & 7;
            addr &= ~7u;
            for (unsigned i = 0; i < 8; ++i) {
                const unsigned b = e + i * 2;
                const std::uint8_t value = static_cast<std::uint8_t>((byte(vt, b) << 1) | (byte(vt, b + 1) >> 7));
                set_dmem(addr + ((index + i * 2) & 15), value);
            }
            break;
        }
        case 9: {   // SFV: four lanes, one byte each, every fourth byte (the lanes as element 0 or 8 chooses them)
            const unsigned index = addr & 7;
            addr &= ~7u;
            const unsigned first = (e == 8) ? 4 : 0;
            for (unsigned i = 0; i < 4; ++i) set_dmem(addr + ((index + i * 4) & 15), static_cast<std::uint8_t>(v_[vt][first + i] >> 7));
            break;
        }
        case 10: {   // SWV
            unsigned b2 = addr & 7;
            addr &= ~7u;
            for (unsigned b = e; b < e + 16; ++b) set_dmem(addr + (b2++ & 15), byte(vt, b & 15));
            break;
        }
        case 11: {   // STV: one lane from each of eight registers
            const unsigned vbase = vt & ~7u;
            unsigned element = 16 - (e & ~1u);
            unsigned at = (addr & 7) - (e & ~1u);
            addr &= ~7u;
            for (unsigned r = vbase; r < vbase + 8; ++r) {
                set_dmem(addr + (at++ & 15), byte(r, element++ & 15));
                set_dmem(addr + (at++ & 15), byte(r, element++ & 15));
            }
            break;
        }
        default: break;
    }
}

}  // namespace cb::emulate::rsp
