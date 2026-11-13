// codebreaker/source/driver_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/driver_writer.{hpp,cpp}
// (the writer, the rounds and the command); this header keeps the old py:: surface (reconstruct, main, MAIN_DOC, the result record) for the
// dynamic exe and the ports that still call it. Conversion and forwarding only.
//
// Not carried over: the internals the module's own port exposed (Writer, _Object, _differences, _MARK ...); what Python reads of a result is
// the record `Reconstruction` below, as before. `set_capstone_dll` stays only so that a caller written for the old module compiles: the new
// module loads the Capstone that ships beside the executable itself.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "cb/build/watcom.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/driver_writer.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::driver_writer {

// (the new module loads capstone.dll itself)
inline void set_capstone_dll(const py::str&) {}

// reconstruct()'s dict
struct Reconstruction {
    py::str source;
    std::optional<py::str> image;
    bool linked = false;
    int64_t rounds = 0;
    py::list<py::str> notes;
    std::optional<py::str> definitions;
    int64_t instructions = 0, raw = 0, data = 0;
    std::optional<py::dict<py::str, bool>> checks;
    std::optional<double> same;
    std::optional<py::list<int64_t>> differing_objects;
    std::optional<py::str> l2;
};

inline const py::str MAIN_DOC(cb::compat::to_py(cb::source::driver_writer::usage()));

namespace shim_ {

inline py::str path_text(const std::filesystem::path& p) { return cb::compat::to_py(cb::fs::to_text(p)); }

}  // namespace shim_

// The new module's options as the old settings name them: OpenWatcom's tree, the file what has been probed is kept in, and the driver kit's
// names of the services (the old table). `log` receives each round's account.
inline cb::source::driver_writer::Options native_options(const std::function<void(const py::str&)>& log = nullptr) {
    const py::str root = py::settings::get_str(py::str("openwatcom"));
    cb::source::driver_writer::Options o = cb::source::driver_writer::options_for(
        cb::build::watcom::OpenWatcom::from_settings(cb::compat::from_py(root)),
        cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state")))) / "assembly_probes.json",
        [](std::uint32_t device, std::uint32_t service) {
            return cb::compat::from_py(symbols::driver_services::label(static_cast<int64_t>(device), static_cast<int64_t>(service)));
        });
    if (log) o.log = [log](const std::string& line) { log(cb::compat::to_py(line)); };
    return o;
}

// Write a driver's source, build it, and make it come back as the original (see the top).
inline Reconstruction reconstruct(const py::str& module_path, const py::str& outdir, int64_t member = 0, const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        const cb::source::driver_writer::Result r = cb::source::driver_writer::reconstruct(
            cb::fs::from_text(cb::compat::from_py(module_path)), cb::fs::from_text(cb::compat::from_py(outdir)), native_options(log), static_cast<std::size_t>(member));
        Reconstruction out;
        out.source = shim_::path_text(r.source);
        if (r.image) out.image = shim_::path_text(*r.image);
        out.linked = r.linked;
        out.rounds = static_cast<int64_t>(r.rounds);
        for (const std::string& n : r.notes) out.notes.append(cb::compat::to_py(n));
        if (r.definitions) out.definitions = shim_::path_text(*r.definitions);
        out.instructions = static_cast<int64_t>(r.instructions);
        out.raw = static_cast<int64_t>(r.raw);
        out.data = static_cast<int64_t>(r.data);
        if (r.built) {
            py::dict<py::str, bool> checks;
            checks.set(py::str("objects"), r.built->checks.objects);
            checks.set(py::str("descriptor"), r.built->checks.descriptor);
            checks.set(py::str("services"), r.built->checks.services);
            checks.set(py::str("exports"), r.built->checks.exports);
            checks.set(py::str("module"), r.built->checks.module);
            out.checks = checks;
            out.same = r.built->same;
            py::list<int64_t> differing;
            for (const std::uint32_t i : r.differing_objects) differing.append(static_cast<int64_t>(i));
            out.differing_objects = differing;
        }
        if (r.l2) out.l2 = shim_::path_text(*r.l2);
        return out;
    });
}

// python -m codebreaker driver-source <driver> [--out DIR]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        cb::source::driver_writer::Environment env;
        env.options = native_options();
        env.state = cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state"))));
        env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
            const std::optional<py::str> found = base::corpus::retail(cb::compat::to_py(name));
            if (!found || found->empty()) return std::nullopt;
            return cb::fs::from_text(cb::compat::from_py(*found));
        };
        return static_cast<int64_t>(
            cb::source::driver_writer::command(args, env, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::source::driver_writer
