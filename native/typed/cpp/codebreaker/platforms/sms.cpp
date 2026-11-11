// codebreaker/platforms/sms.cpp -- C++17 port of codebreaker.platforms.sms (see sms.hpp).
#include "codebreaker/platforms/sms.hpp"

#include "codebreaker/formats/sms.hpp"

namespace codebreaker::platforms::sms {

bool _recognise(const py::str& path) {
    return formats::sms::is_sms(path);
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

// platforms.machine('sms', ...)
static Platform make_platform() {
    Cpu cpu;
    cpu.language = py::str("z80:LE:16:default");
    cpu.bits = 8;
    cpu.endian = py::str("little");
    cpu.decoder = Cpu::Module{py::str("codebreaker.decompile.z80")};
    cpu.registers = strs({"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"});
    cpu.stack = py::str("SP");
    return machine(py::str("sms"), py::str("Master System / Game Gear"), py::str("z80"), py::str("cartridge image (.sms, .gg, .sg)"), cpu,
                   [](const py::str& path) { return _recognise(path); },
                   {py::str("codebreaker.formats.sms"), py::str("read")}, {py::str("codebreaker.emulate.z80"), py::str("recorded")},
                   {py::str("codebreaker.build.z80_asm"), py::str("build")}, {py::str("codebreaker.source.sms_writer"), py::str("reconstruct")}, strs({"A", "F", "BC", "DE", "HL", "IX", "IY"}),
                   py::str("CodeBreaker's own assembler, or WLA-DX"));
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::sms
