// codebreaker/formats/nes.cpp -- C++17 port of codebreaker.formats.nes (see nes.hpp).

#include "codebreaker/formats/nes.hpp"

#include "codebreaker/decompile/m6502.hpp"

namespace codebreaker::formats::nes {

const py::bytes MAGIC = py::bytes("NES\x1a");
const py::dict<int64_t, py::str> MIRRORING = {{0, py::str("horizontal")}, {1, py::str("vertical")}};

py::bytearray ROM::memory() const {
    return _memory(*this);
}

py::list<int64_t> ROM::functions() const {
    return nes::functions(*this);
}

std::optional<int64_t> ROM::mapped(int64_t address) const {
    return _mapped(*this, address);
}

bool is_nes(const py::str& path) {
    try {
        py::file f = py::open_rb(path);
        py::bytes head = f.read(4);
        f.close();
        return head == MAGIC;
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
    if (data.slice(0, 4) != MAGIC || py::len(data) < 16) {
        throw py::ValueError(py::percent("%s: not an iNES image", path));
    }
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    int64_t prg_units = data[4];
    int64_t chr_units = data[5];
    int64_t f6 = data[6];
    int64_t f7 = data[7];
    rom->nes2 = (f7 & 0x0C) == 0x08;
    rom->mapper = (f6 >> 4) | (f7 & 0xF0);
    rom->submapper = 0;
    if (rom->nes2) {
        rom->mapper |= (data[8] & 0x0F) << 8;
        rom->submapper = data[8] >> 4;
        prg_units |= (data[9] & 0x0F) << 8;
        chr_units |= (data[9] & 0xF0) << 4;
    }
    rom->four_screen = (f6 & 0x08) != 0;
    rom->mirroring = rom->four_screen ? py::str("four-screen") : MIRRORING.at(f6 & 1);
    rom->battery = (f6 & 0x02) != 0;
    int64_t at = 16;
    rom->trainer = (f6 & 0x04) ? data.slice(at, at + 512) : py::bytes("");
    at += py::len(rom->trainer);
    rom->header = data.slice(0, 16);
    rom->prg = data.slice(at, at + prg_units * 0x4000);
    at += py::len(rom->prg);
    rom->chr = data.slice(at, at + chr_units * 0x2000);
    at += py::len(rom->chr);
    rom->extra = data.slice_from(at);            // (anything after: some dumps carry a title)
    if (py::len(rom->prg) != prg_units * 0x4000) {
        throw py::ValueError(py::percent("%s: the PRG is cut short (%d of %d bytes)", path, py::len(rom->prg), prg_units * 0x4000));
    }
    py::list<py::bytes> banks;
    for (int64_t i = 0; i < py::len(rom->prg); i += 0x4000) {
        banks.append(rom->prg.slice(i, i + 0x4000));
    }
    rom->banks = banks;
    rom->code_at = py::len(rom->prg) == 0x4000 ? 0xC000 : 0x8000;
    py::bytearray mem = rom->memory();
    py::dict<py::str, int64_t> vectors;
    vectors.set(py::str("nmi"), py::unpack_from1<int64_t>("<H", mem.view(), 0xFFFA));
    vectors.set(py::str("reset"), py::unpack_from1<int64_t>("<H", mem.view(), 0xFFFC));
    vectors.set(py::str("irq"), py::unpack_from1<int64_t>("<H", mem.view(), 0xFFFE));
    rom->vectors = vectors;
    return rom;
}

py::bytearray _memory(const ROM& rom) {
    py::bytearray mem = py::bytearray::zeros(0x10000);
    if (rom.mapper == 0 || py::len(rom.prg) <= 0x8000) {
        if (py::len(rom.prg) == 0x4000) {
            mem.set_slice(0x8000, 0xC000, rom.prg);
            mem.set_slice(0xC000, 0x10000, rom.prg);
        } else {
            mem.set_slice(0x8000, 0x8000 + py::len(rom.prg), rom.prg.slice(0, 0x8000));
        }
    } else {
        mem.set_slice(0x8000, 0xC000, rom.banks[0]);
        mem.set_slice(0xC000, 0x10000, rom.banks[-1]);
    }
    return mem;
}

std::optional<int64_t> _mapped(const ROM& rom, int64_t address) {
    if (address < 0x8000) {
        return std::nullopt;
    }
    if (py::len(rom.prg) == 0x4000) {
        return (address - 0x8000) & 0x3FFF;
    }
    if (rom.mapper == 0 || py::len(rom.prg) <= 0x8000) {
        return address - 0x8000;
    }
    return address < 0xC000 ? (address - 0x8000) : py::len(rom.prg) - 0x4000 + (address - 0xC000);
}

py::list<int64_t> functions(const ROM& rom) {
    py::bytearray memory = rom.memory();
    const py::bytes mem = memory.view();
    py::set<int64_t> starts;
    for (int64_t v : rom.vectors.values()) {
        if (v >= 0x8000) {
            starts.add(v);
        }
    }
    py::list<int64_t> todo = py::list<int64_t>::from(starts);
    py::set<int64_t> seen;
    while (!todo.empty()) {
        int64_t pc = todo.pop();
        while (0x8000 <= pc && pc <= 0xFFFF && !seen.contains(pc)) {
            seen.add(pc);
            std::optional<decompile::m6502::Insn> insn = decompile::m6502::decode(mem, pc, pc);
            if (!insn) {
                break;
            }
            if (insn->mnemonic == py::str("JSR") && *insn->target >= 0x8000 && !starts.contains(*insn->target)) {
                starts.add(*insn->target);
                todo.append(*insn->target);
            }
            if (insn->target && *insn->target >= 0x8000) {
                todo.append(*insn->target);
            }
            if (decompile::m6502::ENDS.contains(insn->mnemonic)) {
                break;
            }
            pc += insn->size;
        }
    }
    return py::sorted(starts);
}

}  // namespace codebreaker::formats::nes
