// codebreaker/build/watcom.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/watcom.hpp for the ports that still use the old
// API (build/module, build/driver, build/flat_thunk, decompile/*, verify/*, ...). Staged under native/cb/compat/shims/ until
// the exe's build is wired for C++20.
//
// Old API: OW, _ow, _scratch, _RTL, IMAGE_BASE, _BASE, _ow_env, import_map, _import_lines, _first, openwatcom_root, ow_tool,
// ow_cc, ow_include, ow_lib, ow_env, Numbered. (_IMPORT_MAPS and _EXPORT_NAMES, the mutable tables nothing outside watcom.cpp
// used, are now owned by the shim's one ImportMaps.)
//
// Two readers stay with the old watcom.cpp, which shrinks to just them -- COMPAT(py): remove when formats/pe and formats/omf
// are refactored: `pe_imports` (a PE module's import table, from formats/pe) and `omf_externs` (an object's external symbols,
// from formats/omf).
#pragma once

#include <optional>
#include <tuple>

#include "cb/build/watcom.hpp"
#include "cb/compat/py_bridge.hpp"
#include "../../../../../ported/include/py_os.hpp"
#include "../../../../../ported/include/py_settings.hpp"

namespace codebreaker::build::watcom {

// ---- COMPAT(py): defined by the old watcom.cpp ------------------------------------------------------------------------
// [(DLL file name, symbol)] of a PE module's import table, symbol "#N" for an import by ordinal. ValueError and the like when
// the module is not a readable PE.
py::list<std::tuple<py::str, py::str>> pe_imports(const py::str& module);
// The external symbols of an OMF object.
py::list<py::str> omf_externs(const py::bytes& obj);

// {"root", "wcc", "wlink", "inc"} of _ow()
struct OW {
    py::str root, wcc, wlink;
    py::list<py::str> inc;
};

// {name: (DLL, number, bytes of arguments removed or None)}
using Numbered = py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>>;

inline const py::str _RTL = py::str("library clib3r.lib\n");
inline constexpr int64_t IMAGE_BASE = 0x6E000000;
inline const py::str _BASE = py::str("option offset=0x6E000000\n");

namespace detail {
namespace nw = cb::build::watcom;

inline nw::OpenWatcom current() { return nw::OpenWatcom::from_settings(py::settings::get_str(py::str("openwatcom")).utf8()); }
inline nw::Target target_of(const py::str& cls) { return cls == py::str("ne") ? nw::Target::Win16 : nw::Target::Win32; }
inline py::str text(const std::filesystem::path& p) { return p.empty() ? py::str("") : py::str(cb::fs::to_text(p)); }

// The process's import maps (the old watcom.cpp kept two global tables).
inline nw::ImportMaps& maps() {
    static nw::ImportMaps m([](const std::filesystem::path& module) {
        std::vector<nw::ImportedName> out;
        for (const auto& [dll, name] : pe_imports(py::str(cb::fs::to_text(module))).vec()) out.push_back({dll.utf8(), name.utf8()});
        return out;
    });
    return m;
}
}  // namespace detail

// Root of the from-source OpenWatcom release tree (CB_OPENWATCOM overrides).
inline py::str openwatcom_root() { return py::str(cb::fs::to_text(detail::current().root())); }

// Absolute path of an OpenWatcom tool ('wcc', 'wcc386', 'wlink', 'wasm', 'wrc'); "" when OpenWatcom is not built.
inline py::str ow_tool(const py::str& name) { return detail::text(detail::current().tool(name.utf8())); }
inline py::str ow_cc(const py::str& cls) { return detail::text(detail::current().cc(detail::target_of(cls))); }
inline py::list<py::str> ow_include(const py::str& cls) {
    py::list<py::str> out;
    for (const auto& p : detail::current().include_dirs(detail::target_of(cls))) out.append(py::str(cb::fs::to_text(p)));
    return out;
}
inline py::str ow_lib(const py::str& cls) { return py::str(cb::fs::to_text(detail::current().lib_dir(detail::target_of(cls)))); }

// Process environment for running OpenWatcom tools on a module class: the process's, with what the tools need set.
inline py::dict<py::str, py::str> ow_env(const py::str& cls) {
    py::dict<py::str, py::str> env = py::os::environ_::copy();   // dict(os.environ)
    for (const auto& [name, value] : detail::current().environment(detail::target_of(cls))) env.set(py::str(name), py::str(value));
    return env;
}

inline OW _ow() {
    OW ow;
    ow.root = openwatcom_root();
    ow.wcc = ow_cc(py::str("pe"));
    ow.wlink = ow_tool(py::str("wlink"));
    ow.inc = ow_include(py::str("pe"));
    return ow;
}
inline py::dict<py::str, py::str> _ow_env(const OW&) { return ow_env(py::str("pe")); }

// Build scratch lives in the temp dir, not beside the source.
inline py::str _scratch(const py::list<py::str>& parts) {
    std::vector<std::string> utf8;
    for (const py::str& p : parts.vec()) utf8.push_back(p.utf8());
    std::vector<std::string_view> views(utf8.begin(), utf8.end());
    return py::str(cb::fs::to_text(cb::build::watcom::scratch_dir(views)));
}

// {name: dll} for the functions a retail PE imports by name (the first DLL wins).
inline py::dict<py::str, py::str> import_map(const py::str& module) {
    py::dict<py::str, py::str> out;
    // (a module that cannot be read has no imports: ImportMaps handles that boundary)
    for (const auto& [c_name, dll] : *detail::maps().of(cb::fs::from_text(module.utf8()))) out.set(py::str(c_name), py::str(dll));
    return out;
}

// wlink `import` directives binding each imported name's decorated symbol to DLL.name -- or to DLL.number for one the original
// imports by number (`numbered`).
inline py::str _import_lines(const py::bytes& obj, const py::set<py::str>& names, const py::dict<py::str, py::str>& imports,
                             const std::optional<Numbered>& numbered = std::nullopt) {
    std::vector<std::string> externs;
    for (const py::str& e : omf_externs(obj).vec()) externs.push_back(e.utf8());
    std::vector<std::string> wanted;
    for (const py::str& n : names) wanted.push_back(n.utf8());
    cb::build::watcom::ImportMap map;
    for (const auto& [name, dll] : imports.items()) map[name.utf8()] = dll.utf8();
    std::unordered_map<std::string, cb::build::watcom::NumberedImport> by_number;
    if (numbered) {
        for (const auto& [name, entry] : numbered->items()) {
            cb::build::watcom::NumberedImport n;
            n.dll = std::get<0>(entry).utf8();
            n.number = cb::compat::int_from_py<std::uint32_t>(std::get<1>(entry));
            if (std::get<2>(entry)) n.argument_bytes = cb::compat::int_from_py<std::uint32_t>(*std::get<2>(entry));
            by_number[name.utf8()] = n;
        }
    }
    return py::str(cb::build::watcom::import_lines(externs, wanted, map, detail::maps(), numbered ? &by_number : nullptr));
}

// The first candidate that is a path that exists, else "".
inline py::str _first(const py::list<py::str>& cands) {
    for (const py::str& c : cands.vec()) {
        if (!c.empty() && py::os::path::exists(c)) return c;
    }
    return py::str("");
}

}  // namespace codebreaker::build::watcom
