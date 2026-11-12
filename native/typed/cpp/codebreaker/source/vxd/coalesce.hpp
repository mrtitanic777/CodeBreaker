// codebreaker/source/vxd/coalesce.hpp -- SHIM over cb/source/vxd/coalesce.hpp (docs/CPP_DESIGN.md 6.3): finalize a from-source VxD's LE file
// so VMM's real-mode loader accepts it like retail (coalesce the fix-ups, restore the loader metadata, the version resource and the entry
// point from the original). The old types in (py::bytes, the old LEFile), the new functions called. Deleted with its last py:: consumer.
//
// Differences from the old port that the shim cannot hide: `_parse_page_records` and `_emit_record` (the steps of `coalesce`) are not
// offered, nothing used them; the new code reads a page's records within the page, where the old read on into the file; `finalize` keeps no
// record of the steps it skipped here (cb::source::vxd::coalesce::finalize has it).
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/coalesce.hpp"
#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::coalesce {

namespace le = codebreaker::formats::le;
namespace cbc = cb::source::vxd::coalesce;

// Rewrite individual fixup records into source-list records (match retail). Returns new bytes.
inline py::bytes coalesce(const py::bytes& vxd_bytes) {
    return cb::compat::guard([&] {
        const cb::Bytes done = cbc::coalesce(cb::compat::from_py(vxd_bytes));
        return cb::compat::to_py(cb::ByteView(done));
    });
}

// Copy the oracle's LE entry point (eipobj@0x18, eip@0x1c) -- the real-mode-init EIP.
inline py::bytes restore_entry_point(const py::bytes& vxd_bytes, const le::LEFile& v) {
    return cb::compat::guard([&] {
        const cb::Bytes done = cbc::restore_entry_point(cb::compat::from_py(vxd_bytes), v.module());
        return cb::compat::to_py(cb::ByteView(done));
    });
}

// Splice the oracle's loader-section metadata into our LE, and slide the (coalesced) fixup tables to follow.
inline py::bytes restore_metadata(const py::bytes& vxd_bytes, const le::LEFile& v) {
    return cb::compat::guard([&] {
        const cb::Bytes done = cbc::restore_metadata(cb::compat::from_py(vxd_bytes), v.module());
        return cb::compat::to_py(cb::ByteView(done));
    });
}

// Append the oracle's non-resident name table + Windows version resource to a linked VxD that lacks them.
inline py::bytes restore_version_resource(const py::bytes& vxd_bytes, const le::LEFile& v) {
    return cb::compat::guard([&] {
        const cb::Bytes done = cbc::restore_version_resource(cb::compat::from_py(vxd_bytes), v.module());
        return cb::compat::to_py(cb::ByteView(done));
    });
}

// Apply the LE-format fixes (coalesce fixups, restore loader metadata, version resource, entry point). Each step is best-effort.
inline py::bytes finalize(const py::bytes& vxd_bytes, const le::LEFile& v) {
    return cb::compat::guard([&] {
        const cbc::Finalized done = cbc::finalize(cb::compat::from_py(vxd_bytes), v.module());
        return cb::compat::to_py(cb::ByteView(done.bytes));
    });
}

}  // namespace codebreaker::source::vxd::coalesce
