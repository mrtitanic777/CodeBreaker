// codebreaker/platforms/xbox360.cpp -- typed C++ of codebreaker.platforms.xbox360 (see xbox360.hpp).
#include "codebreaker/platforms/xbox360.hpp"

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms::xbox360 {

// build/functions.powerpc_tools(): (compiler, linker, runtime library), or None when settings.json does not name them.
// Repeated here: build/functions.cpp (the typed port of the build) links only together with the whole decompiler (through
// decompile/c_text), which this platform description does not need.
static std::optional<std::tuple<py::str, py::str, py::str>> powerpc_tools_() {
    std::optional<py::str> d = py::settings::get_opt_str(py::str("powerpc_tools")), lib = py::settings::get_opt_str(py::str("powerpc_lib"));
    py::str dd = d ? *d : py::str(""), ll = lib ? *lib : py::str("");
    py::str cl = py::os::path::join(dd, py::str("cl.exe")), link = py::os::path::join(dd, py::str("link.exe"));
    if (d && !d->empty() && py::os::path::isfile(cl) && py::os::path::isfile(link) && py::os::path::isfile(ll)) return std::make_tuple(cl, link, ll);
    return std::nullopt;
}

bool _recognise(const py::str& path) {
    if (formats::xex::is_xex(path)) return true;
    auto lane = base::corpus::lane(path);
    return lane && *lane == py::str("pe") && formats::pe::open_pe(path).machine == 0x01F2;
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

py::str _open(const py::str& path) { return formats::xex::is_xex(path) ? formats::xex::basefile(path) : path; }

py::str readable(const py::str& path) { return _open(path); }

py::str compiler() {
    auto tools = powerpc_tools_();
    return tools ? std::get<0>(*tools) : py::str("");
}

py::dict<py::str, py::str> tools() {
    auto t = powerpc_tools_();
    py::str cl, link, lib;
    if (t) std::tie(cl, link, lib) = *t;
    py::dict<py::str, py::str> out;
    out.set(py::str("cl"), cl);
    out.set(py::str("link"), link);
    out.set(py::str("libcmt"), lib);
    return out;
}

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

static py::list<py::str> registers(int64_t from) {
    py::list<py::str> out;
    for (int64_t i = from; i < 32; ++i) out.append(py::percent(py::str("r%d"), i));
    return out;
}

static Ref ref(const char* module, const char* name, std::initializer_list<const char*> args = {}) {
    return Ref{py::str(module), py::str(name), strs(args)};
}

static Platform make_platform() {
    const char* here = "codebreaker.platforms.xbox360";
    Platform p;
    p.name = py::str("powerpc");
    p.title = py::str("Xbox 360 (PowerPC)");
    p.family = py::str("PowerPC");
    p.source = py::str("c");
    p.machine = 0x01F2;
    p.lane = py::str("codebreaker.verify.lanes.powerpc");
    p.steps = py::str("codebreaker.engine.lanes");                    // (its steps on the engine: the lanes' steps every lane of a binary has)
    p.loader.container = py::str("XEX2 -> PE");
    p.loader.readable = ref(here, "_open");
    p.loader.open = ref("codebreaker.formats.powerpc_image", "read");
    p.cpu.language = py::str("PowerPC:BE:64:Xenon:homed");
    p.cpu.bits = 64;
    p.cpu.endian = py::str("big");
    p.cpu.decoder = py::str("codebreaker.emulate.powerpc");
    p.cpu.registers = registers(0);
    p.cpu.stack = py::str("r1");
    p.convention.names = strs({"__cdecl"});
    p.convention.arguments = int64_t(8);
    p.convention.word = 8;
    p.convention.order = py::str("registers");
    p.convention.results = strs({"r3", "f1"});
    p.convention.pops = py::str("caller");
    p.convention.preserved = registers(14);
    p.emulator.engine = py::str("CodeBreaker's own interpreter");
    p.emulator.run = ref("codebreaker.emulate.powerpc", "recorded");
    p.compiler.language = py::str("c");
    p.compiler.toolchain = py::str("the Xbox 360 SDK");
    p.compiler.tool = ref(here, "compiler");
    p.compiler.tools = ref(here, "tools");
    p.compiler.flags = ref("codebreaker.build.functions", "CFLAGS_POWERPC");
    p.packager.functions = ref("codebreaker.build.functions", "build_powerpc");
    p.packager.tools = ref(here, "tools");
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::xbox360
