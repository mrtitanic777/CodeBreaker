#include "source/iso_disc.hpp"

#include <algorithm>

namespace cb::source::iso_disc {

Bytes minimal(ByteView system_area, ByteView program, const std::string& name, std::string_view system_id, std::uint32_t volume_base) {
    constexpr std::size_t kS = kSector;
    constexpr std::uint32_t kRootSector = 17, kProgramSector = 18;
    Bytes disc((kProgramSector + (program.size() + kS - 1) / kS) * kS, 0);
    std::copy_n(system_area.begin(), std::min(system_area.size(), std::size_t{16} * kS), disc.begin());
    const auto both32 = [&](std::size_t at, std::uint32_t v) {   // a 32-bit field written little-endian and then big-endian
        for (std::size_t i = 0; i < 4; ++i) {
            disc[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
            disc[at + 4 + i] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
        }
    };
    const auto record = [&](std::size_t at, std::uint32_t sector, std::uint32_t size, bool directory, const std::string& id) {
        const std::size_t length = 33 + id.size() + (id.size() % 2 == 0 ? 1 : 0);
        disc[at] = static_cast<std::uint8_t>(length);
        both32(at + 2, sector);
        both32(at + 10, size);
        disc[at + 25] = directory ? 2 : 0;
        disc[at + 28] = 1;   // the volume sequence number, both-endian
        disc[at + 31] = 1;
        disc[at + 32] = static_cast<std::uint8_t>(id.size());
        std::copy(id.begin(), id.end(), disc.begin() + static_cast<std::ptrdiff_t>(at + 33));
        return length;
    };
    const std::size_t pvd = 16 * kS;
    disc[pvd] = 1;
    std::copy_n("CD001", 5, disc.begin() + static_cast<std::ptrdiff_t>(pvd + 1));
    disc[pvd + 6] = 1;
    const auto text_at = [&](std::size_t at, std::string_view s, std::size_t width) {
        for (std::size_t i = 0; i < width; ++i) disc[at + i] = i < s.size() ? static_cast<std::uint8_t>(s[i]) : ' ';
    };
    text_at(pvd + 8, system_id, 32);
    text_at(pvd + 40, "CODEBREAKER", 32);
    both32(pvd + 80, static_cast<std::uint32_t>(disc.size() / kS));
    disc[pvd + 128] = 0;   // the logical block size: 2048, both-endian
    disc[pvd + 129] = 8;
    disc[pvd + 130] = 8;
    disc[pvd + 131] = 0;
    record(pvd + 156, kRootSector + volume_base, kS, true, std::string(1, '\0'));
    std::size_t at = kRootSector * kS;
    at += record(at, kRootSector + volume_base, kS, true, std::string(1, '\0'));
    at += record(at, kRootSector + volume_base, kS, true, std::string(1, '\1'));
    record(at, kProgramSector + volume_base, static_cast<std::uint32_t>(program.size()), false, name + ";1");
    std::copy(program.begin(), program.end(), disc.begin() + static_cast<std::ptrdiff_t>(kProgramSector * kS));
    return disc;
}

}  // namespace cb::source::iso_disc
