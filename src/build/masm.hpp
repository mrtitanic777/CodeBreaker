// build/masm.hpp -- assemble reconstructed assembly with the period MASM.
//
//     codebreaker masm [file.asm] [--selftest]
//
//     masm::Assembler assembler(masm::Context{tools, options, dos, runner, &cache});
//     dos_sessions::Product p = assembler.assemble(source);                       // .obj, .lst, .log
//     dos_sessions::Finished many = assembler.assemble_many({{"F0", src0}, {"F1", src1}});
//     auto functions = assembler.functions(source);                               // {name: bytes, fix-up mask}
//
// Much of KERNEL/USER/GDI was hand-written assembly, and assembly can only be proven byte-exact by RE-ASSEMBLING it and
// diffing the bytes: translating it to C never reproduces the original encoding. MASM is a 16-bit DOS program (there is no
// NTVDM on 64-bit Windows), so this runs it on CodeBreaker's own emulator (build/native_dos), which is proven
// byte-identical to DOSBox for MASM and about 20 times faster, and falls back to DOSBox-X when the native run cannot
// complete (`CB_NATIVE_DOS` forces one or the other). MASM emits an OMF `.obj` exactly like the C compilers, so everything
// downstream is shared: formats/omf pulls the code bytes and the relocation mask. This module only runs the assembler.
//
// `masm_root` selects the assembler: the default is MASM 4.00 (16-bit, proven byte-exact for the NE core); an override such
// as the vendored MASM 5.10A (32-bit `.386` / USE32) is what the PE path needs.
//
// The DOSBox side. A session boots DOSBox once and assembles batch after batch (booting is the dominant cost), so an
// Assembler keeps a pool of persistent sessions per assembler folder, at most `Options::pool_max` of them -- a dozen
// DOSBoxes saturate every core and starve the very MASM they run. A call with more sources than `Options::batch` is cut into
// batches (one dispatch holds only so many commands) and the batches run side by side, each on a session of its own. With
// the warm pool off, or when a session dies, every batch boots DOSBox for itself.
//
// Assembling is deterministic -- the same source, assembler and INCLUDE/LIB files give the same bytes -- so an Assembler
// given a cache keeps what it made (kind "masm", keyed by the source, the stem and the bytes of every file under the
// assembler's folder), and a call whose sources are all kept starts no emulator at all.
//
// Nothing here is global: the tools, the options, the emulators and the cache belong to the caller; the pools belong to
// the Assembler and are closed with it.
#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/cache.hpp"
#include "base/config.hpp"
#include "build/dos_run.hpp"
#include "build/dos_sessions.hpp"
#include "build/native_dos.hpp"
#include "core/cli.hpp"
#include "core/ordered_map.hpp"
#include "formats/omf.hpp"

namespace cb::build::masm {

// Where the assembler and the emulator are (settings.json: masm, dosbox_x).
struct Tools {
    std::filesystem::path masm;       // the default assembler's folder: BIN/MASM.EXE, INC, LIB
    std::filesystem::path dosbox_x;   // the emulator's program; empty: none
    bool dosbox_forbidden = false;    // CB_NO_DOSBOX: no DOSBox is ever started (the byte-match spot-check is optional)

    static Tools from_config(const config::Config& config);
};

// How the DOSBox side runs. The environment names are the ones the program has always read.
struct Options {
    std::string dosbox_core = "dynamic";   // `normal` is the safe interpreter, `dynamic` JITs the emulated x86 (much
                                           // faster, validated byte-identical to `normal`): WT_DOSBOX_CORE
    std::size_t pool_max = 3;              // persistent DOSBoxes PER assembler folder: WT_POOL_MAX
    bool warm_pool = true;                 // false: every batch boots its own DOSBox: WT_NO_WARM_POOL=1
    std::size_t worker_max_uses = 0;       // retire a session after this many batches; 0 = never: WT_WORKER_MAX_USES
                                           // (retiring a session boots another, whose first dispatch tends to flake)
    std::size_t batch = 40;                // sources per dispatch: WT_ASM_BATCH (the cold boot's autoexec holds only so many)

    static Options from_environment();
};

// What an Assembler needs: the tools, the options, the two ways to run DOS programs (the native executive, and the DOSBox
// runner for the fallback when no warm session can be had), and (optional) the cache.
struct Context {
    Tools tools;
    Options options;
    const native_dos::DosExecutive& dos;
    const DosRunner& runner;
    base::cache::Cache* cache = nullptr;
    // How a pool of warm sessions is made for an assembler folder (DOSBox sessions over `tools.dosbox_x` when empty). Tests
    // give sessions of their own.
    std::function<std::unique_ptr<dos_sessions::WorkerPool>(const std::filesystem::path& masm_root)> make_pool;
};

class Assembler {
public:
    explicit Assembler(Context context);
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;
    ~Assembler();   // closes the warm sessions

    // Close the warm sessions now (the next batch boots new ones).
    void close_sessions();

    // `source` assembled: the object, the listing and the log MASM wrote (a part it did not make is nullopt). `root` replaces
    // the default assembler folder. Falls back to DOSBox when the native run cannot complete. NotFoundError when MASM.EXE is
    // not under the folder; ToolError when DOSBox is needed and cannot be had (not set up, or CB_NO_DOSBOX).
    dos_sessions::Product assemble(std::string_view source, std::string_view stem = "t",
                                   std::chrono::milliseconds timeout = std::chrono::seconds(180),
                                   const std::optional<std::filesystem::path>& root = std::nullopt);

    // Many (stem, source) pairs, using the cache: a source already kept is served at once, only the rest is assembled (and
    // then kept). The results are in the order the sources were given. Stems must be unique (UsageError otherwise) and DOS
    // 8.3-safe (callers use F0..FN). `timeout` is for one DOSBox dispatch; the default is 120 s and 3 s per source.
    dos_sessions::Finished assemble_many(std::span<const dos_sessions::Source> items,
                                         std::optional<std::chrono::milliseconds> timeout = std::nullopt,
                                         const std::optional<std::filesystem::path>& root = std::nullopt);

    // The public functions of `source`'s object with their bytes and fix-up masks (formats/omf). ToolError when MASM made no
    // object (its log, cut to 600 characters, is in the message).
    OrderedMap<std::string, formats::omf::FunctionBytes> functions(std::string_view source);

private:
    struct Pools;

    dos_sessions::Finished assemble_uncached(std::span<const dos_sessions::Source> items,
                                             std::optional<std::chrono::milliseconds> timeout,
                                             const std::filesystem::path& root);
    dos_sessions::Finished assemble_with_dosbox(std::span<const dos_sessions::Source> items,
                                                std::optional<std::chrono::milliseconds> timeout,
                                                const std::filesystem::path& root);
    dos_sessions::Finished assemble_batch(std::span<const dos_sessions::Source> items,
                                          std::optional<std::chrono::milliseconds> timeout,
                                          const std::filesystem::path& root);
    dos_sessions::Finished cold_boot(std::span<const dos_sessions::Source> items,
                                     std::optional<std::chrono::milliseconds> timeout,
                                     const std::filesystem::path& root);
    void require_dosbox() const;

    Context context_;
    std::unique_ptr<Pools> pools_;
};

// MASM's own selftest program: a proc `_addasm` that pushes bp, adds two arguments and returns.
extern const std::string_view kSelfTestSource;

// Assemble kSelfTestSource and check the bytes of `_addasm`; the account of it goes to `report`. True when it is byte-exact.
bool selftest(Assembler& assembler, std::string& report);

// `masm [file.asm] [--selftest]`: assemble a file and list the segments of its object, or run the selftest. Exit code 1
// when nothing was made.
cli::Command command(const Context& context);

}  // namespace cb::build::masm
