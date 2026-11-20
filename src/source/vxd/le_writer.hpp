// source/vxd/le_writer.hpp -- synthesize an LE VxD directly from the recovered disassembly.
//
// The object bytes come from the recovery's code and the relocation table from its fix-ups, instead of assembling and linking. That
// sidesteps LINK and MASM entirely, so cross-object relocations (which MASM and COFF cannot express as a near call across sections) are
// trivial: the fix-up records are written by hand. The file itself is written by formats/le (`write_parts`, the one place LE files are
// written), told which of the original linker's layout choices to keep.
//
// The file round-trips through formats/le to the same objects, object bytes and fix-ups, AND is accepted by the real Win95 VMM loader: a
// VKD.VXD synthesized this way boots Windows 95 to the logon. It reproduces the original byte for byte except for the order LINK386
// happened to write the fix-up records in (a linker artifact that does not affect loading): the recovered code, fix-ups, object and module
// names, device id and version resource all match exactly.
//
//     const Emission code(Disassembly::run(driver, names), names);
//     Bytes file = build_le(code, driver, "VKD");
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "formats/le.hpp"
#include "source/vxd/masm.hpp"

namespace cb::source::vxd::le_writer {

// The LE fix-up source type of the field at (object, offset): 0x08 (a 32-bit self-relative field) when it is the rel32 of a near branch,
// E8/E9 call/jmp (5 bytes) or 0F 8x jcc (6 bytes), else 0x07 (a 32-bit offset). A decoded branch that ends at the field is authoritative;
// when NO decoded instruction ends at offset+4 (the field is not a clean instruction tail: an undecoded or misdecoded branch, including
// code past the object's declared size) the raw opcode byte is trusted. `ends` are the offsets where a decoded instruction ends, `code` the
// object's whole bytes. `told` is the source type a recovered fix-up carries (the internal references do), and it wins: the shape of the
// code cannot always tell (VMM has `cmp eax, imm32` fields relocated self-relative).
std::uint8_t fixup_source_type(const masm::Emission& code, std::uint32_t object, std::int64_t offset, const std::set<std::int64_t>& ends,
                               ByteView object_bytes, std::optional<std::uint8_t> told);

// The original's data pages in page-map order, each found by its NUMBER in the page map (a linker may number them out of order: the loader
// fetches a page, and its fix-up row, by that number), with the highest number. A page's place runs a whole page; the highest-numbered page
// holds only last_page_bytes of it. `first`: where the data pages start (default: as the header says).
struct OraclePages {
    std::vector<std::pair<std::uint32_t, Bytes>> pages;   // (number, the page's bytes as the file has them)
    std::uint32_t top = 0;
};
OraclePages oracle_pages(const formats::le::Module& driver, std::optional<std::uint32_t> first = std::nullopt);

// The fix-up records of every page: {page -> record bytes}. Sources on a page that share one target (same source type and target) are
// combined into one source-list record (source | 0x20, a count, the target, the ascending source offsets) exactly as LINK386 wrote them:
// Win95's LE loader caps the records per page, so the one-record-per-source form overflows and the module is rejected as damaged.
// Records are ordered by their first source's offset; a list of more than 255 sources spills into the next record; a 4-byte source that
// crosses into the next page is also listed there with a negative offset. A selector-only fix-up has no 32-bit field and is left out.
std::map<std::uint32_t, std::vector<Bytes>> emit_fixups(const masm::Emission& code, const formats::le::Module& driver);

// A synthesized LE VxD for the recovery of `driver` (the original): its object pages from the recovery's code, its fix-up records from its
// fix-ups, in the original linker's layout, read from the original. The header goes at 0x80, behind the original's DOS stub, and the layout
// is read from the original's header at 0x80 too: for the few files whose header sits elsewhere, those are the bytes their rebuilt drivers
// have always had. FormatError when the original is not laid out as the writer needs.
Bytes build_le(const masm::Emission& code, const formats::le::Module& driver, std::string_view name);

// A single-module W3 container (Win95's vmm32.vxd) rebuilt from the recovered module. Unlike a standalone driver, the embedded LE keeps its
// container-absolute layout at driver.lfa(); its object data comes from the recovery's code and its fix-up records from its fix-ups, while the
// DOS stub and real-mode loader, the W3 header and entry, the LE header fields, the object and name tables and the version resource are the
// container's own. The member is written like build_le's, in front of it the container's own program and directory; the directory is then
// packed again with the member's size as written. The recovery gives more fix-up records than LINK386 wrote (Win95's VMM: 3824 against
// 3804, 98 bytes more; Win98's: 48 more), so the fix-up section, the import-table offsets and the member's size grow with them, and where
// the tables then run into the data pages (Win95's VMM) the pages move to the next 0x200, the non-resident names and resource with them.
// FormatError when `driver` is not a member of a container.
Bytes build_w3(const masm::Emission& code, const formats::le::Module& driver);

}  // namespace cb::source::vxd::le_writer
