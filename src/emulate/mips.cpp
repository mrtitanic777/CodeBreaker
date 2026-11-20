#include "emulate/mips.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "core/error.hpp"

namespace cb::emulate::mips {

namespace dm = decompile::mips;

namespace {

constexpr std::uint32_t kBiosBase = 0x1FC00000;
constexpr std::size_t kBiosSize = 0x80000;
constexpr std::uint32_t kRamWindow = 0x800000;   // the 2 MB is seen four times

std::uint32_t physical(std::uint32_t address) { return address >= 0xC0000000u ? address : address & 0x1FFFFFFFu; }

std::uint32_t mask_to(std::uint32_t v, unsigned size) { return size == 4 ? v : v & ((1u << (8 * size)) - 1); }

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const unsigned shift = 32 - bits;
    return static_cast<std::int32_t>(v << shift) >> shift;
}

// ---- the GTE's registers (the commands are not run) ------------------------------------------------------------------

std::uint32_t gte_read(const Cpu& cpu, unsigned n) {
    const auto& d = cpu.gte_data;
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

void gte_write(Cpu& cpu, unsigned n, std::uint32_t v) {
    auto& d = cpu.gte_data;
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

std::uint32_t gte_control_read(const Cpu& cpu, unsigned n) {
    const auto& c = cpu.gte_control;
    switch (n) {
        case 4: case 12: case 20: case 26: case 27: case 29: case 30: return static_cast<std::uint32_t>(sext(c[n], 16));   // the 16-bit control registers
        case 31: return (c[31] & 0x7FFFF000u) | ((c[31] & 0x7F87E000u) != 0 ? 0x80000000u : 0);                            // FLAG: bit 31 is the OR of the errors
        default: return c[n & 31];
    }
}

}  // namespace

std::string BusAccess::text() const { return std::format("{}{}", kind == Kind::Read ? 'r' : 'w', size); }

std::string BiosCall::text() const {
    return std::format("{:X}:{:X}({:X},{:X},{:X},{:X})", table, function, args[0], args[1], args[2], args[3]);
}

std::string Fault::text() const {
    switch (kind) {
        case Kind::NoInstruction: return std::format("no instruction at {:08X}", pc);
        case Kind::NotRun: return "not run: " + mnemonic;
        case Kind::AddressError: return std::format("address error at {:08X}", address);
        case Kind::BusError: return std::format("bus error at {:08X}", address);
        case Kind::Overflow: return "arithmetic overflow";
        case Kind::Break: return std::format("break {}", code);
        case Kind::Syscall: return "syscall";
    }
    return {};
}

// ---- the bus ---------------------------------------------------------------------------------------------------------

Bus::Bus() : ram_(kRamSize, 0), scratch_(kScratchSize, 0) {
    map(kDeviceLow, kDeviceHigh);
    map(0xFFFE0000u, 0xFFFE01FFu);   // the cache control register
}

Bus::Bus(ByteView ram) : Bus() {
    if (ram.size() > kRamSize) throw UsageError("a RAM image is at most 2 MB");
    std::copy(ram.begin(), ram.end(), ram_.begin());
}

void Bus::map(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write)});
}

void Bus::set_bios(ByteView image) {
    bios_.assign(image.begin(), image.begin() + static_cast<std::ptrdiff_t>(std::min(image.size(), kBiosSize)));
}

const Bus::Range* Bus::range_of(std::uint32_t phys) const {
    for (auto it = ranges_.rbegin(); it != ranges_.rend(); ++it)
        if (phys >= it->lo && phys <= it->hi) return &*it;
    return nullptr;
}

bool Bus::mapped(std::uint32_t address) const {
    const std::uint32_t p = physical(address);
    return p < kRamWindow || (p >= kScratchBase && p < kScratchBase + kScratchSize) || (p >= kBiosBase && p < kBiosBase + kBiosSize) ||
           range_of(p) != nullptr;
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    const std::uint32_t p = physical(address);
    if (const Range* range = range_of(p)) {
        const std::uint32_t v = range->read ? mask_to(range->read(p, size), size) : 0;
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), p, v});
        return v;
    }
    const std::uint8_t* from = nullptr;
    std::size_t room = 0;
    if (p < kRamWindow) {
        from = ram_.data() + (p & (kRamSize - 1));
        room = kRamSize - (p & (kRamSize - 1));
    } else if (p >= kScratchBase && p < kScratchBase + kScratchSize) {
        from = scratch_.data() + (p - kScratchBase);
        room = kScratchSize - (p - kScratchBase);
    } else if (p >= kBiosBase && p < kBiosBase + kBiosSize && p - kBiosBase < bios_.size()) {
        from = bios_.data() + (p - kBiosBase);
        room = bios_.size() - (p - kBiosBase);
    }
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size && i < room; ++i) v |= static_cast<std::uint32_t>(from[i]) << (8 * i);
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    const std::uint32_t p = physical(address);
    if (const Range* range = range_of(p)) {
        const std::uint32_t v = mask_to(value, size);
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), p, v});
        if (range->write) range->write(p, size, v);
        return;
    }
    std::uint8_t* to = nullptr;
    std::size_t room = 0;
    if (p < kRamWindow) {
        to = ram_.data() + (p & (kRamSize - 1));
        room = kRamSize - (p & (kRamSize - 1));
    } else if (p >= kScratchBase && p < kScratchBase + kScratchSize) {
        to = scratch_.data() + (p - kScratchBase);
        room = kScratchSize - (p - kScratchBase);
    }
    for (unsigned i = 0; i < size && i < room; ++i) to[i] = static_cast<std::uint8_t>(value >> (8 * i));
}

// ---- the CPU ---------------------------------------------------------------------------------------------------------

Cpu::Cpu(Bus& bus) : bus_(&bus) {}

void Cpu::jump_to(std::uint32_t address) {
    pc = address;
    next_pc = address + 4;
}

void Cpu::set(unsigned reg, std::uint32_t value) {
    if (reg == 0) return;
    r[reg] = value;
    if (pending_ && pending_->reg == reg) pending_.reset();   // the load in flight is lost: the instruction after the load wrote it
}

void Cpu::queue_load(unsigned reg, std::uint32_t value) {
    if (reg == 0) return;
    issued_ = Load{static_cast<std::uint8_t>(reg), value};
}

std::optional<Fault> Cpu::step() {
    if (pc % 4 != 0) return Fault{Fault::Kind::AddressError, pc, pc, {}, 0};
    if (!bus_->mapped(pc)) return Fault{Fault::Kind::BusError, pc, pc, {}, 0};
    const std::uint32_t word = bus_->read(pc, 4);
    const std::optional<dm::Insn> insn = dm::decode_word(word, pc);
    if (!insn) return Fault{Fault::Kind::NoInstruction, pc, 0, {}, 0};

    const std::uint32_t at = pc, was_next = next_pc;
    pc = next_pc;
    next_pc += 4;
    issued_.reset();
    const std::optional<Load> arriving = pending_;   // set() drops it when the instruction writes that register
    std::optional<Fault> fault = execute(*insn, at);
    if (fault) {
        if (fault->kind != Fault::Kind::Break && fault->kind != Fault::Kind::Syscall) {
            pc = at;
            next_pc = was_next;
        }
        pending_ = arriving;   // (the instruction did not finish)
        if (fault->kind == Fault::Kind::Break || fault->kind == Fault::Kind::Syscall) {
            if (pending_) {
                r[pending_->reg] = pending_->value;
                pending_.reset();
            }
            ++steps;
        }
        return fault;
    }
    if (pending_) {   // still there: the instruction did not write the register
        r[pending_->reg] = pending_->value;
    }
    pending_ = issued_;
    ++steps;
    return std::nullopt;
}

std::optional<Fault> Cpu::load(const dm::Insn& insn, std::uint32_t at, std::uint32_t address) {
    const std::string& m = insn.mnemonic;
    const unsigned rt = insn.rt;
    const bool word_access = m == "lw" || m == "lwc2";
    const bool half_access = m == "lh" || m == "lhu";
    if ((word_access && address % 4 != 0) || (half_access && address % 2 != 0)) return Fault{Fault::Kind::AddressError, at, address, {}, 0};
    if (!bus_->mapped(address)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
    if (cop0[12] & 0x10000u) return std::nullopt;   // the cache is isolated: nothing is read
    if (m == "lb") queue_load(rt, static_cast<std::uint32_t>(sext(bus_->read(address, 1), 8)));
    else if (m == "lbu") queue_load(rt, bus_->read(address, 1));
    else if (m == "lh") queue_load(rt, static_cast<std::uint32_t>(sext(bus_->read(address, 2), 16)));
    else if (m == "lhu") queue_load(rt, bus_->read(address, 2));
    else if (m == "lw") queue_load(rt, bus_->read(address, 4));
    else if (m == "lwc2") gte_write(*this, rt, bus_->read(address, 4));
    else {   // lwl, lwr: the aligned word, merged with the register (or the load in flight to it)
        const std::uint32_t mem = bus_->read(address & ~3u, 4);
        const std::uint32_t shift = address & 3;
        const std::uint32_t current = pending_ && pending_->reg == rt ? pending_->value : r[rt];
        std::uint32_t v;
        if (m == "lwl") {
            static constexpr std::uint32_t kKeep[4] = {0x00FFFFFF, 0x0000FFFF, 0x000000FF, 0};
            v = (current & kKeep[shift]) | (mem << (24 - 8 * shift));
        } else {
            static constexpr std::uint32_t kKeep[4] = {0, 0xFF000000, 0xFFFF0000, 0xFFFFFF00};
            v = (current & kKeep[shift]) | (mem >> (8 * shift));
        }
        queue_load(rt, v);
    }
    return std::nullopt;
}

std::optional<Fault> Cpu::store(const dm::Insn& insn, std::uint32_t at, std::uint32_t address) {
    const std::string& m = insn.mnemonic;
    const bool word_access = m == "sw" || m == "swc2";
    if ((word_access && address % 4 != 0) || (m == "sh" && address % 2 != 0)) return Fault{Fault::Kind::AddressError, at, address, {}, 0};
    if (!bus_->mapped(address)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
    if (cop0[12] & 0x10000u) return std::nullopt;   // the cache is isolated: the write goes nowhere
    const std::uint32_t v = r[insn.rt];
    if (m == "sb") bus_->write(address, 1, v);
    else if (m == "sh") bus_->write(address, 2, v);
    else if (m == "sw") bus_->write(address, 4, v);
    else if (m == "swc2") bus_->write(address, 4, gte_read(*this, insn.rt));
    else {   // swl, swr
        const std::uint32_t aligned = address & ~3u;
        const std::uint32_t mem = bus_->read(aligned, 4);
        const std::uint32_t shift = address & 3;
        std::uint32_t out;
        if (m == "swl") {
            static constexpr std::uint32_t kKeep[4] = {0xFFFFFF00, 0xFFFF0000, 0xFF000000, 0};
            out = (mem & kKeep[shift]) | (v >> (24 - 8 * shift));
        } else {
            static constexpr std::uint32_t kKeep[4] = {0, 0x000000FF, 0x0000FFFF, 0x00FFFFFF};
            out = (mem & kKeep[shift]) | (v << (8 * shift));
        }
        bus_->write(aligned, 4, out);
    }
    return std::nullopt;
}

// ---- the GTE's commands that need no division: NCLIP AVSZ3 AVSZ4 SQR OP GPF GPL MVMVA -----------------------------------------------

namespace {

constexpr std::uint32_t kFlagMac1Plus = 1u << 30, kFlagMac2Plus = 1u << 29, kFlagMac3Plus = 1u << 28;
constexpr std::uint32_t kFlagMac1Minus = 1u << 27, kFlagMac2Minus = 1u << 26, kFlagMac3Minus = 1u << 25;
constexpr std::uint32_t kFlagIr1 = 1u << 24, kFlagColour = 1u << 21, kFlagOtz = 1u << 18, kFlagMac0Plus = 1u << 16, kFlagMac0Minus = 1u << 15;

class GteCommand {
public:
    GteCommand(Cpu& cpu, std::uint32_t command)
        : d_(cpu.gte_data), c_(cpu.gte_control), shift_(((command >> 19) & 1) * 12), lm_(((command >> 10) & 1) != 0), command_(command) {
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

std::optional<Fault> Cpu::execute(const dm::Insn& insn, std::uint32_t at) {
    const std::uint32_t w = insn.word;
    const unsigned rs = insn.rs, rt = insn.rt, rd = insn.rd, sa = insn.shamt;
    const std::uint32_t a = r[rs], b = r[rt];
    const std::int32_t simm = sext(w, 16);
    const std::uint32_t uimm = w & 0xFFFF;
    const std::uint32_t branch_target = at + 4 + static_cast<std::uint32_t>(simm) * 4;
    const auto fault = [&](Fault::Kind kind) { return Fault{kind, at, 0, {}, 0}; };

    switch (w >> 26) {
        case 0x00:
            switch (w & 0x3F) {
                case 0x00: set(rd, b << sa); return std::nullopt;
                case 0x02: set(rd, b >> sa); return std::nullopt;
                case 0x03: set(rd, static_cast<std::uint32_t>(static_cast<std::int32_t>(b) >> sa)); return std::nullopt;
                case 0x04: set(rd, b << (a & 31)); return std::nullopt;
                case 0x06: set(rd, b >> (a & 31)); return std::nullopt;
                case 0x07: set(rd, static_cast<std::uint32_t>(static_cast<std::int32_t>(b) >> (a & 31))); return std::nullopt;
                case 0x08: next_pc = a; return std::nullopt;
                case 0x09: next_pc = a; set(rd, at + 8); return std::nullopt;
                case 0x0C: {
                    Fault f = fault(Fault::Kind::Syscall);
                    f.code = (w >> 6) & 0xFFFFF;
                    return f;
                }
                case 0x0D: {
                    Fault f = fault(Fault::Kind::Break);
                    f.code = (w >> 6) & 0xFFFFF;
                    return f;
                }
                case 0x10: set(rd, hi); return std::nullopt;
                case 0x11: hi = a; return std::nullopt;
                case 0x12: set(rd, lo); return std::nullopt;
                case 0x13: lo = a; return std::nullopt;
                case 0x18: {
                    const std::int64_t p = static_cast<std::int64_t>(static_cast<std::int32_t>(a)) * static_cast<std::int32_t>(b);
                    lo = static_cast<std::uint32_t>(p);
                    hi = static_cast<std::uint32_t>(static_cast<std::uint64_t>(p) >> 32);
                    return std::nullopt;
                }
                case 0x19: {
                    const std::uint64_t p = static_cast<std::uint64_t>(a) * b;
                    lo = static_cast<std::uint32_t>(p);
                    hi = static_cast<std::uint32_t>(p >> 32);
                    return std::nullopt;
                }
                case 0x1A: {
                    const std::int32_t n = static_cast<std::int32_t>(a), d = static_cast<std::int32_t>(b);
                    if (d == 0) {
                        lo = n >= 0 ? 0xFFFFFFFFu : 1u;
                        hi = a;
                    } else if (n == INT32_MIN && d == -1) {
                        lo = 0x80000000u;
                        hi = 0;
                    } else {
                        lo = static_cast<std::uint32_t>(n / d);
                        hi = static_cast<std::uint32_t>(n % d);
                    }
                    return std::nullopt;
                }
                case 0x1B:
                    if (b == 0) {
                        lo = 0xFFFFFFFFu;
                        hi = a;
                    } else {
                        lo = a / b;
                        hi = a % b;
                    }
                    return std::nullopt;
                case 0x20: {
                    const std::int64_t s = static_cast<std::int64_t>(static_cast<std::int32_t>(a)) + static_cast<std::int32_t>(b);
                    if (s != static_cast<std::int32_t>(s)) return fault(Fault::Kind::Overflow);
                    set(rd, static_cast<std::uint32_t>(s));
                    return std::nullopt;
                }
                case 0x21: set(rd, a + b); return std::nullopt;
                case 0x22: {
                    const std::int64_t s = static_cast<std::int64_t>(static_cast<std::int32_t>(a)) - static_cast<std::int32_t>(b);
                    if (s != static_cast<std::int32_t>(s)) return fault(Fault::Kind::Overflow);
                    set(rd, static_cast<std::uint32_t>(s));
                    return std::nullopt;
                }
                case 0x23: set(rd, a - b); return std::nullopt;
                case 0x24: set(rd, a & b); return std::nullopt;
                case 0x25: set(rd, a | b); return std::nullopt;
                case 0x26: set(rd, a ^ b); return std::nullopt;
                case 0x27: set(rd, ~(a | b)); return std::nullopt;
                case 0x2A: set(rd, static_cast<std::int32_t>(a) < static_cast<std::int32_t>(b) ? 1 : 0); return std::nullopt;
                case 0x2B: set(rd, a < b ? 1 : 0); return std::nullopt;
                default: return fault(Fault::Kind::NoInstruction);
            }
        case 0x01: {
            const bool negative = static_cast<std::int32_t>(a) < 0;
            const unsigned kind = rt;
            const bool taken = (kind == 0 || kind == 16) ? negative : !negative;   // bltz bltzal : bgez bgezal
            if (kind >= 16) set(31, at + 8);
            if (taken) next_pc = branch_target;
            return std::nullopt;
        }
        case 0x02: next_pc = ((at + 4) & 0xF0000000u) | ((w & 0x3FFFFFF) << 2); return std::nullopt;
        case 0x03:
            set(31, at + 8);
            next_pc = ((at + 4) & 0xF0000000u) | ((w & 0x3FFFFFF) << 2);
            return std::nullopt;
        case 0x04: if (a == b) next_pc = branch_target; return std::nullopt;
        case 0x05: if (a != b) next_pc = branch_target; return std::nullopt;
        case 0x06: if (static_cast<std::int32_t>(a) <= 0) next_pc = branch_target; return std::nullopt;
        case 0x07: if (static_cast<std::int32_t>(a) > 0) next_pc = branch_target; return std::nullopt;
        case 0x08: {
            const std::int64_t s = static_cast<std::int64_t>(static_cast<std::int32_t>(a)) + simm;
            if (s != static_cast<std::int32_t>(s)) return fault(Fault::Kind::Overflow);
            set(rt, static_cast<std::uint32_t>(s));
            return std::nullopt;
        }
        case 0x09: set(rt, a + static_cast<std::uint32_t>(simm)); return std::nullopt;
        case 0x0A: set(rt, static_cast<std::int32_t>(a) < simm ? 1 : 0); return std::nullopt;
        case 0x0B: set(rt, a < static_cast<std::uint32_t>(simm) ? 1 : 0); return std::nullopt;
        case 0x0C: set(rt, a & uimm); return std::nullopt;
        case 0x0D: set(rt, a | uimm); return std::nullopt;
        case 0x0E: set(rt, a ^ uimm); return std::nullopt;
        case 0x0F: set(rt, uimm << 16); return std::nullopt;
        case 0x10:
            if (insn.mnemonic == "mfc0") queue_load(rt, cop0[rd]);
            else if (insn.mnemonic == "mtc0") cop0[rd] = b;
            else cop0[12] = (cop0[12] & ~0xFu) | ((cop0[12] >> 2) & 0xFu);   // rfe: the mode bits move back
            return std::nullopt;
        case 0x12:
            if (insn.mnemonic == "mfc2") queue_load(rt, gte_read(*this, rd));
            else if (insn.mnemonic == "cfc2") queue_load(rt, gte_control_read(*this, rd));
            else if (insn.mnemonic == "mtc2") gte_write(*this, rd, b);
            else if (insn.mnemonic == "ctc2") gte_control[rd] = b;
            else if (insn.form != dm::Form::Command || !GteCommand(*this, static_cast<std::uint32_t>(insn.imm)).run(insn.mnemonic))
                return Fault{Fault::Kind::NotRun, at, 0, insn.mnemonic, 0};   // a GTE command this does not run (RTPS RTPT and the colour commands), or a branch on the coprocessor's condition
            return std::nullopt;
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x32:
            return load(insn, at, a + static_cast<std::uint32_t>(simm));
        case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2E: case 0x3A:
            return store(insn, at, a + static_cast<std::uint32_t>(simm));
        default: return fault(Fault::Kind::NoInstruction);
    }
}

// ---- running a function ------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("stopped"));
        case End::Limit: return "limit";
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    for (unsigned i = 0; i < 4; ++i) cpu.r[4 + i] = options.a[i];
    for (unsigned i = 0; i < 8; ++i) cpu.r[16 + i] = options.s[i];
    for (unsigned i = 0; i < 8; ++i) cpu.r[8 + i] = options.t[i];
    cpu.r[28] = options.gp;
    cpu.r[29] = options.sp;
    cpu.r[31] = kReturnAddress;
    cpu.jump_to(entry);
    const std::size_t log_from = bus.log().size();

    RunResult out;
    while (cpu.steps < options.max_steps) {
        if (cpu.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        const std::uint32_t p = cpu.pc & 0x1FFFFFFFu;
        if (p == 0xA0 || p == 0xB0 || p == 0xC0) {   // a BIOS call: logged, answered with 0, and returned from
            BiosCall call;
            call.table = static_cast<std::uint8_t>(p);
            call.function = cpu.r[9];
            for (unsigned i = 0; i < 4; ++i) call.args[i] = cpu.r[4 + i];
            out.bios.push_back(call);
            cpu.r[2] = 0;
            cpu.jump_to(cpu.r[31]);
            ++cpu.steps;
            continue;
        }
        const std::optional<Fault> fault = cpu.step();
        if (!fault) continue;
        if (fault->kind == Fault::Kind::Syscall && (cpu.r[4] == 1 || cpu.r[4] == 2)) {   // EnterCriticalSection, ExitCriticalSection
            BiosCall call;
            call.table = 0;
            call.function = cpu.r[4];
            out.bios.push_back(call);
            cpu.r[2] = cpu.r[4] == 1 ? 1 : 0;
            continue;
        }
        out.end = End::Stopped;
        out.fault = fault;
        break;
    }
    out.r = cpu.r;
    out.gte_data = cpu.gte_data;
    out.gte_control = cpu.gte_control;
    out.hi = cpu.hi;
    out.lo = cpu.lo;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_from), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    const std::pair<const char*, unsigned> names[] = {{"V0", 2},  {"V1", 3},  {"S0", 16}, {"S1", 17}, {"S2", 18}, {"S3", 19}, {"S4", 20},
                                                      {"S5", 21}, {"S6", 22}, {"S7", 23}, {"GP", 28}, {"SP", 29}, {"FP", 30}, {"RA", 31}};
    for (const auto& [name, number] : names) run.results[name] = static_cast<std::uint64_t>(result.r[number]);
    run.results["HI"] = static_cast<std::uint64_t>(result.hi);
    run.results["LO"] = static_cast<std::uint64_t>(result.lo);
    for (const BusAccess& access : result.io) {
        cb::emulate::IoAccess io;
        io.kind = access.text();
        io.port = access.address;
        io.value = access.value;
        io.size = access.size;
        run.io.push_back(std::move(io));
    }
    for (const BiosCall& call : result.bios) {
        cb::emulate::Call c;
        c.name = std::format("{:X}:{:X}", call.table, call.function);
        c.args = std::vector<std::int64_t>(call.args.begin(), call.args.end());
        run.calls.push_back(std::move(c));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Little;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options) { return record(run(bus, entry, options)); }

}  // namespace cb::emulate::mips
