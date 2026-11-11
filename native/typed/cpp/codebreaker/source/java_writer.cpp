// codebreaker/source/java_writer.cpp -- typed C++ port of codebreaker.source.java_writer (see java_writer.hpp).

#include "codebreaker/source/java_writer.hpp"

#include <algorithm>

#include "codebreaker/source/module_writer.hpp"
#include "codebreaker/store/functions.hpp"
#include "py_settings.hpp"

// The two outside calls this module makes, whose Python modules (codebreaker.java.transpile, codebreaker.verify.lanes.java)
// have no typed port yet: declared here as the Python calls them, defined by those ports.
namespace codebreaker::java::transpile {
// transpile.convert(java_dir, out_dir, platform=..., write_report=False): the Java source tree as C++ in out_dir
void convert(const py::str& java_dir, const py::str& out_dir, const std::optional<py::str>& platform, bool write_report);
}  // namespace codebreaker::java::transpile
namespace codebreaker::verify::lanes::java {
py::str source_dir(const py::str& path);
extern const std::optional<py::str> PLATFORM;     // no platform: the generic stdafx.h, nothing a host compiler lacks
}  // namespace codebreaker::verify::lanes::java

namespace codebreaker::source::java_writer {

const py::str BANNER("// %s -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [%s] %s\n");

namespace {

namespace F = store::functions;

py::str cell_text(const engine::native_engine::Cell& c) { return engine::native_engine::cell_str(c); }

// `(x or "")` of a store cell
py::str or_empty(const engine::native_engine::Cell& c) { return cell_text(c); }

}  // namespace

py::str write_java(const py::str& path, const std::shared_ptr<engine::native_engine::FunctionStore>& fs, const py::str& sha,
                   const py::str& digest, const py::str& out_dir0, const std::function<void(const py::str&)>& log0) {
    std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    py::str out_dir = out_dir0;
    py::str name = py::os::path::basename(py::os::path::normpath(path));
    bool held = false;
    if (py::os::path::isdir(out_dir)) {
        for (const py::str& f : py::os::listdir(out_dir))
            if ((f.endswith(py::str(".h")) || f.endswith(py::str(".cpp"))) && !module_writer::made_by_us(py::os::path::join(out_dir, f))) held = true;
    }
    if (held) {
        log(py::percent(py::str("%s: holds files written by hand -- the C++ goes to the state folder instead"), out_dir));
        // settings.state("source", name): a path inside the state folder, whose parent is created if it is not there
        out_dir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("source"), name);
        py::os::makedirs(py::os::path::dirname(out_dir), 0777, true);
    }
    java::transpile::convert(verify::lanes::java::source_dir(path), out_dir, verify::lanes::java::PLATFORM, false);
    auto best = fs->best(sha, digest);
    py::dict<py::str, int64_t> counts;
    for (const py::str& k : F::ORDER()) counts.set(k, 0);
    struct Row {
        py::str cls, status, said;
    };
    std::vector<Row> rows;
    std::vector<py::str> classes;
    for (const py::str& a : fs->addresses(sha)) classes.push_back(a);
    std::sort(classes.begin(), classes.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    for (const py::str& cls : classes) {
        auto row = best.get(cls);
        py::str status = row ? cell_text(row->klass) : py::str("no-source");
        py::str why = row ? or_empty(row->reason) : py::str("not judged on the current verifier");
        py::str detail = row ? or_empty(row->detail) : py::str("");
        counts.set(status, counts.at(status) + 1);      // (a status outside the classes is a KeyError, as dict.fromkeys' counts are)
        py::list<py::str> said_lines = py::splitlines(status == py::str("no-compile") ? why : (!detail.empty() ? detail : why));
        py::str said = said_lines.empty() ? py::str("") : said_lines[0].slice(0, 200);
        rows.push_back({cls, status, said});
        for (const char* ext : {".cpp", ".h"}) {
            py::str target = py::os::path::join(out_dir, cls + py::str(ext));
            if (py::os::path::isfile(target)) {
                py::str text = py::open(target, py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
                py::file f = py::open(target, py::str("w"), py::str("utf-8"));
                f.write(py::percent(BANNER, cls + py::str(ext), F::TITLE().at(status), said) + text);
                f.close();
                break;
            }
        }
    }
    int64_t total = static_cast<int64_t>(rows.size());
    py::list<py::str> summary;
    for (const py::str& k : F::ORDER())
        if (counts.at(k)) summary.append(py::percent(py::str("%s %d"), F::TITLE().at(k), counts.at(k)));
    py::list<py::str> lines;
    lines.append(py::percent(py::str("# %s -- C++03 transpiled from Java"), name));
    lines.append(py::str(""));
    lines.append(py::percent(py::str("%d classes: %s"), total, py::join(py::str(", "), summary)));
    lines.append(py::str(""));
    lines.append(py::str("| class | status | why |"));
    lines.append(py::str("|---|---|---|"));
    for (const Row& r : rows)
        lines.append(py::percent(py::str("| %s | %s | %s |"), r.cls, F::TITLE().at(r.status), r.said.slice(0, 160).replace(py::str("|"), py::str("\\|"))));
    {
        py::file f = py::open(py::os::path::join(out_dir, py::str("STATUS.md")), py::str("w"), py::str("utf-8"));
        f.write(py::join(py::str("\n"), lines) + py::str("\n"));
        f.close();
    }
    log(py::percent(py::str("%s: %d classes -> %s  (%s)"), name, total, out_dir, py::join(py::str(", "), summary)));
    return out_dir;
}

}  // namespace codebreaker::source::java_writer
