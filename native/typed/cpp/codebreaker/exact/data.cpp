// codebreaker/exact/data.cpp -- typed C++ of codebreaker.exact.data (see data.hpp).

#include "codebreaker/exact/data.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>

namespace codebreaker::exact::data {

using py::str;
namespace re = py::re;
namespace omf = codebreaker::formats::omf;

const str BS = str(std::u32string(1, 92));


namespace {

// a pattern compiled once per literal (a Pattern is a handle to pyre's cached regex)
const re::Pattern& rx(const char* pattern, int64_t flags = 0) {
    static std::map<std::pair<const char*, int64_t>, re::Pattern> compiled;
    auto key = std::make_pair(pattern, flags);
    auto it = compiled.find(key);
    if (it == compiled.end()) it = compiled.emplace(key, re::compile(str(pattern), flags)).first;
    return it->second;
}

str pct(const char* fmt, int64_t v) { return py::percent(str(fmt), v); }

bool truthy(const std::optional<str>& s) { return s.has_value() && !s->empty(); }

// int.from_bytes(b, "little") of the bytes there are
int64_t from_bytes_le(const py::bytes& b) {
    int64_t v = 0;
    for (int64_t i = py::len(b) - 1; i >= 0; i--) v = (v << 8) | b[i];
    return v;
}

// sorted(): the strings in code point order
template <class C> std::vector<str> sorted_strs(const C& c) {
    std::vector<str> out;
    for (const auto& x : c) out.push_back(x);
    std::sort(out.begin(), out.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
    return out;
}

// glob.glob(join(src, PREFIX + "*" + SUFFIX)) on Windows: the names in the folder's own order, matched without regard to case
py::list<str> glob_names(const str& dir, const char* prefix, const char* suffix) {
    py::list<str> out;
    str p = str(prefix), s = str(suffix);
    py::list<str> names;
    try {
        names = py::os::listdir(dir);
    } catch (const py::OSError&) {                           // (glob.glob of a folder that is not there: nothing)
        return out;
    }
    for (const str& name : names) {
        str low = name.lower();
        if (low.size() >= p.size() + s.size() && low.startswith(p) && low.endswith(s)) out.append(py::os::path::join(dir, name));
    }
    return out;
}

// the lines of a latin-1 text file as `for l in open(p, encoding="latin1")` gives them (each with its newline)
py::list<str> lines_latin1(const str& p) {
    py::file f = py::open(p, str("r"), str("latin1"));
    py::list<str> out;
    for (const str& l : f) out.append(l);
    f.close();
    return out;
}

int depth_ = 0;
struct Depth {
    Depth() {
        if (++depth_ > 480) {
            depth_ = 0;
            throw RecursionError("maximum recursion depth exceeded");
        }
    }
    ~Depth() {
        if (depth_ > 0) --depth_;
    }
};

}  // namespace

str cstr(const py::bytes& b) {
    std::u32string o;
    for (int64_t c : b) {
        char32_t ch = static_cast<char32_t>(c);
        if (ch == U'\n') {
            o += U"\\n";
        } else if (ch == U'\t') {
            o += U"\\t";
        } else if (ch == U'\r') {
            o += U"\\r";
        } else if (ch == U'"' || ch == U'\\') {
            o += U'\\';
            o += ch;
        } else if (c >= 32 && c < 127) {
            o += ch;
        } else {
            o += U'\\';
            o += str(pct("%03o", c)).u32();
        }
    }
    return str(U"\"" + o + U"\"");
}

std::optional<std::tuple<Obj, py::list<int64_t>>> _functions_only(const modules::Project& prj, const modules::Row& row) {
    const str& mod = std::get<0>(row);
    const std::optional<int64_t>& a = std::get<1>(row);
    const std::optional<int64_t>& e = std::get<2>(row);
    const std::optional<str>& x = std::get<4>(row);
    py::list<str> paths;
    if (a.has_value()) {
        for (const str& q : modules::files_of(prj.src(), *a, *e))
            if (py::os::path::basename(q).startswith(str("f"))) paths.append(q);
    }
    if (paths.empty()) return std::nullopt;
    std::shared_ptr<modules::Toolchain> tc = prj.toolchain();
    str default_ = prj.switches().has_value() ? *prj.switches() : tc->p_get(str("switches")).value_or(str(""));
    std::optional<str> header_tag = tc->p_get(str("file_switches"));
    auto combined = modules::combine(paths, default_, header_tag);
    str text = std::get<0>(combined);
    py::set<str> flags = std::get<1>(combined);
    str fl;
    if (truthy(x)) {
        fl = *x;
    } else if (!flags.empty()) {
        fl = sorted_strs(flags)[0];
    } else {
        fl = default_;
    }
    if (truthy(header_tag) && !fl.empty()) text = py::percent(str("/* %s: %s */\n"), *header_tag, fl) + text;
    auto r = tc->compile(text.encode("latin1"), prj.headers(), prj.include(), prj.switches());
    if (!std::get<0>(r).has_value()) {
        const str& con = std::get<1>(r);
        throw py::SystemExit(py::percent(str("%s's functions do not compile alone:\n%s"), mod, con.slice_from(std::max<int64_t>(py::len(con) - 1500, 0))));
    }
    py::list<int64_t> addrs;
    for (const str& q : paths) addrs.append(py::int_parse(py::os::path::basename(q).slice(1, 6), 16));
    return std::make_tuple(omf::read(*std::get<0>(r)), addrs);
}

Walked walk(const modules::Project& prj, const modules::Row& row) {
    const std::optional<int64_t>& dstart_opt = std::get<3>(row);
    py::list<modules::Row> rows;
    rows = modules::rows(prj);
    if (!dstart_opt.has_value()) {
        for (const modules::Row& r : rows)
            if (std::get<3>(r).has_value()) throw py::TypeError("'>' not supported between instances of 'int' and 'NoneType'");
        throw py::TypeError("no data start: '%04x' format needs a real number, not NoneType");
    }
    int64_t dstart = *dstart_opt;
    std::vector<int64_t> nxt;
    for (const modules::Row& r : rows)
        if (std::get<3>(r).has_value() && *std::get<3>(r) > dstart) nxt.push_back(*std::get<3>(r));
    std::sort(nxt.begin(), nxt.end());
    py::bytes ds = prj.image().slice_from(prj.dgroup());
    int64_t dend = !nxt.empty() ? nxt[0] : py::len(ds);
    std::optional<std::tuple<Obj, py::list<int64_t>>> got;
    got = _functions_only(prj, row);
    Walked w;
    w.dstart = dstart;
    w.dend = dend;
    if (!got.has_value()) {
        w.runs.append(std::make_tuple(dstart, dend, str("(start)"), str("vars")));
        return w;
    }
    const Obj& o = std::get<0>(*got);
    const py::list<int64_t>& addrs = std::get<1>(*got);
    str text = omf::code_segments(o)[0];
    py::list<std::tuple<str, int64_t, int64_t>> fns = omf::functions(o, text);
    py::bytes lit = o.seg.get(str("_DATA"), py::bytes()).slice(0, o.filled.get(str("_DATA"), 0));
    py::dict<int64_t, str> owner;
    for (const auto& f : o.fix) {
        const str& seg = std::get<0>(f);
        int64_t fo = std::get<1>(f);
        const str& kind = std::get<2>(f);
        const omf::Target& tgt = std::get<3>(f);
        int64_t disp = std::get<4>(f);
        if (seg == text && kind == "off" && std::get<0>(tgt) == "seg" && std::holds_alternative<str>(std::get<1>(tgt)) &&
            std::get<str>(std::get<1>(tgt)) == "_DATA") {
            int64_t tw = (from_bytes_le(o.seg.at(text).slice(fo, fo + 2)) + disp) & 0xFFFF;
            std::optional<str> name;
            for (const auto& fn : fns) {
                if (std::get<1>(fn) <= fo && fo < std::get<1>(fn) + std::get<2>(fn)) {
                    name = std::get<0>(fn);
                    break;
                }
            }
            if (name.has_value() && !owner.contains(tw)) owner[tw] = *name;
        }
    }
    py::list<Lit> strs;
    int64_t k = 0;
    py::bytes nul(std::string(1, '\0'));
    while (k < py::len(lit)) {
        int64_t nz = lit.find(nul, k);
        int64_t e2 = nz >= 0 ? nz : py::len(lit);
        std::optional<str> own;
        if (owner.contains(k)) own = owner.at(k);
        strs.append(std::make_tuple(k, lit.slice(k, e2 + 1), own));
        k = e2 + 1;
    }
    std::optional<str> cur;
    for (int64_t i = 0; i < py::len(strs); i++) {
        const Lit& l = strs[i];
        if (!std::get<2>(l).has_value()) {
            strs[i] = std::make_tuple(std::get<0>(l), std::get<1>(l), cur);
        } else {
            cur = std::get<2>(l);
        }
    }
    py::list<py::bytes> lits;
    for (const Lit& l : strs) lits.append(std::get<1>(l));
    py::list<std::optional<int64_t>> places = _align(ds, lits, dstart, dend);
    py::list<std::tuple<int64_t, int64_t, std::optional<str>, str>> runs;
    int64_t pos = dstart;
    std::optional<str> last;
    for (int64_t i = 0; i < py::len(strs); i++) {
        const std::optional<int64_t>& at = places[i];
        if (!at.has_value()) continue;                       // (missing)
        if (*at != pos) runs.append(std::make_tuple(pos, *at, last, str("?")));
        pos = *at + py::len(std::get<1>(strs[i]));
        last = std::get<2>(strs[i]);
    }
    if (pos < dend) runs.append(std::make_tuple(pos, dend, last, str("?")));
    for (const auto& r : runs) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        py::bytes seg = ds.slice(lo, hi);
        bool all_text = true;
        int64_t zeros = 0;
        for (int64_t c : seg) {
            if (!((c >= 32 && c < 127) || c == 0 || c == 9 || c == 10 || c == 13)) all_text = false;
            if (c == 0) zeros++;
        }
        bool txt = all_text && zeros <= std::max<int64_t>(1, py::len(seg) / 6);
        w.runs.append(std::make_tuple(lo, hi, std::get<2>(r).has_value() && !std::get<2>(r)->empty() ? *std::get<2>(r) : str("(start)"),
                                      txt ? str("strings") : str("vars")));
    }
    w.strings = strs;
    // list(zip(names, addrs))
    int64_t n = std::min<int64_t>(py::len(fns), py::len(addrs));
    for (int64_t i = 0; i < n; i++) w.functions.append(std::make_tuple(std::get<0>(fns[i]), addrs[i]));
    return w;
}

py::list<std::optional<int64_t>> _align(const py::bytes& ds, const py::list<py::bytes>& lits, int64_t lo, int64_t hi) {
    int64_t n = py::len(lits);
    std::map<std::pair<int64_t, int64_t>, bool> memo;
    // fits(i, pos): from literal i on, each can be found after the one before (the first occurrence at or after pos)
    auto fits = [&](int64_t i, int64_t pos) {
        std::vector<std::pair<int64_t, int64_t>> chain;
        bool result;
        while (true) {
            if (i == n) {
                result = true;
                break;
            }
            auto key = std::make_pair(i, pos);
            auto it = memo.find(key);
            if (it != memo.end()) {
                result = it->second;
                break;
            }
            chain.push_back(key);
            int64_t at = ds.find(lits[i], pos, hi);
            if (at < 0) {
                result = false;
                break;
            }
            pos = at + py::len(lits[i]);
            i++;
        }
        for (const auto& key : chain) memo[key] = result;
        return result;
    };
    py::list<std::optional<int64_t>> out;
    int64_t pos = lo;
    for (int64_t i = 0; i < n; i++) {
        const py::bytes& s = lits[i];
        std::optional<int64_t> best;
        int64_t at = ds.find(s, pos, hi);
        while (at >= 0) {
            if (fits(i + 1, at + py::len(s))) best = at;
            at = ds.find(s, at + 1, hi);
        }
        if (!best.has_value()) {
            out.append(std::nullopt);
            continue;
        }
        out.append(best);
        pos = *best + py::len(s);
    }
    return out;
}

namespace {

// (name, type, declarator) of each datum a file's extern lines declare
using FileExterns = std::vector<std::tuple<str, str, str>>;

FileExterns parse_externs(const str& p) {
    FileExterns out;
    for (const str& l0 : lines_latin1(p)) {
        if (!l0.startswith(str("extern"))) continue;
        str l = rx(R"re(/\*.*?\*/)re").sub(str(""), l0).strip().rstrip(str(";"));
        std::optional<re::Match> m = rx(R"re(extern\s+(unsigned\s+\w+|unsigned|struct\s+\w+|union\s+\w+|\w+)\s+(.*))re").match(l);
        // (a function; a pointer to one is data)
        if (!m || (m->group(2).contains(str("(")) && !m->group(2).replace(str(" "), str("")).contains(str("(*")))) continue;
        for (const str& it : py::split(m->group(2), str(","))) {
            std::optional<re::Match> n = rx(R"re(\b(\w+)\s*(?:\[|$))re").search(it.strip().lstrip(str("*")));
            if (n) out.emplace_back(n->group(1), m->group(1).strip(), it.strip());
        }
    }
    return out;
}

// What a file said when it was last read: kept while its size and time of writing are what they were (a draft asks
// for the same function files again and again; reading and parsing them is its cost).
struct Parsed {
    std::filesystem::file_time_type time;
    std::uintmax_t size = 0;
    FileExterns entries;
};

// (known: the time and size the folder's listing gave, so that asking for them costs no call of its own)
const FileExterns& externs_of(const str& p, const std::optional<std::pair<std::filesystem::file_time_type, std::uintmax_t>>& known = std::nullopt) {
    static std::map<std::u32string, Parsed> memo;
    std::error_code ec1, ec2;
    std::filesystem::path fp = std::filesystem::u8path(p.utf8());
    std::filesystem::file_time_type time;
    std::uintmax_t size = 0;
    if (known) {
        time = known->first;
        size = known->second;
    } else {
        time = std::filesystem::last_write_time(fp, ec1);
        size = std::filesystem::file_size(fp, ec2);
    }
    if (ec1 || ec2) {                                        // (not there: the open says so, as Python's does)
        static FileExterns none;
        parse_externs(p);
        return none;
    }
    auto it = memo.find(p.u32());
    if (it != memo.end() && it->second.time == time && it->second.size == size) return it->second.entries;
    Parsed parsed{time, size, parse_externs(p)};
    return (memo[p.u32()] = std::move(parsed)).entries;
}

}  // namespace

Declarations externs(const str& src, const std::optional<py::list<str>>& files) {
    Declarations decl;
    auto add = [&](const FileExterns& entries) {
        for (const auto& e : entries) {
            const str& key = std::get<0>(e);
            if (!decl.contains(key)) decl[key] = py::set<std::tuple<str, str>>();
            decl.at(key).add(std::make_tuple(std::get<1>(e), std::get<2>(e)));
        }
    };
    if (files.has_value()) {
        for (const str& p : *files) add(externs_of(p));
        return decl;
    }
    // glob.glob(join(src, "f*.c")): the folder's own order, the names matched without regard to case
    std::error_code ec;
    auto it = std::filesystem::directory_iterator(std::filesystem::u8path(src.utf8()), ec);
    if (ec) return decl;                                      // (glob.glob of a folder that is not there: nothing)
    for (const auto& e : it) {
        str name(e.path().filename().u8string());
        str low = name.lower();
        if (low.size() >= 3 && low.startswith(str("f")) && low.endswith(str(".c"))) {
            std::error_code e1, e2;
            auto time = e.last_write_time(e1);
            auto size = e.is_regular_file(e2) ? e.file_size(e2) : std::uintmax_t(0);
            if (e1 || e2) add(externs_of(py::os::path::join(src, name)));
            else add(externs_of(py::os::path::join(src, name), std::make_pair(time, size)));
        }
    }
    return decl;
}

py::dict<int64_t, str>& CODE() {
    static py::dict<int64_t, str> d;
    return d;
}
py::dict<str, str>& FUNCS() {
    static py::dict<str, str> d;
    return d;
}
py::dict<str, str>& HOME() {
    static py::dict<str, str> d;
    return d;
}

py::dict<int64_t, str> _code_names(const modules::Project& prj) {
    py::dict<int64_t, str> out;
    modules::Symbols syms = prj.symbols();
    for (const auto& kv : syms.items()) {
        const str& n = std::get<0>(kv);
        const str& t = std::get<0>(std::get<1>(kv));
        int64_t a = std::get<1>(std::get<1>(kv));
        if (t == "C" && !n.contains(str(":"))) {
            int64_t key = a & 0xFFFF;
            if (!out.contains(key)) out[key] = n.lstrip(str("_"));
        }
    }
    for (const str& f : glob_names(prj.src(), "f", ".c")) {
        std::optional<re::Match> m = rx(R"re(f([0-9a-f]{5})\.c)re").fullmatch(py::os::path::basename(f));
        if (m) {
            int64_t key = py::int_parse(m->group(1), 16) & 0xFFFF;
            if (!out.contains(key)) out[key] = str("f") + m->group(1);
        }
    }
    return out;
}

const py::dict<str, int64_t> SIZES = {{str("char"), 1}, {str("unsigned char"), 1}, {str("int"), 2},  {str("unsigned"), 2},
                                      {str("unsigned int"), 2}, {str("short"), 2},  {str("unsigned short"), 2}, {str("long"), 4},
                                      {str("unsigned long"), 4}, {str("float"), 4}, {str("double"), 8}};

std::optional<int64_t> _declared_size(const py::set<std::tuple<str, str>>& ty, const Structs& structs) {
    std::vector<std::tuple<str, str>> items;
    for (const auto& t : ty) items.push_back(t);
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a).u32() != std::get<0>(b).u32()) return std::get<0>(a).u32() < std::get<0>(b).u32();
        return std::get<1>(a).u32() < std::get<1>(b).u32();
    });
    for (const auto& ti : items) {
        const str& t = std::get<0>(ti);
        const str& i = std::get<1>(ti);
        if (i.contains(str("(")) && !i.contains(str("(*"))) continue;
        int64_t stars = py::split(i, str("["))[0].contains(str("*")) ? 1 : 0;
        py::list<int64_t> dims;
        for (const re::Match& m : rx(R"re(\[(\d+)\])re").finditer(i)) dims.append(py::int_parse(m.group(1)));
        if (i.replace(str(" "), str("")).contains(str("[]"))) continue;      // (an array of no count: its run says)
        return std::get<0>(_sizeof(t, stars, dims, structs, 2));
    }
    return std::nullopt;
}

Structs parse_structs(const modules::Project& prj) {
    std::vector<std::tuple<str, py::bytes>> texts;
    {
        py::dict<str, py::bytes> hs = prj.headers();
        for (const auto& kv : hs.items()) texts.emplace_back(std::get<0>(kv), std::get<1>(kv));
    }
    for (const str& d : prj.include()) {
        for (const str& f : glob_names(d, "", ".h")) {
            py::file fh = py::open_rb(f);
            py::bytes b = fh.read();
            fh.close();
            texts.emplace_back(py::os::path::basename(f), b);
        }
    }
    Structs out;
    HOME().clear();
    // What a header's text says: the structs it defines, in order.  A text is parsed once (a draft asks for the same
    // headers again and again); one that fails (a dimension like 010 is int()'s ValueError) is parsed again each time,
    // and the structs before the failure are put in first, as the Python has them when it raises.
    static std::map<std::string, std::vector<std::pair<str, py::list<Field>>>> memo;
    for (const auto& ht : texts) {
        const str& hname = std::get<0>(ht);
        auto apply = [&](const std::vector<std::pair<str, py::list<Field>>>& structs) {
            for (const auto& s : structs) {
                out[s.first] = s.second;
                if (!HOME().contains(s.first)) HOME()[s.first] = hname;
            }
        };
        auto cached = memo.find(std::get<1>(ht).raw());
        if (cached != memo.end()) {
            apply(cached->second);
            continue;
        }
        std::vector<std::pair<str, py::list<Field>>> found;
        try {
            str t = rx(R"re(/\*.*?\*/)re", 16).sub(str(" "), std::get<1>(ht).decode("latin1"));
            for (const re::Match& m : rx(R"re(struct\s+(\w+)\s*\{([^{}]*)\}\s*;)re").finditer(t)) {
                py::list<Field> fields;
                for (const str& decl0 : py::split(m.group(2), str(";"))) {
                    str decl = py::join(str(" "), py::split(decl0));
                    if (decl.empty()) continue;
                    std::optional<re::Match> dm = rx(R"re(((?:unsigned\s+)?(?:struct\s+\w+|union\s+\w+|\w+))\s+(.*))re").match(decl);
                    if (!dm) continue;
                    for (const str& it0 : py::split(dm->group(2), str(","))) {
                        str it = it0.strip();
                        int64_t stars = py::len(it) - py::len(it.lstrip(str("*")));
                        std::optional<re::Match> nm = rx(R"re(\**\s*(\w+))re").match(it);
                        py::list<int64_t> dims;
                        for (const re::Match& dmm : rx(R"re(\[(\w+)\])re").finditer(it)) {
                            str x = dmm.group(1);
                            if (re::fullmatch(str(R"re(\d+|0x[0-9a-fA-F]+)re"), x)) dims.append(py::int_parse(x, 0));
                        }
                        if (nm) fields.append(std::make_tuple(dm->group(1), stars, dims, nm->group(1)));
                    }
                }
                found.emplace_back(m.group(1), fields);
            }
        } catch (...) {
            apply(found);
            throw;
        }
        apply(found);
        memo.emplace(std::get<1>(ht).raw(), std::move(found));
    }
    return out;
}

std::tuple<int64_t, int64_t> _sizeof(const str& base, int64_t stars, const py::list<int64_t>& dims, const Structs& structs, int64_t align) {
    int64_t n, al;
    if (stars) {
        n = 2;
        al = 2;
    } else if (base.startswith(str("struct "))) {
        std::tie(n, al) = _struct_size(base.slice_from(7), structs, align);
    } else {
        n = SIZES.get(base, 2);
        al = std::min(n, align);
    }
    for (int64_t d : dims) n *= d;
    return {n, al};
}

std::tuple<int64_t, int64_t> _struct_size(const str& name, const Structs& structs, int64_t align) {
    Depth depth;
    int64_t pos = 0, top = 1;
    py::list<Field> fields = structs.get(name, py::list<Field>());
    for (const Field& f : fields) {
        auto [n, al] = _sizeof(std::get<0>(f), std::get<1>(f), std::get<2>(f), structs, align);
        pos += py::mod(-pos, al);
        pos += n;
        top = std::max(top, al);
    }
    return {pos + py::mod(-pos, top), top};
}

namespace {

bool any_ends_with_bracket(const py::set<std::tuple<str, str>>& ty) {
    for (const auto& ti : ty)
        if (std::get<1>(ti).endswith(str("]"))) return true;
    return false;
}

str word_x(const char* fmt, int64_t v) { return py::percent(str(fmt), v); }

// (v ^ 0x8000) - 0x8000
int64_t signed16(int64_t v) { return (v ^ 0x8000) - 0x8000; }

str join_strs(const str& sep, const std::vector<str>& v) {
    std::u32string o;
    for (size_t i = 0; i < v.size(); i++) {
        if (i) o += sep.u32();
        o += v[i].u32();
    }
    return str(o);
}

}  // namespace

str _definition(const py::bytes& ds, int64_t a, int64_t nx, const py::dict<int64_t, str>& syms, const Declarations& decl,
                const py::dict<int64_t, py::bytes>& strs, const std::optional<Structs>& structs, int64_t align,
                const std::optional<py::set<str>>& local) {
    auto W = [&](int64_t o) { return py::unpack_from1<int64_t>("<H", ds, o); };
    int64_t n = nx - a;
    str nm = syms.get(a, str("g_") + word_x("%04x", a));
    py::set<std::tuple<str, str>> ty = decl.get(nm, py::set<std::tuple<str, str>>());
    std::vector<int64_t> words;
    for (int64_t q = 0; q < py::floordiv(n, 2); q++) words.push_back(W(a + 2 * q));

    ValFn val = [&](int64_t v) -> std::optional<str> {
        if (strs.contains(v)) return cstr(strs.at(v));
        if (syms.contains(v) && v != a) {
            str s = syms.at(v);
            return str(!any_ends_with_bracket(decl.get(s, py::set<std::tuple<str, str>>())) ? "&" : "") + s;
        }
        return std::nullopt;
    };
    std::vector<std::tuple<str, str>> fp;
    for (const auto& ti : ty) {
        const str& i = std::get<1>(ti);
        if (i.contains(str("(*")) && i.replace(str(" "), str("")).contains(str(")("))) fp.push_back(ti);
    }
    if (!fp.empty() && n % 2 == 0) {                        // (a table of functions: their names)
        std::sort(fp.begin(), fp.end(), [](const auto& x, const auto& y) {
            if (std::get<0>(x).u32() != std::get<0>(y).u32()) return std::get<0>(x).u32() < std::get<0>(y).u32();
            return std::get<1>(x).u32() < std::get<1>(y).u32();
        });
        str t = std::get<0>(fp[0]);
        str i = std::get<1>(fp[0]);
        std::vector<str> vals;
        for (int64_t v : words) {
            if (CODE().contains(v) && !CODE().at(v).empty()) {
                vals.push_back(CODE().at(v));
            } else {
                vals.push_back(v == 0 ? str("0") : py::percent(str("(%s (*)())0x%04x /* ? */"), t, v));
            }
        }
        for (const str& v : vals)
            if (!(v.startswith(str("0")) || v.startswith(str("(")))) FUNCS()[v] = t;
        str decl_ = i.contains(str("[")) ? rx(R"re(\[\s*\w*\s*\])re").sub(py::percent(str("[%d]"), py::floordiv(n, 2)), i.replace(nm, nm)) : i;
        if (i.contains(str("["))) return py::percent(str("%s %s = { %s };"), t, decl_, join_strs(str(", "), vals));
        if (vals.empty()) throw py::IndexError("list index out of range");
        return py::percent(str("%s %s = %s;"), t, i, vals[0]);
    }
    bool isarr = any_ends_with_bracket(ty);
    bool isptr = false;
    for (const auto& ti : ty)
        if (std::get<1>(ti).contains(str("*"))) isptr = true;
    std::vector<str> kinds;
    for (const auto& ti : ty) kinds.push_back(std::get<0>(ti));
    kinds = sorted_strs(kinds);
    std::optional<str> base;
    for (const str& k : kinds)
        if (k.startswith(str("struct "))) {
            base = k;
            break;
        }
    if (!base.has_value() && !kinds.empty()) base = kinds[0];
    int64_t ptrs = 0;
    for (int64_t v : words)
        if (val(v).has_value()) ptrs++;
    auto base_in = [&](std::initializer_list<const char*> xs) {
        if (!base.has_value()) return false;
        for (const char* x : xs)
            if (*base == x) return true;
        return false;
    };
    if (!ty.empty() && local.has_value() && !local->contains(nm) && ptrs >= 2 && (base_in({"char", "int", "unsigned"}) || (!isarr && !isptr))) {
        ty = py::set<std::tuple<str, str>>();                // (another module's type, and the bytes are a table of places: the bytes say what it is)
        isarr = false;
        isptr = false;
        base = std::nullopt;
    }
    bool has_paren = false;
    for (const auto& ti : ty)
        if (std::get<1>(ti).contains(str("("))) has_paren = true;
    std::optional<int64_t> k;
    if (isptr && !isarr && !has_paren) {
        k = 2;
    } else if (!isarr && !isptr) {
        if (base.has_value() && SIZES.contains(*base)) k = SIZES.at(*base);
    }
    if (!ty.empty() && k.has_value() && *k != 0 && *k < n && n - *k >= 2)           // (a variable, and after it one nothing names)
        return _definition(ds, a, a + *k, syms, decl, strs, structs, align, local) + str("\n") +
               _definition(ds, a + *k, nx, syms, decl, strs, structs, align, local);
    if (!ty.empty() && k.has_value() && *k == 1 && base_in({"char", "unsigned char"}) && n <= 2 && (n == 1 || ds[a + 1] == 0)) {
        int64_t c = ds[a];                                   // (a char: the byte after it the next word's pad)
        str ch = (c > 32 && c < 127 && c != '\'' && c != '\\') ? str("'") + str(std::u32string(1, static_cast<char32_t>(c))) + str("'")
                                                               : str(std::to_string(c));
        return py::percent(str("%s %s = %s;%s"), *base, nm, ch, str(""));
    }
    str note = (ty.empty() && !syms.contains(a)) ? str("                 /* (nothing names it) */") : str("");
    if (structs.has_value() && !structs->empty() && base.has_value() && !base->empty() && base->startswith(str("struct ")) && structs->contains(base->slice_from(7)) && !isptr) {
        auto [size, al] = _struct_size(base->slice_from(7), *structs, align);
        (void)al;
        if (size != 0 && n % size == 0) {
            std::vector<str> elems;
            for (int64_t q = 0; q < py::floordiv(n, size); q++) elems.push_back(_struct_value(ds, a + q * size, base->slice_from(7), *structs, align, val));
            if (isarr || elems.size() > 1)
                return py::percent(str("%s %s[] = {\n    %s,\n};"), *base, nm, join_strs(str(",\n    "), elems));
            return py::percent(str("%s %s = %s;"), *base, nm, (elems.empty() ? throw py::IndexError("list index out of range") : elems[0]));
        }
    }
    if (base.has_value() && !base->empty() && !base_in({"char", "int", "unsigned", "long"}) && isptr) {
        str cast = py::percent(str("(%s *)"), *base);
        std::vector<str> vals;
        for (int64_t v : words) {
            std::optional<str> x = val(v);
            vals.push_back(x.has_value() ? *x : (v == 0 ? str("0") : py::percent(str("(%s *)0x%04x /* ? */"), *base, v)));
        }
        for (str& x : vals)
            if (x.startswith(str("&"))) x = cast + x;
        if (isarr) {
            bool anyw = false;
            for (int64_t v : words)
                if (v) anyw = true;
            return py::percent(str("%s *%s[%d] = { %s };"), *base, nm, py::floordiv(n, 2), anyw ? join_strs(str(", "), vals) : str("0"));
        }
        if (vals.empty()) throw py::IndexError("list index out of range");
        return py::percent(str("%s *%s = %s;"), *base, nm, vals[0]);
    }
    if (base.has_value() && *base == "char" && isarr && !isptr) {
        py::bytes body = ds.slice(a, nx);
        py::bytes s = body.rstrip(py::bytes(std::string(1, '\0')));
        str inner;
        if (!s.empty() && !s.contains(py::bytes(std::string(1, '\0')))) {
            inner = cstr(s);
        } else if (!s.empty()) {
            std::vector<str> parts;
            for (int64_t c : body) parts.push_back(str(std::to_string(c)));
            inner = str("{ ") + join_strs(str(", "), parts) + str(" }");
        } else {
            inner = str("\"\"");
        }
        return py::percent(str("char %s[%d] = %s;%s"), nm, n, inner, note);
    }
    if (n == 2) {
        int64_t v = words[0];
        std::optional<str> pv = val(v);
        if (pv.has_value() && pv->startswith(str("\"@(#)")) && !syms.contains(a)) return py::percent(str("static char *sccsid = %s;"), *pv);
        if (pv.has_value() && pv->startswith(str("\""))) return py::percent(str("char *%s = %s;%s"), nm, *pv, note);
        if (pv.has_value()) return py::percent(str("char *%s = (char *)%s;%s"), nm, *pv, note);
        if (isptr)
            return py::percent(str("char *%s = %s;%s"), nm, v == 0 ? str("0") : py::percent(str("(char *)0x%04x /* ? */"), v), note);
        str tname = base_in({"int", "unsigned"}) ? *base : str("int");
        bool is_unsigned = base.has_value() && *base == "unsigned";
        return py::percent(str("%s %s = %d;%s"), tname, nm, is_unsigned ? v : signed16(v), note);
    }
    if (n == 4 && base.has_value() && *base == "long")
        return py::percent(str("long %s = %dL;%s"), nm, py::unpack_from1<int64_t>("<i", ds, a), note);
    bool anyval = false;
    for (int64_t v : words)
        if (val(v).has_value()) anyval = true;
    if (n % 2 == 0 && (isptr || anyval)) {
        std::vector<str> vs;
        for (int64_t v : words) {
            std::optional<str> x = val(v);
            str s = x.has_value() ? *x : (v == 0 ? str("0") : py::percent(str("(char *)0x%04x /* ? */"), v));
            vs.push_back(s.startswith(str("&")) ? str("(char *)") + s : s);
        }
        return py::percent(str("char *%s[%d] = { %s };%s"), nm, py::floordiv(n, 2), join_strs(str(", "), vs), note);
    }
    if (n % 2 == 0) {
        std::vector<str> vs;
        for (int64_t v : words) vs.push_back(str(std::to_string(signed16(v))));
        return py::percent(str("int %s[%d] = { %s };%s"), nm, py::floordiv(n, 2), join_strs(str(", "), vs), note);
    }
    std::vector<str> vs;
    for (int64_t c : ds.slice(a, nx)) vs.push_back(str(std::to_string(c)));
    return py::percent(str("char %s[%d] = { %s };%s"), nm, n, join_strs(str(", "), vs), note);
}

str _struct_value(const py::bytes& ds, int64_t at, const str& name, const Structs& structs, int64_t align, const ValFn& val) {
    Depth depth;
    auto W = [&](int64_t o) { return py::unpack_from1<int64_t>("<H", ds, o); };
    std::vector<str> parts;
    int64_t pos = 0;
    for (const Field& f : structs.at(name)) {
        const str& base = std::get<0>(f);
        int64_t stars = std::get<1>(f);
        const py::list<int64_t>& dims = std::get<2>(f);
        auto [n, al] = _sizeof(base, stars, dims, structs, align);
        pos += py::mod(-pos, al);
        int64_t p = at + pos;
        bool nodims = dims.empty();
        int64_t bsize = SIZES.get(base, 2);
        if (stars) {
            int64_t v = W(p);
            std::optional<str> x = val(v);
            str star(std::u32string(static_cast<size_t>(stars), U'*'));
            str xs;
            if (!x.has_value()) {
                xs = v == 0 ? str("0") : py::percent(str("(%s %s)0x%04x /* ? */"), base, star, v);
            } else if (x->startswith(str("&")) || !x->startswith(str("\""))) {
                xs = py::percent(str("(%s %s)%s"), base, star, *x);
            } else {
                xs = *x;
            }
            parts.push_back(xs);
        } else if (base.startswith(str("struct ")) && nodims) {
            parts.push_back(_struct_value(ds, p, base.slice_from(7), structs, align, val));
        } else if ((base == "char" || base == "unsigned char") && !nodims) {
            py::bytes body = ds.slice(p, p + n);
            py::bytes s = body.rstrip(py::bytes(std::string(1, '\0')));
            if (!s.contains(py::bytes(std::string(1, '\0')))) {
                parts.push_back(cstr(s));
            } else {
                std::vector<str> bs;
                for (int64_t c : body) bs.push_back(str(std::to_string(c)));
                parts.push_back(str("{ ") + join_strs(str(", "), bs) + str(" }"));
            }
        } else if (bsize == 1 && nodims) {
            int64_t c = ds[p];
            if (c > 32 && c < 127 && c != '\'' && c != '\\')
                parts.push_back(str("'") + str(std::u32string(1, static_cast<char32_t>(c))) + str("'"));
            else
                parts.push_back(c > 9 ? py::percent(str("0x%02x"), c) : str(std::to_string(c)));
        } else if (bsize == 4 && nodims) {
            int64_t v = base.startswith(str("unsigned")) ? py::unpack_from1<int64_t>("<I", ds, p) : py::unpack_from1<int64_t>("<i", ds, p);
            parts.push_back(py::percent(str("%dL"), v));
        } else if (nodims) {
            int64_t v = W(p);
            parts.push_back(str(std::to_string(base.startswith(str("unsigned")) ? v : signed16(v))));
        } else {
            int64_t k = bsize;
            std::vector<str> vs;
            for (int64_t j = 0; j < n; j += k) vs.push_back(str(std::to_string(from_bytes_le(ds.slice(p + j, p + j + k)))));
            parts.push_back(str("{ ") + join_strs(str(", "), vs) + str(" }"));
        }
        pos += n;
    }
    while (parts.size() > 1 && (parts.back() == "0" || parts.back() == "0L")) parts.pop_back();   // (the rest of an element left to the compiler's zeros)
    return str("{ ") + join_strs(str(", "), parts) + str(" }");
}

py::dict<str, str> draft(const Project& prj, const modules::Row& row) {
    str rule = prj.data_order();
    if (rule != "after_next_function") throw py::SystemExit(py::percent(str("no data rule %r yet"), rule));
    Walked w = walk(prj, row);
    const py::list<Run>& rs = w.runs;
    int64_t dstart = w.dstart, dend = w.dend;
    const py::list<std::tuple<str, int64_t>>& fnaddr = w.functions;
    py::bytes ds = prj.image().slice_from(prj.dgroup());
    auto W = [&](int64_t o) { return py::unpack_from1<int64_t>("<H", ds, o); };
    py::dict<int64_t, str> syms;
    {
        modules::Symbols all = prj.symbols();
        for (const auto& kv : all.items()) {
            const str& n = std::get<0>(kv);
            const str& t = std::get<0>(std::get<1>(kv));
            int64_t a = std::get<1>(std::get<1>(kv));
            // (not a function's)
            if (t == "D" && !n.contains(str(":")) && n.startswith(str("_")) && !py::os::path::exists(prj.path(n.slice_from(1) + str(".c"))))
                if (!syms.contains(a)) syms[a] = n.slice_from(1);
        }
    }
    Declarations decl;
    decl = externs(prj.src());
    py::list<str> mine;
    if (std::get<1>(row).has_value()) {
        for (const str& q : modules::files_of(prj.src(), *std::get<1>(row), *std::get<2>(row)))
            if (py::os::path::basename(q).startswith(str("f"))) mine.append(q);
    }
    py::set<str> local;
    if (!mine.empty())
        for (const str& k : externs(prj.src(), mine).keys()) local.add(k);
    for (const str& nm : decl.keys()) {                      // (the engine's names of places: g_ADDR)
        std::optional<re::Match> m = re::fullmatch(str("g_([0-9a-f]{4})"), nm);
        if (m) {
            int64_t addr = py::int_parse(m->group(1), 16);
            if (dstart <= addr && addr < dend && !syms.contains(addr)) syms[addr] = nm;
        }
    }
    py::dict<int64_t, py::bytes> strs;
    {
        int64_t k = dstart;
        py::bytes nul(std::string(1, '\0'));
        while (k < dend) {
            if ((ds[k] && ds[k] >= 32 && ds[k] < 127) || ds[k] == 9 || ds[k] == 10) {
                int64_t e = ds.index(nul, k);
                bool ok = true;
                for (int64_t c : ds.slice(k, e))
                    if (!((c >= 32 && c < 127) || c == 9 || c == 10 || c == 13)) ok = false;
                if (ok) {
                    strs[k] = ds.slice(k, e);
                    k = e + 1;
                    continue;
                }
            }
            k += 1;
        }
    }
    py::set<int64_t> targets;
    for (int64_t q = dstart; q < dend - 1; q++) targets.add(W(q));
    {
        std::vector<int64_t> tv;
        for (int64_t v : targets) tv.push_back(v);
        std::set<int64_t> ends;                              // {s + len(t) for s, t in strs.items()}
        for (const auto& kv : strs.items()) ends.insert(std::get<0>(kv) + py::len(std::get<1>(kv)));
        for (int64_t v : tv) {                               // (a 0 pointed at, no variable's place: an empty string)
            if (dstart <= v && v < dend && ds[v] == 0 && !syms.contains(v) && !strs.contains(v)) {
                if (v == dstart || ds[v - 1] == 0 || ends.count(v - 1)) {
                    strs[v] = py::bytes();
                    ends.insert(v);
                }
            }
        }
    }
    Structs structs;
    structs = parse_structs(prj);
    std::vector<str> order;
    py::dict<str, str> file_of;
    for (const auto& na : fnaddr) {
        order.push_back(std::get<0>(na).lstrip(str("_")));
        file_of[std::get<0>(na).lstrip(str("_"))] = py::percent(str("f%05x"), std::get<1>(na));
    }
    // (an OrderedDict: file name -> its definitions)
    std::vector<str> file_order;
    std::map<std::u32string, std::vector<str>> files;
    auto lines_of = [&](const str& key) -> std::vector<str>& {
        if (!files.count(key.u32())) file_order.push_back(key);
        return files[key.u32()];
    };
    std::vector<std::tuple<str, int64_t, int64_t>> varruns;
    std::vector<std::pair<int64_t, int64_t>> spans;          // (s, s + len(strs[s]) + 1): strs does not change while the runs are walked
    for (const auto& kv : strs.items()) spans.emplace_back(std::get<0>(kv), std::get<0>(kv) + py::len(std::get<1>(kv)) + 1);
    for (const Run& r : rs) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        const str& after = std::get<2>(r);
        const str& kind = std::get<3>(r);
        if (kind == "strings") continue;
        std::optional<str> fn;
        if (after == "(start)") {
            int64_t q = lo;                                  // (the file's start: its initializers' strings first)
            while (q < hi) {                                 // (a string nothing points at is a char array of its own)
                if (strs.contains(q) && targets.contains(q)) {
                    q += py::len(strs.at(q)) + 1;
                } else if (ds[q] == 0 && (q & 1)) {
                    q += 1;
                } else {
                    break;
                }
            }
            lo = q;
            if (!order.empty()) fn = order[0];
        } else {
            int64_t q = hi;                                  // (after a function's literals: variables, then the
            while (true) {                                   //  strings of the initializers before the next one)
                std::optional<int64_t> cand;
                for (const auto& se : spans) {
                    int64_t s = se.first, end = se.second;
                    if (lo <= s && s < q && (end == q || end == q - 1) && targets.contains(s))
                        if (!cand.has_value() || s > *cand) cand = s;
                }
                if (!cand.has_value()) break;
                q = *cand;
            }
            hi = q;
            fn = after.lstrip(str("_"));                     // (its variables were declared before that function)
        }
        if (lo >= hi) continue;
        str key;
        if (fn.has_value() && file_of.contains(*fn)) {
            key = str("d") + file_of.at(*fn).slice_from(1) + str(".c");
        } else if (!std::get<1>(row).has_value()) {
            key = std::get<0>(py::os::path::splitext(std::get<0>(row))) + str("-data.c");
        } else {
            if (fnaddr.empty()) throw py::IndexError("list index out of range");
            key = py::percent(str("e%05x.c"), std::get<1>(fnaddr[-1]));
        }
        int64_t lo2 = ((lo & 1) && ds[lo] == 0) ? lo + 1 : lo;
        varruns.emplace_back(key, lo2, hi);
    }
    // inside the variables' runs nothing is a literal: what looks like a string there is a variable's bytes, and a
    // word that points into a run points at a variable (one of its own, named after its place)
    for (const auto& vr : varruns) {
        int64_t lo2 = std::get<1>(vr), hi = std::get<2>(vr);
        std::vector<int64_t> gone;
        for (int64_t s : strs.keys())
            if (lo2 <= s && s < hi) gone.push_back(s);
        for (int64_t s : gone) strs.pop(s);
    }
    for (const auto& vr : varruns) {
        int64_t lo2 = std::get<1>(vr), hi = std::get<2>(vr);
        for (int64_t t : targets)
            if (lo2 < t && t < hi && !syms.contains(t) && t % 2 == 0) syms[t] = str("g_") + word_x("%04x", t);
    }
    CODE().clear();
    for (const auto& kv : _code_names(prj).items()) CODE()[std::get<0>(kv)] = std::get<1>(kv);
    FUNCS().clear();
    for (const auto& vr : varruns) {
        const str& key = std::get<0>(vr);
        int64_t lo2 = std::get<1>(vr), hi = std::get<2>(vr);
        std::vector<str>& lines = lines_of(key);
        std::vector<int64_t> places{lo2};
        for (int64_t a : syms.keys())
            if (lo2 <= a && a < hi) places.push_back(a);
        std::sort(places.begin(), places.end());
        places.erase(std::unique(places.begin(), places.end()), places.end());
        std::vector<std::tuple<int64_t, int64_t>> keep;
        for (int64_t a : places) {                           // (a place inside a variable of a declared size is part of it)
            if (!keep.empty() && a < std::get<1>(keep.back())) continue;
            std::optional<int64_t> size = _declared_size(decl.get(syms.get(a, str("")), py::set<std::tuple<str, str>>()), structs);
            keep.emplace_back(a, a + size.value_or(0));
        }
        std::vector<int64_t> pl;
        for (const auto& ae : keep) pl.push_back(std::get<0>(ae));
        for (size_t i = 0; i < pl.size(); i++) {
            int64_t nx = i + 1 < pl.size() ? pl[i + 1] : hi;
            lines.push_back(_definition(ds, pl[i], nx, syms, decl, strs, structs, 2, local));
        }
    }
    py::set<str> defined;
    for (const str& key : file_order) {
        str all = join_strs(str("\n"), files[key.u32()]);
        for (const re::Match& m : rx(R"re(\b(\w+)(?=(?:\[\d*\])?\s*=))re").finditer(all)) defined.add(m.group(1));
    }
    py::set<str> known;
    for (const str& v : syms.values()) known.add(v);
    py::dict<str, str> result;
    for (const str& key : file_order) {
        std::vector<str>& lines = files[key.u32()];
        py::set<str> used;
        for (const str& l : lines) {
            str body = l.contains(str("=")) ? l.slice_from(l.find(str("=")) + 1) : str("");
            str scrubbed = rx(R"re("(?:[^"\\]|\\.)*")re").sub(str(""), body);
            for (const re::Match& m : rx(R"re(\b(\w+)\b)re").finditer(scrubbed))
                if (known.contains(m.group(1))) used.add(m.group(1));
        }
        std::vector<str> refs;
        for (const str& u : sorted_strs(used))
            if (!defined.contains(u)) refs.push_back(u);
        std::vector<str> head;
        // lines[:0] = [...] puts the includes first, then the functions, then the variables
        std::vector<str> externs_vars;
        for (const str& r : refs) {
            std::tuple<str, str> first;
            if (decl.contains(r)) {
                std::vector<std::tuple<str, str>> items;
                for (const auto& ti : decl.at(r)) items.push_back(ti);
                std::sort(items.begin(), items.end(), [](const auto& x, const auto& y) {
                    if (std::get<0>(x).u32() != std::get<0>(y).u32()) return std::get<0>(x).u32() < std::get<0>(y).u32();
                    return std::get<1>(x).u32() < std::get<1>(y).u32();
                });
                first = items[0];
            } else {
                first = std::make_tuple(str("int"), r);
            }
            externs_vars.push_back(py::percent(str("extern %s %s;"), std::get<0>(first), std::get<1>(first)));
        }
        lines.insert(lines.begin(), externs_vars.begin(), externs_vars.end());
        py::set<str> named_set;
        for (const str& l : lines)
            for (const re::Match& m : rx(R"re(\b(\w+)\b)re").finditer(l))
                if (FUNCS().contains(m.group(1))) named_set.add(m.group(1));
        std::vector<str> externs_fns;
        for (const str& f : sorted_strs(named_set)) externs_fns.push_back(py::percent(str("extern %s %s();"), FUNCS().at(f), f));
        lines.insert(lines.begin(), externs_fns.begin(), externs_fns.end());
        std::vector<str> hs;
        str joined = join_strs(str("\n"), lines);
        for (const re::Match& m : rx(R"re(\bstruct\s+(\w+))re").finditer(joined)) {
            str s = m.group(1);
            if (HOME().contains(s) && std::find(hs.begin(), hs.end(), HOME().at(s)) == hs.end() && mine.empty()) hs.push_back(HOME().at(s));   // (a module with functions sees them already)
        }
        std::vector<str> incs;
        for (const str& h : hs) incs.push_back(py::percent(str("#include \"%s\""), h));
        lines.insert(lines.begin(), incs.begin(), incs.end());
        result[key] = join_strs(str("\n"), lines) + str("\n");
    }
    return result;
}

int64_t main(const Project& prj, const py::list<str>& argv) {
    py::list<str> want;
    for (const str& a : argv)
        if (!a.startswith(str("-"))) want.append(a);
    auto has_flag = [&](const char* f) { return argv.contains(str(f)); };
    for (const modules::Row& row : modules::rows(prj)) {
        const str& name = std::get<0>(row);
        if (!want.empty() && !want.contains(name) && !want.contains(std::get<0>(py::os::path::splitext(name)))) continue;
        if (name.upper().endswith(str(".OBJ"))) continue;
        if (has_flag("--draft") || has_flag("--write")) {
            for (const auto& kv : draft(prj, row).items()) {
                const str& fn = std::get<0>(kv);
                const str& text = std::get<1>(kv);
                py::print(str("== ") + fn);
                py::print(text);
                str p = py::os::path::join(prj.src(), fn);
                if (has_flag("--write")) {
                    if (py::os::path::exists(p)) {
                        py::print(str("   (exists: not written)"));
                    } else {
                        std::ofstream f(std::filesystem::u8path(p.utf8()), std::ios::binary);
                        f << text.utf8();
                    }
                }
            }
            continue;
        }
        Walked w = walk(prj, row);
        py::print(py::percent(str("%s: data %04x..%04x, %d literals of its functions"), name, w.dstart, w.dend, py::len(w.strings)));
        for (const Run& r : w.runs)
            py::print(py::percent(str("   %04x..%04x %4d bytes after %-10s %s"), std::get<0>(r), std::get<1>(r), std::get<1>(r) - std::get<0>(r),
                                  std::get<2>(r), std::get<3>(r)));
    }
    return 0;
}

}  // namespace codebreaker::exact::data
