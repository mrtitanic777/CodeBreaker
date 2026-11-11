// codebreaker/exact/repair.cpp -- typed C++ port of codebreaker.exact.repair (see repair.hpp).

#include "codebreaker/exact/repair.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "codebreaker/exact/omf.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::repair {

namespace re = py::re;

namespace {

// "%+d" % n
py::str signed_(int64_t n) { return (n >= 0 ? py::str("+") : py::str("")) + py::to_str(n); }

// the first word of a text (text.split()[0])
py::str first_word(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    auto space = [](char32_t c) { return c == U' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F) || c == 0x85 || c == 0xA0; };
    while (i < u.size() && space(u[i])) i++;
    if (i == u.size()) throw py::IndexError("list index out of range");
    size_t j = i;
    while (j < u.size() && !space(u[j])) j++;
    return py::str(u.substr(i, j - i));
}

py::str read_latin1(const py::str& path) {
    py::file f = py::file::open(path, py::str("r"), py::str("latin1"), std::nullopt, py::str(""));
    const py::str t = f.read_text();
    f.close();
    return t;
}

void write_latin1(const py::str& path, const py::str& text) {
    py::file f = py::file::open(path, py::str("w"), py::str("latin1"), std::nullopt, py::str(""));
    f.write(text);
    f.close();
}

// {r[1]: r[4] for r in rows}: the verdict by function name (a later row of the same name wins)
struct Verdicts {
    std::map<std::pair<bool, std::u32string>, py::str> v;
    static std::pair<bool, std::u32string> key(const std::optional<py::str>& name) {
        return name ? std::make_pair(true, name->u32()) : std::make_pair(false, std::u32string());
    }
    explicit Verdicts(const py::list<Row>& rows) {
        for (const Row& r : rows) v[key(std::get<1>(r))] = std::get<4>(r);
    }
    std::optional<py::str> get(const std::optional<py::str>& name) const {
        const auto it = v.find(key(name));
        if (it == v.end()) return std::nullopt;
        return it->second;
    }
};

}  // namespace

std::optional<std::tuple<int64_t, py::list<int64_t>>> _shifts(const py::list<py::str>& notes) {
    static const re::Pattern NOTE = re::compile(py::str(R"(\? (\d+) values? (?:each )?([+-]\d+) in the program \(([^)]*)\))"));
    for (const py::str& n : notes) {
        const std::optional<re::Match> m = NOTE.search(n);
        if (m) {
            py::list<int64_t> offs;
            for (const py::str& x : py::split(m->group(3), py::str(","))) {
                const py::str s = x.strip();
                if (s.empty()) continue;
                py::str digits = s;
                while (!digits.empty() && digits.u32()[0] == U'+') digits = digits.slice(1, py::len(digits));
                offs.append(py::int_parse(digits, 16));
            }
            return std::make_tuple(py::int_parse(m->group(2), 10), offs);
        }
    }
    return std::nullopt;
}

py::list<std::tuple<py::str, py::str>> candidates(const py::str& text, const py::list<int64_t>& mine, int64_t d) {
    py::list<std::tuple<py::str, py::str>> out;
    std::set<int64_t> want;
    for (int64_t v : mine) want.insert(v & 0xFF);
    const re::Pattern member = re::compile(py::str(R"(\b([A-Z][A-Z_]*[A-Z_])([1-9]\d{0,2}|0)\b)"));   // (a family named by ordinals: DK0, DK46, TK12)
    std::map<std::u32string, std::set<std::u32string>> named;
    for (const re::Match& m : member.finditer(text)) {
        if (want.count(py::int_parse(m.group(2), 10) & 0xFF)) named[m.group(1).u32()].insert(m.group(0).u32());
    }
    for (const auto& fam_these : named) {
        const py::str fam(fam_these.first);
        const std::set<std::u32string>& these = fam_these.second;
        // re.escape(fam): the family's letters and underscores are not escaped
        const re::Pattern rx = re::compile(py::str("\\b(") + fam + py::str(")([1-9]\\d{0,2}|0)\\b"));
        const py::str t = rx.sub(
            [&](const re::Match& m) -> py::str {
                if (these.count(m.group(0).u32())) return m.group(1) + py::to_str(py::int_parse(m.group(2), 10) + d);
                return m.group(0);
            },
            text);
        std::vector<py::str> sorted_these;
        for (const std::u32string& s : these) sorted_these.push_back(py::str(s));
        py::list<py::str> shown;
        for (const py::str& s : sorted_these) shown.append(s);
        if (t != text) out.append({py::join(py::str(", "), shown) + py::str(" moved by ") + signed_(d), t});
        const py::str t2 = rx.sub(
            [&](const re::Match& m) -> py::str { return m.group(1) + py::to_str(py::int_parse(m.group(2), 10) + d); }, text);
        if (t2 != text && t2 != t) out.append({py::str("every ") + fam + py::str("<n> moved by ") + signed_(d), t2});
    }
    for (int64_t v : want) {
        const re::Pattern lit = re::compile(py::str("(?<![\\w.])") + py::to_str(v) + py::str("(?![\\w.])"));
        const py::list<re::Match> hits = lit.finditer(text);
        for (const re::Match& m : hits) {
            const int64_t line = text.slice(0, m.start()).count(py::str("\n")) + 1;
            out.append({py::percent(py::str("the literal %d made %d (line %d)"), v, v + d, line),
                        text.slice(0, m.start()) + py::to_str(v + d) + text.slice(m.end(), py::len(text))});
        }
        if (hits.size() > 1) {
            out.append({py::percent(py::str("every literal %d made %d"), v, v + d), lit.sub(py::to_str(v + d), text)});
        }
    }
    return out;
}

pascal_join::Project _only(const pascal_join::Project& prj, const py::str& name) {
    pascal_join::Project p = prj;
    py::dict<py::str, pascal_join::CompilandCfg> one;
    one.set(name, prj.cfg.compilands.at(name));
    p.cfg.compilands = one;
    return p;
}

std::optional<py::str> _piece_of(const pascal_join::Compiland& c, const py::str& h) {
    const re::Pattern head(py::str("\\s*(PROCEDURE|FUNCTION)\\s+P") + h + py::str("\\b"), re::I);
    for (const py::str& d : c.dirs) {
        for (const auto& step : py::os::walk(d)) {
            const py::str& root = std::get<0>(step);
            const py::list<py::str>& files = std::get<2>(step);
            if (py::os::path::relpath(root, d).count(py::str("\\")) > 3) continue;
            for (const py::str& f : files) {
                if (f.lower() == py::str("p") + h.lower() + py::str(".pas")) {
                    const py::str p = py::os::path::join(root, f);
                    py::file fh = py::file::open(p, py::str("r"), py::str("latin1"));
                    const py::str first = fh.readline_text();
                    fh.close();
                    if (head.match(first)) return p;
                }
            }
        }
    }
    return std::nullopt;
}

py::list<Job> _pascal(const ledger::Project& prj, bool verbose) {
    (void)verbose;
    const pascal_join::Project pp = prj.pascal_project();
    const py::list<pascal_join::Row> rows = pascal_join::run(pp, false);
    py::list<Job> out;
    for (const pascal_join::Row& r : rows) {
        const py::str& label = std::get<0>(r);
        const std::optional<py::str>& pn = std::get<1>(r);
        const std::optional<int64_t>& a = std::get<2>(r);
        const py::str& verdict = std::get<4>(r);
        const py::list<py::str>& notes = std::get<5>(r);
        std::optional<std::tuple<int64_t, py::list<int64_t>>> s;
        if (verdict == py::str("differs")) s = _shifts(notes);
        if (!s) continue;
        if (!pn) throw py::AttributeError("'NoneType' object has no attribute 'startswith'");
        if (!pn->startswith(py::str("P"))) continue;
        const int64_t d = std::get<0>(*s);
        const py::list<int64_t>& offs = std::get<1>(*s);
        const py::str name = first_word(label);
        const pascal_join::CompilandCfg& u = pp.cfg.compilands.at(name);
        const pascal_join::Compiland c(pp, name, u);
        const py::str h = pn->slice(1, py::len(*pn));
        const py::list<py::list<py::str>> parts = c.parts();
        const py::list<py::str>* part = nullptr;
        for (const py::list<py::str>& p : parts) {
            if (c.names_of(p).contains(h)) {
                part = &p;
                break;
            }
        }
        if (!part) throw py::StopIteration();
        auto [raw, con] = pp.toolchain().compile(c.source(*part).encode("latin1"), py::str("T.PAS"));
        (void)con;
        if (!raw) throw py::TypeError("a bytes-like object is required, not 'NoneType'");
        const omf::Obj o = omf::read(*raw);
        std::optional<py::str> seg;
        for (const omf::SegDef& sd : o.segs) {
            const py::str& x = std::get<0>(sd);
            if (x == py::str("HEAP") || x == py::str("STACK") || x == py::str("DATA") || x == py::str("CONST") || x == py::str("MEMORY")) continue;
            if (o.filled.contains(x) && o.filled.at(x)) {
                seg = x;
                break;
            }
        }
        if (!seg) throw py::IndexError("list index out of range");
        py::dict<py::str, int64_t> of;
        for (const auto& f : omf::functions(o, *seg)) of.set(std::get<0>(f).upper(), std::get<1>(f));
        const int64_t at_of = of.at(py::str("P") + h);
        py::list<int64_t> mine;
        for (int64_t k : offs) mine.append(static_cast<int64_t>(static_cast<unsigned char>(o.seg.at(*seg)[at_of + k])));
        const std::optional<py::str> path = _piece_of(c, h);
        if (!path) continue;
        const py::str* base = nullptr;
        for (const py::str& dd : c.dirs) {
            if (path->startswith(dd)) {
                base = &dd;
                break;
            }
        }
        if (!base) throw py::StopIteration();
        const py::str rel = py::os::path::relpath(*path, *base);
        Job j;
        j.fn = pn;
        j.at = a;
        j.path = *path;
        j.dest = py::os::path::join(c.dirs[0], rel);
        j.mine = mine;
        j.d = d;
        j.unit = name;
        const pascal_join::Project captured = pp;
        j.check = [captured, name]() { return pascal_join::run(_only(captured, name), false); };
        out.append(j);
    }
    return out;
}

py::list<Job> _sourced(const ledger::Project& prj, bool verbose) {
    (void)verbose;
    py::list<Job> out;
    for (const std::shared_ptr<const ledger::Project>& part_p : prj.sets()) {
        const ledger::Project& part = *part_p;
        const auto tcs = part.toolchains();
        const compare::Symbols syms = part.symbols();
        for (const auto& source : part.sources()) {
            const std::optional<int64_t> at = std::get<0>(source);
            const py::str path = std::get<1>(source);
            auto per_of = [part_p, tcs, syms, at, path]() {
                const ledger::Project& part = *part_p;   // (the set is kept alive by the job: a set of "also" is owned by this pointer)
                py::list<std::tuple<py::str, py::list<Row>>> per;
                for (const auto& lt : tcs) {
                    compare::Symbols learnt;
                    per.append({std::get<0>(lt), ledger::check_file(part, *std::get<1>(lt), path, at, syms, learnt)});
                }
                return per;
            };
            const py::list<ledger::Row7> rows = ledger::_merge(per_of());
            for (const ledger::Row7& r : rows) {
                const std::optional<py::str>& name = std::get<1>(r);
                const std::optional<int64_t>& a = std::get<2>(r);
                const py::str& verdict = std::get<4>(r);
                const py::list<py::str>& notes = std::get<5>(r);
                std::optional<std::tuple<int64_t, py::list<int64_t>>> s;
                if (verdict == py::str("differs")) s = _shifts(notes);
                if (!s) continue;
                const int64_t d = std::get<0>(*s);
                const py::list<int64_t>& offs = std::get<1>(*s);
                const auto& tc = std::get<1>(tcs[0]);
                py::bytes src;
                {
                    py::file f = py::open_rb(path);
                    src = f.read();
                    f.close();
                }
                auto [raw, con] = tc->compile(src, part.headers(), part.include(), part.switches());
                (void)con;
                if (!raw) throw py::TypeError("a bytes-like object is required, not 'NoneType'");
                const omf::Obj o = omf::read(*raw);
                std::optional<std::tuple<py::str, int64_t>> hit;
                for (const py::str& seg : omf::code_segments(o)) {
                    for (const auto& f : omf::functions(o, seg)) {
                        if (name && std::get<0>(f) == *name) {
                            hit = std::make_tuple(seg, std::get<1>(f));
                            break;
                        }
                    }
                    if (hit) break;
                }
                if (!hit) continue;
                py::list<int64_t> mine;
                for (int64_t k : offs) mine.append(static_cast<int64_t>(static_cast<unsigned char>(o.seg.at(std::get<0>(*hit))[std::get<1>(*hit) + k])));
                Job j;
                j.fn = name;
                j.at = a;
                j.path = path;
                j.dest = path;
                j.mine = mine;
                j.d = d;
                j.check = [per_of]() {
                    py::list<Row> rows6;
                    for (const ledger::Row7& r7 : ledger::_merge(per_of())) {
                        rows6.append(Row(std::get<0>(r7), std::get<1>(r7), std::get<2>(r7), std::get<3>(r7), std::get<4>(r7), std::get<5>(r7)));
                    }
                    return rows6;
                };
                out.append(j);
            }
        }
    }
    return out;
}

int64_t main(const ledger::Project& prj, const py::list<py::str>& argv, std::ostream& out) {
    bool write = false, verbose = false;
    for (const py::str& a : argv) {
        if (a == py::str("--write")) write = true;
        if (a == py::str("-v")) verbose = true;
    }
    const py::list<Job> todo = prj.has_compilands() ? _pascal(prj, verbose) : _sourced(prj, verbose);
    int64_t fixed = 0;
    for (const Job& j : todo) {
        const Verdicts before(j.check());
        const py::str text = read_latin1(j.path);
        std::optional<py::str> kept;
        for (const auto& cand : candidates(text, j.mine, j.d)) {
            const py::str& what = std::get<0>(cand);
            const py::str& t = std::get<1>(cand);
            py::os::makedirs(py::os::path::dirname(j.dest), 0777, true);
            std::optional<py::str> had;
            if (py::os::path::exists(j.dest)) had = read_latin1(j.dest);
            write_latin1(j.dest, t);
            const Verdicts after(j.check());
            bool good = after.get(j.fn) == std::optional<py::str>(py::str("identical"));
            if (good) {
                for (const auto& kv : before.v) {
                    if (kv.second == py::str("identical")) {
                        const auto it = after.v.find(kv.first);
                        if (it == after.v.end() || !(it->second == py::str("identical"))) {
                            good = false;
                            break;
                        }
                    }
                }
            }
            if (good && write) {
                kept = what;
                break;
            }
            if (!had) py::os::remove(j.dest);   // (not kept, or only asked: as it was)
            else write_latin1(j.dest, *had);
            if (good) {
                kept = what + py::str(" (not written: --write)");
                break;
            }
        }
        if (!j.at) throw py::TypeError("%x format: an integer is required, not NoneType");
        out << py::percent(py::str("%s %05x: %s"), j.fn ? *j.fn : py::str("None"), *j.at,
                           kept ? *kept : py::str("no change of those tried makes it the program's")).utf8() << "\n";
        if (kept) fixed += 1;
    }
    out << py::percent(py::str("%d of %d mended"), fixed, static_cast<int64_t>(todo.size())).utf8() << "\n";
    return 0;
}

}  // namespace codebreaker::exact::repair
