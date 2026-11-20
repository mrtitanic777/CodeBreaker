// source/vxd/masm.hpp -- assemble-and-compare for a recovered VxD object.
//
// The disassembly proves the listing is a faithful VIEW of the bytes. This closes the loop the other way: re-emit an executable
// object as MASM, assemble it, and compare the assembled code bytes with the original object -- the byte-exact test.
//
// The emission rule that keeps the comparison honest:
//   * an instruction with NO fix-up is emitted symbolically (mnemonic and operands) and re-encoded by the assembler -- this is
//     what the test measures: does it reproduce the original compiler's encoding?
//   * an instruction WITH a fix-up (a cross-object call or reference whose original bytes are a zero placeholder and a
//     fix-up record), a VxD service call, and every data byte are emitted as raw `db`: exact by construction, since an
//     object-level assembly has no linker to resolve fix-ups.
// So a byte difference can only come from the assembler choosing another legal encoding than the compiler did (A1 or 8B for a
// moffs load, imm8 or imm32, rel8 or rel32); the report lists exactly those.
//
// The assemblers are MASM-compatible: x86asm (NASM's MASM mode, built in), or MASM 6.11d for Windows (both run directly), or the period
// ML.EXE under DOS, which the caller supplies as a `PeriodAssembler` because running DOS is not this module's business.
//
//     const Emission code(Disassembly::run(driver, names), names);
//     const ModuleReport report = compare_module(driver, names, toolchain);
//
//     codebreaker vxd masm FILE [--module NAME | OBJECT]
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

#include "base/corpus.hpp"
#include "core/bytes.hpp"
#include "formats/le.hpp"
#include "source/vxd/ddk.hpp"
#include "source/vxd/disassembly.hpp"

namespace cb::source::vxd::masm {

// ---- the recovered driver, as the emitters work on it -----------------------------------------------------------------------

// A copy of what the recovery found, which the emission is free to change: the idiomatic emitter names labels the DDK's macros
// would collide with and drops instructions that overlap a region the macros regenerate. The recovery itself stays as it was.
struct EmissionParts {
    std::optional<formats::le::Ddb> ddb;
    Disassembly::FixupMap fixups;
    std::vector<Bytes> code;   // by object (object 1 is code[0])
    std::map<Place, Instruction> instructions;
    std::map<Place, std::string> labels;
    std::map<Place, std::vector<Place>> jump_tables;
};

class Emission {
public:
    Emission(const Disassembly& recovered, const DdkNames& names);
    // From parts (a recovery the caller changed, or one made by hand). `driver` and `names` must outlive the Emission.
    Emission(const formats::le::Module& driver, const DdkNames& names, EmissionParts parts);

    const formats::le::Module& driver() const { return *driver_; }
    const DdkNames& names() const { return *names_; }
    const std::optional<formats::le::Ddb>& ddb() const { return ddb_; }
    // Every fix-up in the order the module lists them.
    const Disassembly::FixupMap& fixups() const { return fixups_; }
    std::optional<formats::le::Fixup> fixup_at(const Place& at) const;
    // The bytes of object `object` (1-based) as loaded; empty for an object that is not there. Changed only through code_mut.
    ByteView code(std::uint32_t object) const;
    Bytes& code_mut(std::uint32_t object);
    std::map<Place, Instruction>& instructions() { return instructions_; }
    const std::map<Place, Instruction>& instructions() const { return instructions_; }
    std::map<Place, std::string>& labels() { return labels_; }
    const std::map<Place, std::string>& labels() const { return labels_; }
    const std::map<Place, std::vector<Place>>& jump_tables() const { return jump_tables_; }
    // The label of a place, nullptr when it has none (an empty name is none).
    const std::string* label_at(const Place& at) const;
    const Instruction* instruction_at(const Place& at) const;

private:
    const formats::le::Module* driver_;
    const DdkNames* names_;
    std::optional<formats::le::Ddb> ddb_;
    Disassembly::FixupMap fixups_;
    std::vector<Bytes> code_;
    std::map<Place, Instruction> instructions_;
    std::map<Place, std::string> labels_;
    std::map<Place, std::vector<Place>> jump_tables_;
};

// ---- MASM text for one instruction ------------------------------------------------------------------------------------------

// Whether the mnemonic is a call, a jump, a loop: an instruction with a direct target.
bool is_branch(std::string_view mnemonic);

// The operands with every `0x1F` written as MASM does (`01Fh`: a number that starts with a letter takes a leading zero).
std::string masm_operands(std::string_view operands);

// MASM text for one instruction (`        mov    eax,ebx`), or nullopt when it has to be raw bytes instead: a far branch in
// capstone's spelling, or a direct branch to an address with no label (MASM rejects `jmp <number>`).
std::optional<std::string> masm_line(const Emission& code, std::uint32_t object, const Instruction& insn);

// `        db  0AAh, 0BBh` for the bytes.
std::string db_line(ByteView bytes);

// An operand that is a bare absolute address, `[0x1234]` or `[0]`. MASM is unreliable with these: `mov eax,[disp]` is the 5-byte
// moffs form it may re-encode to 6 bytes (a drift), and `add [disp],reg` it rejects (it reads a bracketed bare number as an
// immediate). Forcing them to raw keeps the exact bytes and the assembler out of trouble.
bool has_absolute_memory(const Instruction& insn);

// A segment-override prefix (CS ES SS DS FS GS: 2E 26 36 3E 64 65). Ring-0 code uses these to reach V86 memory and often carries
// a redundant CS: that MASM drops silently, a length drift that cascades into every later call target.
bool has_segment_prefix(const Instruction& insn);

// ---- the object, as MASM source ---------------------------------------------------------------------------------------------

struct BodyOptions {
    std::set<std::uint32_t> force_raw;                  // instructions to emit as db
    std::size_t base_line = 0;                          // the lines before the body, for the line map
    std::map<std::uint32_t, std::string> extra_labels;  // label names to define at offsets (the exported DDB)
    // A `Q<hexoff>:` label before every instruction. Labels emit no bytes, so the code is the same, but the COFF symbol table
    // then shows each instruction's true assembled offset: one assembly finds every drifting instruction at once.
    bool probe = false;
    std::optional<std::pair<std::uint32_t, std::uint32_t>> range;   // [lo, hi): one source file of a split object
};

struct Body {
    std::vector<std::string> lines;
    // 1-based source line (base_line + index) of a symbolic instruction -> its offset in the object.
    std::map<std::size_t, std::uint32_t> line_map;
};

// The body lines of one object: instructions symbolically; data, fix-up-bearing and service instructions as raw db; labels at
// branch and fix-up targets. Shared by the object test, the split emitter and the full link.
Body object_body(const Emission& code, std::uint32_t object, const BodyOptions& options = {});

struct ObjectSource {
    std::string text;   // CRLF line ends
    std::map<std::size_t, std::uint32_t> line_map;   // a source line -> the instruction offset, to trace an assembler error
};
// One executable object (or one offset range of it) as an assemblable MASM `use32` segment.
ObjectSource emit_object(const Emission& code, std::uint32_t object, std::string_view segment_name = "_LTEXT",
                         const std::set<std::uint32_t>& force_raw = {}, bool probe = false,
                         const std::optional<std::pair<std::uint32_t, std::uint32_t>>& range = std::nullopt);

// An executable object cut into function-aligned offset ranges of about `max_instructions` each: the real source was many files,
// and ML cannot assemble one huge source. Cuts fall on function starts (`sub_` labels).
std::vector<std::pair<std::uint32_t, std::uint32_t>> split_ranges(const Emission& code, std::uint32_t object, std::size_t max_instructions = 6000);

// Offsets in [lo, hi) whose branch leaves the range: the target's label lives in another file, so in an isolated per-file
// assembly the instruction must be byte-preserved.
std::set<std::uint32_t> cross_range_force(const Emission& code, std::uint32_t object, std::uint32_t lo, std::uint32_t hi);

// ---- the assemblers ---------------------------------------------------------------------------------------------------------

// What an assembler gave: the object file, when it made one, and what it printed.
struct Assembled {
    std::optional<Bytes> object;
    std::string log;
};

// The period ML.EXE (a Phar Lap TNT program) run under DOS, in as few sessions as it can: texts in, results aligned with them.
using PeriodAssembler = std::function<std::vector<Assembled>(std::span<const std::string> texts)>;

struct Toolchain {
    bool x86asm = false;                                // x86asm, CodeBreaker's own assembler (NASM's MASM mode), in this process; `standard` sets it
    std::optional<std::filesystem::path> native_ml;     // MASM 6.11d for Windows
    std::filesystem::path include_dir;                  // the DDK's INC32
    PeriodAssembler period;                             // the fallback; none: no assembler when the two above are missing

    // The tools as this machine has them: x86asm always; MASM 6.11d from the NT 3.51 DDK under `checkout_root`/toolchains, else
    // the Win95 DDK's MASM611C. CB_FORCE_ML leaves x86asm and MASM out, to pin the authentic period assembler.
    static Toolchain standard(const DdkFolders& ddk, const std::filesystem::path& checkout_root, PeriodAssembler period = {});
};

// Assemble MASM text to a COFF object with the best assembler the toolchain has: x86asm, else MASM 6.11d, else the period one.
Assembled assemble_coff(const Toolchain& tools, std::string_view masm_text);
// Many texts; with the period assembler they share its sessions.
std::vector<Assembled> assemble_coff_batch(const Toolchain& tools, std::span<const std::string> texts);

// Assemble source that includes the DDK's macro headers (`include vmm.inc`): only MASM does, so x86asm is not tried. MASM 6.11d, else the
// period assembler.
std::vector<Assembled> assemble_macro_batch(const Toolchain& tools, std::span<const std::string> texts);

// The code section of a COFF object and where each symbol of it lies (section-relative): the true assembled offset of every
// probe-labelled instruction. The code section is the first that is marked code and has bytes, else the largest. FormatError for
// an object that has no sections or is cut short.
struct CoffText {
    Bytes text;
    std::map<std::string, std::uint32_t> symbols;
};
CoffText coff_text(ByteView object);

// ---- the byte-exact test ----------------------------------------------------------------------------------------------------

struct RangeResult {
    std::size_t symbolic = 0, raw = 0;   // instructions re-encoded by the assembler, and byte-preserved
    bool match = false;
    std::set<std::uint32_t> force;
    Bytes text;   // the range's assembled bytes (the original's when it matches)
};

// Drive one offset range [lo, hi) of an object to a byte-exact assembly through probe labels; a branch leaving the range is
// byte-preserved. `first` seeds the first pass with an assembly already made (its force must be cross_range_force), so a caller can
// assemble every range's first pass in one batch.
struct FirstPass {
    Assembled assembled;
    std::map<std::size_t, std::uint32_t> line_map;
};
RangeResult converge_range(const Emission& code, std::uint32_t object, const Toolchain& tools, std::uint32_t lo, std::uint32_t hi,
                           ByteView original, std::size_t max_repair, const std::optional<FirstPass>& first = std::nullopt);

struct ObjectReport {
    std::uint32_t object = 0;
    bool match = false;
    std::size_t original_length = 0, assembled_length = 0;
    std::size_t symbolic = 0, forced_raw = 0, files = 0;
    std::set<std::uint32_t> force_raw;
    bool all_raw = false;   // code that had to be forced whole
    Bytes assembled, original;
};

// Assemble-and-compare one object, maximising genuine re-encodes (no wholesale byte copy): every instruction is symbolic and
// probe-labelled, one assembly shows which ones the assembler encoded differently, and only those are byte-preserved. A large
// object is split into function-aligned files of about `max_instructions`, each converged on its own.
ObjectReport compare_object(const Emission& code, std::uint32_t object, const Toolchain& tools, std::size_t max_repair = 8,
                            std::size_t max_instructions = 6000);

struct ModuleReport {
    bool byte_exact = false;   // every executable object rebuilds byte for byte
    std::vector<ObjectReport> objects;
    std::size_t symbolic = 0, forced_raw = 0, all_raw_objects = 0;
};
// Every executable object of the driver. The first assembly of every range of every object is one batch; only a range that does
// not converge on it takes further, per-range assemblies.
ModuleReport compare_module(const Emission& code, const Toolchain& tools, std::size_t max_repair = 8, std::size_t max_instructions = 6000);

// ---- the command ------------------------------------------------------------------------------------------------------------

struct Environment {
    const DdkNames& names;
    const Toolchain& tools;
};
// `FILE [--module NAME]` every executable object, or `FILE OBJECT` one. Returns 0 when everything is byte-exact, else 1.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::masm
