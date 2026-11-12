// cb/source/vxd/coalesce.hpp -- finalize a from-source VxD's LE file so VMM's real-mode loader accepts it like retail.
//
// Two fixes the linker (wlink or our link lane) does not get right on its own, both found by booting the rebuilt stack
// and reading BOOTLOG:
//
//  1. Fix-up source-list coalescing. wlink writes one fix-up record per source (source type 0x07/0x08). Retail LINK folds
//     every source that shares one target into a single SOURCE-LIST record (source bit 0x20, type 0x27/0x28):
//     [src|0x20][flags][count][target ...][addend?][source offsets ...]. Our single records make each page's fix-up table
//     1.5 to 2 times retail's; the compact form keeps the loader's per-page handling the same. What every (object, offset)
//     points at does not change, so relocation is identical.
//  2. The entry point (the real-mode-init EIP, header 0x18/0x1c). wlink leaves it 0:0, VMM calls the VxD's real-mode
//     init through it, and 0:0 is a jump to null: the load hangs. It is restored from the original (a VxD with no
//     real-mode init legitimately keeps 0:0).
//
// Two more restore what a linked driver lacks of the original's loader metadata (the resource, resident-name and entry
// tables, the version resource). `finalize` applies all four, each as far as the module allows. Coalescing only shrinks the
// fix-up tables, so they fit in the original span; the data-pages offset stays and the freed bytes are zeroed as slack.
//
//     codebreaker vxd finalize BUILT.vxd ORIGINAL.vxd OUT.vxd
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/le.hpp"

namespace cb::source::vxd::coalesce {

// ---- fix-up records of one page ---------------------------------------------------------------------------------------------

// One fix-up record of a page, taken apart. The target is kept as the bytes the page stores (they differ in width with the
// flags), so that two records with the same target are the same bytes.
struct PageRecord {
    std::uint8_t source = 0;               // the source type, the source-list bit cleared
    std::uint8_t flags = 0;                // the target flags
    Bytes target;                          // the target field as stored
    Bytes addend;                          // the additive field (two or four bytes); empty without one
    std::vector<std::int16_t> sources;     // where in the page the fields to patch are
};

// A page's records. A record of a kind this code does not take apart (an import by name, an entry) ends the parse: it and
// everything after it stay as they are, in `rest`.
struct Page {
    std::vector<PageRecord> records;
    Bytes rest;
};

// The records of one page: `page` is exactly the page's bytes of the record table. TruncatedError when a record does not
// fit in them.
Page parse_page(ByteView page);

// A record in the form retail uses: one source as a plain record, several as a source list. A list holds at most 255
// sources, so more are written as several records.
Bytes emit_record(const PageRecord& record);

// ---- the fixes -----------------------------------------------------------------------------------------------------------------

// Rewrite the fix-up records into source-list records, as retail has them. FormatError when the tables do not lie in the file,
// UnsupportedError when the coalesced records would not fit in the span the originals had.
Bytes coalesce(ByteView vxd);

// Copy the original's entry point (EIP object and EIP, header 0x18 and 0x1c).
Bytes restore_entry_point(ByteView vxd, const formats::le::Module& original);

// Splice the original's loader metadata -- the resource table, the resident names and the entry (export) table -- into the
// driver and slide the (coalesced) fix-up tables to follow.
//
// LINK 2.60 and wlink write these tables differently from retail's LINK386: the module name padded to eight ("DOSMGR  " for
// retail's "DOSMGR"), the DDB published as an extra *named* export (DOSMGR_DDB) that retail does not carry, a non-zero
// resource-table pointer. The relocation gate compares object data only and passes them, but VMM reads this metadata during
// init and faults on the difference (DOSMGR: no byte of object data differs, yet Device_Init triple-faults). The tables are
// link output, not source: copying them is the same kind of thing as restoring the entry point.
//
// The object table and the page map come before this region and are the same in a driver whose objects match, so the
// original's resident-name offset equals ours; the driver is returned unchanged when it does not, and when the tables are
// not where they must be. When our region is longer than the original's it shrinks (the freed bytes are zeroed before the
// data pages, whose offset does not move); when shorter, it grows by taking the slack between the fix-up records and the data pages.
Bytes restore_metadata(ByteView vxd, const formats::le::Module& original);

// Append the original's non-resident name table and Windows version resource to a driver that lacks them, and point the
// header at them (0x88/0x8c and 0xb8/0xbc, file offsets in LE). The DDK builds the resource with RC and ADRC2VXD under
// DOSBox; without it, copying the bytes gives the module the version resource VMM and Setup read.
Bytes restore_version_resource(ByteView vxd, const formats::le::Module& original);

// What `finalize` did.
struct Finalized {
    Bytes bytes;
    std::vector<std::string> skipped;   // "step: why" for each fix the module defeated
};

// All four fixes, each on its own: a module that defeats one still gets the others.
Finalized finalize(ByteView vxd, const formats::le::Module& original);

// `BUILT ORIGINAL OUT`: finalize the driver BUILT against the ORIGINAL and write OUT. Returns the exit code.
int main(std::span<const std::string> args);

}  // namespace cb::source::vxd::coalesce
