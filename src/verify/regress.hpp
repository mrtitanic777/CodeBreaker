// verify/regress.hpp -- Logan's milestone regression, run for real: `codebreaker regress <all15|setup|slow>`.
//
// What the quick gate does not cover, and what is run before a milestone:
//
//   all15   the 15 modules of Setup's MINI.CAB (KRNL386, GDI, USER, the drivers and DLLs) rebuilt from source, put on
//           Setup's CD in place of the originals, and Windows 95 Setup booted in QEMU to its Welcome dialog and pressed
//           Continue (verify/gate16/setup16 continue_test). PASS = the License Agreement page, in every run: each run
//           is a fresh CD and a fresh copy of the prepared disk, and the regression's rule is two runs.
//   setup   the same with Setup's own seven 16-bit programs (SUWIN, SETUPX, SYSDETMG, NETDI, SUEXPAND, W95INF16,
//           SETUP.EXE) rebuilt as well -- all assembly, as the chain was proved (no gate) -- and put in their cabinets.
//   install the whole install, to the desktop: all 22 modules ours (the 15 of MINI.CAB, Setup's seven), Setup's wizard to Start Copying,
//           the copy, Finish, FIX95CPU's disk (a host this fast fails the first restart at IOS, whatever the modules), the restart, and
//           Windows 95 finishing its setup (verify/gate16/install16). PASS = the desktop on the screen. An install is an hour or more of
//           machine time, so one run is the default (--runs 2 for the milestone's rule); the last screen of each stage is kept in
//           <state>/phase3/shots/<tag>-<stage>.png.
//   slow    the commands of a cases file (tests/regress/slow.json) run through the codebreaker program: the Xbox 360
//           recompile of Tetris, Minecraft and Up (`recompile-project --check`) and whatever else is slow enough to wait
//           for a milestone. A case passes when the exit code is the one it expects and its output has the lines it names.
//
// The rules of the machine room are kept here: at most 3 machines at once (`--vms`, default 1), the build's gate
// workers are the settings' (at most 4), only the machines this run started are stopped (setup16 starts each in a job
// of its own: no process tree is ever walked), the run's copies -- 650 MB of disk and a CD per run, the built images --
// are deleted when it ends, and nothing starts with less than 30 GB free on the state drive.
//
//     regress::Report r = regress::run(regress::Tier::All15, services, options);
//     cb::out("{}", r.text());
//     return r.pass ? 0 : 1;
//
// `Services` holds what the run needs from the world (the builder, the machine, the disk); production_services() builds
// them from the settings, and a test gives fakes.
#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/cli.hpp"
#include "verify/gate16/install16.hpp"
#include "verify/gate16/setup16.hpp"

namespace cb::verify::regress {

namespace setup16 = cb::verify::gate16::setup16;
namespace install16 = cb::verify::gate16::install16;

enum class Tier { All15, Setup, Install, Slow };

// "all15", "setup", "install", "slow".
std::string_view word(Tier tier);
std::optional<Tier> tier_named(std::string_view name);

// MINI.CAB's modules (state/mini/original), and Setup's own programs (state/setup/original), upper case.
const std::vector<std::string>& all15_names();
const std::vector<std::string>& setup_names();

// ---- what a run found --------------------------------------------------------------------------------------------------

// One module's build.
struct Built {
    std::string name;                      // upper case: the name Setup's CD knows it by
    bool linked = false;
    std::filesystem::path image;           // the linked file (in the run's own folder; gone with the run)
    std::string error;
    std::size_t functions = 0;             // in the module
    std::size_t with_text = 0;             // given C by the decompiler
    std::size_t assembled = 0;             // C rejected by the compiler, written as its own instructions
    std::size_t rejected = 0;              // stand-ins
    std::size_t gated = 0;                 // C the gate found different from the original: rebuilt from instructions
    std::chrono::seconds took{0};
};

struct Boot {
    std::string tag;
    std::string verdict = "SLOW";          // setup16's or install16's word: PASS, FAIL(hang), FAIL(stuck) ...
    bool pass = false;
    std::filesystem::path screenshot;      // the last screen (kept: <state>/phase3/shots/<tag>.png)
    std::chrono::seconds took{0};
    std::vector<std::string> log;          // install: one line a stage
};

struct Case {
    std::string name;
    std::string status;                    // "PASS", "FAIL" or "SKIPPED"
    std::string why;                       // what failed, or why it was skipped
    std::chrono::seconds took{0};
};

struct Report {
    Tier tier = Tier::All15;
    bool pass = false;
    std::string refused;                   // why the run did not start (disk, tools): nothing was run
    std::vector<Built> built;
    std::vector<Boot> boots;
    std::vector<Case> cases;
    std::chrono::seconds took{0};
    std::vector<std::string> notes;        // what is worth a look but is not a failure
    // The report as lines of text: one row per module built, per boot, per case, then the verdict.
    std::string text() const;
};

// ---- what the run is told ----------------------------------------------------------------------------------------------

struct Options {
    std::optional<std::size_t> runs;       // boots, each from a fresh CD and disk (default 2; for install, 1)
    std::size_t vms = 1;                   // boots at once; at most 3
    std::vector<std::string> only;         // rebuild only these (the others stay the originals): a quick look, not the regression
    std::optional<std::filesystem::path> keep_built;   // put the built images here and keep them
    double min_free_gb = 30;
    std::string tag = "regress";
    std::filesystem::path cases;           // slow: the cases file
    std::optional<std::filesystem::path> exe;   // slow: the codebreaker program the cases run
    std::chrono::minutes case_timeout{90};
    // Off by default (`--reuse`, or the setting regress_reuse). A boot tier whose inputs are byte-identical to those of a run that passed -- the
    // built images, the disk and the CD, the floppies, the emulator and its settings, this program -- is a pass without starting a machine
    // (the report says so). Only a pass is written down; a failure is always run again.
    bool reuse = false;
};

constexpr std::size_t kMaxVms = 3;

// What the run needs from the world.
struct Services {
    // Build `names` (modules of `originals`) side by side; the image of each in `outdir/<NAME>/`. `all_assembly`: build with
    // every function written as its instructions and no gate (Setup's own programs).
    std::function<std::vector<Built>(const std::vector<std::string>& names, const std::filesystem::path& originals,
                                     const std::filesystem::path& outdir, bool all_assembly)> build;
    // Boot Setup with these files in place of its own, and press Continue.
    std::function<setup16::Outcome(const setup16::Replace& replace, const std::string& tag)> boot;
    // install: boot Setup with these files in place of its own, and drive it to the desktop (FIX95CPU's disk and all).
    std::function<install16::Outcome(const setup16::Replace& replace, const std::string& tag)> install;
    // Free space of the drive the state folder is on, in GB.
    std::function<double()> free_gb;
    // What a tier needs and this machine lacks (the emulator, the Setup CD, the prepared disk, the originals): one line each,
    // empty when everything is there. Nothing is run when something is missing.
    std::function<std::vector<std::string>(Tier tier)> missing;
    // Where the run's own folder is made (the built images; gone at the end).
    std::filesystem::path work;
    // Where the run's locks are made (empty: no locks). Runs on one checkout share the module build folders (state/build/<name>) and a
    // boot's tag names its disk and screenshots: a run takes the build lock while it builds, and a tag's lock while that boot runs, and
    // waits ("waiting for regress run pid N") when another run has them. The locks are the system's: a killed run's goes with it.
    std::filesystem::path lock_dir;
    std::filesystem::path mini_original, setup_original;
    // reuse: a text that names every input of a tier's boots (equal text: byte-identical inputs); empty when it cannot be made. And the folder the
    // passing runs are written down in (empty: no reuse).
    std::function<std::string(Tier tier, const setup16::Replace& replace)> inputs_key;
    std::filesystem::path passed_dir;
    // slow: run `argv` with `exe`; the exit code and the output.
    std::function<std::pair<int, std::string>(const std::vector<std::string>& argv, std::chrono::minutes timeout)> run_exe;
    // slow: the path a case names by setting (`package_setting`), nullopt when it is not set.
    std::function<std::optional<std::filesystem::path>(const std::string& setting)> setting_path;
};

// Run the tier. UsageError for a wrong option (vms > 3, a name not of the tier).
Report run(Tier tier, const Services& services, const Options& options);

// The services over the settings: the lane kit's module builder, setup16 with the QEMU and the Setup CD of the settings.
Services production_services(const config::Config& config);

// `regress <all15|setup|slow>`.
cli::Command command(const config::Config& config);

}  // namespace cb::verify::regress
