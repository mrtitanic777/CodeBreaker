// codebreaker/formats/nes.hpp -- C++17 port of codebreaker.formats.nes.
//
// nes.py -- NES cartridge images (iNES and NES 2.0, `.nes`).
//
//     py::ref<ROM> rom = nes::read(path);
//     rom->prg, rom->chr              the program ROM and the character (tile) ROM
//     rom->mapper, rom->submapper     the cartridge board's number (0 = NROM: no bank switching)
//     rom->mirroring, rom->battery    how the screen's name tables mirror; whether it saves
//     rom->memory()                   the 64 KB the CPU sees at reset (RAM zero, PRG mapped)
//     rom->vectors                    {"nmi", "reset", "irq"}: the three addresses at $FFFA
//     rom->code_at                    the address PRG's first byte sits at
//     rom->functions()                function starts: the vectors, and every JSR target
//
// The file: a 16-byte header ("NES" 1A, PRG size in 16 KB units, CHR size in 8 KB units, two
// flag bytes holding the mapper number's nibbles, mirroring, battery and trainer), an optional
// 512-byte trainer, the PRG, then the CHR. NES 2.0 marks itself in byte 7 (bits 2-3 = 2) and
// widens the mapper number and the sizes.
//
// Where the program sits. On NROM (mapper 0) 32 KB of PRG fill $8000-$FFFF, and 16 KB sit at
// $C000 and repeat at $8000. Other boards switch banks; as most of them do at power-on, the last
// 16 KB bank is taken to be fixed at $C000 and the first at $8000 -- the program's start and its
// vectors are there -- and the other banks are listed by number, their addresses not known
// without running the board.
#pragma once

#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::nes {

extern const py::bytes MAGIC;
extern const py::dict<int64_t, py::str> MIRRORING;

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes header;
    bool nes2 = false;
    int64_t mapper = 0;
    int64_t submapper = 0;
    bool four_screen = false;
    py::str mirroring;
    bool battery = false;
    py::bytes trainer;
    py::bytes prg;
    py::bytes chr;
    py::bytes extra;                 // (anything after: some dumps carry a title)
    py::list<py::bytes> banks;
    int64_t code_at = 0;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // Where in the PRG the byte the CPU reads at `address` is: an offset, or nullopt.
    std::optional<int64_t> mapped(int64_t address) const;
};

bool is_nes(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// The CPU's 64 KB at power-on: PRG mapped (see the module's notes), the rest zero.
py::bytearray _memory(const ROM& rom);

// Where in the PRG the byte the CPU reads at `address` is: an offset, or None.
std::optional<int64_t> _mapped(const ROM& rom, int64_t address);

// Function starts in the fixed address space: the three vectors, then every JSR target
// reached by following the code from them (fall through, branches, JMP, JSR).
py::list<int64_t> functions(const ROM& rom);

}  // namespace codebreaker::formats::nes
