// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/emulate/r4300.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <format>
#include <limits>

#include "cb/core/error.hpp"
#include "cb/decompile/mips.hpp"

namespace cb::emulate::r4300 {

namespace dm = decompile::mips;

namespace {

constexpr std::size_t kSpSize = 0x2000;
constexpr std::size_t kPifRamSize = 0x40;
constexpr std::uint32_t kSpStatus = 0x04040010;
constexpr std::uint32_t kViCurrent = 0x04400010;
constexpr std::uint32_t kPiDramAddr = 0x04600000;
constexpr std::uint32_t kPiWrLen = 0x0460000C;
constexpr unsigned kViHalfLines = 525;

std::uint64_t sx32(std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); }
std::int64_t sx16(std::uint32_t v) { return static_cast<std::int16_t>(v & 0xFFFF); }
std::uint32_t lo32(std::uint64_t v) { return static_cast<std::uint32_t>(v); }

std::uint64_t mask_to(std::uint64_t v, unsigned size) { return size == 8 ? v : v & ((std::uint64_t{1} << (8 * size)) - 1); }

std::uint64_t read_big(const std::uint8_t* from, unsigned size) {
    std::uint64_t v = 0;
    for (unsigned i = 0; i < size; ++i) v = v << 8 | from[i];
    return v;
}
void write_big(std::uint8_t* to, unsigned size, std::uint64_t value) {
    for (unsigned i = 0; i < size; ++i) to[i] = static_cast<std::uint8_t>(value >> (8 * (size - 1 - i)));
}

// The high 64 bits of an unsigned 64 x 64 product.
std::uint64_t mulhi_unsigned(std::uint64_t a, std::uint64_t b) {
    const std::uint64_t a_lo = a & 0xFFFFFFFF, a_hi = a >> 32, b_lo = b & 0xFFFFFFFF, b_hi = b >> 32;
    const std::uint64_t cross1 = a_hi * b_lo + ((a_lo * b_lo) >> 32);
    const std::uint64_t cross2 = a_lo * b_hi + (cross1 & 0xFFFFFFFF);
    return a_hi * b_hi + (cross1 >> 32) + (cross2 >> 32);
}

const Block* block_of(std::uint32_t physical) {
    for (const Block& b : kBlocks)
        if (physical >= b.lo && physical <= b.hi) return &b;
    return nullptr;
}

}  // namespace

std::string BusAccess::text() const { return std::format("{}{}", kind == Kind::Read ? 'r' : 'w', size); }

std::string Fault::text() const {
    switch (kind) {
        case Kind::NoInstruction: return std::format("no instruction at {:08X}", pc);
        case Kind::NotRun: return "not run: " + mnemonic;
        case Kind::AddressError: return std::format("address error at {:08X}", address);
        case Kind::BusError: return std::format("bus error at {:08X}", address);
        case Kind::TlbMiss: return std::format("tlb miss at {:08X}", address);
        case Kind::Overflow: return "arithmetic overflow";
        case Kind::Trap: return "trap";
        case Kind::Break: return std::format("break {}", code);
        case Kind::Syscall: return "syscall";
    }
    return {};
}

// ---- the bus ---------------------------------------------------------------------------------------------------------

Bus::Bus() : rdram_(kRdramSize, 0), sp_(kSpSize, 0), pif_(kPifRamSize, 0) {}

Bus::Bus(ByteView rdram, ByteView rom) : Bus() {
    if (rdram.size() > kRdramSize) throw UsageError("an RDRAM image is at most 8 MB");
    std::copy(rdram.begin(), rdram.end(), rdram_.begin());
    rom_.assign(rom.begin(), rom.end());
}

void Bus::map(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write)}); }

const Bus::Range* Bus::range_of(std::uint32_t physical) const {
    for (auto it = ranges_.rbegin(); it != ranges_.rend(); ++it)
        if (physical >= it->lo && physical <= it->hi) return &*it;
    return nullptr;
}

bool Bus::mapped(std::uint32_t p) const {
    return p < kRdramSize || (p >= kDmem && p < kDmem + kSpSize) || (p >= kRomBase && p <= kRomEnd) || (p >= kPifRam && p < kPifRam + kPifRamSize) ||
           block_of(p) != nullptr || range_of(p) != nullptr;
}

std::uint64_t Bus::read(std::uint32_t p, unsigned size) {
    if (const Range* range = range_of(p)) {
        const std::uint64_t v = range->read ? mask_to(range->read(p, size), size) : 0;
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), p, v});
        return v;
    }
    const std::uint8_t* from = nullptr;
    std::size_t room = 0;
    if (p < kRdramSize) {
        from = rdram_.data() + p;
        room = kRdramSize - p;
    } else if (p >= kDmem && p < kDmem + kSpSize) {
        from = sp_.data() + (p - kDmem);
        room = kSpSize - (p - kDmem);
    } else if (p >= kRomBase && p <= kRomEnd) {
        if (p - kRomBase < rom_.size()) {
            from = rom_.data() + (p - kRomBase);
            room = rom_.size() - (p - kRomBase);
        }
    } else if (p >= kPifRam && p < kPifRam + kPifRamSize) {
        from = pif_.data() + (p - kPifRam);
        room = kPifRamSize - (p - kPifRam);
    } else if (block_of(p)) {
        const std::uint64_t v = register_read(p, size);
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), p, v});
        return v;
    }
    std::uint64_t v = 0;
    for (unsigned i = 0; i < size; ++i) v = v << 8 | (i < room ? from[i] : 0);
    return v;
}

void Bus::write(std::uint32_t p, unsigned size, std::uint64_t value) {
    if (const Range* range = range_of(p)) {
        const std::uint64_t v = mask_to(value, size);
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), p, v});
        if (range->write) range->write(p, size, v);
        return;
    }
    std::uint8_t* to = nullptr;
    std::size_t room = 0;
    if (p < kRdramSize) {
        to = rdram_.data() + p;
        room = kRdramSize - p;
    } else if (p >= kDmem && p < kDmem + kSpSize) {
        to = sp_.data() + (p - kDmem);
        room = kSpSize - (p - kDmem);
    } else if (p >= kPifRam && p < kPifRam + kPifRamSize) {
        to = pif_.data() + (p - kPifRam);
        room = kPifRamSize - (p - kPifRam);
    } else if (block_of(p)) {
        const std::uint64_t v = mask_to(value, size);
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), p, v});
        register_write(p, size, v);
        return;
    }
    if (!to) return;   // (the cartridge is read-only)
    std::uint8_t scratch[8];
    write_big(scratch, size, value);
    for (unsigned i = 0; i < size && i < room; ++i) to[i] = scratch[i];
}

std::uint32_t Bus::latched_word(std::uint32_t aligned) const {
    std::uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i) {
        const auto it = latched_.find(aligned + i);
        v = v << 8 | (it != latched_.end() ? it->second : 0);
    }
    return v;
}

// A register's word: what a program polls for has a value of its own, the rest is what was written.
std::uint64_t Bus::register_read(std::uint32_t p, unsigned size) {
    const auto word = [&](std::uint32_t aligned) -> std::uint32_t {
        if (aligned == kSpStatus) return 1;   // halted
        if (aligned == kViCurrent) return scanline_++ % kViHalfLines;
        return latched_word(aligned);
    };
    std::uint8_t bytes[16] = {};
    const std::uint32_t first = p & ~3u;
    const std::uint32_t last = (p + size - 1) & ~3u;
    for (std::uint32_t a = first, k = 0; a <= last; a += 4, ++k) write_big(bytes + 4 * k, 4, word(a));
    return read_big(bytes + (p - first), size);
}

void Bus::register_write(std::uint32_t p, unsigned size, std::uint64_t value) {
    std::uint8_t bytes[8];
    write_big(bytes, size, value);
    for (unsigned i = 0; i < size; ++i) latched_[p + i] = bytes[i];
    if (p <= kPiWrLen && p + size > kPiWrLen && p >= kPiDramAddr) pi_dma(latched_word(kPiWrLen));   // the PI's cartridge-to-RDRAM DMA
}

void Bus::pi_dma(std::uint32_t length_field) {
    const std::uint32_t dram = latched_word(kPiDramAddr) & 0x00FFFFFF;
    const std::uint32_t cart = latched_word(kPiDramAddr + 4) & 0x1FFFFFFF;
    const std::uint32_t length = (length_field & 0x00FFFFFF) + 1;
    if (cart < kRomBase) return;   // (only the cartridge's ROM is the source of a DMA here)
    const std::uint32_t from = cart - kRomBase;
    for (std::uint32_t i = 0; i < length; ++i) {
        if (dram + i >= kRdramSize) break;
        rdram_[dram + i] = from + i < rom_.size() ? rom_[from + i] : 0;
    }
}

// ---- the CPU ---------------------------------------------------------------------------------------------------------

Cpu::Cpu(Bus& bus) : bus_(&bus) {}

void Cpu::jump_to(std::uint32_t address) {
    pc = address;
    next_pc = address + 4;
}

std::uint32_t Cpu::fpu_read32(unsigned n) const {
    if (fr() || (n & 1) == 0) return lo32(fpr[n]);
    return static_cast<std::uint32_t>(fpr[n & ~1u] >> 32);
}
void Cpu::fpu_write32(unsigned n, std::uint32_t v) {
    if (fr() || (n & 1) == 0) fpr[n] = (fpr[n] & ~std::uint64_t{0xFFFFFFFF}) | v;
    else fpr[n & ~1u] = (fpr[n & ~1u] & 0xFFFFFFFF) | static_cast<std::uint64_t>(v) << 32;
}
std::uint64_t Cpu::fpu_read64(unsigned n) const { return fr() ? fpr[n] : fpr[n & ~1u]; }
void Cpu::fpu_write64(unsigned n, std::uint64_t v) { (fr() ? fpr[n] : fpr[n & ~1u]) = v; }

std::optional<Fault> Cpu::translate(std::uint32_t va, bool write, std::uint32_t at, std::uint32_t& physical) const {
    if (va >= 0x80000000u && va < 0xC0000000u) {   // KSEG0 and KSEG1: the first 512 MB, whatever the cache does
        physical = va & 0x1FFFFFFFu;
        return std::nullopt;
    }
    const std::uint32_t asid = static_cast<std::uint32_t>(cop0[10]) & 0xFF;
    for (const TlbEntry& e : tlb) {
        const std::uint32_t span = e.mask | 0x1FFF;              // the page pair's size minus one
        if (((va ^ static_cast<std::uint32_t>(e.hi)) & ~span) != 0) continue;
        const bool global = (e.lo0 & e.lo1 & 1) != 0;
        if (!global && (static_cast<std::uint32_t>(e.hi) & 0xFF) != asid) continue;
        const std::uint64_t entry_lo = (va & ((span >> 1) + 1)) != 0 ? e.lo1 : e.lo0;
        if ((entry_lo & 2) == 0 || (write && (entry_lo & 4) == 0)) break;   // invalid, or not dirty
        const std::uint32_t page = span >> 1;                    // the page's size minus one
        physical = (static_cast<std::uint32_t>((entry_lo >> 6) & 0xFFFFF) << 12 & ~page) | (va & page);
        return std::nullopt;
    }
    return Fault{Fault::Kind::TlbMiss, at, va, {}, 0};
}

std::optional<Fault> Cpu::load(std::uint32_t address, unsigned size, std::uint32_t at, std::uint64_t& value) {
    if (address % size != 0) return Fault{Fault::Kind::AddressError, at, address, {}, 0};
    std::uint32_t p = 0;
    if (const auto f = translate(address, false, at, p)) return f;
    if (!bus_->mapped(p)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
    value = bus_->read(p, size);
    return std::nullopt;
}

std::optional<Fault> Cpu::store(std::uint32_t address, unsigned size, std::uint32_t at, std::uint64_t value) {
    if (address % size != 0) return Fault{Fault::Kind::AddressError, at, address, {}, 0};
    std::uint32_t p = 0;
    if (const auto f = translate(address, true, at, p)) return f;
    if (!bus_->mapped(p)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
    bus_->write(p, size, value);
    return std::nullopt;
}

std::optional<Fault> Cpu::step() {
    if (pc % 4 != 0) return Fault{Fault::Kind::AddressError, pc, pc, {}, 0};
    std::uint32_t p = 0;
    if (const auto f = translate(pc, false, pc, p)) return f;
    if (!bus_->mapped(p)) return Fault{Fault::Kind::BusError, pc, pc, {}, 0};
    const std::uint32_t word = static_cast<std::uint32_t>(bus_->read(p, 4));
    if (!dm::decode_word(word, pc, dm::Variant::R4300)) return Fault{Fault::Kind::NoInstruction, pc, 0, {}, 0};

    const std::uint32_t at = pc, was_next = next_pc;
    pc = next_pc;
    next_pc += 4;
    const std::optional<Fault> fault = execute(word, at);
    if (fault) {
        if (fault->kind != Fault::Kind::Break && fault->kind != Fault::Kind::Syscall) {
            pc = at;
            next_pc = was_next;
        } else {
            ++steps;
        }
        return fault;
    }
    r[0] = 0;
    cop0[9] = (cop0[9] + 1) & 0xFFFFFFFF;   // Count
    ++steps;
    return std::nullopt;
}

void Cpu::branch(bool taken, std::uint32_t target, bool likely) {
    if (taken) {
        next_pc = target;
    } else if (likely) {   // the delay slot is skipped
        pc = next_pc;
        next_pc = pc + 4;
    }
}

std::optional<Fault> Cpu::execute(std::uint32_t word, std::uint32_t at) {
    const unsigned opcode = word >> 26, rs = (word >> 21) & 31, rt = (word >> 16) & 31;
    const std::int64_t imm = sx16(word);
    const std::uint64_t uimm = word & 0xFFFF;
    const std::uint32_t target = at + 4 + static_cast<std::uint32_t>(imm) * 4;
    const std::uint64_t a = r[rs], b = r[rt];
    const auto overflow = [&] { return Fault{Fault::Kind::Overflow, at, 0, {}, 0}; };
    switch (opcode) {
        case 0: return special(word, at);
        case 1: return regimm(word, at);
        case 2: next_pc = ((at + 4) & 0xF0000000u) | ((word & 0x3FFFFFF) << 2); return std::nullopt;
        case 3:
            set(31, sx32(at + 8));
            next_pc = ((at + 4) & 0xF0000000u) | ((word & 0x3FFFFFF) << 2);
            return std::nullopt;
        case 4: case 20: branch(a == b, target, opcode == 20); return std::nullopt;
        case 5: case 21: branch(a != b, target, opcode == 21); return std::nullopt;
        case 6: case 22: branch(static_cast<std::int64_t>(a) <= 0, target, opcode == 22); return std::nullopt;
        case 7: case 23: branch(static_cast<std::int64_t>(a) > 0, target, opcode == 23); return std::nullopt;
        case 8: {
            const std::int64_t sum = static_cast<std::int64_t>(static_cast<std::int32_t>(lo32(a))) + imm;
            if (sum != static_cast<std::int32_t>(sum)) return overflow();
            set(rt, static_cast<std::uint64_t>(sum));
            return std::nullopt;
        }
        case 9: set(rt, sx32(lo32(a) + static_cast<std::uint32_t>(imm))); return std::nullopt;
        case 10: set(rt, static_cast<std::int64_t>(a) < imm ? 1 : 0); return std::nullopt;
        case 11: set(rt, a < static_cast<std::uint64_t>(imm) ? 1 : 0); return std::nullopt;
        case 12: set(rt, a & uimm); return std::nullopt;
        case 13: set(rt, a | uimm); return std::nullopt;
        case 14: set(rt, a ^ uimm); return std::nullopt;
        case 15: set(rt, sx32(static_cast<std::uint32_t>(uimm) << 16)); return std::nullopt;
        case 16: return system_control(word, at);
        case 17: return fpu(word, at);
        case 24: {
            const std::uint64_t sum = a + static_cast<std::uint64_t>(imm);
            if ((((a ^ sum) & (static_cast<std::uint64_t>(imm) ^ sum)) >> 63) != 0) return overflow();
            set(rt, sum);
            return std::nullopt;
        }
        case 25: set(rt, a + static_cast<std::uint64_t>(imm)); return std::nullopt;
        default: return memory(word, at);
    }
}

std::optional<Fault> Cpu::special(std::uint32_t word, std::uint32_t at) {
    const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, rd = (word >> 11) & 31, sa = (word >> 6) & 31, function = word & 63;
    const std::uint64_t a = r[rs], b = r[rt];
    const std::uint32_t a32 = lo32(a), b32 = lo32(b);
    const auto overflow = [&] { return Fault{Fault::Kind::Overflow, at, 0, {}, 0}; };
    const auto trap = [&](bool taken) -> std::optional<Fault> {
        if (taken) return Fault{Fault::Kind::Trap, at, 0, {}, 0};
        return std::nullopt;
    };
    const auto s64 = [](std::uint64_t v) { return static_cast<std::int64_t>(v); };
    switch (function) {
        case 0: set(rd, sx32(b32 << sa)); break;
        case 2: set(rd, sx32(b32 >> sa)); break;
        case 3: set(rd, sx32(static_cast<std::uint32_t>(static_cast<std::int32_t>(b32) >> sa))); break;
        case 4: set(rd, sx32(b32 << (a32 & 31))); break;
        case 6: set(rd, sx32(b32 >> (a32 & 31))); break;
        case 7: set(rd, sx32(static_cast<std::uint32_t>(static_cast<std::int32_t>(b32) >> (a32 & 31)))); break;
        case 8: next_pc = a32; break;
        case 9:
            set(rd, sx32(at + 8));
            next_pc = a32;
            break;
        case 12: return Fault{Fault::Kind::Syscall, at, 0, {}, (word >> 6) & 0xFFFFF};
        case 13: return Fault{Fault::Kind::Break, at, 0, {}, (word >> 6) & 0xFFFFF};
        case 15: break;   // SYNC
        case 16: set(rd, hi); break;
        case 17: hi = a; break;
        case 18: set(rd, lo); break;
        case 19: lo = a; break;
        case 20: set(rd, b << (a & 63)); break;
        case 22: set(rd, b >> (a & 63)); break;
        case 23: set(rd, static_cast<std::uint64_t>(s64(b) >> (a & 63))); break;
        case 24: {
            const std::int64_t p = static_cast<std::int64_t>(static_cast<std::int32_t>(a32)) * static_cast<std::int32_t>(b32);
            lo = sx32(static_cast<std::uint32_t>(p));
            hi = sx32(static_cast<std::uint32_t>(static_cast<std::uint64_t>(p) >> 32));
            break;
        }
        case 25: {
            const std::uint64_t p = static_cast<std::uint64_t>(a32) * b32;
            lo = sx32(static_cast<std::uint32_t>(p));
            hi = sx32(static_cast<std::uint32_t>(p >> 32));
            break;
        }
        case 26: {
            const std::int32_t n = static_cast<std::int32_t>(a32), d = static_cast<std::int32_t>(b32);
            if (d == 0) {
                lo = n < 0 ? 1 : sx32(0xFFFFFFFF);
                hi = sx32(a32);
            } else if (n == std::numeric_limits<std::int32_t>::min() && d == -1) {
                lo = sx32(a32);
                hi = 0;
            } else {
                lo = sx32(static_cast<std::uint32_t>(n / d));
                hi = sx32(static_cast<std::uint32_t>(n % d));
            }
            break;
        }
        case 27:
            if (b32 == 0) {
                lo = sx32(0xFFFFFFFF);
                hi = sx32(a32);
            } else {
                lo = sx32(a32 / b32);
                hi = sx32(a32 % b32);
            }
            break;
        case 28: {
            lo = a * b;
            hi = mulhi_unsigned(a, b) - (s64(a) < 0 ? b : 0) - (s64(b) < 0 ? a : 0);
            break;
        }
        case 29:
            lo = a * b;
            hi = mulhi_unsigned(a, b);
            break;
        case 30:
            if (b == 0) {
                lo = s64(a) < 0 ? 1 : ~std::uint64_t{0};
                hi = a;
            } else if (s64(a) == std::numeric_limits<std::int64_t>::min() && s64(b) == -1) {
                lo = a;
                hi = 0;
            } else {
                lo = static_cast<std::uint64_t>(s64(a) / s64(b));
                hi = static_cast<std::uint64_t>(s64(a) % s64(b));
            }
            break;
        case 31:
            if (b == 0) {
                lo = ~std::uint64_t{0};
                hi = a;
            } else {
                lo = a / b;
                hi = a % b;
            }
            break;
        case 32: {
            const std::int64_t sum = static_cast<std::int64_t>(static_cast<std::int32_t>(a32)) + static_cast<std::int32_t>(b32);
            if (sum != static_cast<std::int32_t>(sum)) return overflow();
            set(rd, static_cast<std::uint64_t>(sum));
            break;
        }
        case 33: set(rd, sx32(a32 + b32)); break;
        case 34: {
            const std::int64_t diff = static_cast<std::int64_t>(static_cast<std::int32_t>(a32)) - static_cast<std::int32_t>(b32);
            if (diff != static_cast<std::int32_t>(diff)) return overflow();
            set(rd, static_cast<std::uint64_t>(diff));
            break;
        }
        case 35: set(rd, sx32(a32 - b32)); break;
        case 36: set(rd, a & b); break;
        case 37: set(rd, a | b); break;
        case 38: set(rd, a ^ b); break;
        case 39: set(rd, ~(a | b)); break;
        case 42: set(rd, s64(a) < s64(b) ? 1 : 0); break;
        case 43: set(rd, a < b ? 1 : 0); break;
        case 44: {
            const std::uint64_t sum = a + b;
            if ((((a ^ sum) & (b ^ sum)) >> 63) != 0) return overflow();
            set(rd, sum);
            break;
        }
        case 45: set(rd, a + b); break;
        case 46: {
            const std::uint64_t diff = a - b;
            if ((((a ^ b) & (a ^ diff)) >> 63) != 0) return overflow();
            set(rd, diff);
            break;
        }
        case 47: set(rd, a - b); break;
        case 48: return trap(s64(a) >= s64(b));
        case 49: return trap(a >= b);
        case 50: return trap(s64(a) < s64(b));
        case 51: return trap(a < b);
        case 52: return trap(a == b);
        case 54: return trap(a != b);
        case 56: set(rd, b << sa); break;
        case 58: set(rd, b >> sa); break;
        case 59: set(rd, static_cast<std::uint64_t>(s64(b) >> sa)); break;
        case 60: set(rd, b << (sa + 32)); break;
        case 62: set(rd, b >> (sa + 32)); break;
        case 63: set(rd, static_cast<std::uint64_t>(s64(b) >> (sa + 32))); break;
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
    return std::nullopt;
}

std::optional<Fault> Cpu::regimm(std::uint32_t word, std::uint32_t at) {
    const unsigned rs = (word >> 21) & 31, kind = (word >> 16) & 31;
    const std::int64_t imm = sx16(word);
    const std::uint32_t target = at + 4 + static_cast<std::uint32_t>(imm) * 4;
    const std::int64_t a = static_cast<std::int64_t>(r[rs]);
    const auto trap = [&](bool taken) -> std::optional<Fault> {
        if (taken) return Fault{Fault::Kind::Trap, at, 0, {}, 0};
        return std::nullopt;
    };
    switch (kind) {
        case 0: case 2: branch(a < 0, target, kind == 2); break;
        case 1: case 3: branch(a >= 0, target, kind == 3); break;
        case 8: return trap(a >= imm);
        case 9: return trap(r[rs] >= static_cast<std::uint64_t>(imm));
        case 10: return trap(a < imm);
        case 11: return trap(r[rs] < static_cast<std::uint64_t>(imm));
        case 12: return trap(a == imm);
        case 14: return trap(a != imm);
        case 16: case 18: set(31, sx32(at + 8)); branch(a < 0, target, kind == 18); break;
        case 17: case 19: set(31, sx32(at + 8)); branch(a >= 0, target, kind == 19); break;
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
    return std::nullopt;
}

std::optional<Fault> Cpu::memory(std::uint32_t word, std::uint32_t at) {
    const unsigned opcode = word >> 26, rs = (word >> 21) & 31, rt = (word >> 16) & 31;
    const std::uint32_t address = lo32(r[rs] + static_cast<std::uint64_t>(sx16(word)));
    std::uint64_t v = 0;
    std::optional<Fault> fault;
    const auto unaligned_word = [&](std::uint32_t& aligned_value) -> std::optional<Fault> {   // the aligned word holding the address
        if (const std::optional<Fault> f = load(address & ~3u, 4, at, v)) return f;
        aligned_value = lo32(v);
        return std::nullopt;
    };
    const auto unaligned_dword = [&]() { return load(address & ~7u, 8, at, v); };
    switch (opcode) {
        case 0x20: if ((fault = load(address, 1, at, v))) return fault; set(rt, static_cast<std::uint64_t>(static_cast<std::int8_t>(v))); break;
        case 0x21: if ((fault = load(address, 2, at, v))) return fault; set(rt, static_cast<std::uint64_t>(static_cast<std::int16_t>(v))); break;
        case 0x23: if ((fault = load(address, 4, at, v))) return fault; set(rt, sx32(lo32(v))); break;
        case 0x24: if ((fault = load(address, 1, at, v))) return fault; set(rt, v); break;
        case 0x25: if ((fault = load(address, 2, at, v))) return fault; set(rt, v); break;
        case 0x27: if ((fault = load(address, 4, at, v))) return fault; set(rt, v); break;
        case 0x37: if ((fault = load(address, 8, at, v))) return fault; set(rt, v); break;
        case 0x22: {   // lwl: the bytes from the address to the end of its word go to the high bytes of the register
            std::uint32_t mem = 0;
            if ((fault = unaligned_word(mem))) return fault;
            const unsigned k = address & 3;
            set(rt, sx32((mem << (8 * k)) | (lo32(r[rt]) & ((std::uint32_t{1} << (8 * k)) - 1))));
            break;
        }
        case 0x26: {   // lwr: the bytes from the start of the word to the address go to the low bytes
            std::uint32_t mem = 0;
            if ((fault = unaligned_word(mem))) return fault;
            const unsigned k = address & 3;
            const std::uint32_t kept = k == 3 ? 0 : ~(0xFFFFFFFFu >> (8 * (3 - k)));
            const std::uint32_t merged = (mem >> (8 * (3 - k))) | (lo32(r[rt]) & kept);
            set(rt, k == 3 ? sx32(merged) : (r[rt] & ~std::uint64_t{0xFFFFFFFF}) | merged);
            break;
        }
        case 0x1A: {
            if ((fault = unaligned_dword())) return fault;
            const unsigned k = address & 7;
            set(rt, (v << (8 * k)) | (r[rt] & ((std::uint64_t{1} << (8 * k)) - 1)));
            break;
        }
        case 0x1B: {
            if ((fault = unaligned_dword())) return fault;
            const unsigned k = address & 7;
            set(rt, (v >> (8 * (7 - k))) | (r[rt] & ~(~std::uint64_t{0} >> (8 * (7 - k)))));
            break;
        }
        case 0x28: return store(address, 1, at, r[rt]);
        case 0x29: return store(address, 2, at, r[rt]);
        case 0x2B: return store(address, 4, at, r[rt]);
        case 0x3F: return store(address, 8, at, r[rt]);
        case 0x2A: {
            std::uint32_t mem = 0;
            if ((fault = unaligned_word(mem))) return fault;
            const unsigned k = address & 3;
            return store(address & ~3u, 4, at, (mem & ~(0xFFFFFFFFu >> (8 * k))) | (lo32(r[rt]) >> (8 * k)));
        }
        case 0x2E: {
            std::uint32_t mem = 0;
            if ((fault = unaligned_word(mem))) return fault;
            const unsigned k = address & 3;
            return store(address & ~3u, 4, at, (mem & ~(0xFFFFFFFFu << (8 * (3 - k)))) | (lo32(r[rt]) << (8 * (3 - k))));
        }
        case 0x2C: {
            if ((fault = unaligned_dword())) return fault;
            const unsigned k = address & 7;
            return store(address & ~7u, 8, at, (v & ~(~std::uint64_t{0} >> (8 * k))) | (r[rt] >> (8 * k)));
        }
        case 0x2D: {
            if ((fault = unaligned_dword())) return fault;
            const unsigned k = address & 7;
            return store(address & ~7u, 8, at, (v & ~(~std::uint64_t{0} << (8 * (7 - k)))) | (r[rt] << (8 * (7 - k))));
        }
        case 0x2F: break;   // CACHE: there is no cache
        case 0x30: if ((fault = load(address, 4, at, v))) return fault; set(rt, sx32(lo32(v))); ll_bit = true; break;
        case 0x34: if ((fault = load(address, 8, at, v))) return fault; set(rt, v); ll_bit = true; break;
        case 0x38: {
            const bool ok = ll_bit;
            if (ok && (fault = store(address, 4, at, r[rt]))) return fault;
            set(rt, ok ? 1 : 0);
            ll_bit = false;
            break;
        }
        case 0x3C: {
            const bool ok = ll_bit;
            if (ok && (fault = store(address, 8, at, r[rt]))) return fault;
            set(rt, ok ? 1 : 0);
            ll_bit = false;
            break;
        }
        case 0x31: if ((fault = load(address, 4, at, v))) return fault; fpu_write32(rt, lo32(v)); break;
        case 0x35: if ((fault = load(address, 8, at, v))) return fault; fpu_write64(rt, v); break;
        case 0x39: return store(address, 4, at, fpu_read32(rt));
        case 0x3D: return store(address, 8, at, fpu_read64(rt));
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
    return std::nullopt;
}

// ---- COP0 ------------------------------------------------------------------------------------------------------------

std::optional<Fault> Cpu::system_control(std::uint32_t word, std::uint32_t at) {
    const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, rd = (word >> 11) & 31;
    const auto read = [&](unsigned n) -> std::uint64_t {
        if (n == 1) {   // Random counts down through the TLB's unwired entries
            const unsigned wired = static_cast<unsigned>(cop0[6] & 0x3F);
            return wired >= 32 ? 31 : 31 - steps % (32 - wired);
        }
        return cop0[n];
    };
    const auto write = [&](unsigned n, std::uint64_t v, bool wide) {
        const std::uint64_t value = wide ? v : sx32(lo32(v));
        switch (n) {
            case 0: cop0[0] = value & 0x8000003F; break;
            case 1: case 8: case 15: break;   // Random, BadVAddr, PRId: read only
            case 2: case 3: cop0[n] = value & 0x3FFFFFFF; break;
            case 5: cop0[5] = value & 0x01FFE000; break;
            case 6: cop0[6] = value & 0x3F; break;
            case 9: cop0[9] = value & 0xFFFFFFFF; break;
            case 10: cop0[10] = value & ~std::uint64_t{0x1F00}; break;
            case 11: cop0[11] = value & 0xFFFFFFFF; cop0[13] &= ~std::uint64_t{0x8000}; break;   // Compare: clears the timer interrupt
            case 13: cop0[13] = (cop0[13] & ~std::uint64_t{0x300}) | (value & 0x300); break;
            case 16: cop0[16] = (cop0[16] & ~std::uint64_t{0x0F00800F}) | (value & 0x0F00800F); break;
            default: cop0[n] = value; break;
        }
    };
    if (rs >= 16) {   // the coprocessor operations
        const unsigned function = word & 63;
        const auto entry_from_registers = [&](TlbEntry& e) {
            e.mask = static_cast<std::uint32_t>(cop0[5]);
            e.hi = cop0[10];
            e.lo0 = cop0[2];
            e.lo1 = cop0[3];
            const std::uint64_t global = cop0[2] & cop0[3] & 1;
            e.lo0 = (e.lo0 & ~std::uint64_t{1}) | global;
            e.lo1 = (e.lo1 & ~std::uint64_t{1}) | global;
        };
        switch (function) {
            case 1: {   // TLBR
                const TlbEntry& e = tlb[cop0[0] & 31];
                cop0[5] = e.mask;
                cop0[10] = e.hi & ~std::uint64_t{0x1F00};
                cop0[2] = e.lo0;
                cop0[3] = e.lo1;
                break;
            }
            case 2: entry_from_registers(tlb[cop0[0] & 31]); break;   // TLBWI
            case 6: entry_from_registers(tlb[read(1) & 31]); break;   // TLBWR
            case 8: {                                                 // TLBP
                cop0[0] |= 0x80000000;
                for (unsigned i = 0; i < tlb.size(); ++i) {
                    const std::uint32_t span = tlb[i].mask | 0x1FFF;
                    const bool global = (tlb[i].lo0 & tlb[i].lo1 & 1) != 0;
                    if (((static_cast<std::uint32_t>(cop0[10]) ^ static_cast<std::uint32_t>(tlb[i].hi)) & ~span) == 0 &&
                        (global || (static_cast<std::uint32_t>(tlb[i].hi) & 0xFF) == (static_cast<std::uint32_t>(cop0[10]) & 0xFF))) {
                        cop0[0] = i;
                        break;
                    }
                }
                break;
            }
            case 24: {   // ERET: to ErrorEPC when ERL is set, else to EPC; clears that bit
                const bool erl = (cop0[12] & 4) != 0;
                const std::uint32_t to = lo32(erl ? cop0[30] : cop0[14]);
                cop0[12] &= ~std::uint64_t{erl ? 4u : 2u};
                ll_bit = false;
                pc = to;
                next_pc = to + 4;
                break;
            }
            default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
        }
        return std::nullopt;
    }
    switch (rs) {
        case 0: set(rt, sx32(lo32(read(rd)))); break;
        case 1: set(rt, read(rd)); break;
        case 4: write(rd, r[rt], false); break;
        case 5: write(rd, r[rt], true); break;
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
    return std::nullopt;
}

// ---- COP1 ------------------------------------------------------------------------------------------------------------

namespace {

float as_float(std::uint32_t bits) { return std::bit_cast<float>(bits); }
double as_double(std::uint64_t bits) { return std::bit_cast<double>(bits); }
std::uint32_t bits_of(float v) { return std::bit_cast<std::uint32_t>(v); }
std::uint64_t bits_of(double v) { return std::bit_cast<std::uint64_t>(v); }

// NaNs in the encoding the R4300i uses (MIPS's, from before IEEE 754-2008): a NaN is quiet when the top bit of its fraction is CLEAR, so the
// default quiet NaN an invalid operation gives (no exception is enabled) is 0x7FBFFFFF, or 0x7FF7FFFFFFFFFFFF for a double. The host's NaNs are
// the other way about, so NaN operands are found from their bits, before the host does any arithmetic.
constexpr std::uint32_t kDefaultNan32 = 0x7FBFFFFF;
constexpr std::uint64_t kDefaultNan64 = 0x7FF7FFFFFFFFFFFF;
bool is_nan(std::uint32_t b) { return (b & 0x7F800000u) == 0x7F800000u && (b & 0x007FFFFFu) != 0; }
bool is_nan(std::uint64_t b) { return (b & 0x7FF0000000000000ull) == 0x7FF0000000000000ull && (b & 0x000FFFFFFFFFFFFFull) != 0; }
bool is_signaling(std::uint32_t b) { return is_nan(b) && (b & 0x00400000u) != 0; }
bool is_signaling(std::uint64_t b) { return is_nan(b) && (b & 0x0008000000000000ull) != 0; }

// What an operation on these operands gives when one is a NaN: a signaling one is an invalid operation, a quiet one is passed on.
template <class Bits>
std::optional<Bits> nan_result(Bits a, Bits b, Bits default_nan) {
    if (is_signaling(a) || is_signaling(b)) return default_nan;
    if (is_nan(a)) return a;
    if (is_nan(b)) return b;
    return std::nullopt;
}

// ADD, SUB, MUL, DIV, SQRT, ABS, MOV, NEG (functions 0-7) on the bits of single or double operands; the result's bits.
template <class Bits>
Bits arithmetic(unsigned function, Bits a_bits, Bits b_bits) {
    using Float = std::conditional_t<sizeof(Bits) == 4, float, double>;
    constexpr Bits sign = Bits{1} << (sizeof(Bits) * 8 - 1);
    constexpr Bits default_nan = sizeof(Bits) == 4 ? static_cast<Bits>(kDefaultNan32) : static_cast<Bits>(kDefaultNan64);
    if (function == 5) return a_bits & ~sign;
    if (function == 6) return a_bits;
    if (function == 7) return a_bits ^ sign;
    if (const std::optional<Bits> n = nan_result(a_bits, function == 4 ? a_bits : b_bits, default_nan)) return *n;
    const Float a = std::bit_cast<Float>(a_bits), b = std::bit_cast<Float>(b_bits);
    Float v = 0;
    switch (function) {
        case 0: v = a + b; break;
        case 1: v = a - b; break;
        case 2: v = a * b; break;
        case 3: v = a / b; break;
        default:
            if (a < 0) return default_nan;   // the square root of a negative number
            v = std::sqrt(a);
            break;
    }
    return std::isnan(v) ? default_nan : std::bit_cast<Bits>(v);   // (0 / 0, infinity - infinity ...: invalid)
}

// `v` rounded as FCR31's rounding mode (or the instruction's own) says.
double rounded(double v, unsigned mode) {
    switch (mode) {
        case 0: return std::nearbyint(v);
        case 1: return std::trunc(v);
        case 2: return std::ceil(v);
        default: return std::floor(v);
    }
}

}  // namespace

std::optional<Fault> Cpu::fpu(std::uint32_t word, std::uint32_t at) {
    const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, fs = (word >> 11) & 31;
    switch (rs) {
        case 0: set(rt, sx32(fpu_read32(fs))); return std::nullopt;
        case 1: set(rt, fpu_read64(fs)); return std::nullopt;
        case 2: set(rt, fs == 0 ? sx32(fcr0) : fs == 31 ? sx32(fcr31) : 0); return std::nullopt;
        case 4: fpu_write32(fs, lo32(r[rt])); return std::nullopt;
        case 5: fpu_write64(fs, r[rt]); return std::nullopt;
        case 6:
            if (fs == 31) fcr31 = lo32(r[rt]) & 0x0183FFFF;
            return std::nullopt;
        case 8: {
            const bool condition = (fcr31 & (1u << 23)) != 0;
            const std::uint32_t target = at + 4 + static_cast<std::uint32_t>(sx16(word)) * 4;
            if (rt > 3) return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
            branch((rt & 1) != 0 ? condition : !condition, target, (rt & 2) != 0);
            return std::nullopt;
        }
        case 16: case 17: case 20: case 21: return fpu_arithmetic(word, at);
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
}

std::optional<Fault> Cpu::fpu_arithmetic(std::uint32_t word, std::uint32_t at) {
    const unsigned format = (word >> 21) & 31, ft = (word >> 16) & 31, fs = (word >> 11) & 31, fd = (word >> 6) & 31, function = word & 63;
    const bool single = format == 16, dbl = format == 17;
    const auto not_run = [&]() {
        const std::optional<dm::Insn> insn = dm::decode_word(word, at, dm::Variant::R4300);
        return Fault{Fault::Kind::NotRun, at, 0, insn ? insn->mnemonic : std::string("cop1"), 0};
    };
    // an operand as a double (exact for a float), whatever its format
    const auto value = [&](unsigned n, unsigned fmt) -> double {
        switch (fmt) {
            case 16: return as_float(fpu_read32(n));
            case 17: return as_double(fpu_read64(n));
            case 20: return static_cast<std::int32_t>(fpu_read32(n));
            default: return static_cast<double>(static_cast<std::int64_t>(fpu_read64(n)));
        }
    };
    if ((single || dbl) && function <= 7) {
        if (single) fpu_write32(fd, arithmetic<std::uint32_t>(function, fpu_read32(fs), fpu_read32(ft)));
        else fpu_write64(fd, arithmetic<std::uint64_t>(function, fpu_read64(fs), fpu_read64(ft)));
        return std::nullopt;
    }
    if ((single || dbl) && function >= 8 && function <= 15) {   // round, trunc, ceil, floor to a long (8-11) or a word (12-15)
        const double v = value(fs, format);
        if (std::isnan(v)) return not_run();
        const double whole = rounded(v, function == 8 || function == 12 ? 0 : function == 9 || function == 13 ? 1 : function == 10 || function == 14 ? 2 : 3);
        if (function < 12) {
            if (whole < -9223372036854775808.0 || whole >= 9223372036854775808.0) return not_run();
            fpu_write64(fd, static_cast<std::uint64_t>(static_cast<std::int64_t>(whole)));
        } else {
            if (whole < -2147483648.0 || whole > 2147483647.0) return not_run();
            fpu_write32(fd, static_cast<std::uint32_t>(static_cast<std::int32_t>(whole)));
        }
        return std::nullopt;
    }
    if (function == 32 || function == 33) {   // cvt.s, cvt.d
        if (format == (function == 32 ? 16u : 17u) || (format != 16 && format != 17 && format != 20 && format != 21)) return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
        const double v = value(fs, format);
        if (function == 32) {
            const float out = format == 21 ? static_cast<float>(static_cast<std::int64_t>(fpu_read64(fs))) : static_cast<float>(v);
            fpu_write32(fd, bits_of(out));
        } else {
            const double out = format == 21 ? static_cast<double>(static_cast<std::int64_t>(fpu_read64(fs))) : v;
            fpu_write64(fd, bits_of(out));
        }
        return std::nullopt;
    }
    if ((function == 36 || function == 37) && (single || dbl)) {   // cvt.w, cvt.l: by the rounding mode FCR31 holds
        const double v = value(fs, format);
        if (std::isnan(v)) return not_run();
        const double whole = rounded(v, fcr31 & 3);
        if (function == 36) {
            if (whole < -2147483648.0 || whole > 2147483647.0) return not_run();
            fpu_write32(fd, static_cast<std::uint32_t>(static_cast<std::int32_t>(whole)));
        } else {
            if (whole < -9223372036854775808.0 || whole >= 9223372036854775808.0) return not_run();
            fpu_write64(fd, static_cast<std::uint64_t>(static_cast<std::int64_t>(whole)));
        }
        return std::nullopt;
    }
    if (function >= 48 && (single || dbl)) {   // c.cond: bit 0 unordered, 1 equal, 2 less than (bit 3 is the signalling variant)
        const double a = value(fs, format), b = value(ft, format);
        const bool unordered = std::isnan(a) || std::isnan(b);
        const unsigned cond = function & 15;
        const bool result = (((cond & 1) != 0) && unordered) || (((cond & 2) != 0) && !unordered && a == b) || (((cond & 4) != 0) && !unordered && a < b);
        fcr31 = (fcr31 & ~(1u << 23)) | (result ? 1u << 23 : 0);
        return std::nullopt;
    }
    return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
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
    for (unsigned i = 0; i < 4; ++i) cpu.r[4 + i] = sx32(options.a[i]);
    for (unsigned i = 0; i < 8; ++i) cpu.r[16 + i] = sx32(options.s[i]);
    for (unsigned i = 0; i < 8; ++i) cpu.r[8 + i] = sx32(options.t[i]);
    cpu.r[28] = sx32(options.gp);
    cpu.r[29] = sx32(options.sp);
    cpu.r[31] = sx32(kReturnAddress);
    cpu.cop0[12] = 0x34000000;   // as the boot code leaves it: COP0, COP1 and FR on
    cpu.cop0[15] = 0x0B22;
    cpu.cop0[16] = 0x7006E463;
    cpu.cop0[1] = 31;
    cpu.jump_to(entry);
    const std::size_t log_from = bus.log().size();

    RunResult out;
    while (cpu.steps < options.max_steps) {
        if (cpu.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        const std::optional<Fault> fault = cpu.step();
        if (!fault) continue;
        out.end = End::Stopped;
        out.fault = fault;
        break;
    }
    out.r = cpu.r;
    out.fpr = cpu.fpr;
    out.hi = cpu.hi;
    out.lo = cpu.lo;
    out.fcr31 = cpu.fcr31;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_from), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    const std::pair<const char*, unsigned> names[] = {{"V0", 2},  {"V1", 3},  {"S0", 16}, {"S1", 17}, {"S2", 18}, {"S3", 19}, {"S4", 20},
                                                      {"S5", 21}, {"S6", 22}, {"S7", 23}, {"GP", 28}, {"SP", 29}, {"FP", 30}, {"RA", 31}};
    for (const auto& [name, number] : names) run.results[name] = result.r[number];
    run.results["HI"] = result.hi;
    run.results["LO"] = result.lo;
    for (const unsigned n : {0u, 20u, 22u, 24u, 26u, 28u, 30u}) run.results[std::format("F{}", n)] = result.fpr[n];
    for (const BusAccess& access : result.io) {
        cb::emulate::IoAccess io;
        io.kind = access.text();
        io.port = access.address;
        io.value = access.value;
        io.size = access.size;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Big;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options) { return record(run(bus, entry, options)); }

}  // namespace cb::emulate::r4300
