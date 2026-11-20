// formats/psp.hpp -- PSP programs, unencrypted: an executable ELF (`.elf`), a relocatable module (`.prx`) and an `EBOOT.PBP` (the package a
// homebrew program is launched from) whose DATA.PSP is one of those.
//
// An ELF is a 32-bit little-endian MIPS file of machine 8 with the Allegrex's flags ($00A20000 in e_flags, formats/mips_elf); an executable has the type
// 2 and is linked at $08804000 (the user area's start), a module the type $FFA0 and is linked at 0 and relocated by the loader. Its imports are 8-byte
// stubs (`jr $ra`, then a SYSCALL the loader fills in) listed by library with the NID (the first four bytes of the SHA-1 of the function's name) of each:
// `imports` reads them, and `nid_name` names the NIDs of the functions of pspsdk's published lists. A program's own functions are exported the same
// way (`exports`).
//
// An EBOOT.PBP is a 40-byte header (`\0PBP`, a version, and the offsets of PARAM.SFO, ICON0.PNG, ICON1.PMF, PIC0.PNG, PIC1.PNG, SND0.AT3, DATA.PSP and
// DATA.PSAR), then those files. Games and the system's own modules are `~PSP` files, encrypted: a program whose module starts "~PSP" is recognised
// (`encrypted`) and not handled; `read` gives nothing for it.
//
// The machine: 32 MB of RAM at $08000000 (kernel and user, seen again at $88000000 and $A8000000), the 16 KB scratchpad at $00010000, the 2 MB of video
// memory at $04000000, and the hardware's blocks (`hardware_name`).
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/mips_elf.hpp"

namespace cb::formats::psp {

inline constexpr std::array<std::string_view, 3> kExtensions = {".elf", ".prx", ".pbp"};
inline constexpr std::uint32_t kUserBase = 0x08800000;
inline constexpr std::uint32_t kRamBase = 0x08000000;
inline constexpr std::uint32_t kRamSize = 0x02000000;
inline constexpr std::uint32_t kScratchpad = 0x00010000;
inline constexpr std::uint32_t kScratchpadSize = 0x4000;
inline constexpr std::uint32_t kVram = 0x04000000;
inline constexpr std::uint32_t kVramSize = 0x00200000;
inline constexpr std::size_t kPbpHeaderSize = 40;

enum class Kind : std::uint8_t { Elf, Prx, Pbp };

struct Program {
    Kind kind = Kind::Elf;
    Bytes file;                      // the whole file (a PBP: the package)
    mips_elf::Elf elf;               // the module (a PBP: its DATA.PSP)
    std::size_t elf_offset = 0;      // where the module starts in `file`
    std::string title;               // a PBP: PARAM.SFO's TITLE
    bool prx() const { return elf.prx(); }
};

// One imported function or variable: its library, NID, the address of its stub (or variable slot), and its name when the NID is a known one.
struct Import {
    std::string library;
    std::uint32_t nid = 0;
    std::uint32_t stub = 0;
    bool variable = false;
    std::string name;
};
// One exported function or variable (library empty: the module's own system exports, module_start and module_info among them).
struct Export {
    std::string library;
    std::uint32_t nid = 0;
    std::uint32_t address = 0;
    bool variable = false;
    std::string name;
};

// What the module says about itself (.rodata.sceModuleInfo): its attributes, name, global pointer and the ranges of its export and import tables.
struct ModuleInfo {
    bool found = false;
    std::uint32_t attributes = 0;
    std::string name;
    std::uint32_t gp = 0, ent_top = 0, ent_end = 0, stub_top = 0, stub_end = 0;
};

// A file as an unencrypted PSP program; nullopt for anything else (an encrypted module, a file that is none).
std::optional<Program> read(const std::filesystem::path& path);
bool probe(const std::filesystem::path& path);
// A `~PSP` module, bare or in a PBP: recognised, and not handled.
bool encrypted(const std::filesystem::path& path);

// The PBP's parts: offsets of its eight files, in the header's order (PARAM.SFO ... DATA.PSAR), and its size.
std::optional<std::array<std::uint32_t, 8>> pbp_offsets(ByteView file);
// A value of a PARAM.SFO (a string or an integer as text).
std::optional<std::string> sfo_value(ByteView sfo, std::string_view key);

ModuleInfo module_info(const mips_elf::Elf& elf);
std::vector<Import> imports(const mips_elf::Elf& elf);
std::vector<Export> exports(const mips_elf::Elf& elf);

// The function a NID names in pspsdk's lists ("sceDisplaySetMode"), or an empty view.
std::string_view nid_name(std::uint32_t nid);
// The NID of a function name (the first four bytes of its SHA-1, little-endian).
std::uint32_t nid_of(std::string_view name);

// The hardware block an address lies in ("GE", "SYSREG", "INTC" ...), or nothing; any view (kernel, uncached) of the address counts.
std::optional<std::string> hardware_name(std::uint32_t address);
// The physical address of a virtual one in the hardware's space ($1C000000-$1FFFFFFF), or nothing.
std::optional<std::uint32_t> register_address(std::uint32_t address);

}  // namespace cb::formats::psp
