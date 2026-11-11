// codebreaker/verify/gate16/runtime16.cpp -- typed C++ port of codebreaker.verify.gate16.runtime16.

#include "codebreaker/verify/gate16/runtime16.hpp"

#include "codebreaker/symbols/libraries/omf.hpp"
#include "codebreaker/symbols/libraries/package.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::gate16::runtime16 {

namespace omf = symbols::libraries::omf;
namespace libraries = symbols::libraries;

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

const py::list<py::str> LIBRARIES = strs({"MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB",
                                          "MLIBCAW.LIB", "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB",
                                          "LIBW.LIB"});
std::optional<py::list<Function>> _routines;

const py::list<Function>& routines(const py::list<py::str>& names) {
    if (!_routines.has_value()) {
        py::str home = py::settings::get_str(py::str("msvc152"));
        py::list<Function> out;
        for (const py::str& lib : names) {
            py::str path = py::os::path::join(py::os::path::join(home, py::str("LIB")), lib);
            if (!py::os::path::isfile(path)) continue;
            py::list<libraries::Function> found = std::get<1>(libraries::functions(path));
            for (const libraries::Function& lf : found) {
                // f._replace(name="%s:%s" % (lib, f.name), mask=omf.fields_only(f.mask))   (a far call stays one)
                Function f;
                f.name = lib + py::str(":") + lf.name;
                f.code = lf.code;
                f.at = lf.at;
                std::string fo;
                if (const py::bytes* m = std::get_if<py::bytes>(&lf.mask)) {
                    fo = omf::fields_only(*m).raw();
                } else {
                    for (int64_t m : std::get<py::list<int64_t>>(lf.mask)) fo.push_back(m == 1 ? 1 : 0);
                }
                f.mask = py::bytes(fo);
                int64_t holes = 0;
                for (int64_t b : std::get<py::bytes>(f.mask)) {
                    if (!b) holes += 1;
                }
                if (py::len(f.code) >= 6 && holes >= 4) out.append(f);
            }
        }
        out.sort([](const Function& r) { return -py::len(r.code); });
        _routines = out;
    }
    return *_routines;
}

py::dict<py::str, py::str> identify(const py::dict<py::str, std::tuple<py::bytes, py::bytes>>& originals) {
    py::dict<py::str, py::str> out;
    py::dict<py::bytes, py::list<Function>> by_start;
    for (const Function& f : routines()) {
        by_start.setdefault(f.code.slice(0, 2), py::list<Function>()).append(f);
    }
    for (const auto& [fn, entry] : originals.items()) {
        const py::bytes& code = std::get<0>(entry);
        const py::bytes& omask = std::get<1>(entry);
        for (const Function& f : by_start.get(code.slice(0, 2), py::list<Function>())) {
            if (py::len(f.code) + 16 < py::len(code)) continue;   // (a routine much shorter than the function: not it)
            if (fits(f, code, omask)) {
                out.set(fn, f.name);
                break;
            }
        }
    }
    return out;
}

}  // namespace codebreaker::verify::gate16::runtime16
