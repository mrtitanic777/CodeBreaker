// codebreaker/exact/names.cpp -- typed C++ of codebreaker.exact.names (see names.hpp).

#include "codebreaker/exact/names.hpp"

#include <algorithm>

#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/x86.hpp"

namespace codebreaker::exact::names {

const py::str ADDR = py::str("0x[0-9a-f]{3,}");
const py::str CODE_NAME = py::str("(?:f|P)([0-9a-fA-F]{4,5})");
const py::str DATA_NAME = py::str("(?:g_|G)([0-9a-fA-F]{1,4})");

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

int64_t hex(const str& s) { return py::int_parse(s, 16); }

py::list<str> split_ws(const str& s) {
    py::list<str> out;
    const std::u32string& u = s.u32();
    auto sp = [](char32_t c) { return str(std::u32string(1, c)).isspace(); };
    size_t i = 0;
    while (true) {
        while (i < u.size() && sp(u[i])) ++i;
        if (i >= u.size()) break;
        size_t j = i;
        while (j < u.size() && !sp(u[j])) ++j;
        out.append(str(u.substr(i, j - i)));
        i = j;
    }
    return out;
}

bool addr_name(const str& n) { return re::fullmatch(str("_?f[0-9a-f]{5}"), n).has_value(); }   // (a function named after its place)

// a function's rows: the masked instructions as one text (the tuple of them), and the rows
struct Func {
    py::list<Row> rows;
    str key;                    // the masked instructions, a tuple in the Python: joined (no instruction holds a \x01)
    int64_t calls_n = 0;
    py::list<int64_t> calls;    // [c for x, c, d in r if c is not None]
};

Func make_func(project::Project& prj, int64_t a, int64_t ln) {
    Func f;
    f.rows = read(prj, a, ln);
    std::u32string k;
    for (const Row& r : f.rows) {
        k += std::get<0>(r).u32();
        k += U'\x01';
        if (std::get<1>(r)) f.calls.append(*std::get<1>(r));
    }
    f.key = str(k);
    return f;
}

}  // namespace

std::tuple<py::dict<int64_t, str>, py::dict<int64_t, str>> known(project::Project& prj, const std::optional<py::set<int64_t>>& starts0) {
    py::dict<int64_t, str> code, data;
    py::set<int64_t> starts;
    if (!starts0) {
        for (const auto& f : functions(prj)) starts.add(std::get<0>(f));
    } else {
        starts = *starts0;
    }
    for (const auto& kv : prj.symbols().items()) {
        const str& n = std::get<0>(kv);
        const str& t = std::get<0>(std::get<1>(kv));
        int64_t a = std::get<1>(std::get<1>(kv));
        if (n.contains(str(":")) || n.contains(str("@"))) continue;
        if (t == "C" || (t == "?" && starts.contains(a))) {
            code.setdefault(a, n);
        } else if (t == "D" || t == "?") {
            data.setdefault(a, n);
        }
    }
    for (const char* key : {"place_by", "names"}) {
        const std::optional<str>& f = std::string(key) == "place_by" ? prj.cfg.place_by : prj.cfg.names;
        std::optional<str> p;
        if (f && !f->empty()) p = std::string(key) == "place_by" ? prj.path(*f) : py::os::path::join(prj.dir, *f);
        if (!p || !py::os::path::exists(*p)) continue;
        py::file fh = py::open(*p, str("r"), str("utf-8"));
        for (const str& l : fh) {
            py::list<str> c = split_ws(py::split(l, str("#"))[0]);
            if (py::len(c) < 2) continue;
            for (const auto& xn : {std::make_tuple(c[1], c[0]), std::make_tuple(c[0], c[1])}) {
                const str& x = std::get<0>(xn);
                const str& n = std::get<1>(xn);
                auto m = re::fullmatch(CODE_NAME, x);
                if (m && !re::fullmatch(CODE_NAME, n)) {
                    code.setdefault(hex(m->group(1)), n);
                    break;
                }
                m = re::fullmatch(DATA_NAME, x);
                if (m && !re::fullmatch(DATA_NAME, n)) {
                    data.setdefault(hex(m->group(1)), n);
                    break;
                }
            }
        }
        fh.close();
    }
    return std::make_tuple(code, data);
}

py::list<std::tuple<int64_t, int64_t>> functions(project::Project& prj) {
    py::list<std::tuple<int64_t, int64_t>> fs;
    try {
        for (const auto& f : carve::functions(prj)) fs.append(std::make_tuple(std::get<0>(f), std::get<1>(f)));
    } catch (const py::Exception&) {
        fs = py::list<std::tuple<int64_t, int64_t>>();
    }
    if (fs.empty()) {                                       // (no carver for the toolchain: the places named)
        auto code = std::get<0>(known(prj, py::set<int64_t>()));
        py::list<int64_t> starts = py::sorted(py::list<int64_t>::from(code.keys()));
        for (int64_t k = 0; k < starts.size(); ++k) {
            int64_t a = starts[k];
            int64_t nx = k + 1 < starts.size() ? starts[k + 1] : std::min<int64_t>(prj.image.size(), a + 0x400);
            fs.append(std::make_tuple(a, nx - a));
        }
    }
    return fs;
}

py::list<Row> read(project::Project& prj, int64_t a, int64_t ln) {
    py::list<Row> out;
    for (const x86::Insn& i : x86::disasm(prj.image, a, a + ln, false)) {
        str ops = i.op_str;
        std::optional<int64_t> tgt;
        if (i.mnemonic == "call" && re::fullmatch(str("0x[0-9a-f]+"), ops)) {
            tgt = hex(ops.slice(2, ops.size()));
            ops = str("T");
        } else if (i.mnemonic.startswith(str("j")) || i.mnemonic.startswith(str("loop"))) {
            ops = str("J");                                 // (a jump within the function: its shape is the code)
        }
        std::optional<int64_t> dat;
        auto m = re::search(str("\\[(0x[0-9a-f]+)\\]"), ops);
        if (m) dat = hex(m->group(1).slice(2, m->group(1).size()));
        out.append(std::make_tuple(i.mnemonic + str(" ") + re::sub(ADDR, str("N"), ops), tgt, dat));
    }
    return out;
}

Carried carry(project::Project& prj, project::Project& other) {
    py::dict<int64_t, Func> mine, theirs;
    for (const auto& f : functions(prj)) mine.set(std::get<0>(f), make_func(prj, std::get<0>(f), std::get<1>(f)));
    for (const auto& f : functions(other)) theirs.set(std::get<0>(f), make_func(other, std::get<0>(f), std::get<1>(f)));
    auto kn = known(other);
    py::dict<int64_t, str> ocode = std::get<0>(kn), odata = std::get<1>(kn);
    py::dict<str, py::list<int64_t>> by_m, by_t;
    for (const auto& kv : mine.items()) by_m.setdefault(std::get<1>(kv).key, py::list<int64_t>()).append(std::get<0>(kv));
    for (const auto& kv : theirs.items()) by_t.setdefault(std::get<1>(kv).key, py::list<int64_t>()).append(std::get<0>(kv));
    Carried res;
    py::dict<int64_t, int64_t>& pairs = res.pairs;
    py::dict<int64_t, int64_t>& dpairs = res.dpairs;
    py::dict<int64_t, str> how;
    py::set<int64_t> pvals;                                 // (pairs.values())
    auto add_pair = [&](int64_t a, int64_t b, const str& why) {
        pairs.set(a, b);
        pvals.add(b);
        how.set(a, why);
    };
    for (const auto& kv : by_m.items()) {                   // 1. the same code, once on each side
        const py::list<int64_t>& ms = std::get<1>(kv);
        py::list<int64_t> ts = by_t.get(std::get<0>(kv), py::list<int64_t>());
        // len(k) >= 4: the instructions of the code
        if (ms.size() == 1 && ts.size() == 1 && mine.at(ms[0]).rows.size() >= 4) add_pair(ms[0], ts[0], str("the same code"));
    }
    py::dict<int64_t, str> mcode0 = std::get<0>(known(prj));
    for (const auto& kv : mcode0.items()) {                 // (anchors: what both name alike - the library's)
        int64_t a = std::get<0>(kv);
        const str& n = std::get<1>(kv);
        std::optional<int64_t> b;
        for (const auto& ob : ocode.items()) {
            if (std::get<1>(ob) == n) {
                b = std::get<0>(ob);
                break;
            }
        }
        if (b && mine.contains(a) && theirs.contains(*b) && !pairs.contains(a) && !pvals.contains(*b) && !addr_name(n)) add_pair(a, *b, str("named so on both sides"));
    }
    auto ocode_name = [&](int64_t b) {                      // (ocode.get(b) or "f%05x" % b)
        auto n = ocode.get(b);
        return n && !n->empty() ? *n : py::percent(str("f%05x"), b);
    };
    std::function<void()> spread = [&]() {
        py::list<std::tuple<int64_t, int64_t>> work;
        for (const auto& p : pairs.items()) work.append(p);
        while (!work.empty()) {                             // 2. calls and data at the same places
            auto ab = work.pop();
            int64_t a = std::get<0>(ab), b = std::get<1>(ab);
            const Func& ra = mine.at(a);
            const Func& rb = theirs.at(b);
            bool same = ra.key == rb.key;
            const py::list<int64_t>& ca_ = ra.calls;
            const py::list<int64_t>& cb_ = rb.calls;
            if (same || ca_.size() == cb_.size()) {         // (the same code, or as many calls in it)
                int64_t n = std::min(ca_.size(), cb_.size());
                for (int64_t k = 0; k < n; ++k) {
                    int64_t ca = ca_[k], cb = cb_[k];
                    bool alike = same;
                    if (!alike && mine.contains(ca) && theirs.contains(cb) && mine.at(ca).calls.size() == theirs.at(cb).calls.size()) {
                        double r = static_cast<double>(mine.at(ca).rows.size()) / static_cast<double>(std::max<int64_t>(theirs.at(cb).rows.size(), 1));
                        alike = 0.8 <= r && r <= 1.25;      // (the callees alike: a caller of the same code may call another function there in this version)
                    }
                    if (alike && mine.contains(ca) && theirs.contains(cb) && !pairs.contains(ca) && !pvals.contains(cb)) {
                        add_pair(ca, cb, py::percent(str("called where %s calls it"), ocode_name(b)));
                        work.append(std::make_tuple(ca, cb));
                    }
                }
            }
            if (same) {
                int64_t n = std::min(ra.rows.size(), rb.rows.size());
                for (int64_t k = 0; k < n; ++k) {
                    const auto& da = std::get<2>(ra.rows[k]);
                    const auto& db = std::get<2>(rb.rows[k]);
                    if (da && db && !dpairs.contains(*da)) dpairs.set(*da, *db);
                }
            }
        }
    };
    spread();
    while (true) {                                          // 3. who calls the paired ones, in the same order
        py::dict<str, py::list<int64_t>> sig_m, sig_t;
        for (const auto& kv : mine.items()) {
            if (pairs.contains(std::get<0>(kv))) continue;
            std::u32string s;
            int64_t known_n = 0;
            for (int64_t c : std::get<1>(kv).calls) {
                if (pairs.contains(c)) {
                    s += str(std::to_string(pairs.at(c))).u32();
                    ++known_n;
                } else {
                    s += U'?';
                }
                s += U',';
            }
            if (known_n >= 2) sig_m.setdefault(str(s), py::list<int64_t>()).append(std::get<0>(kv));
        }
        for (const auto& kv : theirs.items()) {
            if (pvals.contains(std::get<0>(kv))) continue;
            std::u32string s;
            int64_t known_n = 0;
            for (int64_t c : std::get<1>(kv).calls) {
                if (pvals.contains(c)) {
                    s += str(std::to_string(c)).u32();
                    ++known_n;
                } else {
                    s += U'?';
                }
                s += U',';
            }
            if (known_n >= 2) sig_t.setdefault(str(s), py::list<int64_t>()).append(std::get<0>(kv));
        }
        py::list<std::tuple<int64_t, int64_t>> nw;
        for (const auto& kv : sig_m.items()) {
            const auto& ms = std::get<1>(kv);
            py::list<int64_t> ts = sig_t.get(std::get<0>(kv), py::list<int64_t>());
            if (ms.size() == 1 && ts.size() == 1) nw.append(std::make_tuple(ms[0], ts[0]));
        }
        if (nw.empty()) break;
        for (const auto& ab : nw) add_pair(std::get<0>(ab), std::get<1>(ab), py::percent(str("calls what %s calls, in its order"), ocode_name(std::get<1>(ab))));
        spread();
    }
    py::set<str> used;
    py::dict<int64_t, str> mcode = std::get<0>(known(prj));
    for (const auto& ab : py::sorted(py::list<std::tuple<int64_t, int64_t>>::from(pairs.items()))) {
        int64_t a = std::get<0>(ab), b = std::get<1>(ab);
        auto n = ocode.get(b);
        if (n && !n->empty() && re::fullmatch(str("_?f[0-9a-f]{5}|P[0-9A-F]{5}"), *n)) continue;   // (an address-name is no name to carry)
        bool clash = n && mcode.contains(a) && mcode.at(a) != *n && !addr_name(mcode.at(a));
        if (n && !n->empty() && !used.contains(*n) && !clash) {
            res.names.set(a, std::make_tuple(*n, how.at(a)));
            used.add(*n);
        }
    }
    py::set<str> dused;
    for (const auto& ab : py::sorted(py::list<std::tuple<int64_t, int64_t>>::from(dpairs.items()))) {
        auto n = odata.get(std::get<1>(ab));
        if (n && !n->empty() && !dused.contains(*n)) {
            res.dnames.set(std::get<0>(ab), std::make_tuple(*n, str("named where the same code names it")));
            dused.add(*n);
        }
    }
    return res;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    auto say = [&](const str& s) { out << s.utf8() << "\n"; };
    if (!argv.contains(str("--from"))) {
        say(str("names: --from OTHER (a project of another version of the program, with names)"));
        return 1;
    }
    str f = argv[argv.index(str("--from")) + 1];
    auto other = project::load(py::os::path::isabs(f) ? f : py::os::path::join(py::os::getcwd(), f));
    Carried c = carry(prj, *other);
    py::dict<int64_t, str> mcode = std::get<0>(known(prj));
    int64_t agree = 0;
    py::list<std::tuple<int64_t, str, str>> clash;
    for (const auto& kv : c.names.items()) {
        int64_t a = std::get<0>(kv);
        const str& n = std::get<0>(std::get<1>(kv));
        if (mcode.contains(a) && mcode.at(a) == n) ++agree;
        if (mcode.contains(a) && mcode.at(a) != n && !addr_name(mcode.at(a))) clash.append(std::make_tuple(a, n, mcode.at(a)));
    }
    if (argv.contains(str("-v"))) {
        for (const auto& kv : py::sorted(py::list<std::tuple<int64_t, std::tuple<str, str>>>::from(c.names.items()))) {
            int64_t a = std::get<0>(kv);
            say(py::percent(str("  f%05x %-24s %s%s"), a, std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv)),
                            !mcode.contains(a) ? str("") : py::percent(str("  (has: %s)"), mcode.at(a))));
        }
    }
    say(py::percent(str("%d functions named (%d of them as the project names them already, %d it names otherwise), %d data"), c.names.size(), agree,
                    clash.size(), c.dnames.size()));
    for (int64_t k = 0; k < std::min<int64_t>(clash.size(), 10); ++k) {
        say(py::percent(str("   f%05x: %s here, %s by the other"), std::get<0>(clash[k]), std::get<2>(clash[k]), std::get<1>(clash[k])));
    }
    if (argv.contains(str("--write"))) {
        auto s = prj.symbols();
        for (const auto& kv : c.names.items()) s.setdefault(std::get<0>(std::get<1>(kv)), std::make_tuple(str("C"), std::get<0>(kv)));
        for (const auto& kv : c.dnames.items()) s.setdefault(std::get<0>(std::get<1>(kv)), std::make_tuple(str("D"), std::get<0>(kv)));
        prj.save_symbols(s);
        say(str("written to ") + prj.path(str("SYMBOLS")));
    }
    return 0;
}

}  // namespace codebreaker::exact::names
