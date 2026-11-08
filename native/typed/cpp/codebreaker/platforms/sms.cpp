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

static Platform make_platform() {
    Platform p;
    p.name = "sms";
    p.title = "Master System / Game Gear";
    p.family = "z80";
    p.source = "asm";
    p.lane = py::str("codebreaker.verify.lanes.") + p.name;
    p.cpu.language = "z80:LE:16:default";
    p.cpu.bits = 8;
    p.cpu.endian = "little";
    p.cpu.decoder = "codebreaker.decompile.z80";
    p.cpu.registers = strs({"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"});
    p.cpu.stack = "SP";
    p.container = "cartridge image (.sms, .gg, .sg)";
    p.reader = Ref{"codebreaker.formats.sms", "read"};
    p.engine = "CodeBreaker's own interpreter";
    p.runner = Ref{"codebreaker.emulate.z80", "recorded"};
    p.language = "asm";
    p.toolchain = "CodeBreaker's own assembler, or WLA-DX";
    p.assembler = Ref{"codebreaker.build.z80_asm", "build"};
    p.writer = Ref{"codebreaker.source.sms_writer", "reconstruct"};
    p.convention_names = strs({"registers"});
    p.convention_arguments = "registers";
    p.convention_order = "registers";
    p.results = strs({"A", "F", "BC", "DE", "HL", "IX", "IY"});
    p.pops = "caller";
    return p;
}

const Platform PLATFORM = make_platform();

}  // namespace codebreaker::platforms::sms
