// codebreaker/exact/linkemu.hpp -- compat shim for cb/exact/linkemu.hpp (docs/CPP_DESIGN.md 6.3). Conversion and
// forwarding only; deleted with its last old-layer consumer.
//
//     link24(objects, symbols, log=None) -> the run file's bytes
//
// objects: [(name, object bytes, odd)] - odd: the code ends on an odd byte. Either an OMF module or a Version 7 a.out.
// The Python's `load` (a module's pieces, publics, fixes) is internal to the new linker and has no old-layer form.
// A failure of the link is py::SystemExit(text), as it was.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/aout.hpp"
#include "cb/exact/linkemu.hpp"
#include "codebreaker/exact/aout.hpp"
#include "codebreaker/formats/omf.hpp"
#include "py.hpp"

namespace codebreaker::exact::linkemu {

// W = lambda b, k: struct.unpack_from("<H", b, k)[0]
inline int64_t W(const py::bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<H", b, k); }

// Where a module's piece of a segment went (the map link24 leaves).
struct Piece {
    py::str seg;
    int64_t length = 0;
    std::optional<int64_t> at;
};
using PieceRef = py::ref<Piece>;

struct Module {
    py::str name;
    py::dict<py::str, PieceRef> pieces;
};

using Object = std::tuple<py::str, py::bytes, bool>;                 // (name, object bytes, odd)
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;     // {name: (kind, address)}

// (where each module's pieces went: a map)
inline py::list<Module> link24_mods;
inline int64_t link24_dgroup = 0;

namespace detail {

// An old omf.read result as the object record of the new tree (the OMF reader is refactored later).
inline cb::exact::ObjectFile object_of(const formats::omf::Obj& o) {
    using namespace cb::exact;
    using cb::compat::to_py;
    ObjectFile out;
    out.kind = ObjectKind::Omf;
    for (const auto& kv : o.seg.items()) out.seg.insert_or_assign(std::get<0>(kv).utf8(), cb::compat::from_py(std::get<1>(kv)));
    for (const auto& s : o.segs)
        out.segs.push_back({std::get<0>(s).utf8(), std::get<1>(s).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<2>(s)),
                            cb::compat::int_from_py<std::uint32_t>(std::get<3>(s))});
    for (const auto& kv : o.groups.items()) out.groups.insert_or_assign(std::get<0>(kv).utf8(), cb::compat::from_py(std::get<1>(kv)));
    for (const auto& p : o.pubs)
        out.pubs.push_back({std::get<0>(p).utf8(), std::get<1>(p) ? std::optional<std::string>(std::get<1>(p)->utf8()) : std::nullopt,
                            cb::compat::int_from_py<std::uint32_t>(std::get<2>(p))});
    out.ext = cb::compat::from_py(o.ext);
    for (const auto& kv : o.comm.items())
        out.comm.insert_or_assign(std::get<0>(kv).utf8(), CommonVar{cb::compat::int_from_py<std::uint32_t>(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))});
    for (const auto& f : o.fix) {
        const std::string kind = std::get<2>(f).utf8(), tk = std::get<0>(std::get<3>(f)).utf8();
        const auto& tv = std::get<1>(std::get<3>(f));
        out.fix.push_back({std::get<0>(f).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<1>(f)),
                           kind == "rel" ? FixKind::Rel : kind == "ptr" ? FixKind::Ptr : kind == "base" ? FixKind::Base : kind == "off" ? FixKind::Off : FixKind::Lo,
                           {tk == "ext" ? TargetKind::External : tk == "grp" ? TargetKind::Group : TargetKind::Segment,
                            std::holds_alternative<py::str>(tv) ? std::get<py::str>(tv).utf8() : (tk == "?" ? "?" : "")},
                           cb::compat::int_from_py<std::int32_t>(std::get<4>(f))});
    }
    for (const auto& kv : o.combine.items()) out.combine.insert_or_assign(std::get<0>(kv).utf8(), cb::compat::int_from_py<std::uint8_t>(std::get<1>(kv)));
    if (o.start) out.start = StartAddress{std::get<0>(*o.start).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<1>(*o.start))};
    for (const auto& kv : o.filled.items()) out.filled.insert_or_assign(std::get<0>(kv).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<1>(kv)));
    return out;
}

}  // namespace detail

inline py::bytes link24(const py::list<Object>& objects, const Symbols& symbols, const std::function<void(const py::str&)>& log = nullptr) {
    try {
        std::vector<cb::exact::linkemu::LinkInput> inputs;
        for (const Object& o : objects) {
            const py::bytes& raw = std::get<1>(o);
            cb::exact::linkemu::LinkInput in;
            in.name = std::get<0>(o).utf8();
            in.odd = std::get<2>(o);
            if (aout::is_aout(raw)) in.object = cb::exact::aout::read(cb::compat::from_py(raw)).object;
            else in.object = detail::object_of(formats::omf::read(raw));
            in.object.raw = cb::compat::from_py(raw);
            inputs.push_back(std::move(in));
        }
        cb::exact::SymbolTable table;
        for (const auto& kv : symbols.items()) {
            const std::string kind = std::get<0>(std::get<1>(kv)).utf8();
            table.insert_or_assign(std::get<0>(kv).utf8(),
                                   cb::exact::SymPlace{kind == "C" ? cb::exact::SymKind::Code : kind == "D" ? cb::exact::SymKind::Data
                                                       : kind == "S" ? cb::exact::SymKind::Segment : cb::exact::SymKind::Unknown,
                                                       std::get<1>(std::get<1>(kv))});
        }
        const cb::exact::linkemu::Log hear = log ? cb::exact::linkemu::Log([&](std::string_view s) { log(py::str(std::string(s))); }) : nullptr;
        const cb::exact::linkemu::LinkResult result = cb::exact::linkemu::link24(inputs, table, hear);
        link24_dgroup = result.dgroup;
        link24_mods = py::list<Module>();
        for (const auto& m : result.modules) {
            Module mod;
            mod.name = py::str(m.name);
            for (const auto& p : m.pieces) {
                PieceRef piece = std::make_shared<Piece>();
                piece->seg = py::str(p.segment);
                piece->length = p.length;
                piece->at = p.at;
                mod.pieces.set(py::str(p.segment), piece);
            }
            link24_mods.append(mod);
        }
        return cb::compat::to_py(cb::ByteView(result.exe));
    } catch (const cb::ToolError& e) {   // (a failure of the link itself was SystemExit(text))
        throw py::SystemExit(py::str(e.output()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

}  // namespace codebreaker::exact::linkemu
