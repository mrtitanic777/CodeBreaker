// codebreaker/source/gb_writer.cpp -- typed C++ of codebreaker.source.gb_writer.

#include "codebreaker/source/gb_writer.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "codebreaker/build/sm83_asm.hpp"

namespace codebreaker::source::gb_writer {

namespace sm83 = codebreaker::decompile::sm83;
namespace gb = codebreaker::formats::gb;
namespace w = codebreaker::source::image_writer;
using Insn = sm83::Insn;

const py::str DOC(R"DOC(gb_writer.py -- a Game Boy program as assembly source that builds back into it.

    python -m codebreaker.source.gb_writer <game.gb> [<out folder>]
    from codebreaker.source import gb_writer
    r = gb_writer.reconstruct(path, folder)
    r["source"]                     game.asm (RGBDS syntax)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

Game Boy programs were written in assembly, so assembly is what comes back. Code is what the
program's own flow reaches from the entry at $0100 and the interrupt vectors in the two banks
mapped at power-on (formats/gb.py); every byte it reaches is written as an instruction, every
other byte as data. The cartridge header ($0104-$014F) is written as data with its fields named
in comments. Functions are named for their vector (`entry`, `vblank`, `stat`, `timer`, `serial`,
`joypad`) or `sub_<address>`, jump targets `L_<address>`; the I/O registers by the names
Pan Docs gives them (rLCDC, rSTAT, ...), as hardware.inc does.

It builds with CodeBreaker's own assembler (build/sm83_asm.py) or with RGBDS
(`rgbasm -o game.o game.asm && rgblink -o game.gb game.o`).
)DOC");

// the I/O registers, as hardware.inc names them
const py::dict<int64_t, py::str> REGISTERS = {
    {0xFF00, py::str("rP1")},   {0xFF01, py::str("rSB")},   {0xFF02, py::str("rSC")},   {0xFF04, py::str("rDIV")},   {0xFF05, py::str("rTIMA")},
    {0xFF06, py::str("rTMA")},  {0xFF07, py::str("rTAC")},  {0xFF0F, py::str("rIF")},   {0xFF10, py::str("rNR10")},  {0xFF11, py::str("rNR11")},
    {0xFF12, py::str("rNR12")}, {0xFF13, py::str("rNR13")}, {0xFF14, py::str("rNR14")}, {0xFF16, py::str("rNR21")},  {0xFF17, py::str("rNR22")},
    {0xFF18, py::str("rNR23")}, {0xFF19, py::str("rNR24")}, {0xFF1A, py::str("rNR30")}, {0xFF1B, py::str("rNR31")},  {0xFF1C, py::str("rNR32")},
    {0xFF1D, py::str("rNR33")}, {0xFF1E, py::str("rNR34")}, {0xFF20, py::str("rNR41")}, {0xFF21, py::str("rNR42")},  {0xFF22, py::str("rNR43")},
    {0xFF23, py::str("rNR44")}, {0xFF24, py::str("rNR50")}, {0xFF25, py::str("rNR51")}, {0xFF26, py::str("rNR52")},  {0xFF40, py::str("rLCDC")},
    {0xFF41, py::str("rSTAT")}, {0xFF42, py::str("rSCY")},  {0xFF43, py::str("rSCX")},  {0xFF44, py::str("rLY")},    {0xFF45, py::str("rLYC")},
    {0xFF46, py::str("rDMA")},  {0xFF47, py::str("rBGP")},  {0xFF48, py::str("rOBP0")}, {0xFF49, py::str("rOBP1")},  {0xFF4A, py::str("rWY")},
    {0xFF4B, py::str("rWX")},   {0xFF4D, py::str("rKEY1")}, {0xFF4F, py::str("rVBK")},  {0xFF51, py::str("rHDMA1")}, {0xFF52, py::str("rHDMA2")},
    {0xFF53, py::str("rHDMA3")}, {0xFF54, py::str("rHDMA4")}, {0xFF55, py::str("rHDMA5")}, {0xFF56, py::str("rRP")}, {0xFF68, py::str("rBCPS")},
    {0xFF69, py::str("rBCPD")}, {0xFF6A, py::str("rOCPS")}, {0xFF6B, py::str("rOCPD")}, {0xFF70, py::str("rSVBK")},  {0xFFFF, py::str("rIE")}};
// the bank controller's registers (writes into ROM space), as hardware.inc names them
const py::dict<int64_t, py::str> MBC_REGISTERS = {
    {0x0000, py::str("rRAMG")}, {0x2000, py::str("rROMB0")}, {0x3000, py::str("rROMB1")}, {0x4000, py::str("rRAMB")}};
const py::list<std::tuple<int64_t, int64_t, py::str>> _HEADER = {
    {0x0104, 0x30, py::str("Nintendo's logo (the boot ROM checks it)")}, {0x0134, 0x0F, py::str("title")},
    {0x0143, 1, py::str("Color support")},                              {0x0144, 2, py::str("licensee")},
    {0x0146, 1, py::str("Super Game Boy support")},                      {0x0147, 1, py::str("cartridge type")},
    {0x0148, 1, py::str("ROM size")},                                    {0x0149, 1, py::str("RAM size")},
    {0x014A, 1, py::str("destination")},                                 {0x014B, 1, py::str("old licensee")},
    {0x014C, 1, py::str("version")},                                     {0x014D, 1, py::str("header checksum")},
    {0x014E, 2, py::str("global checksum")}};

namespace {
py::dict<int64_t, std::tuple<int64_t, py::str>> header_at() {
    py::dict<int64_t, std::tuple<int64_t, py::str>> d;
    for (const auto& [at, size, what] : _HEADER) {
        d.set(at, std::make_tuple(size, what));
    }
    return d;
}
}  // namespace

const py::dict<int64_t, std::tuple<int64_t, py::str>> _HEADER_AT = header_at();

std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const gb::ROM& rom) {
    py::bytearray mem = rom.memory();
    const py::bytes& memory = mem;
    int64_t limit = py::min(py::len(rom.data), static_cast<int64_t>(0x8000));
    return w::walk<Insn>(
        rom.functions(), [&](int64_t pc) { return sm83::decode(memory, pc, pc); },
        [&](int64_t pc) { return 0 <= pc && pc < limit && !(0x104 <= pc && pc < 0x150); },   // (not the header)
        [&](int64_t pc, const Insn& insn) { return pc + insn.size <= limit && !(pc < 0x104 && 0x104 < pc + insn.size); },
        [&](const Insn& insn) { return *insn.target < limit && py::truthy(insn.flow); },
        [&](const Insn& insn) {
            return insn.flow == py::str("jump") || insn.flow == py::str("ret") || insn.flow == py::str("indirect") ||
                   insn.pattern == py::str("STOP");
        });
}

w::Result reconstruct(const py::str& path, const py::str& folder) {
    py::ref<gb::ROM> rom = gb::read(path);
    py::os::makedirs(folder, 0777, true);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    auto [code, targets, starts] = _walk(*rom);
    py::dict<int64_t, py::str> names;
    for (const auto& [name, at] : rom->vectors.items()) {
        if (starts.contains(at)) {
            names.set(at, name);
        }
    }
    py::dict<int64_t, w::Function> functions = w::functions(starts, names, py::str("sub_%04X"));
    py::set<int64_t> covered = w::covered<Insn>(code);
    int64_t limit = py::min(py::len(rom->data), static_cast<int64_t>(0x8000));
    py::set<int64_t> line_at;
    for (int64_t a = 0; a < limit; ++a) {
        if (!covered.contains(a)) {
            line_at.add(a);
        }
    }
    py::dict<int64_t, py::str> labels = w::labels(targets, names, [&](int64_t a) { return line_at.contains(a); }, py::str("L_%04X"));
    const py::list<py::str> pointer{py::str("LD HL,nn"), py::str("LD DE,nn"), py::str("LD SP,nn")};
    for (const Insn& insn : code.values()) {
        if (!py::truthy(insn.values)) {
            continue;
        }
        int64_t v = insn.values[-1];
        if (!line_at.contains(v) || v < 0x0150) {
            continue;
        }
        if (insn.pattern.startswith(py::str("LD (nn)"))) {
            continue;   // a write into ROM space goes to the bank controller, not to data
        }
        if (pointer.contains(insn.pattern) || (insn.flow == py::str("") && insn.pattern.contains(py::str("(nn)")))) {
            labels.setdefault(v, code.contains(v) ? names.get(v, py::percent("L_%04X", v)) : py::percent("D_%04X", v));
        }
    }
    py::dict<int64_t, py::str> known = REGISTERS.copy();
    known.update(MBC_REGISTERS);
    py::set<int64_t> used_set;
    for (const Insn& i : code.values()) {
        if (i.pattern.startswith(py::str("LDH")) && known.contains(i.values[0])) {
            used_set.add(i.values[0]);
        }
    }
    for (const Insn& i : code.values()) {
        if (i.pattern.contains(py::str("(nn)")) && py::truthy(i.values) && known.contains(i.values[-1])) {
            used_set.add(i.values[-1]);
        }
    }
    py::list<int64_t> used = py::sorted(used_set);
    py::dict<int64_t, py::str> hardware;
    for (int64_t a : used) {
        hardware.set(a, known.at(a));
    }
    py::str cgb_note = py::truthy(rom->cgb) ? py::percent(", Game Boy Color (%s)", rom->cgb) : py::str("");
    w::Assembly out = w::lines(
        py::list<py::str>{py::percent("; %s -- written by CodeBreaker from %s", stem + py::str(".asm"), py::os::path::basename(path)),
                          py::percent("; \"%s\", %s, %d KB ROM%s%s", rom->title, rom->mbc, py::len(rom->data) / 1024, cgb_note,
                                      rom->sgb ? py::str(", Super Game Boy") : py::str("")),
                          py::str("")},
        py::str("rgbds"), py::os::path::basename(path));
    for (int64_t a : used) {
        out.append(py::percent("DEF %-8s EQU $%04X", hardware.at(a), a));
    }
    if (py::truthy(used)) {
        out.append(py::str(""));
    }
    py::dict<int64_t, py::str> names_for_text = labels.copy();
    names_for_text.update(hardware);

    auto lines = [](const py::list<int64_t>& data) { return w::data_lines(data, py::str("db"), py::str("ds %d, $%02X")); };
    for (int64_t bank = 0; bank < py::len(rom->banks); ++bank) {
        int64_t base = bank == 0 ? 0 : gb::BANK;
        int64_t first = bank * gb::BANK;
        int64_t end = first + py::len(rom->banks[bank]);
        bool mapped = bank <= 1;   // (the banks seen at power-on: where flow was followed)
        out.part(py::percent("bank%d", bank), py::str("bank"), std::nullopt, py::len(rom->banks[bank]));
        out.append(py::percent("SECTION \"bank%d\", %s", bank,
                               bank == 0 ? py::str("ROM0[$0000]") : py::percent("ROMX[$4000], BANK[%d]", bank)));

        auto header = [&](int64_t /*offset*/, int64_t addr, const std::function<void()>& flush) -> int64_t {
            if (bank != 0 || !_HEADER_AT.contains(addr)) {
                return 0;
            }
            flush();
            auto [size, what] = _HEADER_AT.at(addr);
            out.extend(w::commented(lines(py::list<int64_t>::from(rom->data.slice(addr, addr + size))), what));
            return size;
        };
        auto code_at = [&](int64_t offset, int64_t addr) -> std::optional<Insn> {
            std::optional<Insn> insn;
            if (mapped) {
                insn = code.get(addr);
            }
            return insn.has_value() && offset + insn->size <= end ? insn : std::nullopt;
        };
        w::emit<Insn>(
            out, first, end, [&](int64_t offset) { return rom->data[offset]; }, [&](int64_t offset) { return base + offset - first; }, code_at,
            mapped ? labels : py::dict<int64_t, py::str>(), functions, [&](const Insn& insn) { return sm83::text(insn, names_for_text); }, lines,
            header);
        out.append(py::str(""));
    }
    py::str source = w::text(out) + py::str("\n");
    py::dict<py::str, w::Extra> more;
    more.set(py::str("rom bytes"), w::Extra(py::len(rom->data)));
    w::Result r = w::result<Insn>(folder, stem + py::str(".asm"), source, functions, code, nullptr, more);
    w::assembled(
        r, [&]() { return build::sm83_asm::build(source, py::len(rom->data), folder); }, rom->data,
        py::os::path::join(folder, stem + py::str(".built") + std::get<1>(py::os::path::splitext(path))));
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    return w::main(argv, reconstruct, DOC);
}

}  // namespace codebreaker::source::gb_writer
