// codebreaker/package.cpp -- see package.hpp.

#include "codebreaker/package.hpp"

#include <vector>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::package {

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// the name of a setting value's type, as Python words a TypeError about it
const char* type_word(const py::settings::Value& v) {
    switch (v.index()) {
        case 1: return "int";
        case 2: return "float";
        case 3: return "bool";
        case 4: return "NoneType";
        default: return "str";
    }
}

}  // namespace

void export_for_carried_over_code() {
    struct Pair {
        const char* env;
        const char* key;
    };
    for (const Pair& p : {Pair{"CB_CORPUS_ROOT", "corpus"}, Pair{"CB_OPENWATCOM", "openwatcom"}, Pair{"CB_DECOMPILER_BIN", "decompiler_bin"}}) {
        auto current = py::os::environ_::get(py::str(p.env));
        if (current && !current->empty()) continue;                                  // not os.environ.get(env)
        py::settings::Value value = py::settings::get(py::str(p.key));
        if (!py::settings::truthy_(value)) continue;                                  // and get(key)
        if (value.index() != 0) throw py::TypeError(std::string("str expected, not ") + type_word(value));   // os.environ[env] = <not a str>
        py::os::environ_::set(py::str(p.env), std::get<py::str>(value));
    }
    // os.environ.setdefault("CB_RESULTS_DB", state("results.sqlite")): (the path is made first, as Python's argument is)
    py::str results = state({py::str("results.sqlite")});
    if (!py::os::environ_::get(py::str("CB_RESULTS_DB"))) py::os::environ_::set(py::str("CB_RESULTS_DB"), results);
}

void init() { export_for_carried_over_code(); }

}  // namespace codebreaker::package
