// cb/verify/lanes/win32_internal.hpp -- what the modules of the 32-bit judge lane share and nobody else needs: the context of a build
// (the emulators a judge runs on, the stubs, the caches) and the runs and searches a judge is made of.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cb/platforms/decoder.hpp"
#include "cb/verify/lanes/win32.hpp"

namespace cb::verify::lanes {

// One Tracer is for one thread's sequence of runs: judges on several threads each take one from the pool, and put it back.
class Win32TracerPool {
public:
    explicit Win32TracerPool(emulate::emulator::Environment environment) : environment_(std::move(environment)) {}

    class Lease {
    public:
        Lease(Win32TracerPool& pool, std::unique_ptr<emulate::emulator::Tracer> tracer) : pool_(pool), tracer_(std::move(tracer)) {}
        Lease(const Lease&) = delete;
        Lease& operator=(const Lease&) = delete;
        ~Lease() { pool_.release(std::move(tracer_)); }
        emulate::emulator::Tracer& get() { return *tracer_; }

    private:
        Win32TracerPool& pool_;
        std::unique_ptr<emulate::emulator::Tracer> tracer_;
    };

    Lease acquire();

private:
    void release(std::unique_ptr<emulate::emulator::Tracer> tracer);

    emulate::emulator::Environment environment_;
    std::mutex lock_;
    std::vector<std::unique_ptr<emulate::emulator::Tracer>> idle_;
};

// The stubs a flat thunk's compiler generated: a call into one goes on into 16-bit code through a pointer only Windows fills in; the judge
// enters it as it enters an import, removing the arguments its `ret N` removes, so the functions that call the 16-bit side can be judged at
// all.
struct Win32Stubs {
    std::map<std::uint64_t, std::string> at;                          // address -> name
    std::map<std::string, std::optional<std::int64_t>> argc;          // name -> arguments its `ret N` removes (none: not a whole number)
};

class Win32Lane::Context {
public:
    Context(const Win32Lane::Services& services, std::filesystem::path original);

    const std::filesystem::path original;
    std::optional<std::string> sha;                                   // the original's content hash: what its fingerprints are filed under
    Win32Stubs stubs;
    lane_common::FingerprintStore fingerprints;
    emulate::pcode::Lifter* lifter = nullptr;                         // the x86-32 lifter; none: the inputs are guessed
    std::function<std::optional<std::int64_t>(std::string_view, std::string_view)> import_argc;
    mutable Win32TracerPool tracers;

    // The address range of an image (a PE module as loaded); (0, 0) when it cannot be read.
    std::pair<std::uint64_t, std::uint64_t> image_span(const std::filesystem::path& module) const;
    // The module's writable data sections as loaded.
    std::vector<std::pair<std::uint64_t, std::uint64_t>> writable(const std::filesystem::path& module) const;
    // A decoder of 32-bit x86 with the operands' detail.
    const platforms::Decoder& decoder() const;

    // What the original's code says of a function: the instructions reachable from its entry (0 when not resolved).
    std::int64_t insn_count(std::uint64_t func) const;
    // The immediates the function (and the functions it calls, a few levels down) compares or masks with, and the data addresses it reads:
    // what its branches test, so what an input has to contain to get past them.
    struct Harvest {
        std::vector<std::uint32_t> constants;
        std::vector<std::uint32_t> globals;
    };
    const Harvest& harvest(std::uint64_t func) const;
    // mem_init that gives every word of the original's writable sections a value of its own: the address of a landing pad.
    const std::vector<guided::MemInit>& data_filled() const;

    mutable std::mutex lock;   // guards the caches below

private:
    mutable std::map<std::filesystem::path, std::pair<std::uint64_t, std::uint64_t>> spans_;
    mutable std::map<std::uint64_t, std::int64_t> insns_;
    mutable std::map<std::uint64_t, Harvest> harvest_;
    mutable std::optional<std::vector<guided::MemInit>> filled_;
    mutable platforms::DecoderSet decoders_;
};

namespace win32 {

using Lane = Win32Lane;
using Vector = Win32Lane::Vector;
using Shape = Win32Lane::Shape;
using Context = Win32Lane::Context;

// ---- one run, as a fingerprint (win32_run.cpp)

// What a run is a function of, as text: its arguments, memory, what imports return, the registers at entry, the step limit.
std::string vector_text(const Vector& v);

// Fingerprint one run of `func` in `module`. For a REBUILT image `rebuilt_of` is the path of the original: its own statics are then left
// out of the comparison and the original's code is mapped beside it. Kept across runs under the module's content hash (the original's only).
compare::Fingerprint fingerprint(const Context& context, const std::filesystem::path& module, std::uint64_t func, const Vector& spec,
                                 const std::optional<std::filesystem::path>& rebuilt_of = std::nullopt);

// The original run on `spec` with its branches logged, or nothing when the run is not an observation (as fingerprint(): an error, or a call
// into nothing).
std::optional<guided::Observation> branch_run(const Context& context, std::uint64_t func, const Vector& spec);
// The same run as the emulator's record, with `shadow` (a symbolic shadow of the run: its inputs are the arguments, registers and memory) kept
// up beside it one instruction at a time; and what the guessing explorer reads of it.
std::optional<emulate::Run> observed_run(const Context& context, std::uint64_t func, const Vector& spec, emulate::native_shadow::Shadow* shadow = nullptr);
guided::Observation observation_of(const emulate::Run& run);

// ---- inputs aimed at branches (win32_paths.cpp)

// Inputs chosen for THIS function. Generic inputs stop at the first guard (`if (msg != 0x4E) return`, `if (!g_initialised) return`), so most
// of a body never ran and was "verified" on its first few instructions. Candidates put each constant the function compares against into each
// argument, and set the globals it reads to non-zero values; a candidate is kept only if the ORIGINAL then runs instructions no earlier input
// reached. At most kMaxDirected. `reached`: what the fixed inputs reached.
std::vector<Vector> directed_vectors(const Context& context, std::uint64_t func, const std::set<std::int64_t>& reached, const Shape& shape);

// Inputs that take branches of the original no seed took (verify/guided).
std::vector<Vector> guided_vectors(const Context& context, std::uint64_t func, const std::vector<Vector>& seeds, const Shape& shape);

// (inputs, exploration): inputs taking paths of the original no seed took, found path by path, and how far the exploration got (none where
// the function could not be resolved or there is no lifter).
std::pair<std::vector<Vector>, std::shared_ptr<concolic::Stats>> explore_vectors(const Context& context, std::uint64_t func,
                                                                                std::vector<Vector> seeds, const Shape& shape);

// The directed inputs, then those found path by path (or guessed), seeded with every input run so far. `found` gets the exploration.
std::vector<Vector> directed_and_guided(const Context& context, std::uint64_t func, const std::set<std::int64_t>& reached, const Shape& shape,
                                        std::shared_ptr<concolic::Stats>* found = nullptr);

}  // namespace win32

}  // namespace cb::verify::lanes
