// codebreaker/source/vxd/le_writer.hpp -- typed C++ of codebreaker.source.vxd.le_writer
//
// le_writer.py -- synthesize an LE VxD directly from the recovered disassembly -- object bytes from D.code, the
// relocation table from D.fx -- instead of assembling + linking. This sidesteps LINK/MASM entirely,
// so cross-object relocations (which MASM/COFF cannot express as a near call across sections) are
// trivial: the fixup records are emitted by hand. The file itself is written by formats/le.py's
// write_image, the one place LE files are written, told which of the original linker's layout
// choices to keep.
//
// The emitted file round-trips through le.py to the same objects, object bytes, and fixups, AND is
// accepted by the real Win95 VMM loader -- a VKD.VXD synthesized this way boots Windows 95 to the
// logon. It reproduces the oracle byte-for-byte except for the order LINK386 happened to emit the
// fixup records in (a linker artifact that does not affect loading): the recovered code, fixups,
// object/module names, device id, and version resource all match exactly.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `D` is the VxDDisasm of disassembly.hpp (its `fx`, `insns`, `code`, `ddb` are the members the Python read);
//     `v` is the oracle le::LEFile.
//   * `ends` (the per-object set of instruction-end offsets) is a py::set<int64_t>.
//   * _emit_fixups answers {page: [record bytes]}, the py::dict le::Image::fixups holds.
//   * The dicts handed to le.write_image are le::Image (see formats/le.hpp); a key the Python leaves out is a
//     std::optional left empty.
//   * le.container() answering None makes build_w3 raise the TypeError the Python raised when it subscripted None.
#pragma once

#include <cstdint>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::le_writer {

namespace le = codebreaker::formats::le;
using codebreaker::source::vxd::disassembly::VxDDisasm;

// LE fixup source type for the field at (oi, off): 0x08 (32-bit self-relative) when the 4-byte
// field is the rel32 of a near branch -- E8/E9 call/jmp (5 bytes) or 0F 8x jcc (6 bytes) -- else
// 0x07 (a 32-bit offset). A decoded branch ending at this field is authoritative; otherwise, when
// NO decoded instruction ends at off+4 (the field is not a clean instruction tail -- an undecoded
// or mis-decoded branch, including code past the object's declared vsize), trust the raw opcode
// byte. `ends` is the set of instruction-end offsets; `code` the object's full (untruncated) bytes.
//
// A recovered fix-up that says its own source type (D.fx's internal references do) keeps it: the
// shape of the code cannot always tell -- VMM has `cmp eax, imm32` fields relocated self-relative.
int64_t _src_type(const VxDDisasm& D, int64_t oi, int64_t off, const py::set<int64_t>& ends, const py::bytes& code);

// The oracle's data pages in page-map order, each found by its number in the page map (a
// linker may number them out of order: the loader fetches a page, and its fix-up row, by that
// number): ([(number, the page's place in the file as bytes)], the highest number). A place runs a
// whole page; the highest-numbered page holds only last_page_bytes of it. `first`: where the
// data pages start (default: as the header says).
std::tuple<py::list<std::tuple<int64_t, py::bytes>>, int64_t> _oracle_pages(const le::LEFile& v, std::optional<int64_t> first = std::nullopt);

// LE fixup page table + record table from D.fx. Sources on a page that share one target (same
// source type + target) are combined into a single source-list record (src|0x20, count, target,
// then the ascending source-offset list) exactly as LINK386 emitted them -- Win95's LE loader
// caps the fixup records per page, so the one-record-per-source form overflows and the module is
// rejected as "damaged". Records are ordered by ascending first-source offset. Returns {page:
// [record bytes]} by the page's place in the page map (le.write_image puts each page's records in
// the row of the page's number, as the loader reads them), encoded and laid into the fix-up tables
// by formats/le.py as every LE's are.
py::dict<int64_t, py::list<py::bytes>> _emit_fixups(const VxDDisasm& D, const le::LEFile& v);

// Return the bytes of a synthesized LE VxD for the recovered disassembly D of oracle v: its
// object pages from D.code and its fix-up records from D.fx, written by le.write_image in the
// original linker's (LINK386's) layout, whose choices are read from the oracle. The header goes
// at 0x80, behind the oracle's DOS stub, and that layout is read from the oracle's header at
// 0x80 too -- for the few files whose header sits elsewhere, those are the bytes their rebuilt
// drivers have always had.
py::bytes build_le(const VxDDisasm& D, const le::LEFile& v, const py::str& vxdname);

// Reconstruct a single-module W3 container (Win95's vmm32.vxd) from the recovered module. Unlike
// a standalone .vxd, the embedded LE keeps its container-absolute layout at v.lfa; its OBJECT DATA
// comes from D.code and its FIXUP records from D.fx, while the DOS stub + real-mode loader, the W3
// header + entry, the LE header fields, the object/name tables and the version resource are the
// container's own metadata.
//
// The member is written by le.write_image, like build_le's drivers, in front of it the
// container's own program and directory; the directory is then packed again with the member's
// size as written. D.fx gives more fix-up records than LINK386 wrote (Win95's VMM: 3824 against
// 3804, 98 bytes more; Win98's: 48 bytes more), so the fix-up section, the import-table offsets
// and the member's size grow with them, and where the tables then run into the data pages (Win95's
// VMM) the pages move to the next 0x200, the non-resident names and resource with them.
py::bytes build_w3(const VxDDisasm& D, const le::LEFile& v, const py::str& name);

}  // namespace codebreaker::source::vxd::le_writer
