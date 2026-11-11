// codebreaker/exact/pascal_join.cpp -- C++17 port of codebreaker.exact.pascal_join (see pascal_join.hpp).

#include "codebreaker/exact/pascal_join.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "codebreaker/exact/habits/cmpp.hpp"
#include "codebreaker/exact/habits/newcomp.hpp"
#include "codebreaker/exact/habits/oldcomp.hpp"
#include "codebreaker/exact/omf.hpp"
#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::pascal_join {

namespace compare = codebreaker::exact::compare;
namespace omf = codebreaker::exact::omf;
namespace cmpp = codebreaker::exact::habits::cmpp;
namespace newcomp = codebreaker::exact::habits::newcomp;
namespace oldcomp = codebreaker::exact::habits::oldcomp;
namespace path = py::os::path;

const py::str NL("\n");

namespace {

// the file's text as open(path, encoding="latin1").read()
py::str read_latin1(const py::str& p) {
    py::file f = py::open(p, py::str("r"), py::str("latin1"));
    return f.read_text();
}

// the file's text as open(path).read() (the default codec)
py::str read_default(const py::str& p) {
    py::file f = py::open(p);
    return f.read_text();
}

// json.load(open(path)) as a value
py::V load_json(const py::str& p) { return py::json_parse(read_default(p).utf8()); }

// the regular expressions of the module, compiled once
const py::re::Pattern& re_piece_name() { static const py::re::Pattern p(py::str("p[0-9a-f]{5}\\.pas"), 0); return p; }
const py::re::Pattern& re_nested() { static const py::re::Pattern p(py::str("\\s*\\{\\+(p[0-9a-f]{5})\\}\\s*"), 0); return p; }
const py::re::Pattern& re_begin() { static const py::re::Pattern p(py::str("BEGIN\\b"), 0); return p; }
const py::re::Pattern& re_g_decl() { static const py::re::Pattern p(py::str("  G_[0-9A-F]{4}"), 0); return p; }
const py::re::Pattern& re_g_join() { static const py::re::Pattern p(py::str(";\\s+(?=G_[0-9A-F]{4})"), 0); return p; }
const py::re::Pattern& re_function() {
    static const py::re::Pattern p(py::str("^FUNCTION +(P[0-9A-F]{5})(\\([^)]*\\))?: *([A-Z_0-9]+)"), py::re::M);
    return p;
}
const py::re::Pattern& re_stub_head() { static const py::re::Pattern p(py::str("(PROCEDURE|FUNCTION) +(P[0-9A-F]{5})"), 0); return p; }
const py::re::Pattern& re_ident() { static const py::re::Pattern p(py::str("[A-Z_][A-Z_0-9]*"), 0); return p; }
const py::re::Pattern& re_proc_line() {
    static const py::re::Pattern p(py::str("\\s*(?:PROCEDURE|FUNCTION) +P([0-9A-F]{5})"), 0);
    return p;
}
const py::re::Pattern& re_proc_name() { static const py::re::Pattern p(py::str("P([0-9A-F]{5})"), 0); return p; }

// a key of a project.json record that is required
[[noreturn]] void missing_key() { throw py::KeyError("key not found"); }

}  // namespace

Compiland::Compiland(const Project& prj_, const py::str& name_, const CompilandCfg& u_)
    : prj(prj_), name(name_), u(u_), over(false) {
    py::str root = path::join(prj.dir, prj.cfg.root);
    py::list<py::str> over_list = u.over ? *u.over : prj.cfg.over;
    for (const py::str& o : over_list) {
        dirs.append(path::join(root, o, u.dir));
    }
    py::list<py::str> own = u.dirs ? *u.dirs : py::list<py::str>{u.dir};
    for (const py::str& d : own) {
        dirs.append(path::join(root, d));
    }
    src = dirs[-1];
    headline = py::percent(u.kind == py::str("PROGRAM") ? "PROGRAM %s (INPUT, OUTPUT);" : "MODULE %s;", name);
    empty = u.kind == py::str("PROGRAM") ? py::str("BEGIN") + NL + py::str("END.") + NL : py::str("END.") + NL;
    if (prj.cfg.map) {   // (pieces of another program's: where each is in this one)
        py::str mp = path::join(root, *prj.cfg.map);
        if (path::exists(mp)) {
            py::V all = load_json(mp);
            const py::V& mine = all.get(py::V(name.utf8()));
            if (mine.is_dict()) {
                for (const auto& kv : mine.items()) map.set(py::str(kv.first.str()), py::str(kv.second.str()));
            }
        }
    }
    over = !over_list.empty();
    if (over) {
        // EDITS (FILE|old|new, FILE|+|a line; * every piece, *~ a regular expression), lowest overlay first
        for (int64_t i = py::len(dirs) - 1; i >= 0; --i) {
            py::str e = path::join(dirs[i], py::str("EDITS"));
            if (!path::exists(e)) continue;
            py::str text = read_latin1(e);
            for (const py::str& l : py::split(text, NL)) {
                if (l.count(py::str("|")) == 2) {
                    py::list<py::str> f = py::split(l.rstrip(NL), py::str("|"));
                    edits.append(std::make_tuple(f[0], f[1], f[2]));
                }
            }
        }
        for (const py::str& d : dirs) {
            py::str r = path::join(d, py::str("REMOVED"));
            if (path::exists(r)) {
                for (const py::str& x : py::split(read_default(r))) removed.add(x);
            }
        }
    }
    if (prj.cfg.kinds) {   // (each procedure with the stack check or without)
        py::str kp = path::join(root, *prj.cfg.kinds);
        if (path::exists(kp)) {
            py::V all = load_json(kp);
            for (const auto& kv : all.items()) {
                const py::V& k = kv.second.get(py::V("kind"));
                std::optional<py::str> kind;
                if (k.is_str()) kind = py::str(k.str());
                kinds.set(py::int_parse(py::str(kv.first.str()), 16), kind);
            }
        }
    }
}

int64_t Compiland::addr_of(const py::str& h) const {
    if (!map.empty() && map.contains(h.upper())) {
        return py::int_parse(map.at(h.upper()), 16);
    }
    return py::int_parse(h, 16);
}

py::str Compiland::where(const py::str& f) const {
    for (const py::str& d : dirs) {
        if (path::exists(path::join(d, f))) {
            return path::join(d, f);
        }
    }
    return path::join(src, f);
}

py::list<py::str> Compiland::pieces() const {
    py::set<py::str> ps;
    for (const py::str& d : dirs) {
        if (!path::isdir(d)) continue;
        for (const py::str& f : py::os::listdir(d)) {
            if (re_piece_name().fullmatch(f)) ps.add(f);
        }
    }
    std::vector<py::str> keep;
    for (const py::str& f : ps) {
        if (!removed.contains(f)) keep.push_back(f);
    }
    // sorted(ps, key=...): the key is the map's place of the piece, else the piece's name; names that give equal keys
    // are in the Python's (hash) order of the set -- here in name order
    auto key_of = [&](const py::str& f) -> py::str {
        if (map.empty()) return f;
        py::str a = f.slice(1, 6);
        return map.get(a.upper(), a);
    };
    std::stable_sort(keep.begin(), keep.end(), [&](const py::str& a, const py::str& b) {
        py::str ka = key_of(a), kb = key_of(b);
        if (ka < kb) return true;
        if (kb < ka) return false;
        return a < b;
    });
    return py::list<py::str>(keep);
}

py::str Compiland::check(const py::str& f) const {
    if (kinds.empty()) {
        return py::str("");
    }
    py::str h = f.slice(1, 6);
    std::optional<int64_t> a;
    if (map.empty() || map.contains(h.upper())) a = addr_of(h);
    std::optional<py::str> k;
    if (a && kinds.contains(*a)) k = kinds.at(*a);
    if (!k) return py::str("");
    return py::str(*k == py::str("check") ? "{$STACKCK+}" : "{$STACKCK-}") + NL;
}

py::str Compiland::expand(const py::str& text, const py::str& d, const std::optional<py::str>& own) const {
    py::list<py::str> out;
    bool again = false;
    for (const py::str& line : py::split(text, NL)) {
        auto m = re_nested().fullmatch(line);
        if (m) {
            py::str nested = m->group(1);
            out.append(check(nested) + expand(read_latin1(where(d + py::str("/") + nested + py::str(".pas"))).rstrip(NL), d, nested));
            again = true;
        } else {
            if (again && own && !own->empty() && re_begin().match(line)) {
                out.append(check(*own).rstrip(NL));   // (back to the outer procedure's own setting)
                again = false;
            }
            out.append(line);
        }
    }
    return py::join(NL, out);
}

py::str Compiland::piece(const py::str& f) const {
    py::str t = check(f) + expand(read_latin1(where(f)).rstrip(NL), f.slice(0, -4) + py::str(".d"), f) + NL;
    for (const auto& [name_, old, new_] : edits) {
        if (name_ == py::str("*")) {
            t = t.replace(old, new_);
        }
        if (name_ == py::str("*~")) {
            t = py::re::sub(old, new_, t);
        }
    }
    return t;
}

py::str Compiland::rd(const py::str& f) const {
    py::str p = where(f);
    py::str t = path::exists(p) ? read_latin1(p).rstrip(NL) + NL : py::str("");
    for (const auto& [name_, old, new_] : edits) {
        if (name_ == f) {
            if (old == py::str("+")) {
                t = t + new_ + NL;
            } else if (t.contains(old)) {
                t = t.replace(old, new_);
            }
        }
    }
    if (f == py::str("GLOBALS.pas") && !edits.empty()) {   // (one declaration a line again)
        py::list<py::str> lines;
        for (const py::str& l : py::split(t, NL)) {
            lines.append(re_g_decl().match(l) ? re_g_join().sub(py::str(";") + NL + py::str("  "), l) : l);
        }
        t = py::join(NL, lines);
    }
    if (f == py::str("MAIN.pas")) {
        for (const auto& [name_, old, new_] : edits) {
            if (name_ == py::str("*")) {
                t = t.replace(old, new_);
            }
            if (name_ == py::str("*~")) {
                t = py::re::sub(old, new_, t);
            }
        }
    }
    return u.stackck ? t.replace(py::str(" {$STACKCK-}"), py::str("")) : t;
}

py::dict<py::str, py::str> Compiland::result_types() const {
    py::dict<py::str, py::str> out;
    py::list<py::str> texts;
    texts.append(rd(py::str("FORWARDS.pas")));
    for (const py::str& f : pieces()) texts.append(rd(f));
    for (const py::str& t : texts) {
        for (const auto& m : re_function().finditer(t)) {
            out.set(m.group(1), m.group(3));
        }
    }
    return out;
}

py::str Compiland::stub(const py::str& f, const py::dict<py::str, py::str>& rt) const {
    py::str head = py::split(rd(f), NL)[0];
    auto m = re_stub_head().match(head);
    py::str body("");
    if (m && m->group(1) == py::str("FUNCTION")) {
        py::str t = rt.get(m->group(2), py::str("WORD"));
        body = py::percent("  %s := %s", m->group(2),
                           t == py::str("BOOLEAN") ? py::str("FALSE")
                                                   : (t == py::str("WORD") || t == py::str("BYTE") || t == py::str("INTEGER") ? py::str("0") : py::str("NIL"))) +
               NL;
    }
    return head.replace(py::str("{TODO}"), py::str("")) + NL + py::str("BEGIN") + NL + body + py::str("END;") + NL;
}

py::list<py::list<py::str>> Compiland::parts() const {
    py::list<py::str> ps = pieces();
    std::vector<int64_t> size;
    int64_t tot = 0;
    for (const py::str& f : ps) {
        size.push_back(py::len(piece(f)));
        tot += size.back();
    }
    int64_t n = u.parts;
    py::list<py::list<py::str>> out;
    for (int64_t i = 0; i < n; ++i) out.append(py::list<py::str>());
    int64_t acc = 0;
    for (int64_t k = 0; k < py::len(ps); ++k) {
        out[std::min(n - 1, py::floordiv(acc * n, std::max<int64_t>(tot, 1)))].append(ps[k]);
        acc += size[static_cast<size_t>(k)];
    }
    py::list<py::list<py::str>> kept;
    for (const py::list<py::str>& x : out) {
        if (!x.empty()) kept.append(x);
    }
    out = kept;
    if (path::exists(where(py::str("MAIN.pas"))) || u.kind == py::str("MODULE")) {
        if (out.empty()) out.append(py::list<py::str>());
        out[-1].append(py::str("MAIN.pas"));
    }
    return out;
}

py::str Compiland::source(const py::list<py::str>& part) const {
    py::str G = rd(py::str("GLOBALS.pas")), X = rd(py::str("EXTERNS.pas")), F = rd(py::str("FORWARDS.pas"));
    py::dict<py::str, py::str> rt = result_types();
    py::dict<py::str, py::str> body;
    for (const py::str& f : part) {
        if (f == py::str("MAIN.pas")) {
            py::str m = rd(f);
            body.set(f, m.empty() ? empty : m);
        } else {
            body.set(f, piece(f));
        }
    }
    py::str joined;
    for (const py::str& v : body.values()) joined = joined + v;
    py::set<py::str> used;
    for (const py::str& w : re_ident().findall(joined)) used.add(w);
    // head, sep, var = G.partition("VAR" + NL)
    py::str marker = py::str("VAR") + NL;
    int64_t at = G.find(marker);
    py::str head = at >= 0 ? G.slice(0, at) : G;
    bool sep = at >= 0;
    py::str var = at >= 0 ? G.slice_from(at + py::len(marker)) : py::str("");

    auto keep = [&](const py::str& text, const py::re::Pattern& pat) -> py::str {
        py::str out("");
        for (const py::str& l : py::split(text, NL)) {
            if (l.empty()) continue;
            auto m = pat.match(l);
            if (!m || used.contains(m->group(1))) out = out + l + NL;
        }
        return out;
    };
    static const py::re::Pattern var_pat(py::str("  (G_[0-9A-F]{4}):"), 0);
    static const py::re::Pattern proc_pat(py::str("(?:PROCEDURE|FUNCTION) +([A-Z_0-9]+)"), 0);
    py::str kept = sep ? keep(var, var_pat) : py::str("");
    py::str t = headline + NL + head + (!kept.strip().empty() ? py::str("VAR") + NL + kept : py::str(""));
    t = t + keep(X, proc_pat) + keep(F, proc_pat);
    for (const py::str& f : pieces()) {
        if (body.contains(f)) {
            t = t + body.at(f);
        } else if (used.contains(py::str("P") + f.slice(1, 6).upper())) {
            t = t + stub(f, rt);
        }
    }
    return t + (body.contains(py::str("MAIN.pas")) ? body.at(py::str("MAIN.pas")) : empty);
}

py::list<py::str> Compiland::names_of(const py::list<py::str>& part) const {
    py::set<py::str> out;
    for (const py::str& f : part) {
        if (f == py::str("MAIN.pas")) {
            out.add(py::str("MAIN"));
            continue;
        }
        for (const py::str& line : py::split(piece(f), NL)) {
            auto m = re_proc_line().match(line);
            if (m && !line.contains(py::str("{STUB}")) && !line.contains(py::str("{TODO}")) && !line.contains(py::str("FORWARD"))) {
                out.add(m->group(1));
            }
        }
    }
    return py::sorted(out);
}

py::dict<int64_t, std::tuple<std::optional<int64_t>, std::optional<int64_t>>> _procedures(const Project& prj) {
    if (!prj.cfg.procedures) missing_key();
    py::str p = path::join(prj.dir, prj.cfg.root, *prj.cfg.procedures);
    py::dict<int64_t, std::tuple<std::optional<int64_t>, std::optional<int64_t>>> out;
    py::V all = load_json(p);
    for (const auto& kv : all.items()) {
        std::optional<int64_t> size, unit;
        const py::V& s = kv.second.get(py::V("size"));
        const py::V& un = kv.second.get(py::V("unit"));
        if (s.is_intlike()) size = s.as_int();
        if (un.is_intlike()) unit = un.as_int();
        out.set(py::int_parse(py::str(kv.first.str()), 16), std::make_tuple(size, unit));
    }
    return out;
}

namespace {

// the OMF object as compare.function reads it: its segments, fix-ups and publics
compare::Obj compare_obj(const omf::Obj& o) {
    compare::Obj c;
    c.seg = o.seg;
    for (const omf::Fixup& f : o.fix) {
        const auto& tgt = std::get<3>(f);
        const auto& v = std::get<1>(tgt);
        py::str second = std::holds_alternative<py::str>(v) ? std::get<py::str>(v) : py::str("");
        c.fix.append(compare::Fixup(std::get<0>(f), std::get<1>(f), std::get<2>(f), compare::Target(std::get<0>(tgt), second), std::get<4>(f)));
    }
    for (const omf::Public& p : o.pubs) {
        c.pubs.append(compare::Public(std::get<0>(p), std::get<1>(p) ? *std::get<1>(p) : py::str(""), std::get<2>(p)));
    }
    return c;
}

// importlib.import_module(mod) then getattr(module, fn), as far as the ported comparers go
Equivalent resolve_equivalent(const Project& prj, const py::str& mod, const py::str& fn) {
    py::str full = mod + py::str(":") + fn;
    if (prj.equivalents.contains(full)) return prj.equivalents.at(full);
    if (mod.empty()) throw py::ValueError("Empty module name");
    if (mod == py::str("codebreaker.exact.habits.newcomp")) {
        if (fn == py::str("equivalent")) return newcomp::equivalent;
        if (fn == py::str("equivalent3")) return newcomp::equivalent3;
    } else if (mod == py::str("codebreaker.exact.habits.oldcomp")) {
        if (fn == py::str("equivalent")) return oldcomp::equivalent;
    } else {
        int64_t dot = mod.find(py::str("."));
        throw py::ModuleNotFoundError(py::percent("No module named '%s'", mod.startswith(py::str("codebreaker.")) ? mod : (dot >= 0 ? mod.slice(0, dot) : mod)));
    }
    throw py::AttributeError(py::percent("module '%s' has no attribute '%s'", mod, fn));
}

}  // namespace

py::list<Row> run(const Project& prj, bool verbose) {
    Toolchain tc = prj.toolchain();
    auto procs = _procedures(prj);
    const py::dict<py::str, int64_t>& units = prj.cfg.units;
    const py::bytes& img = prj.image;
    py::list<Row> rows;
    for (const auto& [name, u] : prj.cfg.compilands.items()) {
        Compiland c(prj, name, u);
        std::optional<int64_t> unit = units.contains(name) ? std::optional<int64_t>(units.at(name)) : u.unit;
        std::vector<int64_t> mine;
        for (const auto& [a, v] : procs.items()) {
            if (std::get<1>(v) == unit) mine.push_back(a);
        }
        std::sort(mine.begin(), mine.end());
        std::optional<int64_t> helper_at;   // (the entry helper: after them all)
        for (int64_t a : mine) {
            const auto& sz = std::get<0>(procs.at(a));
            if (!sz) missing_key();
            int64_t e = a + *sz;
            if (!helper_at || e > *helper_at) helper_at = e;
        }
        for (const py::list<py::str>& part : c.parts()) {
            py::str text = c.source(part);
            auto [raw, con] = tc.compile(text.encode("latin1"), py::str("T.PAS"));
            py::str label = py::percent("%s %s", name, part[0]);
            if (!raw) {
                py::list<py::str> last = py::splitlines(con.strip());
                rows.append(Row(label, std::nullopt, std::nullopt, 0, py::str("does not compile"), last.slice_from(-1)));
                continue;
            }
            omf::Obj o = omf::read(*raw);
            compare::Obj co = compare_obj(o);
            std::optional<py::str> seg_found;
            for (const omf::SegDef& s : o.segs) {
                const py::str& sn = std::get<0>(s);
                if (!(sn == py::str("HEAP") || sn == py::str("STACK") || sn == py::str("DATA") || sn == py::str("CONST") || sn == py::str("MEMORY")) &&
                    o.filled.get(sn, 0)) {
                    seg_found = sn;
                    break;
                }
            }
            if (!seg_found) throw py::IndexError("list index out of range");
            py::str seg = *seg_found;
            py::list<std::tuple<py::str, int64_t, int64_t>> fs = omf::functions(o, seg);
            py::dict<int64_t, int64_t> at_of;   // (two names at one offset - a main program's block and
            for (const auto& [n, of, ln] : fs) {   // ENTGQQ, the entry: each has the code that starts there)
                (void)n;
                at_of.set(of, std::max(at_of.get(of, 0), ln));
            }
            py::dict<py::str, std::tuple<int64_t, int64_t>> off_of;
            for (const auto& [n, of, ln] : fs) {
                (void)ln;
                off_of.set(n.upper(), std::make_tuple(of, at_of.at(of)));
            }
            // where each compiled procedure stands in the program: P<ADDR> at ADDR, the block at main, the helper after
            std::vector<std::tuple<int64_t, int64_t, std::optional<int64_t>>> places;
            int64_t main_at = std::holds_alternative<py::str>(u.main) ? py::int_parse(std::get<py::str>(u.main), 0) : std::get<int64_t>(u.main);
            for (const auto& [n, of, ln] : fs) {
                (void)ln;
                auto m = re_proc_name().fullmatch(n.upper());
                std::optional<int64_t> a;
                if (m) {
                    if (c.map.empty() || c.map.contains(m->group(1))) a = c.addr_of(m->group(1));
                } else if (n.upper() == name.upper()) {
                    a = main_at;
                } else if (n == py::str("(helper)")) {
                    a = helper_at;
                }
                places.emplace_back(of, at_of.at(of), a);
            }
            compare::Where where = [places](int64_t x) -> int64_t {
                for (const auto& [of, ln, a] : places) {
                    if (a && of <= x && x < of + std::max<int64_t>(ln, 1)) {
                        return *a + x - of;
                    }
                }
                return -1;
            };
            for (const py::str& h : c.names_of(part)) {
                py::str pn = h == py::str("MAIN") ? name.upper() : py::str("P") + h;
                if (h != py::str("MAIN") && !c.map.empty() && !c.map.contains(h)) {
                    rows.append(Row(label, pn, std::nullopt, 0, py::str("no place"), py::list<py::str>{py::str("not in the program (no place in the map)")}));
                    continue;
                }
                int64_t a = h == py::str("MAIN") ? main_at : c.addr_of(h);
                if (!off_of.contains(pn)) {
                    rows.append(Row(label, pn, a, 0, py::str("no place"), py::list<py::str>{py::str("not in the compiled code")}));
                    continue;
                }
                auto [of, ln] = off_of.at(pn);
                std::optional<int64_t> size;
                if (procs.contains(a)) size = std::get<0>(procs.at(a));
                compare::Symbols syms = prj.symbols().copy();
                compare::Symbols learn;
                auto [bad, notes] = compare::function(co, img, pn, of, ln, a, prj.compare, syms, learn, prj.decode, seg, where);
                if (size && *size != ln) {
                    notes.append(py::percent("%d bytes, the program's %d", ln, *size));
                }
                py::str verdict = compare::identical(bad, notes) ? py::str("identical") : py::str("differs");
                std::optional<py::str> eq = u.equivalent ? u.equivalent : prj.cfg.equivalent;
                if (verdict == py::str("differs") && eq && !eq->empty() && size && *size) {   // (the same code but for another compiler state's habits?)
                    int64_t colon = eq->find(py::str(":"));
                    py::str mod = colon >= 0 ? eq->slice(0, colon) : *eq;
                    py::str fn = colon >= 0 ? eq->slice_from(colon + 1) : py::str("");
                    py::set<int64_t> hole = omf::holes(o, seg);
                    py::bytes got = o.seg.at(seg).slice(of, of + ln);
                    py::bytes want = img.slice(a, a + *size);
                    cmpp::HDRLEN = tc.helper_data;   // (the bytes after the entry helper's call)
                    bool same;
                    py::list<py::str> why;
                    try {
                        py::set<int64_t> fx;
                        for (int64_t k : hole) {
                            if (of <= k && k < of + ln) fx.add(k - of);
                        }
                        Equivalent f = resolve_equivalent(prj, mod, fn);
                        std::tie(same, why) = f(want, got, fx);
                    } catch (const py::Exception& e) {
                        same = false;
                        why = py::list<py::str>{py::str(std::string(e.what()))};
                    }
                    if (same) {
                        verdict = py::str("equivalent");
                        notes = py::list<py::str>{py::str("the same code but for the habits of another compiler state: ") +
                                                  py::join(py::str("; "), why).slice(0, 200)};
                    }
                }
                rows.append(Row(label, pn, a, ln, verdict, notes));
                if (verbose) {
                    py::print(py::percent("%-22s %-10s %05x %s %s", label, pn, a, verdict, py::join(py::str("; "), notes.slice(0, 2))));
                }
            }
        }
    }
    return rows;
}

}  // namespace codebreaker::exact::pascal_join
