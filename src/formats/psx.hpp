// formats/psx.hpp -- PlayStation executables: the PS-X EXE (`.exe`, `.psx`, or no extension, as a disc's SLUS_000.00 is named).
//
// A PS-X EXE is a 2048-byte header and then the program, which the BIOS copies to `t_addr` in RAM and starts at `pc0`:
//
//     $000  "PS-X EXE"                      $008  8 zero bytes
//     $010  pc0     the first instruction    $014  gp0   the value $gp starts with
//     $018  t_addr  where the text is loaded $01C  t_size  its length (a multiple of 2048)
//     $020  data address and size (unused)   $028  bss (memfill) address and size
//     $030  the stack: base and offset (the BIOS sets $sp to base + offset; 0 leaves the BIOS's own)
//     $038  20 bytes of registers the BIOS saves (zero in a file)
//     $04C  the region marker, text: "Sony Computer Entertainment Inc. for North America area", zeros to $800
//
// The processor is the R3000A (little-endian MIPS I; decompile/mips). Memory: 2 MB of RAM, seen at $00000000, $80000000 (KSEG0, where
// programs are linked: $80010000 is the first address past the kernel) and $A0000000 (KSEG1, uncached). The program is the bytes
// after the header: when the file is longer than the header and `t_size` say, the extra bytes are part of what the program
// carries, and are kept.
//
// A disc (an image of it: `.bin` + `.cue`, `.iso`, `.img`; formats/disc_image) holds the program in its ISO 9660 volume. The BIOS
// reads SYSTEM.CNF from the root, whose line `BOOT = cdrom:\SLUS_007.55;1` names the program (with arguments after it, sometimes),
// and with no SYSTEM.CNF boots PSX.EXE. `read` and `probe` take a disc as they take a file: the program is the one the disc boots.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"
#include "formats/disc_image.hpp"

namespace cb::formats::psx {

inline constexpr std::size_t kHeaderSize = 0x800;
inline constexpr std::string_view kMagic = "PS-X EXE";
inline constexpr std::size_t kRamSize = 0x200000;
inline constexpr std::uint32_t kRamMask = 0x001FFFFF;
inline constexpr std::array<std::string_view, 3> kExtensions = {".exe", ".psx", ".ps-exe"};

// The header's fields.
struct Header {
    std::uint32_t pc0 = 0;
    std::uint32_t gp0 = 0;
    std::uint32_t t_addr = 0;
    std::uint32_t t_size = 0;
    std::uint32_t data_addr = 0, data_size = 0;
    std::uint32_t bss_addr = 0, bss_size = 0;
    std::uint32_t sp_base = 0, sp_offset = 0;
    std::string marker;                 // the region marker at $4C
};

struct Exe {
    Bytes data;                         // the whole file
    Header header;
    // From a disc: the program's path on it ("/SLUS_007.55") and SYSTEM.CNF's BOOT value ("cdrom:\SLUS_007.55;1"; empty when
    // the disc has no SYSTEM.CNF and PSX.EXE was booted). Both empty for a program read from its own file.
    std::string boot_file;
    std::string boot;

    // The program: the bytes after the header.
    ByteView text() const { return ByteView(data).subspan(kHeaderSize); }
    // The first address past the program's bytes.
    std::uint32_t text_end() const { return header.t_addr + static_cast<std::uint32_t>(data.size() - kHeaderSize); }
    // The address bits above the RAM mask ($80000000 for a program linked in KSEG0).
    std::uint32_t region() const { return header.t_addr & ~kRamMask; }

    // The RAM as the program finds it: 2 MB, the text at `t_addr`.
    Bytes memory() const;

    // Function starts: the entry point and every JAL target reached by following the code from it (the part of the
    // address space that holds the text, seen through the RAM mask; the addresses come back as the program writes them).
    // `decode` is the MIPS decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the file has no "PS-X EXE" magic or is shorter than its header.
Exe parse(ByteView data);
// A PS-X EXE, or the program a disc image boots.
Exe read(const std::filesystem::path& path);

// The file a SYSTEM.CNF's `key` line names, as a path in the volume ("BOOT = cdrom:\SLUS_007.55;1" -> "/SLUS_007.55"):
// the device, the ";1" version and any arguments after the file are not part of it. nullopt when there is no such line.
// `value`: the line's value as written. (The PlayStation 2's key is BOOT2; a BOOT2 line is not a BOOT line.)
std::optional<std::string> boot_file(std::string_view system_cnf, std::string_view key = "BOOT", std::string* value = nullptr);

// The program a disc boots: SYSTEM.CNF's BOOT file, else PSX.EXE. FormatError when the image has no ISO 9660 volume, no program
// to boot, or one that is not a PS-X EXE.
Exe parse_disc(const disc::Sectors& sectors);

// "PS-X EXE" at the start of the file, or a disc image that boots one.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::psx
