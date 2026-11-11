// codebreaker/platforms/msx.cpp -- C++17 port of codebreaker.platforms.msx (see msx.hpp).
#include "codebreaker/platforms/msx.hpp"

#include "codebreaker/formats/msx.hpp"

namespace codebreaker::platforms::msx {

bool _recognise(const py::str& path) {
    return formats::msx::is_msx(path);
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

// platforms.machine('msx', ...)
static Platform make_platform() {
    Cpu cpu;
    cpu.language = py::str("z80:LE:16:default");
    cpu.bits = 8;
    cpu.endian = py::str("little");
    cpu.decoder = Cpu::Module{py::str("codebreaker.decompile.z80")};
    cpu.registers = strs({"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"});
    cpu.stack = py::str("SP");
    return machine(py::str("msx"), py::str("MSX"), py::str("z80"), py::str("cartridge image (.rom)"), cpu,
                   [](const py::str& path) { return _recognise(path); },
                   {py::str("codebreaker.formats.msx"), py::str("read")}, {py::str("codebreaker.emulate.z80"), py::str("recorded")},
                   {py::str("codebreaker.build.z80_asm"), py::str("build_flat")}, {py::str("codebreaker.source.msx_writer"), py::str("reconstruct")}, strs({"A", "F", "BC", "DE", "HL", "IX", "IY"}),
                   py::str("CodeBreaker's own assembler, or sjasmplus / pasmo"));
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::msx
