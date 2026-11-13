// cb/verify/lanes/powerpc.hpp -- what is particular to reconstructing a PowerPC image.
//
// The image is decompiled and made compilable (decompile/c_text), every function is compiled with a PowerPC compiler and one image
// linked (build/functions), and each function is run beside the original in the PowerPC interpreter (emulate/powerpc).
//
// Both sides get the same eight argument registers. The rebuilt image is placed beside the original in the same memory, so a global is
// the same storage for both and a call through a pointer the original holds still lands in the original. Compared: how the run ends,
// the calls to functions outside the image in order, the result in r3 (not for a function that returns nothing), the memory left
// changed outside the stack, and whether r14-r31 and the stack pointer were handed back.
//
// How many arguments a function outside the image takes is not stated anywhere in a PowerPC image. Where every call to it in the module
// passes the same number, that many are compared; otherwise the call is compared by name only and a match involving it is marked weak.
// An argument that points into either image's data is compared by what it points at, since the rebuilt image keeps its own copy of a
// constant.
//
// The inputs are the fixed ones (`vectors`), those aimed at the constants the function's own code compares with (`directed`), and those
// found by following the original's run symbolically path by path (`explore`, with the lifter of emulate/pcode, the shadow of
// emulate/shadow and verify/concolic) or, where there is no lifter, guessed from the comparisons it makes (verify/guided).
//
// What the lane needs of its surroundings is given to it (`Services`): the decompiler's services, the function builder and the
// PowerPC tools, the reader of an image's memory, the lifter, and where fingerprints are kept. A `Kind` (kind.hpp).
#pragma once

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
#include "cb/emulate/pcode.hpp"
#include "cb/emulate/powerpc.hpp"
#include "cb/verify/compare.hpp"
#include "cb/verify/concolic.hpp"
#include "cb/verify/guided.hpp"
#include "cb/verify/lane_common.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/reconstruct.hpp"

namespace cb::verify::lanes {

class PowerPcLane final : public Kind {
public:
    // Bump when what a verdict depends on changes. 4: branch-guided inputs; calls into absent code. 5: inputs found path by path (the
    // shadow and the solver), a verdict's limits saying paths=N / paths-all.
    static constexpr int kRev = 5;
    static constexpr std::size_t kArguments = 8;                      // r3-r10
    static constexpr std::uint32_t kRebuiltOffset = 0x01000000;       // the rebuilt image sits at least this far above the original
    // A question over 64-bit registers is a bigger one for the solver than the 16-bit lane's: those it cannot answer cost it four times as
    // long before it gives up. A quarter of concolic::kSolverSteps -- still z3's own steps, the same on any machine.
    static constexpr std::uint32_t kSolverSteps = 500000;

    using Region = emulate::powerpc::Region;
    using Span = emulate::powerpc::AddressRange;                       // [lo, hi)

    // ---- the inputs --------------------------------------------------------------------------------------------------------

    // An input: eight argument registers and what calls outside the image return (compare::Input), the fill of the memory behind
    // pointer-shaped arguments, and memory the input sets itself.
    struct Vector : compare::Input {
        std::optional<emulate::powerpc::Fill> fill;
        std::optional<std::vector<Region>> mem_init;
    };

    // The inputs every function is run on. Memory an argument points at reads as zeros unless the image put something there, on both
    // sides alike.
    static std::vector<Vector> vectors();
    // Inputs aimed at what the function's own code tests: the constants it compares values with (cmpi, cmpli; `word` reads the image's
    // code), given in each argument register, as what calls outside return, and as what the memory behind a pointer holds. At most `most`
    // constants.
    static std::vector<Vector> directed(const std::function<std::optional<std::uint32_t>(std::uint32_t)>& word, Span span, std::size_t most = 6);

    static guided::Input to_guided(const Vector& v);
    static Vector from_guided(const guided::Input& v);

    // Input `v` with the named inputs (shadow.vars) set to `values`: an argument register, what the calls outside the image return, bytes of memory -- behind the pointers given as
    // whole pages, the input's `fill` under them. Nothing when no value differs.
    static std::optional<Vector> derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names);

    // ---- what a run showed -------------------------------------------------------------------------------------------------

    // The images in memory: the original alone, the original with the rebuilt one, and the extent of each.
    struct Memory {
        emulate::powerpc::PageImage alone, both;
        std::vector<Span> images;                                      // [original] or [original, rebuilt]
    };
    static Memory memory_of(std::span<const Region> original, std::span<const Region> rebuilt);

    // One argument of a recorded call, written as what it is rather than its bit pattern where the two builds would legitimately differ.
    static std::string argument(emulate::powerpc::Machine& machine, std::uint64_t value, std::span<const Span> images);

    // What one run of one function did, in a form two builds can be compared on. `argc` {function outside: how many arguments}; `span`
    // the function's own addresses (none for the rebuilt side); `images` the extent of each image (the first is where calls into code that
    // is not there land).
    static compare::Fingerprint fingerprint(const emulate::powerpc::PageImage& shared, std::uint32_t entry, const Vector& v,
                                            const emulate::powerpc::ExternTable& externs, const std::map<std::string, std::int64_t>& argc,
                                            std::span<const Span> images, const std::optional<Span>& span = std::nullopt, bool novalue = false);

    // ---- one function ------------------------------------------------------------------------------------------------------

    // One function to judge.
    struct Item {
        std::string key;                      // its address as the store's keys write it
        std::uint32_t va = 0;
        Span span;                            // [its start, the next function's start)
        std::int64_t insns = 0;
        std::uint32_t rebuilt_va = 0;
        bool novalue = false;                 // it returns nothing
    };
    // What judging a function needs of the whole build.
    struct Context {
        Memory memory;
        emulate::powerpc::ExternTable ext_original, ext_rebuilt;
        std::map<std::string, std::int64_t> argc;
        std::optional<std::string> sha;       // the original's content hash: what fingerprints of the original are filed under
        lane_common::FingerprintStore fingerprints;
        emulate::pcode::Lifter* lifter = nullptr;   // the PowerPC lifter; none: the inputs are guessed
        std::vector<Region> original_regions; // the original's bytes, for the constants its code compares with
        std::vector<Span> writable;           // the original's writable data: the globals an input may set
        mutable std::mutex lock;              // guards `directed_by_span`
        mutable std::map<std::pair<std::uint32_t, std::uint32_t>, std::vector<Vector>> directed_by_span;
    };

    // The directed inputs, then those found path by path (or guessed). `explored` gets the exploration, when there was one.
    static std::vector<Vector> directed_and_guided(const Context& context, std::uint32_t va, Span span,
                                                   std::shared_ptr<concolic::Stats>* explored = nullptr);
    // Inputs for the paths of the original no seed took, found path by path: `kept` and the exploration's counts.
    static std::pair<std::vector<Vector>, std::shared_ptr<concolic::Stats>> explore(const Context& context, std::uint32_t va, Span span,
                                                                                    std::span<const Vector> seeds);
    // The verdict of one function. Safe to call from several threads at once.
    static Judgement judge_function(const Context& context, const Item& item);

    // ---- the kind ----------------------------------------------------------------------------------------------------------

    // What a lane reads of an image: its bytes at their addresses, where it is loaded, where its code lies, and its writable data.
    struct Image {
        std::vector<Region> regions;
        std::uint32_t base = 0;
        Span code;
        std::vector<Span> writable;
        std::vector<std::uint32_t> functions;   // the function starts the image states (its .pdata and the targets of its calls)
    };
    using ReadImage = std::function<Image(const std::filesystem::path&)>;

    struct Services {
        decompile::c_text::Services& decompiler;
        build::functions::FunctionBuilder& builder;
        build::functions::PowerPcTools tools;
        ReadImage read_image;
        emulate::pcode::LifterSet* lifters = nullptr;                  // none: inputs are guessed, not found path by path
        lane_common::FingerprintStore fingerprints;                    // unset: nothing is kept
        std::function<std::optional<std::string>(const std::filesystem::path&)> content_sha;   // the corpus's SHA-256
    };

    explicit PowerPcLane(Services services) : services_(std::move(services)) {}

    const std::string& name() const override;
    // The digest covers the revision, the comparer's, the compiler's options, the fixed inputs and the exploration's limits.
    Profile profile(const std::filesystem::path& module_path) const override;
    // The decompiled module made compilable. A preparation carries its ModulePPC to build().
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    // Compile every function, link one image, and place it beside the original. The functions the compiler rejected are `missing`.
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    struct PreparedPowerPc final : Prepared {
        decompile::c_text::ModulePPC module;
        void set_text(const std::string& name, std::string text) override {
            module.made.set_text(name, text);         // (the build reads the module's)
            texts[name] = std::move(text);
        }
    };

    // What the reconstruction path is told: a verdict is per function.
    static reconstruct::KindTraits traits();

private:
    Services services_;
};

}  // namespace cb::verify::lanes
