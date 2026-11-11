// codebreaker/recompile/config.cpp -- typed C++ port of codebreaker.recompile.config (see config.hpp).

#include "codebreaker/recompile/config.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/switches.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::config {

namespace powerpc_image = codebreaker::formats::powerpc_image;

const py::list<std::tuple<py::str, bool>> REGISTER_SETTINGS = {
    {py::str("skip_lr"), false},         {py::str("skip_msr"), true},
    {py::str("ctr_as_local"), false},    {py::str("xer_as_local"), false},
    {py::str("reserved_as_local"), true}, {py::str("cr_as_local"), false},
    {py::str("non_argument_as_local"), false}, {py::str("non_volatile_as_local"), false}};

namespace {

// ---- reading code words -----------------------------------------------------------------------------------
// the [a, b) a Python slice of a sequence of length n makes of the indices (a, b)
std::pair<int64_t, int64_t> slice_span(int64_t n, int64_t a, int64_t b) {
    auto norm = [n](int64_t i) {
        if (i < 0) {
            i += n;
            return i < 0 ? int64_t(0) : i;
        }
        return i > n ? n : i;
    };
    return {norm(a), norm(b)};
}

// int.from_bytes(code[o:o + 4], "big"): a slice that runs off the end is shorter (and no slice is 0)
int64_t word_at(const std::string& c, int64_t o) {
    const int64_t n = static_cast<int64_t>(c.size());
    if (o >= 0 && o + 4 <= n) {
        const unsigned char* p = reinterpret_cast<const unsigned char*>(c.data()) + o;
        return (static_cast<int64_t>(p[0]) << 24) | (static_cast<int64_t>(p[1]) << 16) | (static_cast<int64_t>(p[2]) << 8) | p[3];
    }
    const auto [a, b] = slice_span(n, o, o + 4);
    int64_t v = 0;
    for (int64_t i = a; i < b; ++i) v = (v << 8) | static_cast<unsigned char>(c[static_cast<size_t>(i)]);
    return v;
}

// code[o:o + 4] == b"\0\0\0\0"
bool zero_word(const std::string& c, int64_t o) {
    const auto [a, b] = slice_span(static_cast<int64_t>(c.size()), o, o + 4);
    if (b - a != 4) return false;
    for (int64_t i = a; i < b; ++i)
        if (c[static_cast<size_t>(i)] != '\0') return false;
    return true;
}

const powerpc_image::Image& need_runs(const powerpc_image::Image& img) {
    if (!img.run_names) throw py::AttributeError("'Image' object has no attribute 'run_names'");
    return img;
}

// ---- a small TOML reader (tomllib.load / loads) for the switch-table files --------------------------------------
struct Value {
    enum class Is { Int, Str, Bool, Arr } is = Is::Int;
    int64_t i = 0;
    std::string s;
    bool b = false;
    std::vector<Value> a;
};

struct Table {
    std::vector<std::pair<std::string, Value>> kv;
    const Value* get(const std::string& k) const {
        for (const auto& p : kv)
            if (p.first == k) return &p.second;
        return nullptr;
    }
};

struct Doc {
    std::vector<Table> switches;      // every [[switch]] table
    bool switch_is_other = false;     // `switch` was defined as something that is not an array of tables
};

[[noreturn]] void toml_error(const std::string& what) { throw TOMLDecodeError(what); }

class Reader {
    const std::string& t_;
    size_t p_ = 0;

    bool eof() const { return p_ >= t_.size(); }
    char cur() const { return p_ < t_.size() ? t_[p_] : '\0'; }
    void skip_blank() {   // spaces and tabs
        while (!eof() && (cur() == ' ' || cur() == '\t')) ++p_;
    }
    void skip_comment() {
        if (cur() == '#')
            while (!eof() && cur() != '\n') ++p_;
    }
    // blank space, newlines and comments (between array items, between statements)
    void skip_all() {
        for (;;) {
            skip_blank();
            if (cur() == '#') skip_comment();
            else if (cur() == '\n' || cur() == '\r') ++p_;
            else break;
        }
    }
    static bool bare(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-'; }

    std::string read_string() {
        const char q = cur();
        ++p_;
        std::string out;
        while (!eof() && cur() != q) {
            if (cur() == '\n') toml_error("Illegal character '\\n'");
            if (q == '"' && cur() == '\\') {
                ++p_;
                const char e = cur();
                ++p_;
                switch (e) {
                    case 'n': out += '\n'; break;
                    case 't': out += '\t'; break;
                    case 'r': out += '\r'; break;
                    case 'b': out += '\b'; break;
                    case 'f': out += '\f'; break;
                    case '"': out += '"'; break;
                    case '\\': out += '\\'; break;
                    default: toml_error("Unescaped '\\' in a string");
                }
            } else {
                out += cur();
                ++p_;
            }
        }
        if (eof()) toml_error("Unterminated string");
        ++p_;
        return out;
    }

    Value read_number() {
        const size_t a = p_;
        while (!eof() && !std::isspace(static_cast<unsigned char>(cur())) && cur() != ',' && cur() != ']' && cur() != '#') ++p_;
        std::string tok = t_.substr(a, p_ - a);
        if (tok.empty()) toml_error("Invalid value");
        bool neg = false;
        size_t i = 0;
        if (tok[0] == '+' || tok[0] == '-') {
            neg = tok[0] == '-';
            i = 1;
        }
        int base = 10;
        if (tok.size() > i + 1 && tok[i] == '0' && (tok[i + 1] == 'x' || tok[i + 1] == 'o' || tok[i + 1] == 'b')) {
            if (i != 0) toml_error("Invalid value");       // (a sign is for decimal integers)
            base = tok[i + 1] == 'x' ? 16 : tok[i + 1] == 'o' ? 8 : 2;
            i += 2;
        }
        if (i >= tok.size()) toml_error("Invalid value");
        if (base == 10 && tok[i] == '0' && tok.size() > i + 1) toml_error("Invalid value");   // (no leading zeros)
        uint64_t v = 0;
        bool prev_us = true;                                    // (an underscore must sit between digits)
        for (; i < tok.size(); ++i) {
            const char c = tok[i];
            if (c == '_') {
                if (prev_us) toml_error("Invalid value");
                prev_us = true;
                continue;
            }
            int d = -1;
            if (c >= '0' && c <= '9') d = c - '0';
            else if (base == 16 && c >= 'a' && c <= 'f') d = c - 'a' + 10;
            else if (base == 16 && c >= 'A' && c <= 'F') d = c - 'A' + 10;
            if (d < 0 || d >= base) toml_error("Invalid value");
            if (v > (static_cast<uint64_t>(INT64_MAX) - static_cast<uint64_t>(d)) / static_cast<uint64_t>(base) + 1) toml_error("Invalid value");
            v = v * static_cast<uint64_t>(base) + static_cast<uint64_t>(d);
            prev_us = false;
        }
        if (prev_us) toml_error("Invalid value");
        Value out;
        out.is = Value::Is::Int;
        out.i = neg ? -static_cast<int64_t>(v) : static_cast<int64_t>(v);
        return out;
    }

    Value read_value() {
        Value out;
        const char c = cur();
        if (c == '[') {
            ++p_;
            out.is = Value::Is::Arr;
            for (;;) {
                skip_all();
                if (cur() == ']') {
                    ++p_;
                    break;
                }
                out.a.push_back(read_value());
                skip_all();
                if (cur() == ',') {
                    ++p_;
                    continue;
                }
                if (cur() == ']') {
                    ++p_;
                    break;
                }
                toml_error("Unclosed array");
            }
            return out;
        }
        if (c == '"' || c == '\'') {
            out.is = Value::Is::Str;
            out.s = read_string();
            return out;
        }
        if (t_.compare(p_, 4, "true") == 0 && !bare(p_ + 4 < t_.size() ? t_[p_ + 4] : ' ')) {
            p_ += 4;
            out.is = Value::Is::Bool;
            out.b = true;
            return out;
        }
        if (t_.compare(p_, 5, "false") == 0 && !bare(p_ + 5 < t_.size() ? t_[p_ + 5] : ' ')) {
            p_ += 5;
            out.is = Value::Is::Bool;
            out.b = false;
            return out;
        }
        return read_number();
    }

public:
    explicit Reader(const std::string& t) : t_(t) {}

    Doc read() {
        Doc doc;
        std::vector<std::string> plain;       // the plain tables and keys seen at the top level, by name
        Table* current = nullptr;
        Table top;
        bool in_top = true;
        std::string cur_name;
        for (;;) {
            skip_all();
            if (eof()) break;
            if (cur() == '[') {
                const bool array = p_ + 1 < t_.size() && t_[p_ + 1] == '[';
                p_ += array ? 2 : 1;
                skip_blank();
                const size_t a = p_;
                while (!eof() && cur() != ']' && cur() != '\n') ++p_;
                std::string name = t_.substr(a, p_ - a);
                while (!name.empty() && (name.back() == ' ' || name.back() == '\t')) name.pop_back();
                if (cur() != ']') toml_error("Expected ']'");
                ++p_;
                if (array) {
                    if (cur() != ']') toml_error("Expected ']]'");
                    ++p_;
                }
                skip_blank();
                skip_comment();
                if (!eof() && cur() != '\n' && cur() != '\r') toml_error("Expected newline or end of document after a statement");
                in_top = false;
                cur_name = name;
                if (name == "switch" && array) {
                    doc.switches.emplace_back();
                    current = &doc.switches.back();
                } else {
                    if (name == "switch") doc.switch_is_other = true;
                    top = Table();                      // (a table the readers here do not use)
                    current = &top;
                }
                continue;
            }
            std::string key;
            if (cur() == '"' || cur() == '\'') {
                key = read_string();
            } else {
                const size_t a = p_;
                while (!eof() && bare(cur())) ++p_;
                key = t_.substr(a, p_ - a);
                if (key.empty()) toml_error("Invalid statement");
            }
            skip_blank();
            if (cur() != '=') toml_error("Expected '=' after a key in a key/value pair");
            ++p_;
            skip_blank();
            Value v = read_value();
            skip_blank();
            skip_comment();
            if (!eof() && cur() != '\n' && cur() != '\r') toml_error("Expected newline or end of document after a statement");
            Table& into = in_top ? top : *current;
            if (into.get(key)) toml_error("Cannot overwrite a value");
            if (in_top && key == "switch") doc.switch_is_other = true;
            into.kv.emplace_back(key, std::move(v));
        }
        return doc;
    }
};

Doc parse(const std::string& text) {
    // (tomllib.loads takes a str; load takes bytes decoded as UTF-8 first)
    return Reader(text).read();
}

// the `switch` tables of a parsed document, as `data.get("switch", [])` yields them
const std::vector<Table>& switch_tables_of(const Doc& d) {
    if (d.switch_is_other) throw py::TypeError("string indices must be integers, not 'str'");
    return d.switches;
}

int64_t int_of(const Value* v) {
    if (v->is != Value::Is::Int) throw py::TypeError("unhashable or non-integer TOML value");
    return v->i;
}

const Value* need(const Table& t, const char* key) {
    const Value* v = t.get(key);
    if (!v) throw py::KeyError(key);
    return v;
}

py::list<int64_t> ints_of(const Value* v) {
    if (v->is != Value::Is::Arr) throw py::TypeError("labels is not a list");
    py::list<int64_t> out;
    for (const Value& x : v->a) out.append(int_of(&x));
    return out;
}

}  // namespace

py::dict<py::str, int64_t> helpers(const powerpc_image::Image& img) {
    py::dict<py::str, int64_t> out;
    py::dict<py::str, int64_t> runs;
    for (const auto& kv : need_runs(img).run_names->items()) runs.set(std::get<1>(kv), std::get<0>(kv));
    for (const auto& pair : {std::make_pair("savegprlr_14_address", "__savegprlr_14"), std::make_pair("restgprlr_14_address", "__restgprlr_14"),
                             std::make_pair("savefpr_14_address", "__savefpr_14"), std::make_pair("restfpr_14_address", "__restfpr_14")}) {
        if (runs.contains(py::str(pair.second))) out.set(py::str(pair.first), runs.at(py::str(pair.second)));
    }
    // the vector runs: li r11,-0x120 (v14) / li r11,-0x400 (v64), then stvx or lvx through r12
    const int64_t lo = std::get<0>(img.code), hi = std::get<1>(img.code);
    const py::bytes code = img.bytes_at(lo, hi - lo);
    const std::string& c = code.raw();
    for (int64_t o = 0; o < static_cast<int64_t>(c.size()) - 7; o += 4) {
        const int64_t w = word_at(c, o);
        if (w != 0x3960FEE0 && w != 0x3960FC00) continue;
        const auto rows = disassemble::listing(code, lo, o + 4, 1);
        if (rows.empty()) continue;
        const py::str& mnem = std::get<2>(rows[0]);
        const py::str ops = std::get<3>(rows[0]).replace(py::str(" "), py::str(""));
        const py::str want = w == 0x3960FEE0 ? py::str("v14,r11,r12") : py::str("v64,r11,r12");
        if (ops != want) continue;
        const py::str n = w == 0x3960FEE0 ? py::str("14") : py::str("64");
        if (mnem.startswith(py::str("stvx"))) {
            const py::str key = py::str("savevmx_") + n + py::str("_address");
            if (!out.contains(key)) out.set(key, lo + o);
        } else if (mnem.startswith(py::str("lvx"))) {
            const py::str key = py::str("restvmx_") + n + py::str("_address");
            if (!out.contains(key)) out.set(key, lo + o);
        }
    }
    out.update(jump_buffers(img, code));
    return out;
}

int64_t _start_before(const py::bytes& code, int64_t lo, int64_t a) {
    const std::string& c = code.raw();
    int64_t o = a - lo;
    while (o >= 4 && word_at(c, o - 4) != 0 && word_at(c, o - 4) != 0x4E800020) o -= 4;
    return lo + o;
}

py::dict<py::str, int64_t> jump_buffers(const powerpc_image::Image& img, const py::bytes& code) {
    const int64_t lo = std::get<0>(img.code);
    py::dict<py::str, int64_t> out;
    const std::string& c = code.raw();
    const int64_t n = static_cast<int64_t>(c.size());
    std::vector<int64_t> words;
    for (int64_t o = 0; o < n - 3; o += 4) words.push_back(word_at(c, o));
    const py::str setjmp_key("setjmp_address"), longjmp_key("longjmp_address");
    for (size_t i = 0; i < words.size(); ++i) {
        const int64_t w = words[i];
        // setjmp stores the caller's r1 at 0x90 of the jmp_buf. The buffer pointer is its first
        // argument (r3) by the ABI, but some system modules (XAM, Guide.TFA) reach it through
        // another register, so match `std r1,0x90(rA)` for any rA -- keying on r3 alone misses
        // those and would leave longjmp below without a setjmp.
        if ((w >> 26) == 62 && ((w >> 21) & 31) == 1 && (w & 0xFFFF) == 0x90 && !out.contains(setjmp_key))
            out.set(setjmp_key, _start_before(code, lo, lo + 4 * static_cast<int64_t>(i)));
        // lwz rD,0x134(rA) then lwz rE,0x90(rA) (D-form, opcode 32) within a few words
        if ((w >> 26) == 32 && (w & 0xFFFF) == 0x134 && !out.contains(longjmp_key)) {
            const int64_t ra = (w >> 16) & 31;
            for (size_t k = i + 1; k < std::min(words.size(), i + 4); ++k) {
                const int64_t v = words[k];
                if ((v >> 26) == 32 && (v & 0xFFFF) == 0x90 && ((v >> 16) & 31) == ra) {
                    const int64_t a = lo + 4 * static_cast<int64_t>(i);
                    std::optional<int64_t> start;
                    for (const auto& kv : img.extents.items()) {
                        const int64_t s = std::get<0>(kv), size = std::get<0>(std::get<1>(kv));
                        if (s <= a && a < s + size) {
                            start = s;
                            break;
                        }
                    }
                    out.set(longjmp_key, start ? *start : _start_before(code, lo, a));
                    break;
                }
            }
        }
    }
    // A longjmp_address with no setjmp_address is the silent crash: XenonRecomp reads setJmpAddress
    // as 0, the guest's setjmp never fills the host jump buffer, and the first longjmp jumps through
    // zeros (rip=0) -- uncatchable. Never emit one without the other.
    if (out.contains(longjmp_key) && !out.contains(setjmp_key)) out.pop(longjmp_key);
    return out;
}

SwitchTables read_switch_tables(const py::str& path) {
    py::file f = py::open(path, py::str("rb"));
    const py::bytes data = f.read();
    f.close();
    const Doc doc = parse(data.raw());
    SwitchTables out;
    for (const Table& s : switch_tables_of(doc)) out.set(int_of(need(s, "base")), ints_of(need(s, "labels")));
    return out;
}

std::tuple<int64_t, int64_t> write_switch_tables(const py::str& path, const py::str& xex_path, const std::optional<py::str>& analysed) {
    auto [found, report] = switches::find(xex_path);
    (void)report;
    py::set<int64_t> have;
    for (const switches::Table& t : found) have.add(t.bctr);
    const py::ref<powerpc_image::Image> img = discover::_image(xex_path);
    const int64_t lo = std::get<0>(img->code), hi = std::get<1>(img->code);
    const py::bytes code = img->bytes_at(lo, hi - lo);
    const std::string& c = code.raw();
    int64_t added = 0;
    if (analysed && !analysed->empty()) {
        const Doc doc = parse(analysed->utf8());
        for (const Table& s : switch_tables_of(doc)) {
            const int64_t base = int_of(need(s, "base"));
            int64_t bctr = base;                    // XenonAnalyse's base is the sequence's start: find its bctr
            while (bctr < hi && word_at(c, bctr - lo) != 0x4E800420) bctr += 4;
            if (have.contains(bctr) || bctr >= hi) continue;
            have.add(bctr);
            switches::Table t;
            t.bctr = base;
            t.r = int_of(need(s, "r"));
            const Value* d = s.get("default");
            t.default_ = d ? int_of(d) : 0;
            t.labels = ints_of(need(s, "labels"));
            t.kind = py::str("XenonAnalyse");
            t.table = 0;
            t.guard = 0;
            t.inline_ = false;
            found.append(t);
            ++added;
        }
    }
    switches::write(path, found);
    return {found.size() - added, added};
}

int64_t _end_of_run(const py::bytes& code, int64_t lo, int64_t a) {
    const std::string& c = code.raw();
    const int64_t n = static_cast<int64_t>(c.size());
    for (int64_t o = a - lo; o < n - 3; o += 4) {
        const int64_t w = word_at(c, o);
        if (w == 0x4E800020 || w == 0x4E800420 || ((w >> 26) == 18 && !(w & 3))) return lo + o + 4;
    }
    return lo + n;
}

py::list<std::tuple<int64_t, int64_t>> extra_functions(const py::str& xex_path, const powerpc_image::Image& img,
                                                       const std::optional<SwitchTables>& switches_in) {
    SwitchTables switches_;
    if (switches_in && !switches_in->empty()) switches_ = *switches_in;
    std::unordered_set<int64_t> labels;
    for (const auto& kv : switches_.items())
        for (int64_t l : std::get<1>(kv)) labels.insert(l);
    const auto [funcs, sources, discovered] = discover::discover(xex_path);
    (void)sources;
    (void)discovered;
    const int64_t lo = std::get<0>(img.code), hi = std::get<1>(img.code);
    const py::bytes code = img.bytes_at(lo, hi - lo);
    const std::string& c = code.raw();
    // XenonRecomp emits each save/restore run itself, an entry per register: nothing inside one is extra
    std::vector<std::pair<int64_t, int64_t>> runs;
    for (const auto& kv : helpers(img).items()) {
        const int64_t a = std::get<1>(kv);
        runs.emplace_back(a, _end_of_run(code, lo, a));
    }
    auto in_run = [&runs](int64_t a) {
        for (const auto& r : runs)
            if (r.first <= a && a < r.second) return true;
        return false;
    };
    std::vector<int64_t> starts;
    for (const auto& kv : funcs.items()) {
        const int64_t a = std::get<0>(kv);
        if (!labels.count(a) && !in_run(a)) starts.push_back(a);
    }
    std::vector<int64_t> bounds(starts);
    for (const auto& kv : img.imports.items()) bounds.push_back(std::get<0>(kv));
    for (const auto& kv : need_runs(img).run_names->items()) bounds.push_back(std::get<0>(kv));
    for (const auto& kv : img.extents.items()) bounds.push_back(std::get<0>(kv));
    for (const auto& r : runs) bounds.push_back(r.first);
    std::sort(bounds.begin(), bounds.end());
    bounds.erase(std::unique(bounds.begin(), bounds.end()), bounds.end());
    std::vector<std::pair<int64_t, int64_t>> pdata;
    for (const auto& kv : img.extents.items()) pdata.emplace_back(std::get<0>(kv), std::get<0>(kv) + std::get<0>(std::get<1>(kv)));
    std::sort(pdata.begin(), pdata.end());
    std::vector<int64_t> pdata_starts;
    for (const auto& p : pdata) pdata_starts.push_back(p.first);
    auto bisect_right = [](const std::vector<int64_t>& v, int64_t x) {
        return static_cast<int64_t>(std::upper_bound(v.begin(), v.end(), x) - v.begin());
    };
    auto max_of = [](const py::list<int64_t>& cases) {
        if (cases.empty()) throw py::ValueError("max() iterable argument is empty");
        int64_t m = cases[0];
        for (int64_t x : cases) m = std::max(m, x);
        return m;
    };

    auto size = [&](int64_t a) -> int64_t {
        // containing(a): a second entry of a .pdata function runs to its end
        const int64_t ci = bisect_right(pdata_starts, a) - 1;
        if (ci >= 0 && pdata[static_cast<size_t>(ci)].first < a && a < pdata[static_cast<size_t>(ci)].second)
            return pdata[static_cast<size_t>(ci)].second - a;
        const int64_t i = bisect_right(bounds, a);
        int64_t end = std::min({i < static_cast<int64_t>(bounds.size()) ? bounds[static_cast<size_t>(i)] : hi, a + 0x4000, hi});
        for (const auto& kv : switches_.items()) {          // its switches' cases are inside it
            const int64_t base = std::get<0>(kv);
            if (a <= base && base < end) end = std::max(end, std::min(_end_of_run(code, lo, max_of(std::get<1>(kv))), hi));
        }
        while (end - 4 > a && zero_word(c, end - 4 - lo)) end -= 4;   // trailing padding
        return end - a;
    };

    py::dict<int64_t, int64_t> found;
    {
        std::vector<int64_t> sorted_starts(starts);
        std::sort(sorted_starts.begin(), sorted_starts.end());
        for (int64_t a : sorted_starts) {
            if (img.extents.contains(a) || img.run_names->contains(a) || !(lo <= a && a < hi)) continue;
            found.set(a, size(a));
        }
    }
    // A switch outside every known function (between .pdata functions): its function runs from the start of
    // that gap (after the previous function's end and any padding) to the end of its last case; left to
    // XenonRecomp, the gap's analysis stops at the bctr and the cases fall outside it.
    std::vector<std::pair<int64_t, int64_t>> covered(pdata);
    for (const auto& kv : found.items()) covered.emplace_back(std::get<0>(kv), std::get<0>(kv) + std::get<1>(kv));
    std::sort(covered.begin(), covered.end());
    std::vector<int64_t> covered_starts;
    for (const auto& p : covered) covered_starts.push_back(p.first);
    for (const auto& kv : switches_.items()) {
        const int64_t bctr = std::get<0>(kv);
        const int64_t i = bisect_right(covered_starts, bctr) - 1;
        if (i >= 0 && covered[static_cast<size_t>(i)].first <= bctr && bctr < covered[static_cast<size_t>(i)].second) continue;
        int64_t start = i >= 0 ? covered[static_cast<size_t>(i)].second : lo;
        while (start < bctr && zero_word(c, start - lo)) start += 4;
        if (img.extents.contains(start) || !(lo <= start && start < hi)) continue;
        int64_t end = std::max(_end_of_run(code, lo, max_of(std::get<1>(kv))), bctr + 4);
        const int64_t j = bisect_right(pdata_starts, start);
        if (j < static_cast<int64_t>(pdata_starts.size())) end = std::min(end, pdata_starts[static_cast<size_t>(j)]);
        found.set(start, end - start);
    }
    // Every branch (b, bc; no link) out of a function is a tail call: its target must be a function too, or
    // XenonRecomp has nothing to call there. New functions have branches of their own: to a fixed point.
    std::unordered_set<int64_t> known;
    for (const auto& kv : img.extents.items()) known.insert(std::get<0>(kv));
    for (const auto& kv : found.items()) known.insert(std::get<0>(kv));
    for (const auto& kv : img.run_names->items()) known.insert(std::get<0>(kv));
    for (const auto& kv : img.imports.items()) known.insert(std::get<0>(kv));
    for (const auto& r : runs) known.insert(r.first);
    std::vector<std::pair<int64_t, int64_t>> todo(pdata);
    for (const auto& kv : found.items()) todo.emplace_back(std::get<0>(kv), std::get<0>(kv) + std::get<1>(kv));
    while (!todo.empty()) {
        std::vector<std::pair<int64_t, int64_t>> fresh;
        for (const auto& se : todo) {
            const int64_t s = se.first, e = se.second;
            for (int64_t o = s - lo; o < std::min(e, hi) - lo; o += 4) {
                const int64_t w = word_at(c, o);
                const int64_t op = w >> 26;
                int64_t d;
                if (op == 18 && !(w & 3)) {                    // b
                    d = w & 0x03FFFFFC;
                    if (d & 0x02000000) d -= 0x04000000;
                } else if (op == 16 && !(w & 3)) {             // bc
                    d = w & 0xFFFC;
                    if (d & 0x8000) d -= 0x10000;
                } else {
                    continue;
                }
                const int64_t t = lo + o + d;
                if ((s <= t && t < e) || known.count(t) || labels.count(t) || !(lo <= t && t < hi) || in_run(t)) continue;
                known.insert(t);
                const int64_t sz = size(t);
                found.set(t, sz);
                fresh.emplace_back(t, t + sz);
            }
        }
        todo = std::move(fresh);
    }
    std::vector<std::tuple<int64_t, int64_t>> items;
    for (const auto& kv : found.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(items.begin(), items.end());
    return py::list<std::tuple<int64_t, int64_t>>(items);
}

py::str make(const py::str& xex_path, const py::str& out_dir, const py::str& switch_tables, const std::optional<py::str>& hooks,
             const std::optional<py::str>& file_path) {
    const py::ref<powerpc_image::Image> img = discover::_image(xex_path);
    auto slashes = [](const py::str& s) { return s.replace(py::str("\\"), py::str("/")); };
    py::list<py::str> lines;
    lines.append(py::str("# XenonRecomp configuration, written by CodeBreaker (codebreaker/recompile/config.py)"));
    lines.append(py::str("[main]"));
    lines.append(py::str("file_path = \"") + slashes(file_path && !file_path->empty() ? *file_path : xex_path) + py::str("\""));
    lines.append(py::str("out_directory_path = \"") + slashes(out_dir) + py::str("\""));
    lines.append(py::str("switch_table_file_path = \"") + slashes(switch_tables) + py::str("\""));
    lines.append(py::str(""));
    for (const auto& kv : REGISTER_SETTINGS) lines.append(std::get<0>(kv) + py::str(" = ") + (std::get<1>(kv) ? py::str("true") : py::str("false")));
    lines.append(py::str(""));
    std::vector<std::tuple<py::str, int64_t>> hs;
    for (const auto& kv : helpers(*img).items()) hs.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(hs.begin(), hs.end());
    for (const auto& kv : hs) lines.append(py::percent(py::str("%s = 0x%08X"), std::get<0>(kv), std::get<1>(kv)));
    const py::str switch_path = py::os::path::join(py::os::path::dirname(py::os::path::abspath(xex_path)), switch_tables);
    std::optional<SwitchTables> tables;
    if (py::os::path::isfile(switch_path)) tables = read_switch_tables(switch_path);
    const auto extra = extra_functions(xex_path, *img, tables);
    lines.append(py::str(""));
    lines.append(py::percent(py::str("# functions .pdata leaves out (%d)"), extra.size()));
    lines.append(py::str("functions = ["));
    for (const auto& kv : extra) lines.append(py::percent(py::str("    { address = 0x%08X, size = 0x%X },"), std::get<0>(kv), std::get<1>(kv)));
    lines.append(py::str("]"));
    lines.append(py::str(""));
    if (hooks && !hooks->empty()) {
        lines.append(hooks->strip());
        lines.append(py::str(""));
    }
    return py::join(py::str("\n"), lines);
}

}  // namespace codebreaker::recompile::config
