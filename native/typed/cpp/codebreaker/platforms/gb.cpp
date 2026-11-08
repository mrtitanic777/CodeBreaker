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

static Platform make_platform() {
    Platform p;
    p.name = "gb";
    p.title = "Game Boy / Game Boy Color";
    p.family = "sm83";
    p.source = "asm";
    p.lane = py::str("codebreaker.verify.lanes.") + p.name;
    p.cpu.language = "sm83:LE:16:default";
    p.cpu.bits = 8;
    p.cpu.endian = "little";
    p.cpu.decoder = "codebreaker.decompile.sm83";
    p.cpu.registers = strs({"A", "F", "BC", "DE", "HL", "SP"});
    p.cpu.stack = "SP";
    p.container = "cartridge image (.gb, .gbc)";
    p.reader = Ref{"codebreaker.formats.gb", "read"};
    p.engine = "CodeBreaker's own interpreter";
    p.runner = Ref{"codebreaker.emulate.sm83", "recorded"};
    p.language = "asm";
    p.toolchain = "CodeBreaker's own assembler, or RGBDS";
    p.assembler = Ref{"codebreaker.build.sm83_asm", "build"};
    p.writer = Ref{"codebreaker.source.gb_writer", "reconstruct"};
    p.convention_names = strs({"registers"});
    p.convention_arguments = "registers";
    p.convention_order = "registers";
    p.results = strs({"A", "F", "BC", "DE", "HL"});
    p.pops = "caller";
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::gb
