// codebreaker/source/vxd/corpus.cpp -- typed C++ of codebreaker.source.vxd.corpus.

#include "codebreaker/source/vxd/corpus.hpp"

#include <algorithm>
#include <chrono>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py_glob.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "json.hpp"

namespace codebreaker::source::vxd::corpus {

py::str ROOT() { return py::settings::checkout_root(); }

namespace {

using clk = std::chrono::steady_clock;

double since(clk::time_point t) { return std::chrono::duration<double>(clk::now() - t).count(); }

// round(x, 1) as Python rounds (half to even on the exact binary value: the C library's rounding of x * 10 is not it, so format)
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
std::string jint_opt(const std::optional<int64_t>& v) { return v ? std::to_string(*v) : std::string("null"); }
std::string jflt(double d) {
    std::string o;
    py::json::put(o, d);
    return o;
}

// the last `n` characters of a string (s[-n:])
py::str tail(const py::str& s, int64_t n) { return py::len(s) > n ? s.slice(py::len(s) - n, py::len(s)) : s; }

// the text of the first `n` characters (s[:n])
py::str head(const py::str& s, int64_t n) { return py::len(s) > n ? s.slice(0, n) : s; }

std::string dump_entry(const Entry& e) {
    if (e.record) {
        const ModuleRecord& r = *e.record;
        std::string o = "{\n" + pad(2) + "\"byte_exact\": " + jbool(r.byte_exact) + ",\n" + pad(2) + "\"objects\": ";
        if (r.objects.empty()) {
            o += "[]";
        } else {
            o += "[\n";
            for (int64_t i = 0; i < r.objects.size(); ++i) {
                const ObjectRow& x = r.objects[i];
                o += pad(3) + "{\n" + pad(4) + "\"obj\": " + jint_opt(x.obj) + ",\n" + pad(4) + "\"match\": " + jbool(x.match) + ",\n" + pad(4) +
                     "\"ok\": " + jbool(x.ok) + ",\n" + pad(4) + "\"len\": " + jint_opt(x.len) + ",\n" + pad(4) + "\"symbolic\": " + jint_opt(x.symbolic) +
                     ",\n" + pad(4) + "\"forced_raw\": " + jint_opt(x.forced_raw) + ",\n" + pad(4) + "\"raw_fallback\": " + jbool(x.raw_fallback) + ",\n" +
                     pad(4) + "\"diffs\": " + jint_opt(x.diffs) + ",\n" + pad(4) + "\"err\": " + (x.err ? jstr(*x.err) : std::string("null")) + "\n" + pad(3) +
                     "}" + (i + 1 < r.objects.size() ? "," : "") + "\n";
            }
            o += pad(2) + "]";
        }
        o += ",\n" + pad(2) + "\"n_obj\": " + std::to_string(r.n_obj) + ",\n" + pad(2) + "\"n_exact\": " + std::to_string(r.n_exact) + ",\n" + pad(2) +
             "\"symbolic\": " + std::to_string(r.symbolic) + ",\n" + pad(2) + "\"forced_raw\": " + std::to_string(r.forced_raw) + ",\n" + pad(2) +
             "\"raw_fallback_objs\": " + std::to_string(r.raw_fallback_objs) + ",\n" + pad(2) + "\"seconds\": " + jflt(r.seconds) + "\n" + pad(1) + "}";
        return o;
    }
    std::string o = "{\n" + pad(2) + "\"error\": " + jstr(e.error ? *e.error : py::str(""));
    if (e.trace) o += ",\n" + pad(2) + "\"trace\": " + jstr(*e.trace);
    return o + "\n" + pad(1) + "}";
}

// a JSON value read from a resumed file -> an Entry (the keys it knows)
Entry entry_of(const ::py::V& v) {
    Entry e;
    if (!v.is_dict()) return e;
    bool has_error = false;   // (a dict with an "error" key: Python's `"error" in r`)
    for (const auto& kv : v.items())
        if (kv.first.str() == "error") has_error = true;
    auto geti = [&](const char* k) -> std::optional<int64_t> {
        const ::py::V& x = v.get(k);
        return x.is_intlike() ? std::optional<int64_t>(x.as_int()) : std::nullopt;
    };
    if (has_error) {
        e.error = py::str(v.get("error").is_str() ? v.get("error").str() : std::string(""));
        if (v.get("trace").is_str()) e.trace = py::str(v.get("trace").str());
        return e;
    }
    ModuleRecord r;
    r.byte_exact = v.get("byte_exact").is_intlike() && v.get("byte_exact").as_int() != 0;
    if (v.get("objects").is_list()) {
        for (const ::py::V& o : v.get("objects").seq()) {
            ObjectRow x;
            x.obj = o.get("obj").is_intlike() ? std::optional<int64_t>(o.get("obj").as_int()) : std::nullopt;
            x.match = o.get("match").is_intlike() && o.get("match").as_int() != 0;
            x.ok = o.get("ok").is_intlike() && o.get("ok").as_int() != 0;
            x.len = o.get("len").is_intlike() ? std::optional<int64_t>(o.get("len").as_int()) : std::nullopt;
            x.symbolic = o.get("symbolic").is_intlike() ? std::optional<int64_t>(o.get("symbolic").as_int()) : std::nullopt;
            x.forced_raw = o.get("forced_raw").is_intlike() ? std::optional<int64_t>(o.get("forced_raw").as_int()) : std::nullopt;
            x.raw_fallback = o.get("raw_fallback").is_intlike() && o.get("raw_fallback").as_int() != 0;
            x.diffs = o.get("diffs").is_intlike() ? std::optional<int64_t>(o.get("diffs").as_int()) : std::nullopt;
            if (o.get("err").is_str()) x.err = py::str(o.get("err").str());
            r.objects.append(x);
        }
    }
    r.n_obj = geti("n_obj").value_or(0);
    r.n_exact = geti("n_exact").value_or(0);
    r.symbolic = geti("symbolic").value_or(0);
    r.forced_raw = geti("forced_raw").value_or(0);
    r.raw_fallback_objs = geti("raw_fallback_objs").value_or(0);
    r.seconds = (v.get("seconds").is_float() || v.get("seconds").is_int()) ? v.get("seconds").as_float() : 0.0;
    e.record = r;
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

const py::list<py::str> _VXD_EXTS = {py::str("*.vxd"), py::str("*.386"), py::str("*.pdr"), py::str("*.mpd")};
const py::list<py::str> _EXTRA_FILES = {py::str("csmapper.sys"), py::str("bigmem.drv"), py::str("drvspace.bin")};

bool _is_le_vxd(const py::str& path) { return formats::detect::holds_drivers(path); }

Entry _one_module(const py::str& name, const le::LEFile& v) {
    (void)name;
    const auto t0 = clk::now();
    Entry e;
    try {
        const masm::ModuleReport rep = masm::compare_module(v);
        ModuleRecord r;
        for (const masm::ObjectReport& o : rep.objects) {
            ObjectRow x;
            x.obj = o.obj;
            x.match = o.match;
            x.ok = o.ok;
            x.len = o.oracle_len;
            x.symbolic = o.symbolic_insns;
            x.forced_raw = o.forced_raw;
            x.raw_fallback = o.raw_fallback;
            x.diffs = o.n_diffs;
            r.objects.append(x);
            r.n_exact += o.match ? 1 : 0;
        }
        r.byte_exact = rep.byte_exact;
        r.n_obj = r.objects.size();
        r.symbolic = rep.symbolic;
        r.forced_raw = rep.forced_raw;
        r.raw_fallback_objs = rep.raw_fallback_objs;
        r.seconds = round1(since(t0));
        e.record = r;
    } catch (const std::exception& ex) {
        e.error = head(py::str(std::string(ex.what())), 120);
        e.trace = tail(py::str(std::string(typeid(ex).name()) + ": " + ex.what()), 300);
    }
    return e;
}

py::str dump_results(const Results& results) {
    if (results.empty()) return py::str("{}");
    std::string o = "{\n";
    const auto items = results.items();
    for (size_t i = 0; i < items.size(); ++i) {
        o += pad(1) + jstr(std::get<0>(items[i])) + ": " + dump_entry(std::get<1>(items[i])) + (i + 1 < items.size() ? ",\n" : "\n");
    }
    return py::str(o + "}");
}

void _save(const Results& results, const py::str& path) {
    const py::str tmp = path + py::str(".tmp");
    {
        // json.dump(results, open(tmp, "w"), indent=1)   (a text-mode write: "\n" is "\r\n")
        py::file f = py::open(tmp, py::str("w"), py::str("cp1252"));
        f.write(dump_results(results));
        f.close();
    }
    py::os::replace(tmp, path);
}

void _summary(std::ostream& out, const Results& results, double secs) {
    int64_t n_mods = 0, full = 0, tobj = 0, eobj = 0, sym = 0, fb = 0, errs = 0;
    for (const auto& kv : results.items()) {
        const Entry& e = std::get<1>(kv);
        if (e.error) {
            errs += 1;
            continue;
        }
        n_mods += 1;
        if (e.record) {
            full += e.record->byte_exact ? 1 : 0;
            tobj += e.record->n_obj;
            eobj += e.record->n_exact;
            sym += e.record->symbolic;
            fb += e.record->raw_fallback_objs;
        }
    }
    out << py::percent(py::str("\n== corpus: %d modules | %d fully byte-exact | %d/%d objects byte-exact | %d symbolic re-encodes | %d raw-fallback objs | %d errors | %.0fs =="),
                       n_mods, full, eobj, tobj, sym, fb, errs, secs)
               .utf8()
        << std::endl;
}

Results run(std::ostream& out, const py::str& srcdir, const py::str& out_json, const std::optional<py::list<py::str>>& redo_in, int64_t workers) {
    py::set<py::str> redo;
    if (redo_in)
        for (const py::str& r : *redo_in) redo.add(r);
    Results results;
    if (py::os::path::exists(out_json)) results = load_results(out_json);
    std::vector<py::str> cand;
    for (const py::str& pat : _VXD_EXTS)
        for (const py::str& p : py::glob::glob(py::os::path::join(srcdir, pat))) cand.push_back(p);
    for (const py::str& f : _EXTRA_FILES) cand.push_back(py::os::path::join(srcdir, f));
    std::vector<py::str> uniq;
    for (const py::str& p : cand)
        if (std::find(uniq.begin(), uniq.end(), p) == uniq.end()) uniq.push_back(p);
    std::sort(uniq.begin(), uniq.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    std::vector<py::str> files;
    for (const py::str& p : uniq)
        if (py::os::path::exists(p) && _is_le_vxd(p)) files.push_back(p);
    using Item = std::tuple<py::str, py::str, le::LEFile>;
    py::list<Item> todo;
    for (const py::str& p : files) {
        py::list<std::tuple<py::str, le::LEFile>> mods;
        try {
            mods = le::open_vxds(p);
        } catch (const std::exception& e) {
            Entry er;
            er.error = head(py::str(std::string(e.what())), 80);
            results.set(py::os::path::basename(p), er);
            continue;
        }
        for (const auto& nv : mods) {
            const py::str key = std::get<0>(nv).lower();
            if (results.contains(key) && !redo.contains(key) && !results.at(key).error) continue;   // resume: already recorded
            todo.append(Item(key, std::get<0>(nv), std::get<1>(nv)));
        }
    }
    const auto t_all = clk::now();
    out << py::percent(py::str("== %d modules to process, %d workers =="), todo.size(), workers).utf8() << std::endl;
    jobs::pool::AsDoneOptions<Item, Entry> o;
    o.jobs = workers;
    jobs::pool::as_done(
        [](const Item& t) { return _one_module(std::get<1>(t), std::get<2>(t)); }, todo,
        [&](const Item& t, const Entry& r) {
            const py::str& key = std::get<0>(t);
            const py::str& name = std::get<1>(t);
            results.set(key, r);
            _save(results, out_json);
            if (r.error) {
                out << py::percent(py::str("  %-14s ERROR %s"), name, head(*r.error, 60)).utf8() << std::endl;
            } else {
                const ModuleRecord& m = *r.record;
                out << py::percent(py::str("  %-14s %2d/%2d byte-exact  sym=%-6d raw=%-5d fb=%d  %-10s [%ds]"), name, m.n_exact, m.n_obj, m.symbolic,
                                   m.forced_raw, m.raw_fallback_objs, m.byte_exact ? py::str("BYTE-EXACT") : py::str("PARTIAL"), static_cast<int64_t>(m.seconds))
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
        outp = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxd"), py::str("byte_exact.json"));
        py::os::makedirs(py::os::path::dirname(outp), 0777, true);
    }
    std::optional<py::list<py::str>> redo;
    py::list<py::str> redo_l;
    int64_t workers = 4;
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == py::str("--redo")) {
            if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
            redo_l = py::split(argv[i + 1], py::str(","));
            redo = redo_l;
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
    run(out, srcdir, outp, redo ? redo : std::optional<py::list<py::str>>(py::list<py::str>()), workers);
    return 0;
}

}  // namespace codebreaker::source::vxd::corpus
