// codebreaker/xna/port.hpp -- COMPAT SHIM over cb/xna/port.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/xna/port.{hpp,cpp}: the rewrites of the game's C# for the PC, the fixes of the compiler's errors, the content
// conversions, prepare and build of the MonoGame project, run_for and check_content. This header keeps the entry points the old code calls
// over the old py:: types, header-only: the tools (the runtime's and the stub generator's folders in the checkout, the state folder, the
// settings the Python read) are put together here from the settings, and the formats not yet in the new tree (xsb, xnb) are the old
// `Hooks` handed on.
//
// DIVERGES: not kept, internals of the module that only its own harness called: `_args`, `_edit_lines`, `_effect_bytes`, `_csproj`,
// `_overlay_dir`, `_copy_tree`, `_xna_assembly`, `_stub_tool`, `stubs`, `prepare`, `_build_once`, `compile_effects`, `convert_songs` and
// `convert_xact` as separate functions (they run inside build). The XACT wave banks' decoder (formats.xbox.xma, no typed port) is not in
// the old Hooks, so a game with wave banks is built from the Python until formats/xbox/xma is refactored (the shim's `build` throws
// UnsupportedError for it); an error's `line` and `col` are still text; a failed build lists its errors by code in sorted order.
#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <tuple>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/xna/port.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/xna/run.hpp"
#include "py.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::xna::port {

namespace nw = cb::xna::port;

using Json = knowledge::sources::Json;

inline const py::str MONOGAME = py::str(std::string(nw::kMonoGameVersion));

// What port.py takes from modules without a typed port.
struct Hooks {
    // xsb.read(path): the bank as a JSON object
    std::function<Json(const py::str& path)> xsb_read;
    // xnb.rename_readers(raw, rename): the .xnb with its type readers renamed, or None when nothing changed
    std::function<std::optional<py::bytes>(const py::bytes& raw, const std::function<py::str(const py::str&)>& rename)> xnb_rename_readers;
};

// A compiler error line: the regex's groups.
struct BuildError {
    py::str file, line, col, code, text;
};

namespace detail_shim {

inline std::filesystem::path path_of(const py::str& s) { return cb::compat::legacy::path_of(s); }

inline nw::Tools tools() {
    nw::Tools t;
    const std::filesystem::path here = path_of(py::settings::checkout_root()) / "codebreaker" / "xna";
    t.runtime_dir = here / "runtime";
    t.apistubs_dir = here / "apistubs";
    t.state = cb::compat::legacy::state_dir();
    const py::str overlays = py::settings::get_str(py::str("xna_overlays"));
    if (!overlays.empty()) t.overlays = path_of(overlays);
    return t;
}

// json.dump(value, f, indent=1) of the old Json value
inline void json_text(const Json& j, std::string& out) {
    switch (j.kind) {
        case Json::None: out += "null"; return;
        case Json::Bool: out += j.b ? "true" : "false"; return;
        case Json::Int: out += std::to_string(j.i); return;
        case Json::Float: out += std::to_string(j.d); return;
        case Json::Str: py::json::put(out, j.s); return;
        case Json::List: {
            out += "[";
            for (size_t i = 0; i < j.list.size(); ++i) {
                if (i) out += ", ";
                json_text(j.list[i], out);
            }
            out += "]";
            return;
        }
        case Json::Dict: {
            out += "{";
            bool first = true;
            for (const auto& kv : j.dict.items()) {
                if (!first) out += ", ";
                first = false;
                py::json::put(out, std::get<0>(kv));
                out += ": ";
                json_text(std::get<1>(kv), out);
            }
            out += "}";
            return;
        }
    }
}

inline nw::Hooks hooks(const Hooks& old) {
    nw::Hooks h;
    if (old.xsb_read) {
        h.sound_bank_cues = [old](const std::filesystem::path& bank) {
            std::string out;
            json_text(old.xsb_read(cb::compat::to_py(cb::fs::to_text(bank))), out);
            return out;
        };
    }
    if (old.xnb_rename_readers) {
        h.rename_readers = [old](cb::ByteView xnb, const std::function<std::string(const std::string&)>& rename) -> std::optional<cb::Bytes> {
            auto r = old.xnb_rename_readers(cb::compat::to_py(xnb), [&](const py::str& name) { return cb::compat::to_py(rename(name.utf8())); });
            if (!r) return std::nullopt;
            return cb::compat::from_py(*r);
        };
    }
    return h;
}

inline std::vector<nw::BuildError> new_errors(const py::list<BuildError>& errors) {
    std::vector<nw::BuildError> out;
    for (const BuildError& e : errors)
        out.push_back({e.file.utf8(), static_cast<int>(py::int_parse(e.line)), static_cast<int>(py::int_parse(e.col)), e.code.utf8(), e.text.utf8()});
    return out;
}

}  // namespace detail_shim

// Names of the types the game's C# under `folder` declares (classes, structs, enums, interfaces, delegates).
inline py::set<py::str> declared_types(const py::str& folder) {
    py::set<py::str> out;
    for (const std::string& n : nw::declared_types(detail_shim::path_of(folder))) out.add(cb::compat::to_py(n));
    return out;
}

// The port's changes to one file of the game's C#: (text, [what changed]). `own`: type names the game declares itself.
inline std::tuple<py::str, py::list<py::str>> rewrite(const py::str& text, const py::set<py::str>& own = py::set<py::str>()) {
    std::set<std::string> names;
    for (const py::str& n : own) names.insert(n.utf8());
    const nw::Rewritten r = nw::rewrite(text.utf8(), names);
    py::list<py::str> changes;
    for (const std::string& c : r.changes) changes.append(cb::compat::to_py(c));
    return {cb::compat::to_py(r.text), changes};
}

inline int64_t fix_access(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_access(detail_shim::new_errors(errors))); }
inline int64_t fix_color(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_color(detail_shim::new_errors(errors))); }
inline int64_t fix_vertex_binding(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_vertex_binding(detail_shim::new_errors(errors))); }
inline int64_t fix_named_arguments(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_named_arguments(detail_shim::new_errors(errors))); }
inline int64_t fix_moved_members(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_moved_members(detail_shim::new_errors(errors))); }
inline int64_t fix_ref_out(const py::list<BuildError>& errors) { return static_cast<int64_t>(nw::fix_ref_out(detail_shim::new_errors(errors))); }

// {method name: [(full class name, first parameter's type)]}: the game's own extension methods.
inline py::dict<py::str, py::list<std::tuple<py::str, py::str>>> extension_methods(const py::str& folder) {
    py::dict<py::str, py::list<std::tuple<py::str, py::str>>> out;
    for (const auto& [name, where] : nw::extension_methods(detail_shim::path_of(folder))) {
        py::list<std::tuple<py::str, py::str>> list;
        for (const nw::Extension& e : where) list.append(std::make_tuple(cb::compat::to_py(e.cls), cb::compat::to_py(e.receiver_type)));
        out.set(cb::compat::to_py(name), list);
    }
    return out;
}

inline int64_t fix_shadowed_extensions(const py::list<BuildError>& errors, const py::dict<py::str, py::list<std::tuple<py::str, py::str>>>& extensions) {
    std::map<std::string, std::vector<nw::Extension>> ext;
    for (const auto& kv : extensions.items()) {
        auto& list = ext[std::get<0>(kv).utf8()];
        for (const auto& e : std::get<1>(kv)) list.push_back({std::get<0>(e).utf8(), std::get<1>(e).utf8()});
    }
    return static_cast<int64_t>(nw::fix_shadowed_extensions(detail_shim::new_errors(errors), ext));
}

// A type reader's name in an .xnb as the port reads it.
inline py::str port_reader_name(const py::str& name, const py::str& program) { return cb::compat::to_py(nw::port_reader_name(name.utf8(), program.utf8())); }

// Each .xnb under game_dir rewritten to port_reader_name's names (once: a stamp records it). -> files rewritten
inline int64_t port_reader_names(const py::str& game_dir, const py::str& program, const Hooks& hooks) {
    return static_cast<int64_t>(nw::port_reader_names(detail_shim::path_of(game_dir), program.utf8(), detail_shim::hooks(hooks)));
}

struct BuildResult {
    bool ok = false;
    std::optional<py::str> exe;
    py::dict<py::str, int64_t> errors;     // error code -> count
    py::list<py::str> first;
    py::str build_output_tail;
    int64_t rounds = 0;
};

inline BuildResult build(const base::converted::Project& project, const std::optional<py::str>& overlay, int64_t rounds, const Hooks& hooks) {
    std::optional<std::filesystem::path> ov;
    if (overlay) ov = detail_shim::path_of(*overlay);
    const nw::BuildResult r = nw::build(cb::base::converted::Project(detail_shim::path_of(project.root)), detail_shim::tools(), detail_shim::hooks(hooks), ov,
                                        static_cast<std::size_t>(rounds));
    BuildResult out;
    out.ok = r.ok;
    if (r.exe) out.exe = cb::compat::to_py(cb::fs::to_text(*r.exe));
    for (const auto& [code, n] : r.errors) out.errors.set(cb::compat::to_py(code), static_cast<int64_t>(n));
    for (const std::string& f : r.first) out.first.append(cb::compat::to_py(f));
    out.build_output_tail = cb::compat::to_py(r.build_output_tail);
    out.rounds = static_cast<int64_t>(r.rounds.size());
    return out;
}

struct RunResult {
    std::optional<int64_t> exit;           // None: still running when the time was up
    py::str output;                        // the last 6000 characters
    py::str log;
};

// Start the built game for `seconds`; xnaport.log and what it printed.
inline RunResult run_for(const py::str& exe, int64_t seconds, const std::vector<std::pair<std::string, std::string>>& env = {}) {
    const std::map<std::string, std::string> e(env.begin(), env.end());
    const nw::RunResult r = nw::run_for(detail_shim::path_of(exe), std::chrono::seconds(seconds), e);
    RunResult out;
    if (r.exit) out.exit = *r.exit;
    out.output = cb::compat::to_py(r.output);
    out.log = cb::compat::to_py(r.log);
    return out;
}

// Every .xnb under the built program's Content/ loaded by the program itself -> {asset: type or "error: why"}, or None when it wrote nothing.
inline std::optional<py::dict<py::str, py::str>> check_content(const py::str& exe, int64_t seconds = 600) {
    const auto r = nw::check_content(detail_shim::path_of(exe), std::chrono::seconds(seconds));
    if (!r) return std::nullopt;
    py::dict<py::str, py::str> out;
    for (const auto& [k, v] : *r) out.set(cb::compat::to_py(k), cb::compat::to_py(v));
    return out;
}

inline const py::str DOC = py::str("xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]");

inline int64_t main(const py::list<py::str>& argv, const Hooks& hooks) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    const py::str converted = py::settings::get_str(py::str("converted"));
    const std::filesystem::path store = converted.empty() ? detail_shim::path_of(py::settings::checkout_root()) / "Converted" : detail_shim::path_of(converted);
    return nw::main(args, cb::base::converted::Store(store), detail_shim::tools(), detail_shim::hooks(hooks));
}

}  // namespace codebreaker::xna::port
