// codebreaker/verify/gate16/runtime16.cpp -- typed C++ port of codebreaker.verify.gate16.runtime16.

#include "codebreaker/verify/gate16/runtime16.hpp"

#include "codebreaker/compare/mask.hpp"

namespace codebreaker::verify::gate16::runtime16 {

namespace omf = symbols::libraries::omf;

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

const py::list<py::str> LIBRARIES = strs({"MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB",
                                          "MLIBCAW.LIB", "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB",
                                          "LIBW.LIB"});
std::optional<py::list<Function>> _routines;

std::function<py::str(const py::str&)> settings_get = [](const py::str& key) -> py::str {
    std::optional<py::str> v = py::os::getenv(py::str("CB_") + key.upper());
    return v ? *v : py::str("");
};

std::function<py::list<Function>(const py::str&)> libraries_functions = [](const py::str& path) -> py::list<Function> {
    py::bytes data = py::open_rb(path).read();
    // libraries.functions: COFF archives and objects are the 32-bit readers' (no port); the rest is OMF
    if (data.slice(0, 8) == py::bytes("!<arch>\n") || data.slice(0, 2) == py::bytes("\x4c\x01", 2) ||
        data.slice(0, 2) == py::bytes("\xf2\x01", 2))
        throw py::NotImplementedError("a COFF library: only the 16-bit OMF reader is ported");
    return omf::functions(data);
};

bool fits(const Function& function, const py::bytes& code, const py::bytes& code_mask) {
    int64_t n = py::len(function.code);
    if (n > py::len(code)) return false;
    py::bytes theirs = !code_mask.empty() ? code_mask.slice(0, n) : py::bytes();
    py::bytes theirs_full = theirs + py::bytes::zeros(n - py::len(theirs));
    py::list<compare::mask::MaybeMask> masks;
    masks.append(compare::mask::Mask(py::bytes(function.mask)));
    masks.append(compare::mask::Mask(theirs_full));
    return compare::mask::same(function.code, code.slice(0, n), masks);
}

const py::list<Function>& routines(const py::list<py::str>& names) {
    if (!_routines.has_value()) {
        py::str home = settings_get(py::str("msvc152"));
        py::list<Function> out;
        for (const py::str& lib : names) {
            py::str path = py::os::path::join(py::os::path::join(home, py::str("LIB")), lib);
            if (!py::os::path::isfile(path)) continue;
            for (Function f : libraries_functions(path)) {
                f.name = lib + py::str(":") + f.name;
                f.mask = py::bytearray(omf::fields_only(py::bytes(f.mask)));   // (a far call stays one)
                int64_t holes = 0;
                for (int64_t b : f.mask) {
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
