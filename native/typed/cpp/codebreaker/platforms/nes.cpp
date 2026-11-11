// codebreaker/platforms/nes.cpp -- C++17 port of codebreaker.platforms.nes (see nes.hpp).
#include "codebreaker/platforms/nes.hpp"

#include "codebreaker/formats/nes.hpp"

namespace codebreaker::platforms::nes {

bool _recognise(const py::str& path) {
    return formats::nes::is_nes(path);
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

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

// platforms.machine('nes', ...)
static Platform make_platform() {
    Cpu cpu;
    cpu.language = py::str("6502:LE:16:NES:default");
    cpu.bits = 8;
    cpu.endian = py::str("little");
    cpu.decoder = Cpu::Module{py::str("codebreaker.decompile.m6502")};
    cpu.registers = strs({"A", "X", "Y", "P", "S"});
    cpu.stack = py::str("S");
    return machine(py::str("nes"), py::str("NES"), py::str("6502"), py::str("iNES (.nes)"), cpu,
                   [](const py::str& path) { return _recognise(path); },
                   {py::str("codebreaker.formats.nes"), py::str("read")}, {py::str("codebreaker.emulate.m6502"), py::str("recorded")},
                   {py::str("codebreaker.build.m6502_asm"), py::str("build")}, {py::str("codebreaker.source.nes_writer"), py::str("reconstruct")}, strs({"A", "X", "Y", "P"}),
                   py::str("CodeBreaker's own assembler, or cc65"));
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::nes
