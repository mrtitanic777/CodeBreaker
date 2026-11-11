// codebreaker/platforms/win16.cpp -- typed C++ of codebreaker.platforms.win16 (see win16.hpp).
#include "codebreaker/platforms/win16.hpp"

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms::win16 {

py::list<formats::ne::Segment> segments(const py::str& path) { return formats::ne::NEFile(path).segments(); }

py::list<formats::ne::Entry> entries(const py::str& path) { return formats::ne::NEFile(path).entries(); }

std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t ordinal, const Runner& run) {
    py::list<int64_t> args;
    for (int64_t i = 0; i < PLATFORM().convention.arguments; i++) args.append(0);   // [0] * arguments
    try {
        RunRecord r = run(path, ordinal, args);
        return {r.ended, r.results.get(py::str("AX"))};
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {                 // the emulator giving up is a result too
        return {py::str("error: ") + py::str(e.type()), std::nullopt};
    }
}

py::str library() { return py::os::path::join(build::watcom::ow_lib(py::str("ne")), py::str("clibm.lib")); }

py::dict<py::str, py::str> tools() {
    py::dict<py::str, py::str> out;
    for (const char* t : {"wcc", "wlink", "wasm"}) {
        py::str p = build::watcom::ow_tool(py::str(t));
        out.set(py::str(t), p);   // (`ow_tool(t) or ""`: an unfound tool is "" already)
    }
    return out;
}

bool _recognise(const py::str& path) {
    auto lane = base::corpus::lane(path);
    return lane && *lane == py::str("ne");
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

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

static Ref ref(const char* module, const char* name, std::initializer_list<const char*> args = {}) {
    return Ref{py::str(module), py::str(name), strs(args)};
}

static Platform make_platform() {
    const char* here = "codebreaker.platforms.win16";
    Platform p;
    p.name = "ne";
    p.title = "16-bit Windows";
    p.family = "x86";
    p.source = "c or asm";
    p.lane = "codebreaker.verify.lanes.win16";
    p.steps = "codebreaker.engine.win16";
    p.loader.container = "NE";
    p.loader.open = ref("codebreaker.formats.ne", "NEFile");
    p.loader.parts = ref(here, "segments");
    p.loader.entries = ref(here, "entries");
    p.loader.exports = ref("codebreaker.build.definitions", "exports");
    p.loader.imports = ref("codebreaker.build.definitions", "imports");
    p.cpu.language = "x86:LE:16:Protected Mode:default";
    p.cpu.bits = 16;
    p.cpu.endian = "little";
    p.cpu.decoder = std::make_tuple(py::str("capstone"), py::str("x86"), int64_t(16));
    p.cpu.registers = strs({"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"});
    p.cpu.stack = "SP";
    p.convention.names = strs({"__stdcall16far", "__stdcall16near", "__cdecl16far", "__cdecl16near", "__fastcall16near",
                               "__fastcall16far"});
    p.convention.arguments = 8;
    p.convention.word = 2;
    p.convention.order = "pascal";
    p.convention.results = strs({"AX", "DX:AX"});
    p.convention.pops = "either";
    p.convention.preserved = strs({"SI", "DI", "DS"});
    p.emulator.engine = "unicorn";
    p.emulator.run = ref("codebreaker.emulate.windows16", "recorded");
    p.emulator.call = ref(here, "call");
    p.compiler.language = "c";
    p.compiler.toolchain = "openwatcom";
    p.compiler.tool = ref("codebreaker.build.watcom", "ow_tool", {"wcc"});
    p.compiler.tools = ref(here, "tools");
    p.compiler.flags = ref("codebreaker.build.functions", "CFLAGS16");
    p.compiler.env = ref("codebreaker.build.watcom", "ow_env", {"ne"});
    p.compiler.compile = ref("codebreaker.build.functions", "_compile");
    p.compiler.library = ref(here, "library");
    p.packager.link = ref("codebreaker.build.module", "_build16");
    p.packager.functions = ref("codebreaker.build.functions", "build16");
    p.packager.tools = ref(here, "tools");
    p.packager.linker = ref("codebreaker.build.watcom", "ow_tool", {"wlink"});
    p.packager.assembler = ref("codebreaker.build.watcom", "ow_tool", {"wasm"});
    p.packager.resources = ref("codebreaker.build.watcom", "ow_tool", {"wrc"});
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::win16
