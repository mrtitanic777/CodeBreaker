// codebreaker/base/capstone_dll.hpp -- SHIM next to cb/base/capstone_dll.hpp (docs/CPP_DESIGN.md 6.3).
//
// The new module finds the Capstone 5 and Unicorn DLLs that ship beside codebreaker.exe. The decoders still on the old
// layer (decompile/describe, analyze/image + native + dos_functions, symbols/x86) are written against Capstone 4.0.2's
// ABI, whose DLL is the Python package's: until they are refactored they keep getting that one, found as they always
// found it. Found, in order: the CB_CAPSTONE_DLL environment variable; the setting "capstone_dll"; <entry>/capstone/lib/
// capstone.dll for each entry of PYTHONPATH; the site-packages of the `python` on PATH and of its user site
// (%APPDATA%\Python\PythonXY\site-packages). ImportError ("No module named 'capstone'") when none holds one.
// COMPAT(py): deleted with the last decoder that needs Capstone 4.
#pragma once

#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::capstone_dll {

namespace detail {

namespace fs = std::filesystem;

inline std::optional<py::str> in_package_folder(const fs::path& site, const char* package, const char* dll) {
    std::error_code ec;
    fs::path p = site / package / "lib" / dll;
    if (fs::is_regular_file(p, ec)) return py::str(p.u8string());
    return std::nullopt;
}

// every site-packages under `root` that a Python layout has: <root>/Lib/site-packages, <root>/PythonXY/site-packages
inline void site_candidates(const fs::path& root, std::vector<fs::path>& out) {
    std::error_code ec;
    out.push_back(root / "Lib" / "site-packages");
    out.push_back(root / "site-packages");
    if (fs::is_directory(root, ec))
        for (const fs::directory_entry& e : fs::directory_iterator(root, ec))
            if (e.is_directory(ec)) out.push_back(e.path() / "site-packages");
}

inline py::str look(const char* env_name, const char* setting, const char* package, const char* dll) {
    std::optional<py::str> env = py::os::getenv(py::str(env_name));
    if (env && !env->empty() && py::os::path::isfile(*env)) return *env;
    try {
        std::optional<py::str> set = py::settings::get_opt_str(py::str(setting));
        if (set && !set->empty() && py::os::path::isfile(*set)) return *set;
    } catch (const py::Exception&) {
    }
    std::vector<fs::path> sites;
    std::optional<py::str> pp = py::os::getenv(py::str("PYTHONPATH"));
    if (pp)
        for (const py::str& e : py::split(*pp, py::os::pathsep).vec())
            if (!e.empty()) sites.push_back(fs::u8path(e.utf8()));
    std::optional<py::str> python = py::shutil::which(py::str("python"));
    if (python) site_candidates(fs::u8path(py::os::path::dirname(*python).utf8()), sites);   // (the interpreter's own folder)
    std::optional<py::str> appdata = py::os::getenv(py::str("APPDATA"));
    if (appdata) site_candidates(fs::u8path(appdata->utf8()) / "Python", sites);           // (the user site)
    for (const fs::path& s : sites) {
        std::optional<py::str> got = in_package_folder(s, package, dll);
        if (got) return *got;
    }
    throw py::ImportError(std::string("No module named '") + package + "'");
}

}  // namespace detail

// The path of capstone.dll (looked for once).
inline py::str find() {
    static std::mutex m;
    static std::optional<py::str> found;
    std::lock_guard<std::mutex> hold(m);
    if (!found) found = detail::look("CB_CAPSTONE_DLL", "capstone_dll", "capstone", "capstone.dll");
    return *found;
}

// The path of unicorn.dll, the one the Python unicorn package carries (unicorn/lib/unicorn.dll), looked for the same way:
// CB_UNICORN_DLL, the setting "unicorn_dll", then the site-packages. ImportError ("No module named 'unicorn'") when none
// holds one.
inline py::str find_unicorn() {
    static std::mutex m;
    static std::optional<py::str> found;
    std::lock_guard<std::mutex> hold(m);
    if (!found) found = detail::look("CB_UNICORN_DLL", "unicorn_dll", "unicorn", "unicorn.dll");
    return *found;
}

}  // namespace codebreaker::base::capstone_dll
