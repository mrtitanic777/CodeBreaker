// cb/source/vxd/link.hpp -- a full-file LE rebuild of a VxD: ML (-coff) and LINK /VXD.
//
// The object-level test (masm) proves each object's CODE reassembles byte for byte. This goes the whole way: the entire driver is written
// as a multi-segment MASM source and a .DEF, assembled to COFF with the DDK's ML, and linked with `/VXD` (MS LINK, which runs natively on
// modern Windows) into a real .vxd (LE), whose objects are then compared with the original's.
//
// One segment per object of the original (a unique CLASS so LINK keeps them separate), its attributes from the object's LE flags, in
// object order; the DDB is published as VXD_DDB and exported @1 so LINK recognises the device. The first cut writes object bytes raw (no
// relocations) to validate the link path; symbolic fix-ups are layered on top of it (`functional`), and the idiomatic DDK-macro source
// (`build_loadable_idiomatic`) goes through ML, LINK and then through the byte-exact container writer.
//
// The tools are given: the assemblers as a masm::Toolchain, the linker as a function. `standard_link` and `link_260` are the real ones.
//
//     const Environment env{names, toolchain, standard_link(link_exe)};
//     CompareBuild r = build_and_compare(env, driver, "VKD");
//
//     codebreaker vxd link FILE [NAME]
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/le.hpp"
#include "cb/source/vxd/idiom.hpp"
#include "cb/source/vxd/masm.hpp"

namespace cb::source::vxd::link {

using masm::Emission;

// What a linker gave: the .vxd, when it made one, and what it printed.
struct Linked {
    std::optional<Bytes> vxd;
    std::string log;
};
// A linker: the COFF object and the .DEF text in, the .vxd out. `work` is a folder of the caller's to put them in (m.obj, m.def, m.vxd).
using LinkFn = std::function<Linked(ByteView object, std::string_view def_text, const std::filesystem::path& work)>;

struct Environment {
    const DdkNames& names;
    const masm::Toolchain& assemblers;
    LinkFn link;   // the linker the builds use
};

// MS LINK 2.50 (the Visual C++ 2.0 linker): `/VXD /NOD /OUT:m.vxd /DEF:m.def m.obj` in `work`. Kept for the byte-exact object experiments and
// for the loadable build, whose container the writer replaces: it is the WRONG linker for a loadable VxD on its own (its /VXD leaves the device
// id, LE 0xC0, blank and permutes the page map, the defects the DDK shipped LINK 2.60 to fix; see MSVC20\README.TXT).
LinkFn standard_link(std::filesystem::path link_exe);

// The DDK's LINK 2.60 with its DLLs, copied once into `stage_folder` (its /VXD self-spawn cannot cope with a space in its own path), then
// `/VXD /NOD /ALIGN:<align> /OUT:m.vxd /DEF:m.def m.obj`. Unlike 2.50 it stamps the device id from the exported <NAME>_DDB and its page map is
// the identity, so the output needs no container patching. `align` is the LE page size: 0x1000 is what VKD ships with (LINK 2.60's own default
// of 512 is also valid and used by other shipping VxDs).
LinkFn link_260(std::filesystem::path msvc20_folder, std::filesystem::path stage_folder, std::uint32_t align = 0x1000);

// ---- the source and the .DEF ------------------------------------------------------------------------------------------------

// The standard DDK VxD segment map, verbatim from BASE/SAMPLES/*/*.DEF (what boots VMD from source). LINK ignores segments absent from the
// object, so one template serves every VxD.
extern const std::string_view kVxdDefSegments;

// The .DEF attributes of an LE object's flags, the ones LINK /VXD maps back to flags.
std::string segment_attributes(std::uint32_t flags);

// The instructions to write raw per object (the encodings the assembler rejects or re-encodes to a mirror).
using ForceRaw = std::map<std::uint32_t, std::set<std::uint32_t>>;

// The body of one object for a FUNCTIONAL (loadable, not byte-exact) rebuild: every INTERNAL fix-up becomes a symbolic reference, so LINK /VXD
// writes a real LE relocation (the byte-exact path writes them raw, which loses the relocation and crashes on load). A near call or jmp with a
// self-relative fix-up (E8/E9 and a relocation on the rel32) is `call/jmp <label>`; any other fix-up field is a 32-bit offset, `dd offset
// <label>` (the instruction split into db, dd and db). An `int 20h` service, and an unlabelled or imported fix-up, stay raw db.
std::vector<std::string> link_body(const Emission& code, std::uint32_t object, const std::map<std::uint32_t, std::string>& extra_labels = {});

struct SourceAndDef {
    std::string source, def;   // CRLF line ends
};
// The source and the .DEF of the whole VxD: one segment per object in object order, the object that holds the DDB with a public VXD_DDB label
// exported @1. `force_raw` is the instructions to byte-preserve per object (the encodings the byte-exact test found), for byte-exact pages.
// `functional` writes internal fix-ups symbolically (`link_body`), so the linked VxD carries real relocations: loadable, though its container is not
// byte-exact. Functional also gives every internal fix-up target a label, which changes `code`.
SourceAndDef emit_source_and_def(Emission& code, std::string_view vxd_name, const ForceRaw& force_raw = {}, bool functional = false);

// ---- the byte-exact test, per object ----------------------------------------------------------------------------------------

struct ForceRawResult {
    ForceRaw force_raw;                          // per executable object: the offsets that made the object-level test exact
    std::map<std::uint32_t, bool> matched;       // per executable object: did it match
};
// The object-level byte-exact test of every executable object. The force-raw sets are the instructions the assembler rejects or re-encodes to a
// mirror; feeding them to the emitter makes the linked page data byte exact by construction.
ForceRawResult byte_exact_force_raw(const Environment& env, const formats::le::Module& driver);

// ---- comparing two drivers --------------------------------------------------------------------------------------------------

struct ObjectMatch {
    std::uint32_t mine = 0, oracle = 0;   // the object of the rebuild and the original object it was matched to
    std::size_t equal = 0, length = 0;    // bytes that are the same, and the object's length
    double percent() const { return length ? 100.0 * static_cast<double>(equal) / static_cast<double>(length) : 0.0; }
};
struct CompareObjects {
    std::vector<ObjectMatch> objects;
    std::size_t exact_objects = 0, total = 0;
};
// Each object of the rebuild matched to the object of the original with the same length and the best byte overlap (LINK reorders objects:
// discardable segments go to the end), and the exactness of each object's page data: the meaningful whole-file measure, how many objects' code
// linked byte for byte. An object no original object has the length of is not in the list.
CompareObjects compare_objects(const formats::le::Module& original, const formats::le::Module& rebuilt);

// ---- the builds -------------------------------------------------------------------------------------------------------------

// What every build reports. `stage` names where a failed build stopped (assemble, link, overlay); `log` is the tail of what that stage printed.
struct Build {
    bool ok = false;
    std::string stage, log;
    std::string source, def;   // what was assembled and what was linked, kept for a failed build
    std::string link_log;      // the tail of the linker's output
    Bytes vxd;
    std::size_t vxd_length = 0, original_length = 0;
};

struct CompareBuild : Build {
    CompareObjects report;
};
// Write, assemble and link the whole driver and compare its objects with the original's. `force_raw` are the instructions to byte-preserve.
CompareBuild build_and_compare(const Environment& env, const formats::le::Module& driver, std::string_view name, const ForceRaw& force_raw = {});

struct FunctionalBuild : Build {
    std::size_t original_fixups = 0, original_internal = 0, rebuilt_fixups = 0, rebuilt_internal = 0;
};
// A LOADABLE .vxd (relocations present, the container NOT byte-exact): internal fix-ups symbolic, assembled, linked, and the LE fix-up count of the
// result against the original's. A rebuilt internal count near the original's is the proof the relocations survived.
FunctionalBuild build_functional(const Environment& env, const formats::le::Module& driver, std::string_view name);

// One object of the overlay: which assembled object it was matched to, and how well.
struct OverlayRow {
    std::uint32_t object = 0;
    std::optional<std::uint32_t> assembled_object;
    double score = 0;                 // the fraction of comparable bytes that already agree
    std::size_t compared = 0;         // how many bytes that is of
    std::size_t expected = 0;         // the bytes the assembled object must supply
    std::size_t assembled_length = 0;
    std::size_t macro = 0;            // the bytes the DDK macros regenerate, absent from the assembled source
    bool ok = false;
};
struct Overlay {
    Bytes vxd;
    std::vector<OverlayRow> rows;
};
// The assembled-from-source CODE (from the LINK output `linked`) laid over the recovered object bytes of `code`, and the result written through
// the byte-exact LE writer, which rebuilds the loader-accepted container from the original.
//
// Why: LINK 2.50's LE container passes the loader for a small VxD (VMD) but Win95's strict LE loader REJECTS it for a dense one (VKD) at load
// time, before any VxD code runs, so the module faults with a "Windows protection error". The CODE LINK produces is faithful (only commutative
// re-encodings differ from the original); only the container is wrong. So the assembled code is kept and the container dropped: the assembled
// bytes go into each recovered object, and the writer wraps them in the original's exact object table, page map and fix-up records.
//
// Matching an assembled object to its recovered counterpart is the fragile step (LINK reorders segments, and two objects can have the same size),
// so it is NOT done by size alone: a first match by size silently overlays the wrong object's code. Every recovered object is scored against every
// unused assembled object by the overlay walk itself (the fraction of comparable bytes that already agree) and pairs are assigned greedily
// best-first: a correct pair agrees on nearly every comparable byte, a wrong pair only on noise, so the score matrix has a strong diagonal. An
// object whose best match falls below `min_score`, or for which no assembled object is long enough to cover its code, ABORTS the build (FormatError
// with the diagnosis) rather than shipping scrambled code.
//
// Byte rules per object: a fix-up's 4-byte field keeps the recovered value (the loader overwrites it from the fix-up record, and it differs from
// LINK's), so it is neither scored nor replaced. The DDB object's macro-regenerated regions (the control procedure and dispatch table, the exported
// service table and the DDB itself, written by Declare_Virtual_Device, Begin_Control_Dispatch and Begin_Service_Table) are not in the assembled
// source, so those recovered offsets are skipped and the assembled bytes map only the object's true code around the holes. Changes `code`.
Overlay overlay_code(Emission& code, const formats::le::Module& original, ByteView linked, double min_score = 0.5);

struct LoadableBuild : Build {
    std::size_t objects = 0, rebuilt_internal = 0, original_internal = 0;
    std::vector<OverlayRow> overlay;
    double min_overlay_score = 0;
};
// A LOADABLE LE .vxd from the idiomatic DDK-macro source and the real toolchain: `idiom::emit(loadable)`, ML, LINK, then the byte-exact container
// writer. Unlike `build_functional` (raw per-object segments, which hit A2107 far-call errors on cross-object references) the DDK's segment macros
// set up the flat model, so cross-object near calls assemble, and every internal fix-up goes through symbolically so LINK writes real relocations.
// LINK's own LE container loads for small VxDs (VMD ran to the desktop with a working mouse) but not for a dense one, so the assembled code is
// rewrapped in the loader-accepted container, with which a from-source VKD boots the real Win95 VMM to the logon. `name` defaults to the DDB's.
LoadableBuild build_loadable_idiomatic(const Environment& env, const formats::le::Module& driver, std::optional<std::string> name = std::nullopt);

// ---- the command ------------------------------------------------------------------------------------------------------------

// `FILE [NAME]`: the byte-exact test of every object, then the whole driver rebuilt and compared. Returns 0 when it links, 1 when it does not.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::link
