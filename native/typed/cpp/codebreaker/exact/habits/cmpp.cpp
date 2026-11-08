// codebreaker/exact/habits/cmpp.cpp -- C++ from codebreaker.exact.habits.cmpp (see cmpp.hpp).

#include "codebreaker/exact/habits/cmpp.hpp"

#include <cctype>
#include <memory>
#include <utility>
#include <vector>

namespace codebreaker::exact::habits::cmpp {

py::str HERE;
int64_t HDRLEN = 3;   // what follows a procedure's call of the entry helper: a byte and a word (build.py sets 2 for MASM 3.00)

namespace {

x86::Decoder& MD() {
    static x86::Decoder d;
    return d;
}

Pcc& pcc_() {
    static Pcc p;
    return p;
}

// ------------------------------------------------------------------------------------------ json.load
// Just enough JSON for the ledger's L["0x%05x" % a]["size"]: a parsed tree, malformed text a ValueError
// (Python's json.JSONDecodeError is one).
struct Json {
    enum Kind { Null, Bool, Int, Float, Str, Arr, Obj } kind = Null;
    int64_t i = 0;
    double f = 0;
    py::str s;
    std::vector<std::pair<py::str, std::shared_ptr<Json>>> members;   // an object (a later duplicate key wins)
};

class JsonReader {
    std::string t_;
    size_t p_ = 0;

    [[noreturn]] void bad_(const char* why) const { throw py::ValueError(std::string(why) + ": line 1 column " + std::to_string(p_ + 1)); }
    void ws_() {
        while (p_ < t_.size() && (t_[p_] == ' ' || t_[p_] == '\t' || t_[p_] == '\n' || t_[p_] == '\r')) ++p_;
    }
    bool eat_(const char* lit) {
        size_t n = std::strlen(lit);
        if (t_.compare(p_, n, lit) == 0) {
            p_ += n;
            return true;
        }
        return false;
    }
    static void put_utf8_(std::string& o, unsigned c) {
        if (c < 0x80) o.push_back(static_cast<char>(c));
        else if (c < 0x800) { o.push_back(static_cast<char>(0xC0 | (c >> 6))); o.push_back(static_cast<char>(0x80 | (c & 0x3F))); }
        else if (c < 0x10000) { o.push_back(static_cast<char>(0xE0 | (c >> 12))); o.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F))); o.push_back(static_cast<char>(0x80 | (c & 0x3F))); }
        else { o.push_back(static_cast<char>(0xF0 | (c >> 18))); o.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F))); o.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F))); o.push_back(static_cast<char>(0x80 | (c & 0x3F))); }
    }
    unsigned hex4_() {
        if (p_ + 4 > t_.size()) bad_("Invalid \\uXXXX escape");
        unsigned v = 0;
        for (int k = 0; k < 4; ++k) {
            char c = t_[p_++];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= static_cast<unsigned>(c - '0');
            else if (c >= 'a' && c <= 'f') v |= static_cast<unsigned>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') v |= static_cast<unsigned>(c - 'A' + 10);
            else bad_("Invalid \\uXXXX escape");
        }
        return v;
    }
    py::str string_() {   // at the opening quote
        ++p_;
        std::string o;
        while (true) {
            if (p_ >= t_.size()) bad_("Unterminated string starting at");
            char c = t_[p_++];
            if (c == '"') break;
            if (static_cast<unsigned char>(c) < 0x20) bad_("Invalid control character at");
            if (c != '\\') { o.push_back(c); continue; }
            if (p_ >= t_.size()) bad_("Unterminated string starting at");
            char e = t_[p_++];
            switch (e) {
                case '"': o.push_back('"'); break;
                case '\\': o.push_back('\\'); break;
                case '/': o.push_back('/'); break;
                case 'b': o.push_back('\b'); break;
                case 'f': o.push_back('\f'); break;
                case 'n': o.push_back('\n'); break;
                case 'r': o.push_back('\r'); break;
                case 't': o.push_back('\t'); break;
                case 'u': {
                    unsigned u = hex4_();
                    if (u >= 0xD800 && u < 0xDC00 && t_.compare(p_, 2, "\\u") == 0) {
                        p_ += 2;
                        unsigned lo = hex4_();
                        u = 0x10000 + ((u - 0xD800) << 10) + (lo - 0xDC00);
                    }
                    put_utf8_(o, u);
                    break;
                }
                default: bad_("Invalid \\escape");
            }
        }
        return py::str(o);
    }
    std::shared_ptr<Json> number_() {
        size_t s = p_;
        bool real = false;
        if (p_ < t_.size() && t_[p_] == '-') ++p_;
        if (p_ >= t_.size() || !std::isdigit(static_cast<unsigned char>(t_[p_]))) bad_("Expecting value");
        if (t_[p_] == '0') ++p_;
        else while (p_ < t_.size() && std::isdigit(static_cast<unsigned char>(t_[p_]))) ++p_;
        if (p_ + 1 < t_.size() && t_[p_] == '.' && std::isdigit(static_cast<unsigned char>(t_[p_ + 1]))) {
            real = true;
            ++p_;
            while (p_ < t_.size() && std::isdigit(static_cast<unsigned char>(t_[p_]))) ++p_;
        }
        if (p_ < t_.size() && (t_[p_] == 'e' || t_[p_] == 'E')) {
            size_t q = p_ + 1;
            if (q < t_.size() && (t_[q] == '+' || t_[q] == '-')) ++q;
            if (q < t_.size() && std::isdigit(static_cast<unsigned char>(t_[q]))) {
                real = true;
                p_ = q;
                while (p_ < t_.size() && std::isdigit(static_cast<unsigned char>(t_[p_]))) ++p_;
            }
        }
        auto j = std::make_shared<Json>();
        py::str text(t_.substr(s, p_ - s));
        if (real) {
            j->kind = Json::Float;
            j->f = py::float_from_str(text);
        } else {
            j->kind = Json::Int;
            j->i = py::int_from_str(text);
        }
        return j;
    }
    std::shared_ptr<Json> value_() {
        ws_();
        if (p_ >= t_.size()) bad_("Expecting value");
        auto j = std::make_shared<Json>();
        char c = t_[p_];
        if (c == '"') {
            j->kind = Json::Str;
            j->s = string_();
        } else if (c == '{') {
            ++p_;
            j->kind = Json::Obj;
            ws_();
            if (p_ < t_.size() && t_[p_] == '}') { ++p_; return j; }
            while (true) {
                ws_();
                if (p_ >= t_.size() || t_[p_] != '"') bad_("Expecting property name enclosed in double quotes");
                py::str k = string_();
                ws_();
                if (p_ >= t_.size() || t_[p_] != ':') bad_("Expecting ':' delimiter");
                ++p_;
                std::shared_ptr<Json> v = value_();
                bool found = false;
                for (auto& m : j->members)
                    if (m.first == k) { m.second = v; found = true; }
                if (!found) j->members.emplace_back(k, v);
                ws_();
                if (p_ < t_.size() && t_[p_] == ',') { ++p_; continue; }
                if (p_ < t_.size() && t_[p_] == '}') { ++p_; break; }
                bad_("Expecting ',' delimiter");
            }
        } else if (c == '[') {
            ++p_;
            j->kind = Json::Arr;
            ws_();
            if (p_ < t_.size() && t_[p_] == ']') { ++p_; return j; }
            while (true) {
                j->members.emplace_back(py::str(), value_());
                ws_();
                if (p_ < t_.size() && t_[p_] == ',') { ++p_; continue; }
                if (p_ < t_.size() && t_[p_] == ']') { ++p_; break; }
                bad_("Expecting ',' delimiter");
            }
        } else if (eat_("true")) {
            j->kind = Json::Bool;
            j->i = 1;
        } else if (eat_("false")) {
            j->kind = Json::Bool;
        } else if (eat_("null")) {
        } else if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            return number_();
        } else {
            bad_("Expecting value");
        }
        return j;
    }

  public:
    explicit JsonReader(std::string t) : t_(std::move(t)) {
        if (t_.compare(0, 3, "\xEF\xBB\xBF") == 0) bad_("Unexpected UTF-8 BOM (decode using utf-8-sig)");
    }
    std::shared_ptr<Json> load() {
        std::shared_ptr<Json> j = value_();
        ws_();
        if (p_ < t_.size()) bad_("Extra data");
        return j;
    }
};

// value[key] for a str key: a KeyError when an object lacks it, a TypeError for anything else
const std::shared_ptr<Json>& subscript_(const std::shared_ptr<Json>& v, const py::str& key) {
    if (v->kind == Json::Obj) {
        for (const auto& m : v->members)
            if (m.first == key) return m.second;
        throw py::KeyError(py::repr(key).utf8());
    }
    if (v->kind == Json::Arr) throw py::TypeError("list indices must be integers or slices, not str");
    if (v->kind == Json::Str) throw py::TypeError("string indices must be integers, not 'str'");
    throw py::TypeError("object is not subscriptable");
}

// a Json used as an int (a slice bound)
int64_t as_int_(const std::shared_ptr<Json>& v) {
    if (v->kind == Json::Int || v->kind == Json::Bool) return v->i;
    throw py::TypeError("slice indices must be integers or None or have an __index__ method");
}

}  // namespace

void set_decoder(x86::Decoder d) { MD() = std::move(d); }
void set_pcc(Pcc p) { pcc_() = std::move(p); }

py::list<WalkItem> walk(const py::bytes& b, int64_t base, bool skip_hdr) {
    py::list<WalkItem> out;
    int64_t p = 0;
    int64_t n = py::len(b);
    while (p < n) {
        if (skip_hdr && p == 3 && b[0] == 0xE8) {
            if (HDRLEN == 2) {   // MS-Pascal 3.x: only the frame size behind the call
                out.append(WalkItem(3, 2, py::percent(py::str("dw 0x%04x"), py::unpack_from1<int64_t>("<H", b, 3)), py::str("hdr")));
                p = 5;
                continue;
            }
            out.append(WalkItem(3, 3, py::percent(py::str("db 0x%02x ; dw 0x%04x"), b[3], py::unpack_from1<int64_t>("<H", b, 4)), py::str("hdr")));
            p = 6;
            continue;
        }
        if (!MD()) throw py::RuntimeError("cmpp: no decoder set (cmpp::set_decoder)");
        std::optional<x86::Insn> i = MD()(b.slice(p, p + 16), base + p);
        if (!i) {
            out.append(WalkItem(p, 1, py::percent(py::str("db 0x%02x"), b[p]), py::str("db")));
            p += 1;
        } else {
            out.append(WalkItem(p, i->size, py::percent(py::str("%s %s"), i->mnemonic, i->op_str), i->mnemonic));
            p += i->size;
        }
    }
    return out;
}

py::set<int64_t> loose_bytes(const py::bytes& b, const py::set<int64_t>& fx) {
    py::set<int64_t> m = py::set<int64_t>::from(fx);
    int64_t n = py::len(b);
    for (const WalkItem& item : walk(b)) {
        int64_t off = std::get<0>(item);
        int64_t sz = std::get<1>(item);
        if (b[off] == 0xE8 && sz == 3) {
            int64_t tgt = off + 3 + py::unpack_from1<int64_t>("<h", b, off + 1);
            if (!(0 <= tgt && tgt < n)) {
                m.add(off + 1);
                m.add(off + 2);
            }
        } else if ((b[off] == 0x9A || b[off] == 0xEA) && sz == 5) {
            for (int64_t k = off + 1; k < off + 5; ++k) m.add(k);
        }
    }
    return m;
}

int64_t main(const py::list<py::str>& argv) {
    static const py::re::Pattern bad_line(py::str("\\^|Error|Warning"), 0);
    static const py::re::Pattern proc_line(py::str("\\s*Procedure/Function : (\\S+)"), 0);
    static const py::re::Pattern num(py::str("0x[0-9a-f]+|\\b\\d+\\b"), 0);
    auto nz = [&](const py::str& t) { return num.sub(py::str("N"), t); };

    py::list<py::str> a_;
    for (const py::str& x : argv.slice(1, py::len(argv))) {
        if (!x.startswith(py::str("--"))) a_.append(x);
    }
    // opt = lambda k, d: (sys.argv[sys.argv.index(k) + 1] if k in sys.argv else d)
    auto opt = [&](const char* k, const std::optional<py::str>& d) -> std::optional<py::str> {
        py::str key(k);
        return argv.contains(key) ? std::optional<py::str>(argv[argv.index(key) + 1]) : d;
    };
    if (argv.contains(py::str("--proc"))) {
        py::list<py::str> kept;
        for (const py::str& x : a_) {
            std::optional<py::str> pn = opt("--proc", std::nullopt);
            if (!pn || x != *pn) kept.append(x);
        }
        a_ = kept;
    }
    py::str exe = a_[0];
    int64_t a = py::int_from_str(a_[1], 16);
    if (!pcc_().image) throw ImportError("No module named 'pcc'");
    py::bytes img = pcc_().image(exe);
    std::shared_ptr<Json> L;
    {
        py::file f = py::file::open(py::os::path::join(HERE, py::str(".."), py::str("ledger"), exe.upper() + py::str(".json")), py::str("r"));
        L = JsonReader(f.read_text().utf8()).load();
    }
    int64_t size = as_int_(subscript_(subscript_(L, py::percent(py::str("0x%05x"), a)), py::str("size")));
    py::bytes want = img.slice(a, a + size);
    if (argv.contains(py::str("--asm"))) {
        py::print(py::percent(py::str("== %s 0x%05x  %d bytes"), exe, a, size));
        for (const WalkItem& item : walk(want, a)) {
            int64_t off = std::get<0>(item);
            int64_t sz = std::get<1>(item);
            py::print(py::percent(py::str("%05x %-14s %s"), a + off, want.slice(off, off + sz).hex().slice(0, 14), std::get<2>(item)));
        }
        return 0;
    }
    CompileResult r = pcc_().compile_pas(a_[2]);
    py::str name = opt("--proc", py::percent(py::str("P%05X"), a)).value().upper();
    if (!r.code || !r.procs.contains(name)) {
        py::print(py::percent(py::str("NO CODE for %s"), name));
        py::list<py::str> bad;
        for (const py::str& l : py::split(r.lst, py::str("\n"))) {
            if (bad_line.search(l)) bad.append(l);
        }
        if (py::len(bad)) {
            py::print(py::join(py::str("\n"), bad.slice(0, 30)));
        } else {
            py::print(r.console.slice(-600, py::len(r.console)));
        }
        return 2;
    }
    auto [o, n] = r.procs.at(name);
    py::bytes got = r.code->slice(o, o + n);
    py::set<int64_t> fx;
    for (int64_t k : r.fix) {
        if (o <= k && k < o + n) fx.add(k - o);
    }
    if (argv.contains(py::str("--cod"))) {
        bool on = false;
        for (const py::str& line : py::split(r.cod, py::str("\n"))) {
            std::optional<py::re::Match> m = proc_line.match(line);
            if (m) on = m->group(1).upper() == name;
            if (on) py::print(line);
        }
    }
    py::set<int64_t> mg = loose_bytes(got, fx);
    py::set<int64_t> mw = loose_bytes(want);
    (void)mw;
    bool same = n == size;
    for (int64_t k = 0; same && k < n; ++k) {
        if (!(mg.contains(k) || got[k] == want[k])) same = false;
    }
    if (same) {
        py::print(py::percent(py::str("IDENTICAL  %s 0x%05x  %d bytes (%d not compared: link-time addresses, calls to other procedures)"), exe, a, n,
                              py::len(mg)));
        return 0;
    }
    py::print(py::percent(py::str("DIFFERS  compiled %d bytes, original %d"), n, size));
    py::list<WalkItem> A = walk(want, a);
    py::list<WalkItem> B = walk(got, a);
    int64_t i = 0, j = 0, shown = 0;
    while (i < py::len(A) && j < py::len(B)) {
        auto [oa, sa, ta, ma] = A[i];
        auto [ob, sb, tb, mb] = B[j];
        bool lo = false;
        for (int64_t k = 0; k < sb && !lo; ++k) lo = mg.contains(ob + k);
        bool same_ = lo ? (nz(ta) == nz(tb)) : (want.slice(oa, oa + sa) == got.slice(ob, ob + sb));
        if (same_) {
            i += 1;
            j += 1;
            continue;
        }
        if (ma == mb && sa == sb && shown < 12) {
            py::print(py::percent(py::str("  %05x  %-34s | %s"), a + oa, ta, tb));
            shown += 1;
            i += 1;
            j += 1;
            continue;
        }
        py::print(py::percent(py::str("  out of step at original %05x / compiled +%d:"), a + oa, ob));
        for (int64_t k = 0; k < 14; ++k) {
            py::str l = i + k < py::len(A) ? py::percent(py::str("%05x %s"), a + std::get<0>(A[i + k]), std::get<2>(A[i + k])) : py::str("");
            py::str rr;
            if (j + k < py::len(B)) {
                bool star = false;
                for (int64_t q = 0; q < std::get<1>(B[j + k]) && !star; ++q) star = mg.contains(std::get<0>(B[j + k]) + q);
                rr = py::percent(py::str("%s%s"), star ? "*" : " ", std::get<2>(B[j + k]));
            }
            py::print(py::percent(py::str("    %-40s | %s"), l, rr));
        }
        return 1;
    }
    return 1;
}

}  // namespace codebreaker::exact::habits::cmpp
