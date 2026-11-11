// codebreaker/source/vxd/coalesce.hpp -- typed C++ of codebreaker.source.vxd.coalesce
//
// coalesce.py -- finalize a from-source VxD's LE file so VMM's real-mode loader accepts it like retail.
//
// Two post-link fixes wlink/our link lane do not get right on their own, both discovered by booting the
// reconstructed stack and reading BOOTLOG:
//
// 1. **Fixup source-list coalescing.** wlink emits one fixup record per source (src type 0x07/0x08).
//    Retail LINK coalesces every source that shares one target into a single SOURCE-LIST record
//    (src bit 0x20, type 0x27/0x28): [src|0x20][tflags][count][target...][additive?][src-offset list].
//    Our individual records make each page's fixup table ~1.5-2x retail's; matching retail's compact
//    form keeps the loader's per-page handling identical. The (obj,off)->target map is unchanged
//    (le.fixups reads both forms the same), so relocation is byte-for-byte identical.
//
// 2. **LE entry point (real-mode-init EIP).** wlink leaves the module entry point (eipobj@0x18,
//    eip@0x1c) at 0:0. VMM calls the VxD's real-mode-init through it during load; 0:0 jumps to null and
//    the load hangs. Restore it from the oracle (a VxD with no real-mode init legitimately keeps 0:0).
//
// `finalize(vxd_bytes, v)` applies both (v is the oracle LEFile). Coalescing only shrinks the fixup
// tables, so they fit in the original span; the data-pages offset is unchanged and the freed bytes are
// zeroed as harmless slack.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * A parsed page record (a dict) is the struct `PageRecord`: either `verbatim` (the rest of the page, kept as
//     it is) or the fields src / tflags / tbytes / add / soffs.
//   * The bytes of a file are py::bytes; the Python's `bytearray` (b) is a py::bytearray inside the functions.  An
//     index past the end raises IndexError, a struct read past it the runtime's StructError, as in Python.
//   * `finalize` catches the exceptions of the runtime's py::Exception family, as the Python's `except Exception`.
//   * The Python's `if __name__ == "__main__"` block (the command line) is not ported.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::coalesce {

namespace le = codebreaker::formats::le;

// one fix-up record of a page, as _parse_page_records takes it apart
struct PageRecord {
    std::optional<py::bytes> verbatim;     // set: the rest of the page, unparsed (a target kind this code does not know)
    int64_t src = 0, tflags = 0;
    py::bytes tbytes, add;
    py::list<int64_t> soffs;
};

// ([records], where the parse ended)
std::tuple<py::list<PageRecord>, int64_t> _parse_page_records(const py::bytes& d, int64_t p, int64_t end);

py::bytes _emit_record(int64_t src_base, int64_t tflags, const py::bytes& tbytes, const py::bytes& add, const py::list<int64_t>& soffs);

// Rewrite individual fixup records into source-list records (match retail). Returns new bytes.
py::bytes coalesce(const py::bytes& vxd_bytes);

// Copy the oracle's LE entry point (eipobj@0x18, eip@0x1c) -- the real-mode-init EIP -- which
// wlink leaves 0:0, making VMM call a null real-mode-init and hang the load.
py::bytes restore_entry_point(const py::bytes& vxd_bytes, const le::LEFile& v);

// Splice the oracle's loader-section metadata -- the resource table, resident-name table and entry
// (export) table -- verbatim into our LE, and slide the (coalesced) fixup tables to follow.
//
// LINK 2.60 / wlink produce these tables differently from retail's LINK386: the module name is space-
// padded to 8 (retail "DOSMGR" 6 vs ours "DOSMGR  "), the DDB is published as an extra *named* resident
// export (DOSMGR_DDB) retail does not carry, and the resource-table pointer (hdr 0x50) is non-zero.
// load_relocated() only compares object DATA, so these pass the relocation gate yet differ byte-for-byte
// from retail; VMM reads this metadata during init and faults on the mismatch (dosmgr: diff==0 yet a
// Device_Init triple-fault). The tables are pure link output, not source -- recovering them verbatim is
// the same category as restore_entry_point (copying the oracle's real-mode-init EIP).
//
// The object table + page map precede this region and are byte-identical for a diff==0 module, so the
// oracle's resident-name offset (hdr 0x58) equals ours; we guard on that and bail otherwise. The entry
// table points the DDB at an obj:rva our object bytes reproduce exactly, so the oracle's table is valid
// as-is. Handles both directions: when our region is longer it shrinks (padding the freed bytes before
// the unchanged data-pages offset); when shorter (the LINK-2.60 compile lane) it grows, absorbing the
// extra bytes by trimming the slack padding between the fixup records and the data pages.
py::bytes restore_metadata(const py::bytes& vxd_bytes, const le::LEFile& v);

// Append the oracle's non-resident name table + Windows version resource (byte-exact) to a linked VxD
// that lacks them, re-pointing hdr 0x88/0x8c (non-res names) and 0xb8/0xbc (winres). The DDK path builds
// the resource with RC + ADRC2VXD under DOSBox; when DOSBox is unavailable this copies the resource bytes
// directly -- metadata, like the DDB -- so the module still carries its version resource (VMM/Setup read
// it, and a VxD missing it faulted earlier). Both offsets are FILE-ABSOLUTE in LE, so they point at the
// appended bytes; object data + fixups are untouched (appended strictly past end-of-data).
py::bytes restore_version_resource(const py::bytes& vxd_bytes, const le::LEFile& v);

// Apply the LE-format fixes (coalesce fixups, restore loader metadata, version resource, entry point).
// Each step is best-effort so a module that defeats one still gets the others.
py::bytes finalize(const py::bytes& vxd_bytes, const le::LEFile& v);

}  // namespace codebreaker::source::vxd::coalesce
