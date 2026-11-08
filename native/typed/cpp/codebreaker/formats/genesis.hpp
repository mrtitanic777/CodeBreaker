// codebreaker/formats/genesis.hpp -- C++17 port of codebreaker.formats.genesis.
//
// genesis.py -- Sega Genesis / Mega Drive cartridge images (`.md`, `.bin`, `.gen`, `.smd`).
//
//     py::ref<ROM> rom = genesis::read(path);
//     rom->data                       the ROM as the 68000 sees it from address 0 (an .smd dump de-interleaved)
//     rom->header                     the header's fields at $100: system, copyright, names, serial, checksum,
//                                     devices, ROM and RAM ranges, region
//     rom->vectors                    {"ssp", "reset", and each exception vector that points into the ROM}
//     rom->memory()                   the ROM placed in a 16 MB address space
//     rom->functions()                function starts: the reset vector, the exception and interrupt handlers,
//                                     every JSR/BSR target reached from them
//     rom->checksum()                 the checksum the header should hold (the sum of the words from $200)
//
// The 68000 starts by reading two longs at address 0: the stack pointer and the program's start.
// Then come the exception vectors (bus error, address error, illegal instruction, ...) and the
// interrupt autovectors -- on the Genesis level 4 is the horizontal blank ($70) and level 6 the
// vertical blank ($78). The header at $100 starts "SEGA" (the TMSS hardware of later consoles
// refuses to start a cartridge without it). There is no bank switching below 4 MB: the ROM is
// simply at $000000-$3FFFFF. An .smd dump (a copier's 512-byte header, then 16 KB blocks stored
// odd bytes first) is turned into that plain image.
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::genesis {

extern const std::tuple<py::str, py::str, py::str, py::str> EXTENSIONS;
// {vector number: name}
extern const py::dict<int64_t, py::str> VECTOR_NAMES;

// ROM.header: a dict with ten fixed keys (a record); the keys "domestic name" and "overseas name" have a space.
struct Header {
    py::str system;
    py::str copyright;
    py::str domestic_name;
    py::str overseas_name;
    py::str serial;
    int64_t checksum = 0;
    py::str devices;
    std::tuple<int64_t, int64_t> rom;
    std::tuple<int64_t, int64_t> ram;
    py::str region;
};
template <class F> void for_each_field(const Header& r, F&& f) {
    f("system", r.system);
    f("copyright", r.copyright);
    f("domestic name", r.domestic_name);
    f("overseas name", r.overseas_name);
    f("serial", r.serial);
    f("checksum", r.checksum);
    f("devices", r.devices);
    f("rom", r.rom);
    f("ram", r.ram);
    f("region", r.region);
}
template <class F> void for_each_field_mut(Header& r, F&& f) {
    f("system", r.system, true);
    f("copyright", r.copyright, true);
    f("domestic name", r.domestic_name, true);
    f("overseas name", r.overseas_name, true);
    f("serial", r.serial, true);
    f("checksum", r.checksum, true);
    f("devices", r.devices, true);
    f("rom", r.rom, true);
    f("ram", r.ram, true);
    f("region", r.region, true);
}

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    bool smd = false;
    py::bytes data;
    Header header;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // the checksum the header should hold: the sum of the words from $200, mod 2**16
    int64_t checksum() const;
};

bool is_genesis(const py::str& path);
py::ref<ROM> read(const py::str& path);

// An .smd image: a 512-byte header, then 16 KB blocks holding each block's odd bytes first.
py::bytes _desmd(const py::bytes& data);

// ValueError if the image is too short for a cartridge.
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// Function starts: the reset vector and the handlers the vectors name, then every JSR and BSR
// target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

}  // namespace codebreaker::formats::genesis
