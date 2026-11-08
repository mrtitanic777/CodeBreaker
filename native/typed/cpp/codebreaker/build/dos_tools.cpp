// codebreaker/build/dos_tools.cpp -- typed C++17 port of codebreaker.build.dos_tools (see dos_tools.hpp).

#include "codebreaker/build/dos_tools.hpp"

#include <chrono>
#include <thread>

#include "codebreaker/build/dos_sessions.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::dos_tools {

using py::str;

str emulator() {
    // settings.dosbox_x(): "" when CB_NO_DOSBOX forbids it (byte-match spot-check only)
    auto no = py::os::environ_::get(str("CB_NO_DOSBOX"));
    str path = (no && !no->empty()) ? str("") : py::settings::get_str(str("dosbox_x"));
    return (!path.empty() && py::os::path::isfile(path)) ? path : str("");
}

std::tuple<bool, py::dict<str, py::bytes>, std::optional<str>> run(const Mounts& mounts_in, const py::list<str>& bat_lines,
                                                                  const py::list<str>& artifacts, double timeout,
                                                                  const py::dict<str, FileData>& files,
                                                                  const std::optional<py::dict<str, str>>& cpu,
                                                                  const std::optional<py::dict<str, str>>& dos,
                                                                  const py::dict<str, str>& variables, const std::optional<str>& work_in) {
    str dbx = emulator();
    if (dbx.empty()) return {false, py::dict<str, py::bytes>(), std::nullopt};
    str work = (work_in && !work_in->empty()) ? *work_in : py::tempfile::mkdtemp(std::nullopt, str("cb_dos_"));
    Mounts mounts = mounts_in.copy();
    bool has_d = false;
    for (const auto& m : mounts)
        if (std::get<0>(m) == str("d")) has_d = true;
    if (!has_d) mounts.append(std::make_tuple(str("d"), work));   // mounts.setdefault("d", work)
    for (const auto& kv : files.items()) {
        const FileData& data = std::get<1>(kv);
        py::file f = py::open(py::os::path::join(work, std::get<0>(kv)), std::holds_alternative<py::bytes>(data) ? str("wb") : str("w"));
        if (std::holds_alternative<py::bytes>(data)) f.write(std::get<py::bytes>(data));
        else f.write(std::get<str>(data));
        f.close();
    }
    {
        py::file f = py::open(py::os::path::join(work, str("BLD.BAT")), str("w"));
        std::string lines;
        for (int64_t i = 0; i < bat_lines.size(); ++i) lines += (i ? "\r\n" : "") + bat_lines[i].utf8();
        f.write(str("@echo off\r\n" + lines + "\r\necho _DONE_ > done.txt\r\n"));
        f.close();
    }
    std::string conf = "[cpu]";
    py::dict<str, str> cpu_set;
    if (cpu) cpu_set = *cpu;
    else cpu_set.set(str("cycles"), str("20000"));
    for (const auto& kv : cpu_set.items()) conf += "\n" + std::get<0>(kv).utf8() + "=" + std::get<1>(kv).utf8();
    if (dos && !dos->empty()) {
        conf += "\n[dos]";
        for (const auto& kv : dos->items()) conf += "\n" + std::get<0>(kv).utf8() + "=" + std::get<1>(kv).utf8();
    }
    conf += "\n[autoexec]";
    for (const auto& m : mounts) conf += "\nmount " + std::get<0>(m).utf8() + " \"" + std::get<1>(m).replace(str("/"), str("\\")).utf8() + "\"";
    for (const auto& kv : variables.items()) conf += "\nset " + std::get<0>(kv).utf8() + "=" + std::get<1>(kv).utf8();
    conf += "\n" + std::get<0>(mounts[mounts.size() - 1]).utf8() + ":\ncall BLD.BAT\nexit\n";
    {
        py::file f = py::open(py::os::path::join(work, str("r.conf")), str("w"));
        f.write(str(conf));
        f.close();
    }
    for (const str& name : artifacts) {
        try {
            py::os::remove(py::os::path::join(work, name));
        } catch (const py::OSError&) {
        }
    }
    try {
        py::os::remove(py::os::path::join(work, str("done.txt")));
    } catch (const py::OSError&) {
    }
    // env.setdefault("SDL_VIDEODRIVER", "dummy"); stdout and stderr to the null device; killed when the time is up
    py::dict<str, str> extra;
    if (!py::os::environ_::get(str("SDL_VIDEODRIVER"))) extra.set(str("SDL_VIDEODRIVER"), str("dummy"));
    try {
        dos_sessions::Popen proc(py::list<str>{dbx, str("-conf"), py::os::path::join(work, str("r.conf")), str("-exit")}, extra);
        auto start = std::chrono::steady_clock::now();
        while (!proc.poll()) {
            if (std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() > timeout) {
                proc.kill();
                proc.wait(5.0);
                return {false, py::dict<str, py::bytes>(), work};
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    } catch (const py::OSError&) {
        return {false, py::dict<str, py::bytes>(), work};
    }
    py::dict<str, py::bytes> made;
    for (const str& name : artifacts) {
        str path = py::os::path::join(work, name);
        if (py::os::path::exists(path)) {
            py::file f = py::open_rb(path);
            made.set(name, f.read());
            f.close();
        }
    }
    return {made.size() == artifacts.size(), made, work};
}

str masm_root() {
    str root = py::settings::get_str(str("masm"));
    return (!root.empty() && py::os::path::isfile(py::os::path::join(py::os::path::join(root, str("BIN")), str("MASM.EXE")))) ? root : str("");
}

masm::Assembly masm(const str& source, const str& stem, double timeout, const std::optional<str>& root) {
    return codebreaker::build::masm::assemble(source, stem, timeout, (root && !root->empty()) ? root : std::nullopt);
}

}  // namespace codebreaker::build::dos_tools
