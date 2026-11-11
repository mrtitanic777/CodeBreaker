// codebreaker/exact/seed.cpp -- typed C++ of codebreaker.exact.seed (see seed.hpp).

#include "codebreaker/exact/seed.hpp"

#include <algorithm>

namespace codebreaker::exact::seed {

const py::str FUNC = py::str(
    "(?m)^((?:static\\s+)?(?:(?:unsigned|struct\\s+\\w+|union\\s+\\w+|[A-Za-z_]\\w*)\\s+\\**\\s*)?\\**\\s*([A-Za-z_]\\w*)\\s*\\([^;{]*\\)\\s*)$");

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

str join(const str& sep, const py::list<str>& xs) {
    std::u32string out;
    for (int64_t i = 0; i < xs.size(); ++i) {
        if (i) out += sep.u32();
        out += xs[i].u32();
    }
    return str(out);
}

py::list<str> slice_lines(const py::list<str>& l, int64_t lo, int64_t hi) { return l.slice(lo, hi); }

str read_latin1(const str& p) {
    py::file f = py::open(p, str("r"), str("latin1"));
    str t = f.read_text();
    f.close();
    return t;
}

}  // namespace

Split split(const str& text) {
    py::list<str> lines = py::split(text, str("\n"));
    Split out;
    int64_t k = 0;
    py::list<str> pending;                                  // (a remark just before a function: its own)
    const int64_t n = lines.size();
    while (k < n) {
        str l = lines[k];
        std::optional<re::Match> m;
        if (!l.startswith(std::make_tuple(" ", "\t", "#", "}"))) m = re::match(FUNC, l);
        if (m) {
            str mname = m->group(2);
            if (!(mname == "if" || mname == "while" || mname == "for" || mname == "switch" || mname == "return" || mname == "sizeof")) {
                int64_t j = k;
                while (j < n && !lines[j].startswith(str("{"))) {   // (K&R: the parameters' declarations)
                    if (lines[j].rstrip().endswith(str(";")) && j > k && lines[j].startswith(std::make_tuple("extern", "static"))) break;
                    ++j;
                }
                if (j < n && lines[j].startswith(str("{"))) {
                    int64_t e = j;
                    int64_t depth = 0;
                    while (e < n) {
                        depth += lines[e].count(str("{")) - lines[e].count(str("}"));
                        if ((depth <= 0 && e > j) || (depth <= 0 && lines[e].strip() == "}")) break;
                        ++e;
                    }
                    py::list<str> body = pending + slice_lines(lines, k, e + 1);
                    out.funcs.set(mname, join(str("\n"), body) + str("\n"));
                    str head = l.slice(0, l.find(mname)).replace(str("static"), str("")).strip();
                    if (!head.empty() && head != "int") out.types.set(mname, head);
                    pending = py::list<str>();
                    k = e + 1;
                    continue;
                }
            }
        }
        if (l.startswith(str("/*")) && !l.rstrip().endswith(str(";"))) {
            py::list<str> r;
            r.append(l);
            while (!lines[k].contains(str("*/")) && k + 1 < n) {
                ++k;
                r.append(lines[k]);
            }
            pending = r;
            ++k;
            continue;
        }
        if (!l.strip().empty()) {
            for (const str& x : pending) out.decls.append(x);
            out.decls.append(l);
        }
        pending = py::list<str>();
        ++k;
    }
    return out;
}

py::dict<int64_t, str> seeds(project::Project& prj, project::Project& other) {
    names::Carried c = names::carry(prj, other);
    const py::dict<int64_t, int64_t>& pairs = c.pairs;
    const py::dict<int64_t, int64_t>& dpairs = c.dpairs;
    auto kn = names::known(other);
    py::dict<int64_t, str> ocode = std::get<0>(kn), odata = std::get<1>(kn);
    py::dict<str, Split> files;
    for (const auto& part : other.sets()) {
        for (const auto& s : part->sources()) {
            const str& path = std::get<1>(s);
            if (path.lower().endswith(str(".c"))) files.set(path, seed::split(read_latin1(path)));
        }
    }
    py::dict<str, str> where;
    for (const auto& kv : files.items()) {
        for (const str& n : std::get<1>(kv).funcs.keys()) where.setdefault(n, std::get<0>(kv));
    }
    auto cname = [](const str& n) { return n.startswith(str("_")) ? n.slice(1, n.size()) : n; };
    py::dict<str, str> rename;                              // (OTHER's address-names - for wherever OTHER's names were taken from - as this program's places)
    for (const auto& kv : pairs.items()) {
        int64_t a = std::get<0>(kv), b = std::get<1>(kv);
        str n = cname(ocode.get(b, str("")));
        if (re::fullmatch(str("f[0-9a-f]{5}"), n)) rename.set(n, py::percent(str("f%05x"), a));
    }
    for (const auto& kv : dpairs.items()) {
        int64_t a = std::get<0>(kv), b = std::get<1>(kv);
        str n = cname(odata.get(b, str("")));
        if (re::fullmatch(str("g_[0-9a-f]{1,4}"), n)) rename.set(n, py::percent(str("g_%04x"), a));
    }
    const str rx("\\b(f[0-9a-f]{5}|g_[0-9a-f]{1,4})\\b");
    py::dict<int64_t, str> out;
    for (const auto& kv : py::sorted(py::list<std::tuple<int64_t, int64_t>>::from(pairs.items()))) {
        int64_t a = std::get<0>(kv), b = std::get<1>(kv);
        str n = cname(ocode.get(b, py::percent(str("f%05x"), b)));
        str fb = py::percent(str("f%05x"), b);
        std::optional<str> path;
        if (where.contains(n) && !where.at(n).empty()) path = where.at(n);
        else if (where.contains(fb) && !where.at(fb).empty()) path = where.at(fb);
        if (!path) continue;
        const Split& sp = files.at(*path);
        str fn;
        if (sp.funcs.contains(n) && !sp.funcs.at(n).empty()) fn = sp.funcs.at(n);
        else if (sp.funcs.contains(fb)) fn = sp.funcs.at(fb);
        else throw py::TypeError("'NoneType' object is not subscriptable");
        py::list<str> others;
        for (const auto& tf : sp.types.items()) {
            const str& f = std::get<0>(tf);
            if (f != n && re::search(py::percent(str("\\b%s\\s*\\("), f), fn)) {
                others.append(py::percent(str("extern %s %s();"), std::get<1>(tf), f));
            }
        }
        str text = py::percent(str("/* seeded from %s %s */\n"), py::os::path::basename(other.exe), py::os::path::relpath(*path, other.dir)) +
                   join(str("\n"), sp.decls + others) + str("\n\n") + fn;
        std::u32string res;
        int64_t last = 0;
        for (const re::Match& m : re::finditer(rx, text)) {
            res += text.slice(last, m.start()).u32();
            str g = m.group(1);
            res += (rename.contains(g) ? rename.at(g) : g).u32();
            last = m.end();
        }
        res += text.slice(last, text.size()).u32();
        out.set(a, str(res));
    }
    return out;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    auto say = [&](const str& s) { out << s.utf8() << "\n"; };
    if (!argv.contains(str("--from"))) {
        say(str("seed: --from OTHER (a project of another version of the program, with sources)"));
        return 1;
    }
    str f = argv[argv.index(str("--from")) + 1];
    auto other = project::load(py::os::path::isabs(f) ? f : py::os::path::join(py::os::getcwd(), f));
    py::dict<int64_t, str> sd = seeds(prj, *other);
    py::set<int64_t> have;
    for (const auto& s : prj.sources()) have.add(std::get<0>(s));
    py::dict<int64_t, str> nw;
    for (const auto& kv : sd.items()) {
        if (!have.contains(std::get<0>(kv))) nw.set(std::get<0>(kv), std::get<1>(kv));
    }
    say(py::percent(str("%d functions seeded from %s (%d the project has a file for already)"), sd.size(), py::os::path::basename(other->exe),
                    sd.size() - nw.size()));
    if (argv.contains(str("--write"))) {
        py::os::makedirs(prj.src, 0777, true);
        for (const auto& part : other->sets()) {            // (the headers the sources include, when this has none)
            for (const str& h : py::os::listdir(part->src)) {
                if (h.lower().endswith(std::make_tuple(".h", ".inc", ".lit")) && !py::os::path::exists(prj.path(h))) {
                    py::file s = py::open_rb(py::os::path::join(part->src, h));
                    py::file d = py::open(prj.path(h), str("wb"));
                    d.write(s.read());
                    d.close();
                    s.close();
                }
            }
        }
        for (const auto& kv : nw.items()) {
            py::file fh = py::open(prj.path(py::percent(str("f%05x.c"), std::get<0>(kv))), str("w"), std::nullopt, std::nullopt, str("\n"));
            fh.write(std::get<1>(kv));
            fh.close();
        }
        say(py::percent(str("written: %d files in %s"), nw.size(), prj.src));
    }
    return 0;
}

}  // namespace codebreaker::exact::seed
