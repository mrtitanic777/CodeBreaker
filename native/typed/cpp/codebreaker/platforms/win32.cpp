// codebreaker/platforms/win32.cpp -- typed C++ of codebreaker.platforms.win32 (see win32.hpp).
#include "codebreaker/platforms/win32.hpp"

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms::win32 {

// build/module._sections(path): [(address, bytes)] for everything of a 32-bit image that is not code (the emulator maps a
// module's code itself, and reads anything else as zeros unless it is given).  Repeated here: build/module.cpp (the typed
// port of the build) cannot be compiled without the z3 headers of the symbolic emulator it includes.
static py::list<std::tuple<int64_t, py::bytes>> _sections(const py::str& path) {
    formats::pe::PEFile pe = formats::pe::open_pe(path);
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const formats::pe::Section& s : pe.sections)
        if (s.rawsize && !(s.chars & 0x20000020))
            out.append(std::make_tuple(int64_t(pe.image_base) + s.vaddr, pe.data.slice(s.rawptr, s.rawptr + s.rawsize)));
    return out;
}

py::list<std::tuple<int64_t, py::bytes>> sections(const py::str& path) {
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const auto& s : formats::pe::open_pe(path).code_sections()) out.append(std::make_tuple(int64_t(std::get<0>(s)), std::get<1>(s)));
    return out;
}

std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t address, const Runner& run) {
    py::list<int64_t> args;
    for (int64_t i = 0; i < std::get<int64_t>(*PLATFORM().convention.arguments); i++) args.append(0);   // [0] * arguments
    try {
        RunRecord r = run(path, address, args, _sections(path));
        return {r.ended, r.results.get(py::str("EAX"))};
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {                 // the emulator giving up is a result too
        return {py::str("error: ") + py::str(e.type()), std::nullopt};
    }
}

py::dict<py::str, py::str> tools() {
    py::dict<py::str, py::str> out;
    for (const char* t : {"wcc386", "wlink"}) {
        py::str p = build::watcom::ow_tool(py::str(t));
        out.set(py::str(t), p);   // (`ow_tool(t) or ""`: an unfound tool is "" already)
    }
    return out;
}

bool _recognise(const py::str& path) {
    auto lane = base::corpus::lane(path);
    return lane && *lane == py::str("pe") && formats::pe::open_pe(path).machine != 0x01F2;
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
    const char* here = "codebreaker.platforms.win32";
    Platform p;
    p.name = py::str("pe");
    p.title = py::str("32-bit Windows");
    p.family = py::str("x86");
    p.source = py::str("c");
    p.machine = 0x014C;
    p.lane = py::str("codebreaker.verify.lanes.win32");
    p.steps = py::str("codebreaker.engine.lanes");                    // (its steps on the engine: the lanes' steps every lane of a binary has)
    p.loader.container = py::str("PE");
    p.loader.open = ref("codebreaker.formats.pe", "open_pe");
    p.loader.parts = ref(here, "sections");
    p.loader.exports = ref("codebreaker.build.definitions", "exports");
    p.loader.imports = ref("codebreaker.build.definitions", "imports");
    p.cpu.language = py::str("x86:LE:32:default:windows");
    p.cpu.bits = 32;
    p.cpu.endian = py::str("little");
    p.cpu.decoder = std::make_tuple(py::str("capstone"), py::str("x86"), int64_t(32));
    p.cpu.registers = strs({"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"});
    p.cpu.stack = py::str("ESP");
    p.convention.names = strs({"__stdcall", "__cdecl", "__fastcall", "__thiscall"});
    p.convention.arguments = int64_t(4);
    p.convention.word = 4;
    p.convention.order = py::str("c");
    p.convention.results = strs({"EAX", "EDX:EAX"});
    p.convention.pops = py::str("either");
    p.convention.preserved = strs({"EBX", "ESI", "EDI", "EBP"});
    p.emulator.engine = py::str("unicorn");
    p.emulator.run = ref("codebreaker.emulate.emulator", "recorded");
    p.emulator.call = ref(here, "call");
    p.compiler.language = py::str("c");
    p.compiler.toolchain = py::str("openwatcom");
    p.compiler.tool = ref("codebreaker.build.watcom", "ow_tool", {"wcc386"});
    p.compiler.tools = ref(here, "tools");
    p.compiler.flags = ref("codebreaker.decompile.c_text", "CFLAGS");
    p.compiler.env = ref("codebreaker.build.watcom", "ow_env", {"pe"});
    p.packager.link = ref("codebreaker.build.module", "_build32");
    p.packager.functions = ref("codebreaker.build.functions", "build32");
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

}  // namespace codebreaker::platforms::win32
