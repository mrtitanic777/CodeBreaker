// source/vxd/from_source.hpp -- a loadable Windows 95 VxD from the recovered source, with no byte transplanted.
//
// The path `link::build_loadable_idiomatic` could not take: the recovered driver as idiomatic DDK-macro assembly with each object in its REAL
// named segment, assembled with the DDK's own ML, linked with the DDK's own LINK 2.60. The four requirements, each pinned by a real boot:
//
//   1. SEGMENT MODEL. The DDB object's code goes in `_LTEXT` (VxD_LOCKED_CODE_SEG) with the control dispatch; the service table and
//      `Declare_Virtual_Device` land in `_LDATA` through the macros; and the DDB object's tail after the DDB is DATA, written into the
//      object after the DDB rather than left as code where a disassembler's over-read would put it ahead of the service table and push the
//      DDB off its true offset. The order on the object is [dispatch][code][service][DDB][data].
//   2. FLAT MODEL. Every other segment is declared `<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'`. `_FLAT` makes it part of the flat
//      group; `USE32` does not, and every cross-segment near call then fails to assemble (A2107). This is what the DDK's MakeCodeSeg expands to.
//   3. THE RIGHT LINKER. The device id (LE 0xC0) and the version resource are loader-required. LINK 2.50 leaves 0xC0 blank and permutes the
//      page map; the DDK shipped LINK 2.60 to fix VxD linking: it stamps 0xC0 from the exported <NAME>_DDB and writes an identity map.
//   4. IDENTITY PAGE MAP AND OBJECT ORDER. Every segment is PRELOAD NONDISCARDABLE in the .DEF: that keeps the linker from grouping the
//      discardable pages at the end of the file (which would permute the page map the loader reads by logical slot) and keeps the objects in
//      .DEF order, the original's order.
//
// What ships is the byte-exact LE writer's container over the same recovered code (LINK 2.60's page layout and fix-up encoding differ from the
// original linker's, and VMM's initialisation does not tolerate that layout, even with byte-exact code); the readable source is proven to
// compile and link, and `compile_ok` / `link_ok` say whether it did.
//
//     const from_source::Built built = from_source::build(env, driver);
//     fs::write_file("vkd.vxd", built.vxd);
//
//     codebreaker vxd from-source FILE [OUT]
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "formats/le.hpp"
#include "source/vxd/disassembly.hpp"
#include "source/vxd/link.hpp"
#include "source/vxd/masm.hpp"

namespace cb::source::vxd::from_source {

using link::Environment;   // the names, the assemblers, the linker (link_260 for this build)

// The 4-character segment class tag LINK /VXD stores in each object-table entry's reserved field (it writes the segment's CLASS name cut
// to four characters: 'LCODE' -> 'LCOD'), which is how a stripped binary keeps its segment structure. One per object, in object order.
std::vector<std::string> object_names(const formats::le::Module& driver);

// The MASM segment name for a 4-character tag: the tag itself, or a safe alias where it is a reserved word of MASM (dosmgr's `LOCK'
// segment would be `LOCK SEGMENT', error A2008). Only the segment NAME changes: its CLASS stays '<tag>CODE'.
std::string segment_name(std::string_view tag);

// {object: (segment name, class)} for each object but the DDB's: a unique, valid MASM name per object. A tag that is blank, NUL-filled, a
// reserved word or a duplicate of another object's gets a synthetic `SEG<n>' name (or a numbered one), so no segment is unnamed or merged by
// the linker; the class follows the name.
std::map<std::uint32_t, std::pair<std::string, std::string>> object_segments(std::span<const std::string> tags, std::uint32_t ddb_object);

// Mark the DDB object's bytes after the DDB as data: the recursive descent over-reads the locked data, tables and BSS as code (procedures
// straddling the DDB). Instructions there are dropped from `code` and a procedure label on data becomes a plain data label. -> where the
// tail starts, nullopt for a driver with no DDB or no tail.
std::optional<std::uint32_t> reclassify_ddb_tail_as_data(masm::Emission& code, const formats::le::Module& driver);

// The loadable source of `code` with each object in its real segment (see the top). Changes `code` (idiom::emit does).
std::string realseg_source(masm::Emission& code, std::span<const std::string> tags);

// The .DEF: _LTEXT and _LDATA (the DDB object) and one entry per other segment, all PRELOAD NONDISCARDABLE.
std::string realseg_def(std::string_view name, std::span<const std::string> tags, std::uint32_t ddb_object = 1);

struct Built {
    Bytes vxd;
    std::size_t vxd_length = 0, oracle_length = 0;
    std::optional<std::string> source;       // the macro source (absent for a driver with other than 4096-byte pages: written by the LE writer alone)
    std::optional<std::string> def;
    std::size_t objects = 0;
    bool page_map_identity = false;          // the file's page map is 1, 2, 3, ...
    std::optional<std::int64_t> ddb_offset, oracle_ddb_offset;
    std::uint16_t device_id = 0;             // LE header 0xC0
    bool device_id_ok = false;               // it is the DDB's
    std::uint32_t resource_length = 0;       // the version resource's length, LE header 0xBC
    std::size_t object1_diffs = 0;           // bytes of object 1 that differ from the original's
    std::string link_log;                    // the tail of what the assembler or linker printed
    std::optional<bool> compile_ok, link_ok; // the source assembled with ML, and linked with LINK 2.60 /VXD
};

// A loadable VxD for `driver`; `name` defaults to the DDB's, else "VXD".
Built build(const Environment& env, const formats::le::Module& driver, std::optional<std::string> name = std::nullopt);

// `FILE [OUT]`: the first driver of the file built from source; its size, page map and DDB are printed and the driver written to OUT.
// Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::from_source
