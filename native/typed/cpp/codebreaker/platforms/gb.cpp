// codebreaker/platforms/gb.cpp -- C++17 port of codebreaker.platforms.gb (see gb.hpp).
#include "codebreaker/platforms/gb.hpp"

#include "codebreaker/formats/gb.hpp"

namespace codebreaker::platforms::gb {

bool _recognise(const py::str& path) {
    return formats::gb::is_gb(path);
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

// platforms.machine('gb', ...)
static Platform make_platform() {
    Cpu cpu;
    cpu.language = py::str("sm83:LE:16:default");
    cpu.bits = 8;
    cpu.endian = py::str("little");
    cpu.decoder = Cpu::Module{py::str("codebreaker.decompile.sm83")};
    cpu.registers = strs({"A", "F", "BC", "DE", "HL", "SP"});
    cpu.stack = py::str("SP");
    return machine(py::str("gb"), py::str("Game Boy / Game Boy Color"), py::str("sm83"), py::str("cartridge image (.gb, .gbc)"), cpu,
                   [](const py::str& path) { return _recognise(path); },
                   {py::str("codebreaker.formats.gb"), py::str("read")}, {py::str("codebreaker.emulate.sm83"), py::str("recorded")},
                   {py::str("codebreaker.build.sm83_asm"), py::str("build")}, {py::str("codebreaker.source.gb_writer"), py::str("reconstruct")}, strs({"A", "F", "BC", "DE", "HL"}),
                   py::str("CodeBreaker's own assembler, or RGBDS"));
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::gb
