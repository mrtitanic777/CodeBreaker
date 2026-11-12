// codebreaker/source/vxd/le_writer.hpp -- SHIM over cb/source/vxd/le_writer.hpp (docs/CPP_DESIGN.md 6.3): synthesize an LE VxD from the recovered
// disassembly (the object bytes from D.code, the relocation table from D.fx). The old types in (VxDDisasm, LEFile), the new writer called, the old
// types out. Deleted with its last py:: consumer (the VxD boot tool).
//
//     py::bytes file = build_le(D, v, "VKD");
//     py::bytes vmm32 = build_w3(D, v, "VMM");
//
// Differences from the old port that the shim cannot hide:
//   * `_src_type` (an internal of the fix-up writer) is not offered.
//   * build_w3's `name` is accepted and unused, as it was; a module that is not in a container is a FormatError (the old port raised TypeError).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/le_writer.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::le_writer {

namespace le = codebreaker::formats::le;
namespace cbw = cb::source::vxd::le_writer;
using codebreaker::source::vxd::disassembly::VxDDisasm;

// The oracle's data pages in page-map order, each found by its number in the page map: ([(number, the page's bytes)], the highest number).
inline std::tuple<py::list<std::tuple<int64_t, py::bytes>>, int64_t> _oracle_pages(const le::LEFile& v, std::optional<int64_t> first = std::nullopt) {
    return cb::compat::guard([&] {
        const cbw::OraclePages found =
            cbw::oracle_pages(v.module(), first ? std::optional<uint32_t>(cb::compat::int_from_py<uint32_t>(*first)) : std::nullopt);
        py::list<std::tuple<int64_t, py::bytes>> pages;
        for (const auto& [number, bytes] : found.pages) pages.append(std::make_tuple(int64_t{number}, cb::compat::to_py(cb::ByteView(bytes))));
        return std::make_tuple(pages, int64_t{found.top});
    });
}

// LE fixup records per page, from D.fx: {page: [record bytes]}.
inline py::dict<int64_t, py::list<py::bytes>> _emit_fixups(const VxDDisasm& D, const le::LEFile& v) {
    return cb::compat::guard([&] {
        py::dict<int64_t, py::list<py::bytes>> out;
        for (const auto& [page, records] : cbw::emit_fixups(masm::shim_::emission_of(D), v.module())) {
            py::list<py::bytes> list;
            for (const cb::Bytes& r : records) list.append(cb::compat::to_py(cb::ByteView(r)));
            out.set(int64_t{page}, list);
        }
        return out;
    });
}

// The bytes of a synthesized LE VxD for the recovered disassembly D of oracle v.
inline py::bytes build_le(const VxDDisasm& D, const le::LEFile& v, const py::str& vxdname) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cbw::build_le(masm::shim_::emission_of(D), v.module(), cb::compat::from_py(vxdname)))); });
}

// A single-module W3 container (Win95's vmm32.vxd) reconstructed from the recovered module.
inline py::bytes build_w3(const VxDDisasm& D, const le::LEFile& v, const py::str&) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cbw::build_w3(masm::shim_::emission_of(D), v.module()))); });
}

}  // namespace codebreaker::source::vxd::le_writer
