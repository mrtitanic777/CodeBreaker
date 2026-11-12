// cb/source/ne_asm/fixups.hpp -- an annotated 16-bit disassembly of an NE segment: every place the loader patches is
// named with the real target.
//
// A segment's code is disassembled with objdump (i8086) and the NE relocation chains of the segment are walked to label
// every fix-up site: `module.ordinal` for an import, `seg:entry` for a reference inside the module. That turns the
// placeholder `call 0x0:0xffff` into the readable `call FAR KERNEL.91`.
//
//     codebreaker ne-asm fixups FILE [SEGMENT] [--start 0xNN] [--count N]      one segment, annotated
//     codebreaker ne-asm fixups FILE --entry ORDINAL                           one exported entry
//
// The disassembler is an external program (objdump of binutils, found through PATH); the text it prints is taken
// as an `Objdump` so that the callers and the tests can give their own.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ne.hpp"

namespace cb::source::ne_asm::fixups {

// ---- opening a module -------------------------------------------------------------------------------------------------

// A normal MZ/NE executable, or a raw embedded NE module (the bytes start with "NE"). FormatError for anything else.
formats::ne::Module open_module(Bytes data);
formats::ne::Module open_module(const std::filesystem::path& file);

// ---- the fix-up map ---------------------------------------------------------------------------------------------------

// A reference to another place of the module: "seg4:0x0010".
struct SegmentRef {
    std::uint8_t segment = 0;
    std::uint16_t offset = 0;
};

// What a fix-up site points at, as the annotation says it.
struct Target {
    // The listing's name for it: "KERNEL.91" (an import by ordinal), "USER.GetDC" (by name), "seg4:0x0010" (inside the
    // module), "GDI.273" (an entry of the module's own entry table, a call into a moveable segment: segment 0xFF is not
    // a segment), "osfixup#3" (an emulator fix-up). A module the table does not hold is "mod<N>".
    std::string label;
    // Set for the references inside the module that name a fixed segment; the others are not places of a segment.
    std::optional<SegmentRef> segment;
    friend bool operator==(const Target&, const Target&) = default;
};

// Offset in the segment -> what the site there points at. Sorted by offset.
using FixupMap = std::map<std::uint32_t, Target>;

// The fix-up sites of `segment` (one of module.segments()), by walking each relocation chain. A chain stops at its
// end (0xFFFF), at a site whose link would lie outside the segment, after an additive record (which holds an addend,
// not a link), at a link to itself, and after kMaxChain sites.
// `modules` is module.module_refs() (it is a parameter because callers hold it). FormatError when the segment lies
// outside the file.
FixupMap build_fixup_map(const formats::ne::Module& module, const formats::ne::Segment& segment, const std::vector<std::string>& modules);

inline constexpr std::size_t kMaxChain = 10000;

// ---- objdump ----------------------------------------------------------------------------------------------------------

// What objdump prints for `code` read as 16-bit x86, the first byte at address `base`.
using Objdump = std::function<std::string(ByteView code, std::uint32_t base)>;

// Runs objdump (`-D -b binary -mi386 -Maddr16,data16,intel --adjust-vma=BASE`) on the bytes. ToolError when objdump
// cannot be started.
std::string run_objdump(ByteView code, std::uint32_t base);

// One line of objdump's output.
struct ListingLine {
    std::optional<std::uint32_t> offset;   // the instruction's address; absent for a header or a blank line
    std::string text;                      // the line as printed
    std::string instruction;               // an instruction line: the assembly (the last tab-separated field, trimmed)
};
std::vector<ListingLine> parse_objdump(std::string_view output);

// ---- the annotated listing --------------------------------------------------------------------------------------------

struct ListingRange {
    std::size_t segment = 1;                // from 1
    std::uint32_t start = 0;                // the first byte of the segment to list
    std::optional<std::uint32_t> count;     // how many bytes; the rest of the segment when absent
};

// The listing of a segment: a header, then objdump's lines, each instruction that has a fix-up site among its bytes
// followed by `   ; -> TARGET`. UsageError when the segment does not exist.
std::string annotated_listing(const formats::ne::Module& module, std::string_view file_name, const ListingRange& range,
                              const Objdump& objdump = run_objdump);

// The listing of the entry with this ordinal (its first 128 bytes); "ordinal N not exported" when there is none.
std::string entry_listing(const formats::ne::Module& module, std::string_view file_name, std::uint32_t ordinal,
                          const Objdump& objdump = run_objdump);

// ---- the command ------------------------------------------------------------------------------------------------------

// `FILE [SEGMENT] [--start N] [--count N]` | `FILE --entry ORDINAL`; the listing goes to stdout. Returns the exit code.
int main(std::span<const std::string> args, const Objdump& objdump = run_objdump);

}  // namespace cb::source::ne_asm::fixups
