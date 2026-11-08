#include "json.hpp"

#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>
#include <mutex>
#include <vector>

#include "pystr.hpp"

namespace py {

namespace {

// Large flat values the shim hands over once and then by number ({"$keep": n, "$v": value}, then
// {"$big": n}): a module's sets each of its functions is written with. Each use gets its own copy.
std::mutex kept_mu;
std::vector<std::pair<int64_t, V>> kept_values;           // the latest few, newest last

V copy_flat(const V& v) {
    if (v.is_dict()) return copy_dict(v);
    if (v.is_list() || v.is_set() || v.is_tuple()) {
        V out = v.is_dict() ? V::dict() : V::list();
        out.k = v.k;
        out.seq() = v.seq();
        return out;
    }
    return v;
}

V keep(const V& d) {
    int64_t n = d.items()[0].second.as_int();
    V v = d.items().size() == 2 ? d.items()[1].second : V();
    std::lock_guard<std::mutex> lock(kept_mu);
    kept_values.push_back({n, v});
    if (kept_values.size() > 64) kept_values.erase(kept_values.begin());
    return copy_flat(v);
}

V kept(int64_t n) {
    std::lock_guard<std::mutex> lock(kept_mu);
    for (auto& kv : kept_values)
        if (kv.first == n) return copy_flat(kv.second);
    raise("RuntimeError", "native/l2: value " + std::to_string(n) + " no longer kept");
}

struct Parser {
    const std::string& s;
    size_t p = 0;
    bool tagged;
    Parser(const std::string& t, bool tg) : s(t), tagged(tg) {}

    [[noreturn]] void fail(const char* what) { raise("ValueError", std::string(what) + " at char " + std::to_string(p)); }
    void ws() {
        while (p < s.size() && (s[p] == ' ' || s[p] == '\t' || s[p] == '\n' || s[p] == '\r')) p++;
    }
    static void put_cp(std::string& out, uint32_t c) {
        if (c < 0x80) out += (char)c;
        else if (c < 0x800) {
            out += (char)(0xc0 | (c >> 6));
            out += (char)(0x80 | (c & 0x3f));
        } else if (c < 0x10000) {
            out += (char)(0xe0 | (c >> 12));
            out += (char)(0x80 | ((c >> 6) & 0x3f));
            out += (char)(0x80 | (c & 0x3f));
        } else {
            out += (char)(0xf0 | (c >> 18));
            out += (char)(0x80 | ((c >> 12) & 0x3f));
            out += (char)(0x80 | ((c >> 6) & 0x3f));
            out += (char)(0x80 | (c & 0x3f));
        }
    }
    uint32_t hex4() {
        if (p + 4 > s.size()) fail("Invalid \\uXXXX escape");
        uint32_t v = 0;
        for (int k = 0; k < 4; k++) {
            char c = s[p++];
            int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
            if (d < 0) fail("Invalid \\uXXXX escape");
            v = v * 16 + (uint32_t)d;
        }
        return v;
    }
    std::vector<V> stack;
    V keys_seen[256];   // object keys are mostly the same few strings: one string value is shared by all that repeat
    // an object key as a string value (a plain one is looked for among the keys seen before, with no copy)
    V key() {
        size_t q = p + 1;
        const size_t n = s.size();
        uint32_t h = 2166136261u;
        while (q < n) {
            unsigned char u = (unsigned char)s[q];
            if (u == '"' || u == '\\' || u < 0x20) break;
            h = (h ^ u) * 16777619u;
            q++;
        }
        if (q >= n || s[q] != '"' || q - p - 1 > 40) return V(string());
        const char* b = s.data() + p + 1;
        const size_t len = q - p - 1;
        V& slot = keys_seen[h & 255];
        p = q + 1;
        if (slot.k == K::Str && slot.str().size() == len && std::memcmp(slot.str().data(), b, len) == 0) return slot;
        slot = V(std::string(b, len));
        return slot;
    }
    std::string string() {
        p++;  // "
        std::string out;
        while (true) {
            // the run of plain characters up to the next quote, backslash or control character: copied at once
            size_t q = p;
            const size_t n = s.size();
            while (q < n) {
                unsigned char u = (unsigned char)s[q];
                if (u == '"' || u == '\\' || u < 0x20) break;
                q++;
            }
            if (q > p) {
                out.append(s, p, q - p);
                p = q;
            }
            if (p >= s.size()) fail("Unterminated string");
            char c = s[p];
            if (c == '"') {
                p++;
                return out;
            }
            if (c == '\\') {
                p++;
                if (p >= s.size()) fail("Unterminated string");
                char e = s[p++];
                switch (e) {
                    case '"': out += '"'; break;
                    case '\\': out += '\\'; break;
                    case '/': out += '/'; break;
                    case 'b': out += '\b'; break;
                    case 'f': out += '\f'; break;
                    case 'n': out += '\n'; break;
                    case 'r': out += '\r'; break;
                    case 't': out += '\t'; break;
                    case 'u': {
                        uint32_t c1 = hex4();
                        if (c1 >= 0xd800 && c1 < 0xdc00 && p + 1 < s.size() && s[p] == '\\' && s[p + 1] == 'u') {
                            size_t save = p;
                            p += 2;
                            uint32_t c2 = hex4();
                            if (c2 >= 0xdc00 && c2 < 0xe000) {
                                put_cp(out, 0x10000 + ((c1 - 0xd800) << 10) + (c2 - 0xdc00));
                                break;
                            }
                            p = save;
                        }
                        put_cp(out, c1);   // (a lone surrogate kept as its 3 bytes, as Python keeps it)
                        break;
                    }
                    default: fail("Invalid \\escape");
                }
                continue;
            }
            fail("Invalid control character");
        }
    }
    V number() {
        size_t start = p;
        if (s[p] == '-') p++;
        if (s.compare(p, 8, "Infinity") == 0) {
            p += 8;
            return V(s[start] == '-' ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity());
        }
        while (p < s.size() && s[p] >= '0' && s[p] <= '9') p++;
        bool frac = false;
        if (p < s.size() && s[p] == '.') {
            frac = true;
            p++;
            while (p < s.size() && s[p] >= '0' && s[p] <= '9') p++;
        }
        if (p < s.size() && (s[p] == 'e' || s[p] == 'E')) {
            frac = true;
            p++;
            if (p < s.size() && (s[p] == '+' || s[p] == '-')) p++;
            while (p < s.size() && s[p] >= '0' && s[p] <= '9') p++;
        }
        if (p == start || (p == start + 1 && s[start] == '-')) fail("Expecting value");
        if (!frac && p - start <= 18) {   // fits an int64: no text copy
            const char* d = s.data() + start;
            const bool ng = *d == '-';
            if (ng) d++;
            int64_t acc = 0;
            for (const char* e = s.data() + p; d < e; d++) acc = acc * 10 + (*d - '0');
            return V((long long)(ng ? -acc : acc));
        }
        std::string t = s.substr(start, p - start);
        if (frac) return V(std::strtod(t.c_str(), nullptr));
        bool neg = t[0] == '-';
        std::string digits = neg ? t.substr(1) : t;
        if (digits.size() > 20) raise("ValueError", "an integer too large for the native L2 core: " + t);
        unsigned long long u = std::strtoull(digits.c_str(), nullptr, 10);
        if (digits.size() == 20 && digits > "18446744073709551615") raise("ValueError", "an integer too large: " + t);
        if (neg) {
            if (u > (unsigned long long)INT64_MAX + 1ULL) raise("ValueError", "an integer too small: " + t);
            return V((long long)(0 - u));
        }
        return V(u);
    }
    V value() {
        ws();
        if (p >= s.size()) fail("Expecting value");
        char c = s[p];
        if (c == '{') {
            p++;
            ws();
            if (p < s.size() && s[p] == '}') {
                p++;
                return untag(V::dict());
            }
            const size_t base = stack.size();   // the keys and values so far, alternating (children use the stack above)
            while (true) {
                ws();
                if (p >= s.size() || s[p] != '"') fail("Expecting property name enclosed in double quotes");
                V k = key();
                ws();
                if (p >= s.size() || s[p] != ':') fail("Expecting ':' delimiter");
                p++;
                V v = value();
                stack.push_back(std::move(k));
                stack.push_back(std::move(v));
                ws();
                if (p < s.size() && s[p] == ',') {
                    p++;
                    continue;
                }
                if (p < s.size() && s[p] == '}') {
                    p++;
                    V d = V::dict();
                    d.dict_().items.reserve((stack.size() - base) / 2);
                    for (size_t n = base; n < stack.size(); n += 2)
                        d.put(std::move(stack[n]), std::move(stack[n + 1]));   // (a key twice: its first place, its last value)
                    stack.resize(base);
                    return untag(std::move(d));
                }
                fail("Expecting ',' delimiter");
            }
        }
        if (c == '[') {
            p++;
            ws();
            if (p < s.size() && s[p] == ']') {
                p++;
                return V::list();
            }
            const size_t base = stack.size();
            while (true) {
                V v = value();
                stack.push_back(std::move(v));
                ws();
                if (p < s.size() && s[p] == ',') {
                    p++;
                    continue;
                }
                if (p < s.size() && s[p] == ']') {
                    p++;
                    std::vector<V> items(std::make_move_iterator(stack.begin() + base), std::make_move_iterator(stack.end()));
                    stack.resize(base);
                    return V::list(std::move(items));
                }
                fail("Expecting ',' delimiter");
            }
        }
        if (c == '"') return V(string());
        if (s.compare(p, 4, "true") == 0) {
            p += 4;
            return V(true);
        }
        if (s.compare(p, 5, "false") == 0) {
            p += 5;
            return V(false);
        }
        if (s.compare(p, 4, "null") == 0) {
            p += 4;
            return V();
        }
        if (s.compare(p, 3, "NaN") == 0) {
            p += 3;
            return V(std::numeric_limits<double>::quiet_NaN());
        }
        if (c == '-' || (c >= '0' && c <= '9') || c == 'I') return number();
        fail("Expecting value");
    }
    V untag(V d) {
        if (!tagged) return d;
        if (tagged && (d.items().size() == 2 || d.items().size() == 1) && d.items()[0].first.str() == "$keep") return keep(d);
        if (tagged && d.items().size() == 1 && d.items()[0].first.str() == "$big") return kept(d.items()[0].second.as_int());
        if (!tagged || d.items().size() != 1) return d;
        auto& kv = d.items()[0];
        const std::string& k = kv.first.str();
        if (k.size() != 2 || k[0] != '$') return d;
        if (k == "$t") {
            V t = kv.second;
            t.k = K::Tuple;
            return t;
        }
        if (k == "$s") return V::set(kv.second.seq());
        if (k == "$d") {
            V out = V::dict();
            for (auto& pair : kv.second.seq()) out.set(pair.seq()[0], pair.seq()[1]);
            return out;
        }
        if (k == "$f") {
            const std::string& f = kv.second.str();
            if (f == "nan") return V(std::numeric_limits<double>::quiet_NaN());
            return V(f[0] == '-' ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity());
        }
        return d;
    }
};

void dump_str(std::string& out, const std::string& s, bool ascii) {
    out += '"';
    size_t pos = 0;
    while (pos < s.size()) {
        {   // a run of characters written as they are is appended at once
            size_t q = pos;
            const size_t n = s.size();
            while (q < n) {
                unsigned char u = (unsigned char)s[q];
                if (u < 0x20 || u == '"' || u == '\\' || (u >= 0x7f && (ascii || u == 0x7f))) break;
                q++;
            }
            if (q > pos) {
                out.append(s, pos, q - pos);
                pos = q;
                if (pos >= n) break;
            }
        }
        unsigned char c = (unsigned char)s[pos];
        if (c < 0x80) {
            pos++;
            switch (c) {
                case '"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
                case '\b': out += "\\b"; break;
                case '\f': out += "\\f"; break;
                default:
                    if (c < 0x20 || (ascii && c == 0x7f)) {   // (ensure_ascii escapes DEL too, as json.dumps)
                        char b[8];
                        std::snprintf(b, sizeof b, "\\u%04x", c);
                        out += b;
                    } else
                        out += (char)c;
            }
            continue;
        }
        size_t start = pos;
        uint32_t cp = decode_cp(s, pos);
        if (!ascii) {
            out.append(s, start, pos - start);
            continue;
        }
        char b[16];
        if (cp >= 0x10000) {
            uint32_t v = cp - 0x10000;
            std::snprintf(b, sizeof b, "\\u%04x\\u%04x", 0xd800 + (v >> 10), 0xdc00 + (v & 0x3ff));
        } else
            std::snprintf(b, sizeof b, "\\u%04x", cp);
        out += b;
    }
    out += '"';
}

// repr(float) of a finite float, written without the temporaries float_repr makes (same text)
void append_float(std::string& out, double f) {
    if (f == 0) {
        out += std::signbit(f) ? "-0.0" : "0.0";
        return;
    }
    char buf[40];
    auto r = std::to_chars(buf, buf + sizeof buf, f, std::chars_format::scientific);   // [-]d[.ddd]e[+-]xx
    const char* p = buf;
    const char* end = r.ptr;
    if (*p == '-') {
        out += '-';
        p++;
    }
    char digits[24];
    int nd = 0;
    while (p < end && *p != 'e') {
        if (*p != '.') digits[nd++] = *p;
        p++;
    }
    int exp = 0;
    if (p < end) std::from_chars(p + 1 + (p[1] == '+' ? 1 : 0), end, exp);
    while (nd > 1 && digits[nd - 1] == '0') nd--;
    if (exp >= -4 && exp < 16) {
        int point = exp + 1;   // digits before the point
        if (point <= 0) {
            out += "0.";
            out.append((size_t)(-point), '0');
            out.append(digits, (size_t)nd);
        } else if (nd <= point) {
            out.append(digits, (size_t)nd);
            out.append((size_t)(point - nd), '0');
            out += ".0";
        } else {
            out.append(digits, (size_t)point);
            out += '.';
            out.append(digits + point, (size_t)(nd - point));
        }
        return;
    }
    out += digits[0];
    if (nd > 1) {
        out += '.';
        out.append(digits + 1, (size_t)(nd - 1));
    }
    char eb[16];
    std::snprintf(eb, sizeof eb, "e%c%02d", exp < 0 ? '-' : '+', exp < 0 ? -exp : exp);
    out += eb;
}

void dump(std::string& out, const V& v, const DumpOpts& o) {
    switch (v.k) {
        case K::None: out += "null"; return;
        case K::Bool: out += v.b ? "true" : "false"; return;
        case K::Int: {
            char b[24];
            auto r = v.u64 ? std::to_chars(b, b + sizeof b, (uint64_t)v.i) : std::to_chars(b, b + sizeof b, v.i);
            out.append(b, (size_t)(r.ptr - b));
            return;
        }
        case K::Float:
            if (std::isnan(v.f)) out += o.tagged ? "{\"$f\":\"nan\"}" : "NaN";
            else if (std::isinf(v.f)) out += o.tagged ? (v.f < 0 ? "{\"$f\":\"-inf\"}" : "{\"$f\":\"inf\"}") : (v.f < 0 ? "-Infinity" : "Infinity");
            else append_float(out, v.f);
            return;
        case K::Str: dump_str(out, v.str(), o.ensure_ascii); return;
        case K::List: case K::Tuple: case K::Set: {
            const char* tag = !o.tagged || v.k == K::List ? nullptr : v.k == K::Tuple ? "{\"$t\":" : "{\"$s\":";
            if (tag) out += tag;
            out += '[';
            bool first = true;
            for (auto& x : v.seq()) {
                if (!first) out += o.item_sep;
                first = false;
                dump(out, x, o);
            }
            out += ']';
            if (tag) out += '}';
            return;
        }
        case K::Dict: {
            bool plain = true;
            for (auto& kv : v.items())
                if (!kv.first.is_str() || (o.tagged && !kv.first.str().empty() && kv.first.str()[0] == '$')) plain = false;
            if (plain || !o.tagged) {
                out += '{';
                bool first = true;
                for (auto& kv : v.items()) {
                    if (!first) out += o.item_sep;
                    first = false;
                    if (kv.first.is_str()) dump_str(out, kv.first.str(), o.ensure_ascii);
                    else {
                        // (json.dumps writes a number or bool key as its text, None as "null")
                        std::string k;
                        dump(k, kv.first, o);
                        if (kv.first.is_none()) k = "null";
                        dump_str(out, k, o.ensure_ascii);
                    }
                    out += o.key_sep;
                    dump(out, kv.second, o);
                }
                out += '}';
                return;
            }
            out += "{\"$d\":[";
            bool first = true;
            for (auto& kv : v.items()) {
                if (!first) out += ',';
                first = false;
                out += '[';
                dump(out, kv.first, o);
                out += ',';
                dump(out, kv.second, o);
                out += ']';
            }
            out += "]}";
            return;
        }
    }
}

}  // namespace

V json_parse(const std::string& text, bool tagged) {
    Parser ps(text, tagged);
    V v = ps.value();
    ps.ws();
    if (ps.p != text.size()) ps.fail("Extra data");
    return v;
}

std::string json_dump(const V& v, const DumpOpts& o) {
    std::string out;
    dump(out, v, o);
    return out;
}

}  // namespace py
