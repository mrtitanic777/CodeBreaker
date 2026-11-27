// cb-unity: alone
#include "emulate/i8048.hpp"

#include "core/error.hpp"

namespace cb::emulate::i8048 {

// ---- the bus --------------------------------------------------------------------------------------------------------

void Bus::load(std::uint16_t address, ByteView bytes) {
    if (static_cast<std::size_t>(address) + bytes.size() > program_.size()) throw UsageError("an 8048 program is at most 4 KB");
    std::copy(bytes.begin(), bytes.end(), program_.begin() + address);
}

std::uint8_t Bus::read(Place place, std::uint8_t address) {
    const std::uint8_t v = read_ ? read_(place, address) : 0;
    log_.push_back({DeviceAccess::Kind::Read, place, address, v});
    return v;
}

void Bus::write(Place place, std::uint8_t value, std::uint8_t address) {
    log_.push_back({DeviceAccess::Kind::Write, place, address, value});
    if (write_) write_(place, address, value);
}

// ---- the run record -------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::i8048
