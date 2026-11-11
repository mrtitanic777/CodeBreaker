// codebreaker/analyze/facts.cpp -- typed C++ port of codebreaker.analyze.facts (see facts.hpp).

#include "codebreaker/analyze/facts.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/store/functions.hpp"
#include "py_json.hpp"

namespace codebreaker::analyze::facts {

namespace {

// The schema, as the four statements executescript() runs.
const char* const SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS fact(\n"
    "    module TEXT NOT NULL, analysis TEXT NOT NULL, addr TEXT NOT NULL DEFAULT '', kind TEXT NOT NULL,\n"
    "    value TEXT, ts REAL)",
    "CREATE INDEX IF NOT EXISTS fact_module ON fact(module, analysis)",
    "CREATE INDEX IF NOT EXISTS fact_addr ON fact(module, addr)",
    "CREATE INDEX IF NOT EXISTS fact_kind ON fact(module, kind)",
};

// ---- json.dumps(value, sort_keys=True) -------------------------------------------------------------
void dump_into(std::string& o, const Json& v) {
    switch (v.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += v.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(v.i); break;
        case Json::Float:
            if (std::isnan(v.d)) o += "NaN";
            else if (std::isinf(v.d)) o += v.d > 0 ? "Infinity" : "-Infinity";
            else o += py::to_str(v.d).utf8();
            break;
        case Json::Str: py::json::put(o, v.s); break;
        case Json::List: {                                   // (a tuple is a list to json.dumps)
            o += '[';
            bool first = true;
            for (const Json& x : v.list) {
                if (!first) o += ", ";
                first = false;
                dump_into(o, x);
            }
            o += ']';
            break;
        }
        case Json::Dict: {
            std::vector<std::string> keys;
            for (const auto& kv : v.dict.items()) keys.push_back(std::get<0>(kv).utf8());   // (UTF-8 order is code point order)
            std::sort(keys.begin(), keys.end());
            o += '{';
            bool first = true;
            for (const std::string& k : keys) {
                if (!first) o += ", ";
                first = false;
                py::json::put(o, py::str(k));
                o += ": ";
                dump_into(o, v.dict.at(py::str(k)));
            }
            o += '}';
            break;
        }
    }
}

// ---- json.loads ------------------------------------------------------------------------------------
class Parser {
  public:
    explicit Parser(const std::string& t) : t_(t) {}

    Json document() {
        ws();
        Json v = value();
        ws();
        if (p_ != t_.size()) fail("Extra data");
        return v;
    }

  private:
    const std::string& t_;
    size_t p_ = 0;

    [[noreturn]] void fail(const std::string& what) const { throw py::ValueError(what + ": char " + std::to_string(p_)); }
    void ws() {
        while (p_ < t_.size() && (t_[p_] == ' ' || t_[p_] == '\t' || t_[p_] == '\n' || t_[p_] == '\r')) p_++;
    }
    bool word(const char* w) {
        const size_t n = std::char_traits<char>::length(w);
        if (t_.compare(p_, n, w) == 0) {
            p_ += n;
            return true;
        }
        return false;
    }

    Json value() {
        if (p_ >= t_.size()) fail("Expecting value");
        const char c = t_[p_];
        Json v;
        if (c == '{') return object();
        if (c == '[') return array();
        if (c == '"') {
            v.kind = Json::Str;
            v.s = py::str(string());
            return v;
        }
        if (word("null")) return v;
        if (word("true")) {
            v.kind = Json::Bool;
            v.b = true;
            return v;
        }
        if (word("false")) {
            v.kind = Json::Bool;
            return v;
        }
        if (word("NaN")) {
            v.kind = Json::Float;
            v.d = std::nan("");
            return v;
        }
        if (word("Infinity")) {
            v.kind = Json::Float;
            v.d = HUGE_VAL;
            return v;
        }
        if (word("-Infinity")) {
            v.kind = Json::Float;
            v.d = -HUGE_VAL;
            return v;
        }
        return number();
    }

    Json number() {
        const size_t start = p_;
        if (p_ < t_.size() && t_[p_] == '-') p_++;
        if (p_ >= t_.size() || !std::isdigit(static_cast<unsigned char>(t_[p_]))) {
            p_ = start;
            fail("Expecting value");
        }
        if (t_[p_] == '0') p_++;
        else while (p_ < t_.size() && std::isdigit(static_cast<unsigned char>(t_[p_]))) p_++;
        bool is_float = false;
        if (p_ + 1 < t_.size() && t_[p_] == '.' && std::isdigit(static_cast<unsigned char>(t_[p_ + 1]))) {
            is_float = true;
            p_++;
            while (p_ < t_.size() && std::isdigit(static_cast<unsigned char>(t_[p_]))) p_++;
        }
        if (p_ < t_.size() && (t_[p_] == 'e' || t_[p_] == 'E')) {
            size_t q = p_ + 1;
            if (q < t_.size() && (t_[q] == '+' || t_[q] == '-')) q++;
            if (q < t_.size() && std::isdigit(static_cast<unsigned char>(t_[q]))) {
                is_float = true;
                while (q < t_.size() && std::isdigit(static_cast<unsigned char>(t_[q]))) q++;
                p_ = q;
            }
        }
        const std::string text = t_.substr(start, p_ - start);
        Json v;
        if (is_float) {
            v.kind = Json::Float;
            v.d = std::strtod(text.c_str(), nullptr);
        } else {
            v.kind = Json::Int;
            errno = 0;
            v.i = std::strtoll(text.c_str(), nullptr, 10);
            if (errno == ERANGE) throw py::OverflowError("integer does not fit int64_t");
        }
        return v;
    }

    static void utf8(std::string& o, uint32_t cp) {
        if (cp < 0x80) o += static_cast<char>(cp);
        else if (cp < 0x800) {
            o += static_cast<char>(0xC0 | (cp >> 6));
            o += static_cast<char>(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            o += static_cast<char>(0xE0 | (cp >> 12));
            o += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            o += static_cast<char>(0x80 | (cp & 0x3F));
        } else {
            o += static_cast<char>(0xF0 | (cp >> 18));
            o += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
            o += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            o += static_cast<char>(0x80 | (cp & 0x3F));
        }
    }

    uint32_t hex4() {
        if (p_ + 4 > t_.size()) fail("Invalid \\uXXXX escape");
        uint32_t v = 0;
        for (int i = 0; i < 4; i++) {
            const char c = t_[p_ + i];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= static_cast<uint32_t>(c - '0');
            else if (c >= 'a' && c <= 'f') v |= static_cast<uint32_t>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') v |= static_cast<uint32_t>(c - 'A' + 10);
            else fail("Invalid \\uXXXX escape");
        }
        p_ += 4;
        return v;
    }

    std::string string() {
        p_++;   // the opening quote
        std::string o;
        for (;;) {
            if (p_ >= t_.size()) fail("Unterminated string starting at");
            const char c = t_[p_++];
            if (c == '"') return o;
            if (c != '\\') {
                if (static_cast<unsigned char>(c) < 0x20) fail("Invalid control character at");
                o += c;
                continue;
            }
            if (p_ >= t_.size()) fail("Unterminated string starting at");
            const char e = t_[p_++];
            switch (e) {
                case '"': o += '"'; break;
                case '\\': o += '\\'; break;
                case '/': o += '/'; break;
                case 'b': o += '\b'; break;
                case 'f': o += '\f'; break;
                case 'n': o += '\n'; break;
                case 'r': o += '\r'; break;
                case 't': o += '\t'; break;
                case 'u': {
                    uint32_t cp = hex4();
                    if (cp >= 0xD800 && cp < 0xDC00 && t_.compare(p_, 2, "\\u") == 0) {
                        const size_t save = p_;
                        p_ += 2;
                        const uint32_t lo = hex4();
                        if (lo >= 0xDC00 && lo < 0xE000) cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                        else p_ = save;
                    }
                    utf8(o, cp);
                    break;
                }
                default: fail("Invalid \\escape");
            }
        }
    }

    Json array() {
        p_++;
        Json v;
        v.kind = Json::List;
        ws();
        if (p_ < t_.size() && t_[p_] == ']') {
            p_++;
            return v;
        }
        for (;;) {
            ws();
            v.list.push_back(value());
            ws();
            if (p_ < t_.size() && t_[p_] == ',') {
                p_++;
                continue;
            }
            if (p_ < t_.size() && t_[p_] == ']') {
                p_++;
                return v;
            }
            fail("Expecting ',' delimiter");
        }
    }

    Json object() {
        p_++;
        Json v;
        v.kind = Json::Dict;
        ws();
        if (p_ < t_.size() && t_[p_] == '}') {
            p_++;
            return v;
        }
        for (;;) {
            ws();
            if (p_ >= t_.size() || t_[p_] != '"') fail("Expecting property name enclosed in double quotes");
            const py::str k(string());
            ws();
            if (p_ >= t_.size() || t_[p_] != ':') fail("Expecting ':' delimiter");
            p_++;
            ws();
            v.dict.set(k, value());
            ws();
            if (p_ < t_.size() && t_[p_] == ',') {
                p_++;
                continue;
            }
            if (p_ < t_.size() && t_[p_] == '}') {
                p_++;
                return v;
            }
            fail("Expecting ',' delimiter");
        }
    }
};

}  // namespace

py::str dumps(const Json& value) {
    std::string o;
    dump_into(o, value);
    return py::str(o);
}

Json loads(const py::str& text) {
    const std::string t = text.utf8();
    return Parser(t).document();
}

py::str key(const py::str& kind, const std::optional<int64_t>& va) {
    if (!va) return py::str("");
    if (kind == py::str("ne")) {
        const decompile::describe::SegOff w = decompile::describe::where(*va);
        return py::percent(py::str("%d:%04x"), std::get<0>(w), std::get<1>(w));
    }
    return py::percent(py::str("%x"), *va);
}

py::str key(const image::Image& img, const std::optional<int64_t>& va) { return key(img.kind, va); }

FactStore::FactStore(const std::optional<py::str>& p) {
    path = (p && !p->empty()) ? *p : store::functions::default_path();
    db = base::cache::Db::connect(path, 60.0);
    db->execute("PRAGMA journal_mode=WAL");
    db->execute("PRAGMA synchronous=NORMAL");
    for (const char* stmt : SCHEMA) db->execute(stmt);
    db->commit();
}

void FactStore::close() { db->close(); }

void FactStore::put(const py::str& module, const py::str& analysis, const py::list<Row>& rows) {
    const double now = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
    // `with self.db:` -- one transaction: the old facts out, the new in, committed (rolled back if it fails)
    try {
        db->execute("DELETE FROM fact WHERE module=? AND analysis=?", {module, analysis});
        std::vector<std::vector<base::cache::SqlParam>> params;
        for (const Row& r : rows) {
            params.push_back({module, analysis, std::get<0>(r), std::get<1>(r), dumps(std::get<2>(r)), now});
        }
        db->executemany("INSERT INTO fact(module, analysis, addr, kind, value, ts) VALUES(?,?,?,?,?,?)", params);
        db->commit();
    } catch (...) {
        try {
            db->execute("ROLLBACK");
        } catch (...) {
        }
        throw;
    }
}

py::list<Fact> FactStore::get(const py::str& module, const std::optional<py::str>& analysis, const std::optional<py::str>& addr,
                              const std::optional<py::str>& kind) {
    std::string q = "SELECT analysis, addr, kind, value FROM fact WHERE module=?";
    std::vector<base::cache::SqlParam> args{module};
    const std::pair<const char*, const std::optional<py::str>*> filters[] = {{"analysis", &analysis}, {"addr", &addr}, {"kind", &kind}};
    for (const auto& f : filters) {
        if (*f.second) {
            q += std::string(" AND ") + f.first + "=?";
            args.push_back(**f.second);
        }
    }
    py::list<Fact> out;
    for (const base::cache::SqlRow& r : db->query(q, args)) {
        out.append(Fact{std::get<py::str>(r[0]), std::get<py::str>(r[1]), std::get<py::str>(r[2]), loads(std::get<py::str>(r[3]))});
    }
    return out;
}

py::dict<py::str, py::dict<py::str, int64_t>> FactStore::tally(const py::str& module) {
    py::dict<py::str, py::dict<py::str, int64_t>> out;
    for (const base::cache::SqlRow& r : db->query("SELECT analysis, kind, COUNT(*) FROM fact WHERE module=? GROUP BY analysis, kind", {module})) {
        const py::str& a = std::get<py::str>(r[0]);
        if (!out.contains(a)) out.set(a, py::dict<py::str, int64_t>());
        out.at(a).set(std::get<py::str>(r[1]), std::get<int64_t>(r[2]));
    }
    return out;
}

Json _plain(const py::set<int64_t>& x) {
    std::vector<std::pair<std::string, int64_t>> items;   // sorted(x, key=repr)
    for (int64_t v : x) items.push_back({py::repr(v).utf8(), v});
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    Json out;
    out.kind = Json::List;
    for (const auto& it : items) {
        Json e;
        e.kind = Json::Int;
        e.i = it.second;
        out.list.push_back(e);
    }
    return out;
}

Json _plain(const py::set<py::str>& x) {
    std::vector<std::pair<std::string, py::str>> items;
    for (const py::str& v : x) items.push_back({py::repr(v).utf8(), v});
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    Json out;
    out.kind = Json::List;
    for (const auto& it : items) {
        Json e;
        e.kind = Json::Str;
        e.s = it.second;
        out.list.push_back(e);
    }
    return out;
}

Json _plain(const py::str& str_of_x) {
    Json out;
    out.kind = Json::Str;
    out.s = str_of_x;
    return out;
}

std::shared_ptr<FactStore> open(const std::optional<py::str>& path) { return std::make_shared<FactStore>(path); }

}  // namespace codebreaker::analyze::facts
