// codebreaker/source/vxd/link.hpp -- SHIM over cb/source/vxd/link.hpp (docs/CPP_DESIGN.md 6.3): a full-file LE rebuild of a VxD via ML (-coff) and
// LINK /VXD. The old types in (VxDDisasm, LEFile, py::dict results), the new builds called, the old types out; the tools are found as the old layer's
// settings, corpus and DOS lane find them (masm.hpp's shim). Deleted with its last py:: consumer (the VxD from-source and OpenWatcom builds).
//
//     BuildAndCompare r = build_and_compare(v, "VKD");
//
// Differences from the old port that the shim cannot hide:
//   * The percentage of an object match (`pct`) is the number rounded half away from zero to a decimal where Python rounded half to even.
//   * `_assemble_coff` runs the DOS lane of the old layer (the native executive, then DOSBox) and leaves the work folder for `_link_vxd`; the folder
//     is the caller's to remove, as the old one was.
//   * `_assemble_idiomatic` uses the native ML when there is one and the period assembler when there is not.
//   * `_source_code_into_container` changes `D.code` as the old one did; a refused overlay is the ValueError it was.
#pragma once

#include <cmath>
#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/link.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/idiom.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::link {

using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;
namespace cbl = cb::source::vxd::link;
using ForceRaw = py::dict<int64_t, py::set<int64_t>>;

// MS LINK 2.50 (the VC++ 2.0 linker), retained for the legacy byte-exact object experiments.
inline const py::str _LINK = py::str("toolchains/period/compilers/msvc-2.0/BIN/LINK.EXE");
inline const py::list<py::str> _LINK260_FILES = {py::str("LINK.EXE"), py::str("DBI.DLL"), py::str("LINK.ERR"), py::str("IMAGEHLP.DLL")};
// the standard DDK VxD segment map
inline const py::str _VXD_DEF_SEGS = cb::compat::to_py(cbl::kVxdDefSegments);

namespace shim_ {

// The tools of one call: the assemblers and the linker of the old layer's settings, and the DDK's names.
struct Tools {
    cb::source::vxd::masm::Toolchain assemblers;
    cbl::Environment env;
    explicit Tools(const py::str& root)
        : assemblers(masm::shim_::toolchain(root)),
          env{disassembly::shim_::names(), assemblers, cbl::standard_link(cb::fs::from_text(py::os::path::join(root, _LINK).utf8()))} {}
    Tools(const Tools&) = delete;
    Tools& operator=(const Tools&) = delete;
};


inline cbl::ForceRaw new_force_raw(const ForceRaw& f) {
    cbl::ForceRaw out;
    for (const auto& kv : f.items()) {
        for (const int64_t k : std::get<1>(kv)) out[static_cast<uint32_t>(std::get<0>(kv))].insert(static_cast<uint32_t>(k));
    }
    return out;
}

inline ForceRaw old_force_raw(const cbl::ForceRaw& f) {
    ForceRaw out;
    for (const auto& [object, offsets] : f) {
        py::set<int64_t> set;
        for (const uint32_t k : offsets) set.add(int64_t{k});
        out.set(int64_t{object}, set);
    }
    return out;
}

inline double round1(double x) { return std::round(x * 10.0) / 10.0; }

// A work folder that stays: the caller removes it.
inline py::str keep_work_folder(const char* prefix) {
    cb::fs::ScopedTempDir dir(prefix);
    return cb::compat::to_py(cb::fs::to_text(dir.release()));
}

}  // namespace shim_

// .DEF segment attributes for an LE object's flags (the ones LINK /VXD maps back to flags).
inline py::str _seg_attrs(int64_t flags) { return cb::compat::to_py(cbl::segment_attributes(static_cast<uint32_t>(flags))); }

// Give every INTERNAL fixup target a label so the functional emit can reference it.
inline void _label_fixup_targets(VxDDisasm& D) {
    cb::compat::guard([&] {
        auto code = masm::shim_::emission_of(D);
        cb::source::vxd::idiom::label_fixup_targets(code);
        for (const auto& [at, label] : code.labels()) D.labels.set(idiom::shim_::old_place(at), cb::compat::to_py(label));
    });
}

// Object body for a FUNCTIONAL (loadable, not byte-exact) rebuild.
inline py::list<py::str> _link_body(const VxDDisasm& D, int64_t oi, const std::optional<py::dict<int64_t, py::str>>& extra_labels = std::nullopt) {
    return cb::compat::guard([&] {
        std::map<uint32_t, std::string> extra;
        if (extra_labels) {
            for (const auto& kv : extra_labels->items()) extra[static_cast<uint32_t>(std::get<0>(kv))] = std::get<1>(kv).utf8();
        }
        return cb::compat::to_py(cbl::link_body(masm::shim_::emission_of(D), static_cast<uint32_t>(oi), extra));
    });
}

// (asm_text, def_text) for the whole VxD; `functional` emits internal fixups symbolically.
inline std::tuple<py::str, py::str> emit_source_and_def(VxDDisasm& D, const py::str& vxdname, const std::optional<ForceRaw>& force_raw = std::nullopt, bool functional = false) {
    return cb::compat::guard([&] {
        auto code = masm::shim_::emission_of(D);
        const cbl::SourceAndDef out = cbl::emit_source_and_def(code, cb::compat::from_py(vxdname), force_raw ? shim_::new_force_raw(*force_raw) : cbl::ForceRaw{}, functional);
        if (functional) {
            for (const auto& [at, label] : code.labels()) D.labels.set(idiom::shim_::old_place(at), cb::compat::to_py(label));
        }
        return std::make_tuple(cb::compat::to_py(out.source), cb::compat::to_py(out.def));
    });
}

// -> (obj or None, log, work dir)
inline std::tuple<std::optional<py::bytes>, py::str, std::optional<py::str>> _assemble_coff(const py::str& asm_text, const py::str& root) {
    return cb::compat::guard([&] {
        const auto made = masm::shim_::period_batch({asm_text}, root);
        const py::str work = shim_::keep_work_folder("cb_idiom_");
        return std::make_tuple(std::get<0>(made.front()), std::get<1>(made.front()), std::optional<py::str>(work));
    });
}

// Link one COFF object into a .vxd with LINK /VXD (native). -> (the file or None, the output)
inline std::tuple<std::optional<py::bytes>, py::str> _link_vxd(const py::bytes& obj_bytes, const py::str& def_text, const py::str& root, const py::str& work) {
    return cb::compat::guard([&] {
        const shim_::Tools tools(root);
        const cbl::Linked made = tools.env.link(cb::compat::from_py(obj_bytes), cb::compat::from_py(def_text), cb::fs::from_text(cb::compat::from_py(work)));
        return std::make_tuple(made.vxd ? std::optional<py::bytes>(cb::compat::to_py(cb::ByteView(*made.vxd))) : std::nullopt, cb::compat::to_py(made.log));
    });
}

// Copy LINK 2.60 + its DLLs into a cached SPACE-FREE dir and return the staged LINK.EXE path.
inline py::str _stage_link260(const py::str&) {
    return cb::compat::guard([&] {
        const std::filesystem::path source = cb::fs::from_text(masm::_ddk_root().utf8()) / "MSVC20";
        const std::filesystem::path stage = std::filesystem::temp_directory_path() / "cb_link260";
        cb::fs::make_dirs(stage);
        for (const py::str& f : _LINK260_FILES) {
            std::error_code ec;
            if (!std::filesystem::exists(stage / f.utf8(), ec)) std::filesystem::copy_file(source / f.utf8(), stage / f.utf8(), ec);
        }
        return cb::compat::to_py(cb::fs::to_text(stage / "LINK.EXE"));
    });
}

// Link one COFF object into a loadable .vxd with the DDK's LINK 2.60 /VXD.
inline std::tuple<std::optional<py::bytes>, py::str> link_vxd_260(const py::bytes& obj_bytes, const py::str& def_text, const py::str&, const py::str& work, int64_t align = 0x1000) {
    return cb::compat::guard([&] {
        const std::filesystem::path source = cb::fs::from_text(masm::_ddk_root().utf8()) / "MSVC20";
        const cbl::Linked made = cbl::link_260(source, std::filesystem::temp_directory_path() / "cb_link260", static_cast<uint32_t>(align))(
            cb::compat::from_py(obj_bytes), cb::compat::from_py(def_text), cb::fs::from_text(cb::compat::from_py(work)));
        return std::make_tuple(made.vxd ? std::optional<py::bytes>(cb::compat::to_py(cb::ByteView(*made.vxd))) : std::nullopt, cb::compat::to_py(made.log));
    });
}

// Per exec object, the force-raw offsets that made the object-level byte-exact test exact, and whether it matched.
inline std::tuple<ForceRaw, py::dict<int64_t, bool>> byte_exact_force_raw(const le::LEFile& v, const py::str& root = py::str(".")) {
    return cb::compat::guard([&] {
        const shim_::Tools tools(py::os::path::abspath(root));
        const cbl::ForceRawResult r = cbl::byte_exact_force_raw(tools.env, v.module());
        py::dict<int64_t, bool> matched;
        for (const auto& [object, ok] : r.matched) matched.set(int64_t{object}, ok);
        return std::make_tuple(shim_::old_force_raw(r.force_raw), matched);
    });
}

// compare_objects' report
struct ObjectMatch {
    int64_t mine = 0, oracle = 0, equal = 0, n = 0;
    double pct = 0;
};
struct CompareObjects {
    py::list<ObjectMatch> objects;
    int64_t exact_objects = 0, total = 0;
};
namespace shim_ {
inline CompareObjects old_report(const cbl::CompareObjects& r) {
    CompareObjects out;
    for (const cbl::ObjectMatch& m : r.objects) {
        out.objects.append(ObjectMatch{int64_t{m.mine}, int64_t{m.oracle}, static_cast<int64_t>(m.equal), static_cast<int64_t>(m.length), round1(m.percent())});
    }
    out.exact_objects = static_cast<int64_t>(r.exact_objects);
    out.total = static_cast<int64_t>(r.total);
    return out;
}
}  // namespace shim_
// Match each linked object to the oracle object with the same vsize + best byte overlap, report per-object exactness.
inline CompareObjects compare_objects(const le::LEFile& orc, const le::LEFile& mine) {
    return cb::compat::guard([&] { return shim_::old_report(cbl::compare_objects(orc.module(), mine.module())); });
}

struct BuildAndCompare {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0;
    std::optional<CompareObjects> report;
};
inline BuildAndCompare build_and_compare(const le::LEFile& v, const py::str& vxdname, const py::str& root = py::str("."), const std::optional<ForceRaw>& force_raw = std::nullopt) {
    return cb::compat::guard([&] {
        const shim_::Tools tools(py::os::path::abspath(root));
        const cbl::CompareBuild r = cbl::build_and_compare(tools.env, v.module(), cb::compat::from_py(vxdname), force_raw ? shim_::new_force_raw(*force_raw) : cbl::ForceRaw{});
        BuildAndCompare out;
        out.ok = r.ok;
        if (!r.ok) {
            out.stage = cb::compat::to_py(r.stage);
            out.log = cb::compat::to_py(r.log);
            if (r.stage == "link") {
                out.asm_ = cb::compat::to_py(r.source);
                out.def_ = cb::compat::to_py(r.def);
            }
            return out;
        }
        out.vxd = cb::compat::to_py(cb::ByteView(r.vxd));
        out.vxd_len = static_cast<int64_t>(r.vxd_length);
        out.oracle_len = static_cast<int64_t>(r.original_length);
        out.link_log = cb::compat::to_py(r.link_log);
        out.report = shim_::old_report(r.report);
        return out;
    });
}

struct BuildFunctional {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0, oracle_fixups = 0, oracle_internal = 0, rebuilt_fixups = 0, rebuilt_internal = 0;
};
// Build a LOADABLE .vxd (relocations present, container NOT byte-exact).
inline BuildFunctional build_functional(const le::LEFile& v, const py::str& vxdname, const py::str& root = py::str(".")) {
    return cb::compat::guard([&] {
        const shim_::Tools tools(py::os::path::abspath(root));
        const cbl::FunctionalBuild r = cbl::build_functional(tools.env, v.module(), cb::compat::from_py(vxdname));
        BuildFunctional out;
        out.ok = r.ok;
        if (!r.ok) {
            out.stage = cb::compat::to_py(r.stage);
            out.log = cb::compat::to_py(r.log);
            out.asm_ = cb::compat::to_py(r.source);
            if (r.stage == "link") out.def_ = cb::compat::to_py(r.def);
            return out;
        }
        out.vxd = cb::compat::to_py(cb::ByteView(r.vxd));
        out.vxd_len = static_cast<int64_t>(r.vxd_length);
        out.oracle_len = static_cast<int64_t>(r.original_length);
        out.oracle_fixups = static_cast<int64_t>(r.original_fixups);
        out.oracle_internal = static_cast<int64_t>(r.original_internal);
        out.rebuilt_fixups = static_cast<int64_t>(r.rebuilt_fixups);
        out.rebuilt_internal = static_cast<int64_t>(r.rebuilt_internal);
        out.link_log = cb::compat::to_py(r.link_log);
        return out;
    });
}

// Assemble idiomatic DDK-macro source to COFF with the NATIVE ML.EXE. -> (obj or None, log, work dir)
inline std::tuple<std::optional<py::bytes>, py::str, py::str> _assemble_idiomatic(const py::str& asm_text, const py::str& root) {
    return cb::compat::guard([&] {
        const auto tools = masm::shim_::toolchain(root);
        const auto made = cb::source::vxd::masm::assemble_macro_batch(tools, std::vector<std::string>{cb::compat::from_py(asm_text)});
        const py::str work = shim_::keep_work_folder("cb_idiom_");
        return std::make_tuple(made.front().object ? std::optional<py::bytes>(cb::compat::to_py(cb::ByteView(*made.front().object))) : std::nullopt, cb::compat::to_py(made.front().log), work);
    });
}

// one object of the overlay report
struct OverlayRow {
    int64_t obj = 0;
    std::optional<int64_t> asm_obj;
    double score = 0;
    int64_t cmp = 0, expected = 0, asm_len = 0, macro = 0;
    bool ok = false;
};
namespace shim_ {
inline OverlayRow old_row(const cbl::OverlayRow& r) {
    OverlayRow out;
    out.obj = r.object;
    if (r.assembled_object) out.asm_obj = int64_t{*r.assembled_object};
    out.score = std::round(r.score * 10000.0) / 10000.0;
    out.cmp = static_cast<int64_t>(r.compared);
    out.expected = static_cast<int64_t>(r.expected);
    out.asm_len = static_cast<int64_t>(r.assembled_length);
    out.macro = static_cast<int64_t>(r.macro);
    out.ok = r.ok;
    return out;
}
}  // namespace shim_

// Overlay the assembled-from-source CODE onto the recovered object bytes `D.code` and emit through the byte-exact LE writer. Raises ValueError when it
// cannot match every object confidently. D.code is changed.
inline std::tuple<py::bytes, py::list<OverlayRow>> _source_code_into_container(VxDDisasm& D, const le::LEFile& v, const py::bytes& fs_bytes, double min_score = 0.5) {
    return cb::compat::guard([&] {
        auto code = masm::shim_::emission_of(D);
        const cbl::Overlay overlay = cbl::overlay_code(code, v.module(), cb::compat::from_py(fs_bytes), min_score);
        for (const le::Object& o : v.objects) D.code.set(o.idx, cb::compat::to_py(code.code(static_cast<uint32_t>(o.idx))));
        py::list<OverlayRow> rows;
        for (const cbl::OverlayRow& r : overlay.rows) rows.append(shim_::old_row(r));
        return std::make_tuple(cb::compat::to_py(cb::ByteView(overlay.vxd)), rows);
    });
}

struct LoadableIdiomatic {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0, objects = 0, rebuilt_internal = 0, oracle_internal = 0;
    py::list<OverlayRow> overlay;
    double min_overlay_score = 0;
};
// Build a LOADABLE LE .vxd from the *idiomatic DDK-macro* source and the real toolchain.
inline LoadableIdiomatic build_loadable_idiomatic(const le::LEFile& v, const std::optional<py::str>& name = std::nullopt, const py::str& root = py::str(".")) {
    return cb::compat::guard([&] {
        const shim_::Tools tools(py::os::path::abspath(root));
        const cbl::LoadableBuild r = cbl::build_loadable_idiomatic(tools.env, v.module(), name ? std::optional<std::string>(name->utf8()) : std::nullopt);
        LoadableIdiomatic out;
        out.ok = r.ok;
        out.asm_ = cb::compat::to_py(r.source);
        if (!r.ok) {
            out.stage = cb::compat::to_py(r.stage);
            out.log = cb::compat::to_py(r.log);
            if (r.stage == "link") out.def_ = cb::compat::to_py(r.def);
            if (r.stage == "overlay") out.link_log = cb::compat::to_py(r.link_log);
            return out;
        }
        out.vxd = cb::compat::to_py(cb::ByteView(r.vxd));
        out.vxd_len = static_cast<int64_t>(r.vxd_length);
        out.oracle_len = static_cast<int64_t>(r.original_length);
        out.objects = static_cast<int64_t>(r.objects);
        out.link_log = cb::compat::to_py(r.link_log);
        for (const cbl::OverlayRow& row : r.overlay) out.overlay.append(shim_::old_row(row));
        out.min_overlay_score = r.min_overlay_score;
        out.rebuilt_internal = static_cast<int64_t>(r.rebuilt_internal);
        out.oracle_internal = static_cast<int64_t>(r.original_internal);
        return out;
    });
}

}  // namespace codebreaker::source::vxd::link
