// formats/cpc.hpp -- Amstrad CPC programs: disk images (`.dsk`), snapshots (`.sna`) and AMSDOS binary files (`.bin`).
//
// An AMSDOS file is a 128-byte header (user, name, type, load address, length, execution address, a checksum of the first 67 bytes) and the
// data. A binary (type 2, or 3 when protected) is what the program is: loaded at its load address and started at its execution address. A
// disk (CPCEMU "MV - CPC" or "EXTENDED CPC DSK") holds files in the AMSDOS directory (64 entries of 32 bytes in the first four sectors after
// the reserved tracks, 1 KB blocks, 128-record extents); the program taken from it is its best binary: the largest whose execution address
// lies inside it. A snapshot ("MV - SNA") is the machine stopped: the registers, and 64 KB of RAM; the program block taken from it is RAM from
// $0040 up to the firmware's own workspace ($A700). The RAM under the firmware (the jumpblock at $B900-$BDFF) is not part of the program.
//
// Tapes (`.cdt`, `.tzx`) and the chunked snapshots (version 3) are not read.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::cpc {

inline constexpr std::array<std::string_view, 3> kExtensions = {".dsk", ".sna", ".bin"};
inline constexpr std::size_t kMemorySize = 0x10000;
inline constexpr std::uint32_t kSnapshotStart = 0x0040;
inline constexpr std::uint32_t kSnapshotEnd = 0xA700;

enum class Kind { Binary, Disk, Snapshot };

// The 128-byte AMSDOS header.
struct AmsdosHeader {
    std::string name;                // "NAME.EXT"
    std::uint8_t type = 0;           // 0 BASIC, 1 protected BASIC, 2 binary, 3 protected binary
    std::uint16_t load = 0;
    std::uint16_t length = 0;
    std::uint16_t exec = 0;
    std::uint32_t logical_length = 0;
};

// A file found on a disk.
struct DiskFile {
    std::string name;                // "NAME.EXT" (user 0 only)
    Bytes data;                      // header included, if it has one
    std::optional<AmsdosHeader> header;
};

struct Registers {
    std::uint8_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0, i = 0, r = 0, im = 0;
    std::uint16_t ix = 0, iy = 0, sp = 0, pc = 0;
};

struct Entry {
    std::uint16_t address = 0;
    std::string name;                // "start"
};

struct Program {
    Kind kind = Kind::Binary;
    std::string name;                // the binary's name (a disk's: the file taken from it)
    std::optional<Registers> registers;   // a snapshot's
    Bytes memory;                    // the 64 KB address space around the program (a snapshot's RAM; zeros otherwise)
    std::uint32_t origin = 0;        // where the program block starts
    Bytes image;                     // the program block
    std::vector<Entry> entries;
    std::vector<std::string> files;  // a disk's files ("NAME.EXT type load length exec")
};

// The AMSDOS header at the start of `data`, if it is one (its checksum holds).
std::optional<AmsdosHeader> amsdos_header(ByteView data);

// The user-0 files of a disk image. FormatError / TruncatedError for a damaged image.
std::vector<DiskFile> disk_files(ByteView data);

// FormatError if the data is not a file of the format. `extension` (".dsk", ".sna", ".bin") says which.
Program parse(ByteView data, std::string_view extension);
Program read(const std::filesystem::path& path);

// A `.dsk` with a CPCEMU header holding a binary with an AMSDOS header, a `.sna` with its header, or a `.bin` with a valid AMSDOS header.
bool probe(const std::filesystem::path& path);

// The firmware's jumpblock ($B900-$BDFF), by the names the Firmware Guide gives.
const std::map<std::uint32_t, std::string>& firmware_names();

}  // namespace cb::formats::cpc
