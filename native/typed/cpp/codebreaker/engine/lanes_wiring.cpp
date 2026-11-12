// codebreaker/engine/lanes_wiring.cpp -- see the header.

#include "codebreaker/engine/lanes_wiring.hpp"

#include <mutex>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/engine/lanes.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
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


// reconstruct::run, its decompile, build and judge made by the engine (engine/lanes): only what changed is made again, and the store is given the
// rows that differ from what it holds. (It was part of verify/reconstruct before that module was refactored; the shim of reconstruct asks for it
// through reconstruct::hooks().) nullopt: the engine is not on for this module.
using NE = engine::native_engine::Cell;

py::str engine_cell_text(const NE& c) {
    if (std::holds_alternative<py::str>(c)) return std::get<py::str>(c);
    if (std::holds_alternative<int64_t>(c)) return std::get<int64_t>(c) ? py::to_str(std::get<int64_t>(c)) : py::str("");
    if (std::holds_alternative<double>(c)) return std::get<double>(c) != 0.0 ? py::to_str(std::get<double>(c)) : py::str("");
    return py::str("");
}
NE engine_cell_of(const py::str& s) { return NE(s); }
NE engine_cell_of(const std::optional<py::str>& s) { return s ? NE(*s) : NE(); }

std::optional<std::tuple<py::str, py::str>> run_on_engine(const py::str& module_path, const std::shared_ptr<recon::Kind>& kind,
                                                          const std::shared_ptr<engine::native_engine::FunctionStore>& fs,
                                                          const std::function<void(const py::str&)>& log) {
    if (!lanes::on(module_path)) return std::nullopt;
    const py::str sha = recon::module_sha(*kind, module_path);
    const py::str name = py::os::path::basename(py::os::path::normpath(module_path)).lower();
    const double t0 = py::time::time();
    lanes::Judged j = lanes::judged(module_path);
    fs->add_module(sha, name, module_path, kind->KIND());
    fs->add_profile(j.profile, j.settings);
    fs->add_functions(sha, j.all_keys);
    fs->put_text(sha, recon::SOURCE, py::str("header"), j.header);
    if (kind->LIBRARY()) {
        py::dict<py::str, std::tuple<py::str, py::str>> found;
        for (const auto& kv : symbols::runtime_library::identify(module_path).items())
            found.set(symbols::names::key(module_path, std::get<0>(kv)), std::get<1>(kv));
        fs->set_identified(sha, found);
    }
    py::dict<py::str, engine::native_engine::Candidate> known = fs->judged(sha, j.profile, recon::SOURCE);
    py::list<engine::native_engine::FunctionStore::RecordRow> rows;
    int64_t fresh_rows = 0;
    for (const lanes::JudgedRow& r : j.rows) {
        bool differs = !known.contains(r.addr);
        if (!differs) {
            const engine::native_engine::Candidate& k = known.at(r.addr);
            differs = !(engine_cell_text(k.sig) == r.sig && engine_cell_text(k.name) == r.name &&
                        engine_cell_text(k.source) == (r.source ? *r.source : py::str("")) && engine_cell_text(k.verdict) == r.verdict &&
                        engine_cell_text(k.reason) == r.reason && engine_cell_text(k.limits) == r.limits && engine_cell_text(k.detail) == r.detail);
        }
        if (!differs) continue;
        engine::native_engine::FunctionStore::RecordRow out;
        out.addr = r.addr;
        out.name = engine_cell_of(r.name);
        out.source = engine_cell_of(r.source);
        out.sig = NE(r.sig);
        out.verdict = r.verdict;
        out.reason = engine_cell_of(r.reason);
        out.limits = engine_cell_of(r.limits);
        out.detail = engine_cell_of(r.detail);
        out.problem = engine_cell_of(r.problem);
        rows.append(out);
        fresh_rows += 1;
    }
    NE rev = NE(kind->SOURCE_REV() ? *kind->SOURCE_REV() : py::to_str(decompile::decompiler::REV));
    fs->record_many(sha, recon::SOURCE, rev, j.profile, rows);
    if (!j.built.empty()) fs->set_built(sha, j.profile, j.built);
    py::list<int64_t> kept_made = j.made.get(py::percent("lane.%s.verdict", kind->KIND()), py::list<int64_t>{0, 0});
    py::dict<py::str, NE> c = fs->scores(j.profile, sha);
    py::list<py::str> parts;
    for (const py::str& k : store::functions::SCORED()) {
        const NE& n = c.at(k);
        const int64_t count = std::holds_alternative<int64_t>(n) ? std::get<int64_t>(n) : 0;
        if (count) parts.append(py::percent("%s %d", store::functions::TITLE().at(k), count));
    }
    log(py::percent("%s: %d functions on the engine, %d judged now, %d kept, %d rows new to the store: %s  (%.0fs)", name, j.all_keys.size(),
                    kept_made[1], kept_made[0], fresh_rows, py::join(py::str(", "), parts), py::time::time() - t0));
    engine::facts::hold();
    return std::tuple<py::str, py::str>{sha, j.profile};
}

// reconstruct::kind_name: the first platform that recognises the file, when it has a lane
std::optional<py::str> kind_name_of(const py::str& module_path) {
    for (const lanes::LanePlatform& p : lanes::platforms())
        if (p.identifies && p.identifies(module_path)) return p.name;
    return std::nullopt;
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
        recon::hooks().kind_name = kind_name_of;
        recon::hooks().engine_run = run_on_engine;
    });
}

}  // namespace codebreaker::engine::lanes_wiring
