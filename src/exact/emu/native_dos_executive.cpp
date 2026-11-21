// CodeBreaker's own DOS executive behind build/native_dos, and the toolchains' DosRunner (native_dos_executive.hpp).
#include "exact/emu/native_dos_executive.hpp"

#include "build/native_dos_runner.hpp"
#include "core/error.hpp"
#include "core/text.hpp"
#include "exact/emu/dos.hpp"

namespace cb::exact::emu {

namespace stdfs = std::filesystem;

// (a session's program may take minutes -- a compiler pass over a large file -- where a probe takes seconds)
NativeDosExecutive::NativeDosExecutive(const config::Config& config)
    : opened_(native_dosexec::open(native_dosexec::locate(config), native_dosexec::limits_from_environment(600))),
      work_(config.state() / "native-dos-mem") {}

const dos::Executive& NativeDosExecutive::executive() const {   // (callers hold one_at_a_time_)
    if (!opened_.usable()) throw build::native_dos::NativeDosError("the DOS executive is not available here");
    return *opened_.executive;
}

build::native_dos::Exit NativeDosExecutive::run(const stdfs::path& exe, const stdfs::path& work, std::string_view cmdline) const {
    dos::RunSpec spec;
    spec.exe = exe;
    spec.workdir = work;
    spec.command_tail = std::string(cmdline);
    spec.environment = {'P', 'A', 'T', 'H', '=', 0};
    spec.fast = true;
    const std::lock_guard hold(one_at_a_time_);
    const dos::Result r = executive().run(spec);
    build::native_dos::Exit out;
    if (r.exit) out.code = static_cast<std::int32_t>(*r.exit);
    out.console = r.console;
    return out;
}

build::native_dos::MemoryResult NativeDosExecutive::run_dpmi(const build::native_dos::MemoryRun& run) const {
    dos::MemFS files;   // a name only: nothing is written there
    for (const auto& [name, data] : run.files) files.put(work_ / name, data);
    dos::RunSpec spec;
    spec.exe = work_ / run.exe;
    spec.workdir = work_;
    spec.command_tail = run.cmdline;
    spec.environment = run.env;
    spec.fast = true;
    spec.dpmi = true;
    spec.files = &files;
    const std::lock_guard hold(one_at_a_time_);
    const dos::Result r = executive().run(spec);
    build::native_dos::MemoryResult out;
    for (const std::string& name : run.artifacts) {
        const stdfs::path at = work_ / text::upper(name);
        if (files.is_file(at) && files.files().contains(dos::MemFS::key(at))) out.made[name] = files.read(at);
    }
    out.console = r.console;
    return out;
}

build::native_dos::SessionExit NativeDosExecutive::run_session(const build::native_dos::SessionRun& run) const {
    dos::RunSpec spec;
    spec.exe = run.exe;
    const auto current = run.drives.find(run.drive);
    spec.workdir = current != run.drives.end() ? current->second : run.exe.parent_path();
    spec.command_tail = run.cmdline;
    spec.environment = run.env;
    spec.fast = true;
    spec.dpmi = run.dpmi;
    spec.drives = run.drives;
    spec.drive = run.drive - 'A';
    try {
        const std::lock_guard hold(one_at_a_time_);
    const dos::Result r = executive().run(spec);
        build::native_dos::SessionExit out;
        if (r.exit) out.code = static_cast<std::int32_t>(*r.exit);
        else throw build::native_dos::NativeDosError(run.exe.filename().string() + ": the program did not end");
        out.console = r.console;
        out.standard_out = r.standard_out;
        return out;
    } catch (const build::native_dos::NativeDosError&) {
        throw;
    } catch (const Error& e) {   // (a service the executive lacks, a CPU fault, a limit: not this program's answer)
        throw build::native_dos::NativeDosError(run.exe.filename().string() + ": " + e.what());
    }
}

std::unique_ptr<build::DosRunner> dos_runner(const config::Config&, const build::native_dos::DosExecutive& dos) {
    return std::make_unique<build::NativeDosRunner>(dos);
}

}  // namespace cb::exact::emu
