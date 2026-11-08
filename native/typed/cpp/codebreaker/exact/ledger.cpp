// codebreaker/exact/ledger.cpp -- typed C++ port of codebreaker.exact.ledger.

#include "codebreaker/exact/ledger.hpp"

#include <algorithm>
#include <cstdio>

#include "codebreaker/exact/omf.hpp"

namespace codebreaker::exact::ledger {

namespace {

// the object as compare.function reads it (see modules.cpp: a group's index, or nothing, becomes text)
compare::Obj to_compare(const formats::omf::Obj& o) {
    compare::Obj c;
    c.seg = o.seg;
    for (const auto& f : o.fix) {
        const auto& tg = std::get<3>(f);
        py::str second;
        if (std::holds_alternative<py::str>(std::get<1>(tg))) {
            second = std::get<py::str>(std::get<1>(tg));
        } else if (std::holds_alternative<int64_t>(std::get<1>(tg))) {
            second = py::str(std::to_string(std::get<int64_t>(std::get<1>(tg))));
        }
        c.fix.append(std::make_tuple(std::get<0>(f), std::get<1>(f), std::get<2>(f), compare::Target(std::get<0>(tg), second), std::get<4>(f)));
    }
    for (const auto& p : o.pubs) c.pubs.append(std::make_tuple(std::get<0>(p), std::get<1>(p).value_or(py::str("")), std::get<2>(p)));
    return c;
}

compare::Project to_compare(const Project& prj) {
    compare::Project c;
    c.dgroup = prj.dgroup();
    c.loadseg = prj.loadseg();
    c.code_base = [&prj](int64_t a) { return prj.code_base(a); };
    c.relocs = [&prj]() -> const py::set<int64_t>& { return prj.relocs(); };
    return c;
}

bool is_code_sym(const Symbols& syms, const py::str& nm) {
    if (!syms.contains(nm)) return false;
    const py::str& kind = std::get<0>(syms.at(nm));
    return kind == py::str("C") || kind == py::str("?");
}

py::str by_join(const By& by) {   // ",".join(by): TypeError for a None
    py::list<py::str> parts;
    for (const auto& b : by) {
        if (!b) throw py::TypeError("sequence item 0: expected str instance, NoneType found");
        parts.append(*b);
    }
    return py::join(py::str(","), parts);
}

}  // namespace

py::set<py::str> _stubs(const py::bytes& src) {
    const py::str s = src.decode("latin1");
    py::set<py::str> out;
    const int64_t m = s.find(py::str("/* SAMEFILE */"));
    if (m < 0) return out;
    static const py::re::Pattern stub = py::re::compile(py::str("(\\w+)\\s*\\([^)]*\\)[^{;]*\\{\\s*\\}"));
    for (const py::str& x : stub.findall(s.slice(m, s.size()))) {
        out.add(x.upper().lstrip(py::str("_")));
    }
    return out;
}

bool _reaches(const Project& prj, const py::list<int64_t>& starts, int64_t a, int64_t ln) {
    // bisect.bisect_right(starts, a)
    int64_t i = 0;
    while (i < starts.size() && starts[i] <= a) ++i;
    std::vector<int64_t> ends;
    if (i < starts.size()) ends.push_back(starts[i]);
    if (prj.code_end()) ends.push_back(prj.code_end());
    if (ends.empty()) ends.push_back(a + ln);
    const int64_t end = *std::min_element(ends.begin(), ends.end());
    const int64_t e = a + ln;
    const py::bytes tail = prj.image().slice(e, end);
    return e + (e & 1) == end + (end & 1) || (0 < tail.size() && tail.size() <= 16 && tail.strip(py::bytes(std::string(1, '\0'))).empty());
}

py::list<Row> check_file(const Project& prj, const Toolchain& tc, const py::str& path, std::optional<int64_t> at, const Symbols& syms,
                         Symbols& learnt) {
    const py::str base = py::os::path::basename(path);
    const py::bytes src = py::file::open(path, py::str("rb")).read();
    auto [raw, con] = tc.compile(src, prj.headers(), prj.include(), prj.switches());
    if (!raw) {
        const py::list<py::str> last = py::splitlines(con.strip());
        py::list<py::str> tail;
        if (!last.empty()) tail.append(last[-1]);                     // last[-1:]
        py::list<Row> one;
        one.append(Row(base, std::nullopt, at, 0, py::str("does not compile"), tail));
        return one;
    }
    const formats::omf::Obj o = omf::read(*raw);
    const compare::Obj co = to_compare(o);
    const compare::Project cp = to_compare(prj);
    const compare::Decoder decode = prj.decoder();
    const py::set<py::str> stubs = _stubs(src);
    const py::list<int64_t> starts = prj.starts();
    const py::bytes image = prj.image();
    py::list<Row> out;
    Symbols got;
    int64_t ti = -1;
    for (const py::str& text : omf::code_segments(o)) {            // (#pragma alloc_text: more code segments than one)
        ++ti;
        const auto fs = omf::functions(o, text);
        std::optional<int64_t> first;
        if (ti == 0 && at && *at >= 0) first = at;
        if (!first) {                                              // (an assembly module placed by its first name SYMBOLS knows)
            std::optional<std::tuple<py::str, int64_t>> named;
            for (const auto& f : fs) {
                if (is_code_sym(syms, std::get<0>(f))) {
                    named = std::make_tuple(std::get<0>(f), std::get<1>(f));
                    break;
                }
            }
            if (named) first = std::get<1>(syms.at(std::get<0>(*named))) - std::get<1>(*named);
            else if (ti == 0 && at && *at < 0) continue;           // (nothing to place it by)
        }
        const bool together = prj.whole(path) && first.has_value();   // (a whole source file: its text lies in one piece)
        if (together && ti == 0 && prj.cfg_str(py::str("named_by")).value_or(py::str("")) == py::str("first procedure")) {
            // (the file is named after a procedure of it - its first frame, as a rule; the main module has code
            // before it): of the module's start and its frames, the one its bytes agree with the program at
            const py::bytes code = o.seg.at(text).slice(0, o.filled.get(text, 0));
            const py::set<int64_t> hole = omf::holes(o, text);
            std::vector<int64_t> cands{0};
            const py::bytes pat(std::string("\x55\x8b\xec", 3));
            for (int64_t p = code.find(pat); p >= 0; p = code.find(pat, p + 3)) cands.push_back(p);
            if (!at) throw py::TypeError("unsupported operand type(s) for -: 'NoneType' and 'int'");
            auto agree = [&](int64_t k0) -> int64_t {
                const int64_t b = *at - k0;
                if (b < 0) return -1;
                int64_t n = 0;
                for (int64_t x = 0; x < std::min<int64_t>(code.size(), 256); ++x) {
                    if (!hole.contains(x) && b + x < image.size() && image[b + x] == code[x]) ++n;
                }
                return n;
            };
            int64_t k0 = cands[0];
            int64_t best_agree = agree(k0);
            for (size_t i = 1; i < cands.size(); ++i) {              // max(cands, key=(agree(k), -k)): the first of the best
                const int64_t a = agree(cands[i]);
                if (a > best_agree || (a == best_agree && -cands[i] > -k0)) {
                    best_agree = a;
                    k0 = cands[i];
                }
            }
            first = *at - k0;
        }
        struct Place {
            int64_t of, ln;
            std::optional<int64_t> a;
        };
        std::vector<Place> places;
        for (size_t k = 0; k < fs.size(); ++k) {
            const auto& [nm, of, ln] = fs[static_cast<int64_t>(k)];
            std::optional<int64_t> a;
            if (k == 0 && first) a = first;
            else if (together) a = *first - std::get<1>(fs[0]) + of;
            else if (is_code_sym(syms, nm)) a = std::get<1>(syms.at(nm));
            places.push_back({of, ln, a});
        }
        const int64_t first_off = fs.empty() ? 0 : std::get<1>(fs[0]);
        const compare::Where where = [places, first, first_off](int64_t x) -> int64_t {
            for (const Place& p : places) {
                if (p.a && p.of <= x && x < p.of + p.ln) return *p.a + x - p.of;
            }
            return first ? *first - first_off + x : -1;
        };
        for (size_t k = 0; k < fs.size(); ++k) {
            const auto& [name, off, ln] = fs[static_cast<int64_t>(k)];
            if (stubs.contains(name.upper().lstrip(py::str("_")))) continue;
            const std::optional<int64_t> a = places[k].a;
            if (!a) {
                out.append(Row(base, name, std::nullopt, ln, py::str("no place"), py::list<py::str>()));
                continue;
            }
            auto [bad, notes] = compare::function(co, image, name, off, ln, *a, cp, syms, got, decode, text, where);
            const py::bytes code = o.seg.at(text);
            if (bad.size() == 1 && bad[0] == ln - 1 && k == fs.size() - 1 && off + ln == code.size() && code.size() % 2 == 0 &&
                code[-1] == 0 && ln >= 4 && code[-4] == 0xE9) {
                bad = py::list<int64_t>();                         // (the assembler's pad to an even length: the next
                notes.append(py::str("? the last byte is the pad to an even length; the next module begins there"));   // module's first byte)
            }
            if (starts.size() > 0 && !_reaches(prj, starts, *a, ln)) {
                notes.append(py::percent(py::str("ends at %05x, before the next function's start"), *a + ln));
            }
            const bool ok = compare::identical(bad, notes);
            out.append(Row(base, name, *a, ln, py::str(ok ? "identical" : "differs"), notes));
            if (ok) got.set(name, std::make_tuple(py::str("C"), *a));
        }
    }
    bool all_identical = true;
    for (const Row& r : out) {
        if (std::get<2>(r) && !(std::get<4>(r) == py::str("identical"))) all_identical = false;
    }
    if (all_identical) {
        for (const auto& kv : got.items()) {
            if (!std::get<0>(kv).contains(py::str(":"))) learnt.set(std::get<0>(kv), std::get<1>(kv));
        }
    }
    return out;
}

py::list<Row7> _merge(const py::list<std::tuple<py::str, py::list<Row>>>& per) {
    if (per.empty()) throw py::ValueError("max() iterable argument is empty");
    py::list<Row> base = std::get<1>(per[0]);                      // max(..., key=len): the first of the longest
    for (const auto& lr : per) {
        if (std::get<1>(lr).size() > base.size()) base = std::get<1>(lr);
    }
    py::list<Row7> rows;
    for (int64_t k = 0; k < base.size(); ++k) {
        By by;
        std::optional<Row> best;
        for (const auto& lr : per) {
            const auto& rs = std::get<1>(lr);
            if (k < rs.size() && std::get<4>(rs[k]) == py::str("identical")) {
                by.append(std::get<0>(lr));
                if (!best) best = rs[k];
            }
        }
        if (!best) {                                               // the first that compiled says what is wrong
            for (const auto& lr : per) {
                const auto& rs = std::get<1>(lr);
                if (k < rs.size() && !(std::get<4>(rs[k]) == py::str("does not compile"))) {
                    best = rs[k];
                    break;
                }
            }
            if (!best) best = base[k];
        }
        const Row& b = *best;
        rows.append(Row7(std::get<0>(b), std::get<1>(b), std::get<2>(b), std::get<3>(b), std::get<4>(b), std::get<5>(b), by));
    }
    return rows;
}

py::list<Row7> run(const Project& prj, bool learn, bool verbose) {
    if (prj.has_compilands()) {                                    // (a Pascal program in pieces: exact/pascal_join.py)
        py::list<Row7> out;
        for (const auto& r : pascal_join::run(prj.pascal_project(), verbose)) {
            By by;
            if (std::get<4>(r) == py::str("identical")) by.append(prj.cfg_str(py::str("toolchain")));
            out.append(Row7(std::get<0>(r), std::get<1>(r), std::get<2>(r), std::get<3>(r), std::get<4>(r), std::get<5>(r), by));
        }
        return out;
    }
    const auto tcs = prj.toolchains();
    const Symbols syms = prj.symbols();
    Symbols learnt;
    py::list<Row7> rows;
    for (const auto& ap : prj.sources()) {
        const std::optional<int64_t> at = std::get<0>(ap);
        const py::str& path = std::get<1>(ap);
        py::list<std::tuple<py::str, py::list<Row>>> per;
        for (const auto& lt : tcs) {
            per.append(std::make_tuple(std::get<0>(lt), check_file(prj, *std::get<1>(lt), path, at, syms.copy(), learnt)));
        }
        const py::list<Row7> r = _merge(per);
        for (const Row7& x : r) rows.append(x);
        if (verbose) {
            for (const Row7& x : r) {
                py::list<py::str> two;
                const auto& notes = std::get<5>(x);
                for (int64_t i = 0; i < notes.size() && i < 2; ++i) two.append(notes[i]);
                py::print(py::percent(py::str("%-10s %-24s %s %s %s"), std::get<0>(x), std::get<1>(x) ? *std::get<1>(x) : py::str("None"), std::get<4>(x),
                                      by_join(std::get<6>(x)), py::join(py::str("; "), two)));
            }
        }
    }
    if (learn && learnt.size() > 0) {
        Symbols s = prj.symbols();
        for (const auto& kv : learnt.items()) s.setdefault(std::get<0>(kv), std::get<1>(kv));
        prj.save_symbols(s);
    }
    return rows;
}

void write(const Project& prj, const py::list<Row7>& rows) {
    const py::file f = py::file::open(prj.path(py::str("LEDGER")), py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
    f.write(py::percent(py::str("# file, function, bytes, compared with %s, the toolchains that give it (codebreaker exact ledger)\n"),
                        py::os::path::basename(prj.exe())));
    const bool several = prj.toolchains().size() > 1;
    for (const Row7& r : rows) {
        const By& by = std::get<6>(r);
        const py::str name = (std::get<1>(r) && !std::get<1>(r)->empty()) ? *std::get<1>(r) : py::str("-");
        const py::str tail = (!by.empty() && several) ? py::str(" ") + by_join(by) : py::str("");
        f.write(py::percent(py::str("%s %s %d %s%s\n"), std::get<0>(r), name, std::get<3>(r), std::get<4>(r), tail));
    }
    f.close();
}

int64_t main(const Project& prj, const py::list<py::str>& argv) {
    const auto sets = prj.sets();
    py::list<Row7> rows;
    bool learn = false, verbose = false, write_it = false;
    for (const py::str& a : argv) {
        if (a == py::str("--learn")) learn = true;
        if (a == py::str("-v")) verbose = true;
        if (a == py::str("--write")) write_it = true;
    }
    for (int64_t k = 0; k < sets.size(); ++k) {                    // (each set of sources with its own toolchain)
        const Project& part = *sets[k];
        const py::list<Row7> r = run(part, learn, verbose);
        if (sets.size() > 1) {
            int64_t same_ = 0;
            for (const Row7& x : r) same_ += std::get<4>(x) == py::str("identical");
            const std::optional<py::str> tcfg = part.cfg_str(py::str("toolchain"));
            py::print(py::percent(py::str("   %s (%s): %d functions, %d identical"), py::os::path::relpath(part.src(), prj.dir()),
                                  tcfg ? *tcfg : py::str("None"), r.size(), same_));
        }
        for (const Row7& x : r) rows.append(x);
    }
    if (write_it) write(prj, rows);
    const int64_t n = rows.size();
    int64_t same = 0, eq = 0, differ = 0, noplace = 0, nocompile = 0;
    for (const Row7& r : rows) {
        const py::str& v = std::get<4>(r);
        same += v == py::str("identical");
        eq += v == py::str("equivalent");
        differ += v == py::str("differs");
        noplace += v == py::str("no place");
        nocompile += v == py::str("does not compile");
    }
    py::print(py::percent(py::str("%d functions, %d identical, %s%d differ, %d no place, %d files do not compile"), n, same,
                          eq ? py::percent(py::str("%d the same code by another compiler state's habits, "), eq) : py::str(""), differ, noplace, nocompile));
    std::vector<py::str> tcs;
    for (const auto& lt : prj.toolchains()) tcs.push_back(std::get<0>(lt));
    if (tcs.size() > 1) {
        for (const py::str& lb : tcs) {
            int64_t only = 0, gives = 0;
            for (const Row7& r : rows) {
                const By& by = std::get<6>(r);
                bool has = false;
                for (const auto& b : by) {
                    if (b && *b == lb) has = true;
                }
                gives += has;
                only += by.size() == 1 && by[0] && *by[0] == lb;
            }
            py::print(py::percent(py::str("   %-10s gives %d (%d of them it alone)"), lb, gives, only));
        }
    }
    return 0;
}

}  // namespace codebreaker::exact::ledger
