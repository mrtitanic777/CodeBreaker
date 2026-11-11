// codebreaker/engine/lanes_wiring.cpp -- see the header.

#include "codebreaker/engine/lanes_wiring.hpp"

#include <mutex>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/engine/lanes.hpp"
#include "codebreaker/engine/win16.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/reconstruct.hpp"

namespace codebreaker::engine::lanes_wiring {

namespace {

namespace recon = verify::reconstruct;

// platforms/win16.py's tools(): {t: watcom.ow_tool(t) or "" for t in ("wcc", "wlink", "wasm")}; platforms/win32.py's: wcc386, wlink
py::dict<py::str, py::str> tools_of(std::initializer_list<const char*> names) {
    py::dict<py::str, py::str> out;
    for (const char* n : names) out.set(py::str(n), build::watcom::ow_tool(py::str(n)));
    return out;
}

// platforms.win32.PLATFORM.recognise: lane_of(path) == "pe" and pe_machine(path) != 0x01F2 (PowerPC: the Xbox 360's)
bool recognises_pe(const py::str& path) {
    try {
        const auto lane = base::corpus::lane(path);
        return lane && *lane == py::str("pe") && formats::pe::open_pe(path).machine != 0x01F2;
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

lanes::LanePlatform win16_platform() {
    lanes::LanePlatform p;
    p.name = py::str("ne");
    p.id = py::str("win16");
    p.lane = py::str("codebreaker.verify.lanes.win16");
    p.tools = [] { return tools_of({"wcc", "wlink", "wasm"}); };
    p.lane_module = [] { return verify::lanes::win16::kind(); };
    p.identifies = [](const py::str& path) { return platforms::win16::recognises(path); };
    p.link = [](build::module::Built& b, const py::str& path, const recon::Prepared& m, const py::str& folder) {
        return build::module::_build16(b, path, *std::static_pointer_cast<decompile::c_text::Module16>(m.lane), folder);
    };
    return p;
}

lanes::LanePlatform win32_platform() {
    lanes::LanePlatform p;
    p.name = py::str("pe");
    p.id = py::str("win32");
    p.lane = py::str("codebreaker.verify.lanes.win32");
    p.tools = [] { return tools_of({"wcc386", "wlink"}); };
    p.lane_module = [] { return verify::lanes::win32::kind(); };
    p.identifies = recognises_pe;
    p.link = [](build::module::Built& b, const py::str& path, const recon::Prepared& m, const py::str& folder) {
        return build::module::_build32(b, path, *std::static_pointer_cast<decompile::c_text::Module32>(m.lane), folder);
    };
    return p;
}

}  // namespace

std::optional<build::module::Built> delegate_build(const py::str& module, const py::str& path, const std::optional<py::str>& outdir, bool call,
                                                   const std::function<void(const py::str&)>& log) {
    const auto v = py::os::environ_::get(py::str("CB_ENGINE"));
    if (v && *v == py::str("0")) return std::nullopt;
    register_core_platforms();
    if (platforms::win16::recognises(path)) return *win16::build(module, outdir, call, log);
    if (recognises_pe(path)) return *lanes::build(module, outdir, call, log);
    return std::nullopt;
}

void register_core_platforms() {
    static std::once_flag once;
    std::call_once(once, [] {
        verify::lanes::win16::register_lane();      // reconstruct::KINDS["ne"]
        verify::lanes::win32::register_lane();      // reconstruct::KINDS["pe"]
        // (platforms.all() is in the order of the descriptors' names: win16 before win32)
        const lanes::LanePlatform ne = win16_platform(), pe = win32_platform();
        lanes::register_platform(ne);
        lanes::register_platform(pe);
        lanes::_declare(ne);
        lanes::_declare(pe);
        win16::declare();
    });
}

}  // namespace codebreaker::engine::lanes_wiring
