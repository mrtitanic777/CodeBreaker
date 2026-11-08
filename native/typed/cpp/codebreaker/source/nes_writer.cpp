// codebreaker/source/nes_writer.cpp -- typed C++ of codebreaker.source.nes_writer.

#include "codebreaker/source/nes_writer.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "codebreaker/build/m6502_asm.hpp"

namespace codebreaker::source::nes_writer {

namespace m6502 = codebreaker::decompile::m6502;
namespace nes = codebreaker::formats::nes;
namespace w = codebreaker::source::image_writer;
using Insn = m6502::Insn;

const py::str DOC(R"DOC(nes_writer.py -- a NES program as assembly source that builds back into it.

    python -m codebreaker.source.nes_writer <game.nes> [<out folder>]
    from codebreaker.source import nes_writer
    r = nes_writer.reconstruct(path, folder)
    r["source"], r["config"]        game.s (ca65 syntax) and game.cfg (ld65), beside the CHR as game.chr
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

NES programs were written in assembly almost without exception, so assembly is what comes back
(docs/PLATFORM_DESCRIPTOR.md). Code is what the program's own flow reaches from its three
vectors (formats/nes.py); every byte it reaches is written as an instruction, every other byte
as data. Function starts are named for their vector (`reset`, `nmi`, `irq`) or `sub_<address>`,
branch and jump targets `L_<address>`, and an absolute operand naming the start of a line gets
that line's label -- so the source can be moved and edited, not only rebuilt. The last six
bytes of the bank that holds them are the vectors, written as `.word nmi, reset, irq`.

It builds with CodeBreaker's own assembler (build/m6502_asm.py) or with the cc65 suite
(`ca65 game.s && ld65 -C game.cfg -o game.nes game.o`).
)DOC");

// the console's registers, by the names the hardware documentation gives them
const py::dict<int64_t, py::str> HARDWARE = {
    {0x2000, py::str("PPUCTRL")},   {0x2001, py::str("PPUMASK")},   {0x2002, py::str("PPUSTATUS")}, {0x2003, py::str("OAMADDR")},
    {0x2004, py::str("OAMDATA")},   {0x2005, py::str("PPUSCROLL")}, {0x2006, py::str("PPUADDR")},   {0x2007, py::str("PPUDATA")},
    {0x4000, py::str("SQ1_VOL")},   {0x4001, py::str("SQ1_SWEEP")}, {0x4002, py::str("SQ1_LO")},    {0x4003, py::str("SQ1_HI")},
    {0x4004, py::str("SQ2_VOL")},   {0x4005, py::str("SQ2_SWEEP")}, {0x4006, py::str("SQ2_LO")},    {0x4007, py::str("SQ2_HI")},
    {0x4008, py::str("TRI_LINEAR")}, {0x400A, py::str("TRI_LO")},    {0x400B, py::str("TRI_HI")},    {0x400C, py::str("NOISE_VOL")},
    {0x400E, py::str("NOISE_LO")},  {0x400F, py::str("NOISE_HI")},   {0x4010, py::str("DMC_FREQ")},  {0x4011, py::str("DMC_RAW")},
    {0x4012, py::str("DMC_START")}, {0x4013, py::str("DMC_LEN")},    {0x4014, py::str("OAMDMA")},    {0x4015, py::str("SND_CHN")},
    {0x4016, py::str("JOY1")},      {0x4017, py::str("JOY2")}};

// [(segment name, CPU address of its first byte, PRG offset, size)]
py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> _banks(const nes::ROM& rom) {
    if (py::len(rom.prg) == 0x4000) {
        return {std::make_tuple(py::str("BANK0"), static_cast<int64_t>(0xC000), static_cast<int64_t>(0), static_cast<int64_t>(0x4000))};
    }
    if (rom.mapper == 0 || py::len(rom.prg) <= 0x8000) {
        return {std::make_tuple(py::str("BANK0"), static_cast<int64_t>(0x8000), static_cast<int64_t>(0), py::len(rom.prg))};
    }
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> out;
    for (int64_t i = 0; i < py::len(rom.banks); ++i) {
        int64_t at = i == py::len(rom.banks) - 1 ? 0xC000 : 0x8000;
        out.append(std::make_tuple(py::percent("BANK%d", i), at, i * 0x4000, static_cast<int64_t>(0x4000)));
    }
    return out;
}

// {PRG offset of an instruction's first byte: Insn} for what flow reaches, and {target}
std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const nes::ROM& rom) {
    py::bytearray mem = rom.memory();
    const py::bytes& memory = mem;
    return w::walk<Insn>(
        rom.functions(), [&](int64_t pc) { return m6502::decode(memory, pc, pc); }, [&](int64_t pc) { return 0x8000 <= pc && pc <= 0xFFFF; },
        [&](int64_t pc, const Insn& insn) {
            std::optional<int64_t> at = rom.mapped(pc);
            if (!at.has_value()) {
                return false;
            }
            std::optional<int64_t> last = rom.mapped(pc + insn.size - 1);
            return last.has_value() && *last == *at + insn.size - 1;
        },
        [&](const Insn& insn) { return *insn.target >= 0x8000; }, [&](const Insn& insn) { return m6502::ENDS.contains(insn.mnemonic); },
        [&](int64_t pc) { return *rom.mapped(pc); });
}

py::list<py::str> _lines(const py::list<int64_t>& data) {
    py::list<py::str> out;
    for (int64_t i = 0; i < py::len(data); i += 16) {
        py::list<py::str> items;
        for (int64_t b : data.slice(i, i + 16)) {
            items.append(py::percent("$%02X", b));
        }
        out.append(py::str("        .byte ") + py::join(py::str(", "), items));
    }
    return out;
}

namespace {

// ", ".join("$%02X" % b for b in data)
py::str hex_bytes(const py::bytes& data) {
    py::list<py::str> items;
    for (int64_t b : data) {
        items.append(py::percent("$%02X", b));
    }
    return py::join(py::str(", "), items);
}

}  // namespace

w::Result reconstruct(const py::str& path, const py::str& folder) {
    py::ref<nes::ROM> rom = nes::read(path);
    py::os::makedirs(folder, 0777, true);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    auto [code, targets, starts] = _walk(*rom);
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> banks = _banks(*rom);
    py::dict<int64_t, py::str> names = {{rom->vectors.at(py::str("reset")), py::str("reset")}};
    names.setdefault(rom->vectors.at(py::str("nmi")), py::str("nmi"));
    names.setdefault(rom->vectors.at(py::str("irq")), py::str("irq"));
    py::dict<int64_t, w::Function> functions = w::functions(starts, names, py::str("sub_%04X"));
    py::set<int64_t> covered = w::covered<Insn>(code);   // PRG offsets inside an instruction
    // a label can only stand where a line starts, in the bank shown at that address
    py::dict<int64_t, int64_t> line_at;
    for (const auto& [seg, base, off0, size] : banks) {
        for (int64_t off = off0; off < off0 + size; ++off) {
            if (!covered.contains(off)) {
                line_at.setdefault(base + off - off0, off);
            }
        }
    }
    py::dict<int64_t, py::str> labels = w::labels(targets, names, [&](int64_t a) { return line_at.contains(a); }, py::str("L_%04X"));
    for (const Insn& insn : code.values()) {   // absolute operands that name a line's start
        if ((insn.mode == py::str("abs") || insn.mode == py::str("abx") || insn.mode == py::str("aby") || insn.mode == py::str("ind")) &&
            insn.mnemonic != py::str("JMP") && insn.mnemonic != py::str("JSR") && insn.operand.has_value() && line_at.contains(*insn.operand) &&
            *insn.operand >= 0x8000) {
            labels.setdefault(*insn.operand, py::percent("D_%04X", *insn.operand));
        }
    }
    py::set<int64_t> used_set;
    for (const Insn& i : code.values()) {
        if ((i.mode == py::str("abs") || i.mode == py::str("abx") || i.mode == py::str("aby")) && i.operand.has_value() &&
            HARDWARE.contains(*i.operand)) {
            used_set.add(*i.operand);
        }
    }
    py::list<int64_t> used = py::sorted(used_set);
    py::dict<int64_t, py::str> hardware;
    for (int64_t a : used) {
        hardware.set(a, HARDWARE.at(a));
    }
    py::list<py::str> head{py::percent("; %s -- written by CodeBreaker from %s", stem + py::str(".s"), py::os::path::basename(path)),
                           py::percent("; mapper %d, %d KB PRG, %d KB CHR, %s mirroring%s", rom->mapper, py::len(rom->prg) / 1024,
                                       py::len(rom->chr) / 1024, rom->mirroring,
                                       rom->battery ? py::str(", battery-backed RAM") : py::str("")),
                           py::str("")};
    for (int64_t a : used) {
        head.append(py::percent("%-10s = $%04X", hardware.at(a), a));
    }
    if (py::truthy(used)) {
        head.append(py::str(""));
    }
    head.append(py::str(".segment \"HEADER\""));
    head.append(py::str("        .byte ") + hex_bytes(rom->header.slice(0, 16)));
    head.append(py::str(""));
    w::Assembly out = w::lines(head, py::str("ca65"), py::os::path::basename(path));
    if (py::truthy(rom->trainer)) {
        out.extend(py::list<py::str>{py::str(".segment \"TRAINER\""), py::str("        .byte ") + hex_bytes(rom->trainer), py::str("")});
    }
    py::dict<int64_t, py::str> text_names = hardware.copy();
    text_names.update(labels);
    for (const auto& bank : banks) {
        const py::str seg = std::get<0>(bank);
        const int64_t base = std::get<1>(bank);
        const int64_t off0 = std::get<2>(bank);
        const int64_t size = std::get<3>(bank);
        out.part(seg, py::str("segment"), std::nullopt, size);
        out.append(py::percent(".segment \"%s\"", seg));
        int64_t end = off0 + size;
        bool vectors_here = base + size == 0x10000;

        auto vectors = [&](int64_t /*off*/, int64_t addr, const std::function<void()>& flush) -> int64_t {
            if (!(vectors_here && addr == 0xFFFA)) {
                return 0;
            }
            flush();
            py::list<py::str> parts;
            for (const char* k : {"nmi", "reset", "irq"}) {
                int64_t v = rom->vectors.at(py::str(k));
                parts.append(labels.get(v, py::percent("$%04X", v)));
            }
            out.append(py::str("        .word ") + py::join(py::str(", "), parts));
            return 6;
        };
        auto code_at = [&](int64_t off, int64_t addr) -> std::optional<Insn> {
            std::optional<Insn> insn = code.get(off);
            if (!insn.has_value() || off + insn->size > end || (vectors_here && addr + insn->size > 0xFFFA)) {
                return std::nullopt;
            }
            return insn;
        };
        w::emit<Insn>(
            out, off0, end, [&](int64_t off) { return rom->prg[off]; }, [&](int64_t off) { return base + off - off0; }, code_at, labels, functions,
            [&](const Insn& insn) { return m6502::text(insn, text_names); }, _lines, vectors);
        out.append(py::str(""));
    }
    if (py::truthy(rom->chr)) {
        {
            py::file f = py::open(py::os::path::join(folder, stem + py::str(".chr")), py::str("wb"));
            f.write(rom->chr);
            f.close();
        }
        out.part(py::str("CHARS"), py::str("segment"), std::nullopt, py::len(rom->chr));
        out.extend(py::list<py::str>{py::str(".segment \"CHARS\""), py::percent("        .incbin \"%s.chr\"", stem), py::str("")});
    }
    py::str source = w::text(out) + py::str("\n");
    py::list<py::str> cfg{py::str("MEMORY {"), py::str("    HEADER: start = $0000, size = $0010, fill = yes, file = %O;")};
    if (py::truthy(rom->trainer)) {
        cfg.append(py::str("    TRAINER: start = $7000, size = $0200, fill = yes, file = %O;"));
    }
    for (const auto& [seg, base, _off, size] : banks) {
        cfg.append(py::percent("    PRG%s: start = $%04X, size = $%04X, fill = yes, file = %%O;", seg.slice_from(4), base, size));
    }
    if (py::truthy(rom->chr)) {
        cfg.append(py::percent("    CHR: start = $0000, size = $%04X, fill = yes, file = %%O;", py::len(rom->chr)));
    }
    cfg.append(py::str("}"));
    cfg.append(py::str("SEGMENTS {"));
    cfg.append(py::str("    HEADER: load = HEADER, type = ro;"));
    if (py::truthy(rom->trainer)) {
        cfg.append(py::str("    TRAINER: load = TRAINER, type = ro;"));
    }
    for (const auto& [seg, _b, _o, _s] : banks) {
        cfg.append(py::percent("    %s: load = PRG%s, type = ro;", seg, seg.slice_from(4)));
    }
    if (py::truthy(rom->chr)) {
        cfg.append(py::str("    CHARS: load = CHR, type = ro;"));
    }
    cfg.append(py::str("}"));
    py::str cfg_text = py::join(py::str("\n"), cfg) + py::str("\n");
    py::str cfg_path = py::os::path::join(folder, stem + py::str(".cfg"));
    {
        py::file f = py::open(cfg_path, py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
        f.write(cfg_text);
        f.close();
    }
    py::dict<py::str, w::Extra> more;
    more.set(py::str("config"), w::Extra(cfg_path));
    more.set(py::str("prg bytes"), w::Extra(py::len(rom->prg)));
    w::Result r = w::result<Insn>(folder, stem + py::str(".s"), source, functions, code, nullptr, more);
    w::assembled(
        r, [&]() { return build::m6502_asm::build(source, cfg_text, folder); }, rom->header + rom->trainer + rom->prg + rom->chr,
        py::os::path::join(folder, stem + py::str(".built.nes")));
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    return w::main(argv, reconstruct, DOC, py::str("PRG bytes"), py::str("prg bytes"));
}

}  // namespace codebreaker::source::nes_writer
