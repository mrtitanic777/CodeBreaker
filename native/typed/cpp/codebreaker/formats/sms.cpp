// codebreaker/formats/sms.cpp -- C++17 port of codebreaker.formats.sms (see sms.hpp).

#include "codebreaker/formats/sms.hpp"

#include <vector>

#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::formats::sms {

const std::array<int64_t, 3> _HEADER_AT = {0x7FF0, 0x3FF0, 0x1FF0};

const py::dict<int64_t, std::tuple<py::str, py::str>> REGIONS = {
    {3, {py::str("Master System"), py::str("Japan")}},
    {4, {py::str("Master System"), py::str("export")}},
    {5, {py::str("Game Gear"), py::str("Japan")}},
    {6, {py::str("Game Gear"), py::str("export")}},
    {7, {py::str("Game Gear"), py::str("international")}},
};

const py::dict<int64_t, int64_t> _SIZES = {
    {0xA, 0x2000}, {0xB, 0x4000}, {0xC, 0x8000},   {0xD, 0xC000},  {0xE, 0x10000},
    {0xF, 0x20000}, {0x0, 0x40000}, {0x1, 0x80000}, {0x2, 0x100000},
};

const std::tuple<py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str>{py::str(".sms"), py::str(".gg"), py::str(".sg")};

py::bytearray ROM::memory() const {
    return _memory(*this);
}

py::list<int64_t> ROM::functions() const {
    return sms::functions(*this);
}

std::optional<int64_t> ROM::mapped(int64_t address) const {
    if (address >= 0xC000 || address >= py::len(data)) {
        return std::nullopt;
    }
    return address;
}

bool is_sms(const py::str& path) {
    if (!path.lower().endswith(EXTENSIONS)) {
        return false;
    }
    try {
        return py::os::path::getsize(path) >= 0x2000;
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

py::ref<ROM> parse(const py::bytes& data_in, const py::str& path) {
    py::bytes data = data_in;
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    if (py::len(data) % PAGE == 512) {                     // a copier's header in front
        data = data.slice_from(512);
    }
    rom->data = data;
    rom->header = std::nullopt;
    for (int64_t at : _HEADER_AT) {
        if (py::len(data) >= at + 16 && data.slice(at, at + 8) == py::bytes("TMR SEGA")) {
            py::bytes h = data.slice(at, at + 16);
            int64_t region = h[15] >> 4;
            int64_t size = h[15] & 0x0F;
            // the product code: BCD, low byte first, its top digit in the high nibble of byte 14
            py::str product = py::percent(py::str("%X%02X%02X"), h[14] >> 4, h[13], h[12]).lstrip(py::str("0"));
            Header hd;
            hd.at = at;
            hd.checksum = std::get<0>(py::unpack_from<int64_t>("<H", h, 10));
            hd.product = py::truthy(product) ? product : py::str("0");
            hd.version = h[14] & 0x0F;
            hd.region = REGIONS.contains(region) ? std::get<1>(REGIONS.at(region)) : py::str("?");
            hd.size = _SIZES.contains(size) ? std::optional<int64_t>(_SIZES.at(size)) : std::nullopt;
            rom->header = hd;
            break;
        }
    }
    py::str ext = std::get<1>(py::os::path::splitext(path)).lower();
    if (rom->header) {
        int64_t nibble = rom->data[rom->header->at + 15] >> 4;
        rom->system = REGIONS.contains(nibble) ? std::get<0>(REGIONS.at(nibble)) : py::str("Master System");
    } else if (ext == py::str(".gg")) {
        rom->system = py::str("Game Gear");
    } else if (ext == py::str(".sg")) {
        rom->system = py::str("SG-1000");
    } else {
        rom->system = py::str("Master System");
    }
    rom->mapper = rom->system != py::str("SG-1000") && py::len(rom->data) > 0xC000;
    py::list<py::bytes> pages;
    for (int64_t i = 0; i < py::len(rom->data); i += PAGE) {
        pages.append(rom->data.slice(i, i + PAGE));
    }
    rom->pages = pages;
    rom->vectors = py::dict<py::str, int64_t>{{py::str("reset"), 0x0000}, {py::str("irq"), 0x0038}, {py::str("nmi"), 0x0066}};
    return rom;
}

py::bytearray _memory(const ROM& rom) {
    py::bytearray mem = py::bytearray::zeros(0x10000);
    int64_t n = py::min(py::len(rom.data), 0xC000);
    mem.set_slice(py::none, n, rom.data.slice_to(n));
    return mem;
}

py::list<int64_t> functions(const ROM& rom) {
    py::bytearray mem = rom.memory();
    const py::bytes code = mem.view();                    // (read-only from here on)
    int64_t limit = py::min(py::len(rom.data), 0xC000);
    py::set<int64_t> starts;
    for (int64_t v : rom.vectors.values()) {
        if (v < limit && _looks_like_code(mem, v)) {
            starts.add(v);
        }
    }
    starts.add(0);
    std::vector<int64_t> todo(starts.begin(), starts.end());
    py::set<int64_t> seen;
    while (!todo.empty()) {
        int64_t pc = todo.back();
        todo.pop_back();
        while (0 <= pc && pc < limit && !seen.contains(pc)) {
            seen.add(pc);
            std::optional<decompile::z80::Insn> insn = decompile::z80::decode(code, pc, pc);
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
            if (flow == py::str("jump") || flow == py::str("ret") || flow == py::str("indirect")) {     // (HALT resumes after an interrupt)
                break;
            }
            pc += insn->size;
        }
    }
    return py::sorted(starts);
}

bool _looks_like_code(const py::bytearray& mem, int64_t at) {
    return !py::in(mem[at], std::tuple<int64_t>{0xFF});
}

}  // namespace codebreaker::formats::sms
