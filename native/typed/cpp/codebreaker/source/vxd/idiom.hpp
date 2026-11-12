// codebreaker/source/vxd/idiom.hpp -- SHIM over cb/source/vxd/idiom.hpp (docs/CPP_DESIGN.md 6.3): a recovered VxD re-emitted in the idiomatic
// DDK-macro source form. The old types in (VxDDisasm, its rows, py::sets of places), the new emitter called, the old types out; the labels
// the emission renamed and the instructions it dropped are written back into the VxDDisasm, as the old port changed it. Deleted with its
// last py:: consumer (link.hpp).
//
//     VxDDisasm D(v); D.run();
//     py::str text = emit_idiomatic(D);              // D.v is the LEFile v
//     py::str loadable = emit_idiomatic(D, true);
//
// Differences from the old port that the shim cannot hide:
//   * Not offered, because nothing used them: `_branch_label`, `_fx_label`, `_dd_target_expr`, `_loadable_insn`, `_loadable_data`,
//     `_db_split_labels` (the loadable emission's own steps), the predicates of the encodings ML cannot keep (`_shift_no_count`,
//     `_ml611_unknown`, ...), the pattern and constant tables, `set_capstone_dll` (Capstone is loaded beside the executable) and
//     `verify_corpus` (the new `verify` takes the drivers, not a folder; it is not reachable from here).
//   * A fix-up with no offset (a selector) is no target where the old port raised a TypeError; a service-table slot that is an import fix-up
//     makes the table unrecoverable where it raised a KeyError.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/idiom.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::idiom {

using codebreaker::source::vxd::disassembly::Place;
using codebreaker::source::vxd::disassembly::PlaceSet;
using codebreaker::source::vxd::disassembly::Row;
using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;
namespace cbi = cb::source::vxd::idiom;
namespace cbm = cb::source::vxd::masm;

namespace shim_ {

inline cb::source::vxd::Place place_of(const Place& p) { return cb::source::vxd::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}; }
inline Place old_place(const cb::source::vxd::Place& p) { return Place(int64_t{p.object}, p.offset); }

inline cbi::PlaceSet places_of(const PlaceSet& set) {
    cbi::PlaceSet out;
    for (const Place& p : set) out.insert(place_of(p));
    return out;
}

inline cb::formats::le::Ddb new_ddb(const le::Ddb& d) {
    cb::formats::le::Ddb n;
    n.object = static_cast<uint32_t>(d.obj);
    n.rva = d.rva;
    n.version = static_cast<uint16_t>(d.version);
    n.device_id = static_cast<uint16_t>(d.device_id);
    n.major = static_cast<uint8_t>(d.major);
    n.minor = static_cast<uint8_t>(d.minor);
    n.flags = static_cast<uint16_t>(d.flags);
    n.name = d.name.utf8();
    n.init_order = static_cast<uint32_t>(d.init_order);
    n.control_proc = static_cast<uint32_t>(d.control_proc);
    return n;
}

}  // namespace shim_

// VxD device id -> the DDK .INC that defines its services.
inline py::dict<int64_t, py::str> device_inc_table() {
    py::dict<int64_t, py::str> out;
    for (uint32_t id = 0; id <= 0x0442; ++id) {
        if (const auto inc = cbi::device_include(id)) out.set(id, cb::compat::to_py(*inc));
    }
    return out;
}
inline const py::dict<int64_t, py::str> DEVICE_INC = device_inc_table();

// INC files to include: vmm.inc always, plus one per foreign device this object calls.
inline py::list<py::str> device_incs(const VxDDisasm& D, int64_t oi) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbi::device_includes(masm::shim_::emission_of(D), static_cast<uint32_t>(oi))); });
}

// `VMMcall <name>` / `VxDcall <name>` for an INT 20h service, or None if the name is unknown.
inline std::optional<py::str> svc_macro(int64_t dev, int64_t svc) {
    const auto text = cbi::service_macro(disassembly::shim_::names(), static_cast<uint32_t>(dev), static_cast<uint32_t>(svc));
    return text ? std::optional<py::str>(cb::compat::to_py(*text)) : std::nullopt;
}

// `Declare_Virtual_Device` line for a recovered DDB; with `loadable` the trailing V86/PM/reference procs too.
inline py::str ddb_macro(const VxDDisasm& D, const le::Ddb& db, bool loadable = false) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbi::declare_virtual_device(masm::shim_::emission_of(D), shim_::new_ddb(db), loadable)); });
}

// the VxD's own exported service table: the names per index, where the raw table is, and the extra labels to put at shared handlers
struct ServiceTable {
    py::list<py::str> names;                              // 'RESERVED' for a null slot
    std::tuple<int64_t, int64_t, int64_t> loc;            // (obj, off, count)
    py::dict<Place, py::list<py::str>> extra;             // handler place -> the service names to emit as extra labels
};
inline std::optional<ServiceTable> service_table(const VxDDisasm& D, const PlaceSet& emittable) {
    return cb::compat::guard([&]() -> std::optional<ServiceTable> {
        const auto found = cbi::service_table(masm::shim_::emission_of(D), shim_::places_of(emittable));
        if (!found) return std::nullopt;
        ServiceTable out;
        out.names = cb::compat::to_py(found->names);
        out.loc = std::make_tuple(int64_t{found->at.object}, found->at.offset, static_cast<int64_t>(found->count));
        for (const auto& [place, names] : found->extra) out.extra.set(shim_::old_place(place), cb::compat::to_py(names));
        return out;
    });
}

// ([(MESSAGE_NAME, handler_label)], the {(obj,off)} the macro regenerates): ([], {}) when no shape is recognised.
using Handlers = py::list<std::tuple<py::str, py::str>>;
inline std::tuple<Handlers, PlaceSet> control_dispatch(const VxDDisasm& D, const PlaceSet& emittable) {
    return cb::compat::guard([&] {
        const cbi::ControlDispatch found = cbi::control_dispatch(masm::shim_::emission_of(D), shim_::places_of(emittable));
        Handlers handlers;
        for (const auto& [message, label] : found.handlers) handlers.append(std::make_tuple(cb::compat::to_py(message), cb::compat::to_py(label)));
        PlaceSet skip;
        for (const cb::source::vxd::Place& p : found.skip) skip.insert(shim_::old_place(p));
        return std::make_tuple(handlers, skip);
    });
}

// Byte length of the on-disk VxD_Desc_Block (0x34, or the modern DDB's size).
inline int64_t _ddb_size(const VxDDisasm& D, const le::Ddb& db) {
    return cb::compat::guard([&] { return static_cast<int64_t>(cbi::ddb_size(masm::shim_::emission_of(D), shim_::new_ddb(db))); });
}

inline py::list<py::str> _all_incs(const VxDDisasm& D) {
    py::list<py::str> out;
    out.append(py::str("VMM"));
    py::set<py::str> others;
    for (const le::Object& o : D.v->objects) {
        if (!o.exec) continue;
        for (const py::str& inc : device_incs(D, o.idx)) others.add(inc);
    }
    others.discard(py::str("VMM"));
    out.extend(py::sorted(others));
    return out;
}

// Render a DATA span readably: printable-ASCII runs (>=4 bytes) as `db 'text'`, the rest as hex.
inline py::list<py::str> _db_data(const py::bytes& bs, int64_t width = 60) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbi::data_lines(cb::compat::from_py(bs), static_cast<std::size_t>(width))); });
}

// LOADABLE mode: the FLAT segment name + class for one LE object.
inline std::tuple<py::str, py::str> obj_seg(int64_t oi, int64_t flags, const std::optional<int64_t>& ddb_obj = std::nullopt) {
    const cbi::SegmentName s = cbi::object_segment(static_cast<uint32_t>(oi), static_cast<uint32_t>(flags),
                                                   ddb_obj ? std::optional<uint32_t>(static_cast<uint32_t>(*ddb_obj)) : std::nullopt);
    return std::make_tuple(cb::compat::to_py(s.name), cb::compat::to_py(s.class_name));
}

// Emit one VxD as idiomatic DDK-macro source; `D` is changed (labels named, dropped insns), as the old port changed it.
inline py::str emit_idiomatic(VxDDisasm& D, bool loadable = false) {
    return cb::compat::guard([&] {
        cbm::Emission code = masm::shim_::emission_of(D);
        const std::string text = cbi::emit(code, loadable);
        for (const auto& [at, label] : code.labels()) D.labels.set(shim_::old_place(at), cb::compat::to_py(label));
        py::list<Place> dropped;
        for (const auto& kv : D.insns.items()) {
            if (!code.instructions().contains(shim_::place_of(std::get<0>(kv)))) dropped.append(std::get<0>(kv));
        }
        for (const Place& p : dropped) D.insns.pop(p);
        return cb::compat::to_py(text);
    });
}

// Write outdir/<name>/<name>.asm -- the whole VxD as one idiomatic DDK-macro source file. Returns the path written.
inline py::str emit_idiomatic_tree(const le::LEFile& v, const py::str& name, const py::str& outdir) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::fs::to_text(cbi::write_tree(v.module(), cb::compat::from_py(name), cb::fs::from_text(cb::compat::from_py(outdir)),
                                                                  disassembly::shim_::names())));
    });
}

}  // namespace codebreaker::source::vxd::idiom
