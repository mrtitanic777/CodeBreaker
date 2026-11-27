#include "emulate/w65816.hpp"

#include <format>

namespace cb::emulate::w65816 {

namespace {

constexpr std::size_t kLogLimit = 200000;   // accesses kept of a run (a DMA of 64 KB is that many writes)

}  // namespace

// ---- Machine -------------------------------------------------------------------------------------------------------

std::string Access::text() const {
    std::string name = formats::snes::register_name(address);
    if (name.empty()) name = std::format("IO_{:04X}", address);
    return std::format("{} {} {:02x}", write ? "w" : "r", name, value);
}

Machine::Machine(const formats::snes::Rom& rom) : rom_(rom) {}

std::uint8_t Machine::read(std::uint32_t address) {
    address &= 0xFFFFFF;
    const std::uint32_t bank = address >> 16, off = address & 0xFFFF;
    if (bank == 0x7E) return wram[off];
    if (bank == 0x7F) return wram[0x10000 + off];
    const bool system = (bank & 0x40) == 0;   // banks $00-$3F and $80-$BF
    if (system && off < 0x2000) return wram[off];
    if (system && off < 0x8000) {
        if (off >= 0x2100 && off < 0x2200) return hardware_read(static_cast<std::uint16_t>(off));
        if (off == 0x4016 || off == 0x4017 || (off >= 0x4200 && off < 0x4220) || (off >= 0x4300 && off < 0x4380)) return hardware_read(static_cast<std::uint16_t>(off));
        return 0;
    }
    const std::optional<std::uint32_t> at = rom_.offset(address);
    return at ? rom_.data[*at] : 0;
}

void Machine::write(std::uint32_t address, std::uint8_t value) {
    address &= 0xFFFFFF;
    const std::uint32_t bank = address >> 16, off = address & 0xFFFF;
    if (bank == 0x7E) {
        wram[off] = value;
        return;
    }
    if (bank == 0x7F) {
        wram[0x10000 + off] = value;
        return;
    }
    const bool system = (bank & 0x40) == 0;
    if (system && off < 0x2000) {
        wram[off] = value;
    } else if (system && off < 0x8000) {
        if (off >= 0x2100 && off < 0x2200) hardware_write(static_cast<std::uint16_t>(off), value);
        else if (off == 0x4016 || off == 0x4017 || (off >= 0x4200 && off < 0x4220) || (off >= 0x4300 && off < 0x4380)) hardware_write(static_cast<std::uint16_t>(off), value);
    }
}

std::uint8_t Machine::hardware_read(std::uint16_t address) {
    std::uint8_t value = 0;
    switch (address) {
        case 0x213E:
        case 0x213F: value = 0x01; break;   // the PPU chips' versions
        case 0x2140: value = 0xAA; break;   // the sound CPU's ready signature
        case 0x2141: value = 0xBB; break;
        case 0x2180: value = wram[wram_address_ & 0x1FFFF]; wram_address_ = (wram_address_ + 1) & 0x1FFFF; break;
        case 0x4210: value = (nmi_reads_++ & 1) ? 0x02 : 0x82; break;
        case 0x4212: value = (status_reads_++ & 1) ? 0x00 : 0x80; break;
        case 0x4214: value = static_cast<std::uint8_t>(quotient_); break;
        case 0x4215: value = static_cast<std::uint8_t>(quotient_ >> 8); break;
        case 0x4216: value = static_cast<std::uint8_t>(remainder_ != 0 || product_ == 0 ? remainder_ : product_); break;
        case 0x4217: value = static_cast<std::uint8_t>((remainder_ != 0 || product_ == 0 ? remainder_ : product_) >> 8); break;
        default:
            if (address >= 0x4300 && address < 0x4380) value = dma_regs_[address - 0x4300];
            break;
    }
    if (log_.size() < kLogLimit) log_.push_back({false, address, value});
    return value;
}

void Machine::hardware_write(std::uint16_t address, std::uint8_t value) {
    if (log_.size() < kLogLimit) log_.push_back({true, address, value});
    switch (address) {
        case 0x2180: wram[wram_address_ & 0x1FFFF] = value; wram_address_ = (wram_address_ + 1) & 0x1FFFF; break;
        case 0x2181: wram_address_ = (wram_address_ & 0x1FF00) | value; break;
        case 0x2182: wram_address_ = (wram_address_ & 0x100FF) | static_cast<std::uint32_t>(value) << 8; break;
        case 0x2183: wram_address_ = (wram_address_ & 0x0FFFF) | static_cast<std::uint32_t>(value & 1) << 16; break;
        case 0x4202: multiplicand_ = value; break;
        case 0x4203:
            product_ = static_cast<std::uint16_t>(multiplicand_ * value);
            remainder_ = 0;
            break;
        case 0x4204: dividend_ = static_cast<std::uint16_t>((dividend_ & 0xFF00) | value); break;
        case 0x4205: dividend_ = static_cast<std::uint16_t>((dividend_ & 0x00FF) | value << 8); break;
        case 0x4206:
            quotient_ = value == 0 ? 0xFFFF : static_cast<std::uint16_t>(dividend_ / value);
            remainder_ = value == 0 ? dividend_ : static_cast<std::uint16_t>(dividend_ % value);
            if (remainder_ == 0) product_ = 0;
            break;
        case 0x420B: dma(value); break;
        default:
            if (address >= 0x4300 && address < 0x4380) dma_regs_[address - 0x4300] = value;
            break;
    }
}

// The channels named in `mask`, one after another: a transfer of DAS bytes (0 is 65536) between the A bus ($43x2-$43x4) and the B-bus register $21xx.
void Machine::dma(std::uint8_t mask) {
    static constexpr std::array<std::array<std::uint8_t, 4>, 8> kPattern = {{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 2, 3}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1}}};
    static constexpr std::array<std::uint8_t, 8> kLength = {1, 2, 2, 4, 4, 4, 2, 4};
    for (unsigned ch = 0; ch < 8; ++ch) {
        if (!(mask >> ch & 1)) continue;
        std::uint8_t* r = &dma_regs_[ch * 0x10];
        const std::uint8_t control = r[0], b_register = r[1];
        std::uint16_t a_address = static_cast<std::uint16_t>(r[2] | r[3] << 8);
        const std::uint32_t a_bank = static_cast<std::uint32_t>(r[4]) << 16;
        std::uint32_t count = static_cast<std::uint32_t>(r[5] | r[6] << 8);
        if (count == 0) count = 0x10000;
        const unsigned mode = control & 7;
        const bool to_a = (control & 0x80) != 0, fixed = (control & 0x08) != 0, down = (control & 0x10) != 0;
        for (std::uint32_t done = 0; done < count; ++done) {
            const std::uint16_t b = static_cast<std::uint16_t>(0x2100 + static_cast<std::uint8_t>(b_register + kPattern[mode][done % kLength[mode]]));
            if (to_a) write(a_bank | a_address, read(b));
            else write(b, read(a_bank | a_address));
            if (!fixed) a_address = static_cast<std::uint16_t>(a_address + (down ? -1 : 1));
        }
        r[2] = static_cast<std::uint8_t>(a_address);
        r[3] = static_cast<std::uint8_t>(a_address >> 8);
        r[5] = r[6] = 0;
    }
}

// ---- the run record ------------------------------------------------------------------------------------------------

std::string Fault::text() const { return std::format("{} at {:06x}", what, pc); }

std::string Run::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

}  // namespace cb::emulate::w65816
