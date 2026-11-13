// codebreaker/source/dos_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/dos_writer.{hpp,cpp} (the
// program and its file) over dos_image and dos_program; this header keeps the old py:: surface (reconstruct, rebuild, main, MAIN_DOC and the
// result records) for the dynamic exe. Conversion and forwarding only, except
//   COMPAT(py): the Windows module of a DOS+Windows file is built by the old port of build/module. It is handed to the new module as a
//       function (Options::build_windows); remove it when build/module is refactored (the new module then takes it itself).
//   The VxD of a driver+vxd file is the new driver writer's (source/driver_writer, Options::write_vxd).
//
// Not carried over: the internals the module's own port exposed (Writer, Ins, _program, _kernel, _as_file, _facts, _compose, ...). What
// Python reads of a result is the record `Result` below, as before.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "cb/build/watcom.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/dos_writer.hpp"
#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/definitions.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/source/driver_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::dos_writer {

namespace dos_functions = codebreaker::analyze::dos_functions;

// What laying the file out takes from the original (kept in <stem>.build.json).
struct Facts {
    py::str file;
    std::optional<py::str> packer;
    bool overlay = false;
    std::optional<int64_t> min_alloc;
    std::optional<bool> full_last;
    std::optional<int64_t> words;
    // EXEPACK: the relocation offsets as it stored them; any other: (offset, segment) pairs
    std::variant<py::list<int64_t>, py::list<std::tuple<int64_t, int64_t>>> order = py::list<std::tuple<int64_t, int64_t>>();
    bool pklite_order = false;
};

// What the Windows module of a DOS+Windows file came to.
struct Windows {
    bool linked = false;
    int64_t functions = 0;
    py::list<py::str> stand_ins;
    py::list<py::str> undefined;
    py::list<build::definitions::Import> imports_extra;
    int64_t ran = 0, ran_same = 0;
};

// What one part of a combined file came to.
struct Part {
    py::str source;
    std::optional<py::str> image;
    bool linked = false;
    bool same = false;
    int64_t rounds = 0;
    int64_t instructions = 0, raw = 0, data = 0;
    py::list<py::str> notes;
};

// The dict reconstruct answers.
struct Result {
    std::variant<std::monostate, py::str, py::list<py::str>> source;
    std::optional<py::str> image;
    bool linked = false;
    bool same = false;
    int64_t rounds = 0;
    py::list<py::str> notes;
    py::str kind;
    int64_t instructions = 0, raw = 0, data = 0;
    py::dict<int64_t, py::ref<dos_functions::Function>> functions;
    std::vector<std::pair<py::str, Part>> parts;
    py::list<py::str> remarks;
    std::optional<Facts> facts;
    std::optional<bool> equivalent;
    std::optional<Windows> windows;
    std::optional<py::str> file;
    std::optional<bool> file_same;
};

using Log = std::function<void(const py::str&)>;

inline const py::str MAIN_DOC(cb::compat::to_py(cb::source::dos_writer::usage()));

namespace shim_ {

namespace nw = cb::source::dos_writer;

inline py::str path_text(const std::filesystem::path& p) { return cb::compat::to_py(cb::fs::to_text(p)); }

// The tools, as the old settings name them: OpenWatcom's tree, and the file what has been probed is kept in.
inline nw::Options options(const Log& log, py::list<build::definitions::Import>* extra_imports) {
    const py::str root = py::settings::get_str(py::str("openwatcom"));
    nw::Options o = nw::options_for(cb::build::watcom::OpenWatcom::from_settings(cb::compat::from_py(root)),
                                    cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state")))) / "assembly_probes.json");
    o.log = [log](const std::string& line) {
        if (log) log(cb::compat::to_py(line));
    };
    // the VxD of a driver+vxd file: the new driver writer's hook (source/driver_writer)
    o.write_vxd = cb::source::driver_writer::vxd_writer(driver_writer::native_options(log));
    // COMPAT(py): the Windows module of a DOS+Windows file, by the old module builder
    o.build_windows = [extra_imports](const std::filesystem::path& program, const std::filesystem::path& outdir) {
        const build::module::Built b = build::module::build(path_text(program), path_text(outdir), true, [](const py::str&) {});
        py::set<py::str> stand = py::set<py::str>::from(b.rejected.keys());
        stand.update(b.absent);
        nw::WindowsBuild w;
        w.linked = b.linked();
        w.functions = static_cast<std::size_t>(b.functions);
        for (const py::str& s : py::sorted(stand)) w.stand_ins.push_back(cb::compat::from_py(s));
        for (const py::str& s : b.undefined) w.undefined.push_back(cb::compat::from_py(s));
        w.ran = static_cast<std::size_t>(py::len(b.ran));
        for (const auto& r : b.ran) {
            if (std::get<3>(r)) ++w.ran_same;
        }
        if (py::truthy(b.error)) w.error = cb::compat::from_py(b.error);
        if (extra_imports != nullptr) *extra_imports = b.imports_extra;
        return w;
    };
    return o;
}

inline py::list<py::str> texts(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const std::string& s : v) out.append(cb::compat::to_py(s));
    return out;
}

inline Facts facts(const nw::Facts& f) {
    Facts out;
    out.file = cb::compat::to_py(f.file);
    if (f.packer) out.packer = cb::compat::to_py(*f.packer);
    out.overlay = f.overlay;
    if (f.min_alloc) out.min_alloc = *f.min_alloc;
    out.full_last = f.full_last;
    if (f.words) out.words = *f.words;
    if (f.packer && *f.packer == "EXEPACK") {
        py::list<int64_t> order;
        for (const std::uint32_t o : f.offsets) order.append(o);
        out.order = order;
    } else {
        py::list<std::tuple<int64_t, int64_t>> order;
        for (const auto& [a, b] : f.pairs) order.append(std::make_tuple<int64_t, int64_t>(a, b));
        out.order = order;
    }
    out.pklite_order = f.packer && *f.packer == "PKLITE";
    return out;
}

inline Result result(const nw::Result& r, const py::list<build::definitions::Import>& extra_imports) {
    Result out;
    if (r.parts.empty()) {
        out.source = r.sources.empty() ? py::str("") : path_text(r.sources.front());
    } else {
        py::list<py::str> sources;
        for (const std::filesystem::path& s : r.sources) sources.append(path_text(s));
        out.source = sources;
    }
    if (r.image) out.image = path_text(*r.image);
    out.linked = r.linked;
    out.same = r.same;
    out.rounds = static_cast<int64_t>(r.rounds);
    out.notes = texts(r.notes);
    out.remarks = texts(r.remarks);
    out.kind = cb::compat::to_py(r.kind);
    out.instructions = static_cast<int64_t>(r.instructions);
    out.raw = static_cast<int64_t>(r.raw);
    out.data = static_cast<int64_t>(r.data);
    namespace ns = ::cb::analyze::dos_functions;
    for (const auto& [at, f] : r.functions) {
        auto old = std::make_shared<dos_functions::Function>(static_cast<int64_t>(f.address), cb::compat::to_py(f.name), cb::compat::to_py(ns::name_of(f.distance)),
                                                             cb::compat::to_py(ns::name_of(f.evidence)), static_cast<int64_t>(f.cs), f.sure);
        old->removes = f.removes;
        old->interrupt = f.interrupt;
        old->size = f.size;
        out.functions.set(static_cast<int64_t>(at), old);
    }
    for (const nw::Part& p : r.parts) {
        Part part;
        part.source = path_text(p.source);
        if (p.image) part.image = path_text(*p.image);
        part.linked = p.linked;
        part.same = p.same;
        part.rounds = static_cast<int64_t>(p.rounds);
        part.instructions = static_cast<int64_t>(p.instructions);
        part.raw = static_cast<int64_t>(p.raw);
        part.data = static_cast<int64_t>(p.data);
        part.notes = texts(p.notes);
        out.parts.emplace_back(cb::compat::to_py(p.name), part);
    }
    if (r.facts) out.facts = facts(*r.facts);
    if (r.equivalent || ((r.kind == "kernel" || r.kind == "driver+vxd") && !r.same)) out.equivalent = r.equivalent;
    if (r.windows) {
        Windows w;
        w.linked = r.windows->linked;
        w.functions = static_cast<int64_t>(r.windows->functions);
        w.stand_ins = texts(r.windows->stand_ins);
        w.undefined = texts(r.windows->undefined);
        w.imports_extra = extra_imports;
        w.ran = static_cast<int64_t>(r.windows->ran);
        w.ran_same = static_cast<int64_t>(r.windows->ran_same);
        out.windows = w;
    }
    if (r.file) out.file = path_text(*r.file);
    out.file_same = r.file_same;
    return out;
}

}  // namespace shim_

inline Result reconstruct(const py::str& module_path, const py::str& outdir, const Log& log = nullptr) {
    return cb::compat::guard([&] {
        py::list<build::definitions::Import> extra_imports;
        const cb::source::dos_writer::Options o = shim_::options(log, &extra_imports);
        return shim_::result(cb::source::dos_writer::reconstruct(cb::fs::from_text(cb::compat::from_py(module_path)),
                                                                 cb::fs::from_text(cb::compat::from_py(outdir)), o),
                             extra_imports);
    });
}

// Make the program from its folder alone (what dos-source wrote). -> the path of the file made.
inline py::str rebuild(const py::str& srcdir, const Log& log = nullptr, const std::optional<py::str>& desc = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::source::dos_writer::Options o = shim_::options(log, nullptr);
        std::optional<std::string> description;
        if (desc) description = cb::compat::from_py(*desc);
        return shim_::path_text(cb::source::dos_writer::rebuild(cb::fs::from_text(cb::compat::from_py(srcdir)), o, description));
    });
}

// python -m codebreaker dos-source <program> [--out DIR]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        cb::source::dos_writer::Environment env;
        env.options = shim_::options(nullptr, nullptr);
        env.state = cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state"))));
        env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
            const std::optional<py::str> found = base::corpus::retail(cb::compat::to_py(name));
            if (!found || found->empty()) return std::nullopt;
            return cb::fs::from_text(cb::compat::from_py(*found));
        };
        return static_cast<int64_t>(cb::source::dos_writer::command(args, env, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::source::dos_writer
