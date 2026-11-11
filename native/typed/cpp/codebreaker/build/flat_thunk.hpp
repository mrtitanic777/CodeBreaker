// codebreaker/build/flat_thunk.hpp -- SHIM over cb/build/flat_thunk.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. No logic; deleted with its last py:: consumer.
//
// The authentic-rebuild and binary-verify engine for a flat-thunk DLL (see cb/build/flat_thunk.hpp): classify a binary by how it
// was built, recover its source, build it, diff the result against the shipped binary.
//
//     classify(path).subtype                   // "flat-thunk" | "win32-c" | "win16-c" | "unknown"
//     recover_flat_thunk(path, outdir)         // <module>.def, <module>k32.def, <module>.thk, <module>.c
//     build_flat_thunk(srcdir) / build_flat_thunk_ow(srcdir)
//     bindiff(built, shipped)
//
//   * Where the tools are comes from the old layer's settings (reference through the old corpus, period_compiler, openwatcom) and the
//     SDK prototypes from the old prototypes table; the new module takes them as plain values.
//   * Not carried over (no old caller used them): the PE/NE/Binary wrappers and `load`, the stub reader and text helpers (underscore
//     names), `_fmt_diff` (the new module's `format_diff` takes its own BinDiff), `_ow_paths`, `_discover_flat_thunk`.
//   * `main` returns the command's exit code (the Python returned nothing, so exit 0, when a step failed); the command is the new
//     module's cb::cli command and its usage is cb::cli's.
//   * A cb::Error reaches the old code as the py:: exception for its meaning (cb::compat::guard).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/build/flat_thunk.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::flat_thunk {

namespace nf = ::cb::build::flat_thunk;

namespace detail {
inline std::filesystem::path path_of(const py::str& s) { return ::cb::compat::legacy::path_of(s); }
inline py::str text_of(const std::filesystem::path& p) { return ::cb::compat::legacy::native(p); }

// Where the tools are, from the settings: `reference` (the Win95 DDK, WIN32SDK and the thunk compiler), `period_compiler`, `openwatcom`.
inline nf::Locations locations() {
    nf::Locations where;
    const py::str reference = base::corpus::reference();
    if (!reference.empty()) where.reference_roots.push_back(path_of(reference));
    where.period_compiler = path_of(py::settings::get_str(py::str("period_compiler")));
    where.openwatcom = ::cb::build::watcom::OpenWatcom::from_settings(py::settings::get_str(py::str("openwatcom")).utf8());
    return where;
}

// The SDK prototypes of the old table (win32), as the new module takes them.
inline std::map<std::string, nf::SdkPrototype> sdk() {
    ::cb::symbols::prototypes::Headers headers;
    for (const auto& [name, p] : symbols::prototypes::table().w32.items()) {
        ::cb::symbols::prototypes::Prototype q;
        q.returns = p.returns.utf8();
        q.convention = p.convention.utf8();
        q.marker = p.marker.utf8();
        q.header = p.header.utf8();
        for (const auto& [type, parameter] : p.parameters.vec()) q.parameters.push_back({type.utf8(), parameter.utf8()});
        headers.w32.insert_or_assign(name.utf8(), std::move(q));
    }
    return nf::sdk_prototypes(::cb::symbols::prototypes::PrototypeTable(std::move(headers)));
}

inline nf::Services services() {
    nf::Services s;
    s.locations = locations();
    s.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
        const std::optional<py::str> found = base::corpus::retail(py::str(std::string(name)));
        if (!found) return std::nullopt;
        return path_of(*found);
    };
    s.sdk = [] { return sdk(); };
    return s;
}
}  // namespace detail

// How the module was built.
struct Classification {
    py::str format, subtype;
    std::optional<py::str> recipe;
    std::optional<int64_t> n_exports, n_imports;
    py::str why;

    // the dict's items in the Python's order, the value as str() gives it
    py::list<std::tuple<py::str, py::str>> items() const {
        py::list<std::tuple<py::str, py::str>> out;
        out.append(std::make_tuple(py::str("format"), format));
        out.append(std::make_tuple(py::str("subtype"), subtype));
        out.append(std::make_tuple(py::str("recipe"), recipe ? *recipe : py::str("None")));
        if (n_exports) out.append(std::make_tuple(py::str("n_exports"), py::str(std::to_string(*n_exports))));
        if (n_imports) out.append(std::make_tuple(py::str("n_imports"), py::str(std::to_string(*n_imports))));
        out.append(std::make_tuple(py::str("why"), why));
        return out;
    }
};

inline Classification classify(const py::str& path) {
    return ::cb::compat::guard([&] {
        const nf::Classification c = nf::classify(detail::path_of(path));
        Classification out;
        out.format = py::str(std::string(nf::format_name(c.format)));
        out.subtype = py::str(std::string(nf::subtype_name(c.subtype)));
        if (c.recipe) out.recipe = py::str(*c.recipe);
        if (c.exports) out.n_exports = static_cast<int64_t>(*c.exports);
        if (c.imports) out.n_imports = static_cast<int64_t>(*c.imports);
        out.why = py::str(c.why);
        return out;
    });
}

// The period tools.
struct Toolchain {
    std::optional<py::str> thunk, ml;
    py::str cl, link, lib, rc, include;
    py::list<py::str> lib_dirs;
};

inline Toolchain toolchain() {
    const nf::Toolchain t = nf::toolchain(detail::locations());
    Toolchain out;
    if (!t.thunk.empty()) out.thunk = detail::text_of(t.thunk);
    if (!t.ml.empty()) out.ml = detail::text_of(t.ml);
    out.cl = detail::text_of(t.cl);
    out.link = detail::text_of(t.link);
    out.lib = detail::text_of(t.lib);
    out.rc = detail::text_of(t.rc);
    out.include = detail::text_of(t.include);
    for (const auto& d : t.lib_dirs) out.lib_dirs.append(detail::text_of(d));
    return out;
}

// What a build made.
struct BuildResult {
    bool ok = false;
    std::optional<py::str> error, log;
    std::optional<py::str> dll, module;
    int64_t n_exports = 0, bytes = 0;
};

namespace detail {
inline BuildResult build_result(const nf::BuildResult& r) {
    BuildResult out;
    out.ok = r.ok;
    if (!r.error.empty()) out.error = py::str(r.error);
    if (!r.log.empty()) out.log = py::str(r.log);
    if (r.dll) out.dll = text_of(*r.dll);
    if (!r.module.empty()) out.module = py::str(r.module);
    out.n_exports = static_cast<int64_t>(r.exports);
    out.bytes = static_cast<int64_t>(r.bytes);
    return out;
}
}  // namespace detail

// From-source OpenWatcom, no Microsoft tools.
inline BuildResult build_flat_thunk_ow(const py::str& srcdir, const std::optional<py::str>& outdir = std::nullopt) {
    return ::cb::compat::guard([&] {
        std::optional<std::filesystem::path> out;
        if (outdir) out = detail::path_of(*outdir);
        return detail::build_result(nf::build_openwatcom(detail::locations().openwatcom, detail::path_of(srcdir), out));
    });
}

// The thunk compiler, ML, CL and LINK on the recovered source.
inline BuildResult build_flat_thunk(const py::str& srcdir, const std::optional<py::str>& outdir = std::nullopt) {
    return ::cb::compat::guard([&] {
        std::optional<std::filesystem::path> out;
        if (outdir) out = detail::path_of(*outdir);
        return detail::build_result(nf::build_period(detail::locations(), detail::path_of(srcdir), out));
    });
}

struct CompileOw {
    bool ok = false;
    std::optional<py::str> error;
    py::list<std::tuple<py::str, int64_t, py::str>> results;     // (file, rc, first_error)
};

inline CompileOw compile_ow(const py::str& srcdir) {
    return ::cb::compat::guard([&] {
        const nf::CompileResult r = nf::compile_openwatcom(detail::locations().openwatcom, detail::path_of(srcdir));
        CompileOw out;
        out.ok = r.ok;
        if (!r.error.empty()) out.error = py::str(r.error);
        for (const nf::CompiledFile& f : r.files) out.results.append(std::make_tuple(py::str(f.file), static_cast<int64_t>(f.exit_code), py::str(f.first_error)));
        return out;
    });
}

// What recover_flat_thunk wrote and found.
struct Recovered {
    std::optional<py::str> error;
    py::str module;
    int64_t n_exports = 0;
    py::list<int64_t> k32_ordinals, unknown_ordinals;
    py::str outdir;
    std::optional<py::str> target16;
    py::list<py::str> dll_candidates;
    int64_t n_signatures_recovered = 0, n_pointer_args = 0, n_sdk_prototypes = 0;
    py::list<py::str> recovered, scaffolded;
};

inline Recovered recover_flat_thunk(const py::str& pe_path, const py::str& outdir) {
    Recovered out;
    try {
        const nf::Recovery r = nf::recover(detail::path_of(pe_path), detail::path_of(outdir), detail::sdk());
        out.module = py::str(r.module);
        out.n_exports = static_cast<int64_t>(r.exports);
        for (const std::uint32_t o : r.kernel32_ordinals) out.k32_ordinals.append(o);
        for (const std::uint32_t o : r.unknown_ordinals) out.unknown_ordinals.append(o);
        out.outdir = detail::text_of(r.outdir);
        if (r.target16) out.target16 = py::str(*r.target16);
        out.dll_candidates = ::cb::compat::to_py(r.dll_candidates);
        out.n_signatures_recovered = static_cast<int64_t>(r.signatures);
        out.n_pointer_args = static_cast<int64_t>(r.pointer_args);
        out.n_sdk_prototypes = static_cast<int64_t>(r.sdk_prototypes);
        out.recovered = ::cb::compat::to_py(r.recovered);
        out.scaffolded = ::cb::compat::to_py(r.scaffolded);
    } catch (const ::cb::UsageError& e) {   // "not a PE": the Python answered with an error entry, not an exception
        out.error = py::str(e.what());
    } catch (const ::cb::Error& e) {
        ::cb::compat::rethrow_as_py(e);
    }
    return out;
}

// What bindiff found (the PE and NE answers are one record, as the Python's dict was).
struct BinDiff {
    std::optional<py::str> error;
    bool named = false;                                    // built / shipped / sizes are in the answer
    py::str built, shipped;
    int64_t size_built = 0, size_shipped = 0;
    std::optional<py::str> kind;                           // "PE" / "NE"
    // PE
    bool entry_match = false;
    std::tuple<py::str, py::str> entry;                    // (hex(built), hex(shipped))
    py::list<py::str> sections_built, sections_shipped;
    bool sections_match = false, exports_match = false;
    std::tuple<int64_t, int64_t> n_exports, n_imports;
    py::list<py::str> import_dlls_built, import_dlls_shipped, imports_only_in_shipped, imports_only_in_built;
    // NE
    std::tuple<int64_t, int64_t> nseg;
    py::list<py::str> seg_kinds_built, seg_kinds_shipped;
    bool seg_kinds_match = false;
    std::optional<py::str> verdict;
};

inline BinDiff bindiff(const py::str& built, const py::str& shipped) {
    BinDiff out;
    const ::cb::Result<nf::BinDiff> r = ::cb::compat::guard([&] { return nf::bindiff(detail::path_of(built), detail::path_of(shipped)); });
    if (!r.ok()) {
        out.error = py::str(r.error().what());
        return out;
    }
    const nf::BinDiff& d = r.value();
    out.named = true;
    out.built = py::str(d.built);
    out.shipped = py::str(d.shipped);
    out.size_built = static_cast<int64_t>(d.size_built);
    out.size_shipped = static_cast<int64_t>(d.size_shipped);
    out.verdict = py::str(std::string(nf::verdict_name(d.verdict)));
    const auto hex = [](std::uint32_t v) { return py::str(std::format("0x{:x}", v)); };
    const auto pair = [](const auto& p) { return std::make_tuple(static_cast<int64_t>(p.first), static_cast<int64_t>(p.second)); };
    const auto kinds = [](const std::vector<nf::SegmentKind>& k) {
        py::list<py::str> names;
        for (const nf::SegmentKind x : k) names.append(py::str(x == nf::SegmentKind::Data ? "DATA" : "CODE"));
        return names;
    };
    if (const nf::PeDiff* pe = std::get_if<nf::PeDiff>(&d.detail)) {
        out.kind = py::str("PE");
        out.entry_match = pe->entry_match;
        out.entry = std::make_tuple(hex(pe->entry.first), hex(pe->entry.second));
        out.sections_built = ::cb::compat::to_py(pe->sections_built);
        out.sections_shipped = ::cb::compat::to_py(pe->sections_shipped);
        out.sections_match = pe->sections_match;
        out.exports_match = pe->exports_match;
        out.n_exports = pair(pe->exports);
        out.n_imports = pair(pe->import_count);
        out.import_dlls_built = ::cb::compat::to_py(pe->import_dlls_built);
        out.import_dlls_shipped = ::cb::compat::to_py(pe->import_dlls_shipped);
        out.imports_only_in_shipped = ::cb::compat::to_py(pe->imports_only_in_shipped);
        out.imports_only_in_built = ::cb::compat::to_py(pe->imports_only_in_built);
    } else {
        const nf::NeDiff& ne = std::get<nf::NeDiff>(d.detail);
        out.kind = py::str("NE");
        out.nseg = pair(ne.segments);
        out.seg_kinds_built = kinds(ne.kinds_built);
        out.seg_kinds_shipped = kinds(ne.kinds_shipped);
        out.seg_kinds_match = ne.kinds_match;
        out.exports_match = ne.exports_match;
    }
    return out;
}

// The command line: `flat-thunk <module> [--recover DIR] [--ow-compile] [--build DIR] [--ow-build DIR] [--diff BUILT]`.
inline std::optional<int64_t> main(const py::list<py::str>& argv) {
    std::vector<std::string> args{"flat-thunk"};
    for (int64_t i = 0; i < argv.size(); ++i) args.push_back(argv[i].utf8());
    const ::cb::cli::Cli app("flat-thunk", {nf::command(detail::services())});
    return ::cb::compat::guard([&] { return static_cast<int64_t>(app.run(args)); });
}

}  // namespace codebreaker::build::flat_thunk
