// codebreaker/source/vxd/loadable_corpus.cpp -- typed C++ of codebreaker.source.vxd.loadable_corpus.

#include "codebreaker/source/vxd/loadable_corpus.hpp"

#include <algorithm>
#include <chrono>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/source/vxd/corpus.hpp"
#include "codebreaker/source/vxd/from_source.hpp"
#include "json.hpp"
#include "py_glob.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::loadable_corpus {

py::str ROOT() { return py::settings::checkout_root(); }

namespace {

using clk = std::chrono::steady_clock;
double since(clk::time_point t) { return std::chrono::duration<double>(clk::now() - t).count(); }

double round1(double x) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.1f", x);
    return std::strtod(buf, nullptr);
}

std::string pad(int n) { return std::string(static_cast<size_t>(n), ' '); }
std::string jstr(const py::str& s) {
    std::string o;
    py::json::put(o, s);
    return o;
}
std::string jbool(bool b) { return b ? "true" : "false"; }
std::string jflt(double d) {
    std::string o;
    py::json::put(o, d);
    return o;
}
py::str head(const py::str& s, int64_t n) { return py::len(s) > n ? s.slice(0, n) : s; }
py::str tail(const py::str& s, int64_t n) { return py::len(s) > n ? s.slice(py::len(s) - n, py::len(s)) : s; }

std::string stage_j(const std::optional<py::str>& s) { return s ? jstr(*s) : std::string("null"); }

std::string dump_entry(const Entry& e) {
    std::string o = "{\n" + pad(2) + "\"ok\": " + jbool(e.ok);
    auto kv = [&](const char* k, const std::string& v) { o += ",\n" + pad(2) + "\"" + k + "\": " + v; };
    switch (e.kind) {
        case Entry::Kind::Built:
            kv("stage", stage_j(e.stage));
            kv("seconds", jflt(e.seconds));
            kv("objects", std::to_string(e.objects));
            kv("device_id", std::to_string(e.device_id));
            kv("device_id_ok", jbool(e.device_id_ok));
            kv("identity", jbool(e.identity));
            kv("resource_len", std::to_string(e.resource_len));
            kv("vxd_len", std::to_string(e.vxd_len));
            break;
        case Entry::Kind::Failed:
            kv("stage", stage_j(e.stage));
            kv("seconds", jflt(e.seconds));
            kv("log", jstr(e.log));
            break;
        case Entry::Kind::Exception:
            kv("stage", stage_j(e.stage));
            kv("log", jstr(e.log));
            kv("trace", jstr(e.trace));
            kv("seconds", jflt(e.seconds));
            break;
        case Entry::Kind::Open:
            kv("stage", stage_j(e.stage));
            kv("log", jstr(e.log));
            break;
    }
    return o + "\n" + pad(1) + "}";
}

Entry entry_of(const ::py::V& v) {
    Entry e;
    if (!v.is_dict()) return e;
    auto has = [&](const char* k) {
        for (const auto& kv : v.items())
            if (kv.first.str() == k) return true;
        return false;
    };
    auto geti = [&](const char* k) { return v.get(k).is_intlike() ? v.get(k).as_int() : int64_t(0); };
    auto getb = [&](const char* k) { return v.get(k).is_intlike() && v.get(k).as_int() != 0; };
    e.ok = getb("ok");
    if (v.get("stage").is_str()) e.stage = py::str(v.get("stage").str());
    e.seconds = (v.get("seconds").is_float() || v.get("seconds").is_int()) ? v.get("seconds").as_float() : 0.0;
    if (v.get("log").is_str()) e.log = py::str(v.get("log").str());
    if (v.get("trace").is_str()) e.trace = py::str(v.get("trace").str());
    if (has("objects")) {
        e.kind = Entry::Kind::Built;
        e.objects = geti("objects");
        e.device_id = geti("device_id");
        e.device_id_ok = getb("device_id_ok");
        e.identity = getb("identity");
        e.resource_len = geti("resource_len");
        e.vxd_len = geti("vxd_len");
    } else if (has("trace")) {
        e.kind = Entry::Kind::Exception;
    } else if (!has("seconds")) {
        e.kind = Entry::Kind::Open;
    } else {
        e.kind = Entry::Kind::Failed;
    }
    return e;
}

Results load_results(const py::str& path) {
    Results out;
    py::file f = py::open(path, py::str("r"), py::str("cp1252"));
    const py::str text = f.read_text();
    f.close();
    const ::py::V top = py::json_parse(text.utf8());
    if (!top.is_dict()) throw py::AttributeError("'list' object has no attribute 'items'");
    for (const auto& kv : top.items()) out.set(py::str(kv.first.str()), entry_of(kv.second));
    return out;
}

}  // namespace

Entry _one(const py::str& name, const le::LEFile& v, const py::str& root) {
    (void)name;
    const auto t0 = clk::now();
    Entry e;
    try {
        const from_source::Built r = from_source::build_from_source(v, std::nullopt, root);
        e.ok = r.ok;
        e.stage = std::nullopt;   // (r.get("stage"): the build never reports one)
        e.seconds = round1(since(t0));
        if (r.ok) {
            e.kind = Entry::Kind::Built;
            e.objects = r.objects;
            e.device_id = r.device_id;
            e.device_id_ok = r.device_id_ok;
            e.identity = r.page_map_identity;
            e.resource_len = r.resource_len;
            e.vxd_len = r.vxd_len;
        } else {
            e.kind = Entry::Kind::Failed;
            e.log = tail(py::str(""), 240);   // (r.get("log") or "")[-240:]: the build keeps no log of a failure
        }
    } catch (const std::exception& ex) {
        e = Entry();
        e.kind = Entry::Kind::Exception;
        e.ok = false;
        e.stage = py::str("exception");
        e.log = head(py::str(std::string(ex.what())), 200);
        e.trace = tail(py::str(std::string(typeid(ex).name()) + ": " + ex.what()), 300);
        e.seconds = round1(since(t0));
    }
    return e;
}

py::str dump_results(const Results& results) {
    if (results.empty()) return py::str("{}");
    std::string o = "{\n";
    const auto items = results.items();
    for (size_t i = 0; i < items.size(); ++i)
        o += pad(1) + jstr(std::get<0>(items[i])) + ": " + dump_entry(std::get<1>(items[i])) + (i + 1 < items.size() ? ",\n" : "\n");
    return py::str(o + "}");
}

void _save(const Results& results, const py::str& path) {
    const py::str tmp = path + py::str(".tmp");
    {
        py::file f = py::open(tmp, py::str("w"), py::str("cp1252"));   // (a text-mode write: "\n" is "\r\n")
        f.write(dump_results(results));
        f.close();
    }
    py::os::replace(tmp, path);
}

void _summary(std::ostream& out, const Results& results, double secs) {
    int64_t n_ok = 0, dev_ok = 0, ident = 0;
    std::vector<std::pair<std::optional<py::str>, int64_t>> stages;   // Counter, in first-seen order
    for (const auto& kv : results.items()) {
        const Entry& e = std::get<1>(kv);
        if (e.ok) {
            n_ok += 1;
            dev_ok += e.device_id_ok ? 1 : 0;
            ident += e.identity ? 1 : 0;
        } else {
            bool found = false;
            for (auto& s : stages)
                if (s.first == e.stage) {
                    s.second += 1;
                    found = true;
                }
            if (!found) stages.emplace_back(e.stage, 1);
        }
    }
    out << py::percent(py::str("\n== loadable corpus: %d/%d BUILD | %d device-id-correct | %d identity-map | %.0fs =="), n_ok, results.size(), dev_ok, ident, secs).utf8()
        << std::endl;
    if (!stages.empty()) {
        std::string d = "{";
        for (size_t i = 0; i < stages.size(); ++i)
            d += (i ? ", " : "") + (stages[i].first ? py::repr(*stages[i].first).utf8() : std::string("None")) + ": " + std::to_string(stages[i].second);
        out << "   failures by stage: " << d << "}" << std::endl;
    }
}

Results run(std::ostream& out, const py::str& srcdir, const py::str& out_json, const std::optional<py::list<py::str>>& redo_in, int64_t workers, const py::str& root_in) {
    const py::str root = py::os::path::abspath(root_in);
    py::set<py::str> redo;
    if (redo_in)
        for (const py::str& r : *redo_in) redo.add(r);
    Results results;
    if (py::os::path::exists(out_json)) results = load_results(out_json);
    std::vector<py::str> cand;
    for (const py::str& pat : corpus::_VXD_EXTS)
        for (const py::str& p : py::glob::glob(py::os::path::join(srcdir, pat))) cand.push_back(p);
    for (const py::str& f : corpus::_EXTRA_FILES) cand.push_back(py::os::path::join(srcdir, f));
    std::vector<py::str> uniq;
    for (const py::str& p : cand)
        if (std::find(uniq.begin(), uniq.end(), p) == uniq.end()) uniq.push_back(p);
    std::sort(uniq.begin(), uniq.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    std::vector<py::str> files;
    for (const py::str& p : uniq)
        if (py::os::path::exists(p) && corpus::_is_le_vxd(p)) files.push_back(p);
    using Item = std::tuple<py::str, py::str, le::LEFile>;
    py::list<Item> todo;
    for (const py::str& p : files) {
        py::list<std::tuple<py::str, le::LEFile>> mods;
        try {
            mods = le::open_vxds(p);
        } catch (const std::exception& e) {
            Entry er;
            er.kind = Entry::Kind::Open;
            er.ok = false;
            er.stage = py::str("open");
            er.log = head(py::str(std::string(e.what())), 80);
            results.set(py::os::path::basename(p), er);
            continue;
        }
        for (const auto& nv : mods) {
            const py::str key = py::split(std::get<0>(nv), py::str("."))[0].lower();
            if (results.contains(key) && !redo.contains(key)) continue;
            todo.append(Item(key, std::get<0>(nv), std::get<1>(nv)));
        }
    }
    const auto t_all = clk::now();
    out << py::percent(py::str("== %d modules to build, %d workers =="), todo.size(), workers).utf8() << std::endl;
    jobs::pool::AsDoneOptions<Item, Entry> o;
    o.jobs = workers;
    jobs::pool::as_done(
        [&](const Item& t) { return _one(std::get<1>(t), std::get<2>(t), root); }, todo,
        [&](const Item& t, const Entry& r) {
            const py::str& key = std::get<0>(t);
            results.set(key, r);
            _save(results, out_json);
            if (r.ok) {
                out << py::percent(py::str("  %-14s OK   objs=%-2d dev=0x%04x dev_ok=%-5s ident=%-5s res=%-4d [%ss]"), key, r.objects, r.device_id,
                                   r.device_id_ok ? py::str("True") : py::str("False"), r.identity ? py::str("True") : py::str("False"), r.resource_len,
                                   py::to_str(r.seconds))
                           .utf8()
                    << std::endl;
            } else {
                out << py::percent(py::str("  %-14s FAIL@%-10s %s"), key, r.stage ? *r.stage : py::str("None"),
                                   tail(r.log, 90).replace(py::str("\n"), py::str(" ")))
                           .utf8()
                    << std::endl;
            }
        },
        o);
    _summary(out, results, since(t_all));
    return results;
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    (void)err;
    py::list<py::str> args;
    for (int64_t i = 0; i < argv.size();) {   // (the value of --redo / --workers is not a positional argument)
        if (argv[i] == py::str("--redo") || argv[i] == py::str("--workers")) {
            i += 2;
            continue;
        }
        if (!argv[i].startswith(py::str("--"))) args.append(argv[i]);
        i += 1;
    }
    const py::str srcdir = !args.empty() ? args[0] : base::corpus::root();
    py::str outp;
    if (args.size() > 1) {
        outp = args[1];
    } else {
        outp = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxd"), py::str("loadable.json"));
        py::os::makedirs(py::os::path::dirname(outp), 0777, true);
    }
    py::list<py::str> redo;
    int64_t workers = 4;
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == py::str("--redo")) {
            if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
            redo = py::split(argv[i + 1], py::str(","));
            break;
        }
    }
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == py::str("--workers")) {
            if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
            workers = py::int_parse(argv[i + 1]);
            break;
        }
    }
    run(out, srcdir, outp, redo, workers, ROOT());
    return 0;
}

}  // namespace codebreaker::source::vxd::loadable_corpus
