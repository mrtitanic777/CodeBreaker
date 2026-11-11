// codebreaker/exact/modules.cpp -- typed C++ of codebreaker.exact.modules (see modules.hpp).

#include "codebreaker/exact/modules.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/exact/omf.hpp"

namespace codebreaker::exact::modules {

namespace {

using py::bytes;
using py::str;

namespace re = py::re;

const re::Pattern& rx(const char* p) {
    static std::map<std::string, re::Pattern> cache;
    auto it = cache.find(p);
    if (it == cache.end()) {
        it = cache.emplace(p, re::Pattern(str(p), 0)).first;
    }
    return it->second;
}

int64_t hex(const str& s) { return py::int_parse(s, 16); }

// s.split(None, maxsplit): whitespace runs separate, the rest after maxsplit cuts is left as it is
py::list<str> split_ws(const str& s, int64_t maxsplit = -1) {
    py::list<str> out;
    const std::u32string& u = s.u32();
    auto sp = [](char32_t c) { return str(std::u32string(1, c)).isspace(); };
    size_t i = 0;
    int64_t cuts = 0;
    while (true) {
        while (i < u.size() && sp(u[i])) ++i;
        if (i >= u.size()) break;
        if (maxsplit >= 0 && cuts == maxsplit) {
            out.append(str(u.substr(i)));
            break;
        }
        size_t j = i;
        while (j < u.size() && !sp(u[j])) ++j;
        out.append(str(u.substr(i, j - i)));
        ++cuts;
        i = j;
    }
    return out;
}

// the lines of a text file as `for l in open(p)` gives them (each with its newline)
py::list<str> lines_of(const str& p) {
    py::file f = py::open(p);
    py::list<str> out;
    for (const str& l : f) {
        out.append(l);
    }
    return out;
}

// open(p, encoding="latin1").read()
str read_latin1(const str& p) {
    py::file f = py::open(p, str("r"), str("latin1"));
    str t = f.read_text();
    f.close();
    return t;
}

bool is_hex_name(const str& s) {   // re.fullmatch(r"[0-9a-fA-F]{4,5}", s)
    return re::fullmatch(str("[0-9a-fA-F]{4,5}"), s).has_value();
}

// "%s".format(name=seg) as far as a profile's switch uses it
str format_name(const str& fmt, const str& seg) {
    const std::u32string& u = fmt.u32();
    std::u32string out;
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'{') {
            if (i + 1 < u.size() && u[i + 1] == U'{') {
                out += U'{';
                ++i;
            } else if (u.compare(i, 6, U"{name}") == 0) {
                out += seg.u32();
                i += 5;
            } else {
                throw py::ValueError("unsupported replacement field in " + fmt.utf8());
            }
        } else if (u[i] == U'}') {
            if (i + 1 < u.size() && u[i + 1] == U'}') {
                out += U'}';
                ++i;
            } else {
                throw py::ValueError("Single '}' encountered in format string");
            }
        } else {
            out += u[i];
        }
    }
    return str(out);
}

bool truthy(const std::optional<str>& s) { return s.has_value() && !s->empty(); }

}  // namespace

py::list<Row> rows(const Project& prj) {
    py::list<std::tuple<str, std::optional<int64_t>, std::optional<int64_t>, std::optional<str>>> out;
    str p = prj.path(str("MODULES"));
    if (!py::os::path::exists(p)) {
        return py::list<Row>();
    }
    for (const str& line : lines_of(p)) {
        str l = py::split(line, str("#"))[0].strip();
        if (l.empty()) {
            continue;
        }
        py::list<str> f = split_ws(l, 2);                   // name code-start [data-start] [switches]: the data's start
        str rest = py::len(f) > 2 ? f[2] : str("");         // may be left out (the data is not checked then)
        auto m = re::match(str("([0-9a-fA-F]{4,5})\\b\\s*(.*)"), rest);
        std::optional<int64_t> d;
        str sw;
        if (m) {
            d = hex(m->group(1));
            sw = m->group(2).strip();
        } else {
            sw = rest.strip();
        }
        std::optional<int64_t> a;
        if (f[1] != "-") {
            a = hex(f[1]);
        }
        out.append(std::make_tuple(f[0], a, d, sw.empty() ? std::nullopt : std::optional<str>(sw)));
    }
    std::vector<int64_t> code;
    for (const auto& r : out) {
        if (std::get<1>(r).has_value()) {
            code.push_back(*std::get<1>(r));
        }
    }
    std::sort(code.begin(), code.end());
    std::optional<int64_t> lib = lib_start(prj);
    if (lib.has_value()) {   // (the library's part ends the module before it: the program's own modules may go on after it)
        code.push_back(*lib);
        std::sort(code.begin(), code.end());
        code.erase(std::unique(code.begin(), code.end()), code.end());
    }
    py::list<Row> res;
    for (const auto& r : out) {
        const str& n = std::get<0>(r);
        const std::optional<int64_t>& a = std::get<1>(r);
        const std::optional<int64_t>& d = std::get<2>(r);
        const std::optional<str>& x = std::get<3>(r);
        if ((x.has_value() ? *x : str("")).startswith(str("@"))) {   // (a part of the file named, in a code segment of its
            continue;                                                // own: compiled with it - see parts)
        }
        std::optional<int64_t> e;
        if (a.has_value()) {
            std::optional<int64_t> later;
            for (int64_t c : code) {
                if (c > *a) {
                    later = c;
                    break;
                }
            }
            if (later.has_value()) {
                e = later;
            } else if (prj.code_end() != 0 && prj.code_end() > *a) {
                e = prj.code_end();
            } else {
                e = prj.own_end();
            }
        }
        res.append(std::make_tuple(n, a, e, d, x));
    }
    return res;
}

std::optional<int64_t> lib_start(const Project& prj) {
    str p = prj.path(str("CRTLIB"));
    if (!py::os::path::exists(p)) {
        return std::nullopt;
    }
    std::vector<int64_t> at;
    for (const str& l : lines_of(p)) {
        if (l.startswith(str("#"))) {
            continue;
        }
        for (const str& x : rx("\\bcode[= ]([0-9a-f]{5})\\b").findall(l)) {
            at.push_back(hex(x));
        }
        auto m = re::match(str("\\S+(?:\\s+!\\S+)?\\s+#\\s*([0-9a-f]{5})\\b"), l);
        if (m) {
            at.push_back(hex(m->group(1)));
        }
    }
    if (at.empty()) {
        return std::nullopt;
    }
    return *std::min_element(at.begin(), at.end());
}

py::list<std::tuple<int64_t, int64_t, str>> parts(const Project& prj, const str& name) {
    str p = prj.path(str("MODULES"));
    if (!py::os::path::exists(p)) {
        return py::list<std::tuple<int64_t, int64_t, str>>();
    }
    std::vector<std::tuple<int64_t, str, str>> rs;
    for (const str& l : lines_of(p)) {
        py::list<str> f = split_ws(py::split(l, str("#"))[0]);
        if (py::len(f) >= 2 && is_hex_name(f[1])) {
            rs.push_back(std::make_tuple(hex(f[1]), f[0], py::len(f) > 2 ? f[2] : str("")));
        }
    }
    std::sort(rs.begin(), rs.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        if (std::get<1>(a).u32() != std::get<1>(b).u32()) return std::get<1>(a).u32() < std::get<1>(b).u32();
        return std::get<2>(a).u32() < std::get<2>(b).u32();
    });
    py::list<std::tuple<int64_t, int64_t, str>> out;
    for (size_t k = 0; k < rs.size(); ++k) {
        const auto& [a, n, x] = rs[k];
        if (n == name && x.startswith(str("@"))) {
            out.append(std::make_tuple(a, k + 1 < rs.size() ? std::get<0>(rs[k + 1]) : prj.own_end(), x.slice_from(1)));
        }
    }
    return out;
}

py::list<int64_t> starts(const Project& prj) {
    str p = prj.path(str("MODULES"));
    std::vector<int64_t> out;
    if (py::os::path::exists(p)) {
        for (const str& l : lines_of(p)) {
            py::list<str> f = split_ws(py::split(l, str("#"))[0]);
            if (py::len(f) >= 2 && is_hex_name(f[1])) {
                out.push_back(hex(f[1]));
            }
        }
    }
    std::sort(out.begin(), out.end());
    py::list<int64_t> r;
    for (int64_t v : out) {
        r.append(v);
    }
    return r;
}

py::list<str> files_of(const str& src, int64_t start, int64_t end) {
    std::vector<std::tuple<int64_t, int64_t, str>> out;
    // glob.glob(join(src, "[dfe]*.c")): the names in the folder's own order, matched without regard to case
    for (const str& name : py::os::listdir(src)) {
        str low = name.lower();
        if (low.size() < 2 || !low.endswith(str(".c")) || (low[0] != str("d") && low[0] != str("f") && low[0] != str("e"))) {
            continue;
        }
        str p = py::os::path::join(src, name);
        auto m = re::fullmatch(str("([dfe])([0-9a-f]{5})\\.c"), py::os::path::basename(p));
        if (m) {
            int64_t a = hex(m->group(2));
            if (start <= a && a < end) {
                out.push_back(std::make_tuple(a, static_cast<int64_t>(std::string("dfe").find(m->group(1).utf8())), p));
            }
        }
    }
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
        return std::get<2>(a).u32() < std::get<2>(b).u32();
    });
    py::list<str> r;
    for (const auto& t : out) {
        r.append(std::get<2>(t));
    }
    return r;
}

namespace {
std::map<std::u32string, py::dict<str, str>>& dt_cache() {
    static std::map<std::u32string, py::dict<str, str>> c;
    return c;
}
}  // namespace

py::dict<str, str> deftypes(const str& srcdir) {
    auto& cache = dt_cache();
    auto it = cache.find(srcdir.u32());
    if (it == cache.end()) {
        py::dict<str, str> out;
        for (const str& name : py::os::listdir(srcdir)) {   // glob.glob(join(srcdir, "f*.c"))
            str low = name.lower();
            if (low.size() < 2 || !low.endswith(str(".c")) || low[0] != str("f")) {
                continue;
            }
            str t = read_latin1(py::os::path::join(srcdir, name));
            for (const auto& m : rx("(?m)^((?:unsigned \\w+|unsigned|struct \\w+|union \\w+|\\w+)(?: ?\\*+)?)\\n(f[0-9a-f]{5})\\(").finditer(t)) {
                str ty = m.group(1).strip();
                if (ty == "int" || ty == "static" || ty.startswith(str("static")) || ty.startswith(str("extern"))) {
                    continue;
                }
                out[m.group(2)] = ty;
            }
        }
        it = cache.emplace(srcdir.u32(), out).first;
    }
    return it->second;
}

std::tuple<str, py::set<str>> combine(const py::list<str>& paths, const str& default_, const std::optional<str>& header_tag,
                                      const py::dict<str, str>& before) {
    py::list<str> incs, ext, bodies;
    py::set<str> seen, flags;
    std::optional<re::Pattern> fs_rx;
    if (truthy(header_tag)) {
        fs_rx = re::Pattern(str("/\\* ") + *header_tag + str(": ([^*]*?) \\*/\\n"), 0);
    }
    auto inc_key = [](const str& l) {   // (once each file, however the line is written)
        return re::sub(str("[\\s\\\"<>]"), str(""), py::split(l, str("/*"))[0].slice_from(8)).lower();
    };
    for (const str& p : paths) {
        str t = read_latin1(p).replace(str("\r\n"), str("\n"));
        std::optional<re::Match> m = fs_rx ? fs_rx->match(t) : std::nullopt;
        if (py::os::path::basename(p).startswith(str("f"))) {
            flags.add(m ? m->group(1) : default_);
        }
        if (m) {
            t = t.slice_from(m->end());
        }
        t = re::sub(str("/\\* [A-Z]+: [0-9, ]+ \\*/\\n"), str(""), t);    // (a module compiles as it is: no pass's askings answered)
        for (const str& d : rx("/\\* SAMEFILE \\*/\\n([^\\n{;]*\\([^)]*\\))\\s*\\{\\s*\\}").findall(t)) {   // (the empty stand-ins of the
            str key = str("extern ") + re::sub(str("\\(.*\\)"), str("()"), d).strip() + str(";");        //  file's other functions:
            if (!seen.contains(key)) {                                                                    //  declared instead - they
                seen.add(key);                                                                            //  are here now)
                ext.append(key);
            }
        }
        t = re::sub(str("/\\* SAMEFILE \\*/\\n[^\\n{;]*\\([^)]*\\)\\s*\\{\\s*\\}\\n\\n?"), str(""), t);
        py::list<str> lines = py::split(t, str("\n"));
        int64_t k = 0;
        py::list<str> pre;
        while (k < py::len(lines)) {
            const str l = lines[k];
            if (l.startswith(str("/*"))) {
                while (true) {
                    pre.append(lines[k]);
                    if (lines[k].contains(str("*/"))) {
                        break;
                    }
                    k += 1;
                }
            } else if (l.startswith(str("#include"))) {
                str key = inc_key(l);
                bool found = false;
                for (const str& x : incs) {
                    if (inc_key(x) == key) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    incs.append(l);
                }
            } else if (l.startswith(str("extern ")) && re::sub(str("\\s*/\\*.*\\*/\\s*$"), str(""), l).endswith(str(";")) &&
                       !l.contains(str("{"))) {
                str key = re::sub(str("\\s*/\\*.*\\*/\\s*$"), str(""), l).strip();
                if (!seen.contains(key)) {
                    seen.add(key);
                    ext.append(l);
                }
            } else if (l.strip().empty()) {
            } else {
                break;
            }
            k += 1;
        }
        py::list<str> all = pre.copy();
        for (int64_t i = k; i < py::len(lines); ++i) {
            all.append(lines[i]);
        }
        str pfx = before.get(p, str(""));
        bodies.append((pfx + py::join(str("\n"), all)).strip(str("\n")));
    }
    str have = py::join(str("\n"), ext);
    py::list<str> fwd;
    str srcdir = !paths.empty() ? py::os::path::dirname(paths[0]) : str(".");
    str body = py::join(str("\n"), bodies);
    for (const auto& kv : deftypes(srcdir).items()) {
        const str& fn = std::get<0>(kv);
        const str& ty = std::get<1>(kv);
        if (!re::search(str("\\b") + fn + str("\\("), body) || re::search(str("\\b") + fn + str("\\(\\)"), have + body)) {
            continue;
        }
        int64_t stars = py::len(ty) - py::len(ty.rstrip(str("*")));
        fwd.append(py::percent("extern %s %s%s();", ty.rstrip(str("*")).strip(), str(std::string(static_cast<size_t>(stars), '*')), fn));
    }
    py::list<str> head;
    for (const str& x : incs) head.append(x);
    for (const str& x : ext) head.append(x);
    for (const str& x : fwd) head.append(x);
    return {py::join(str("\n"), head) + str("\n\n") + py::join(str("\n\n"), bodies) + str("\n"), flags};
}

std::optional<str> first_segment(const Project& prj, std::optional<int64_t> start) {
    std::optional<str> name = prj.cfg_str(str("first_segment"));
    if (!truthy(name) || !start.has_value()) {
        return std::nullopt;
    }
    std::vector<int64_t> later;
    for (int64_t s : prj.code_segments()) {
        if (s > 0) {
            later.push_back(s);
        }
    }
    if (later.empty() || *start < *std::min_element(later.begin(), later.end())) {
        return name;
    }
    return std::nullopt;
}

std::optional<std::tuple<str, py::list<int64_t>, py::set<str>>> source(const Project& prj, const Row& row) {
    const str& mod = std::get<0>(row);
    std::optional<int64_t> start = std::get<1>(row);
    std::optional<int64_t> end = std::get<2>(row);
    const std::optional<str>& sw = std::get<4>(row);
    if (mod.upper().endswith(str(".OBJ"))) {
        return std::nullopt;
    }
    str whole = py::os::path::join(prj.src(), mod);
    if (py::os::path::exists(whole)) {   // (a file as it is: of data only, or an assembler's)
        str text = read_latin1(whole);
        std::optional<str> seg = first_segment(prj, start);
        if (seg.has_value() && !mod.lower().endswith(str(".c"))) {
            text = text.replace(str("_TEXT"), *seg);   // (the first code segment's name, in the assembly too)
        }
        return std::make_tuple(text, py::list<int64_t>(), py::set<str>());
    }
    py::list<str> paths = start.has_value() ? files_of(prj.src(), *start, *end) : py::list<str>();
    str dp = py::os::path::join(prj.src(), std::get<0>(py::os::path::splitext(mod)) + str("-data.c"));
    if (py::os::path::exists(dp)) {
        paths.append(dp);
    }
    py::dict<str, str> before;
    auto part_list = parts(prj, mod);
    for (const auto& pr : part_list) {   // (the functions of another code segment, after a pragma)
        int64_t a = std::get<0>(pr), e = std::get<1>(pr);
        const str& seg = std::get<2>(pr);
        py::list<str> pp = files_of(prj.src(), a, e);
        py::list<str> fns;
        for (const str& q : pp) {
            for (const auto& d : rx("(?m)^([A-Za-z][^;(\\n]*?\\b(\\w+))\\([^)\\n]*\\)\\s*$").findall_groups<str, str>(
                     [&] {
                         py::file f = py::open(q, str("r"), str("latin1"));
                         str t = f.read_text();
                         f.close();
                         return t;
                     }())) {
                bool have = false;
                for (const str& x : fns) {
                    if (x == std::get<1>(d)) have = true;
                }
                if (!have) {
                    fns.append(std::get<1>(d));
                }
            }
        }
        if (!pp.empty()) {
            before[pp[0]] = py::percent("#pragma alloc_text(%s, %s)\n\n", seg, py::join(str(", "), fns));
        }
        for (const str& q : pp) {
            paths.append(q);
        }
    }
    std::shared_ptr<Toolchain> tc = prj.toolchain();
    str default_ = prj.switches().has_value() ? *prj.switches() : tc->p_get(str("switches")).value_or(str(""));
    auto combined = combine(paths, default_, tc->p_get(str("file_switches")), before);
    const str& text0 = std::get<0>(combined);
    const py::set<str>& flags = std::get<1>(combined);
    str fl;
    if (sw.has_value()) {
        fl = *sw;
    } else if (!flags.empty()) {
        std::vector<str> fv;
        for (const str& f : flags) fv.push_back(f);
        fl = *std::min_element(fv.begin(), fv.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
    } else {
        fl = default_;
    }
    str text = text0;
    std::optional<str> seg = first_segment(prj, start);
    if (seg.has_value() && truthy(tc->p_get(str("segment_switch")))) {   // (a module of the first code segment: its name given)
        fl = (!fl.empty() ? fl + str(" ") : str("")) + format_name(*tc->p_get(str("segment_switch")), *seg);
    }
    if (truthy(tc->p_get(str("file_switches"))) && !fl.empty()) {
        text = py::percent("/* %s: %s */\n", *tc->p_get(str("file_switches")), fl) + text;
    }
    py::list<int64_t> addrs;
    for (const str& p : paths) {
        auto m = re::fullmatch(str("f([0-9a-f]{5})\\.c"), py::os::path::basename(p));
        if (!m) {
            continue;
        }
        int64_t a = hex(m->group(1));
        bool inpart = false;   // (the functions of the main code segment: another one's are not made stand-ins)
        for (const auto& pr : part_list) {
            if (std::get<0>(pr) <= a && a < std::get<1>(pr)) inpart = true;
        }
        if (!inpart) {
            addrs.append(a);
        }
    }
    return std::make_tuple(text, addrs, flags);
}

std::tuple<std::shared_ptr<const Project>, std::shared_ptr<Toolchain>> set_of(const Project& prj, const str& mod) {
    py::list<std::shared_ptr<const Project>> also = prj.also_sets();
    for (const std::shared_ptr<const Project>& part : also) {
        for (const char* k : {"unplaced", "sources"}) {
            std::optional<str> pat = part->cfg_str(str(k));
            if (truthy(pat) && re::fullmatch(*pat, mod, re::I).has_value()) {
                return {part, part->toolchain()};
            }
        }
    }
    return {nullptr, prj.toolchain()};
}

std::tuple<std::optional<bytes>, str, py::list<int64_t>> compile_module(const Project& prj, const Row& row, const std::shared_ptr<Toolchain>& tc_in) {
    auto s = source(prj, row);
    if (!s.has_value()) {
        return {std::nullopt, str(""), py::list<int64_t>()};
    }
    const str& text = std::get<0>(*s);
    const py::list<int64_t>& addrs = std::get<1>(*s);
    const Project* part = &prj;
    std::shared_ptr<const Project> keep;
    std::shared_ptr<Toolchain> tc = tc_in;
    if (py::os::path::exists(py::os::path::join(prj.src(), std::get<0>(row)))) {   // (a whole file: by the toolchain of the set it is of)
        auto [p, own] = set_of(prj, std::get<0>(row));
        if (p) {
            keep = p;
            part = p.get();
            tc = own;
        } else {
            tc = tc ? tc : own;
        }
    }
    if (!tc) {
        tc = prj.toolchain();
    }
    auto r = tc->compile(text.encode("latin1"), part->headers(), part->include(), part->switches());
    return {std::get<0>(r), std::get<1>(r), addrs};
}

namespace {

compare::Obj to_compare(const formats::omf::Obj& o) {
    compare::Obj c;
    c.seg = o.seg;
    for (const auto& f : o.fix) {
        const auto& tg = std::get<3>(f);
        str second;   // (a group's index, or nothing, as text: the places compare.py asks of a target are names)
        if (std::holds_alternative<str>(std::get<1>(tg))) {
            second = std::get<str>(std::get<1>(tg));
        } else if (std::holds_alternative<int64_t>(std::get<1>(tg))) {
            second = str(std::to_string(std::get<int64_t>(std::get<1>(tg))));
        }
        c.fix.append(std::make_tuple(std::get<0>(f), std::get<1>(f), std::get<2>(f), compare::Target(std::get<0>(tg), second), std::get<4>(f)));
    }
    for (const auto& p : o.pubs) {
        c.pubs.append(std::make_tuple(std::get<0>(p), std::get<1>(p).value_or(str("")), std::get<2>(p)));
    }
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

int64_t W(const bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<H", b, k); }

}  // namespace

CheckResult check(const Project& prj, const Row& row, const compare::Decoder& decode, const std::optional<bytes>& raw_in) {
    const str& mod = std::get<0>(row);
    std::optional<int64_t> dstart = std::get<3>(row);
    CheckResult out;
    if (mod.upper().endswith(str(".OBJ"))) {
        out.obj = true;
        return out;
    }
    std::optional<bytes> raw = raw_in;
    if (!raw.has_value()) {
        auto c = compile_module(prj, row);
        if (!std::get<0>(c).has_value()) {
            out.error = std::get<1>(c);
            return out;
        }
        raw = std::get<0>(c);
    }
    formats::omf::Obj o = formats::omf::read(*raw);
    compare::Obj co = to_compare(o);
    compare::Project cp = to_compare(prj);
    bytes img = prj.image();
    Symbols syms = prj.symbols();
    Symbols got;
    py::list<str> segs = formats::omf::code_segments(o);
    str text = !segs.empty() ? segs[0] : str("_TEXT");
    for (const auto& fn : formats::omf::functions(o, text)) {
        const str& name = std::get<0>(fn);
        int64_t off = std::get<1>(fn), ln = std::get<2>(fn);
        auto own = re::fullmatch(str("_?f([0-9a-f]{5})"), name);
        std::optional<int64_t> a;
        if (own) {
            a = hex(own->group(1));
        } else if (syms.contains(name) && std::get<0>(syms.at(name)) == "C") {
            a = std::get<1>(syms.at(name));
        }
        if (!a.has_value()) {
            out.res.append(std::make_tuple(name, std::nullopt, ln, str("no place")));
            continue;
        }
        auto br = compare::function(co, img, name, off, ln, *a, cp, syms, got, decode, text);
        bool same = compare::identical(std::get<0>(br), std::get<1>(br));
        out.res.append(std::make_tuple(name, a, ln, same ? str("same") : py::percent("differs %d", py::len(std::get<0>(br)))));
    }
    bytes d;
    if (dstart.has_value()) {   // (no data start: not checked)
        auto it = o.seg.get(str("_DATA"), bytes());
        int64_t filled = o.filled.get(str("_DATA"), 0);
        d = it.slice(0, filled);
    }
    bytes ds = img.slice_from(prj.dgroup());
    py::dict<int64_t, std::tuple<std::tuple<str, std::variant<std::monostate, str, int64_t>>, int64_t>> holes;
    for (const auto& f : o.fix) {
        if (std::get<0>(f) == "_DATA" && std::get<2>(f) == "off") {
            holes[std::get<1>(f)] = std::make_tuple(std::get<3>(f), std::get<4>(f));
        }
    }
    int64_t k = 0;
    while (k < py::len(d)) {
        if (holes.contains(k)) {
            auto [tgt, disp] = holes.at(k);
            if (*dstart + k + 2 > py::len(ds)) {
                out.dbad.append(py::percent("%04x: past the program's data", *dstart + k));
                k += 2;
                continue;
            }
            int64_t theirs = W(ds, *dstart + k);
            int64_t mine = (W(d, k) + disp) & 0xFFFF;
            const auto& second = std::get<1>(tgt);
            bool second_is_data = std::holds_alternative<str>(second) && std::get<str>(second) == "_DATA";
            if (std::get<0>(tgt) != "ext" && second_is_data && theirs != ((*dstart + mine) & 0xFFFF)) {
                out.dbad.append(py::percent("%04x: -> %04x, the program's -> %04x", *dstart + k, *dstart + mine, theirs));
            }
            k += 2;
            continue;
        }
        if (*dstart + k >= py::len(ds) || ds[*dstart + k] != d[k]) {
            out.dbad.append(py::percent("%04x: %02x, the program's %02x", *dstart + k, d[k],
                                        *dstart + k < py::len(ds) ? ds[*dstart + k] : -1));
        }
        k += 1;
    }
    out.dlen = py::len(d);
    out.raw = raw;
    return out;
}

int64_t main(const Project& prj, const py::list<str>& argv, const compare::Decoder& decode) {
    py::list<str> want;
    for (const str& a : argv) {
        if (!a.startswith(str("-"))) {
            want.append(a);
        }
    }
    bool verbose = false;
    for (const str& a : argv) {
        if (a == "-v") verbose = true;
    }
    int64_t tot = 0, same = 0;
    for (const Row& row : rows(prj)) {
        const str& mod = std::get<0>(row);
        bool wanted = want.empty();
        for (const str& w : want) {
            if (w == mod || w == std::get<0>(py::os::path::splitext(mod))) wanted = true;
        }
        if (!wanted) {
            continue;
        }
        CheckResult j = check(prj, row, decode);
        if (j.obj) {
            py::print(py::percent("%-14s the kit's object", mod));
            continue;
        }
        if (j.error.has_value()) {
            py::list<str> lines = py::splitlines(j.error->strip());
            py::list<str> last;
            if (!lines.empty()) {
                last.append(lines[py::len(lines) - 1]);
            }
            py::print(py::percent("%-14s does not compile: %s", mod, last));
            continue;
        }
        int64_t nsame = 0;
        for (const auto& x : j.res) {
            if (std::get<3>(x) == "same") nsame += 1;
        }
        tot += py::len(j.res);
        same += nsame;
        int64_t d3 = std::get<3>(row).value_or(0);
        py::print(py::percent("%-14s %s  %3d functions, %3d identical; data %04x..%04x %s", mod,
                              std::get<1>(row).has_value() ? py::percent("%05x", *std::get<1>(row)) : str("  -  "), py::len(j.res),
                              nsame, d3, d3 + j.dlen, j.dbad.empty() ? str("IDENTICAL") : py::percent("%d differ", py::len(j.dbad))));
        if (verbose) {
            for (const auto& x : j.res) {
                if (std::get<3>(x) != "same") {
                    py::print(py::percent("     %-12s %s %s", std::get<0>(x),
                                          std::get<1>(x).has_value() ? py::percent("%05x", *std::get<1>(x)) : str("?"), std::get<3>(x)));
                }
            }
            for (int64_t i = 0; i < std::min<int64_t>(20, py::len(j.dbad)); ++i) {
                py::print(str("     data ") + j.dbad[i]);
            }
        }
    }
    py::print(py::percent("%d functions in the modules, %d identical", tot, same));
    return 0;
}

}  // namespace codebreaker::exact::modules
