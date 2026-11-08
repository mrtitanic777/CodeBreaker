// codebreaker/java/overlays.cpp -- typed C++ of codebreaker.java.overlays.
//
// Per-project code overlays -- re-apply hand-fixes on top of freshly generated output.
//
// WHY THIS EXISTS
//     Most CodeBreaker fixes belong in the emitter (a general transform), the runtime headers, or the platform
//     reference layer (docs/platform/reference/*) -- those are GENERAL and benefit every conversion.
//     But a real port also accumulates fixes that are GAME/VERSION-SPECIFIC edits to GENERATED files:
//     a leak `delete` in one method, a value-comparing std::set swapped in for a broken JSet, a UI tweak,
//     an input remap. These don't generalize into an emitter rule, so a reconversion would silently WIPE
//     them. That is the failure this module prevents.
//
//     A file placed under overlays/<project_key>/<relpath> is copied over out_dir/<relpath> AFTER emission
//     (and after reference emission), so it is the final say. It may REPLACE a generated file or ADD a new
//     one (e.g. a hand-written Socket.cpp); because the vcxproj step runs AFTER this and enumerates out_dir,
//     added .cpp files get compiled automatically.
//
//     Precedence (last wins): runtime headers -> emitted classes -> platform references -> OVERLAYS.
//
//     This is the catch-all, not an excuse: when an overlaid fix turns out to be a general pattern, migrate
//     it into the emitter/runtime/reference and delete the overlay. MANIFEST.md in each project dir records
//     what each overlay is and whether it's slated for migration.

#include "codebreaker/java/overlays.hpp"

namespace codebreaker::java::overlays {

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
// (codebreaker.base.settings has no typed port yet; this is its `state` for the two parts used here,
// over py::settings::get_str, which is its `get` for a str setting.)
py::str state_(const py::str& a, const py::str& b) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

}  // namespace

py::str project_key(const std::optional<py::str>& out_dir, const std::optional<py::str>& java_dir) {
    (void)java_dir;
    py::str base = py::os::path::basename(py::os::path::normpath((out_dir && !out_dir->empty()) ? *out_dir : py::str("")));
    return base;
}

py::str overlays_root() {
    py::str configured = py::settings::get_str(py::str("java_overlays"));
    if (!configured.empty()) {
        return configured;
    }
    return state_(py::str("java"), py::str("overlays"));
}

std::optional<py::str> resolve_overlay_dir(const std::optional<py::str>& out_dir, const std::optional<py::str>& java_dir,
                                           const std::optional<py::str>& explicit_) {
    if (explicit_ && !explicit_->empty()) {
        return explicit_;
    }
    py::str key = project_key(out_dir, java_dir);
    if (key.empty()) {
        return std::nullopt;
    }
    return py::os::path::join(overlays_root(), key);
}

py::list<py::str> apply_overlays(const py::str& out_dir, const std::optional<py::str>& overlay_dir,
                                 const std::function<void(const py::str&)>& prog_in) {
    std::function<void(const py::str&)> prog = prog_in ? prog_in : [](const py::str&) {};
    if (!overlay_dir || overlay_dir->empty() || !py::os::path::isdir(*overlay_dir)) {
        return py::list<py::str>();
    }
    py::list<py::str> applied;
    int64_t added = 0;
    for (const auto& entry : py::os::walk(*overlay_dir)) {
        const py::str& root = std::get<0>(entry);
        const py::list<py::str>& files = std::get<2>(entry);
        for (const py::str& fn : files) {
            if (fn == py::str("MANIFEST.md")) {
                continue;
            }
            py::str src = py::os::path::join(root, fn);
            py::str rel = py::os::path::relpath(src, *overlay_dir);
            py::str dst = py::os::path::join(out_dir, rel);
            py::str dst_parent = py::os::path::dirname(dst);
            if (!dst_parent.empty()) {
                py::os::makedirs(dst_parent, 0777, true);
            }
            bool existed = py::os::path::exists(dst);
            py::shutil::copyfile(src, dst);
            applied.append(rel);
            if (!existed) {
                added += 1;
            }
            prog(py::percent(py::str("[overlay] %s %s"), existed ? py::str("replaced") : py::str("ADDED   "), rel));
        }
    }
    if (!applied.empty()) {
        prog(py::percent(py::str("[overlay] applied %d project overlay file(s) (%d new) from %s"), py::len(applied), added,
                         *overlay_dir));
    }
    return applied;
}

}  // namespace codebreaker::java::overlays
