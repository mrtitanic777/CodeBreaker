// codebreaker/source/sms_writer.cpp -- typed C++ of codebreaker.source.sms_writer.

#include "codebreaker/source/sms_writer.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "codebreaker/build/z80_asm.hpp"

namespace codebreaker::source::sms_writer {

namespace z80 = codebreaker::decompile::z80;
namespace sms = codebreaker::formats::sms;
namespace w = codebreaker::source::image_writer;
using Insn = z80::Insn;

const py::str DOC(R"DOC(sms_writer.py -- a Master System / Game Gear program as assembly source that builds back into it.

    python -m codebreaker.source.sms_writer <game.sms> [<out folder>]
    from codebreaker.source import sms_writer
    r = sms_writer.reconstruct(path, folder)
    r["source"]                     game.asm (WLA-DX syntax)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

These programs were written in assembly, so assembly is what comes back. Code is what the
program's own flow reaches from the reset, interrupt and NMI vectors in the pages mapped at
power-on (formats/sms.py); every byte it reaches is written as an instruction, every other byte
as data. Function starts are named for their vector (`reset`, `irq`, `nmi`) or `sub_<address>`,
jump targets `L_<address>`, and absolute words naming the start of a line get that line's label,
so the source can be moved and edited, not only rebuilt. The console's ports and the mapper's
registers are written by name.

It builds with CodeBreaker's own assembler (build/z80_asm.py) or with WLA-DX
(`wla-z80 -o game.o game.asm && wlalink linkfile game.sms`).
)DOC");

// the console's I/O ports and the mapper's registers, by the names the hardware documentation gives them
const py::dict<int64_t, py::str> PORTS = {{0x3E, py::str("MEMORY_CONTROL")}, {0x3F, py::str("IO_CONTROL")}, {0x7E, py::str("VCOUNTER")},
                                          {0x7F, py::str("PSG")},            {0xBE, py::str("VDP_DATA")},   {0xBF, py::str("VDP_CONTROL")},
                                          {0xDC, py::str("JOYPAD_1")},       {0xDD, py::str("JOYPAD_2")}};
const py::dict<int64_t, py::str> GG_PORTS = {{0x00, py::str("GG_START")}, {0x06, py::str("GG_STEREO")}};
const py::dict<int64_t, py::str> REGISTERS = {{0xFFFC, py::str("MAPPER_CONTROL")}, {0xFFFD, py::str("MAPPER_SLOT0")},
                                              {0xFFFE, py::str("MAPPER_SLOT1")},   {0xFFFF, py::str("MAPPER_SLOT2")}};

// {ROM offset: Insn} for what flow reaches in the power-on mapping, its targets, the starts.
std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const sms::ROM& rom) {
    py::bytearray mem = rom.memory();
    const py::bytes& memory = mem;
    int64_t limit = py::min(py::len(rom.data), static_cast<int64_t>(0xC000));
    return w::walk<Insn>(
        rom.functions(), [&](int64_t pc) { return z80::decode(memory, pc, pc); }, [&](int64_t pc) { return 0 <= pc && pc < limit; },
        [&](int64_t pc, const Insn& insn) { return pc + insn.size <= limit; },
        [&](const Insn& insn) { return *insn.target < limit && py::truthy(insn.flow); },
        [&](const Insn& insn) {   // (HALT resumes after an interrupt)
            return insn.flow == py::str("jump") || insn.flow == py::str("ret") || insn.flow == py::str("indirect");
        });
}

int64_t _slot(int64_t page) {
    return page == 0 ? 0 : page == 1 ? 1 : 2;
}

w::Result reconstruct(const py::str& path, const py::str& folder) {
    py::ref<sms::ROM> rom = sms::read(path);
    py::os::makedirs(folder, 0777, true);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    auto [code, targets, starts] = _walk(*rom);
    py::dict<int64_t, py::str> names = {{0x0000, py::str("reset")}};
    for (const auto& [name, at] : {std::make_pair(py::str("irq"), static_cast<int64_t>(0x38)), std::make_pair(py::str("nmi"), static_cast<int64_t>(0x66))}) {
        if (starts.contains(at)) {
            names.setdefault(at, name);
        }
    }
    py::dict<int64_t, w::Function> functions = w::functions(starts, names, py::str("sub_%04X"));
    py::set<int64_t> covered = w::covered<Insn>(code);
    int64_t limit = py::min(py::len(rom->data), static_cast<int64_t>(0xC000));
    py::set<int64_t> line_at;
    for (int64_t a = 0; a < limit; ++a) {
        if (!covered.contains(a)) {
            line_at.add(a);
        }
    }
    py::dict<int64_t, py::str> labels = w::labels(targets, names, [&](int64_t a) { return line_at.contains(a); }, py::str("L_%04X"));
    // pointers into the program: a word loaded into a pointer register (HL DE IX IY, not BC, which mostly
    // holds counts), or read and written through (nn), that is the start of a line past the vectors
    const py::list<py::str> pointer{py::str("LD HL,nn"), py::str("LD DE,nn"), py::str("LD IX,nn"), py::str("LD IY,nn"), py::str("LD SP,nn")};
    for (const Insn& insn : code.values()) {
        if (!py::truthy(insn.values)) {
            continue;
        }
        int64_t v = insn.values[-1];
        if (!line_at.contains(v) || v < 0x0100) {
            continue;
        }
        if (pointer.contains(insn.pattern) || (insn.flow == py::str("") && insn.pattern.contains(py::str("(nn)")))) {
            labels.setdefault(v, code.contains(v) ? names.get(v, py::percent("L_%04X", v)) : py::percent("D_%04X", v));
        }
    }
    // the hardware the code names
    // (a Game Gear adds its own ports to the console's)
    py::dict<int64_t, py::str> ports = PORTS.copy();
    if (rom->system == py::str("Game Gear")) {
        ports.update(GG_PORTS);
    }
    py::set<int64_t> used_ports, used_regs;
    for (const Insn& insn : code.values()) {
        if ((insn.pattern == py::str("OUT (n),A") || insn.pattern == py::str("IN A,(n)")) && ports.contains(insn.values[0])) {
            used_ports.add(insn.values[0]);
        }
        if (insn.pattern.contains(py::str("(nn)")) && py::truthy(insn.values) && REGISTERS.contains(insn.values[-1])) {
            used_regs.add(insn.values[-1]);
        }
    }
    py::dict<int64_t, py::str> constants;
    for (int64_t a : used_regs) {
        constants.set(a, REGISTERS.at(a));
    }
    py::dict<int64_t, py::str> port_names;
    for (int64_t a : used_ports) {
        port_names.set(a, ports.at(a));
    }
    int64_t banks = py::len(rom->pages);
    py::str detail("");
    if (rom->header.has_value()) {
        detail = py::percent(", header product %s version %d (%s)", rom->header->product, rom->header->version, rom->header->region);
    }
    w::Assembly out = w::lines(
        py::list<py::str>{py::percent("; %s -- written by CodeBreaker from %s", stem + py::str(".asm"), py::os::path::basename(path)),
                          py::percent("; %s, %d KB ROM%s", rom->system, py::len(rom->data) / 1024, detail),
                          py::str(""),
                          py::str(".MEMORYMAP"),
                          py::str("SLOTSIZE $4000"),
                          py::str("DEFAULTSLOT 2"),
                          py::str("SLOT 0 $0000"),
                          py::str("SLOT 1 $4000"),
                          py::str("SLOT 2 $8000"),
                          py::str(".ENDME"),
                          py::str(""),
                          py::str(".ROMBANKMAP"),
                          py::percent("BANKSTOTAL %d", banks),
                          py::str("BANKSIZE $4000"),
                          py::percent("BANKS %d", banks),
                          py::str(".ENDRO"),
                          py::str("")},
        py::str("wla-z80"), py::os::path::basename(path));
    std::vector<int64_t> port_keys = port_names.keys();
    std::sort(port_keys.begin(), port_keys.end());
    for (int64_t a : port_keys) {
        out.append(py::percent(".DEFINE %-16s $%02X", port_names.at(a), a));
    }
    std::vector<int64_t> constant_keys = constants.keys();
    std::sort(constant_keys.begin(), constant_keys.end());
    for (int64_t a : constant_keys) {
        out.append(py::percent(".DEFINE %-16s $%04X", constants.at(a), a));
    }
    if (py::truthy(port_names) || py::truthy(constants)) {
        out.append(py::str(""));
    }
    py::dict<int64_t, py::str> text_names = labels.copy();
    text_names.update(constants);

    auto text = [&](const Insn& insn) -> py::str {
        py::str t = z80::text(insn, text_names);
        if ((insn.pattern == py::str("OUT (n),A") || insn.pattern == py::str("IN A,(n)")) && port_names.contains(insn.values[0])) {
            t = t.replace(py::percent("($%02X)", insn.values[0]), py::percent("(%s)", port_names.at(insn.values[0])));
        }
        return t;
    };
    const int64_t slot_base[3] = {0x0000, 0x4000, 0x8000};
    for (int64_t page = 0; page < banks; ++page) {
        int64_t base = slot_base[_slot(page)];
        out.part(py::percent("bank%d", page), py::str("bank"), std::nullopt, py::len(rom->pages[page]));
        out.extend(py::list<py::str>{py::percent(".BANK %d SLOT %d", page, _slot(page)), py::str(".ORG $0000")});
        int64_t first = page * sms::PAGE;
        int64_t end = first + py::len(rom->pages[page]);
        bool mapped = page <= 2 && first < limit;   // (the pages seen at power-on: where flow was followed)

        auto code_at = [&](int64_t offset, int64_t addr) -> std::optional<Insn> {
            std::optional<Insn> insn;
            if (mapped) {
                insn = code.get(addr);
            }
            return insn.has_value() && offset + insn->size <= end ? insn : std::nullopt;
        };
        w::emit<Insn>(
            out, first, end, [&](int64_t offset) { return rom->data[offset]; }, [&](int64_t offset) { return base + offset - first; }, code_at,
            mapped ? labels : py::dict<int64_t, py::str>(), functions, text,
            [](const py::list<int64_t>& data) { return w::data_lines(data); });
        out.append(py::str(""));
    }
    py::str source = w::text(out) + py::str("\n");
    py::dict<py::str, w::Extra> more;
    more.set(py::str("rom bytes"), w::Extra(py::len(rom->data)));
    w::Result r = w::result<Insn>(folder, stem + py::str(".asm"), source, functions, code, nullptr, more);
    w::assembled(
        r, [&]() { return build::z80_asm::build(source, folder); }, rom->data,
        py::os::path::join(folder, stem + py::str(".built") + std::get<1>(py::os::path::splitext(path))));
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    return w::main(argv, reconstruct, DOC);
}

}  // namespace codebreaker::source::sms_writer
