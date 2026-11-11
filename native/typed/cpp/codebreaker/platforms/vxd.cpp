// codebreaker/platforms/vxd.cpp -- typed C++ of codebreaker.platforms.vxd (see vxd.hpp).
#include "codebreaker/platforms/vxd.hpp"

#include "codebreaker/build/driver.hpp"
#include "codebreaker/formats/detect.hpp"

namespace codebreaker::platforms::vxd {

bool _recognise(const py::str& path) {
    return formats::detect::holds_drivers(path, py::list<py::bytes>{py::bytes("W3")});
}

bool recognises(const py::str& path) {
    try {
        return _recognise(path);
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

py::dict<py::str, formats::le::Place> entries(const py::str& path) {
    const auto members = formats::le::members(path);
    return formats::le::entry_points(std::get<1>(members[0]));
}

py::dict<py::str, py::str> tools() {
    py::dict<py::str, py::str> out;
    for (const char* t : {"wasm", "wlink"}) out.set(py::str(t), build::driver::tool(py::str(t)));
    return out;
}

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

static Ref ref(const char* module, const char* name, std::initializer_list<const char*> args = {}) {
    return Ref(py::str(module), py::str(name), strs(args));
}

static Platform make_platform() {
    const char* here = "codebreaker.platforms.vxd";
    Platform p;
    p.name = "le";
    p.title = "device drivers";
    p.family = "x86";
    p.source = "asm";
    p.lane = "codebreaker.verify.lanes.driver";
    p.loader.container = "LE";
    p.loader.open = ref("codebreaker.formats.le", "members");
    p.loader.entries = ref(here, "entries");
    p.loader.parts = ref("codebreaker.formats.le", "members");
    p.cpu.language = "x86:LE:32:default:windows";
    p.cpu.bits = 32;
    p.cpu.endian = "little";
    p.cpu.decoder = Cpu::Capstone{py::str("x86"), 32};
    p.cpu.registers = strs({"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"});
    p.cpu.stack = "ESP";
    p.convention.names = strs({"registers"});
    p.convention.arguments = py::str("registers");
    p.convention.word = 4;
    p.convention.order = "registers";
    p.convention.results = strs({"EAX", "CF", "ZF"});
    p.convention.pops = "callee";
    p.convention.preserved = strs({"EBX", "ECX", "EDX", "ESI", "EDI", "EBP"});
    p.emulator.engine = "unicorn";
    p.emulator.run = ref("codebreaker.emulate.driver", "recorded");
    p.compiler.language = "asm";
    p.compiler.toolchain = "openwatcom wasm, the DDK's macros";
    p.compiler.tool = ref("codebreaker.build.driver", "tool", {"wasm"});
    p.compiler.tools = ref(here, "tools");
    p.packager.writer = ref("codebreaker.source.driver_writer", "reconstruct");
    p.packager.functions = ref("codebreaker.build.driver", "build");
    p.packager.tools = ref(here, "tools");
    p.recognise_fn = [](const py::str& path) { return _recognise(path); };
    p.finish();
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::vxd
