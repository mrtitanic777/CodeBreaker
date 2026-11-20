// cb-unity: alone (file-local names that other files in a batch also use)
#include "emulate/arm.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <format>

#include "core/error.hpp"
#include "decompile/arm_ext.hpp"

namespace cb::emulate::arm {

namespace da = decompile::arm;

namespace {

std::uint32_t mask_to(std::uint32_t v, unsigned size) { return size == 4 ? v : v & ((1u << (8 * size)) - 1); }

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const unsigned shift = 32 - bits;
    return static_cast<std::int32_t>(v << shift) >> shift;
}

struct Sum {
    std::uint32_t value;
    bool carry;
    bool overflow;
};

// x + y + carry-in, with the carry out and the signed overflow.
Sum add_with_carry(std::uint32_t x, std::uint32_t y, bool carry_in) {
    const std::uint64_t wide = static_cast<std::uint64_t>(x) + y + (carry_in ? 1 : 0);
    const std::uint32_t result = static_cast<std::uint32_t>(wide);
    return {result, (wide >> 32) != 0, (~(x ^ y) & (x ^ result) & 0x80000000u) != 0};
}

}  // namespace

std::string BusAccess::text() const { return std::format("{}{}", kind == Kind::Read ? 'r' : 'w', size); }

std::string BiosCall::text() const {
    if (!name.empty()) return std::format("{}({:x},{:x},{:x},{:x})", name, args[0], args[1], args[2], args[3]);
    return std::format("SWI:{:02X}({:X},{:X},{:X},{:X})", function, args[0], args[1], args[2], args[3]); }

std::string Fault::text() const {
    switch (kind) {
        case Kind::NoInstruction: return std::format("no instruction at {:08X}", pc);
        case Kind::NotRun: return "not run: " + mnemonic;
        case Kind::BusError: return std::format("bus error at {:08X}", address);
        case Kind::Swi: return "swi";
    }
    return {};
}

// ---- the bus ---------------------------------------------------------------------------------------------------------

Bus::Bus(System system)
    : system_(system),
      ewram_(system >= System::N3ds ? 0 : system == System::Gba ? kEwramSize : kNdsMainSize, 0),
      iwram_(system >= System::N3ds ? 0 : system == System::Gba ? kIwramSize : kNdsSharedSize, 0),
      io_(system >= System::N3ds ? 0 : system == System::Gba ? kIoSize : kNdsIoSize, 0),
      palette_(system >= System::N3ds ? 0 : system == System::Gba ? kPaletteSize : 0x800, 0),
      vram_(system >= System::N3ds ? 0 : system == System::Gba ? kVramSize : kNdsVramSize, 0),
      oam_(system >= System::N3ds ? 0 : system == System::Gba ? kOamSize : 0x800, 0),
      sram_(system >= System::N3ds ? 0 : system == System::Gba ? kSramSize : 0, 0xFF) {
    if (system == System::Nds9) {
        itcm_.assign(kNdsItcmSize, 0);
        dtcm_.assign(kNdsDtcmSize, 0);
    } else if (system == System::Nds7) {
        wram7_.assign(kNdsWram7Size, 0);
    }
}

void Bus::load(std::uint32_t address, ByteView data) {
    if (paged()) {
        for (std::size_t i = 0; i < data.size(); ++i) paged_.write(address + static_cast<std::uint32_t>(i), 1, data[i]);
        return;
    }
    for (std::size_t i = 0; i < data.size(); ++i) {
        std::size_t room = 0;
        if (std::uint8_t* to = place(address + static_cast<std::uint32_t>(i), room)) *to = data[i];
    }
}

std::uint32_t Bus::cp15_read(std::uint32_t reg) {
    const auto it = cp15_.find(reg);
    if (it != cp15_.end()) return it->second;
    switch (reg >> 4) {
        case 0x000: return (reg & 15) == 0 ? 0x41059461u : 0x0F0D2112u;   // the ARM946E-S's ID code (opcode 2: 0) and cache type (1)
        case 0x10: return 0x00052078u;                                     // the control register as the boot code leaves it
        default: return 0;
    }
}

void Bus::cp15_write(std::uint32_t reg, std::uint32_t value) {
    cp15_[reg] = value;
    if (reg == 0x910) {   // c9, c1, opcode 2 = 0: the data TCM region
        dtcm_base_ = value & 0xFFFFF000u;
        dtcm_size_ = (value >> 1 & 0x1F) >= 3 && (value >> 1 & 0x1F) <= 23 ? 512u << (value >> 1 & 0x1F) : 0;
        if (dtcm_size_ > kNdsDtcmSize) dtcm_size_ = kNdsDtcmSize;
    }
}

void Bus::map(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write)}); }

const Bus::Range* Bus::range_of(std::uint32_t address) const {
    for (auto it = ranges_.rbegin(); it != ranges_.rend(); ++it)
        if (address >= it->lo && address <= it->hi) return &*it;
    return nullptr;
}

std::uint8_t* Bus::place(std::uint32_t a, std::size_t& room) {
    const auto at = [&](Bytes& memory, std::uint32_t offset) {
        room = memory.size() - offset;
        return memory.data() + offset;
    };
    if (paged()) return nullptr;
    if (system_ != System::Gba) {
        if (system_ == System::Nds9) {
            if (a - dtcm_base_ < dtcm_size_) return at(dtcm_, (a - dtcm_base_) & (kNdsDtcmSize - 1));   // (over the main RAM it lies in)
            if (a < 0x02000000) return at(itcm_, a & (kNdsItcmSize - 1));
        }
        switch (a >> 24) {
            case 0x02: return at(ewram_, a & (kNdsMainSize - 1));
            case 0x03:
                if (system_ == System::Nds7 && a >= kNdsWram7Base) return at(wram7_, a & (kNdsWram7Size - 1));
                return at(iwram_, a & (kNdsSharedSize - 1));
            case 0x04: return (a & 0xFFFFFF) < io_.size() ? at(io_, a & 0xFFFFFF) : nullptr;
            case 0x05: return system_ == System::Nds9 ? at(palette_, a & 0x7FF) : nullptr;
            case 0x06: {
                if (system_ != System::Nds9) return nullptr;
                const std::uint32_t within = a & 0x1FFFFF;
                switch ((a >> 21) & 7) {   // engine A background, B background, A objects, B objects, the LCDC window
                    case 0: return within < 0x80000 ? at(vram_, within) : nullptr;
                    case 1: return within < 0x20000 ? at(vram_, 0x80000 + within) : nullptr;
                    case 2: return within < 0x40000 ? at(vram_, 0xA0000 + within) : nullptr;
                    case 3: return within < 0x20000 ? at(vram_, 0xE0000 + within) : nullptr;
                    case 4: return within < 0xA4000 ? at(vram_, 0x100000 + within) : nullptr;
                    default: return nullptr;
                }
            }
            case 0x07: return system_ == System::Nds9 ? at(oam_, a & 0x7FF) : nullptr;
            default: return nullptr;
        }
    }
    switch (a >> 24) {
        case 0x02: return at(ewram_, a & (kEwramSize - 1));
        case 0x03: return at(iwram_, a & (kIwramSize - 1));
        case 0x04: return (a & 0xFFFFFF) < kIoSize ? at(io_, a & 0xFFFFFF) : nullptr;
        case 0x05: return at(palette_, a & (kPaletteSize - 1));
        case 0x06: {
            std::uint32_t off = a & 0x1FFFF;
            if (off >= kVramSize) off -= 0x8000;   // (the last 32 KB of the 128 KB window repeat the object tiles)
            return at(vram_, off);
        }
        case 0x07: return at(oam_, a & (kOamSize - 1));
        case 0x0E: case 0x0F: return at(sram_, a & (kSramSize - 1));
        default: return nullptr;
    }
}

bool Bus::mapped(std::uint32_t address) const {
    if (range_of(address)) return true;
    if (paged()) return system_ == System::N3ds ? address >= 0x00100000 && address < 0x20000000 : address >= 0x81000000;
    if (system_ != System::Gba) {
        std::size_t room = 0;
        return const_cast<Bus*>(this)->place(address, room) != nullptr;   // (place only finds where; it changes nothing)
    }
    switch (address >> 24) {
        case 0x00: return address < kBiosSize;
        case 0x02: case 0x03: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F: return true;
        case 0x04: return (address & 0xFFFFFF) < kIoSize;
        default: return false;
    }
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    const std::uint32_t region = address >> 24;
    if (const Range* range = range_of(address)) {
        const std::uint32_t v = range->read ? mask_to(range->read(address, size), size) : 0;
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), address, v});
        return v;
    }
    std::uint32_t v = 0;
    if (system_ == System::Gba && region >= 0x08 && region <= 0x0D) {
        const std::size_t offset = address & 0x1FFFFFF;
        for (unsigned i = 0; i < size; ++i) {
            const std::size_t at = offset + i;
            const std::uint32_t byte = at < rom_.size() ? rom_[at] : ((((address + i) >> 1) & 0xFFFF) >> (8 * ((address + i) & 1))) & 0xFF;   // (past the image: the address bus's value)
            v |= byte << (8 * i);
        }
        return v;
    }
    if (paged()) return static_cast<std::uint32_t>(paged_.read(address, size));
    std::size_t room = 0;
    if (big_endian_ && size < 4) address ^= 4 - size;   // (a byte is at address ^ 3 of the reversed word, a halfword at address ^ 2)
    const std::uint8_t* from = place(address, room);
    for (unsigned i = 0; from && i < size && i < room; ++i) v |= static_cast<std::uint32_t>(from[i]) << (8 * i);
    if (region == 0x04 && from) log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), address, v});
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    const std::uint32_t region = address >> 24;
    if (const Range* range = range_of(address)) {
        const std::uint32_t v = mask_to(value, size);
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), address, v});
        if (range->write) range->write(address, size, v);
        return;
    }
    if (system_ == System::Gba && region >= 0x08 && region <= 0x0D) return;   // the cartridge's ROM
    if (system_ == System::Nds9 && size == 1 && (region == 0x05 || region == 0x07 || (region == 0x06 && (address >> 21 & 7) != 4))) return;   // (8-bit writes to video memory are dropped)
    if (system_ == System::Gba && size == 1 && (region == 0x05 || region == 0x06 || region == 0x07)) {   // a byte written to video memory
        if (region == 0x07 || (region == 0x06 && (address & 0x1FFFF) >= 0x10000)) return;   // (object memory ignores it)
        value = (value & 0xFF) * 0x0101;
        address &= ~1u;
        size = 2;
    }
    if (paged()) {
        paged_.write(address, size, value);
        return;
    }
    std::size_t room = 0;
    if (big_endian_ && size < 4) address ^= 4 - size;
    std::uint8_t* to = place(address, room);
    for (unsigned i = 0; to && i < size && i < room; ++i) to[i] = static_cast<std::uint8_t>(value >> (8 * i));
    if (region == 0x04 && to) log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), address, mask_to(value, size)});
}

// ---- the CPU ---------------------------------------------------------------------------------------------------------

Cpu::Cpu(Bus& bus) : bus_(&bus) {}

unsigned Cpu::bank_of(std::uint32_t mode) {
    switch (mode & 0x1F) {
        case 0x11: return 1;
        case 0x12: return 2;
        case 0x13: return 3;
        case 0x17: return 4;
        case 0x1B: return 5;
        default: return 0;
    }
}

void Cpu::set_cpsr(std::uint32_t value) {
    const unsigned from = bank_of(cpsr), to = bank_of(value);
    if (from != to) {
        banks_[from].sp = r[13];
        banks_[from].lr = r[14];
        if (from == 1) {
            std::copy_n(r.begin() + 8, 5, fiq_high_.begin());
            std::copy(user_high_.begin(), user_high_.end(), r.begin() + 8);
        }
        if (to == 1) {
            std::copy_n(r.begin() + 8, 5, user_high_.begin());
            std::copy(fiq_high_.begin(), fiq_high_.end(), r.begin() + 8);
        }
        r[13] = banks_[to].sp;
        r[14] = banks_[to].lr;
    }
    cpsr = value;
}

std::uint32_t Cpu::spsr() const {
    const unsigned bank = bank_of(cpsr);
    return bank == 0 ? 0 : banks_[bank].spsr;
}

void Cpu::restore_from_spsr() {
    if (bank_of(cpsr) != 0) set_cpsr(spsr());
}

void Cpu::jump_to(std::uint32_t address) {
    cpsr = (address & 1) ? (cpsr | 0x20) : (cpsr & ~0x20u);
    r[15] = address & ((address & 1) ? ~1u : ~3u);
}

void Cpu::branch(std::uint32_t address) {
    r[15] = address;
    branched_ = true;
}

void Cpu::branch_interworking(std::uint32_t address) {
    cpsr = (address & 1) ? (cpsr | 0x20) : (cpsr & ~0x20u);
    branch(address & ((address & 1) ? ~1u : ~3u));
}

std::uint32_t Cpu::saturate(std::int64_t value) {
    if (value > INT32_MAX) {
        cpsr |= 0x08000000u;
        return 0x7FFFFFFFu;
    }
    if (value < INT32_MIN) {
        cpsr |= 0x08000000u;
        return 0x80000000u;
    }
    return static_cast<std::uint32_t>(value);
}

void Cpu::set_nz(std::uint32_t v) {
    cpsr = (cpsr & 0x3FFFFFFF) | (v & 0x80000000u) | (v == 0 ? 0x40000000u : 0u);
}

bool Cpu::condition(unsigned cond) const {
    switch (cond) {
        case 0: return flag_z();
        case 1: return !flag_z();
        case 2: return flag_c();
        case 3: return !flag_c();
        case 4: return flag_n();
        case 5: return !flag_n();
        case 6: return flag_v();
        case 7: return !flag_v();
        case 8: return flag_c() && !flag_z();
        case 9: return !flag_c() || flag_z();
        case 10: return flag_n() == flag_v();
        case 11: return flag_n() != flag_v();
        case 12: return !flag_z() && flag_n() == flag_v();
        case 13: return flag_z() || flag_n() != flag_v();
        default: return true;
    }
}

std::uint32_t Cpu::operand_register(const da::Insn& insn, unsigned n, bool shifted_by_register) const {
    if (n != 15) return r[n];
    return r[15] + (insn.thumb() ? 4u : shifted_by_register ? 12u : 8u);
}

void Cpu::write_register(const da::Insn& insn, unsigned n, std::uint32_t value) {
    if (n == 15) branch(value & (insn.thumb() ? ~1u : ~3u));
    else r[n] = value;
}

Cpu::Shifted Cpu::shift(da::Shift type, std::uint32_t v, std::uint32_t amount, bool, bool rrx) const {
    if (rrx) return {(flag_c() ? 0x80000000u : 0u) | (v >> 1), (v & 1) != 0};
    if (amount == 0) return {v, flag_c()};
    switch (type) {
        case da::Shift::Lsl:
            if (amount < 32) return {v << amount, ((v >> (32 - amount)) & 1) != 0};
            return {0, amount == 32 && (v & 1)};
        case da::Shift::Lsr:
            if (amount < 32) return {v >> amount, ((v >> (amount - 1)) & 1) != 0};
            return {0, amount == 32 && (v >> 31)};
        case da::Shift::Asr:
            if (amount < 32) return {static_cast<std::uint32_t>(static_cast<std::int32_t>(v) >> amount), ((v >> (amount - 1)) & 1) != 0};
            return {(v >> 31) ? 0xFFFFFFFFu : 0u, (v >> 31) != 0};
        case da::Shift::Ror: {
            const unsigned n = amount & 31;
            if (n == 0) return {v, (v >> 31) != 0};
            const std::uint32_t out = da::rotate_right(v, n);
            return {out, (out >> 31) != 0};
        }
    }
    return {v, flag_c()};
}

Cpu::Shifted Cpu::shifter(const da::Insn& insn) const {
    if (insn.imm_form) return {insn.imm, (insn.thumb() || insn.rot == 0) ? flag_c() : (insn.imm >> 31) != 0};
    const std::uint32_t v = operand_register(insn, insn.rm, insn.shift_by_reg);
    const std::uint32_t amount = insn.shift_by_reg ? (r[insn.rs] & 0xFF) : insn.amount;
    return shift(insn.shift, v, amount, insn.shift_by_reg, insn.rrx);
}

std::optional<Fault> Cpu::step() {
    const std::uint32_t pc = r[15];
    if (!bus_->mapped(pc)) return Fault{Fault::Kind::BusError, pc, pc, {}, 0};
    std::optional<da::Insn> insn;
    if (thumb()) {
        const std::uint32_t a = bus_->read(pc & ~1u, 2);
        const std::uint32_t b = (a >> 11) == 0x1E || (bus_->arch() >= da::Arch::V7A && a >= 0xE800) ? bus_->read((pc & ~1u) + 2, 2) : 0;
        insn = da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), pc | 1, bus_->arch());
    } else {
        insn = da::decode_arm(bus_->read(pc & ~3u, 4), pc, bus_->arch());
    }
    if (!insn) return Fault{Fault::Kind::NoInstruction, pc, 0, {}, 0};
    branched_ = false;
    const bool is_it = insn->op == da::Op::Ext && insn->thumb() && insn->ext == da::t2::kIt;
    const bool in_it = it_left_ > 0 && !is_it;
    const unsigned cond = in_it ? da::t2::it_condition(it_first_, it_mask_, it_index_) : insn->cond;
    if (in_it) {
        ++it_index_;
        --it_left_;
    }
    if (!condition(cond)) {
        r[15] = pc + insn->length;
        ++steps;
        return std::nullopt;
    }
    if (in_it && insn->length == 2 && (static_cast<unsigned>(insn->op) < 16 || insn->op == da::Op::Mul) && insn->op != da::Op::Tst && insn->op != da::Op::Teq && insn->op != da::Op::Cmp && insn->op != da::Op::Cmn)
        insn->s = false;   // (the 16-bit data-processing instructions do not set the flags inside an IT block)
    const std::optional<Fault> fault = execute(*insn);
    if (fault && fault->kind != Fault::Kind::Swi) return fault;
    if (!branched_) r[15] = pc + insn->length;
    ++steps;
    return fault;
}

std::optional<Fault> Cpu::execute(const da::Insn& i) {
    const std::uint32_t at = r[15];
    const auto bus_error = [&](std::uint32_t address) { return Fault{Fault::Kind::BusError, at, address, {}, 0}; };
    if (static_cast<unsigned>(i.op) < 16) {
        const bool test = i.op == da::Op::Tst || i.op == da::Op::Teq || i.op == da::Op::Cmp || i.op == da::Op::Cmn;
        std::uint32_t a = 0;
        if (i.op != da::Op::Mov && i.op != da::Op::Mvn) {
            a = operand_register(i, i.rn, !i.imm_form && i.shift_by_reg);
            if (i.thumb() && i.rn == 15 && i.imm_form) a &= ~3u;   // (ADD rd, pc, #imm: the pc is word-aligned)
        }
        const Shifted b = shifter(i);
        const bool carry_in = flag_c();
        std::uint32_t result = 0;
        bool logical = false;
        Sum sum{};
        switch (i.op) {
            case da::Op::And: case da::Op::Tst: result = a & b.value; logical = true; break;
            case da::Op::Eor: case da::Op::Teq: result = a ^ b.value; logical = true; break;
            case da::Op::Orr: result = a | b.value; logical = true; break;
            case da::Op::Mov: result = b.value; logical = true; break;
            case da::Op::Bic: result = a & ~b.value; logical = true; break;
            case da::Op::Mvn: result = ~b.value; logical = true; break;
            case da::Op::Add: case da::Op::Cmn: sum = add_with_carry(a, b.value, false); break;
            case da::Op::Adc: sum = add_with_carry(a, b.value, carry_in); break;
            case da::Op::Sub: case da::Op::Cmp: sum = add_with_carry(a, ~b.value, true); break;
            case da::Op::Sbc: sum = add_with_carry(a, ~b.value, carry_in); break;
            case da::Op::Rsb: sum = add_with_carry(b.value, ~a, true); break;
            case da::Op::Rsc: sum = add_with_carry(b.value, ~a, carry_in); break;
            default: break;
        }
        if (!logical) result = sum.value;
        const bool exception_return = i.s && !test && i.rd == 15 && !i.thumb();
        if (i.s && !exception_return) {
            set_nz(result);
            if (logical) cpsr = (cpsr & ~0x20000000u) | (b.carry ? 0x20000000u : 0u);
            else cpsr = (cpsr & 0x0FFFFFFF) | (sum.carry ? 0x20000000u : 0u) | (sum.overflow ? 0x10000000u : 0u) | (cpsr & 0xC0000000u);
        }
        if (test) return std::nullopt;
        if (exception_return) {
            restore_from_spsr();
            branch(result & (thumb() ? ~1u : ~3u));
        } else {
            write_register(i, i.rd, result);
        }
        return std::nullopt;
    }
    switch (i.op) {
        case da::Op::Mul: case da::Op::Mla: {
            std::uint32_t v = r[i.rm] * r[i.rs];
            if (i.op == da::Op::Mla) v += r[i.rn];
            if (i.s) set_nz(v);
            r[i.rd] = v;
            return std::nullopt;
        }
        case da::Op::Umull: case da::Op::Umlal: case da::Op::Smull: case da::Op::Smlal: {
            const bool is_signed = i.op == da::Op::Smull || i.op == da::Op::Smlal, accumulate = i.op == da::Op::Umlal || i.op == da::Op::Smlal;
            std::uint64_t product = is_signed ? static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(r[i.rm])) * static_cast<std::int32_t>(r[i.rs]))
                                              : static_cast<std::uint64_t>(r[i.rm]) * r[i.rs];
            if (accumulate) product += static_cast<std::uint64_t>(r[i.rn]) << 32 | r[i.rd];
            if (i.s) cpsr = (cpsr & 0x3FFFFFFF) | static_cast<std::uint32_t>((product >> 32) & 0x80000000u) | (product == 0 ? 0x40000000u : 0u);
            r[i.rd] = static_cast<std::uint32_t>(product);
            r[i.rn] = static_cast<std::uint32_t>(product >> 32);
            return std::nullopt;
        }
        case da::Op::Ldr: case da::Op::Str: {
            const std::uint32_t base = i.rn == 15 ? (i.thumb() ? (r[15] + 4) & ~3u : r[15] + 8) : r[i.rn];
            const std::uint32_t offset = i.imm_form ? i.imm : shift(i.shift, r[i.rm], i.amount, false, i.rrx).value;
            const std::uint32_t indexed = i.up ? base + offset : base - offset;
            const std::uint32_t address = i.pre ? indexed : base;
            if (i.size == 8) {   // LDRD, STRD: two words from the (word-aligned) address
                const std::uint32_t first = address & ~3u;
                if (!bus_->mapped(first) || !bus_->mapped(first + 4)) return bus_error(address);
                const bool wb = i.pre ? i.wb : true;
                if (i.op == da::Op::Str) {
                    bus_->write(first, 4, r[i.rd]);
                    bus_->write(first + 4, 4, r[i.rd + 1]);
                    if (wb && i.rn != 15) r[i.rn] = indexed;
                } else {
                    const std::uint32_t low = bus_->read(first, 4), high = bus_->read(first + 4, 4);
                    if (wb && i.rn != 15) r[i.rn] = indexed;
                    r[i.rd] = low;
                    r[i.rd + 1] = high;
                }
                return std::nullopt;
            }
            const unsigned size = i.size;
            const std::uint32_t aligned = address & ~(size - 1);
            if (!bus_->mapped(aligned)) return bus_error(address);
            const bool write_back = i.pre ? i.wb : true;
            if (i.op == da::Op::Str) {
                const std::uint32_t v = i.rd == 15 ? r[15] + (i.thumb() ? 4u : 12u) : r[i.rd];
                bus_->write(aligned, size, mask_to(v, size));
                if (write_back && i.rn != 15) r[i.rn] = indexed;
                return std::nullopt;
            }
            std::uint32_t v = 0;
            if (size == 4) {
                v = da::rotate_right(bus_->read(aligned, 4), 8 * (address & 3));
            } else if (size == 1) {
                v = bus_->read(address, 1);
                if (i.sign) v = static_cast<std::uint32_t>(sext(v, 8));
            } else if (i.sign) {
                v = (address & 1) ? static_cast<std::uint32_t>(sext(bus_->read(address, 1), 8)) : static_cast<std::uint32_t>(sext(bus_->read(address, 2), 16));
            } else {
                v = bus_->read(aligned, 2);
                if (address & 1) v = da::rotate_right(v, 8);
            }
            if (write_back && i.rn != 15) r[i.rn] = indexed;
            if (i.rd == 15 && v5() && !i.thumb()) branch_interworking(v);   // (ARMv5: a load into the pc takes the state from bit 0)
            else write_register(i, i.rd, v);
            return std::nullopt;
        }
        case da::Op::Ldm: case da::Op::Stm: {
            const bool load = i.op == da::Op::Ldm;
            const unsigned count = static_cast<unsigned>(std::popcount(static_cast<unsigned>(i.list)));
            const std::uint32_t base = i.rn == 15 ? r[15] + (i.thumb() ? 4u : 8u) : r[i.rn];
            std::uint32_t low = base;   // the lowest address transferred
            if (i.up) low = i.pre ? base + 4 : base;
            else low = i.pre ? base - 4 * count : base - 4 * count + 4;
            const std::uint32_t new_base = i.up ? base + 4 * count : base - 4 * count;
            if (i.hat && !(load && (i.list & 0x8000))) return Fault{Fault::Kind::NotRun, at, 0, load ? "ldm^" : "stm^", 0};
            std::uint32_t address = low & ~3u;
            const unsigned first = static_cast<unsigned>(std::countr_zero(static_cast<unsigned>(i.list)));
            for (unsigned reg = 0; reg < 16; ++reg) {
                if (!((i.list >> reg) & 1)) continue;
                if (!bus_->mapped(address)) return bus_error(address);
                if (load) {
                    const std::uint32_t v = bus_->read(address, 4);
                    if (reg == 15) {
                        if (i.hat) {
                            restore_from_spsr();
                            branch(v & (thumb() ? ~1u : ~3u));
                        } else if (v5()) {
                            branch_interworking(v);   // (ARMv5: LDM and POP into the pc switch state)
                        } else {
                            branch(v & (thumb() ? ~1u : ~3u));
                        }
                    } else {
                        r[reg] = v;
                    }
                } else {
                    std::uint32_t v = reg == 15 ? r[15] + 12 : r[reg];
                    if (reg == i.rn && i.wb && reg != first) v = new_base;   // (the base is stored as written back unless it is the first register)
                    bus_->write(address, 4, v);
                }
                address += 4;
            }
            if (i.wb && i.rn != 15 && !(load && ((i.list >> i.rn) & 1))) r[i.rn] = new_base;
            return std::nullopt;
        }
        case da::Op::Swp: {
            const std::uint32_t address = r[i.rn];
            const std::uint32_t aligned = i.size == 1 ? address : address & ~3u;
            if (!bus_->mapped(aligned)) return bus_error(address);
            const std::uint32_t old = i.size == 1 ? bus_->read(address, 1) : da::rotate_right(bus_->read(aligned, 4), 8 * (address & 3));
            bus_->write(aligned, i.size, mask_to(r[i.rm], i.size));
            r[i.rd] = old;
            return std::nullopt;
        }
        case da::Op::B: case da::Op::Bl:
            if (i.op == da::Op::Bl) r[14] = i.thumb() ? ((r[15] + i.length) | 1u) : r[15] + 4;
            branch(*i.target & (i.thumb() ? ~1u : ~3u));
            return std::nullopt;
        case da::Op::Bx: {
            const std::uint32_t target = operand_register(i, i.rm, false);
            cpsr = (target & 1) ? (cpsr | 0x20) : (cpsr & ~0x20u);
            branch(target & ((target & 1) ? ~1u : ~3u));
            return std::nullopt;
        }
        case da::Op::Blx: {
            const std::uint32_t next = r[15] + i.length;
            const std::uint32_t target = i.target ? *i.target : operand_register(i, i.rm, false);
            r[14] = i.thumb() ? (next | 1u) : next;
            branch_interworking(target);
            return std::nullopt;
        }
        case da::Op::Clz: r[i.rd] = static_cast<std::uint32_t>(std::countl_zero(r[i.rm])); return std::nullopt;
        case da::Op::Qadd: case da::Op::Qsub: case da::Op::Qdadd: case da::Op::Qdsub: {
            const std::int64_t m = static_cast<std::int32_t>(r[i.rm]);
            std::int64_t n = static_cast<std::int32_t>(r[i.rn]);
            if (i.op == da::Op::Qdadd || i.op == da::Op::Qdsub) n = static_cast<std::int32_t>(saturate(n * 2));
            r[i.rd] = saturate(i.op == da::Op::Qadd || i.op == da::Op::Qdadd ? m + n : m - n);
            return std::nullopt;
        }
        case da::Op::Smla: case da::Op::Smul: case da::Op::Smlalxy: {
            const auto half = [](std::uint32_t v, bool top) { return static_cast<std::int32_t>(static_cast<std::int16_t>(top ? v >> 16 : v)); };
            const std::int32_t product = half(r[i.rm], (i.xy & 1) != 0) * half(r[i.rs], (i.xy & 2) != 0);
            if (i.op == da::Op::Smul) {
                r[i.rd] = static_cast<std::uint32_t>(product);
            } else if (i.op == da::Op::Smla) {
                const std::int64_t sum = static_cast<std::int64_t>(product) + static_cast<std::int32_t>(r[i.rn]);
                if (sum != static_cast<std::int32_t>(sum)) cpsr |= 0x08000000u;   // (overflow sets Q; the result wraps)
                r[i.rd] = static_cast<std::uint32_t>(sum);
            } else {
                const std::int64_t sum = static_cast<std::int64_t>((static_cast<std::uint64_t>(r[i.rn]) << 32) | r[i.rd]) + product;
                r[i.rd] = static_cast<std::uint32_t>(sum);
                r[i.rn] = static_cast<std::uint32_t>(sum >> 32);
            }
            return std::nullopt;
        }
        case da::Op::Smlaw: case da::Op::Smulw: {
            const std::int32_t y = static_cast<std::int16_t>((i.xy & 2) ? r[i.rs] >> 16 : r[i.rs]);
            const std::int32_t product = static_cast<std::int32_t>((static_cast<std::int64_t>(static_cast<std::int32_t>(r[i.rm])) * y) >> 16);
            if (i.op == da::Op::Smulw) {
                r[i.rd] = static_cast<std::uint32_t>(product);
            } else {
                const std::int64_t sum = static_cast<std::int64_t>(product) + static_cast<std::int32_t>(r[i.rn]);
                if (sum != static_cast<std::int32_t>(sum)) cpsr |= 0x08000000u;
                r[i.rd] = static_cast<std::uint32_t>(sum);
            }
            return std::nullopt;
        }
        case da::Op::Pld: return std::nullopt;   // (a hint: no effect)
        case da::Op::Mcr: case da::Op::Mrc: {
            if (i.cp != 15) return Fault{Fault::Kind::NotRun, at, 0, std::format("{} p{}", i.op == da::Op::Mcr ? "mcr" : "mrc", i.cp), 0};
            const std::uint32_t reg = static_cast<std::uint32_t>(i.opc1) << 12 | static_cast<std::uint32_t>(i.crn) << 8 | static_cast<std::uint32_t>(i.crm) << 4 | i.opc2;
            if (i.op == da::Op::Mcr) {
                const std::uint32_t v = i.rd == 15 ? r[15] + 8 : r[i.rd];
                bus_->log_access(BusAccess::Kind::Write, 4, kNdsCp15Base | reg, v);
                bus_->cp15_write(reg, v);
            } else {
                const std::uint32_t v = bus_->cp15_read(reg);
                bus_->log_access(BusAccess::Kind::Read, 4, kNdsCp15Base | reg, v);
                if (i.rd == 15) cpsr = (cpsr & 0x0FFFFFFFu) | (v & 0xF0000000u);
                else r[i.rd] = v;
            }
            return std::nullopt;
        }
        case da::Op::Ext: return i.thumb() ? execute_thumb_ext(i, at) : execute_ext(i, at);
        case da::Op::Bkpt: return Fault{Fault::Kind::NotRun, at, 0, "bkpt", 0};
        case da::Op::Swi: return Fault{Fault::Kind::Swi, at, 0, {}, i.imm};
        case da::Op::Mrs: r[i.rd] = i.spsr ? spsr() : cpsr; return std::nullopt;
        case da::Op::Msr: {
            const std::uint32_t v = i.imm_form ? i.imm : r[i.rm];
            std::uint32_t mask = 0;
            for (unsigned k = 0; k < 4; ++k)
                if ((i.fields >> k) & 1) mask |= 0xFFu << (8 * k);
            if (i.spsr) {
                const unsigned bank = bank_of(cpsr);
                if (bank != 0) banks_[bank].spsr = (banks_[bank].spsr & ~mask) | (v & mask);
                return std::nullopt;
            }
            if ((cpsr & 0x1F) == 0x10) mask &= 0xF0000000u;   // (User mode changes the flags only)
            mask &= ~0x20u;                                    // (the state is changed by BX)
            set_cpsr((cpsr & ~mask) | (v & mask));
            return std::nullopt;
        }
        default: return Fault{Fault::Kind::NoInstruction, at, 0, {}, 0};
    }
}

// The ARMv6 and later instructions of decompile/arm_ext that this interpreter runs; the others stop the run as not run (the VFP, the parallel arithmetic).
std::optional<Fault> Cpu::execute_ext(const da::Insn& i, std::uint32_t at) {
    namespace ex = da::ext;
    const std::uint32_t w = i.word;
    const unsigned rd12 = (w >> 12) & 15, rn16 = (w >> 16) & 15, rm0 = w & 15, rs8 = (w >> 8) & 15;
    const auto rotated = [&](std::uint32_t v) { return da::rotate_right(v, 8 * ((w >> 10) & 3)); };
    const auto sext8 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v))); };
    const auto sext16 = [](std::uint32_t v) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v))); };
    const auto swap32 = [](std::uint32_t v) { return v >> 24 | (v >> 8 & 0xFF00u) | (v << 8 & 0xFF0000u) | v << 24; };
    const auto half = [](std::uint32_t v, bool top) { return static_cast<std::int32_t>(static_cast<std::int16_t>(top ? v >> 16 : v)); };
    const auto load = [&](unsigned size, std::uint32_t address, std::uint32_t& out) -> std::optional<Fault> {
        if (!bus_->mapped(address)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
        out = bus_->read(address & ~(size - 1), size);
        return std::nullopt;
    };
    switch (ex::semantics(i.ext)) {
        case ex::Sem::Rev: r[rd12] = swap32(r[rm0]); return std::nullopt;
        case ex::Sem::Rev16: r[rd12] = (r[rm0] & 0x00FF00FFu) << 8 | (r[rm0] >> 8 & 0x00FF00FFu); return std::nullopt;
        case ex::Sem::Revsh: r[rd12] = sext16(swap32(r[rm0]) >> 16); return std::nullopt;
        case ex::Sem::Sxtb: r[rd12] = sext8(rotated(r[rm0])); return std::nullopt;
        case ex::Sem::Sxth: r[rd12] = sext16(rotated(r[rm0])); return std::nullopt;
        case ex::Sem::Uxtb: r[rd12] = rotated(r[rm0]) & 0xFF; return std::nullopt;
        case ex::Sem::Uxth: r[rd12] = rotated(r[rm0]) & 0xFFFF; return std::nullopt;
        case ex::Sem::Sxtab: r[rd12] = r[rn16] + sext8(rotated(r[rm0])); return std::nullopt;
        case ex::Sem::Sxtah: r[rd12] = r[rn16] + sext16(rotated(r[rm0])); return std::nullopt;
        case ex::Sem::Uxtab: r[rd12] = r[rn16] + (rotated(r[rm0]) & 0xFF); return std::nullopt;
        case ex::Sem::Uxtah: r[rd12] = r[rn16] + (rotated(r[rm0]) & 0xFFFF); return std::nullopt;
        case ex::Sem::Ldrex: case ex::Sem::Ldrexb: case ex::Sem::Ldrexh: {
            const unsigned size = ex::semantics(i.ext) == ex::Sem::Ldrex ? 4 : ex::semantics(i.ext) == ex::Sem::Ldrexb ? 1 : 2;
            std::uint32_t v = 0;
            if (const auto fault = load(size, r[rn16], v)) return fault;
            r[rd12] = v;
            return std::nullopt;
        }
        case ex::Sem::Strex: case ex::Sem::Strexb: case ex::Sem::Strexh: {
            const unsigned size = ex::semantics(i.ext) == ex::Sem::Strex ? 4 : ex::semantics(i.ext) == ex::Sem::Strexb ? 1 : 2;
            if (!bus_->mapped(r[rn16])) return Fault{Fault::Kind::BusError, at, r[rn16], {}, 0};
            bus_->write(r[rn16] & ~(size - 1), size, r[rm0]);
            r[rd12] = 0;   // (the exclusive monitor always succeeds: one processor runs)
            return std::nullopt;
        }
        case ex::Sem::Clrex: case ex::Sem::Hint: return std::nullopt;
        case ex::Sem::Umaal: {
            const std::uint64_t sum = static_cast<std::uint64_t>(r[rm0]) * r[rs8] + r[rd12] + r[rn16];
            r[rd12] = static_cast<std::uint32_t>(sum);
            r[rn16] = static_cast<std::uint32_t>(sum >> 32);
            return std::nullopt;
        }
        case ex::Sem::Smmul: case ex::Sem::Smmla: case ex::Sem::Smmls: {
            const std::int64_t product = static_cast<std::int64_t>(static_cast<std::int32_t>(r[rm0])) * static_cast<std::int32_t>(r[rs8]);
            const std::int64_t accumulator = static_cast<std::int64_t>(static_cast<std::uint64_t>(r[rd12]) << 32);
            const auto kind = ex::semantics(i.ext);
            const std::int64_t total = kind == ex::Sem::Smmul ? product : kind == ex::Sem::Smmla ? accumulator + product : accumulator - product;
            r[rn16] = static_cast<std::uint32_t>(static_cast<std::uint64_t>(total) >> 32);
            return std::nullopt;
        }
        case ex::Sem::Ssat: case ex::Sem::Usat: {
            const bool is_signed = ex::semantics(i.ext) == ex::Sem::Ssat;
            const unsigned bits = ((w >> 16) & 31) + (is_signed ? 1 : 0);
            unsigned amount = (w >> 7) & 31;
            std::int64_t v = static_cast<std::int32_t>(r[rm0]);
            if ((w >> 6) & 1) v = amount == 0 ? (v < 0 ? -1 : 0) : v >> amount;
            else v = static_cast<std::int32_t>(r[rm0] << amount);
            const std::int64_t high = is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1;
            const std::int64_t low = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
            std::int64_t out = v;
            if (v > high) out = high;
            if (v < low) out = low;
            if (out != v) cpsr |= 0x08000000u;
            r[rd12] = static_cast<std::uint32_t>(out);
            return std::nullopt;
        }
        case ex::Sem::Pkhbt: r[rd12] = (r[rn16] & 0xFFFFu) | ((r[rm0] << ((w >> 7) & 31)) & 0xFFFF0000u); return std::nullopt;
        case ex::Sem::Pkhtb: {
            const unsigned amount = (w >> 7) & 31;
            const std::int32_t shifted = amount == 0 ? (static_cast<std::int32_t>(r[rm0]) < 0 ? -1 : 0) : static_cast<std::int32_t>(r[rm0]) >> amount;
            r[rd12] = (r[rn16] & 0xFFFF0000u) | (static_cast<std::uint32_t>(shifted) & 0xFFFFu);
            return std::nullopt;
        }
        case ex::Sem::Usad8: case ex::Sem::Usada8: {
            std::uint32_t sum = ex::semantics(i.ext) == ex::Sem::Usada8 ? r[rd12] : 0;
            for (unsigned k = 0; k < 32; k += 8) {
                const std::int32_t a = static_cast<std::int32_t>(r[rm0] >> k & 0xFF), b = static_cast<std::int32_t>(r[rs8] >> k & 0xFF);
                sum += static_cast<std::uint32_t>(a > b ? a - b : b - a);
            }
            r[rn16] = sum;
            return std::nullopt;
        }
        case ex::Sem::Smuad: case ex::Sem::Smlad: case ex::Sem::Smusd: case ex::Sem::Smlsd: {
            const auto kind = ex::semantics(i.ext);
            const std::int64_t low = static_cast<std::int64_t>(half(r[rm0], false)) * half(r[rs8], false);
            const std::int64_t high = static_cast<std::int64_t>(half(r[rm0], true)) * half(r[rs8], true);
            std::int64_t total = (kind == ex::Sem::Smuad || kind == ex::Sem::Smlad) ? low + high : low - high;
            if (kind == ex::Sem::Smlad || kind == ex::Sem::Smlsd) total += static_cast<std::int32_t>(r[rd12]);
            if (total != static_cast<std::int32_t>(total)) cpsr |= 0x08000000u;
            r[rn16] = static_cast<std::uint32_t>(total);
            return std::nullopt;
        }
        default: return execute_vfp(i, at);
    }
}

// The VFP rows that move data and do the single- and double-precision arithmetic on the host floats (rounding mode and exception flags are not modelled).
std::optional<Fault> Cpu::execute_vfp(const da::Insn& i, std::uint32_t at) {
    namespace ex = da::ext;
    const std::uint32_t w = i.word;
    const std::string_view name = ex::mnemonic(i.ext);
    const bool dbl = (w >> 8) & 1;
    const unsigned rn = (w >> 16) & 15, rt = (w >> 12) & 15, rt2 = (w >> 16) & 15;
    // register indices in vfp_ (a double d is the pair of singles 2d, 2d+1)
    const unsigned d_dest = dbl ? (((w >> 12) & 15) | ((w >> 22) & 1) << 4) * 2 : ((w >> 12) & 15) << 1 | ((w >> 22) & 1);
    const unsigned d_m = dbl ? ((w & 15) | ((w >> 5) & 1) << 4) * 2 : (w & 15) << 1 | ((w >> 5) & 1);
    const unsigned d_n = dbl ? (((w >> 16) & 15) | ((w >> 7) & 1) << 4) * 2 : ((w >> 16) & 15) << 1 | ((w >> 7) & 1);
    const auto get_f = [&](unsigned k) { return std::bit_cast<float>(vfp_[k]); };
    const auto set_f = [&](unsigned k, float v) { vfp_[k] = std::bit_cast<std::uint32_t>(v); };
    const auto get_d = [&](unsigned k) { return std::bit_cast<double>(static_cast<std::uint64_t>(vfp_[k + 1]) << 32 | vfp_[k]); };
    const auto set_d = [&](unsigned k, double v) {
        const auto bits = std::bit_cast<std::uint64_t>(v);
        vfp_[k] = static_cast<std::uint32_t>(bits);
        vfp_[k + 1] = static_cast<std::uint32_t>(bits >> 32);
    };
    const std::string_view base_name = name.substr(0, name.find('.'));
    const auto word_address = [&](std::uint32_t reg) { return reg == 15 ? (r[15] + (i.thumb() ? 4u : 8u)) & ~3u : r[reg]; };
    const auto bad = [&](std::uint32_t address) { return !bus_->mapped(address); };
    if (base_name == "vldr" || base_name == "vstr") {
        const std::uint32_t offset = (w & 0xFF) * 4;
        const std::uint32_t address = ((w >> 23) & 1) ? word_address(rn) + offset : word_address(rn) - offset;
        for (unsigned k = 0; k < (dbl ? 2u : 1u); ++k) {
            const std::uint32_t a = address + 4 * k;
            if (bad(a)) return Fault{Fault::Kind::BusError, at, a, {}, 0};
            if (base_name == "vldr") vfp_[d_dest + k] = bus_->read(a, 4);
            else bus_->write(a, 4, vfp_[d_dest + k]);
        }
        return std::nullopt;
    }
    if (base_name == "vpush" || base_name == "vpop" || base_name == "vldmia" || base_name == "vstmia" || base_name == "vldmdb" || base_name == "vstmdb") {
        const unsigned count = w & 0xFF;   // words
        const bool load = base_name == "vpop" || base_name == "vldmia" || base_name == "vldmdb";
        const bool down = base_name == "vpush" || base_name == "vldmdb" || base_name == "vstmdb";
        const unsigned base_reg = (base_name == "vpush" || base_name == "vpop") ? 13 : rn;
        std::uint32_t address = down ? r[base_reg] - 4 * count : r[base_reg];
        const std::uint32_t start = address;
        for (unsigned k = 0; k < count; ++k, address += 4) {
            if (bad(address)) return Fault{Fault::Kind::BusError, at, address, {}, 0};
            if (load) vfp_[(d_dest + k) & 63] = bus_->read(address, 4);
            else bus_->write(address, 4, vfp_[(d_dest + k) & 63]);
        }
        if (base_name == "vpush" || base_name == "vpop" || ((w >> 21) & 1)) r[base_reg] = down ? start : r[base_reg] + 4 * count;
        return std::nullopt;
    }
    if (base_name == "vmov" && name.size() == 4) {   // between the core registers and the VFP
        if ((w & 0x0FE00FD0) == 0x0C400B10) {   // two core registers and a double
            const unsigned m = ((w & 15) | ((w >> 5) & 1) << 4) * 2;
            if ((w >> 20) & 1) {
                r[rt] = vfp_[m];
                r[rt2] = vfp_[m + 1];
            } else {
                vfp_[m] = r[rt];
                vfp_[m + 1] = r[rt2];
            }
            return std::nullopt;
        }
        const unsigned n = ((w >> 16) & 15) << 1 | ((w >> 7) & 1);
        if ((w >> 20) & 1) r[rt] = vfp_[n];
        else vfp_[n] = r[rt];
        return std::nullopt;
    }
    if (base_name == "vmrs" || base_name == "vmsr") {
        if (base_name == "vmsr") fpscr_ = r[rt];
        else if (rt == 15) cpsr = (cpsr & 0x0FFFFFFFu) | (fpscr_ & 0xF0000000u);
        else r[rt] = fpscr_;
        return std::nullopt;
    }
    if (name.starts_with("vmov.")) {
        if (dbl) set_d(d_dest, get_d(d_m));
        else vfp_[d_dest] = vfp_[d_m];
        return std::nullopt;
    }
    if (base_name == "vabs") {
        if (dbl) set_d(d_dest, std::fabs(get_d(d_m)));
        else vfp_[d_dest] = vfp_[d_m] & 0x7FFFFFFFu;
        return std::nullopt;
    }
    if (base_name == "vneg") {
        if (dbl) set_d(d_dest, -get_d(d_m));
        else vfp_[d_dest] = vfp_[d_m] ^ 0x80000000u;
        return std::nullopt;
    }
    if (base_name == "vsqrt") {
        if (dbl) set_d(d_dest, std::sqrt(get_d(d_m)));
        else set_f(d_dest, std::sqrt(get_f(d_m)));
        return std::nullopt;
    }
    if (base_name == "vadd" || base_name == "vsub" || base_name == "vmul" || base_name == "vdiv" || base_name == "vnmul" || base_name == "vmla" || base_name == "vmls" ||
        base_name == "vnmla" || base_name == "vnmls") {
        const auto apply = [&](auto a, auto b, auto d) {
            if (base_name == "vadd") return a + b;
            if (base_name == "vsub") return a - b;
            if (base_name == "vmul") return a * b;
            if (base_name == "vdiv") return a / b;
            if (base_name == "vnmul") return -(a * b);
            if (base_name == "vmla") return d + a * b;
            if (base_name == "vmls") return d - a * b;
            if (base_name == "vnmla") return -d - a * b;
            return -d + a * b;
        };
        if (dbl) set_d(d_dest, apply(get_d(d_n), get_d(d_m), get_d(d_dest)));
        else set_f(d_dest, apply(get_f(d_n), get_f(d_m), get_f(d_dest)));
        return std::nullopt;
    }
    if (base_name == "vcmp" || base_name == "vcmpe") {
        const bool zero = ((w >> 16) & 15) == 5;
        const double a = dbl ? get_d(d_dest) : get_f(d_dest), b = zero ? 0.0 : dbl ? get_d(d_m) : get_f(d_m);
        std::uint32_t flags = 0x30000000u;   // unordered: C and V
        if (a == b) flags = 0x60000000u;
        else if (a < b) flags = 0x80000000u;
        else if (a > b) flags = 0x20000000u;
        fpscr_ = (fpscr_ & 0x0FFFFFFFu) | flags;
        return std::nullopt;
    }
    if (name == "vcvt.f64.f32") {
        set_d(d_dest, static_cast<double>(get_f(((w & 15) << 1) | ((w >> 5) & 1))));
        return std::nullopt;
    }
    if (name == "vcvt.f32.f64") {
        set_f(((w >> 12) & 15) << 1 | ((w >> 22) & 1), static_cast<float>(get_d(((w & 15) | ((w >> 5) & 1) << 4) * 2)));
        return std::nullopt;
    }
    if (base_name == "vcvt" || base_name == "vcvtr") {
        const bool to_float = name.starts_with("vcvt.f");
        const bool is_signed = name.find(".s32") != std::string_view::npos;
        if (to_float) {
            const std::uint32_t src = vfp_[((w & 15) << 1) | ((w >> 5) & 1)];
            const double v = is_signed ? static_cast<double>(static_cast<std::int32_t>(src)) : static_cast<double>(src);
            if (dbl) set_d(d_dest, v);
            else set_f(d_dest, static_cast<float>(v));
        } else {
            const double v = dbl ? get_d(d_m) : static_cast<double>(get_f(d_m));
            const unsigned dest = ((w >> 12) & 15) << 1 | ((w >> 22) & 1);
            if (std::isnan(v)) vfp_[dest] = 0;
            else if (is_signed) vfp_[dest] = static_cast<std::uint32_t>(v >= 2147483647.0 ? INT32_MAX : v <= -2147483648.0 ? INT32_MIN : static_cast<std::int32_t>(v));
            else vfp_[dest] = v <= 0 ? 0u : v >= 4294967295.0 ? 0xFFFFFFFFu : static_cast<std::uint32_t>(v);
        }
        return std::nullopt;
    }
    return Fault{Fault::Kind::NotRun, at, 0, std::string(name), 0};
}


// The Thumb additions: IT, CBZ and CBNZ, the hints, the 16-bit extensions and byte reversals, and the VFP's Thumb words (the ARM rows with their halves swapped).
std::optional<Fault> Cpu::execute_thumb_ext(const da::Insn& i, std::uint32_t at) {
    namespace t2 = da::t2;
    if (i.ext == t2::kIt) {
        it_first_ = static_cast<std::uint8_t>(i.rs);
        it_mask_ = static_cast<std::uint8_t>(i.list);
        it_index_ = 0;
        it_left_ = static_cast<std::uint8_t>(4 - std::countr_zero(static_cast<unsigned>(i.list)));
        return std::nullopt;
    }
    if (i.ext == t2::kCbz || i.ext == t2::kCbnz) {
        if ((r[i.rn] == 0) == (i.ext == t2::kCbz)) branch(*i.target & ~1u);
        return std::nullopt;
    }
    if (i.ext >= t2::kHint && i.ext < t2::kHint + 5) return std::nullopt;
    if (i.ext >= t2::kExtend && i.ext < t2::kExtend + 7) {
        const std::uint32_t v = r[i.rm];
        const auto swap32 = [](std::uint32_t x) { return x >> 24 | (x >> 8 & 0xFF00u) | (x << 8 & 0xFF0000u) | x << 24; };
        switch (i.ext - t2::kExtend) {
            case 0: r[i.rd] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v))); break;
            case 1: r[i.rd] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v))); break;
            case 2: r[i.rd] = v & 0xFFFF; break;
            case 3: r[i.rd] = v & 0xFF; break;
            case 4: r[i.rd] = swap32(v); break;
            case 5: r[i.rd] = (v & 0x00FF00FFu) << 8 | (v >> 8 & 0x00FF00FFu); break;
            default: r[i.rd] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(swap32(v) >> 16))); break;
        }
        return std::nullopt;
    }
    if (i.ext == t2::kMovt) {
        r[i.rd] = (r[i.rd] & 0xFFFFu) | i.imm << 16;
        return std::nullopt;
    }
    if (i.ext == t2::kUbfx || i.ext == t2::kSbfx) {   // lsb in imm, width in rs
        const std::uint32_t mask = i.rs == 32 ? 0xFFFFFFFFu : (1u << i.rs) - 1u;
        std::uint32_t v = (r[i.rn] >> i.imm) & mask;
        if (i.ext == t2::kSbfx && i.rs < 32 && (v >> (i.rs - 1)) & 1) v |= ~mask;
        r[i.rd] = v;
        return std::nullopt;
    }
    if (i.ext == t2::kBfi) {   // lsb in imm, width in rs; Rn = pc means BFC
        const std::uint32_t mask = (i.rs == 32 ? 0xFFFFFFFFu : (1u << i.rs) - 1u) << i.imm;
        const std::uint32_t source = i.rn == 15 ? 0 : r[i.rn] << i.imm;
        r[i.rd] = (r[i.rd] & ~mask) | (source & mask);
        return std::nullopt;
    }
    if (i.ext >= t2::kExtend32 && i.ext < t2::kExtend32 + 4) {
        const std::uint32_t v = da::rotate_right(r[i.rm], i.amount);
        switch (i.ext - t2::kExtend32) {
            case 0: r[i.rd] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v))); break;
            case 1: r[i.rd] = v & 0xFFFF; break;
            case 2: r[i.rd] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v))); break;
            default: r[i.rd] = v & 0xFF; break;
        }
        return std::nullopt;
    }
    if (i.ext >= 0 && i.ext < 1000) {
        da::Insn arm_form = i;
        arm_form.word = (i.word << 16) | (i.word >> 16);
        return execute_ext(arm_form, at);
    }
    const std::string text = da::text(i);
    return Fault{Fault::Kind::NotRun, at, 0, "thumb2 " + text.substr(0, text.find(' ')), 0};
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

namespace {

// The BIOS calls that are arithmetic or memory moves, as the BIOS does them; the others return 0.
void bios_call(Cpu& cpu, std::uint32_t function) {
    auto& r = cpu.r;
    const bool ds = cpu.bus().system() != System::Gba;   // (the DS numbers the calls differently: Div is 9, Sqrt 13)
    const auto divide = [&](std::int32_t number, std::int32_t divisor) {
        if (divisor == 0) {
            r[0] = 0;
            r[1] = static_cast<std::uint32_t>(number);
            r[3] = 0;
            return;
        }
        const std::int64_t q = static_cast<std::int64_t>(number) / divisor, rem = static_cast<std::int64_t>(number) % divisor;
        r[0] = static_cast<std::uint32_t>(q);
        r[1] = static_cast<std::uint32_t>(rem);
        r[3] = static_cast<std::uint32_t>(q < 0 ? -q : q);
    };
    switch (function) {
        case 0x06: if (ds) break; divide(static_cast<std::int32_t>(r[0]), static_cast<std::int32_t>(r[1])); return;
        case 0x09: if (!ds) break; divide(static_cast<std::int32_t>(r[0]), static_cast<std::int32_t>(r[1])); return;
        case 0x07: if (ds) break; divide(static_cast<std::int32_t>(r[1]), static_cast<std::int32_t>(r[0])); return;
        case 0x08: case 0x0D: {
            if ((function == 0x08) == ds) break;
            std::uint32_t root = static_cast<std::uint32_t>(std::sqrt(static_cast<double>(r[0])));
            while (static_cast<std::uint64_t>(root) * root > r[0]) --root;
            while (static_cast<std::uint64_t>(root + 1) * (root + 1) <= r[0]) ++root;
            r[0] = root;
            return;
        }
        case 0x0B: case 0x0C: {
            const bool fill = (r[2] >> 24) & 1;
            const bool words = function == 0x0C || ((r[2] >> 26) & 1);
            std::uint32_t count = r[2] & 0x1FFFFF;
            if (function == 0x0C) count = (count + 7) & ~7u;
            const unsigned size = words ? 4 : 2;
            std::uint32_t from = r[0] & ~(size - 1), to = r[1] & ~(size - 1);
            const std::uint32_t pattern = cpu.bus().mapped(from) ? cpu.bus().read(from, size) : 0;
            for (std::uint32_t k = 0; k < count; ++k) {
                if (cpu.bus().mapped(to)) cpu.bus().write(to, size, fill ? pattern : (cpu.bus().mapped(from) ? cpu.bus().read(from, size) : 0));
                to += size;
                if (!fill) from += size;
            }
            return;
        }
        default: break;
    }
    r[0] = 0;   // (every other call returns 0 and changes nothing else)
}

}  // namespace

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    for (unsigned i = 0; i < 4; ++i) cpu.r[i] = options.a[i];
    for (unsigned i = 0; i < 8; ++i) cpu.r[4 + i] = options.s[i];
    cpu.r[13] = options.sp;
    cpu.r[14] = options.return_address;
    cpu.jump_to(entry);
    const std::size_t log_from = bus.log().size();

    RunResult out;
    while (cpu.steps < options.max_steps) {
        if ((cpu.r[15] & ~1u) == options.return_address) {
            out.end = End::Return;
            break;
        }
        if (!options.imports.empty()) {
            if (const auto it = options.imports.find(cpu.r[15] & ~1u); it != options.imports.end()) {   // a call into an imported function
                BiosCall call;
                call.name = it->second;
                for (unsigned i = 0; i < 4; ++i) call.args[i] = cpu.r[i];
                out.bios.push_back(call);
                if (std::find(options.ending_imports.begin(), options.ending_imports.end(), it->second) != options.ending_imports.end()) {
                    out.end = End::Stopped;
                    out.fault = Fault{Fault::Kind::NotRun, cpu.r[15], 0, "import " + it->second, 0};
                    break;
                }
                cpu.r[0] = 0;
                cpu.jump_to(cpu.r[14]);
                ++cpu.steps;
                continue;
            }
        }
        const std::optional<Fault> fault = cpu.step();
        if (!fault) continue;
        if (fault->kind == Fault::Kind::Swi) {   // a BIOS call: logged and answered
            BiosCall call;
            call.function = bus.paged() ? fault->code & 0xFFFF : cpu.thumb() ? fault->code & 0xFF : (fault->code >> 16) & 0xFF;
            for (unsigned i = 0; i < 4; ++i) call.args[i] = cpu.r[i];
            out.bios.push_back(call);
            if (std::find(options.ending.begin(), options.ending.end(), call.function) != options.ending.end()) {
                out.end = End::Stopped;
                out.fault = Fault{Fault::Kind::NotRun, fault->pc, 0, std::format("svc {:02X}", call.function), call.function};
                break;
            }
            if (bus.paged()) {   // (a kernel call: logged, answered with success)
                cpu.r[0] = 0;
                continue;
            }
            bios_call(cpu, call.function);
            continue;
        }
        out.end = End::Stopped;
        out.fault = fault;
        break;
    }
    out.r = cpu.r;
    out.cpsr = cpu.cpsr;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_from), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    const std::pair<const char*, unsigned> names[] = {{"R0", 0},  {"R1", 1},  {"R4", 4},  {"R5", 5},  {"R6", 6},  {"R7", 7},
                                                      {"R8", 8},  {"R9", 9},  {"R10", 10}, {"R11", 11}, {"SP", 13}};
    for (const auto& [name, number] : names) run.results[name] = static_cast<std::uint64_t>(result.r[number]);
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
        c.name = call.name.empty() ? std::format("SWI:{:02X}", call.function) : call.name;
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

}  // namespace cb::emulate::arm
