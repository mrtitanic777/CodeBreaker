// cb/build/dos_tools.hpp -- run DOS programs (the assemblers, compilers and linkers of the 1980s) in DOSBox-X.
//
//     dos_tools::Emulator emulator = dos_tools::Emulator::from_config(config);
//     dos_tools::DosboxRunner runner(emulator);
//     DosRunResult r = runner.run(spec);                  // see cb/build/dos_run.hpp
//
// The runner mounts each folder of the spec as a drive (the work folder is d: unless the spec names it; the last
// drive listed is the current one), writes the commands to BLD.BAT, runs them and reads the named files back. `ok`
// is false -- and nothing is pretended -- when the emulator is not set up or a file was not made. The emulator
// runs without a window, under a Job Object of ours: a run that outlasts its timeout is ended, and so is its DOSBox
// should we exit first.
//
// MASM sources are assembled by cb/build/masm (an Assembler over the native DOS executive, with warm DOSBox
// sessions); this module is only the general runner the compilers (msvc152, period16, msc6) share.
#pragma once

#include <filesystem>
#include <optional>

#include "cb/base/config.hpp"
#include "cb/build/dos_run.hpp"

namespace cb::build::dos_tools {

// Where DOSBox-X is (settings.json: dosbox_x).
struct Emulator {
    std::filesystem::path dosbox_x;   // the program; empty: none set up
    bool forbidden = false;           // CB_NO_DOSBOX: no DOSBox is ever started (the byte-match spot-check is optional)

    static Emulator from_config(const config::Config& config);

    // The program to run: dosbox_x when it names a file and DOSBox is not forbidden, else nullopt.
    std::optional<std::filesystem::path> program() const;
};

// The folder settings.json "masm" names when it holds BIN/MASM.EXE, else nullopt.
std::optional<std::filesystem::path> masm_root(const config::Config& config);

class DosboxRunner final : public DosRunner {
public:
    explicit DosboxRunner(Emulator emulator) : emulator_(std::move(emulator)) {}

    // ok is false, with no work folder, when no emulator is set up; false with the work folder when DOSBox could not
    // be started, ran past the timeout (it is ended), or did not make every artifact. The work folder is kept.
    DosRunResult run(const DosRunSpec& spec) const override;

private:
    Emulator emulator_;
};

// The text of BLD.BAT and r.conf for a spec run in `work` (exposed for the tests: they are the contract with DOSBox).
std::string batch_text(const DosRunSpec& spec);
std::string dosbox_conf(const DosRunSpec& spec, const std::filesystem::path& work);

}  // namespace cb::build::dos_tools
