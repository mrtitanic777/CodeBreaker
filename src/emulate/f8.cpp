// cb-unity: alone
#include "emulate/f8.hpp"

#include "core/error.hpp"

namespace cb::emulate::f8 {

void Bus::load(std::uint16_t address, ByteView bytes, bool writable) {
    if (static_cast<std::size_t>(address) + bytes.size() > memory_.size()) throw UsageError("an F8 program is at most 64 KB");
    std::copy(bytes.begin(), bytes.end(), memory_.begin() + address);
    if (writable) make_writable(address, static_cast<std::uint16_t>(address + bytes.size() - 1));
}

void Bus::make_writable(std::uint16_t lo, std::uint16_t hi) {
    for (std::uint32_t a = lo; a <= hi; ++a) writable_[a] = true;
}

std::uint8_t Bus::port_in(std::uint8_t port) {
    const std::uint8_t v = in_ ? in_(port) : 0;
    log_.push_back({PortAccess::Kind::In, port, v});
    return v;
}

void Bus::port_out(std::uint8_t port, std::uint8_t value) {
    log_.push_back({PortAccess::Kind::Out, port, value});
    if (out_) out_(port, value);
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::f8
