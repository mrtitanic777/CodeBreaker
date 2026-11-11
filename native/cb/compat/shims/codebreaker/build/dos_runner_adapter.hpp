// codebreaker/build/dos_runner_adapter.hpp -- SHIM support (docs/CPP_DESIGN.md 6.3): the old build/dos_tools::run as a
// cb::build::DosRunner, so the refactored toolchain drivers (msvc152, period16) can run DOS programs through the old runner
// until build/dos_tools is refactored. COMPAT(py): remove with it (dos_tools then implements DosRunner itself).
//
// The old function is only declared here; the old dos_tools.cpp defines it.
#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include "cb/build/dos_run.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"

namespace codebreaker::build::dos_tools {   // COMPAT(py): defined in build/dos_tools.cpp (not yet refactored)
using FileData = std::variant<py::str, py::bytes>;
using Mounts = py::list<std::tuple<py::str, py::str>>;   // [(drive, folder)] in order
std::tuple<bool, py::dict<py::str, py::bytes>, std::optional<py::str>> run(
    const Mounts& mounts, const py::list<py::str>& bat_lines, const py::list<py::str>& artifacts, double timeout,
    const py::dict<py::str, FileData>& files, const std::optional<py::dict<py::str, py::str>>& cpu,
    const std::optional<py::dict<py::str, py::str>>& dos, const py::dict<py::str, py::str>& variables,
    const std::optional<py::str>& work);
}  // namespace codebreaker::build::dos_tools

namespace codebreaker::build {

class OldDosRunner final : public cb::build::DosRunner {
public:
    cb::build::DosRunResult run(const cb::build::DosRunSpec& spec) const override {
        namespace dt = codebreaker::build::dos_tools;
        dt::Mounts mounts;
        for (const auto& m : spec.mounts) mounts.append(std::make_tuple(py::str(m.drive), py::str(cb::fs::to_text(m.folder))));
        py::list<py::str> bat, artifacts;
        for (const std::string& line : spec.commands) bat.append(py::str(line));
        for (const std::string& a : spec.artifacts) artifacts.append(py::str(a));
        py::dict<py::str, dt::FileData> files;
        for (const auto& [name, data] : spec.files) {
            if (const auto* text = std::get_if<std::string>(&data)) files.set(py::str(name), dt::FileData(py::str(*text)));
            else files.set(py::str(name), dt::FileData(cb::compat::to_py(cb::ByteView(std::get<cb::Bytes>(data)))));
        }
        auto settings = [](const cb::OrderedMap<std::string, std::string>& m) {
            py::dict<py::str, py::str> d;
            for (const auto& [k, v] : m) d.set(py::str(k), py::str(v));
            return d;
        };
        std::optional<py::dict<py::str, py::str>> cpu, dos;
        if (!spec.cpu.empty()) cpu = settings(spec.cpu);
        if (!spec.dos.empty()) dos = settings(spec.dos);
        std::optional<py::str> work;
        if (spec.work) work = py::str(cb::fs::to_text(*spec.work));
        const auto [ok, made, ran_in] =
            dt::run(mounts, bat, artifacts, static_cast<double>(spec.timeout.count()), files, cpu, dos, settings(spec.variables), work);
        cb::build::DosRunResult out;
        out.ok = ok;
        for (const auto& [name, bytes] : made.items()) out.made[name.utf8()] = cb::compat::from_py(bytes);
        if (ran_in) out.work = cb::fs::from_text(ran_in->utf8());
        return out;
    }
};

}  // namespace codebreaker::build
