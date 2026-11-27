#include "emulate/huc6280.hpp"

#include <bit>
#include <format>

#include "formats/pce.hpp"

namespace cb::emulate::huc6280 {

namespace {

bool is_ram_bank(std::uint8_t bank) { return bank >= 0xF8 && bank <= 0xFB; }

}  // namespace

std::string Access::text() const {
    std::string name = formats::pce::register_name(offset, write);
    if (vdc != 0xFF) name += "(" + formats::pce::vdc_register_name(vdc) + ")";
    return std::format("{} {} {:02x}", write ? "w" : "r", name, value);
}

Machine::Machine(ByteView rom) : rom_(rom) { mpr = {0xFF, 0xF8, 2, 3, 4, 5, 6, 0}; }

std::uint8_t Machine::read(std::uint16_t logical) {
    const std::uint8_t bank = mpr[logical >> 13];
    const std::uint16_t offset = logical & 0x1FFF;
    if (bank == formats::pce::kHardwareBank) return hardware_read(offset);
    if (is_ram_bank(bank)) return ram[offset];
    if (bank > 0x7F) return 0xFF;
    const std::size_t count = rom_.size() / kBankSize;
    if (count == 0) return 0xFF;
    std::size_t index = bank;
    if (std::has_single_bit(count)) index &= count - 1;
    else if (index >= count) return 0xFF;
    return rom_[index * kBankSize + offset];
}

void Machine::write(std::uint16_t logical, std::uint8_t value) {
    const std::uint8_t bank = mpr[logical >> 13];
    const std::uint16_t offset = logical & 0x1FFF;
    if (bank == formats::pce::kHardwareBank) hardware_write(offset, value);
    else if (is_ram_bank(bank)) ram[offset] = value;
}

std::uint8_t Machine::hardware_read(std::uint16_t offset) {
    std::uint8_t value = 0;
    std::uint8_t vdc = 0xFF;
    if (offset < 0x0400) {
        const unsigned reg = offset & 3;
        if (reg == 0) value = (vdc_status_reads_++ & 1) ? 0x20 : 0x00;   // the vertical-blank bit, on every other read
        else if (reg >= 2) vdc = vdc_select_;
    } else if (offset >= 0x1000 && offset < 0x1400) {
        value = 0xFF;   // no button pressed
    }
    log_.push_back({false, offset, value, vdc});
    return value;
}

void Machine::hardware_write(std::uint16_t offset, std::uint8_t value) {
    std::uint8_t vdc = 0xFF;
    if (offset < 0x0400) {
        const unsigned reg = offset & 3;
        if (reg == 0) vdc_select_ = value & 0x1F;
        else if (reg >= 2) vdc = vdc_select_;
    }
    log_.push_back({true, offset, value, vdc});
}

std::string Fault::text() const { return std::format("no instruction for opcode {:02x} at {:04x}", opcode, pc); }

std::string Run::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

}  // namespace cb::emulate::huc6280
