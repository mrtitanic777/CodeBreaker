// cb/source/ne_asm/idiom.hpp -- a 16-bit NE code segment as idiomatic Microsoft SDK cMacros source (cProc / cBegin / cEnd).
//
// The KERNEL, USER and GDI analog of the VxD's kit-macro form (source/vxd/idiom). The mechanical form of recon.hpp -- a flat `_TEXT segment`,
// numeric labels, branches across functions written as `db` -- becomes structured, named source that still assembles BYTE-EXACT through the
// SDK's CMACROS.INC:
//
//     OPTION M510        ; MASM 5.10 compatibility: cMacros 5.20 pastes with the old `&'
//     .286p              ; lar, lsl and the like
//     ?PLM = 1 / ?WIN = 1
//     include cmacros.inc
//     sBegin CODE ... sEnd CODE
//     ml /c /Ic:\inc /Fom.obj m.asm            (16-bit OMF, no -coff)
//
// The _TEXT of the object is compared with the shipped segment's, relocations masked, exactly as the VxD path does. Every function is
// `cProc name,<NEAR>` / `cBegin nogen` / body / `cEnd nogen`: `nogen` adds no bytes, so the body keeps the shipped bytes and gains structure,
// names and symbolic branches. What the assembler encodes another way is pinned as `db` (verify_range's loop), so the source is byte-exact and what
// is measured is how much of it stays live.
//
// Two assemblers do it: the period ML under DOS (CMACROS.INC as shipped), or a nasm built from the nasm-masm fork (its cmacros_shim.inc replaces
// CMACROS.INC), a hundred times faster and with no DOS. nasm picks other, equivalent encodings on the ambiguous cases, so the loop pins nasm's own
// divergers, found by direct comparison.
//
//     codebreaker ne-asm idiom [MODULE [SEG [LO [HI]]]]      HI may be `whole`; the env var NE_SAVE=FILE keeps the source
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/build/dos_run.hpp"
#include "cb/source/ne_asm/fixups.hpp"
#include "cb/source/ne_asm/recon.hpp"
#include "cb/source/ne_asm/segdiff.hpp"
#include "cb/source/ne_asm/segment.hpp"

namespace cb::source::ne_asm::idiom {

enum class Backend { Ml, Nasm };
std::string_view backend_name(Backend backend);

// The two assemblers; one may be missing (a run on the other backend does not need it).
struct Assemblers {
    recon::Assemble ml, nasm;
    // UsageError when the backend's assembler is not there.
    const recon::Assemble& of(Backend backend) const;
};

// nasm in MASM mode on a source (16-bit OMF), CMACROS.INC redirected to the shim of the nasm folder (<nasm folder>/test/masm/cmacros_shim.inc),
// copied to `include_folder` (a path with no space in it) first. Nothing is kept of the run.
recon::Assemble nasm_assembler(std::filesystem::path nasm, std::filesystem::path include_folder);

// Where the period ML runs from: ML.EXE and ML.ERR (the DDK's MASM611C), the DOS extender it needs (MSVC 1.52's DOSXNT.EXE) and the SDK's include
// files (INC16: CMACROS.INC and the rest). `stage` is a folder with no space in its path: the DOS emulator cannot mount one that has, so the
// three programs and the includes (in <stage>/inc) are copied there once.
struct MlTools {
    std::filesystem::path ml_folder, extender, includes, stage;
};
// ML under a DOS emulator on a source: `ml /c /Ic:\inc /Fom.obj /Flm.lst m.asm`. `runner` must outlive the assembler.
recon::Assemble ml_assembler(const build::DosRunner& runner, MlTools tools);

// ---- the source -------------------------------------------------------------------------------------------------------------

// A name `cProc` accepts for the function at `offset`: the export's name or the recovered one with anything outside [A-Za-z0-9_$?@] made `_` (a
// digit first gets a `_` before it), else sub_<offset>. The name never decides byte-exactness.
std::string procedure_name(const segment::Seg& seg, std::uint32_t offset);

struct Emitted {
    std::string source;
    // The segment offsets of branch displacements that leave the range: unresolved (0000) in an isolated object and right once the whole segment
    // is there, so a range's comparison masks them as it masks a relocation.
    std::set<std::uint32_t> unresolved;
};
// Functions covering [start, end) as cMacros source. `functions`: the function starts of the range (default: the carve's); none at all makes the
// whole range one function, so a segment the carve found nothing in is written rather than dropped. `force_db`: offsets pinned as db (the divergers
// the loop found), `data_off`: offsets inside safe data regions, pinned as db too.
Emitted emit(const segment::Seg& seg, std::uint32_t start, std::uint32_t end, const std::optional<std::vector<std::uint32_t>>& functions = std::nullopt,
             const std::set<std::uint32_t>& force_db = {}, const std::set<std::uint32_t>& data_off = {});

// ---- verifying ---------------------------------------------------------------------------------------------------------------

struct Reconciled {
    std::string source;
    std::optional<segdiff::Verdict> verdict;
    std::set<std::uint32_t> force_db;
    std::size_t code_instructions = 0;
};
// Emit [start, end), assemble, and pin each diverging instruction as db until the object is byte-exact: the reconciliation of recon.hpp with this
// emitter. One range is one assembly unit, so an instruction re-encoded shifts everything after it; the listing finds them (length first, then
// same-length), and where it cannot place a difference (a misdecoded region, an inline jump table read as code) the whole function holding the first
// difference becomes db, which realigns what follows.
Reconciled reconcile_range(const segment::Seg& seg, std::uint32_t start, std::uint32_t end, const Assemblers& assemblers, Backend backend,
                           const std::optional<std::vector<std::uint32_t>>& functions = std::nullopt, std::size_t max_passes = 60);

struct Verified {
    bool ok = false;                 // byte-exact
    std::size_t bytes = 0;           // the range's length
    std::size_t code_instructions = 0, pinned = 0, live = 0;
    std::string source;
    std::optional<segdiff::Verdict> verdict;   // when it is not exact
};
Verified verify_range(const segment::Seg& seg, std::uint32_t start, std::uint32_t end, const Assemblers& assemblers, Backend backend,
                      const std::optional<std::vector<std::uint32_t>>& functions = std::nullopt);

// ---- the command -------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;
    Assemblers assemblers;
    std::filesystem::path state;           // recovered names are <state>/idiomatic/<module>/names.json
    fixups::Objdump objdump = fixups::run_objdump;
};
// `[MODULE [SEG [LO [HI]]]] [--backend ml|nasm] [--ords FILE]`: the range as cMacros source, byte-exact, and how much of it is live. Returns 0
// (1 when the segment has no code).
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::ne_asm::idiom
