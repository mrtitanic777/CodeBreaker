// codebreaker/source/genesis_writer.cpp -- typed C++ of codebreaker.source.genesis_writer.

#include "codebreaker/source/genesis_writer.hpp"

#include <functional>

#include "codebreaker/build/m68k_asm.hpp"

namespace codebreaker::source::image_writer {
// image_writer's templates read an instruction's size from `insn.size`; the 68000's Insn keeps the operand size
// there (a letter) and its length in `length`, which is what the Python passes as `size=_length` and what the
// default `_size` has to be for this instruction type too.
template <> inline int64_t _size<decompile::m68k::Insn>(const decompile::m68k::Insn& insn) {
    return insn.length;
}
}  // namespace codebreaker::source::image_writer

namespace codebreaker::source::genesis_writer {

namespace m68k = codebreaker::decompile::m68k;
namespace genesis = codebreaker::formats::genesis;
namespace w = codebreaker::source::image_writer;
using Insn = m68k::Insn;

const py::str DOC(R"DOC(genesis_writer.py -- a Genesis / Mega Drive program as assembly source that builds back into it.

    python -m codebreaker.source.genesis_writer <game.md> [<out folder>]
    from codebreaker.source import genesis_writer
    r = genesis_writer.reconstruct(path, folder)
    r["source"]                     game.asm (Motorola syntax: asm68k, vasm)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

Genesis programs were written in 68000 assembly (most of them; some in C, compiled), and
assembly is what comes back. Code is what the program's own flow reaches from the reset vector
and the exception and interrupt handlers the vector table names (formats/genesis.py); every
byte it reaches is written as an instruction, every other byte as data. The vector table is
written as `dc.l` lines naming the handlers, the header at $100 as data with its fields named.
Functions are named for their vector (`reset`, `vblank`, `hblank`, ...) or `sub_<address>`, jump
targets `L_<address>`, and absolute and PC-relative operands naming the start of a line get that
line's label -- so the source can be moved and edited, not only rebuilt. The console's ports
(VDP, I/O, Z80 bus, TMSS) are written by name.

It builds with CodeBreaker's own assembler (build/m68k_asm.py), or with asm68k / vasm.
)DOC");

// the console's registers, by the names the hardware documentation gives them
const py::dict<int64_t, py::str> HARDWARE = {
    {0xC00000, py::str("VDP_DATA")},  {0xC00004, py::str("VDP_CTRL")},  {0xC00008, py::str("VDP_HVCOUNTER")}, {0xC00011, py::str("PSG")},
    {0xA00000, py::str("Z80_RAM")},   {0xA04000, py::str("YM2612")},    {0xA10001, py::str("IO_VERSION")},   {0xA10003, py::str("IO_DATA1")},
    {0xA10005, py::str("IO_DATA2")},  {0xA10007, py::str("IO_DATA3")},  {0xA10009, py::str("IO_CTRL1")},     {0xA1000B, py::str("IO_CTRL2")},
    {0xA1000D, py::str("IO_CTRL3")},  {0xA11100, py::str("Z80_BUSREQ")}, {0xA11200, py::str("Z80_RESET")},    {0xA14000, py::str("TMSS")},
    {0xA130F1, py::str("SRAM_CTRL")}};
const py::list<std::tuple<int64_t, int64_t, py::str>> _HEADER = {
    {0x100, 16, py::str("system")},          {0x110, 16, py::str("copyright")},
    {0x120, 48, py::str("domestic name")},   {0x150, 48, py::str("overseas name")},
    {0x180, 14, py::str("serial and version")}, {0x18E, 2, py::str("checksum")},
    {0x190, 16, py::str("devices")},         {0x1A0, 8, py::str("ROM start and end")},
    {0x1A8, 8, py::str("RAM start and end")}, {0x1B0, 12, py::str("backup RAM")},
    {0x1BC, 12, py::str("modem")},           {0x1C8, 40, py::str("notes")},
    {0x1F0, 16, py::str("regions")}};

namespace {
py::dict<int64_t, std::tuple<int64_t, py::str>> header_at() {
    py::dict<int64_t, std::tuple<int64_t, py::str>> d;
    for (const auto& [at, n, what] : _HEADER) {
        d.set(at, std::make_tuple(n, what));
    }
    return d;
}
}  // namespace

const py::dict<int64_t, std::tuple<int64_t, py::str>> _HEADER_AT = header_at();

int64_t _length(const Insn& insn) {
    return insn.length;
}

std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const genesis::ROM& rom) {
    const py::bytes& mem = rom.data;
    int64_t limit = py::len(mem);
    return w::walk<Insn>(
        rom.functions(), [&](int64_t pc) { return m68k::decode(mem, pc, pc); },
        [&](int64_t pc) { return 0x200 <= pc && pc < limit; },
        [&](int64_t pc, const Insn& insn) { return pc + insn.length <= limit; },
        [&](const Insn& insn) {
            int64_t target = *insn.target;
            return 0x200 <= target && target < limit && target % 2 == 0 && py::truthy(insn.flow) && insn.mnemonic != py::str("TRAP");
        },
        [&](const Insn& insn) {
            return insn.flow == py::str("jump") || insn.flow == py::str("ret") || insn.flow == py::str("indirect") ||
                   insn.flow == py::str("halt");
        },
        nullptr, nullptr, _length);
}

py::list<std::tuple<int64_t, py::str>> _refs(const Insn& insn) {
    py::list<std::tuple<int64_t, py::str>> out;
    for (const m68k::Op& o : insn.operands) {
        if (o.kind == py::str("absw") || o.kind == py::str("absl") || o.kind == py::str("pcdisp") || o.kind == py::str("pcindex")) {
            out.append(std::make_tuple(o.num() & 0xFFFFFFFF, o.kind));
        } else if (o.kind == py::str("imm") && o.size_is("L")) {
            out.append(std::make_tuple(o.num(), o.kind));
        }
    }
    return out;
}

w::Result reconstruct(const py::str& path, const py::str& folder) {
    py::ref<genesis::ROM> rom = genesis::read(path);
    py::os::makedirs(folder, 0777, true);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    auto [code, targets, starts] = _walk(*rom);
    // a handler is named for its vector when one vector names it (or it is the reset, vblank or hblank
    // handler); one many vectors share is the program's catch-all
    py::dict<int64_t, py::list<py::str>> sharing;
    for (const auto& [name, at] : rom->vectors.items()) {
        sharing.setdefault(at, py::list<py::str>()).append(name);
    }
    py::dict<int64_t, py::str> names;
    for (const auto& [at, vecs] : sharing.items()) {
        if (!starts.contains(at) || (vecs.contains(py::str("ssp")) && py::len(vecs) == 1)) {
            continue;
        }
        py::list<py::str> main_;
        for (const py::str& v : vecs) {
            if (v == py::str("reset") || v == py::str("vblank") || v == py::str("hblank") || v == py::str("external")) {
                main_.append(v);
            }
        }
        names.set(at, py::len(vecs) > 2 ? py::str("exception") : py::truthy(main_) ? main_[0] : vecs[0]);
    }
    py::dict<int64_t, w::Function> functions = w::functions(starts, names, py::str("sub_%06X"));
    py::set<int64_t> covered = w::covered<Insn>(code, _length);
    int64_t limit = py::len(rom->data);

    auto line_start = [&](int64_t a) { return 0x200 <= a && a < limit && !covered.contains(a) && a % 2 == 0; };
    py::dict<int64_t, py::str> labels = w::labels(targets, names, line_start, py::str("L_%06X"));
    py::set<int64_t> used;
    for (const Insn& insn : code.values()) {
        for (const auto& [v, kind] : _refs(insn)) {
            if (HARDWARE.contains(v)) {
                used.add(v);
            } else if (line_start(v) && (kind != py::str("imm") || code.contains(v) || v >= 0x400)) {
                labels.setdefault(v, names.get(v, py::percent(code.contains(v) ? "L_%06X" : "D_%06X", v)));
            }
        }
    }
    py::dict<int64_t, py::str> hardware;
    for (int64_t a : py::sorted(used)) {
        hardware.set(a, HARDWARE.at(a));
    }
    const genesis::Header& h = rom->header;
    w::Assembly out = w::lines(
        py::list<py::str>{py::percent("; %s -- written by CodeBreaker from %s", stem + py::str(".asm"), py::os::path::basename(path)),
                          py::percent("; \"%s\" / \"%s\", %s, %d KB ROM%s", h.domestic_name, h.overseas_name, h.serial,
                                      py::len(rom->data) / 1024, rom->smd ? py::str(", from an .smd dump") : py::str("")),
                          py::str("")},
        py::str("asm68k"), py::os::path::basename(path));
    for (const auto& [a, n] : hardware.items()) {
        out.append(py::percent("%-16s equ $%06X", n, a));
    }
    if (py::truthy(hardware)) {
        out.append(py::str(""));
    }
    py::dict<int64_t, py::str> text_names = labels.copy();
    text_names.update(hardware);
    // the vector table: 64 longs
    out.part(py::str("vectors"), py::str("vector table"), std::nullopt, 0x100);
    out.append(py::str("        org $0"));
    for (int64_t i = 0; i < 64; ++i) {
        // int.from_bytes(rom.data[i * 4:i * 4 + 4], "big")
        py::bytes word = rom->data.slice(i * 4, i * 4 + 4);
        int64_t v = 0;
        for (int64_t k = 0; k < py::len(word); ++k) {
            v = (v << 8) | word[k];
        }
        std::optional<py::str> name = i ? labels.get(v) : std::nullopt;
        py::str what = i == 0 ? py::str("initial stack pointer") : i == 1 ? py::str("reset") : genesis::VECTOR_NAMES.get(i, py::str(""));
        out.append(py::percent("        dc.l %s%s", name.has_value() ? *name : py::percent("$%08X", v),
                               py::truthy(what) ? py::str("  ; ") + what : py::str("")));
    }

    auto lines = [](const py::list<int64_t>& data) { return w::data_lines(data, py::str("dc.b"), py::str("dcb.b %d,$%02X")); };

    auto header = [&](int64_t offset, int64_t /*addr*/, const std::function<void()>& flush) -> int64_t {
        if (!_HEADER_AT.contains(offset)) {
            return 0;
        }
        flush();
        auto [n, what] = _HEADER_AT.at(offset);
        py::bytes field = rom->data.slice(offset, offset + n);
        bool printable = true;
        for (int64_t k = 0; k < py::len(field); ++k) {
            int64_t b = field[k];
            if (!(32 <= b && b < 127 && b != 0x22)) {
                printable = false;
                break;
            }
        }
        py::list<py::str> text;
        if (printable) {
            text.append(py::percent("        dc.b \"%s\"", field.decode("ascii")));   // a text field as its text
        } else {
            text = lines(py::list<int64_t>::from(field));
        }
        out.extend(w::commented(text, what));
        return n;
    };
    w::emit<Insn>(
        out, 0x100, limit, [&](int64_t offset) -> int64_t { return rom->data[offset]; }, [](int64_t offset) { return offset; },
        [&](int64_t offset, int64_t) { return code.get(offset); }, labels, functions,
        [&](const Insn& insn) { return m68k::text(insn, text_names); }, lines, header, _length);
    py::str source = w::text(out) + py::str("\n");
    py::dict<py::str, w::Extra> more;
    more.set(py::str("rom bytes"), w::Extra(py::len(rom->data)));
    w::Result r = w::result<Insn>(folder, stem + py::str(".asm"), source, functions, code, _length, more);
    w::assembled(
        r, [&]() { return build::m68k_asm::build(source); }, rom->data, py::os::path::join(folder, stem + py::str(".built.md")));
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    return w::main(argv, reconstruct, DOC);
}

}  // namespace codebreaker::source::genesis_writer
