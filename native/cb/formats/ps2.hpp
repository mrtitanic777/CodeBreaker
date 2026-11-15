// cb/formats/ps2.hpp -- PlayStation 2 programs: an Emotion Engine executable (`.elf`, what ps2sdk's `ee-gcc` links and the homebrew loaders run) and a
// disc image (`.iso`) whose SYSTEM.CNF names one as its BOOT2 program.
//
// A program is a 32-bit little-endian MIPS ELF of machine 8 whose flags say R5900 ($00920000, formats/mips_elf). It loads at $00100000 (KUSEG, where the
// kernel maps the first 32 MB of RAM) and starts at the ELF's entry point. A disc is read as an ISO 9660 volume (formats/iso9660); SYSTEM.CNF holds
// `BOOT2 = cdrom0:\SLUS_203.14;1`, the program to run. The programs on a disc are the executables of a game; the console's own system software and
// the IOP's (its I/O processor's) modules are other files and stay data.
//
// The machine the program runs on: the EE's 32 MB of RAM, the 16 KB scratchpad at $70000000, the IOP's RAM and the BIOS, and the hardware registers
// of the Emotion Engine's units (the timers, IPU, GIF, VIF0 and VIF1, the DMA controller, the interrupt controller and the Graphics Synthesizer's
// privileged registers); `hardware_name` says which register an address is. The kernel's system calls are named by `syscall_name`.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/formats/mips_elf.hpp"

namespace cb::formats::ps2 {

inline constexpr std::array<std::string_view, 2> kExtensions = {".elf", ".iso"};
inline constexpr std::uint32_t kRamSize = 0x02000000;
inline constexpr std::uint32_t kScratchpad = 0x70000000;
inline constexpr std::uint32_t kScratchpadSize = 0x4000;

enum class Kind : std::uint8_t { Elf, Disc };

struct Program {
    Kind kind = Kind::Elf;
    mips_elf::Elf elf;          // the executable (a disc's BOOT2 file)
    std::string boot;           // a disc: SYSTEM.CNF's BOOT2 value ("cdrom0:\SLUS_203.14;1")
    std::string boot_file;      // ... and its path in the volume ("/SLUS_203.14")
    const Bytes& file() const { return elf.data; }
};

// An R5900 ELF: its bytes parsed (FormatError for anything else).
Program parse_elf(ByteView data);
// A file as a PlayStation 2 program: an R5900 ELF, or an ISO whose BOOT2 file is one. nullopt for anything else (an unreadable file too).
std::optional<Program> read(const std::filesystem::path& path);
bool probe(const std::filesystem::path& path);

// The BOOT2 file named by SYSTEM.CNF's text, as a path in the volume ("/SLUS_203.14"), or nothing.
std::optional<std::string> boot_file(std::string_view system_cnf, std::string* boot_value = nullptr);

// The name of the hardware register at a virtual address (any of its views: $1000F000, $B000F000), or nothing. Registers are named by the
// manuals' names (T0_COUNT, GIF_STAT, D2_CHCR, I_STAT, GS_CSR ...).
std::optional<std::string> hardware_name(std::uint32_t address);
// The physical address of a virtual one in the registers' space ($10000000-$1FFFFFFF), or nothing.
std::optional<std::uint32_t> register_address(std::uint32_t address);

// The EE kernel's system call of that number (the value in $v1; a negative number is the interrupt-context variant), or "syscall_<n>".
std::string syscall_name(std::int32_t number);

}  // namespace cb::formats::ps2
