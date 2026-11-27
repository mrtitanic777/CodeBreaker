#include "emulate/lynx.hpp"

#include <algorithm>

namespace cb::emulate::lynx {

// What the devices answer a read (see the header).
std::uint8_t device_read(std::uint16_t address) {
    if (address == 0xFD81) return 0xFF;                                          // INTSET: every interrupt pending
    if (address == 0xFD8C) return 0xA0;                                          // SERCTL: ready to send, send register empty
    if (address >= 0xFD00 && address < 0xFD20 && (address & 3) == 3) return 0x08;   // a timer's CTLB: timer done
    return 0;
}

m6502::Bus make_bus(ByteView image) {
    m6502::Bus bus(image.first(std::min<std::size_t>(image.size(), 0x10000)));
    bus.map(0xFC00, 0xFDFF, [](std::uint16_t a) { return device_read(a); });   // Suzy and Mikey: logged
    bus.map(0xFFF9, 0xFFF9);                                                   // the memory map control: logged
    return bus;
}

}  // namespace cb::emulate::lynx
