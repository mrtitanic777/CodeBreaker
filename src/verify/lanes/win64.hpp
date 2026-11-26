// verify/lanes/win64.hpp -- what is particular to reconstructing an x64 Windows module.
//
// The module is decompiled and made compilable (decompile/c_text_x64: the runtime library's functions named and left to the
// original), every function is compiled with Visual C++'s x64 compiler and one DLL linked (build/functions), and each function
// is run beside the original in Unicorn's x64.
//
// Both sides run in one memory: the original image at its own base, the rebuilt DLL above it. What the rebuilt C reaches that it
// does not have itself -- a global, an import, a function of the runtime library -- is the original's at the original's address,
// so both sides read and write the same globals and call the same imports. Every import slot of the original holds a landing
// address of its own; a call that lands there is recorded (its name and as many arguments as the Windows SDK declares it with)
// and answered with the input's stand-in result.
//
// The Windows x64 convention: the first four arguments in RCX, RDX, R8 and R9, the rest on the stack above 32 bytes the caller
// keeps for them. Compared: how the run ends, the result in RAX (not for a function that returns nothing), the import calls in
// order, the module's writable data and the memory behind pointer arguments as the run left them, how far the stack moved, and
// whether RBX, RBP, RSI, RDI and R12-R15 were handed back. A pointer into the stack is compared as "stack": the two frames are
// laid out differently.
//
// What the lane needs of its surroundings is given to it (`Services`): the decompiler's services, the function builder and the
// x64 tools, how many arguments an import takes, and where fingerprints are kept. A `Kind` (kind.hpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "build/functions.hpp"
#include "decompile/c_text.hpp"
#include "verify/compare.hpp"
#include "verify/lane_common.hpp"
#include "verify/lanes/kind.hpp"
#include "verify/reconstruct.hpp"

namespace cb::verify::lanes {

class Win64Lane final : public Kind {
public:
    // Bump when what a verdict depends on changes.
    static constexpr int kRev = 6;   // 5: inputs aimed at the constants the code compares with; scratch memory of pointers. 6: one function at a time   // 2: a fault is one ending, whichever access faulted. 3: calls to short addresses, undeclared calls refused
    static constexpr std::size_t kArguments = 6;                              // RCX, RDX, R8, R9 and two on the stack
    static constexpr std::uint64_t kStackBase = 0x10000000, kStackSize = 0x100000;
    static constexpr std::uint64_t kScratchBase = 0x20000000, kScratchSize = 0x10000;   // where pointer arguments are aimed
    static constexpr std::uint64_t kLandingBase = 0x7FF000000000;             // one 16-byte landing per import slot
    static constexpr std::uint64_t kReturnAddress = 0x7FFE00000000;           // the run ends when it gets here
    static constexpr std::uint64_t kMaxSteps = 200000;                        // instructions a run may take
    static constexpr std::uint64_t kRebuiltGap = 0x10000000;                  // the rebuilt DLL sits at least this far above the original

    // An input: the six arguments and what imports return (compare::Input), and what the scratch memory holds: bytes counting up, or
    // (`pointers`) every 8-byte word the address of another place in it -- so code that follows a pointer it read can go on.
    struct Vector : compare::Input {
        bool pointers = false;
    };
    // The inputs every function is run on: small numbers, edge values, and pointers into the scratch memory (whose bytes count up).
    static std::vector<Vector> vectors();
    // Inputs aimed at what the function's own code tests: the constants it compares values with (`cmp`, `sub` with an immediate in
    // `code`, its bytes at `va`), each given in every argument and as what imports return, and beside pointers. At most `most`.
    static std::vector<Vector> directed(ByteView code, std::uint64_t va, std::size_t most = 6);

    // An image in memory: its extent, its sections' bytes, which of them the code writes, and its import slots.
    struct Image {
        std::uint64_t base = 0, end = 0;
        struct Region {
            std::uint64_t va = 0;
            Bytes bytes;
            bool writable = false;
        };
        std::vector<Region> regions;
        std::map<std::uint64_t, std::string> slots;   // import slot -> the import's name
    };
    // An x64 PE as the lane runs it (FormatError for a file that is not one).
    static Image read_image(const std::filesystem::path& path);

    // What judging a function needs of the whole build.
    struct Context {
        Image original;
        std::optional<Image> rebuilt;
        std::map<std::string, int> argc;               // {import: the arguments the Windows SDK declares it with}
        // {where a rebuilt function starts: where the original's does}: judging one function, every other rebuilt one is a jump to the
        // original's, so the verdict is on the function's own C (a wrong callee does not sink its callers)
        std::map<std::uint64_t, std::uint64_t> originals;
        std::optional<std::string> sha;                // the original's content hash: what its fingerprints are filed under
        lane_common::FingerprintStore fingerprints;
    };
    // One function to judge.
    struct Item {
        std::string key;
        std::uint64_t va = 0, end = 0;                 // [its start, the next function's start)
        std::uint64_t rebuilt_va = 0;
        bool novalue = false;                          // it returns nothing
    };

    // What one run of `entry` did on input `v`: with the rebuilt DLL in memory too (`rebuilt`: every rebuilt function but `entry`
    // jumping to the original's), and the original's own instructions counted (`own`, for the original's side).
    static compare::Fingerprint fingerprint(const Context& context, std::uint64_t entry, const Vector& v, bool rebuilt, bool novalue,
                                            std::optional<std::pair<std::uint64_t, std::uint64_t>> own = std::nullopt);
    // The verdict of one function. Safe to call from several threads at once.
    static Judgement judge_function(const Context& context, const Item& item);

    struct Services {
        decompile::c_text::Services& decompiler;
        build::functions::FunctionBuilder& builder;
        build::functions::X64Tools tools;
        std::function<std::optional<int>(std::string_view import)> import_argc;   // none: an import's call is compared by its name
        lane_common::FingerprintStore fingerprints;
        std::function<std::optional<std::string>(const std::filesystem::path&)> content_sha;
    };

    // Visual C++'s x64 compiler and linker on this computer: the newest Hostx64/x64 of a Visual Studio under `roots` (default: the
    // standard places); nullopt when none is installed.
    static std::optional<build::functions::X64Tools> find_tools(const std::vector<std::filesystem::path>& roots = {});

    explicit Win64Lane(Services services) : services_(std::move(services)) {}

    const std::string& name() const override;
    Profile profile(const std::filesystem::path& module_path) const override;
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    struct PreparedX64 final : Prepared {
        decompile::c_text::ModuleX64 module;
        void set_text(const std::string& name, std::string text) override {
            module.made.set_text(name, text);
            texts[name] = std::move(text);
        }
    };

    static reconstruct::KindTraits traits();

private:
    Services services_;
};

}  // namespace cb::verify::lanes
