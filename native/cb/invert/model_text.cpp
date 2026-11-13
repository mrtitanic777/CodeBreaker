#include "cb/invert/model_text.hpp"

#include <unordered_map>

namespace cb::invert::model {

namespace {
constexpr std::string_view kModel = "invert";
}

void fail(const std::string& what) { throw FormatError(kModel, 0, what); }
void unsupported(const std::string& what) { throw UnsupportedError(kModel, 0, what); }
void broken(const std::string& what) { throw InternalError("invert model: " + what); }

// ---- text --------------------------------------------------------------------------------------------------------------------------------

std::string replace_all(std::string s, std::string_view from, std::string_view to) {
    if (from.empty()) return s;
    std::size_t i = 0;
    while ((i = s.find(from, i)) != std::string::npos) {
        s.replace(i, from.size(), to);
        i += to.size();
    }
    return s;
}

std::vector<std::string> split(const std::string& s, std::string_view sep) {
    std::vector<std::string> out;
    std::size_t from = 0;
    for (;;) {
        const std::size_t at = s.find(sep, from);
        if (at == std::string::npos) {
            out.push_back(s.substr(from));
            return out;
        }
        out.push_back(s.substr(from, at - from));
        from = at + sep.size();
    }
}

std::vector<std::string> split_at_most(const std::string& s, std::string_view sep, int max_splits) {
    std::vector<std::string> out;
    std::size_t from = 0;
    while (static_cast<int>(out.size()) < max_splits) {
        const std::size_t at = s.find(sep, from);
        if (at == std::string::npos) break;
        out.push_back(s.substr(from, at - from));
        from = at + sep.size();
    }
    out.push_back(s.substr(from));
    return out;
}

std::vector<std::string> split_words(const std::string& s) {
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && is_space_char(s[i])) ++i;
        if (i >= s.size()) break;
        std::size_t j = i;
        while (j < s.size() && !is_space_char(s[j])) ++j;
        out.push_back(s.substr(i, j - i));
        i = j;
    }
    return out;
}

std::pair<std::string, std::string> partition(const std::string& s, std::string_view sep) {
    const std::size_t at = s.find(sep);
    if (at == std::string::npos) return {s, ""};
    return {s.substr(0, at), s.substr(at + sep.size())};
}

std::pair<std::string, std::string> split_pair(const std::string& s, std::string_view sep) {
    std::vector<std::string> parts = split(s, sep);
    if (parts.size() != 2) fail("an operand that is not two parts: " + s);
    return {parts[0], parts[1]};
}

std::pair<std::string, std::string> rpartition(const std::string& s, std::string_view sep) {
    const std::size_t at = s.rfind(sep);
    if (at == std::string::npos) return {"", s};
    return {s.substr(0, at), s.substr(at + sep.size())};
}

std::string strip(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && is_space_char(s[a])) ++a;
    while (b > a && is_space_char(s[b - 1])) --b;
    return s.substr(a, b - a);
}

std::string rstrip(const std::string& s) {
    std::size_t b = s.size();
    while (b > 0 && is_space_char(s[b - 1])) --b;
    return s.substr(0, b);
}

std::string lstrip_blanks(const std::string& s) {
    std::size_t a = 0;
    while (a < s.size() && s[a] == ' ') ++a;
    return s.substr(a);
}

long long indent_of(const std::string& s) { return static_cast<long long>(s.size() - lstrip_blanks(s).size()); }

std::string slice(const std::string& s, long long from) {
    const long long n = static_cast<long long>(s.size());
    if (from < 0) from = std::max(0LL, from + n);
    return from >= n ? std::string() : s.substr(static_cast<std::size_t>(from));
}

std::string slice(const std::string& s, long long from, long long to) {
    const long long n = static_cast<long long>(s.size());
    if (from < 0) from = std::max(0LL, from + n);
    if (to < 0) to = std::max(0LL, to + n);
    if (to > n) to = n;
    return from >= to ? std::string() : s.substr(static_cast<std::size_t>(from), static_cast<std::size_t>(to - from));
}

long long count_of(const std::string& s, std::string_view part) {
    long long n = 0;
    for (std::size_t i = 0; (i = s.find(part, i)) != std::string::npos; i += part.size()) ++n;
    return n;
}

std::string join(const std::vector<std::string>& v, std::string_view sep) {
    std::string out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) out += sep;
        out += v[i];
    }
    return out;
}

std::string spaces(long long n) { return std::string(static_cast<std::size_t>(std::max(0LL, n)), ' '); }

bool is_number(const std::string& s) {
    if (s.size() > 2 && s[0] == '0' && s[1] == 'x' && std::all_of(s.begin() + 2, s.end(), is_hex_char)) return true;
    return is_digits(s);
}

namespace {
[[noreturn]] void bad_number(const std::string& s) { fail("not a number: '" + s + "'"); }
}

long long parse_number(const std::string& s) {
    std::size_t i = 0;
    bool negative = false;
    if (i < s.size() && (s[i] == '-' || s[i] == '+')) negative = s[i++] == '-';
    if (i >= s.size()) bad_number(s);
    long long v = 0;
    if (i + 1 < s.size() && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
        for (i += 2; i < s.size(); ++i) {
            const char c = static_cast<char>(s[i] | 0x20);
            if (!is_hex_char(c)) bad_number(s);
            v = v * 16 + (is_digit_char(c) ? c - '0' : c - 'a' + 10);
        }
        if (s.size() == (negative || s[0] == '+' ? 3u : 2u)) bad_number(s);
    } else {
        const bool leading_zero = s[i] == '0' && i + 1 < s.size();
        for (std::size_t j = i; j < s.size(); ++j) {
            if (!is_digit_char(s[j]) || (leading_zero && s[j] != '0')) bad_number(s);
            v = v * 10 + (s[j] - '0');
        }
    }
    return negative ? -v : v;
}

std::optional<long long> try_parse_hex(const std::string& s0) {
    const std::string s = strip(s0);
    std::size_t i = 0;
    bool negative = false;
    if (i < s.size() && (s[i] == '-' || s[i] == '+')) negative = s[i++] == '-';
    if (i + 1 < s.size() && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) i += 2;
    if (i >= s.size()) return std::nullopt;
    long long v = 0;
    for (; i < s.size(); ++i) {
        const char c = static_cast<char>(s[i] | 0x20);
        if (!is_hex_char(c)) return std::nullopt;
        v = v * 16 + (is_digit_char(c) ? c - '0' : c - 'a' + 10);
    }
    return negative ? -v : v;
}

long long parse_hex(const std::string& s) {
    const auto v = try_parse_hex(s);
    if (!v) bad_number(s);
    return *v;
}

std::string hex(long long v) {
    static constexpr char kDigits[] = "0123456789abcdef";
    const bool negative = v < 0;
    unsigned long long m = negative ? 0ULL - static_cast<unsigned long long>(v) : static_cast<unsigned long long>(v);
    std::string out;
    do {
        out.insert(out.begin(), kDigits[m & 15]);
        m >>= 4;
    } while (m);
    if (negative) out.insert(out.begin(), '-');
    return out;
}

std::string decimal(long long v) { return std::to_string(v); }

std::string utf8_char(long long cp) {
    if (cp < 0 || cp > 0x10ffff) fail("a character past U+10FFFF");
    std::string out;
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xc0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3f));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xe0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3f));
        out += static_cast<char>(0x80 | (cp & 0x3f));
    } else {
        out += static_cast<char>(0xf0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3f));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3f));
        out += static_cast<char>(0x80 | (cp & 0x3f));
    }
    return out;
}

long long floor_quotient(long long a, long long b) {
    long long q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

// ---- Value ----------------------------------------------------------------------------------------------------------------------------

Value Value::intern(const std::string& s) {
    thread_local std::unordered_map<std::string, std::shared_ptr<const std::string>> table;
    auto it = table.find(s);
    if (it == table.end()) it = table.emplace(s, std::make_shared<const std::string>(s)).first;
    return Value(it->second);
}

Value Value::make(const std::string& s) { return s.size() <= 1 ? intern(s) : Value(std::make_shared<const std::string>(s)); }

Value Value::from_int(long long v) { return Value(std::make_shared<const std::string>(std::to_string(v))); }

// ---- Rx -------------------------------------------------------------------------------------------------------------------------------

std::string Hit::group(int g) const {
    if (!has(g)) return {};
    return subject_->substr(static_cast<std::size_t>(start(g)), static_cast<std::size_t>(end(g) - start(g)));
}

Rx::Rx(const std::string& pattern) : re_(std::make_shared<const Regex>(pattern)) {}

Hit Rx::wrap(const std::string& s, const std::optional<RegexMatch>& m) const {
    Hit h;
    if (!m) return h;
    h.subject_ = std::make_shared<const std::string>(s);
    for (std::size_t g = 0; g < m->group_count(); ++g) {
        h.caps_.push_back(m->has(g) ? static_cast<long>(m->start(g)) : -1L);
        h.caps_.push_back(m->has(g) ? static_cast<long>(m->end(g)) : -1L);
    }
    return h;
}

Hit Rx::match(const std::string& s) const { return wrap(s, re_->match(s)); }
Hit Rx::fullmatch(const std::string& s) const { return wrap(s, re_->full_match(s)); }
Hit Rx::search(const std::string& s) const { return wrap(s, re_->search(s)); }
bool Rx::found(const std::string& s) const { return re_->test(s); }

std::vector<Hit> Rx::all(const std::string& s) const {
    std::vector<Hit> out;
    re_->for_each_match(s, [&](const RegexMatch& m) {
        out.push_back(wrap(s, std::optional<RegexMatch>(m)));
        return true;
    });
    return out;
}

std::vector<std::string> Rx::find_all(const std::string& s) const {
    std::vector<std::string> out;
    const int group = re_->group_count() > 0 ? 1 : 0;
    re_->for_each_match(s, [&](const RegexMatch& m) {
        out.emplace_back(m.group(static_cast<std::size_t>(group)));
        return true;
    });
    return out;
}

std::string Rx::replace_all(const std::string& s, const std::string& with) const {
    return replace_each(s, [&](const Hit& h) {
        std::string out;
        for (std::size_t i = 0; i < with.size(); ++i) {
            if (with[i] == '\\' && i + 1 < with.size() && is_digit_char(with[i + 1])) {
                out += h.group(with[i + 1] - '0');
                ++i;
            } else {
                out += with[i];
            }
        }
        return out;
    });
}

const Rx& RX(const std::string& pattern) {
    thread_local std::unordered_map<std::string, std::unique_ptr<Rx>> cache;
    auto it = cache.find(pattern);
    if (it == cache.end()) it = cache.emplace(pattern, std::make_unique<Rx>(pattern)).first;
    return *it->second;
}

std::string escape(const std::string& literal) {
    std::string out;
    for (const char c : literal) {
        if (!is_word_char(c) && static_cast<unsigned char>(c) < 0x80) out += '\\';
        out += c;
    }
    return out;
}

}  // namespace cb::invert::model
