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

// platforms.machine("nes", "NES", "6502", "iNES (.nes)", Cpu(...), ...)
static Platform make_platform() {
    Platform p;
    p.name = "nes";
    p.title = "NES";
    p.family = "6502";
    p.source = "asm";
    p.lane = py::str("codebreaker.verify.lanes.") + p.name;
    p.cpu.language = "6502:LE:16:NES:default";
    p.cpu.bits = 8;
    p.cpu.endian = "little";
    p.cpu.decoder = "codebreaker.decompile.m6502";
    p.cpu.registers = strs({"A", "X", "Y", "P", "S"});
    p.cpu.stack = "S";
    p.container = "iNES (.nes)";
    p.reader = Ref{"codebreaker.formats.nes", "read"};
    p.engine = "CodeBreaker's own interpreter";
    p.runner = Ref{"codebreaker.emulate.m6502", "recorded"};
    p.language = "asm";
    p.toolchain = "CodeBreaker's own assembler, or cc65";
    p.assembler = Ref{"codebreaker.build.m6502_asm", "build"};
    p.writer = Ref{"codebreaker.source.nes_writer", "reconstruct"};
    p.convention_names = strs({"registers"});
    p.convention_arguments = "registers";
    p.convention_order = "registers";
    p.results = strs({"A", "X", "Y", "P"});
    p.pops = "caller";
    return p;
}

const Platform PLATFORM = make_platform();

}  // namespace codebreaker::platforms::nes
