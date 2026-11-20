// source/vxd/idiom.hpp -- re-emit a recovered VxD in the idiomatic DDK-macro source form.
//
// The byte-exact tree is correct but low-level: `db`/`cmp`/`jmp` and a raw `int 20h`. Real Win95 VxD source used the DDK macros
// (the Dr. Dobb's shell and VMM.INC confirm it): the DDB is a `Declare_Virtual_Device`, the control procedure is
// `Begin_Control_Dispatch` / `Control_Dispatch <Msg>, <Handler>` / `End_Control_Dispatch`, a service call is `VMMcall` or
// `VxDcall <name>`, functions are `BeginProc` / `EndProc`, segments are `VxD_LOCKED_CODE_SEG`. Those macros EXPAND to exactly the
// bytes we recover (`VMMCall Get_System_Time` assembles to cd203f000100), so the idiomatic form is not only readable: it
// re-assembles to the same image, `include vmm.inc` and all.
//
// This module transforms the recovered structures into that macro form and, through `verify`, checks that every driver's source
// goes through the DDK's ML.
//
//     const Emission code(Disassembly::run(driver, names), names);
//     std::string source = emit(code);             // the byte-exact flavour
//     std::string loadable = emit(code, true);     // relocations as `dd offset <label>`: a loadable driver
//
//     codebreaker vxd idiom FILE | FOLDER [OUT] | --verify [FOLDER]
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "base/corpus.hpp"
#include "source/vxd/disassembly.hpp"
#include "source/vxd/masm.hpp"

namespace cb::source::vxd::idiom {

using masm::Emission;
using PlaceSet = std::unordered_set<Place, PlaceHash>;

// ---- the DDK's names --------------------------------------------------------------------------------------------------------

// The DDK .INC that defines the services of a device (so `VxDcall <name>` resolves); nullopt for a device whose .INC does not
// ship in INC32 (VDD has none): a call to it keeps its raw `int 20h` and DWORD.
std::optional<std::string_view> device_include(std::uint32_t device);

// The INC files an object needs: vmm.inc always, and one per foreign device it calls.
std::vector<std::string> device_includes(const Emission& code, std::uint32_t object);

// `        VMMcall <name>` / `        VxDcall <name>` for an INT 20h service, or nullopt when the name is unknown (the caller keeps
// the raw bytes). Bit 0x8000 of the service word is DL_Jmp_Mask: the macros VxDCall and VMMCall generate the word without it,
// VxDJmp and VMMJmp with it. A reference marked that way is patched by the VMM's dynamic linker into a JMP, a tail transfer that
// does not return; written as a call it would invert the call and return contract and fault (Simulate_Far_Jmp is 0x00018049).
std::optional<std::string> service_macro(const DdkNames& names, std::uint32_t device, std::uint32_t service);

// The `Declare_Virtual_Device` line of the DDB. With the recovered fields (the REAL init order above all) the macro's DDB
// matches the original byte for byte. A VxD may also carry a V86 API procedure (+0x1C), a protected-mode one (+0x20) and reference
// data (+0x2C): each a DDB fix-up to a handler, and the trailing macro arguments. The fields are 0 in the image (the loader
// relocates them), so leaving the arguments out keeps the DDB byte-exact but loses the relocation: the API entry stays NULL and a
// call into it jumps to 0 (this broke VKD's boot). They are written only when `loadable`.
std::string declare_virtual_device(const Emission& code, const formats::le::Ddb& ddb, bool loadable = false);

// The VxD's own exported service table: the DDB's Service_Table_Ptr (+0x30) and Size (+0x34), one fix-up to a handler per entry.
struct ServiceTable {
    // Service i's own DDK name (`RESERVED` for a null slot), taken per INDEX and not from the handler's label so that services
    // which share one handler routine (retail stubs many debug services onto Out_Debug_String) keep distinct names.
    std::vector<std::string> names;
    Place at;               // where the raw table lies, so the object walk can skip it
    std::size_t count = 0;
    // A handler -> the service names to define there as extra labels (those that are not already its label), so that
    // `<name>_Service <svc>` resolves for every one.
    std::map<Place, std::vector<std::string>> extra;
};
// nullopt when there is no table, a handler landed mid-instruction (the pointer and a branch disagreed on alignment: the table stays
// raw db and the DDB keeps its zero fields), a slot has no fix-up and is not zero, or two services carry the same name. Emitted as
// `Begin_Service_Table` / `<name>_Service <svc>, LOCAL` / `End_Service_Table` with `Create_<name>_Service_Table`, so the DDB's fields
// are filled. Every service is LOCAL: the INC marks a few as not, which would EXTRN a handler that is defined here.
std::optional<ServiceTable> service_table(const Emission& code, const PlaceSet& emittable);

// The control dispatch: the handler of each message, and the places the macro regenerates (so the walk skips them). Two shapes the
// macro compiles to are recovered: a jump table (`cmp`/`sub eax,N; jae default; jmp [table+eax*4]`, the table perhaps far from the
// prologue) and a compare chain (`cmp eax,N; je handler`, repeated). Every handler must be an `emittable` label. When the procedure
// matches neither shape, or a handler is unresolved, nothing is recovered and the whole procedure is written as ordinary code:
// correct and complete, and never losing the routines between the prologue and the table.
struct ControlDispatch {
    std::vector<std::pair<std::string, std::string>> handlers;   // (message name, handler label)
    PlaceSet skip;
};
ControlDispatch control_dispatch(const Emission& code, const PlaceSet& emittable);

// ---- pieces of the listing --------------------------------------------------------------------------------------------------

// The length of the on-disk VxD_Desc_Block, so the object walk can skip it (the macro writes it again). The modern DDB has the
// `Prev` marker at +0x3C and its own size at +0x40; the classic one has neither and is 0x34 bytes. The marker is built by `dd 'Prev'`:
// the character constant is packed as the DWORD 0x50726576 and stored little-endian, so the bytes on disk are "verP". A modern DDB
// whose size field is zeroed in the image (the loader fills it) is still emitted by the macro in full: 0x50 bytes.
std::size_t ddb_size(const Emission& code, const formats::le::Ddb& ddb);

// A DATA span, readably: printable runs of at least four bytes as `db 'text'` (a quote doubled, long runs wrapped at `width`),
// everything else as `db 0XXh, ...` sixteen to a line. It assembles to the same bytes as all hex. For data only: a byte-preserved
// instruction stays hex.
std::vector<std::string> data_lines(ByteView bytes, std::size_t width = 60);

// The FLAT segment name and class of an object in the loadable emission. A UNIQUE class per object keeps LINK from merging objects
// of one class, which would fold a writable data object into the read-only code object (a fault on the first write); `CODE` in the
// class marks an object executable, `DATA` read and write, as in the original. The object that carries the DDB uses the DDK's own
// _LTEXT / LCODE, so the linker merges the recovered locked code with the macro-written DDB and service table into one locked
// object, as retail does.
struct SegmentName {
    std::string name, class_name;
};
SegmentName object_segment(std::uint32_t object, std::uint32_t flags, std::optional<std::uint32_t> ddb_object = std::nullopt);

// Give every INTERNAL fix-up target a label so the loadable emission can reference it: the recovery leaves many cross-object
// targets it never reached anonymous. A target inside an instruction is skipped (the walk steps by instruction and never lands
// there). A target with bit 0x80000000 is a HIGH-LINEAR ALIAS (Win95 maps a VxD into the shared arena at 0x80000000 and up): its
// real offset is the low 31 bits, which is what gets the label; the alias bit is restored where the pointer is written. A target
// beyond the object's size stays raw.
void label_fixup_targets(Emission& code);

// ---- the source -------------------------------------------------------------------------------------------------------------

// One VxD as idiomatic DDK-macro source (CRLF line ends): the includes, Declare_Virtual_Device, the segments, the control
// procedure as the dispatch macros, functions as BeginProc / EndProc, service calls as VMMcall / VxDcall. A label that collides with
// one the macros make (<name>_DDB, <name>_Service_Table, the `_<service>` thunks) is renamed; the instructions that overlap a region
// the macros regenerate are dropped. Both change `code`.
//
// In loadable mode the DDB and the service table are written in place at their real offsets, relocations are symbolic, and every
// instruction without a relocation is byte-preserved (re-encoding by ML would pick shorter forms, shift every later label and leave
// data pointers wrong).
std::string emit(Emission& code, bool loadable = false);

// The source file of the driver: the header comment, then `emit`. Written to <out>/<name>/<name>.asm (lower case); returns the path.
std::filesystem::path write_tree(const formats::le::Module& driver, std::string_view name, const std::filesystem::path& out_folder,
                                 const DdkNames& names);

struct NamedDriver {
    std::string name;                // lower case
    const formats::le::Module* driver;
};
// Assemble every driver's idiomatic source through the DDK's ML and say which assemble. The sources are batched: many in one session
// of the assembler (starting it, not assembling, is the cost), each session under `budget` lines (a larger source runs alone).
std::map<std::string, bool> verify(std::span<const NamedDriver> drivers, const DdkNames& names, const masm::Toolchain& tools, std::size_t budget = 40000);

// ---- the command ------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;
    const DdkNames& names;
    const masm::Toolchain& tools;
    std::filesystem::path output_folder;   // where a folder's sources go by default (the state folder's vxd/idiom)
};
// `FILE` the first 1600 characters of its source | `FOLDER [OUT]` every *.vxd and *.386 to a source tree each | `--verify [FOLDER]`.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::idiom
