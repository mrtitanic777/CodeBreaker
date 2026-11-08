// codebreaker/platforms/dos.cpp -- typed C++ of codebreaker.platforms.dos (see dos.hpp).
#include "codebreaker/platforms/dos.hpp"

#include <optional>

#include "codebreaker/build/driver.hpp"
#include "codebreaker/formats/detect.hpp"

namespace codebreaker::platforms::dos {

bool _recognise(const py::str& path) {
    const std::optional<py::str> k = formats::detect::kind(path);
    return k && *k == py::str("dos");
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
    return Ref{py::str(module), py::str(name), strs(args)};
}

static Platform make_platform() {
    const char* here = "codebreaker.platforms.dos";
    Platform p;
    p.name = "dos";
    p.title = "DOS programs";
    p.family = "x86";
    p.source = "asm";
    p.lane = "codebreaker.verify.lanes.dos";
    p.loader.container = "MZ, COM, SYS";
    p.loader.open = ref("codebreaker.formats.dos", "read");
    p.loader.parts = ref("codebreaker.formats.dos", "chain");
    p.cpu.language = "x86:LE:16:Real Mode:default";
    p.cpu.bits = 16;
    p.cpu.endian = "little";
    p.cpu.decoder = std::make_tuple(py::str("capstone"), py::str("x86"), int64_t(16));
    p.cpu.registers = strs({"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"});
    p.cpu.stack = "SP";
    p.convention.names = strs({"__cdecl16near", "__cdecl16far"});
    p.convention.word = 2;
    p.convention.order = "c";
    p.convention.results = strs({"AX", "DX:AX"});
    p.convention.pops = "caller";
    p.compiler.language = "asm";
    p.compiler.toolchain = "openwatcom wasm";
    p.compiler.tool = ref("codebreaker.build.driver", "tool", {"wasm"});
    p.compiler.tools = ref(here, "tools");
    p.packager.writer = ref("codebreaker.source.dos_writer", "reconstruct");
    p.packager.tools = ref(here, "tools");
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::dos
