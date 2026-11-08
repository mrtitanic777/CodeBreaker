// codebreaker/formats/gb.cpp -- C++17 port of codebreaker.formats.gb (see gb.hpp).

#include "codebreaker/formats/gb.hpp"

#include <vector>

#include "codebreaker/decompile/sm83.hpp"

namespace codebreaker::formats::gb {

const std::tuple<py::str, py::str> EXTENSIONS = std::tuple<py::str, py::str>{py::str(".gb"), py::str(".gbc")};

const py::bytes LOGO = py::bytes::fromhex(py::str(
    "CEED6666CC0D000B03730083000C000D0008111F8889000EDCCC6EE6DDDDD999BBBB67636E0EECCCDDDC999FBBB9333E"));

const py::dict<int64_t, py::str> MBC = {
    {0x00, py::str("ROM")},
    {0x01, py::str("MBC1")},
    {0x02, py::str("MBC1+RAM")},
    {0x03, py::str("MBC1+RAM+BATTERY")},
    {0x05, py::str("MBC2")},
    {0x06, py::str("MBC2+BATTERY")},
    {0x08, py::str("ROM+RAM")},
    {0x09, py::str("ROM+RAM+BATTERY")},
    {0x0F, py::str("MBC3+TIMER+BATTERY")},
    {0x10, py::str("MBC3+TIMER+RAM+BATTERY")},
    {0x11, py::str("MBC3")},
    {0x12, py::str("MBC3+RAM")},
    {0x13, py::str("MBC3+RAM+BATTERY")},
    {0x19, py::str("MBC5")},
    {0x1A, py::str("MBC5+RAM")},
    {0x1B, py::str("MBC5+RAM+BATTERY")},
    {0x1C, py::str("MBC5+RUMBLE")},
    {0x1D, py::str("MBC5+RUMBLE+RAM")},
    {0x1E, py::str("MBC5+RUMBLE+RAM+BATTERY")},
    {0x20, py::str("MBC6")},
    {0x22, py::str("MBC7")},
    {0xFC, py::str("POCKET CAMERA")},
    {0xFE, py::str("HuC3")},
    {0xFF, py::str("HuC1")},
};

py::bytearray ROM::memory() const {
    py::bytearray mem = py::bytearray::zeros(0x10000);
    int64_t n = py::min(py::len(data), 0x8000);
    mem.set_slice(py::none, n, data.slice_to(n));
    return mem;
}

py::list<int64_t> ROM::functions() const {
    return gb::functions(*this);
}

bool is_gb(const py::str& path) {
    if (!path.lower().endswith(EXTENSIONS)) {
        try {
            py::file f = py::open_rb(path);
            f.seek(0x104);
            bool same = f.read(py::len(LOGO)) == LOGO;
            f.close();
            return same;
        } catch (const py::OSError&) {
            return false;
        }
    }
    try {
        return py::os::path::getsize(path) >= 0x8000;
    } catch (const py::OSError&) {
        return false;
    }
}

py::ref<ROM> read(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes data = f.read();
    f.close();
    return parse(data, path);
}

py::ref<ROM> parse(const py::bytes& data, const py::str& path) {
    if (py::len(data) < 0x150) {
        throw py::ValueError(py::percent(py::str("%s: too short for a Game Boy cartridge"), path).utf8());
    }
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->data = data;                                   // bytes(data): a copy
    const py::bytes& h = rom->data;
    rom->logo_ok = h.slice(0x104, 0x104 + py::len(LOGO)) == LOGO;
    int64_t cgb = h[0x143];
    rom->cgb = cgb == 0x80 ? py::str("also") : cgb == 0xC0 ? py::str("only") : py::str("");
    // the title: up to the first NUL, read as latin-1
    py::bytes raw = h.slice(0x134, py::truthy(rom->cgb) ? 0x13F : 0x143);
    int64_t nul = raw.find(0);
    if (nul >= 0) {
        raw = raw.slice_to(nul);
    }
    rom->title = raw.decode("latin-1").strip();
    rom->sgb = h[0x146] == 0x03;
    rom->mbc = MBC.contains(h[0x147]) ? MBC.at(h[0x147]) : py::percent(py::str("type $%02X"), h[0x147]);
    rom->header_checksum = h[0x14D];
    int64_t x = 0;
    for (int64_t i = 0x134; i < 0x14D; i++) {
        x = (x - h[i] - 1) & 0xFF;
    }
    rom->header_checksum_ok = x == h[0x14D];
    py::list<py::bytes> banks;
    for (int64_t i = 0; i < py::len(rom->data); i += BANK) {
        banks.append(rom->data.slice(i, i + BANK));
    }
    rom->banks = banks;
    py::dict<py::str, int64_t> vectors;
    vectors.set(py::str("entry"), 0x0100);
    vectors.set(py::str("vblank"), 0x40);
    vectors.set(py::str("stat"), 0x48);
    vectors.set(py::str("timer"), 0x50);
    vectors.set(py::str("serial"), 0x58);
    vectors.set(py::str("joypad"), 0x60);
    rom->vectors = vectors;
    return rom;
}

py::list<int64_t> functions(const ROM& rom) {
    py::bytearray mem = rom.memory();
    const py::bytes code = mem.view();                  // (read-only from here on)
    int64_t limit = py::min(py::len(rom.data), 0x8000);
    py::set<int64_t> starts;
    starts.add(0x0100);
    for (const auto& kv : rom.vectors.items()) {
        const py::str& name = std::get<0>(kv);
        int64_t at = std::get<1>(kv);
        if (name != py::str("entry") && !py::in(mem[at], std::tuple<int64_t, int64_t>{0xFF, 0x00}) &&
            decompile::sm83::decode(code, at, at)) {
            starts.add(at);
        }
    }
    std::vector<int64_t> todo(starts.begin(), starts.end());
    py::set<int64_t> seen;
    while (!todo.empty()) {
        int64_t pc = todo.back();
        todo.pop_back();
        while (0 <= pc && pc < limit && !seen.contains(pc)) {
            if (0x104 <= pc && pc < 0x150) {            // the header is data
                break;
            }
            seen.add(pc);
            std::optional<decompile::sm83::Insn> insn = decompile::sm83::decode(code, pc, pc);
            if (!insn) {
                break;
            }
            std::optional<int64_t> t = insn->target;
            const py::str& flow = insn->flow;
            if ((flow == py::str("call") || flow == py::str("ccall")) && t && *t < limit && !starts.contains(*t)) {
                starts.add(*t);
                todo.push_back(*t);
            }
            if (t && *t < limit &&
                (flow == py::str("jump") || flow == py::str("branch") || flow == py::str("call") || flow == py::str("ccall"))) {
                todo.push_back(*t);
            }
            if (flow == py::str("jump") || flow == py::str("ret") || flow == py::str("indirect") ||
                insn->pattern == py::str("STOP")) {
                break;
            }
            pc += insn->size;
        }
    }
    return py::sorted(starts);
}

}  // namespace codebreaker::formats::gb
