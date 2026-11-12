// codebreaker/build/dos_runner_adapter.hpp -- SHIM support (docs/CPP_DESIGN.md 6.3): the DOSBox-X runner of cb/build/dos_tools made
// from the old settings (settings.json: dosbox_x), so the staged shims of the toolchain drivers (msvc152, period16, masm) can run DOS
// programs without the application's configuration. COMPAT(py): remove with the legacy settings.
#pragma once

#include "cb/build/dos_tools.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"

namespace codebreaker::build {

class OldDosRunner final : public cb::build::DosRunner {
public:
    OldDosRunner() : runner_(emulator()) {}
    cb::build::DosRunResult run(const cb::build::DosRunSpec& spec) const override { return runner_.run(spec); }

private:
    static cb::build::dos_tools::Emulator emulator() {
        cb::build::dos_tools::Emulator e;
        e.dosbox_x = cb::compat::legacy::path_of(py::settings::get_str(py::str("dosbox_x")));
        const auto forbidden = cb::fs::get_env("CB_NO_DOSBOX");
        e.forbidden = forbidden && !forbidden->empty();
        return e;
    }
    cb::build::dos_tools::DosboxRunner runner_;
};

}  // namespace codebreaker::build
