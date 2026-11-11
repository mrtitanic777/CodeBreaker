// codebreaker/source/ne_asm/sweep.cpp -- typed C++ of codebreaker.source.ne_asm.sweep.

#include "codebreaker/source/ne_asm/sweep.hpp"

#include <algorithm>
#include <memory>
#include <ostream>
#include <set>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/ne_asm/idiom.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "codebreaker/symbols/debug_build.hpp"
#include "py_glob.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::ne_asm::sweep {

py::str Corpus::resolve(const py::str& module) {
    // (the Python's `corpus.resolve(m) or corpus.retail(m) or m` never reached its fallbacks: resolve() raises
    // ValueError for an unknown name. Here the fallbacks are what they say.)
    try {
        return base::corpus::resolve(module);
    } catch (const py::ValueError&) {
        const std::optional<py::str> r = base::corpus::retail(module);
        return r ? *r : module;
    }
}

Corpus _target() { return Corpus(); }

py::str ROOT() { return py::settings::checkout_root(); }

void _setup() { py::os::chdir(ROOT()); }

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
py::str state_(const py::str& a, const py::str& b, const py::str& c) {
    const py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b, c);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// open(path, "w", newline="\r\n").write(text)
void write_crlf(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str("\r\n"));
    f.write(text);
    f.close();
}

// the tag of a module: its name without the extension, lower-cased
py::str tag_of(const py::str& module) { return std::get<0>(py::os::path::splitext(py::os::path::basename(module))).lower(); }

}  // namespace

py::list<std::tuple<py::str, py::str>> ne_modules() {
    _setup();
    const py::str base = base::corpus::root();
    py::set<py::str> seen;
    py::list<std::tuple<py::str, py::str>> out;
    const char* const exts[] = {"exe", "dll", "drv", "386", "cpl", "scr", "fon"};
    // (the corpus folder holds WINDOWS/SYSTEM, not SYSTEM: the Python found no module at all; the old layouts stay)
    for (const py::str& sub : {base + py::str("/WINDOWS/SYSTEM"), base + py::str("/WINDOWS/SYSTEM32"), base + py::str("/WINDOWS"),
                               base + py::str("/WINDOWS/COMMAND"), base + py::str("/SYSTEM"), base + py::str("/SYSTEM32"), base,
                               base + py::str("/COMMAND")}) {
        for (const char* ext : exts) {
            std::vector<py::str> found;
            for (const py::str& p : py::glob::glob(py::os::path::join(sub, py::str(std::string("*.") + ext)))) found.push_back(p);
            std::sort(found.begin(), found.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            for (const py::str& p : found) {
                const py::str nm = py::os::path::basename(p).lower();
                if (seen.contains(nm)) continue;
                seen.add(nm);
                const std::optional<py::str> fmt = formats::detect::format_of(p);
                if (fmt && *fmt == py::str("NE")) out.append(std::make_tuple(nm, p));
            }
        }
    }
    return out;
}

py::str _resolve(const py::str& module) {
    try {
        return _target().resolve(module);
    } catch (const std::exception&) {
        const std::optional<py::str> h = base::corpus::retail(module);
        return h ? *h : module;
    }
}

ModuleRecord sweep_module(const py::str& module, const py::str& path, const py::str& backend, const std::optional<py::str>& emit) {
    const formats::ne::NEFile ne(path);
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    const py::str tag = tag_of(module);
    const py::str nj = state_(py::str("idiomatic"), tag, py::str("names.json"));
    py::dict<int64_t, py::dict<int64_t, py::str>> ov;
    if (py::os::path::exists(nj)) ov = symbols::debug_build::load_override(nj);
    ModuleRecord rec;
    rec.module = module;
    const auto segs = ne.segments();
    for (int64_t i = 1; i <= segs.size(); ++i) {
        if (segs[i - 1].kind != py::str("CODE")) continue;
        std::optional<py::dict<int64_t, py::str>> over;
        if (ov.contains(i)) over = ov.at(i);
            const segment::Seg sg(ne, i, o, over);
            const int64_t n = sg.code.size();
        if (n == 0) continue;
        const idiom::Verified r = idiom::verify_range(sg, 0, n, std::nullopt, py::str("."), backend);
            rec.segs.append(SegRecord{i, n, r.ok, r.n_code, r.db, r.live});
            if (emit && !emit->empty() && r.ok) {
            const py::str d = py::os::path::join(*emit, tag);
            py::os::makedirs(d, 0777, true);
            write_crlf(py::os::path::join(d, py::percent(py::str("%s_seg%02d.asm"), tag, i)), r.asm_);
        }
    }
    rec.ok = !rec.segs.empty();
    for (const SegRecord& s : rec.segs) {
        rec.ok = rec.ok && s.ok;
        rec.nok += s.ok ? 1 : 0;
        rec.code += s.n_code;
        rec.live += s.live;
    }
    rec.nseg = rec.segs.size();
    return rec;
}

namespace {

// ---- json.dump(..., indent=1) of the ledger
std::string pad(int n) { return std::string(static_cast<size_t>(n), ' '); }
std::string jstr(const py::str& s) {
    std::string o;
    py::json::put(o, s);
    return o;
}
std::string jbool(bool b) { return b ? "true" : "false"; }

std::string dump_record(const ModuleRecord& r, int d) {
    std::string o = "{\n" + pad(d + 1) + "\"module\": " + jstr(r.module) + ",\n" + pad(d + 1) + "\"segs\": ";
    if (r.segs.empty()) {
        o += "[]";
    } else {
        o += "[\n";
        for (int64_t i = 0; i < r.segs.size(); ++i) {
            const SegRecord& s = r.segs[i];
            o += pad(d + 2) + "{\n" + pad(d + 3) + "\"seg\": " + std::to_string(s.seg) + ",\n" + pad(d + 3) + "\"n\": " + std::to_string(s.n) + ",\n" +
                 pad(d + 3) + "\"ok\": " + jbool(s.ok) + ",\n" + pad(d + 3) + "\"n_code\": " + std::to_string(s.n_code) + ",\n" + pad(d + 3) +
                 "\"db\": " + std::to_string(s.db) + ",\n" + pad(d + 3) + "\"live\": " + std::to_string(s.live) + "\n" + pad(d + 2) + "}" +
                 (i + 1 < r.segs.size() ? "," : "") + "\n";
        }
        o += pad(d + 1) + "]";
    }
    o += ",\n" + pad(d + 1) + "\"ok\": " + jbool(r.ok) + ",\n" + pad(d + 1) + "\"nseg\": " + std::to_string(r.nseg) + ",\n" + pad(d + 1) +
         "\"nok\": " + std::to_string(r.nok) + ",\n" + pad(d + 1) + "\"code\": " + std::to_string(r.code) + ",\n" + pad(d + 1) +
         "\"live\": " + std::to_string(r.live) + "\n" + pad(d) + "}";
    return o;
}

}  // namespace

py::list<Summary> sweep(std::ostream& out, std::optional<int64_t> limit, int64_t start, const std::optional<py::str>& emit, const py::str& backend) {
    py::list<std::tuple<py::str, py::str>> all = ne_modules();
    // mods = ne_modules()[start:]; if limit: mods = mods[:limit]   (Python slices: negative ends count from the end)
    const int64_t n_all = all.size();
    auto norm = [&](int64_t i, int64_t n) { return i < 0 ? std::max<int64_t>(0, i + n) : std::min(i, n); };
    std::vector<std::tuple<py::str, py::str>> mods;
    for (int64_t i = norm(start, n_all); i < n_all; ++i) mods.push_back(all[i]);
    if (limit && *limit != 0) {
        const int64_t e = norm(*limit, static_cast<int64_t>(mods.size()));
        mods.resize(static_cast<size_t>(e));
    }
    if (emit && !emit->empty()) py::os::makedirs(*emit, 0777, true);
    out << py::percent(py::str("NE idiomatic sweep: %d NE modules (backend=%s)\n"), static_cast<int64_t>(mods.size()), backend).utf8() << "\n";
    int64_t t_mods = 0, t_full = 0, t_segs = 0, t_segok = 0, t_code = 0, t_live = 0, t_err = 0;
    py::list<Summary> summary;
    for (const auto& m : mods) {
        const py::str& nm = std::get<0>(m);
        try {
            const ModuleRecord rec = sweep_module(nm, std::get<1>(m), backend, emit);
            t_mods += 1;
            t_full += rec.ok ? 1 : 0;
            t_segs += rec.nseg;
            t_segok += rec.nok;
            t_code += rec.code;
            t_live += rec.live;
            summary.append(Summary{nm, rec, std::nullopt});
            out << py::percent(py::str("  %-16s segs %d/%d exact  %d instrs  %.0f%% live%s"), nm, rec.nok, rec.nseg, rec.code,
                               100.0 * static_cast<double>(rec.live) / static_cast<double>(std::max<int64_t>(1, rec.code)),
                               rec.ok ? py::str("") : py::str("  (partial)"))
                       .utf8()
                << "\n";
        } catch (const std::exception& e) {
            t_err += 1;
            const py::str msg = py::str(std::string(e.what()));
            summary.append(Summary{nm, std::nullopt, msg.slice(0, 200)});
            out << py::percent(py::str("  %-16s ERROR %s"), nm, msg.slice(0, 70)).utf8() << "\n";
        }
    }
    out << "\n" << std::string(60, '=') << "\n";
    out << "NE IDIOMATIC SWEEP -- " << t_mods << " modules\n";
    out << "  fully byte-exact modules : " << t_full << "/" << t_mods << "\n";
    out << "  CODE segments byte-exact : " << t_segok << "/" << t_segs << "\n";
    out << py::percent(py::str("  instructions             : %d  (%.1f%% live idiomatic, rest db-pinned)"), t_code,
                       100.0 * static_cast<double>(t_live) / static_cast<double>(std::max<int64_t>(1, t_code)))
               .utf8()
        << "\n";
    out << "  module errors            : " << t_err << "\n";
    if (emit && !emit->empty()) {
        // json.dump(dict(totals=tot, modules=summary), f, indent=1)  (a text-mode write: "\n" is "\r\n")
        std::string j = "{\n \"totals\": {\n  \"mods\": " + std::to_string(t_mods) + ",\n  \"full\": " + std::to_string(t_full) + ",\n  \"segs\": " +
                        std::to_string(t_segs) + ",\n  \"segok\": " + std::to_string(t_segok) + ",\n  \"code\": " + std::to_string(t_code) +
                        ",\n  \"live\": " + std::to_string(t_live) + ",\n  \"err\": " + std::to_string(t_err) + "\n },\n \"modules\": ";
        if (summary.empty()) {
            j += "[]";
        } else {
            j += "[\n";
            for (int64_t i = 0; i < summary.size(); ++i) {
                const Summary& s = summary[i];
                if (s.record) {
                    j += "  " + dump_record(*s.record, 2);
                } else {
                    j += "  {\n   \"module\": " + jstr(s.module) + ",\n   \"error\": " + jstr(*s.error) + "\n  }";
                }
                j += (i + 1 < summary.size() ? ",\n" : "\n");
            }
            j += " ]";
        }
        j += "\n}";
        write_crlf(py::os::path::join(*emit, py::str("_ne_idiom_summary.json")), py::str(j));
        out << "  wrote _ne_idiom_summary.json\n";
    }
    return summary;
}

namespace {
const char* const USAGE = "usage: ne_sweep.py [-h] [--limit LIMIT] [--start START] [--emit EMIT] [--backend {nasm,ml}] [--list]\n";
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    std::optional<int64_t> limit;
    int64_t start = 0;
    std::optional<py::str> emit;
    py::str backend("nasm");
    bool list = false;
    auto fail = [&](const std::string& msg) {
        err << USAGE << "ne_sweep.py: error: " << msg << "\n";
        return int64_t(2);
    };
    auto negative_number = [](const py::str& s) { return static_cast<bool>(py::re::match(py::str("-\\.?\\d"), s)); };
    // the value of `--name value` / `--name=value` at argv[i] (advancing i); `found` says whether this was that option
    auto value_of = [&](int64_t& i, const char* name, bool& found, std::optional<py::str>& val) {
        const py::str a = argv[i];
        const py::str nm(name);
        found = false;
        if (a == nm) {
            found = true;
            if (i + 1 >= argv.size() || (argv[i + 1].startswith(py::str("-")) && py::len(argv[i + 1]) > 1 && !negative_number(argv[i + 1]))) {
                val = std::nullopt;
            } else {
                val = argv[++i];
            }
        } else if (a.startswith(nm + py::str("="))) {
            found = true;
            val = a.slice(py::len(nm) + 1, py::len(a));
        }
    };
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str a = argv[i];
        if (a == py::str("-h") || a == py::str("--help")) {
            out << USAGE;
            return 0;
        }
        bool found = false;
        std::optional<py::str> val;
        value_of(i, "--limit", found, val);
        if (found) {
            if (!val) return fail("argument --limit: expected one argument");
            try {
                limit = py::int_parse(*val);
            } catch (const py::ValueError&) {
                return fail("argument --limit: invalid int value: " + py::repr(*val).utf8());
            }
            continue;
        }
        value_of(i, "--start", found, val);
        if (found) {
            if (!val) return fail("argument --start: expected one argument");
            try {
                start = py::int_parse(*val);
            } catch (const py::ValueError&) {
                return fail("argument --start: invalid int value: " + py::repr(*val).utf8());
            }
            continue;
        }
        value_of(i, "--emit", found, val);
        if (found) {
            if (!val) return fail("argument --emit: expected one argument");
            emit = val;
            continue;
        }
        value_of(i, "--backend", found, val);
        if (found) {
            if (!val) return fail("argument --backend: expected one argument");
            if (*val != py::str("nasm") && *val != py::str("ml"))
                return fail("argument --backend: invalid choice: " + py::repr(*val).utf8() + " (choose from 'nasm', 'ml')");
            backend = *val;
            continue;
        }
        if (a == py::str("--list")) {
            list = true;
        } else {
            return fail("unrecognized arguments: " + a.utf8());
        }
    }
    _setup();
    if (list) {
        for (const auto& m : ne_modules()) out << std::get<0>(m).utf8() << "\n";
        return 0;
    }
    sweep(out, limit, start, emit, backend);
    return 0;
}

}  // namespace codebreaker::source::ne_asm::sweep
