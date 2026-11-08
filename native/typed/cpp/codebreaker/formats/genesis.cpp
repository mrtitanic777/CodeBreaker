// codebreaker/formats/genesis.cpp -- C++17 port of codebreaker.formats.genesis (see genesis.hpp).

#include "codebreaker/formats/genesis.hpp"

#include <vector>

#include "codebreaker/decompile/m68k.hpp"

namespace codebreaker::formats::genesis {

const std::tuple<py::str, py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str, py::str>{py::str(".md"), py::str(".bin"), py::str(".gen"), py::str(".smd")};

// (the Python adds the sixteen trap vectors after the table, in a loop)
const py::dict<int64_t, py::str> VECTOR_NAMES = [] {
    py::dict<int64_t, py::str> names = {
        {2, py::str("bus_error")},  {3, py::str("address_error")}, {4, py::str("illegal_instruction")},
        {5, py::str("zero_divide")}, {6, py::str("chk")},          {7, py::str("trapv")},
        {8, py::str("privilege")},  {9, py::str("trace")},         {10, py::str("line_a")},
        {11, py::str("line_f")},    {24, py::str("spurious")},     {25, py::str("level1")},
        {26, py::str("external")},  {27, py::str("level3")},       {28, py::str("hblank")},
        {29, py::str("level5")},    {30, py::str("vblank")},       {31, py::str("level7")},
    };
    for (int64_t n = 0; n < 16; n++) {
        names.set(32 + n, py::percent(py::str("trap%d"), n));
    }
    return names;
}();

py::bytearray ROM::memory() const {
    py::bytearray mem = py::bytearray::zeros(0x1000000);
    mem.set_slice(py::none, py::len(data), data.slice_to(0x400000));
    return mem;
}

py::list<int64_t> ROM::functions() const {
    return genesis::functions(*this);
}

int64_t ROM::checksum() const {
    py::bytes body = data.slice_from(0x200);
    if (py::len(body) % 2) {
        body = body + py::bytes(std::string(1, '\0'));
    }
    int64_t total = 0;
    for (int64_t i = 0; i < py::len(body); i += 2) {
        total += (body[i] << 8) | body[i + 1];
    }
    return total & 0xFFFF;
}

bool is_genesis(const py::str& path) {
    py::bytes head;
    try {
        py::file f = py::open_rb(path);
        head = f.read(0x400);
        f.close();
    } catch (const py::OSError&) {
        return false;
    }
    if (head.slice(0x100, 0x104) == py::bytes("SEGA") || head.slice(0x101, 0x105) == py::bytes("SEGA")) {
        return true;
    }
    return path.lower().endswith(std::tuple<py::str, py::str, py::str>{py::str(".md"), py::str(".gen"), py::str(".smd")}) &&
           py::len(head) >= 0x200;
}

py::ref<ROM> read(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes data = f.read();
    f.close();
    return parse(data, path);
}

py::bytes _desmd(const py::bytes& data) {
    py::bytes body = data.slice_from(512);
    py::bytearray out = py::bytearray::zeros(py::len(body));
    for (int64_t b = 0; b < py::len(body); b += 0x4000) {
        py::bytes block = body.slice(b, b + 0x4000);
        int64_t n = py::len(block);
        int64_t half = n / 2;
        // out[b + 1:b + n:2] = block[:half]; out[b:b + n:2] = block[half:]
        for (int64_t i = 0; i < half; i++) {
            out[b + 1 + 2 * i] = block[i];
        }
        for (int64_t i = 0; i < n - half; i++) {
            out[b + 2 * i] = block[half + i];
        }
    }
    return out.view();
}

py::ref<ROM> parse(const py::bytes& data_in, const py::str& path) {
    py::bytes data = data_in;
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->smd = py::len(data) % 0x4000 == 512 && data.slice(0x100, 0x104) != py::bytes("SEGA") &&
               data.slice(8, 10) == py::bytes("\xAA\xBB");
    if (rom->smd || (path.lower().endswith(py::str(".smd")) && py::len(data) % 0x4000 == 512)) {
        data = _desmd(data);
        rom->smd = true;
    }
    if (py::len(data) < 0x200) {
        throw py::ValueError(py::percent(py::str("%s: too short for a Genesis cartridge"), path).utf8());
    }
    rom->data = data;                                   // bytes(data): a copy
    const py::bytes& h = rom->data;

    // h[at:at + n].decode("latin-1").strip()
    auto field = [&h](int64_t at, int64_t n) { return h.slice(at, at + n).decode("latin-1").strip(); };
    Header hd;
    hd.system = field(0x100, 16);
    hd.copyright = field(0x110, 16);
    hd.domestic_name = field(0x120, 48);
    hd.overseas_name = field(0x150, 48);
    hd.serial = field(0x180, 14);
    hd.checksum = py::unpack_from1<int64_t>(">H", h, 0x18E);
    hd.devices = field(0x190, 16);
    hd.rom = py::unpack_from<int64_t, int64_t>(">II", h, 0x1A0);
    hd.ram = py::unpack_from<int64_t, int64_t>(">II", h, 0x1A8);
    hd.region = field(0x1F0, 3);
    rom->header = hd;
    // struct.unpack_from(">64I", h, 0)
    std::vector<int64_t> longs;
    for (int64_t n = 0; n < 64; n++) {
        longs.push_back(py::unpack_from1<int64_t>(">I", h, 4 * n));
    }
    py::dict<py::str, int64_t> vectors;
    vectors.set(py::str("ssp"), longs[0]);
    vectors.set(py::str("reset"), longs[1]);
    for (int64_t n : VECTOR_NAMES.keys()) {
        int64_t v = longs[static_cast<size_t>(n)];
        if (v < py::len(rom->data) && v % 2 == 0 && v >= 0x200) {
            vectors.set(VECTOR_NAMES.at(n), v);
        }
    }
    rom->vectors = vectors;
    return rom;
}

py::list<int64_t> functions(const ROM& rom) {
    namespace m68k = codebreaker::decompile::m68k;
    const py::bytes& mem = rom.data;
    int64_t limit = py::len(mem);
    py::set<int64_t> starts;
    for (const py::str& k : rom.vectors.keys()) {
        int64_t v = rom.vectors.at(k);
        if (k != py::str("ssp") && v < limit && v % 2 == 0) {
            starts.add(v);
        }
    }
    std::vector<int64_t> todo(starts.begin(), starts.end());
    py::set<int64_t> seen;
    while (!todo.empty()) {
        int64_t pc = todo.back();
        todo.pop_back();
        while (0 <= pc && pc < limit && !seen.contains(pc)) {
            seen.add(pc);
            // m68k::decode(mem, pc, pc). The typed decoder's reader copies the bytes it is given, so it is handed the
            // window the instruction can lie in (at most 10 bytes long) instead of the whole ROM: the same result
            // (an instruction cut off by the ROM's end is cut off in the window too), without copying 4 MB per step.
            std::optional<m68k::Insn> insn = m68k::decode(mem.slice(pc, pc + 16), 0, pc);
            if (!insn) {
                break;
            }
            std::optional<int64_t> t = insn->target;
            const py::str& flow = insn->flow;
            if (t && *t < limit && *t % 2 == 0) {
                if (flow == py::str("call") && insn->mnemonic != py::str("TRAP") && !starts.contains(*t)) {
                    starts.add(*t);
                }
                if (flow == py::str("jump") || flow == py::str("branch") || flow == py::str("call")) {
                    todo.push_back(*t);
                }
            }
            if (flow == py::str("jump") || flow == py::str("ret") || flow == py::str("indirect") || flow == py::str("halt")) {
                break;
            }
            pc += insn->length;
        }
    }
    return py::sorted(starts);
}

}  // namespace codebreaker::formats::genesis
