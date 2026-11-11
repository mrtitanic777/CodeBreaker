// codebreaker/translate/backends/cpp03_java_units.cpp -- java_units of codebreaker.translate.backends.cpp03 (see cpp03.hpp).
//
// Run the Java front end over java_dir and lower every unit. In a file of its own: the back end (cpp03.cpp) does not
// stand on javac, so a program that links only the back end does not link the front end.
#include <algorithm>

#include "codebreaker/translate/backends/cpp03.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/translate/frontends/java/package.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::cpp03 {

using py::str;
namespace lower = codebreaker::translate::frontends::java::lower;

// Run the Java front end over java_dir and lower every unit. -> [ir.Unit]
py::list<ir::NodeP> java_units(const str& java_dir, const py::list<str>& classpath) {
    str out = py::tempfile::mkdtemp(std::nullopt, str("cb-treedump-"));
    try {
        py::dict<str, ir::Dict> units = translate::frontends::java::dump(java_dir, out, classpath);
        std::vector<str> paths = units.keys();
        std::sort(paths.begin(), paths.end());
        py::list<ir::NodeP> res;
        for (const str& p : paths) res.append(lower::lower(units.at(p), p));
        py::shutil::rmtree(out, true);
        return res;
    } catch (...) {
        py::shutil::rmtree(out, true);
        throw;
    }
}

}  // namespace codebreaker::translate::backends::cpp03
