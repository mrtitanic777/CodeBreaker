// exact/emu/native_dos_executive.hpp -- CodeBreaker's own DOS executive (exact/emu/dosexec, linked in, on Unicorn) behind
// the interface the assemblers and compilers take (build/native_dos), and the DosRunner every DOS toolchain is given.
//
//     const NativeDosExecutive dos(config);
//     std::unique_ptr<build::DosRunner> runner = dos_runner(config, dos);
//
// The runner is build::NativeDosRunner on this executive: a run the executive cannot complete has `ok` false and says why.
#pragma once

#include <filesystem>
#include <memory>
#include <mutex>

#include "base/config.hpp"
#include "build/dos_run.hpp"
#include "build/native_dos.hpp"
#include "exact/emu/native_dosexec.hpp"

namespace cb::exact::emu {

class NativeDosExecutive final : public build::native_dos::DosExecutive {
public:
    explicit NativeDosExecutive(const config::Config& config);
    bool available() const override { return opened_.usable(); }
    build::native_dos::Exit run(const std::filesystem::path& exe, const std::filesystem::path& work, std::string_view cmdline) const override;
    build::native_dos::MemoryResult run_dpmi(const build::native_dos::MemoryRun& run) const override;
    build::native_dos::SessionExit run_session(const build::native_dos::SessionRun& run) const override;

private:
    const dos::Executive& executive() const;
    native_dosexec::Opened opened_;
    std::filesystem::path work_;
    mutable std::mutex one_at_a_time_;   // (the executive runs one program at a time: a compile batch's threads take turns)
};

// The DosRunner for the DOS toolchains: the executive. `dos` must outlive the runner.
std::unique_ptr<build::DosRunner> dos_runner(const config::Config& config, const build::native_dos::DosExecutive& dos);

}  // namespace cb::exact::emu
