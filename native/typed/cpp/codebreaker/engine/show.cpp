// codebreaker/engine/show.cpp -- C++ from codebreaker.engine.show (see show.hpp).

#include "codebreaker/engine/show.hpp"

#include <cmath>
#include <ctime>
#include <variant>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/engine.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/ui/service.hpp"
#include "py_os.hpp"

namespace codebreaker::engine::show {

namespace {

using facts::Json;

// the first line of a (possibly many line) text: s.split("\n")[0]
py::str first_line(const py::str& s) {
    int64_t at = s.find(py::str("\n"));
    return at < 0 ? s : s.slice(0, at);
}

py::str short_str(const py::str& v, int64_t width) {
    py::list<py::str> lines = py::splitlines(v);
    if (!py::truthy(lines)) {
        lines.append(py::str(""));
    }
    py::list<py::str> shown;
    for (int64_t i = 0; i < py::len(lines) && i < 12; ++i) {
        shown.append(py::str("      ") + lines[i].slice(0, width));
    }
    return py::percent(py::str("%d lines\n%s%s"), py::len(lines), py::join(py::str("\n"), shown),
                       py::len(lines) > 12 ? py::str("\n      ...") : py::str(""));
}

py::str short_json(const Json& v, int64_t width) {
    switch (v.kind) {
        case Json::Str:
            return short_str(v.s, width);
        case Json::Dict: {
            py::list<py::str> parts;
            int64_t n = 0;
            for (const auto& kv : v.dict.items()) {
                if (n++ >= 12) {
                    break;
                }
                parts.append(py::percent(py::str("%s: %s"), std::get<0>(kv), first_line(short_json(std::get<1>(kv), 40))));
            }
            return py::str("{") + py::join(py::str(", "), parts) + (py::len(v.dict) > 12 ? py::str(", ...") : py::str("")) + py::str("}");
        }
        case Json::List: {
            py::list<py::str> parts;
            for (size_t i = 0; i < v.list.size() && i < 6; ++i) {
                parts.append(first_line(short_json(v.list[i], 40)));
            }
            return py::percent(py::str("[%d] "), static_cast<int64_t>(v.list.size())) + py::join(py::str(", "), parts) +
                   (v.list.size() > 6 ? py::str(", ...") : py::str(""));
        }
        default: {
            py::str text;
            if (v.kind == Json::None) text = py::str("None");
            else if (v.kind == Json::Bool) text = py::str(v.b ? "True" : "False");
            else if (v.kind == Json::Int) text = py::repr(v.i);
            else text = py::repr(v.d);
            return text.slice(0, width) + (py::len(text) > width ? py::str("...") : py::str(""));
        }
    }
}

// the module's text, which `facts` with nothing to show prints
const char* const DOC =
    "show.py -- what a binary's database holds, and why.\n"
    "\n"
    "    python -m codebreaker facts <binary>                  every kind: how many facts, their size, who made them\n"
    "    python -m codebreaker facts <binary> <kind>           each fact of the kind: its key, hash, size, when made,\n"
    "                                                          whether its step's version is the version of now\n"
    "    python -m codebreaker facts <binary> <kind> <key>     one fact: its value (in short) and what it was made from\n"
    "    python -m codebreaker facts <binary> <kind> [key] --make      made current first (the engine runs)\n"
    "    python -m codebreaker facts --databases               every database, its size, and the cap (db_max_gb)\n"
    "\n"
    "<binary> is a path or a corpus name. Nothing is made unless --make is given.\n"
    "\n"
    "When the CodeBreaker service runs (ui/serve.py) it answers (its databases open, its step versions\n"
    "known; what is printed comes when it is done); else the command answers itself, here.\n";

// a cell of a kinds() row as `%s` shows it (None is "None")
py::str cell_text(const native_engine::Cell& c) {
    if (std::holds_alternative<std::monostate>(c)) {
        return py::str("None");
    }
    return native_engine::cell_str(c);
}

}  // namespace

py::str _mb(int64_t n) {
    if (n >= (int64_t(1) << 20)) {
        return py::percent(py::str("%.1f MB"), py::truediv(n, 1048576.0));
    }
    return py::percent(py::str("%.1f KB"), py::truediv(n, 1024.0));
}

py::str _short(const facts::Value& v, int64_t width) {
    if (const py::bytes* b = std::get_if<py::bytes>(&v)) {
        return py::percent(py::str("%d bytes"), py::len(*b));
    }
    if (const py::str* s = std::get_if<py::str>(&v)) {
        return short_str(*s, width);
    }
    return short_json(std::get<Json>(v), width);
}

py::str _when(double t) {
    if (t == 0) {
        return py::str("-");
    }
    // time.strftime("%Y-%m-%d %I:%M:%S %p", time.localtime(t)): the whole seconds, rounded down
    __time64_t secs = static_cast<__time64_t>(std::floor(t));
    std::tm tm{};
    if (_localtime64_s(&tm, &secs) != 0) {
        throw py::OSError("[Errno 22] Invalid argument");
    }
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M:%S %p", &tm);
    return py::str(buf);
}

int64_t main(const py::list<py::str>& argv) {
    std::optional<int64_t> code = ui::service::cli(py::str("facts"), argv);   // the running service's answer, when there is one
    if (code) {
        return *code;
    }
    if (argv.contains(py::str("--databases"))) {
        int64_t total = 0;
        for (const auto& d : facts::databases()) {
            const py::str& p = std::get<0>(d);
            int64_t size = std::get<1>(d);
            double t = std::get<2>(d);
            total += size;
            py::print(py::percent(py::str("  %-70s %10s  opened %s"), py::os::path::basename(p), _mb(size), _when(t)));
        }
        py::print(py::percent(py::str("  all %s (cap %s: setting db_max_gb)"), _mb(total), _mb(facts::max_bytes())));
        return 0;
    }
    bool make = argv.contains(py::str("--make"));
    py::list<py::str> args;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--"))) {
            args.append(a);
        }
    }
    if (!py::truthy(args)) {
        py::print(py::str(DOC));
        return 2;
    }
    std::optional<py::str> path = base::corpus::retail(args[0]);
    if (!path) {
        py::print(py::percent(py::str("no binary %r"), args[0]));
        return 2;
    }
    std::shared_ptr<native_engine::Database> db = engine::open(*path);
    auto log = [](const py::str& line) { py::print(line); };
    if (make && py::len(args) > 1) {
        double t = py::time::time();
        if (py::len(args) > 2) {
            engine::get(db, args[1], args[2], log);
        } else {
            engine::get(db, args[1], py::str(""), log);
        }
        py::print(py::percent(py::str("made in %.1fs: %s"), py::time::time() - t, py::repr(engine::last_run(*db))));
    }
    py::print(py::percent(py::str("%s  (%s)"), *path, db->path));
    if (py::len(args) == 1) {
        py::print(py::percent(py::str("  %-20s %8s %10s  %s"), py::str("kind"), py::str("facts"), py::str("size"), py::str("made by")));
        for (const native_engine::Cells& row : db->kinds()) {
            int64_t size = std::holds_alternative<std::monostate>(row[2]) ? 0 : native_engine::cell_int(row[2]);
            py::print(py::percent(py::str("  %-20s %8d %10s  %s"), native_engine::cell_str(row[0]), native_engine::cell_int(row[1]),
                                  _mb(size), cell_text(row[3])));
        }
        return 0;
    }
    py::str kind = args[1];
    std::optional<py::str> now;
    try {
        now = engine::_step_of(kind).version();
    } catch (const py::LookupError&) {
        now = std::nullopt;
    }
    if (py::len(args) == 2) {
        py::list<facts::Row> rows = db->rows(kind);
        for (const facts::Row& r : rows) {
            py::str state = r.given() ? py::str("given") : ((now && r.version == *now) ? py::str("version of now") : py::str("OLD VERSION"));
            py::print(py::percent(py::str("  %-14s %s  %10s  %s  %6.2fs  %s"), py::truthy(r.key) ? r.key : py::str("\"\""),
                                  r.hash.slice(0, 12), _mb(r.size), _when(r.made), r.took, state));
        }
        py::print(py::percent(py::str("  %d facts of %s"), py::len(rows), kind));
        return 0;
    }
    py::str key = args[2];
    std::optional<engine::Why> w = engine::why(*db, kind, key);
    if (!w) {
        py::print(py::percent(py::str("  no fact %s %s (--make to make it)"), kind, key));
        return 1;
    }
    py::print(py::percent(py::str("  %s %s  hash %s, %s"), kind, key, w->hash, _mb(w->size)));
    bool current = w->version_now && w->version == *w->version_now;
    py::print(py::percent(py::str("  made by    %s, version %s%s"), w->step, w->version,
                          current ? py::str("")
                                  : py::percent(py::str(" (the version of now is %s: stale)"),
                                                w->version_now ? *w->version_now : py::str("None"))));
    py::print(py::percent(py::str("  made       %s, in %.2fs"), _when(w->made), w->took));
    py::print(py::percent(py::str("  from       %d facts"), py::len(w->inputs)));
    int64_t shown = 0;
    for (const auto& in : w->inputs) {
        if (shown++ >= 40) {
            break;
        }
        py::print(py::percent(py::str("    %-20s %-14s %s  %s"), std::get<0>(in), py::truthy(std::get<1>(in)) ? std::get<1>(in) : py::str("\"\""),
                              std::get<2>(in).slice(0, 12), std::get<3>(in) ? py::str("the same now") : py::str("CHANGED")));
    }
    if (py::len(w->inputs) > 40) {
        py::print(py::percent(py::str("    ... %d more"), py::len(w->inputs) - 40));
    }
    py::print(py::percent(py::str("  value      %s"), _short(db->row(kind, key)->value())));
    return 0;
}

}  // namespace codebreaker::engine::show
