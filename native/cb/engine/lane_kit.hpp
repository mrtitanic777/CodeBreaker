// cb/engine/lane_kit.hpp -- the lanes of the reconstruction path, wired from CodeBreaker's own pieces.
//
//     LaneKit kit(config::Config::load(checkout));
//     verify::reconstruct::Kinds kinds;
//     kit.add_lanes(kinds);                                       // "pe", "ne", "le" -- each a verify/lanes Kind
//     verify::reconstruct::Reconstructor rec(kit.reconstruct_services(kinds, store));
//     rec.run("lz32.dll");                                        // decompile, build, judge, record -- the results are in `store`
//
//     build::module::ModuleBuilder builder(kit.module_services());    // module build over the same decompiler, tools and gate
//     verify::gate16::Isolated16 gate(kit.isolated16_spec());          // the 16-bit gate over the same emulators
//
// The lanes, the reconstructor, the module builder and the gate are each given what they need (their `Services`); nothing in them looks
// for a thing in the world. This is where the application builds those things from the settings -- the corpus, the cache, the knowledge
// base, the decompiler and what it asks of the symbol tables and the analyses, OpenWatcom and the other tools, the emulators and the
// p-code lifters -- and hands each one to whoever asks. One kit is one process's surroundings: its parts are made when first used, and
// share each other (one decompiler, one function builder, one knowledge base).
//
// What a lane's tools need that is particular to a module (the imports a 32-bit build binds, by the number the module calls them) is
// kept here as the decompiler describes each module (Environment::described) and given to the lane's build through
// `Win32Lane::Services::import_lines_for`.
//
// Wired besides: "dos" and the machines' lanes (lane_kit_machines).
// Not wired: the Java lane (the transpiler's Services have no production assembly yet): `add_lanes` leaves "java" out and `java_missing()` says why.
// The PowerPC lane ("powerpc") is wired when the Xbox 360 compiler is set up (settings powerpc_tools, powerpc_lib).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/build/functions.hpp"
#include "cb/build/module.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/jobs/workers.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/verify/gate16/isolated16.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/reconstruct.hpp"

namespace cb::engine {

class LaneKit {
public:
    // The settings (the checkout, the state folder, where each tool is). The corpus is the configuration's products unless given.
    explicit LaneKit(config::Config config, std::shared_ptr<base::Corpus> corpus = nullptr);
    ~LaneKit();
    LaneKit(const LaneKit&) = delete;
    LaneKit& operator=(const LaneKit&) = delete;

    // ---- the parts ------------------------------------------------------------------------------------------------------
    const config::Config& config() const;
    base::Corpus& corpus();
    base::cache::Cache& cache();
    knowledge::Knowledge& knowledge();
    decompile::decompiler::Decompiler& decompiler();
    build::functions::FunctionBuilder& builder();
    // The lifters of the path explorer; null when the program is not where the settings say.
    emulate::pcode::LifterSet* lifters();
    // Where a run keeps its work (build/watcom::scratch_dir()).
    std::filesystem::path scratch() const;

    // ---- the lanes ------------------------------------------------------------------------------------------------------
    // Each lane by the name results are filed under; null when its tools are not set up on this machine (a lane's profile says so too).
    std::shared_ptr<const verify::lanes::Kind> win32_lane();       // "pe"
    std::shared_ptr<const verify::lanes::Kind> win16_lane();       // "ne"
    std::shared_ptr<const verify::lanes::Kind> driver_lane();      // "le"
    std::shared_ptr<const verify::lanes::Kind> powerpc_lane();     // "powerpc": the Xbox 360 compiler (settings powerpc_tools, powerpc_lib) is set up
    std::shared_ptr<const verify::lanes::Kind> java_lane();        // "java"
    std::shared_ptr<const verify::lanes::Kind> dos_lane();         // "dos": real-mode programs, written back as assembly and compared whole; null without OpenWatcom's wasm and wlink
    // "nes", "sms", "gb", "genesis", "zx", "msx": a cartridge, snapshot or tape written back by its machine's writer, rebuilt, and run beside the original's; null for another name.
    std::shared_ptr<const verify::lanes::Kind> machine_lane(std::string_view name);
    // Adds every lane that can be made, with the traits the reconstruction path is told of it.
    void add_lanes(verify::reconstruct::Kinds& kinds);
    // Why the Java lane is not added.
    static std::string_view java_missing();

    // ---- what the path is given -----------------------------------------------------------------------------------------
    // reconstruct's services over the corpus and `store`. Judging runs on threads (the budget of the machine).
    verify::reconstruct::Services reconstruct_services(const verify::reconstruct::Kinds& kinds, FunctionStore& store);
    // Services of the module builder (build/module): the same decompiler, function builder, emulators and gate.
    build::module::Services module_services();
    // The 16-bit gate's spec (verify/gate16/isolated16).
    verify::gate16::Isolated16Spec isolated16_spec();
    // What the gate needs of a 16-bit module's decompiled form: how each function is called, which return four bytes, where each is.
    verify::gate16::IsolatedTables gate_tables(const std::filesystem::path& original);

    // ---- worker processes -----------------------------------------------------------------------------------------------
    // Unicorn is one lock per process, so judging many functions on threads uses about one core: with a launcher the names to judge are shared out over
    // worker processes of the host program (and a module's build can be made in one). The host registers the kinds with `register_worker_kinds` at the
    // top of main (jobs::run_as_worker) and gives the kit its launcher; without one everything runs in this process.
    static void register_worker_kinds(jobs::WorkerKinds& kinds);
    void set_worker_launcher(jobs::WorkerLauncher launcher);
    // The lane filed under `name` ("pe", "ne", "le", "powerpc", "dos", "java", the machines); null when there is none or its tools are not set up.
    std::shared_ptr<const verify::lanes::Kind> lane_named(std::string_view name);
    // `Services::judge_batch` of reconstruct: nullopt (threads judge) without a launcher or for a share too small to be worth a process.
    std::optional<std::map<std::string, verify::Judgement>> judge_apart(const verify::reconstruct::Services::JudgeBatch& batch);
    // `Services::build_apart` of the module builder.
    std::optional<build::module::Built> build_apart(const std::string& module, const std::optional<std::filesystem::path>& outdir, bool call);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// ---- the host program ------------------------------------------------------------------------------------------------
// What a program that hosts the kit's workers does first in main (codebreaker.exe adds these two lines at the top of its main):
//
//     #include "cb/engine/lane_kit.hpp"
//     if (const int rc = cb::engine::host_main_hook(argc, argv); rc >= 0) return rc;
//
// When the program was started as a worker (`--cb-worker <kind> ...`) it runs that share and returns the exit code (>= 0); otherwise it returns -1
// and the program goes on, now known to be a host: `hosting_workers()` is true, and the pipeline commands (source/pipeline) share judging and
// building out over worker processes of it without their Environment saying so.
int host_main_hook(int argc, char** argv);
bool hosting_workers();

}  // namespace cb::engine
