// cb/verify/lanes/win32.hpp -- what is particular to reconstructing a 32-bit Windows module: the 32-bit judge lane.
//
// The module is decompiled and made compilable (decompile/c_text), every function is compiled with OpenWatcom and one DLL linked
// (build/functions), and each function is run beside the original in the 32-bit emulator (emulate/emulator, Unicorn) and compared
// (verify/compare): how the run ended, the calls into imports and their arguments, the result, the memory left changed outside the
// function's own frame, the stack, the saved registers, and the instructions that ran.
//
// What runs a function and how it is called is fixed here -- the stdcall convention with four arguments that every function is given
// (its real count is not known), or the first one or two in ECX and EDX for a member function (the decompiler says which). The
// rebuilt DLL is mapped beside the original in the same memory, so a global is the same storage for both and a call through a pointer
// the original holds still lands in the original; the rebuilt side's own statics (a compiler temporary, a register's stand-in) are
// left out of the comparison, and an argument that points into its image is compared as "somewhere in the image".
//
// The inputs are the fixed ones (`vectors`, then `confirm_vectors` for a body that matched on all of them), those aimed at the
// constants and globals the function's own code tests (`directed_vectors`), and those found by following the original's run
// symbolically path by path (`explore_vectors`: the lifter of emulate/pcode, the shadow of emulate/native_shadow, the solver of
// verify/concolic) or, where there is no lifter, guessed from the comparisons it makes (verify/guided).
//
// A run is a pure function of the module's bytes, the function and the input, so the original's fingerprints are kept across
// rebuilds (lane_common::remembered). The rebuilt side's are not.
//
// What the lane needs of its surroundings is given to it (`Services`): the decompiler's services, the function builder and the
// 32-bit tools, how the emulator finds modules and how many arguments an import takes, the flat thunk stubs of a module, the lifters,
// and where fingerprints are kept. A `Kind` (kind.hpp); the registry is told by `traits()` that a verdict is per function.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "cb/build/functions.hpp"
#include "cb/decompile/c_text.hpp"
#include "cb/emulate/emulator.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/verify/compare.hpp"
#include "cb/verify/concolic.hpp"
#include "cb/verify/guided.hpp"
#include "cb/verify/lane_common.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/reconstruct.hpp"

namespace cb::verify::lanes {

class Win32Lane final : public Kind {
public:
    // Bump when what a verdict depends on changes. 13: one build per batch; bodies the compiler rejects are dropped by name. 24, 25: a
    // rebuild stopped at the step limit where the original returned is run again with `kRoom`. 43-45: striped objects, globals with
    // values of their own, the original's data filled with pads. 46: an original cut off at the limit is matched by the calls it made,
    // the rebuild given `kRoom`. 47: the branch tracer enters a flat thunk's stubs as the fingerprint runs do. 48: branch-guided inputs
    // found path by path (Shadow32, verify/concolic), a verdict's limits saying paths=N / paths-all.
    static constexpr int kRev = 48;
    static constexpr std::size_t kArity = 4;                        // the arguments every function is given
    static constexpr std::size_t kMaxCalls = 64;                    // calls into imports a fingerprint records
    static constexpr std::size_t kMaxDirected = 16;                 // directed inputs kept per body
    static constexpr std::size_t kHarvestDepth = 2;                 // how far into callees to look for what branches test
    static constexpr std::size_t kHarvestFunctions = 12;
    static constexpr std::size_t kMaxReach = 6000;                  // addresses of reached instructions a fingerprint keeps
    static constexpr std::uint32_t kFillMax = 0x40000;              // bytes of a data section given values of its own
    static constexpr std::int64_t kMaxSupport = 1500;               // out-of-batch callee bodies pulled in per batch (the profile says it)
    // The rebuild is given this many steps when it stopped at the limit where the original returned.
    static constexpr std::int64_t kRoom = 25 * emulate::emulator::kMaxInsns;

    // A region of pointers that all resolve to a safe RET, so a pointer or handle argument that is dereferenced OR called through lands on
    // a clean return instead of a null fault. Both live inside the tracer's scratch window (0x200000..0x210000).
    static constexpr std::uint32_t kObject = 0x206000, kSafe = 0x207800;
    // The oracle's stack (and the RET landing pad just under it): writes there are the function's own frame, laid out differently by every
    // compiler.
    static constexpr std::uint32_t kStackLow = emulate::emulator::kStackBase;
    static constexpr std::uint32_t kStackHigh = emulate::emulator::kStackBase + emulate::emulator::kStackSize;

    // ---- the inputs --------------------------------------------------------------------------------------------------------

    // An input: the stack arguments and what imported calls return (compare::Input), memory it sets before the run, and what only this lane
    // adds: the first (two) arguments in ECX (and EDX), the arguments as the calls name them, a larger step limit for a rebuild stopped at
    // the limit, and the registers EBX ESI EDI EBP held at entry.
    struct Vector : compare::Input {
        std::optional<std::vector<guided::MemInit>> mem_init;
        std::optional<std::uint32_t> ecx, edx;
        std::optional<std::vector<std::int64_t>> given;      // before ecx / edx took some
        std::optional<std::int64_t> room;
        std::optional<std::array<std::uint32_t, 4>> sentinels;
    };
    // How the arguments are given to a function: the identity, or the first one or two in ECX and EDX.
    using Shape = std::function<Vector(const Vector&)>;

    static Vector in_ecx(const Vector& v);       // a member function's object in ECX, the rest on the stack
    static Vector in_ecx_edx(const Vector& v);   // the first two in ECX and EDX

    // The fixed inputs: zeros; scratch pointers; provisioned objects whose every field is a landing pad; counting numbers.
    static std::vector<Vector> vectors();
    // More inputs, run only for a body that already matched on every fixed one: imports that succeed, objects whose fields are not
    // pointers, boundary integers, reversed orderings, mixes of pointers and integers.
    static std::vector<Vector> confirm_vectors();
    // A pretend object whose every field is a pointer to a landing pad of its own: a call through a field returns, and WHICH field was
    // called through is seen.
    static std::vector<guided::MemInit> provisioned_memory();

    static guided::Input to_guided(const Vector& v);
    static Vector from_guided(const guided::Input& v);

    // Input `v` (as the run is given it: shaped) with the named inputs (the shadow's variables) set to `values`: an argument dword, ECX or
    // EDX, what the answered imports return, bytes of memory (put after the input's own, so they are what the run finds there). Nothing
    // when no value differs.
    static std::optional<Vector> derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names, const Shape& shape);

    // The bits of the original's return value that are not a result at all: a function returning 16 or 8 bits leaves the rest of EAX as it
    // was, often a copy of a register the caller owns. 0, 0xFFFF0000, 0xFFFFFF00 or 0xFFFFFFFF.
    using RunOriginal = std::function<compare::Fingerprint(const Vector&)>;
    static std::uint32_t undefined_ret_bits(const RunOriginal& original, const Vector& v, const compare::Fingerprint& a);

    // The import-call part of a fingerprint's name for an import: upper case, the module's extension dropped, one separator, so that the two
    // builds' import-table spellings of the same import compare equal ("KERNEL32.dll!#21" and "KERNEL32!#21").
    static std::string normalise_import(std::string_view name);
    // Toolchain and thunk imports that differ by code generation (OpenWatcom's against Microsoft's), not by the body's logic: left out of the
    // fingerprint.
    static bool is_noise(std::string_view api);

    // ---- one function ------------------------------------------------------------------------------------------------------

    // One function to judge.
    struct Item {
        std::string key;                         // its address as the store's keys write it
        std::uint32_t va = 0;
        std::filesystem::path dll;               // the rebuilt DLL
        std::uint64_t entry = 0;                 // where the function is in it
        int regs = 0;                            // 2: first two arguments in ECX and EDX, 1: the first in ECX, else 0
    };

    // How the lane finds what it asks of the world (everything but the build's own files and the decompiler's).
    struct Services {
        decompile::c_text::Services& decompiler;
        build::functions::FunctionBuilder& builder;
        build::functions::Tools32 tools;
        // How the emulator finds modules by name and path (the corpus's retail copies), and the headers' declarations.
        emulate::emulator::Environment tracer;
        // How many arguments (dwords) an import takes: the headers' answer, else the knowledge base's table; nullopt when not known.
        std::function<std::optional<std::int64_t>(std::string_view dll, std::string_view name)> import_argc;
        // The starts of the functions the thunk compiler generated in a flat-thunk module: not C, not rebuilt, and entered as imports.
        std::function<std::vector<std::uint64_t>(const std::filesystem::path& module)> flat_thunk_stubs;
        emulate::pcode::LifterSet* lifters = nullptr;                // none: inputs are guessed, not found path by path
        lane_common::FingerprintStore fingerprints;                  // unset: nothing is kept
        std::function<std::optional<std::string>(const std::filesystem::path&)> content_sha;   // the corpus's SHA-256
    };

    // What judging a function needs of the whole build: shared by every function of it, and safe to use from several threads.
    class Context;

    explicit Win32Lane(Services services);
    ~Win32Lane() override;

    const std::string& name() const override;
    // The digest covers the revision, the comparer's, the compiler's options, the fixed inputs and the exploration's limits.
    Profile profile(const std::filesystem::path& module_path) const override;
    // The module's functions (the carve's starts), decompiled and made compilable. A preparation carries its Compilable32 to build().
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    // Compile the functions with an address, link one DLL, and place it beside the original. The functions the compiler rejected, and
    // those the link did not place, are `missing`; a flat thunk's stubs are left to the original.
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    struct PreparedModule final : Prepared {
        decompile::c_text::Compilable32 module;
        std::map<std::string, std::int64_t> va;      // name -> address (the inverse of by_va)
    };

    // The addresses of the module's functions, ascending (the carve's).
    static std::vector<std::uint32_t> functions(const std::filesystem::path& module_path);

    // What the reconstruction path is told: a verdict is per function.
    static reconstruct::KindTraits traits();

    // The verdict of one function. Safe to call from several threads at once.
    static Judgement judge_function(const Context& context, const Item& item);

private:
    Services services_;
};

}  // namespace cb::verify::lanes
