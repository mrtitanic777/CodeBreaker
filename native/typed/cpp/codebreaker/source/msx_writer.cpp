// codebreaker/source/msx_writer.cpp -- typed C++ of codebreaker.source.msx_writer.

#include "codebreaker/source/msx_writer.hpp"

#include "codebreaker/formats/msx.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py_os.hpp"

namespace codebreaker::source::msx_writer {

namespace msx = codebreaker::formats::msx;

const py::str DOC(R"DOC(msx_writer.py -- an MSX cartridge as assembly source that builds back into it.

    python -m codebreaker.source.msx_writer <game.rom> [<out folder>]
    from codebreaker.source import msx_writer
    r = msx_writer.reconstruct(path, folder)    # as source/z80_writer.py's write()

The cartridge (formats/msx.py) is written through source/z80_writer.py from its header's entry
points, with the MSX's names: the VDP, PSG and PPI ports and the BIOS's documented entry points
and system variables, as the MSX2 Technical Handbook names them.
)DOC");

const py::dict<int64_t, py::str> PORTS = {{0x98, py::str("VDP_DATA")}, {0x99, py::str("VDP_CTRL")}, {0xA0, py::str("PSG_ADDR")},
                                          {0xA1, py::str("PSG_WRITE")}, {0xA2, py::str("PSG_READ")}, {0xA8, py::str("PPI_A")},
                                          {0xA9, py::str("PPI_B")},     {0xAA, py::str("PPI_C")},     {0xAB, py::str("PPI_CTRL")}};
const py::dict<int64_t, py::str> NAMES = {
    {0x000C, py::str("RDSLT")},   {0x0014, py::str("WRSLT")},   {0x001C, py::str("CALSLT")}, {0x0024, py::str("ENASLT")},
    {0x0030, py::str("CALLF")},   {0x0041, py::str("DISSCR")},  {0x0044, py::str("ENASCR")}, {0x0047, py::str("WRTVDP")},
    {0x004A, py::str("RDVRM")},   {0x004D, py::str("WRTVRM")},  {0x0050, py::str("SETRD")},  {0x0053, py::str("SETWRT")},
    {0x0056, py::str("FILVRM")},  {0x0059, py::str("LDIRMV")},  {0x005C, py::str("LDIRVM")}, {0x005F, py::str("CHGMOD")},
    {0x0062, py::str("CHGCLR")},  {0x0069, py::str("CLRSPR")},  {0x006C, py::str("INITXT")}, {0x006F, py::str("INIT32")},
    {0x0072, py::str("INIGRP")},  {0x0075, py::str("INIMLT")},  {0x0090, py::str("GICINI")}, {0x0093, py::str("WRTPSG")},
    {0x0096, py::str("RDPSG")},   {0x009C, py::str("CHSNS")},   {0x009F, py::str("CHGET")},  {0x00A2, py::str("CHPUT")},
    {0x00C3, py::str("CLS")},     {0x00D5, py::str("GTSTCK")},  {0x00D8, py::str("GTTRIG")}, {0x0138, py::str("RSLREG")},
    {0x013B, py::str("WSLREG")},  {0xF3AE, py::str("LINL40")},  {0xF3DF, py::str("RG0SAV")}, {0xFC9E, py::str("JIFFY")},
    {0xFCC1, py::str("EXPTBL")},  {0xFD9F, py::str("H_TIMI")}};

z80_writer::Written reconstruct(const py::str& path, const py::str& folder) {
    py::ref<msx::Program> p = msx::read(path);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    const msx::ReadHeader& h = p->header;
    py::list<py::str> header{py::percent("MSX cartridge %s; %d KB at $%04X; INIT $%04X STATEMENT $%04X DEVICE $%04X TEXT $%04X",
                                         py::os::path::basename(path), py::len(p->image) / 1024, p->origin, h.init, h.statement, h.device,
                                         h.text)};
    return z80_writer::write(p->image, p->origin, p->entries, folder, stem, PORTS, NAMES, header);
}

int64_t main(const py::list<py::str>& argv) {
    return image_writer::main(
        argv, [](const py::str& path, const py::str& folder) -> image_writer::Result { return reconstruct(path, folder); }, DOC,
        py::str("bytes"), py::str("bytes"));
}

}  // namespace codebreaker::source::msx_writer
