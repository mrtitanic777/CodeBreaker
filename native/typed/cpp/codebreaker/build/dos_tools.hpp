// codebreaker/build/dos_tools.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/build/dos_tools.{hpp,cpp}
// (a cb::build::DosRunner over DOSBox-X); this header keeps the old py:: surface (emulator, run, masm_root, masm) for the ports and the
// dynamic exe that still use it. Conversion and forwarding only; the settings come from the old settings shim. Deleted with its last
// py:: consumer.
//
// DIVERGES, for the old callers: text files are written with DOS line ends (a "\r\n" already there stays one: Python's text mode made it
// "\r\r\n"), BLD.BAT and r.conf likewise.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/build/dos_tools.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/build/masm.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::dos_tools {

using FileData = std::variant<py::str, py::bytes>;
using Mounts = py::list<std::tuple<py::str, py::str>>;   // [(drive, folder)] in order

// DOSBox-X (settings: dosbox_x), or "" (not set up, not a file, or CB_NO_DOSBOX forbids it)
inline py::str emulator() {
    ::cb::build::dos_tools::Emulator e;
    e.dosbox_x = ::cb::compat::legacy::path_of(py::settings::get_str(py::str("dosbox_x")));
    const auto forbidden = ::cb::fs::get_env("CB_NO_DOSBOX");
    e.forbidden = forbidden && !forbidden->empty();
    const auto program = e.program();
    return program ? py::str(::cb::fs::to_text(*program)) : py::str("");
}

// -> (every artifact was made, {name: bytes}, the work folder or None)
//
// cpu, dos: the emulator's [cpu] and [dos] settings (default cpu: cycles=20000); variables: set before the
// commands run (PATH, INCLUDE, LIB ...); work: the folder to run in (default: a new one), kept.
inline std::tuple<bool, py::dict<py::str, py::bytes>, std::optional<py::str>> run(
    const Mounts& mounts, const py::list<py::str>& bat_lines, const py::list<py::str>& artifacts, double timeout = 90,
    const py::dict<py::str, FileData>& files = py::dict<py::str, FileData>(),
    const std::optional<py::dict<py::str, py::str>>& cpu = std::nullopt,
    const std::optional<py::dict<py::str, py::str>>& dos = std::nullopt,
    const py::dict<py::str, py::str>& variables = py::dict<py::str, py::str>(),
    const std::optional<py::str>& work = std::nullopt) {
    namespace nb = ::cb::build;
    nb::DosRunSpec spec;
    for (const auto& m : mounts) spec.mounts.push_back({std::get<0>(m).utf8(), ::cb::fs::from_text(std::get<1>(m).utf8())});
    for (const py::str& line : bat_lines) spec.commands.push_back(line.utf8());
    for (const py::str& name : artifacts) spec.artifacts.push_back(name.utf8());
    spec.timeout = std::chrono::seconds(static_cast<long long>(timeout));
    for (const auto& kv : files.items()) {
        const FileData& data = std::get<1>(kv);
        if (std::holds_alternative<py::bytes>(data)) spec.files[std::get<0>(kv).utf8()] = ::cb::compat::from_py(std::get<py::bytes>(data));
        else spec.files[std::get<0>(kv).utf8()] = std::get<py::str>(data).utf8();
    }
    auto settings = [](const py::dict<py::str, py::str>& d) {
        ::cb::OrderedMap<std::string, std::string> out;
        for (const auto& kv : d.items()) out.insert_or_assign(std::get<0>(kv).utf8(), std::get<1>(kv).utf8());
        return out;
    };
    if (cpu) spec.cpu = settings(*cpu);
    if (dos) spec.dos = settings(*dos);
    spec.variables = settings(variables);
    if (work && !work->empty()) spec.work = ::cb::fs::from_text(work->utf8());

    ::cb::build::dos_tools::Emulator e;
    e.dosbox_x = ::cb::compat::legacy::path_of(py::settings::get_str(py::str("dosbox_x")));
    const auto forbidden = ::cb::fs::get_env("CB_NO_DOSBOX");
    e.forbidden = forbidden && !forbidden->empty();
    const nb::DosRunResult r = ::cb::build::dos_tools::DosboxRunner(e).run(spec);

    py::dict<py::str, py::bytes> made;
    for (const auto& [name, bytes] : r.made) made.set(py::str(name), ::cb::compat::to_py(::cb::ByteView(bytes)));
    return {r.ok, made, r.work ? std::optional<py::str>(py::str(::cb::fs::to_text(*r.work))) : std::nullopt};
}

// the folder settings "masm" names when it holds BIN/MASM.EXE, else ""
inline py::str masm_root() {
    py::str root = py::settings::get_str(py::str("masm"));
    return (!root.empty() && py::os::path::isfile(py::os::path::join(py::os::path::join(root, py::str("BIN")), py::str("MASM.EXE"))))
               ? root
               : py::str("");
}

// Assemble `source` (build/masm). -> {obj, lst, log, work}; a part the assembler did not make is missing.
inline masm::Assembly masm(const py::str& source, const py::str& stem = py::str("t"), double timeout = 180,
                           const std::optional<py::str>& root = std::nullopt) {
    return codebreaker::build::masm::assemble(source, stem, timeout, (root && !root->empty()) ? root : std::nullopt);
}

}  // namespace codebreaker::build::dos_tools
