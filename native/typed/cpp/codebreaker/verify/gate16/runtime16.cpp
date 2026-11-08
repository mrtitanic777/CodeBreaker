// codebreaker/verify/gate16/runtime16.cpp -- typed C++ port of codebreaker.verify.gate16.runtime16.

#include "codebreaker/verify/gate16/runtime16.hpp"

#include "codebreaker/compare/mask.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::gate16::runtime16 {

namespace omf = symbols::libraries::omf;
namespace coff = symbols::libraries::coff;

static py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

const py::list<py::str> LIBRARIES = strs({"MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB",
                                          "MLIBCAW.LIB", "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB",
                                          "LIBW.LIB"});
std::optional<py::list<Function>> _routines;

// libraries.functions(path)[1]: every function of a library or object file
std::function<py::list<coff::Function>(const py::str&)> libraries_functions = [](const py::str& path) -> py::list<coff::Function> {
    py::bytes data = py::open_rb(path).read();
    if (data.slice(0, 8) == py::bytes("!<arch>\n")) {
        py::list<coff::Function> out;
        for (const auto& [_label, body] : coff::members(data)) {
            py::list<coff::Function> found;
            try {
                found = coff::functions(body);
            } catch (const py::StructError&) {
                continue;
            } catch (const py::IndexError&) {
                continue;
            }
            for (const coff::Function& f : found) out.append(f);
        }
        return out;
    }
    if (data.slice(0, 2) == py::bytes("\x4c\x01", 2)) return coff::functions(data);   // I386
    if (data.slice(0, 2) == py::bytes("\xf2\x01", 2)) return coff::functions(data);   // POWERPC
    py::list<coff::Function> out;
    for (const omf::Function& f : omf::functions(data)) out.append(coff::Function{f.name, f.code, f.mask, f.at});
    return out;
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
        py::str home = py::settings::get_str(py::str("msvc152"));
        py::list<Function> out;
        for (const py::str& lib : names) {
            py::str path = py::os::path::join(py::os::path::join(home, py::str("LIB")), lib);
            if (!py::os::path::isfile(path)) continue;
            for (const coff::Function& lf : libraries_functions(path)) {
                // f._replace(name="%s:%s" % (lib, f.name), mask=omf.fields_only(f.mask))   (a far call stays one)
                Function f;
                f.name = lib + py::str(":") + lf.name;
                f.code = lf.code;
                f.at = lf.at;
                std::string fo;
                if (const py::bytearray* m = std::get_if<py::bytearray>(&lf.mask)) {
                    fo = omf::fields_only(py::bytes(*m)).raw();
                } else {
                    for (int64_t m : std::get<py::list<int64_t>>(lf.mask)) fo.push_back(m == 1 ? 1 : 0);
                }
                f.mask = py::bytearray(py::bytes(fo));
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
