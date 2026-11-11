// codebreaker/platforms/genesis.cpp -- C++17 port of codebreaker.platforms.genesis (see genesis.hpp).
#include "codebreaker/platforms/genesis.hpp"

#include "codebreaker/formats/genesis.hpp"

namespace codebreaker::platforms::genesis {

bool _recognise(const py::str& path) {
    return formats::genesis::is_genesis(path);
}

bool recognises(const Platform& platform, const py::str& path) {
    (void)platform;  // this platform's _recognise is the one above
    try {
        return _recognise(path);
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

// platforms.machine("genesis", ...)
static Platform make_platform() {
    Cpu cpu;
    cpu.language = py::str("68000:BE:32:default");
    cpu.bits = 32;
    cpu.endian = py::str("big");
    cpu.decoder = Cpu::Module{py::str("codebreaker.decompile.m68k")};
    // tuple("D%d" % i for i in range(8)) + tuple("A%d" % i for i in range(8)) + ("SR",)
    py::list<py::str> regs;
    for (int64_t i = 0; i < 8; ++i) regs.append(py::percent("D%d", i));
    for (int64_t i = 0; i < 8; ++i) regs.append(py::percent("A%d", i));
    regs.append(py::str("SR"));
    cpu.registers = regs;
    cpu.stack = py::str("A7");
    // results: D0-D7 and A0-A6
    py::list<py::str> results;
    for (int64_t i = 0; i < 8; ++i) results.append(py::percent("D%d", i));
    for (int64_t i = 0; i < 7; ++i) results.append(py::percent("A%d", i));
    return machine(py::str("genesis"), py::str("Genesis / Mega Drive"), py::str("68000"), py::str("cartridge image (.md, .bin, .gen, .smd)"), cpu,
                   [](const py::str& path) { return _recognise(path); },
                   {py::str("codebreaker.formats.genesis"), py::str("read")}, {py::str("codebreaker.emulate.m68k"), py::str("recorded")},
                   {py::str("codebreaker.build.m68k_asm"), py::str("build")}, {py::str("codebreaker.source.genesis_writer"), py::str("reconstruct")},
                   results, py::str("CodeBreaker's own assembler"));
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::genesis
