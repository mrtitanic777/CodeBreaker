// formats/ppc_program.hpp -- a Nintendo PowerPC program of any of the three formats (a DOL, an ELF, a Wii U RPX), as the console lanes use
// it: which console it is for, where it starts, and the file as a run of regions that tile it (the header, the code, the data, the rest).
//
//     std::optional<Program> p = ppc_program::parse(bytes);      // nullopt when the bytes are none of the three
//     p->console                 GameCube, Wii or WiiU
//     p->entry                   where the program starts
//     p->regions                 [offset, offset + size) of the file, in order, covering all of it, each with the address it is loaded at
//
// The consoles: a DOL or ELF that touches the Wii's hardware or its second memory is a Wii program, any other the GameCube's; an RPX or RPL is
// a Wii U program. All three run PowerPC 750 instructions (decompile/ppc): the Gekko, the Broadway and the Espresso.
//
// A Wii U RPX is read decompressed (formats/ppc_elf): `file` is the decompressed file, which is what the source writer rebuilds.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/dol.hpp"
#include "formats/ppc_elf.hpp"

namespace cb::formats::ppc_program {

enum class Console { GameCube, Wii, WiiU };
enum class Kind { Dol, Elf, Rpx };

std::string_view console_text(Console console);   // "GameCube", "Wii", "Wii U"
std::string_view lane_name(Console console);      // "gamecube", "wii", "wiiu"
std::string_view cpu_text(Console console);       // "Gekko", "Broadway", "Espresso"
std::string_view kind_text(Kind kind);            // "DOL", "ELF", "RPX"

// What a stretch of the file is.
enum class Role {
    Header,   // a DOL's header, an ELF's header and its program and section header tables
    Code,     // instructions: a DOL's text sections, an ELF's executable sections
    Data,     // loaded data
    Other,    // bytes no section of the program is loaded from (padding, symbols, strings, debugging information)
};

struct Region {
    std::size_t offset = 0, size = 0;   // where it is in the file
    std::uint32_t address = 0;          // where it is loaded (Header and Other: the file offset, as the source's address)
    Role role = Role::Other;
    std::string name;                   // "text0", "data3", ".text", "elf header", "section headers", "gap"
};

struct Program {
    Kind kind = Kind::Dol;
    Console console = Console::GameCube;
    Bytes file;                                       // the program as a file (a decompressed RPX)
    std::uint32_t entry = 0;
    std::vector<ppc_segment::Segment> segments;       // what is loaded
    std::vector<Region> regions;                      // the file, tiled
    std::optional<dol::Dol> dol;                      // the headers, for the kind that has them
    std::optional<ppc_elf::Elf> elf;

    // The code regions and the loaded data, as (address, bytes) for the interpreter.
    struct Piece {
        std::uint32_t address = 0;
        ByteView bytes;
    };
    std::vector<Piece> loaded() const;
    // The first code region that holds `address`, or nullptr.
    const Region* code_at(std::uint32_t address) const;
};

// The program the bytes are, or nullopt when they are none of the three formats.
std::optional<Program> parse(ByteView bytes);
std::optional<Program> read(const std::filesystem::path& path);

// Is the file a program of this console? (Parses the file: the console is decided by what the code touches.)
bool probe(const std::filesystem::path& path, Console console);

}  // namespace cb::formats::ppc_program
