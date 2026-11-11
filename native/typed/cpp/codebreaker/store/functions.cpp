// codebreaker/store/functions.cpp -- typed C++ port of codebreaker.store.functions (see functions.hpp).

#include "codebreaker/store/functions.hpp"

#include "codebreaker/engine/facts.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::store::functions {

py::list<engine::native_engine::Overview> overview(const std::optional<py::str>& path) {
    auto fs = open(path, true);
    if (!fs) return py::list<engine::native_engine::Overview>();
    auto result = fs->overview();                      // (the C++ store works it out itself)
    fs->close();
    return result;
}

py::dict<py::str, engine::native_engine::Cell> latest(const py::str& module_sha, const std::optional<py::str>& path) {
    auto fs = open(path, true);
    if (!fs) return py::dict<py::str, engine::native_engine::Cell>();
    auto result = fs->latest(module_sha);
    fs->close();
    return result;
}

py::str default_path() {
    auto env = py::os::environ_::get(py::str("CB_FUNCTIONS_DB"));
    if (env && !env->empty()) return *env;
    // settings.state("functions.sqlite"): the state folder is created
    py::str state = py::settings::get_str(py::str("state"));
    py::str path = py::os::path::join(state, py::str("functions.sqlite"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

py::str databases_folder(const std::optional<py::str>& path_in, const std::optional<bool>& own_in) {
    bool own;
    if (own_in) {
        own = *own_in;
    } else {
        auto env = py::os::environ_::get(py::str("CB_FUNCTIONS_DB"));
        own = (path_in && !path_in->empty()) || (env && !env->empty());
    }
    py::str path = (path_in && !path_in->empty()) ? *path_in : default_path();
    if (!own) return engine::facts::root();
    if (py::os::path::basename(path).lower() == "functions.sqlite") {
        return py::os::path::join(py::os::path::dirname(py::os::path::abspath(path)), py::str("db"));
    }
    return std::get<0>(py::os::path::splitext(py::os::path::abspath(path))) + ".db";
}

py::str _ro(const py::str& path) {
    return py::str("file:") + path.replace(py::str("\\"), py::str("/")).replace(py::str("?"), py::str("%3f")).replace(py::str("#"), py::str("%23")) +
           py::str("?mode=ro");
}

std::shared_ptr<engine::native_engine::FunctionStore> FunctionStore(const std::optional<py::str>& path, bool readonly) {
    return std::make_shared<engine::native_engine::FunctionStore>(path, readonly);
}

std::shared_ptr<engine::native_engine::FunctionStore> open(const std::optional<py::str>& path, bool readonly) {
    if (readonly && !py::os::path::isfile((path && !path->empty()) ? *path : default_path())) return nullptr;
    return FunctionStore(path, readonly);
}

}  // namespace codebreaker::store::functions
