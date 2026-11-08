// codebreaker/formats/gb.hpp -- C++17 port of codebreaker.formats.gb.
//
// gb.py -- Game Boy and Game Boy Color cartridge images (`.gb`, `.gbc`).
//
//     py::ref<ROM> rom = gb::read(path);
//     rom->data, rom->banks           the ROM and its 16 KB banks
//     rom->title, rom->cgb, rom->sgb  the header's title; Color (only / also) and Super Game Boy support
//     rom->mbc                        the cartridge type's memory bank controller ("ROM", "MBC1", "MBC3", "MBC5", ...)
//     rom->memory()                   the 64 KB the CPU sees at power-on: bank 0 at $0000, bank 1 at $4000
//     rom->vectors                    {"entry": $0100, "vblank": $40, "stat": $48, "timer": $50, "serial": $58, "joypad": $60}
//     rom->functions()                function starts: the entry, the interrupt vectors in use, every CALL/RST target
//
// The cartridge header is at $0100-$014F: the entry (usually NOP / JP), Nintendo's logo (the boot
// ROM checks it), the title, the Color and Super Game Boy flags, the cartridge type (which bank
// controller, RAM, battery), the ROM and RAM sizes, the header checksum (the boot ROM checks it)
// and the global checksum (nothing checks it). Bank 0 is always at $0000-$3FFF; the controller
// switches the bank at $4000-$7FFF, which holds bank 1 at power-on -- other banks are listed by
// number, their addresses known only by running the program.
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::gb {

constexpr int64_t BANK = 0x4000;
extern const std::tuple<py::str, py::str> EXTENSIONS;
extern const py::bytes LOGO;
extern const py::dict<int64_t, py::str> MBC;

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes data;
    bool logo_ok = false;
    py::str cgb;                       // "", "also" or "only"
    py::str title;
    bool sgb = false;
    py::str mbc;
    int64_t header_checksum = 0;
    bool header_checksum_ok = false;
    py::list<py::bytes> banks;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
};

bool is_gb(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// Function starts in the power-on address space ($0000-$7FFF): the entry at $0100, each
// interrupt vector that holds code (unused ones are commonly $FF or a lone RETI), then every
// CALL and RST target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

}  // namespace codebreaker::formats::gb
