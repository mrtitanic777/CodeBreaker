// codebreaker/build/driver.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/driver.hpp for the ports that still use the old API
// (platforms/dos, platforms/vxd, source/assembly, source/driver_writer, verify/lanes/driver). No logic: conversion and forwarding.
// Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: SEGMENT_CLASSES, _DECLARE_PATTERN, _TIMEOUT, tool, kit_folder, source_of, original_of, assemble, link, Structure, structure,
// compare, BuildResult, build, report, MAIN_DOC, main. (_run, _patched and _code_at were helpers of the structural check: not
// carried over.)
//
// The old functions found the OpenWatcom tree, the DDK's include folder, the corpus and the state folder themselves, through the
// old settings, the corpus and symbols/driver_services; the shim does the same and hands the new module what it finds --
// COMPAT(py): remove when symbols/driver_services and base/corpus are refactored (they then provide the kit folder and the corpus).
//
// DIVERGES, for the old callers: build() of a source folder without its .asm and main() print/raise as the new module does
// (UsageError, which reaches old code as ValueError); `driver-build` with no source prints the new command's usage (exit 2).
#pragma once

#include <optional>
#include <string>
#include <tuple>

#include "watcom.hpp"   // the shim beside this one: the old settings-driven OpenWatcom

#include "cb/build/driver.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::driver {

namespace nd = cb::build::driver;

// What the kit's module-definition file says of each class of segment, as wlink spells it.
inline const py::list<std::tuple<py::str, py::str>> SEGMENT_CLASSES = [] {
    py::list<std::tuple<py::str, py::str>> out;
    for (const nd::SegmentClass& c : nd::kSegmentClasses) out.append(std::make_tuple(py::str(std::string(c.name)), py::str(std::string(c.attributes))));
    return out;
}();
inline const py::str _DECLARE_PATTERN = py::str("^\\s*Declare_Virtual_Device\\s+(\\w+)");
inline const int64_t _TIMEOUT = 300;

namespace detail {

inline cb::build::watcom::OpenWatcom open_watcom() {
    return cb::build::watcom::OpenWatcom::from_settings(py::settings::get_str(py::str("openwatcom")).utf8());
}
inline std::filesystem::path state_folder() { return cb::fs::from_text(py::settings::get_str(py::str("state")).utf8()); }
inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(s.utf8()); }
inline py::str text_of(const std::filesystem::path& p) { return py::str(p.empty() ? std::string() : cb::fs::to_text(p)); }

// The tools as the old functions found them: the OpenWatcom of the settings, the DDK's include folder, the state folder.
inline nd::Tools tools() {
    const py::str kit = symbols::driver_services::include_dir();
    return nd::find_tools(open_watcom(), path_of(kit), state_folder());
}

inline std::vector<std::filesystem::path> corpus_drivers() {
    std::vector<std::filesystem::path> out;
    for (const py::str& p : formats::le::corpus_files().vec()) out.push_back(path_of(p));
    return out;
}

inline std::optional<std::filesystem::path> retail(std::string_view name) {
    const auto found = base::corpus::retail(py::str(std::string(name)));
    if (!found) return std::nullopt;
    return path_of(*found);
}

}  // namespace detail

// The assembler or linker to use. Absolute path, or '' when there is none.
inline py::str tool(const py::str& name) { return detail::text_of(nd::tool(detail::open_watcom(), name.utf8())); }

// The kit's include files in a folder whose path has no spaces: the kit's own folder if it has none, else a copy under state/.
inline py::str kit_folder() {
    return cb::compat::guard([&] {
        const py::str src = symbols::driver_services::include_dir();
        return detail::text_of(nd::kit_folder(detail::path_of(src), detail::state_folder() / "driver_kit"));
    });
}

// The .asm to build: `path` itself, or in a folder the file named after the folder. (ValueError: none in it)
inline py::str source_of(const py::str& path) {
    return cb::compat::guard([&] { return detail::text_of(nd::source_of(detail::path_of(path))); });
}

// The corpus driver a source file was made from: the one with the same name, or None.
inline std::optional<py::str> original_of(const py::str& source) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        const auto found = nd::original_of(detail::path_of(source), detail::corpus_drivers());
        if (!found) return std::nullopt;
        return detail::text_of(*found);
    });
}

// -> (object file or None, the assembler's messages)
inline std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work) {
    return cb::compat::guard([&] {
        const nd::ToolResult r = nd::assemble(detail::tools(), detail::path_of(source), detail::path_of(work));
        return std::make_tuple(r.file ? std::optional<py::str>(detail::text_of(*r.file)) : std::nullopt, py::str(r.log));
    });
}

// Link one object file into a driver exporting <device>_DDB. -> (file or None, messages)
inline std::tuple<std::optional<py::str>, py::str> link(const py::str& obj, const py::str& device, const py::str& out, bool dynamic = false,
                                                        const std::optional<py::list<std::tuple<py::str, py::str>>>& classes = std::nullopt,
                                                        bool named = true, const std::optional<py::str>& definitions = std::nullopt) {
    return cb::compat::guard([&] {
        nd::LinkOptions options;
        options.dynamic = dynamic;
        options.named = named;
        if (classes) {
            for (const auto& [cls, attributes] : classes->vec()) options.classes.emplace_back(cls.utf8(), attributes.utf8());
        }
        if (definitions) options.definitions = definitions->utf8();
        const nd::ToolResult r = nd::link(detail::tools(), detail::path_of(obj), device.utf8(), detail::path_of(out), options);
        return std::make_tuple(r.file ? std::optional<py::str>(detail::text_of(*r.file)) : std::nullopt, py::str(r.log));
    });
}

// What compare() looks at, for one image.
struct Structure {
    py::list<std::tuple<int64_t, int64_t>> objects;               // (vsize, flags)
    std::optional<formats::le::Descriptor> descriptor;
    py::list<std::optional<formats::le::Place>> services;
    py::list<std::tuple<py::str, int64_t>> exports;               // (name, ordinal)
    bool export_is_descriptor = false;
    int64_t module = 0;
    int64_t references = 0;
};
inline Structure structure(const formats::le::LEFile& lf) {
    return cb::compat::guard([&] {
        const nd::Structure s = nd::structure(lf.module());
        Structure out;
        for (const auto& o : s.objects) out.objects.append(std::make_tuple(int64_t{o.size}, int64_t{o.flags}));
        if (s.descriptor) out.descriptor = formats::le::conv::descriptor(*s.descriptor);
        for (const auto& p : s.services) {
            out.services.append(p ? std::optional<formats::le::Place>(std::make_tuple(int64_t{p->object}, p->offset)) : std::nullopt);
        }
        for (const auto& e : s.exports) out.exports.append(std::make_tuple(py::str(e.name.value_or("")), int64_t{e.ordinal}));
        out.export_is_descriptor = s.export_is_descriptor;
        out.module = s.module_flags;
        out.references = static_cast<int64_t>(s.references);
        return out;
    });
}

// Check a rebuilt image against the original (two LEFile). -> (checks, notes, same): checks {name: True/False} for objects, descriptor,
// services, exports, module; notes, one line for each that failed; same = share of bytes equal outside patched places (None when no
// objects pair up).
inline std::tuple<py::dict<py::str, bool>, py::list<py::str>, std::optional<double>> compare(const formats::le::LEFile& built,
                                                                                            const formats::le::LEFile& original) {
    return cb::compat::guard([&] {
        const nd::Comparison c = nd::compare(built.module(), original.module());
        py::dict<py::str, bool> checks;
        checks.set(py::str("objects"), c.checks.objects);
        checks.set(py::str("descriptor"), c.checks.descriptor);
        checks.set(py::str("services"), c.checks.services);
        checks.set(py::str("exports"), c.checks.exports);
        checks.set(py::str("module"), c.checks.module);
        py::list<py::str> notes;
        for (const std::string& n : c.notes) notes.append(py::str(n));
        return std::make_tuple(checks, notes, c.same);
    });
}

// {"name", "source", "assembled", "linked", "path", "log", "original", "checks", "notes", "same"}
struct BuildResult {
    py::str name, source;
    bool assembled = false, linked = false;
    std::optional<py::str> path;
    py::str log;
    std::optional<py::str> original;
    py::dict<py::str, bool> checks;
    py::list<py::str> notes;
    std::optional<double> same;
};

// Assemble and link the driver whose source is `source` (a folder or an .asm). With `original` (a file or a corpus name; found by name
// when not given) the result is checked against it.
inline BuildResult build(const py::str& source, const std::optional<py::str>& original = std::nullopt, const std::optional<py::str>& out = std::nullopt) {
    return cb::compat::guard([&] {
        nd::BuildRequest request;
        request.source = detail::path_of(source);
        const std::filesystem::path asm_file = nd::source_of(request.source);
        const std::string name = cb::text::lower(asm_file.stem().string());
        request.work = out ? detail::path_of(*out) : detail::state_folder() / "driver_build" / name;
        if (original && !original->empty()) {
            request.original = detail::retail(original->utf8());
        } else {
            request.original = nd::original_of(asm_file, detail::corpus_drivers());
        }
        const nd::BuildResult r = nd::build(detail::tools(), request);
        BuildResult o;
        o.name = py::str(r.name);
        o.source = detail::text_of(r.source);
        o.assembled = r.assembled;
        o.linked = r.linked;
        if (r.path) o.path = detail::text_of(*r.path);
        o.log = py::str(r.log);
        if (r.original) o.original = detail::text_of(*r.original);
        if (r.checks) {
            o.checks.set(py::str("objects"), r.checks->objects);
            o.checks.set(py::str("descriptor"), r.checks->descriptor);
            o.checks.set(py::str("services"), r.checks->services);
            o.checks.set(py::str("exports"), r.checks->exports);
            o.checks.set(py::str("module"), r.checks->module);
        }
        for (const std::string& n : r.notes) o.notes.append(py::str(n));
        o.same = r.same;
        return o;
    });
}

// One line for a build result.
inline py::str report(const BuildResult& r) {
    nd::BuildResult n;
    n.name = r.name.utf8();
    n.assembled = r.assembled;
    n.linked = r.linked;
    n.log = r.log.utf8();
    for (const py::str& note : r.notes.vec()) n.notes.push_back(note.utf8());
    if (!r.checks.empty()) {
        nd::Checks c;
        c.objects = r.checks[py::str("objects")];
        c.descriptor = r.checks[py::str("descriptor")];
        c.services = r.checks[py::str("services")];
        c.exports = r.checks[py::str("exports")];
        c.module = r.checks[py::str("module")];
        n.checks = c;
    }
    n.same = r.same;
    return py::str(nd::report(n));
}

// python -m codebreaker driver-build <source folder or .asm> ... [--original FILE] [--out DIR]
inline const py::str MAIN_DOC = py::str("python -m codebreaker driver-build <source folder or .asm> ... [--original FILE] [--out DIR]");
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t {
        nd::Environment env;
        env.tools = detail::tools();
        env.retail = [](std::string_view name) { return detail::retail(name); };
        env.corpus_drivers = detail::corpus_drivers();
        env.state = detail::state_folder();
        const cb::cli::Cli app("codebreaker", {nd::command(env)});
        std::vector<std::string> args{"driver-build"};
        for (const py::str& a : argv.vec()) args.push_back(a.utf8());
        return app.run(args);
    });
}

}  // namespace codebreaker::build::driver
