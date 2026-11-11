// codebreaker/source/zx_writer.cpp -- see zx_writer.hpp.

#include "codebreaker/source/zx_writer.hpp"

#include <algorithm>

#include "codebreaker/formats/zx.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py_os.hpp"

namespace codebreaker::source::zx_writer {

const py::str DOC(R"DOC(zx_writer.py -- a ZX Spectrum program as assembly source that builds back into it.

    python -m codebreaker.source.zx_writer <game.z80|.sna|.tap> [<out folder>]
    from codebreaker.source import zx_writer
    r = zx_writer.reconstruct(path, folder)     # as source/z80_writer.py's write()

The program block (formats/zx.py: a snapshot's RAM above the screen, a tape's code block) is
written through source/z80_writer.py, with the Spectrum's names: its ULA port, the 48K ROM's
documented entry points and the system variables, as the Spectrum manual and The Complete
Spectrum ROM Disassembly name them.
)DOC");

const py::dict<int64_t, py::str> PORTS = {{0xFE, py::str("ULA")}, {0x1F, py::str("KEMPSTON")}};
const py::dict<int64_t, py::str> NAMES = {
    {0x0010, py::str("PRINT_A")},  {0x03B5, py::str("BEEPER")},  {0x04C2, py::str("SA_BYTES")}, {0x0556, py::str("LD_BYTES")},
    {0x0D6B, py::str("CLS")},      {0x0DAF, py::str("CL_ALL")},  {0x1601, py::str("CHAN_OPEN")}, {0x203C, py::str("PR_STRING")},
    {0x2D2B, py::str("STACK_BC")}, {0x2DE3, py::str("PRINT_FP")}, {0x5C08, py::str("LAST_K")},   {0x5C36, py::str("CHARS")},
    {0x5C3B, py::str("FLAGS")},    {0x5C48, py::str("BORDCR")},  {0x5C4B, py::str("VARS")},     {0x5C4F, py::str("CHANS")},
    {0x5C53, py::str("PROG")},     {0x5C76, py::str("SEED")},    {0x5C78, py::str("FRAMES")},   {0x5C8D, py::str("ATTR_P")},
    {0x5CB2, py::str("RAMTOP")}};

z80_writer::Written reconstruct(const py::str& path, const py::str& folder) {
    formats::zx::Program p = formats::zx::read(path);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    py::list<py::str> header;
    header.append(py::percent(py::str("ZX Spectrum %s %s; %d bytes at $%04X"), p.kind, py::os::path::basename(path), py::len(p.image), p.origin));
    if (!p.registers.empty()) {
        // sorted(p.registers.items()): by name, the values after it never compared (the names are all different)
        std::vector<std::tuple<py::str, int64_t>> items = p.registers.items();
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        py::list<py::str> parts;
        for (const auto& kv : items) {
            const py::str& k = std::get<0>(kv);
            if (k.endswith(py::str("_"))) continue;
            bool wide = k == py::str("sp") || k == py::str("ix") || k == py::str("iy");
            // "%s=%0*X" % (k.upper(), 4 if k in ("sp", "ix", "iy") else 2, v)
            parts.append(py::percent(py::str(wide ? "%s=%04X" : "%s=%02X"), k.upper(), std::get<1>(kv)));
        }
        header.append(py::str("registers at the snapshot: ") + py::join(py::str(" "), parts));
    }
    return z80_writer::write(p.image, p.origin, p.entries, folder, stem, PORTS, NAMES, header);
}

int64_t main(const py::list<py::str>& argv) {
    return image_writer::main(
        argv, [](const py::str& path, const py::str& folder) -> image_writer::Result { return reconstruct(path, folder); }, DOC, py::str("bytes"), py::str("bytes"));
}

}  // namespace codebreaker::source::zx_writer
