// codebreaker/base/capstone_dll.cpp -- see capstone_dll.hpp.

#include "codebreaker/base/capstone_dll.hpp"

#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::capstone_dll {

namespace fs = std::filesystem;

namespace {

std::optional<py::str> in_package_folder(const fs::path& site, const char* package, const char* dll) {
    std::error_code ec;
    fs::path p = site / package / "lib" / dll;
    if (fs::is_regular_file(p, ec)) return py::str(p.u8string());
    return std::nullopt;
}

// every site-packages under `root` that a Python layout has: <root>/Lib/site-packages, <root>/PythonXY/site-packages
void site_candidates(const fs::path& root, std::vector<fs::path>& out) {
    std::error_code ec;
    out.push_back(root / "Lib" / "site-packages");
    out.push_back(root / "site-packages");
    if (fs::is_directory(root, ec))
        for (const fs::directory_entry& e : fs::directory_iterator(root, ec))
            if (e.is_directory(ec)) out.push_back(e.path() / "site-packages");
}

py::str look(const char* env_name, const char* setting, const char* package, const char* dll) {
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

}  // namespace

py::str find() {
    static std::mutex m;
    static std::optional<py::str> found;
    std::lock_guard<std::mutex> hold(m);
    if (!found) found = look("CB_CAPSTONE_DLL", "capstone_dll", "capstone", "capstone.dll");
    return *found;
}

py::str find_unicorn() {
    static std::mutex m;
    static std::optional<py::str> found;
    std::lock_guard<std::mutex> hold(m);
    if (!found) found = look("CB_UNICORN_DLL", "unicorn_dll", "unicorn", "unicorn.dll");
    return *found;
}

}  // namespace codebreaker::base::capstone_dll
