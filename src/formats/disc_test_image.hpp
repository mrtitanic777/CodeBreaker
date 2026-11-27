// formats/disc_test_image.hpp -- small CD images made in a test: an ISO 9660 volume of a few files in the root, cooked (2048 bytes
// a sector), and the same as raw 2352-byte sectors in mode 1 or mode 2 form 1 (the PlayStation's).
//
//     const Bytes cooked = disc_test::iso({{"SYSTEM.CNF", text}, {"SLUS_000.01", exe}});
//     const Bytes raw = disc_test::raw(cooked, 2);
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::disc_test {

inline constexpr std::size_t kSector = 2048;

struct File {
    std::string name;   // in the root, without ";1"
    Bytes data;
};

// A volume: the system area (sectors 0-15), the primary descriptor at 16, the terminator at 17, the root directory at 18, then
// each file from sector 19 on, each starting on a sector of its own.
inline Bytes iso(const std::vector<File>& files) {
    std::size_t sectors = 19;
    for (const File& f : files) sectors += std::max<std::size_t>(1, (f.data.size() + kSector - 1) / kSector);
    Bytes disc(sectors * kSector, 0);
    const auto both32 = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) {
            disc[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
            disc[at + 4 + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
        }
    };
    const auto record = [&](std::size_t at, std::uint32_t lba, std::uint32_t size, bool directory, std::string_view name) {
        const std::size_t length = 33 + name.size() + (name.size() % 2 == 0 ? 1 : 0);
        disc[at] = static_cast<std::uint8_t>(length);
        both32(at + 2, lba);
        both32(at + 10, size);
        disc[at + 25] = directory ? 2 : 0;
        disc[at + 32] = static_cast<std::uint8_t>(name.size());
        std::copy(name.begin(), name.end(), disc.begin() + static_cast<std::ptrdiff_t>(at) + 33);
        return length;
    };
    const std::size_t pvd = 16 * kSector;
    disc[pvd] = 1;
    std::copy_n("CD001", 5, disc.begin() + static_cast<std::ptrdiff_t>(pvd) + 1);
    disc[pvd + 6] = 1;
    both32(pvd + 80, static_cast<std::uint32_t>(sectors));
    record(pvd + 156, 18, kSector, true, std::string(1, '\0'));
    disc[17 * kSector] = 255;
    std::copy_n("CD001", 5, disc.begin() + 17 * static_cast<std::ptrdiff_t>(kSector) + 1);
    disc[17 * kSector + 6] = 1;
    std::size_t at = 18 * kSector;
    at += record(at, 18, kSector, true, std::string(1, '\0'));
    at += record(at, 18, kSector, true, std::string(1, '\1'));
    std::size_t lba = 19;
    for (const File& f : files) {
        at += record(at, static_cast<std::uint32_t>(lba), static_cast<std::uint32_t>(f.data.size()), false, f.name + ";1");
        std::copy(f.data.begin(), f.data.end(), disc.begin() + static_cast<std::ptrdiff_t>(lba * kSector));
        lba += std::max<std::size_t>(1, (f.data.size() + kSector - 1) / kSector);
    }
    return disc;
}

// The same sectors raw: the sync pattern, the sector's address and `mode`, (mode 2) an 8-byte subheader, the data, and zeros where
// the error codes go.
inline Bytes raw(const Bytes& cooked, std::uint8_t mode = 1) {
    Bytes out;
    for (std::size_t s = 0; s * kSector < cooked.size(); ++s) {
        const std::array<std::uint8_t, 16> header = {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 2, static_cast<std::uint8_t>(s), mode};
        out.insert(out.end(), header.begin(), header.end());
        if (mode == 2) out.insert(out.end(), {0, 0, 8, 0, 0, 0, 8, 0});   // the subheader, twice: form 1, data
        out.insert(out.end(), cooked.begin() + static_cast<std::ptrdiff_t>(s * kSector), cooked.begin() + static_cast<std::ptrdiff_t>((s + 1) * kSector));
        out.insert(out.end(), 2352 - 16 - (mode == 2 ? 8 : 0) - kSector, 0);
    }
    return out;
}

inline Bytes bytes_of(std::string_view text) { return Bytes(text.begin(), text.end()); }

}  // namespace cb::formats::disc_test
