// cb/emulate/sh2.cpp -- the SH-2's interpreter (sh2.hpp).

#include "cb/emulate/sh2.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <limits>

#include "cb/core/error.hpp"
#include "cb/decompile/sh2.hpp"

namespace cb::emulate::sh2 {

namespace {

// Which of the 65536 words are instructions (decoded once, from the table the disassembler and the assembler read).
const std::vector<bool>& valid_words(Variant variant) {
    static const std::array<std::vector<bool>, 2> tables = [] {
        std::array<std::vector<bool>, 2> t;
        for (const Variant v : {Variant::Sh2, Variant::Sh4}) {
            std::vector<bool>& row = t[v == Variant::Sh4 ? 1 : 0];
            row.resize(0x10000);
            for (std::uint32_t w = 0; w < 0x10000; ++w) row[w] = decompile::sh2::decode_word(static_cast<std::uint16_t>(w), 0, v).has_value();
        }
        return t;
    }();
    return tables[variant == Variant::Sh4 ? 1 : 0];
}

// The divide unit's registers, as offsets from $FFFFFE00.
constexpr std::uint32_t kDvsr = 0x100, kDvdnt = 0x104, kDvcr = 0x108, kDvdnth = 0x110, kDvdntl = 0x114;
constexpr std::uint32_t kChipBytes = 0x200;

std::uint32_t chip_offset(std::uint32_t address) {
    const std::uint32_t off = address - kChipBase;
    return off >= 0x120 && off < 0x140 ? off - 0x20 : off;   // (the divide unit's registers are seen twice)
}

std::int32_t as_signed(std::uint32_t v) { return static_cast<std::int32_t>(v); }
std::int16_t low_signed16(std::uint32_t v) { return static_cast<std::int16_t>(v & 0xFFFF); }

}  // namespace

std::string BusAccess::text() const {
    if (kind == Kind::Call) return "call";
    if (kind == Kind::Cache) return "cache";
    return std::format("{}{}", kind == Kind::Read ? 'r' : 'w', static_cast<unsigned>(size));
}

// ---- the bus ----------------------------------------------------------------------------------------------------

Bus::Bus() : chip_(kChipBytes, 0) {}
Bus::Bus(Variant variant) : variant_(variant), chip_(kChipBytes, 0) {}

void Bus::on_enter(std::uint32_t lo, std::uint32_t hi, EnterFn entered) { enter_.push_back({lo & kPhysicalMask, hi & kPhysicalMask, std::move(entered)}); }

void Bus::enter(std::uint32_t pc, const std::array<std::uint32_t, 16>& regs) const {
    const std::uint32_t physical = pc & kPhysicalMask;
    for (const Enter& e : enter_)
        if (physical >= e.lo && physical <= e.hi) e.fn(pc, regs);
}

void Bus::note(BusAccess::Kind kind, std::uint32_t address, std::uint32_t value) { log_.push_back({kind, 4, address, value}); }

void Bus::add_ram(std::uint32_t lo, std::size_t size) {
    lo &= kPhysicalMask;
    for (const Region& r : regions_)
        if (lo < r.lo + r.data.size() && r.lo < lo + size) throw UsageError(std::format("RAM at {:08x} overlaps the region at {:08x}", lo, r.lo));
    regions_.push_back({lo, Bytes(size, 0)});
}

const Bus::Region* Bus::region_of(std::uint32_t physical) const {
    for (const Region& r : regions_)
        if (physical >= r.lo && physical - r.lo < r.data.size()) return &r;
    return nullptr;
}
Bus::Region* Bus::region_of(std::uint32_t physical) { return const_cast<Region*>(static_cast<const Bus*>(this)->region_of(physical)); }

const Bus::Range* Bus::range_of(std::uint32_t physical) const {
    for (const Range& r : ranges_)
        if (physical >= r.lo && physical <= r.hi) return &r;
    return nullptr;
}

void Bus::load(std::uint32_t address, ByteView data) {
    const std::uint32_t physical = address & kPhysicalMask;
    Region* r = region_of(physical);
    if (!r || physical - r->lo + data.size() > r->data.size()) throw UsageError(std::format("no RAM holds {} bytes at {:08x}", data.size(), address));
    std::copy(data.begin(), data.end(), r->data.begin() + (physical - r->lo));
}

void Bus::map(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo & kPhysicalMask, hi & kPhysicalMask, std::move(read), std::move(write)});
}

bool Bus::mapped(std::uint32_t address) const {
    if (is_chip(address)) return true;
    const std::uint32_t physical = address & kPhysicalMask;
    return range_of(physical) != nullptr || region_of(physical) != nullptr;
}

bool Bus::is_ram(std::uint32_t address) const {
    return !is_chip(address) && range_of(address & kPhysicalMask) == nullptr && region_of(address & kPhysicalMask) != nullptr;
}

ByteSpan Bus::region(std::uint32_t address) {
    Region* r = region_of(address & kPhysicalMask);
    return r ? ByteSpan(r->data) : ByteSpan();
}
ByteView Bus::region(std::uint32_t address) const {
    const Region* r = region_of(address & kPhysicalMask);
    return r ? ByteView(r->data) : ByteView();
}
std::uint32_t Bus::region_base(std::uint32_t address) const {
    const Region* r = region_of(address & kPhysicalMask);
    return r ? r->lo : 0;
}

std::uint32_t Bus::peek(std::uint32_t address, unsigned size) const {
    const std::uint32_t physical = address & kPhysicalMask;
    const Region* r = region_of(physical);
    if (!r || physical - r->lo + size > r->data.size()) return 0;
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size; ++i) {   // (the SH-4 is little-endian: the byte at the lowest address is the lowest)
        const unsigned k = little_endian() ? size - 1 - i : i;
        v = v << 8 | r->data[physical - r->lo + k];
    }
    return v;
}

std::uint32_t Bus::chip_word(std::uint32_t offset) const {
    return static_cast<std::uint32_t>(chip_[offset]) << 24 | static_cast<std::uint32_t>(chip_[offset + 1]) << 16 |
           static_cast<std::uint32_t>(chip_[offset + 2]) << 8 | chip_[offset + 3];
}
void Bus::set_chip_word(std::uint32_t offset, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) chip_[offset + i] = static_cast<std::uint8_t>(value >> (24 - 8 * i));
}

std::uint32_t Bus::chip_read(std::uint32_t offset, unsigned size) const {
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size && offset + i < chip_.size(); ++i) v = v << 8 | chip_[offset + i];
    return v;
}

// 32-bit / 32-bit: DVDNT written. The quotient is in DVDNT and DVDNTL, the remainder in DVDNTH; a divisor of zero (or the one quotient that
// does not fit) sets OVF and gives the largest value of the dividend's sign.
void Bus::divide32() {
    const std::int32_t dividend = as_signed(chip_word(kDvdnt));
    const std::int32_t divisor = as_signed(chip_word(kDvsr));
    std::uint32_t quotient, remainder;
    if (divisor == 0 || (dividend == std::numeric_limits<std::int32_t>::min() && divisor == -1)) {
        set_chip_word(kDvcr, chip_word(kDvcr) | 1);
        quotient = dividend < 0 ? 0x80000000u : 0x7FFFFFFFu;
        remainder = divisor == 0 ? static_cast<std::uint32_t>(dividend) : 0;
    } else {
        quotient = static_cast<std::uint32_t>(dividend / divisor);
        remainder = static_cast<std::uint32_t>(dividend % divisor);
    }
    set_chip_word(kDvdnt, quotient);
    set_chip_word(kDvdntl, quotient);
    set_chip_word(kDvdnth, remainder);
}

// 64-bit / 32-bit: DVDNTL written (DVDNTH holds the high half).
void Bus::divide64() {
    const std::int64_t dividend = static_cast<std::int64_t>(static_cast<std::uint64_t>(chip_word(kDvdnth)) << 32 | chip_word(kDvdntl));
    const std::int32_t divisor = as_signed(chip_word(kDvsr));
    std::uint32_t quotient, remainder;
    bool overflow = divisor == 0 || (dividend == std::numeric_limits<std::int64_t>::min() && divisor == -1);
    std::int64_t q = 0, r = 0;
    if (!overflow) {
        q = dividend / divisor;
        r = dividend % divisor;
        overflow = q > std::numeric_limits<std::int32_t>::max() || q < std::numeric_limits<std::int32_t>::min();
    }
    if (overflow) {
        set_chip_word(kDvcr, chip_word(kDvcr) | 1);
        quotient = dividend < 0 ? 0x80000000u : 0x7FFFFFFFu;
        remainder = divisor == 0 ? static_cast<std::uint32_t>(dividend) : static_cast<std::uint32_t>(r);
    } else {
        quotient = static_cast<std::uint32_t>(q);
        remainder = static_cast<std::uint32_t>(r);
    }
    set_chip_word(kDvdnt, quotient);
    set_chip_word(kDvdntl, quotient);
    set_chip_word(kDvdnth, remainder);
}

void Bus::chip_write(std::uint32_t offset, unsigned size, std::uint32_t value) {
    for (unsigned i = 0; i < size && offset + i < chip_.size(); ++i) chip_[offset + i] = static_cast<std::uint8_t>(value >> (8 * (size - 1 - i)));
    if (size != 4) return;
    if (offset == kDvdnt) divide32();
    else if (offset == kDvdntl) divide64();
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    std::uint32_t value = 0;
    std::uint32_t logged = address & kPhysicalMask;
    if (variant_ == Variant::Sh4 && address >= kP4Base) {
        value = p4_read(address, size);
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), address, value});
        return value;
    }
    if (address >= kChipBase && variant_ == Variant::Sh2) {
        logged = address;
        value = chip_read(chip_offset(address), size);
    } else if (const Range* range = range_of(address & kPhysicalMask)) {
        value = range->read ? range->read(address & kPhysicalMask, size) : 0;
        if (size < 4) value &= (1u << (8 * size)) - 1;
    } else {
        return peek(address, size);
    }
    log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), logged, value});
    return value;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    if (size < 4) value &= (1u << (8 * size)) - 1;
    if (variant_ == Variant::Sh4 && address >= kP4Base) {
        if (address >= 0xE0000000u && address < 0xE4000000u) {   // the store queues: written as memory, copied out by PREF
            if (size == 4) store_queue_[(address >> 2) & 15] = value;
            return;
        }
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), address, value});
        p4_write(address, size, value);
        return;
    }
    if (address >= kChipBase && variant_ == Variant::Sh2) {
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), address, value});
        chip_write(chip_offset(address), size, value);
        return;
    }
    const std::uint32_t physical = address & kPhysicalMask;
    if (const Range* range = range_of(physical)) {
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), physical, value});
        if (range->write) range->write(physical, size, value);
        return;
    }
    Region* r = region_of(physical);
    if (!r || physical - r->lo + size > r->data.size()) return;
    for (unsigned i = 0; i < size; ++i) {
        const unsigned shift = little_endian() ? 8 * i : 8 * (size - 1 - i);
        r->data[physical - r->lo + i] = static_cast<std::uint8_t>(value >> shift);
    }
}

// ---- the SH-4's P4 area ------------------------------------------------------------------------------------------------

std::uint32_t Bus::p4_read(std::uint32_t address, unsigned size) {
    if (address >= 0xE0000000u && address < 0xE4000000u) return size == 4 ? store_queue_[(address >> 2) & 15] : 0;
    std::uint32_t v = 0;
    for (unsigned i = 0; i < size; ++i) {   // (little-endian)
        const auto it = p4_.find(address + (size - 1 - i));
        v = v << 8 | (it == p4_.end() ? 0u : it->second);
    }
    return v;
}

void Bus::p4_write(std::uint32_t address, unsigned size, std::uint32_t value) {
    for (unsigned i = 0; i < size; ++i) p4_[address + i] = static_cast<std::uint8_t>(value >> (8 * i));
}

// A PREF of a store queue address: the 32 bytes of the queue go to external memory at the address QACR0 or QACR1 and the address's low bits give.
void Bus::flush_store_queue(std::uint32_t address) {
    const unsigned queue = (address >> 5) & 1;
    const std::uint32_t qacr = p4_read(queue == 0 ? 0xFF000038u : 0xFF00003Cu, 4);
    const std::uint32_t target = ((qacr & 0x1C) << 24) | (address & 0x03FFFFE0u);
    for (unsigned k = 0; k < 8; ++k) write(target + 4 * k, 4, store_queue_[queue * 8 + k]);
}

// ---- faults ------------------------------------------------------------------------------------------------------

std::string Fault::text() const {
    switch (kind) {
        case Kind::NoInstruction: return std::format("no instruction at {:08x}", pc);
        case Kind::AddressError: return std::format("address error at {:08x}", address);
        case Kind::BusError: return std::format("bus error at {:08x}", address);
        case Kind::IllegalSlot: return std::format("illegal slot at {:08x}", pc);
        case Kind::Trap: return std::format("trapa {}", code);
        case Kind::Sleep: return "sleep";
        case Kind::FpuDisabled: return std::format("fpu disabled at {:08x}", pc);
    }
    return "fault";
}

// ---- the processor -----------------------------------------------------------------------------------------------

Cpu::Cpu(Bus& bus) : bus_(&bus), sh4_(bus.variant() == Variant::Sh4) {
    if (sh4_) sr = 0x700000F0;   // MD, RB and BL set and the interrupts masked, as a reset leaves it
}

void Cpu::set_sr(std::uint32_t value) {
    const std::uint32_t keep = sh4_ ? sr::kMaskSh4 : sr::kMask;
    const std::uint32_t next = value & keep;
    if (sh4_ && ((next ^ sr) & sr::RB) != 0)   // the register bank changes: R0-R7 are the other eight
        for (unsigned i = 0; i < 8; ++i) std::swap(r[i], bank[i]);
    sr = next;
}

std::optional<Fault> Cpu::load(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t& value) {
    if (size > 1 && address % size != 0) return Fault{Fault::Kind::AddressError, at, address, 0};
    if (!bus_->mapped(address)) return Fault{Fault::Kind::BusError, at, address, 0};
    value = bus_->read(address, size);
    return std::nullopt;
}

std::optional<Fault> Cpu::store(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t value) {
    if (size > 1 && address % size != 0) return Fault{Fault::Kind::AddressError, at, address, 0};
    if (!bus_->mapped(address)) return Fault{Fault::Kind::BusError, at, address, 0};
    bus_->write(address, size, value);
    return std::nullopt;
}

// One step of the division, as the manual's code does it.
void Cpu::div1(unsigned rm, unsigned rn) {
    const bool old_q = (sr & sr::Q) != 0;
    const bool m = (sr & sr::M) != 0;
    bool q = (r[rn] >> 31) != 0;
    r[rn] = (r[rn] << 1) | (t() ? 1u : 0u);
    const std::uint32_t before = r[rn];
    // (old Q, M) = (0,0) and (1,1) subtract the divisor, (0,1) and (1,0) add it; the carry or borrow, and which way Q follows it, are the
    // manual's four cases
    bool tmp1;
    if (old_q == m) {
        r[rn] -= r[rm];
        tmp1 = r[rn] > before;
    } else {
        r[rn] += r[rm];
        tmp1 = r[rn] < before;
    }
    q = m ? (q ? tmp1 : !tmp1) : (q ? !tmp1 : tmp1);
    sr = q ? (sr | sr::Q) : (sr & ~sr::Q);
    set_t(q == m);
}

std::optional<Fault> Cpu::mac_w(std::uint32_t at, unsigned rm, unsigned rn) {
    std::uint32_t tn = 0, tm = 0;
    if (auto f = load(at, r[rn], 2, tn)) return f;
    r[rn] += 2;
    if (auto f = load(at, r[rm], 2, tm)) return f;
    r[rm] += 2;
    const std::int64_t product = static_cast<std::int64_t>(low_signed16(tm)) * low_signed16(tn);
    if ((sr & sr::S) == 0) {
        const std::int64_t mac = static_cast<std::int64_t>(static_cast<std::uint64_t>(mach) << 32 | macl) + product;
        mach = static_cast<std::uint32_t>(static_cast<std::uint64_t>(mac) >> 32);
        macl = static_cast<std::uint32_t>(mac);
    } else {
        const std::int64_t sum = static_cast<std::int64_t>(as_signed(macl)) + product;
        if (sum > std::numeric_limits<std::int32_t>::max()) {
            macl = 0x7FFFFFFF;
            mach |= 1;
        } else if (sum < std::numeric_limits<std::int32_t>::min()) {
            macl = 0x80000000;
            mach |= 1;
        } else {
            macl = static_cast<std::uint32_t>(sum);
        }
    }
    return std::nullopt;
}

std::optional<Fault> Cpu::mac_l(std::uint32_t at, unsigned rm, unsigned rn) {
    std::uint32_t tn = 0, tm = 0;
    if (auto f = load(at, r[rn], 4, tn)) return f;
    r[rn] += 4;
    if (auto f = load(at, r[rm], 4, tm)) return f;
    r[rm] += 4;
    const std::int64_t product = static_cast<std::int64_t>(as_signed(tm)) * as_signed(tn);
    std::int64_t mac = static_cast<std::int64_t>(static_cast<std::uint64_t>(mach) << 32 | macl);
    // a sum that leaves 64 bits is clamped to the 64-bit range first (the 48-bit limit below is tighter when S is set)
    const bool overflow = (product > 0 && mac > std::numeric_limits<std::int64_t>::max() - product) ||
                          (product < 0 && mac < std::numeric_limits<std::int64_t>::min() - product);
    if (overflow) mac = product > 0 ? std::numeric_limits<std::int64_t>::max() : std::numeric_limits<std::int64_t>::min();
    else mac += product;
    if ((sr & sr::S) != 0) {
        constexpr std::int64_t kMax48 = 0x00007FFFFFFFFFFF, kMin48 = -0x0000800000000000;
        mac = std::clamp(mac, kMin48, kMax48);
    }
    mach = static_cast<std::uint32_t>(static_cast<std::uint64_t>(mac) >> 32);
    macl = static_cast<std::uint32_t>(mac);
    return std::nullopt;
}

std::optional<Fault> Cpu::step() {
    if (pc % 2 != 0) return Fault{Fault::Kind::AddressError, pc, pc, 0};
    if (!bus_->is_ram(pc)) return Fault{Fault::Kind::BusError, pc, pc, 0};
    if (bus_->has_enter_hooks()) bus_->enter(pc, r);
    Flow flow;
    if (auto f = execute(static_cast<std::uint16_t>(bus_->peek(pc, 2)), pc, false, flow)) return f;
    ++steps;
    if (!flow.delayed) {
        pc = flow.taken ? flow.target : pc + 2;
        return std::nullopt;
    }
    const std::uint32_t slot = pc + 2;
    if (!bus_->is_ram(slot)) return Fault{Fault::Kind::BusError, slot, slot, 0};
    Flow ignored;
    if (auto f = execute(static_cast<std::uint16_t>(bus_->peek(slot, 2)), slot, true, ignored)) return f;
    ++steps;
    if (flow.restore_sr) set_sr(flow.new_sr);
    pc = flow.taken ? flow.target : pc + 4;
    return std::nullopt;
}

std::optional<Fault> Cpu::execute(std::uint16_t w, std::uint32_t at, bool in_slot, Flow& flow) {
    if (!valid_words(bus_->variant())[w]) return Fault{Fault::Kind::NoInstruction, at, 0, 0};
    if (sh4_) {
        Sh4Outcome o = execute_sh4(w, at, in_slot, flow);
        if (o.handled) return o.fault;
    }
    const unsigned n = (w >> 8) & 15, m = (w >> 4) & 15, d = w & 15;
    const std::uint32_t i8 = w & 0xFF;
    const std::uint32_t s8 = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(w & 0xFF)));
    const std::uint32_t pc_long = ((at + 4) & ~3u);
    const auto illegal_in_slot = [&]() -> std::optional<Fault> {
        if (in_slot) return Fault{Fault::Kind::IllegalSlot, at, 0, 0};
        return std::nullopt;
    };
    const auto branch = [&](std::uint32_t target, bool delayed) {
        flow.taken = true;
        flow.delayed = delayed;
        flow.target = target;
    };
    const auto sext8 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v))); };
    const auto sext16 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v))); };
    std::uint32_t v = 0;

    switch (w >> 12) {
        case 0x0:
            switch (w & 0xF) {
                case 0x2: r[n] = (w & 0xF0) == 0 ? sr : (w & 0xF0) == 0x10 ? gbr : vbr; break;
                case 0x3:
                    if (auto f = illegal_in_slot()) return f;
                    pr = (w & 0xF0) == 0 ? at + 4 : pr;   // BSRF links; BRAF does not
                    branch(at + 4 + r[n], true);
                    break;
                case 0x4: if (auto f = store(at, r[0] + r[n], 1, r[m])) return f; break;
                case 0x5: if (auto f = store(at, r[0] + r[n], 2, r[m])) return f; break;
                case 0x6: if (auto f = store(at, r[0] + r[n], 4, r[m])) return f; break;
                case 0x7: macl = r[n] * r[m]; break;
                case 0x8: if ((w & 0xFF) == 0x08) sr &= ~sr::T; else if ((w & 0xFF) == 0x18) sr |= sr::T; else mach = macl = 0; break;
                case 0x9:
                    if ((w & 0xFF) == 0x19) sr &= ~(sr::Q | sr::M | sr::T);   // DIV0U
                    else if ((w & 0xFF) == 0x29) r[n] = t() ? 1 : 0;           // MOVT
                    break;                                                      // (0x09: NOP)
                case 0xA: r[n] = (w & 0xF0) == 0 ? mach : (w & 0xF0) == 0x10 ? macl : pr; break;
                case 0xB:
                    if (auto f = illegal_in_slot()) return f;
                    if ((w & 0xFF) == 0x1B) return Fault{Fault::Kind::Sleep, at, 0, 0};
                    if ((w & 0xFF) == 0x0B) {   // RTS
                        branch(pr, true);
                    } else {                    // RTE: PC and SR from the stack
                        std::uint32_t new_pc = 0, new_sr = 0;
                        if (auto f = load(at, r[15], 4, new_pc)) return f;
                        r[15] += 4;
                        if (auto f = load(at, r[15], 4, new_sr)) return f;
                        r[15] += 4;
                        sr = new_sr & sr::kMask;
                        branch(new_pc, true);
                    }
                    break;
                case 0xC: if (auto f = load(at, r[0] + r[m], 1, v)) return f; r[n] = sext8(v); break;
                case 0xD: if (auto f = load(at, r[0] + r[m], 2, v)) return f; r[n] = sext16(v); break;
                case 0xE: if (auto f = load(at, r[0] + r[m], 4, v)) return f; r[n] = v; break;
                case 0xF: if (auto f = mac_l(at, m, n)) return f; break;
                default: break;
            }
            break;
        case 0x1: if (auto f = store(at, r[n] + d * 4, 4, r[m])) return f; break;
        case 0x2:
            switch (w & 0xF) {
                case 0x0: if (auto f = store(at, r[n], 1, r[m])) return f; break;
                case 0x1: if (auto f = store(at, r[n], 2, r[m])) return f; break;
                case 0x2: if (auto f = store(at, r[n], 4, r[m])) return f; break;
                case 0x4: if (auto f = store(at, r[n] - 1, 1, r[m])) return f; r[n] -= 1; break;
                case 0x5: if (auto f = store(at, r[n] - 2, 2, r[m])) return f; r[n] -= 2; break;
                case 0x6: if (auto f = store(at, r[n] - 4, 4, r[m])) return f; r[n] -= 4; break;
                case 0x7:   // DIV0S
                    sr = (r[n] >> 31) ? (sr | sr::Q) : (sr & ~sr::Q);
                    sr = (r[m] >> 31) ? (sr | sr::M) : (sr & ~sr::M);
                    set_t(((r[n] ^ r[m]) >> 31) != 0);
                    break;
                case 0x8: set_t((r[n] & r[m]) == 0); break;
                case 0x9: r[n] &= r[m]; break;
                case 0xA: r[n] ^= r[m]; break;
                case 0xB: r[n] |= r[m]; break;
                case 0xC: {   // CMP/STR: any byte of the two equal
                    const std::uint32_t x = r[n] ^ r[m];
                    set_t((x & 0xFF) == 0 || (x & 0xFF00) == 0 || (x & 0xFF0000) == 0 || (x & 0xFF000000) == 0);
                    break;
                }
                case 0xD: r[n] = (r[m] << 16) | (r[n] >> 16); break;   // XTRCT
                case 0xE: macl = static_cast<std::uint32_t>(static_cast<std::uint16_t>(r[n]) * static_cast<std::uint32_t>(static_cast<std::uint16_t>(r[m]))); break;
                case 0xF: macl = static_cast<std::uint32_t>(static_cast<std::int32_t>(low_signed16(r[n])) * static_cast<std::int32_t>(low_signed16(r[m]))); break;
                default: break;
            }
            break;
        case 0x3:
            switch (w & 0xF) {
                case 0x0: set_t(r[n] == r[m]); break;
                case 0x2: set_t(r[n] >= r[m]); break;
                case 0x3: set_t(as_signed(r[n]) >= as_signed(r[m])); break;
                case 0x4: div1(m, n); break;
                case 0x5: {   // DMULU.L
                    const std::uint64_t p = static_cast<std::uint64_t>(r[n]) * r[m];
                    mach = static_cast<std::uint32_t>(p >> 32);
                    macl = static_cast<std::uint32_t>(p);
                    break;
                }
                case 0x6: set_t(r[n] > r[m]); break;
                case 0x7: set_t(as_signed(r[n]) > as_signed(r[m])); break;
                case 0x8: r[n] -= r[m]; break;
                case 0xA: {   // SUBC
                    const std::uint32_t tmp0 = r[n];
                    const std::uint32_t tmp1 = r[n] - r[m];
                    r[n] = tmp1 - (t() ? 1u : 0u);
                    set_t(tmp0 < tmp1 || tmp1 < r[n]);
                    break;
                }
                case 0xB: {   // SUBV
                    const std::uint32_t res = r[n] - r[m];
                    set_t((((r[n] ^ r[m]) & (r[n] ^ res)) >> 31) != 0);
                    r[n] = res;
                    break;
                }
                case 0xC: r[n] += r[m]; break;
                case 0xD: {   // DMULS.L
                    const std::int64_t p = static_cast<std::int64_t>(as_signed(r[n])) * as_signed(r[m]);
                    mach = static_cast<std::uint32_t>(static_cast<std::uint64_t>(p) >> 32);
                    macl = static_cast<std::uint32_t>(p);
                    break;
                }
                case 0xE: {   // ADDC
                    const std::uint32_t tmp0 = r[n];
                    const std::uint32_t tmp1 = r[n] + r[m];
                    r[n] = tmp1 + (t() ? 1u : 0u);
                    set_t(tmp0 > tmp1 || tmp1 > r[n]);
                    break;
                }
                case 0xF: {   // ADDV
                    const std::uint32_t res = r[n] + r[m];
                    set_t(((~(r[n] ^ r[m]) & (r[n] ^ res)) >> 31) != 0);
                    r[n] = res;
                    break;
                }
                default: break;
            }
            break;
        case 0x4:
            switch (w & 0xFF) {
                case 0x00: set_t((r[n] >> 31) != 0); r[n] <<= 1; break;                                   // SHLL
                case 0x01: set_t((r[n] & 1) != 0); r[n] >>= 1; break;                                      // SHLR
                case 0x02: if (auto f = store(at, r[n] - 4, 4, mach)) return f; r[n] -= 4; break;
                case 0x03: if (auto f = store(at, r[n] - 4, 4, sr)) return f; r[n] -= 4; break;
                case 0x04: set_t((r[n] >> 31) != 0); r[n] = (r[n] << 1) | (t() ? 1u : 0u); break;          // ROTL
                case 0x05: set_t((r[n] & 1) != 0); r[n] = (r[n] >> 1) | (t() ? 0x80000000u : 0u); break;   // ROTR
                case 0x06: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; mach = v; break;
                case 0x07: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; sr = v & sr::kMask; break;
                case 0x08: r[n] <<= 2; break;
                case 0x09: r[n] >>= 2; break;
                case 0x0A: mach = r[n]; break;
                case 0x0B:   // JSR
                    if (auto f = illegal_in_slot()) return f;
                    pr = at + 4;
                    branch(r[n], true);
                    break;
                case 0x0E: sr = r[n] & sr::kMask; break;
                case 0x10: r[n] -= 1; set_t(r[n] == 0); break;                                             // DT
                case 0x11: set_t(as_signed(r[n]) >= 0); break;
                case 0x12: if (auto f = store(at, r[n] - 4, 4, macl)) return f; r[n] -= 4; break;
                case 0x13: if (auto f = store(at, r[n] - 4, 4, gbr)) return f; r[n] -= 4; break;
                case 0x15: set_t(as_signed(r[n]) > 0); break;
                case 0x16: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; macl = v; break;
                case 0x17: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; gbr = v; break;
                case 0x18: r[n] <<= 8; break;
                case 0x19: r[n] >>= 8; break;
                case 0x1A: macl = r[n]; break;
                case 0x1B:   // TAS.B
                    if (auto f = load(at, r[n], 1, v)) return f;
                    set_t(v == 0);
                    if (auto f = store(at, r[n], 1, v | 0x80)) return f;
                    break;
                case 0x1E: gbr = r[n]; break;
                case 0x20: set_t((r[n] >> 31) != 0); r[n] <<= 1; break;                                   // SHAL
                case 0x21: set_t((r[n] & 1) != 0); r[n] = static_cast<std::uint32_t>(as_signed(r[n]) >> 1); break;   // SHAR
                case 0x22: if (auto f = store(at, r[n] - 4, 4, pr)) return f; r[n] -= 4; break;
                case 0x23: if (auto f = store(at, r[n] - 4, 4, vbr)) return f; r[n] -= 4; break;
                case 0x24: {   // ROTCL
                    const std::uint32_t old = t() ? 1u : 0u;
                    set_t((r[n] >> 31) != 0);
                    r[n] = (r[n] << 1) | old;
                    break;
                }
                case 0x25: {   // ROTCR
                    const std::uint32_t old = t() ? 0x80000000u : 0u;
                    set_t((r[n] & 1) != 0);
                    r[n] = (r[n] >> 1) | old;
                    break;
                }
                case 0x26: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; pr = v; break;
                case 0x27: if (auto f = load(at, r[n], 4, v)) return f; r[n] += 4; vbr = v; break;
                case 0x28: r[n] <<= 16; break;
                case 0x29: r[n] >>= 16; break;
                case 0x2A: pr = r[n]; break;
                case 0x2B:   // JMP
                    if (auto f = illegal_in_slot()) return f;
                    branch(r[n], true);
                    break;
                case 0x2E: vbr = r[n]; break;
                default:
                    if ((w & 0xF) == 0xF) {   // MAC.W
                        if (auto f = mac_w(at, m, n)) return f;
                    }
                    break;
            }
            break;
        case 0x5: if (auto f = load(at, r[m] + d * 4, 4, v)) return f; r[n] = v; break;
        case 0x6:
            switch (w & 0xF) {
                case 0x0: if (auto f = load(at, r[m], 1, v)) return f; r[n] = sext8(v); break;
                case 0x1: if (auto f = load(at, r[m], 2, v)) return f; r[n] = sext16(v); break;
                case 0x2: if (auto f = load(at, r[m], 4, v)) return f; r[n] = v; break;
                case 0x3: r[n] = r[m]; break;
                case 0x4: {   // (when n == m the loaded value is what stays: the increment is lost)
                    if (auto f = load(at, r[m], 1, v)) return f;
                    if (n != m) r[m] += 1;
                    r[n] = sext8(v);
                    break;
                }
                case 0x5: {
                    if (auto f = load(at, r[m], 2, v)) return f;
                    if (n != m) r[m] += 2;
                    r[n] = sext16(v);
                    break;
                }
                case 0x6: {
                    if (auto f = load(at, r[m], 4, v)) return f;
                    if (n != m) r[m] += 4;
                    r[n] = v;
                    break;
                }
                case 0x7: r[n] = ~r[m]; break;
                case 0x8: r[n] = (r[m] & 0xFFFF0000u) | ((r[m] & 0xFF) << 8) | ((r[m] >> 8) & 0xFF); break;
                case 0x9: r[n] = (r[m] << 16) | (r[m] >> 16); break;
                case 0xA: {   // NEGC
                    const std::uint32_t temp = 0u - r[m];
                    r[n] = temp - (t() ? 1u : 0u);
                    set_t(temp != 0 || temp < r[n]);
                    break;
                }
                case 0xB: r[n] = 0u - r[m]; break;
                case 0xC: r[n] = r[m] & 0xFF; break;
                case 0xD: r[n] = r[m] & 0xFFFF; break;
                case 0xE: r[n] = sext8(r[m]); break;
                case 0xF: r[n] = sext16(r[m]); break;
                default: break;
            }
            break;
        case 0x7: r[n] += s8; break;
        case 0x8:
            switch ((w >> 8) & 0xF) {
                case 0x0: if (auto f = store(at, r[m] + d, 1, r[0])) return f; break;
                case 0x1: if (auto f = store(at, r[m] + d * 2, 2, r[0])) return f; break;
                case 0x4: if (auto f = load(at, r[m] + d, 1, v)) return f; r[0] = sext8(v); break;
                case 0x5: if (auto f = load(at, r[m] + d * 2, 2, v)) return f; r[0] = sext16(v); break;
                case 0x8: set_t(r[0] == s8); break;
                case 0x9: case 0xB: case 0xD: case 0xF: {   // BT BF BT/S BF/S
                    if (auto f = illegal_in_slot()) return f;
                    const bool take = ((w >> 8) & 2) ? !t() : t();   // 0x89 and 0x8D take it when T is set; 0x8B and 0x8F when it is clear
                    const bool delayed = ((w >> 8) & 4) != 0;
                    flow.delayed = delayed;
                    if (take) branch(at + 4 + s8 * 2, delayed);
                    break;
                }
                default: break;
            }
            break;
        case 0x9:
            if (auto g = illegal_in_slot()) return g;
            if (auto f = load(at, at + 4 + i8 * 2, 2, v)) return f;
            r[n] = sext16(v);
            break;
        case 0xA: case 0xB: {
            if (auto f = illegal_in_slot()) return f;
            const std::uint32_t field = static_cast<std::uint32_t>(static_cast<std::int32_t>(w & 0xFFF) - ((w & 0x800) ? 0x1000 : 0));
            if ((w >> 12) == 0xB) pr = at + 4;
            branch(at + 4 + field * 2, true);
            break;
        }
        case 0xC:
            switch ((w >> 8) & 0xF) {
                case 0x0: if (auto f = store(at, gbr + i8, 1, r[0])) return f; break;
                case 0x1: if (auto f = store(at, gbr + i8 * 2, 2, r[0])) return f; break;
                case 0x2: if (auto f = store(at, gbr + i8 * 4, 4, r[0])) return f; break;
                case 0x3: if (auto f = illegal_in_slot()) return f; return Fault{Fault::Kind::Trap, at, 0, i8};
                case 0x4: if (auto f = load(at, gbr + i8, 1, v)) return f; r[0] = sext8(v); break;
                case 0x5: if (auto f = load(at, gbr + i8 * 2, 2, v)) return f; r[0] = sext16(v); break;
                case 0x6: if (auto f = load(at, gbr + i8 * 4, 4, v)) return f; r[0] = v; break;
                case 0x7: if (auto f = illegal_in_slot()) return f; r[0] = pc_long + i8 * 4; break;   // MOVA
                case 0x8: set_t((r[0] & i8) == 0); break;
                case 0x9: r[0] &= i8; break;
                case 0xA: r[0] ^= i8; break;
                case 0xB: r[0] |= i8; break;
                case 0xC: case 0xD: case 0xE: case 0xF: {
                    if (auto f = load(at, gbr + r[0], 1, v)) return f;
                    const unsigned op = (w >> 8) & 3;
                    if (op == 0) set_t((v & i8) == 0);
                    else {
                        v = op == 1 ? (v & i8) : op == 2 ? (v ^ i8) : (v | i8);
                        if (auto f = store(at, gbr + r[0], 1, v)) return f;
                    }
                    break;
                }
                default: break;
            }
            break;
        case 0xD: if (auto g = illegal_in_slot()) return g; if (auto f = load(at, pc_long + i8 * 4, 4, v)) return f; r[n] = v; break;
        case 0xE: r[n] = s8; break;
        default: break;
    }
    return std::nullopt;
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("stopped"));
        case End::Limit: return "limit";
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    const std::size_t log_start = bus.log().size();
    Cpu cpu(bus);
    cpu.pc = entry;
    cpu.pr = kReturnAddress;
    cpu.gbr = options.gbr;
    cpu.vbr = options.vbr;
    cpu.r[0] = options.r0;
    cpu.r[15] = options.sp;
    if (cpu.sh4()) {
        cpu.fpscr = options.fpscr & fpscr::kMask;
        for (std::size_t i = 0; i < options.fa.size(); ++i) cpu.fp[(cpu.fpscr & fpscr::FR) != 0 ? 1 : 0][4 + i] = options.fa[i];
    }
    for (std::size_t i = 0; i < options.a.size(); ++i) cpu.r[4 + i] = options.a[i];
    for (std::size_t i = 0; i < options.s.size(); ++i) cpu.r[8 + i] = options.s[i];

    RunResult result;
    while (cpu.steps < options.max_steps) {
        if (cpu.pc == kReturnAddress) {
            result.end = End::Return;
            break;
        }
        if (const std::optional<Fault> f = cpu.step()) {
            result.end = End::Stopped;
            result.fault = f;
            break;
        }
    }
    if (result.end == End::Limit && cpu.pc == kReturnAddress) result.end = End::Return;
    result.r = cpu.r;
    result.pr = cpu.pr;
    result.gbr = cpu.gbr;
    result.vbr = cpu.vbr;
    result.mach = cpu.mach;
    result.macl = cpu.macl;
    result.sr = cpu.sr;
    result.fpscr = cpu.fpscr;
    result.fpul = cpu.fpul;
    for (unsigned i = 0; i < 16; ++i) result.fr[i] = cpu.fr(i);
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return result;
}

}  // namespace cb::emulate::sh2
