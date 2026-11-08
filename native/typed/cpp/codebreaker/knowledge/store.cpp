// codebreaker/knowledge/store.cpp -- typed C++ port of codebreaker.knowledge.store (see store.hpp).

#include "codebreaker/knowledge/store.hpp"

#include <mutex>

#include "codebreaker/knowledge/sources.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::knowledge::store {

const py::str AUTHORED("authored");
const py::str DERIVED("derived");
const py::str LEARNED("learned");
const py::str GIVEN("given");

namespace {

std::recursive_mutex& lock() {
    static std::recursive_mutex m;
    return m;
}
py::dict<py::str, std::shared_ptr<engine::native_engine::KnowledgeBase>>& opened() {
    static py::dict<py::str, std::shared_ptr<engine::native_engine::KnowledgeBase>> d;   // path -> KnowledgeBase, of this process
    return d;
}

// sources.py asks `store.open()` for the process's knowledge base: this is the native base it left to whoever
// links it.
const bool hooked_ = [] {
    sources::store_open() = []() -> std::shared_ptr<sources::KnowledgeBase> { return open(); };
    return true;
}();

}  // namespace

py::str default_path() {
    auto env = py::os::environ_::get(py::str("CB_KNOWLEDGE_DB"));
    if (env && !env->empty()) return *env;
    // settings.state("db", "knowledge.sqlite"): a path inside the state folder, whose folder is created
    py::str path = py::os::path::join(py::os::path::join(py::settings::get_str(py::str("state")), py::str("db")),
                                      py::str("knowledge.sqlite"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

py::str beside(const py::str& store_path) {
    return py::os::path::join(py::os::path::join(py::os::path::dirname(py::os::path::abspath(store_path)), py::str("db")),
                              py::str("knowledge.sqlite"));
}

std::shared_ptr<engine::native_engine::KnowledgeBase> KnowledgeBase(const py::str& path) {
    return std::make_shared<engine::native_engine::KnowledgeBase>(path);
}

std::shared_ptr<engine::native_engine::KnowledgeBase> open(const std::optional<py::str>& path_in) {
    py::str path = py::os::path::abspath((path_in && !path_in->empty()) ? *path_in : default_path());
    std::lock_guard<std::recursive_mutex> g(lock());
    auto found = opened().get(path);
    if (found) return found;
    auto kb = KnowledgeBase(path);
    opened().set(path, kb);
    return kb;
}

}  // namespace codebreaker::knowledge::store
