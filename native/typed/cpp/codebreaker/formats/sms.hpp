// codebreaker/formats/sms.hpp -- C++17 port of codebreaker.formats.sms.
//
// sms.py -- Sega Master System, Game Gear and SG-1000 cartridge images (`.sms`, `.gg`, `.sg`).
//
//     py::ref<ROM> rom = sms::read(path);
//     rom->data                   the ROM (a copier's 512-byte header, when there is one, left out)
//     rom->system                 "Master System" / "Game Gear" / "SG-1000"
//     rom->header                 the "TMR SEGA" header's fields, or nullopt (SG-1000 and some homebrew have none)
//     rom->pages                  the 16 KB pages
//     rom->memory()               the 64 KB the Z80 sees at power-on: pages 0-2 in slots 0-2, RAM zero
//     rom->vectors                {"reset": 0x0000, "irq": 0x0038, "nmi": 0x0066}
//     rom->functions()            function starts: the vectors and every CALL/RST target reached from them
//
// A cartridge image is the ROM as it is, nothing in front (a 512-byte copier header is recognised
// by the size and dropped). The Master System BIOS checks a header at $7FF0 (or $3FF0 / $1FF0 for
// small ROMs): "TMR SEGA", two reserved bytes, the checksum, the product code and version, the
// region and the ROM size. The Game Gear's is the same with regions 5-7.
//
// The Z80 sees the ROM through Sega's mapper: three 16 KB slots at $0000, $4000 and $8000, each
// showing the page written to $FFFD / $FFFE / $FFFF (the first 1 KB always page 0, where the
// vectors are); RAM is 8 KB at $C000, repeated at $E000. At power-on the slots hold pages 0, 1
// and 2, which is what memory() maps; other pages are listed by number, their addresses known only
// by running the program (it writes the mapper registers itself). SG-1000 cartridges have no mapper.
#pragma once

#include <array>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::sms {

constexpr int64_t PAGE = 0x4000;
extern const std::array<int64_t, 3> _HEADER_AT;
// {region nibble: (system, region name)}
extern const py::dict<int64_t, std::tuple<py::str, py::str>> REGIONS;
// {size nibble: ROM size in bytes}
extern const py::dict<int64_t, int64_t> _SIZES;
extern const std::tuple<py::str, py::str, py::str> EXTENSIONS;

// ROM.header: a dict with six fixed keys (a record)
struct Header {
    int64_t at = 0;
    int64_t checksum = 0;
    py::str product;
    int64_t version = 0;
    py::str region;
    std::optional<int64_t> size;   // _SIZES.get(size): None for a size code that is not listed
};
template <class F> void for_each_field(const Header& r, F&& f) {
    f("at", r.at);
    f("checksum", r.checksum);
    f("product", r.product);
    f("version", r.version);
    f("region", r.region);
    f("size", r.size);
}
template <class F> void for_each_field_mut(Header& r, F&& f) {
    f("at", r.at, true);
    f("checksum", r.checksum, true);
    f("product", r.product, true);
    f("version", r.version, true);
    f("region", r.region, true);
    f("size", r.size, true);
}

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes data;
    std::optional<Header> header;
    py::str system;
    bool mapper = false;
    py::list<py::bytes> pages;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // Where in the ROM the byte the Z80 reads at `address` at power-on is: an offset, or nullopt.
    std::optional<int64_t> mapped(int64_t address) const;
};

bool is_sms(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));
py::bytearray _memory(const ROM& rom);

// Function starts in the power-on address space ($0000-$BFFF): the reset, interrupt and NMI
// vectors, then every CALL and RST target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

// An interrupt vector the program does not use is often filled with $FF (RST $38) or zero.
bool _looks_like_code(const py::bytearray& mem, int64_t at);

}  // namespace codebreaker::formats::sms
