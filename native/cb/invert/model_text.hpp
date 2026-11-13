// cb/invert/model_text.hpp -- what the compiler models share: the text a model reads (Capstone's operand strings) and the text it writes (C
// expressions), and the errors a model raises.
//
// The models of Microsoft C 2.03 / 3.00 (early_c) and Microsoft C 6.0a (msc6) track what each register holds as the C expression that would
// compute it, a string. Two things about those strings are real parts of the models and live here:
//
//     Value   a register's expression, or nothing. Two Values are "the same expression" (`same_as`) when one was copied from the other, not
//             when their text is equal: the models ask whether a register still holds the very expression it was given, which two loads of the
//             same variable do not.
//     Rx      a regular expression over operand text, compiled once (RX(pattern) keeps one per pattern text), whose matches own a copy of the
//             text they matched.
//
// Errors: a function whose bytes the model cannot read throws FormatError (the bytes are not code of the modelled compiler); a shape the model
// knows and does not handle throws UnsupportedError; a broken internal expectation throws InternalError.
#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/regex.hpp"

namespace cb::invert::model {

// ---- errors -----------------------------------------------------------------------------------------------------------------------

[[noreturn]] void fail(const std::string& what);          // the bytes are not code of the modelled compiler: FormatError
[[noreturn]] void unsupported(const std::string& what);   // a shape the model recognises and does not handle: UnsupportedError
[[noreturn]] void broken(const std::string& what);        // the model's own expectation failed: InternalError

// ---- text helpers --------------------------------------------------------------------------------------------------------------------

inline bool starts(const std::string& s, std::string_view p) { return s.starts_with(p); }
inline bool ends(const std::string& s, std::string_view p) { return s.ends_with(p); }
inline bool has(const std::string& s, std::string_view p) { return s.find(p) != std::string::npos; }

inline bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
inline bool is_hex_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
inline bool is_digit_char(char c) { return c >= '0' && c <= '9'; }
inline bool is_space_char(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

std::string replace_all(std::string s, std::string_view from, std::string_view to);
std::vector<std::string> split(const std::string& s, std::string_view sep);                  // every part, the separator not included
std::vector<std::string> split_at_most(const std::string& s, std::string_view sep, int max_splits);
std::vector<std::string> split_words(const std::string& s);                                  // on runs of whitespace
std::pair<std::string, std::string> partition(const std::string& s, std::string_view sep);   // (head, tail); tail empty without the separator
std::pair<std::string, std::string> split_pair(const std::string& s, std::string_view sep);  // exactly two parts, else FormatError
std::pair<std::string, std::string> rpartition(const std::string& s, std::string_view sep);  // (head, tail); head empty without the separator
std::string strip(const std::string& s);
std::string rstrip(const std::string& s);
std::string lstrip_blanks(const std::string& s);                                             // leading spaces only
long long indent_of(const std::string& s);                                                   // leading spaces
std::string slice(const std::string& s, long long from);                                     // s[from:], negative from the end, clamped
std::string slice(const std::string& s, long long from, long long to);
// v[from:to] of a list, with the same clamping.
template <class T>
std::vector<T> slice(const std::vector<T>& v, long long from, long long to) {
    const long long n = static_cast<long long>(v.size());
    if (from < 0) from = std::max(0LL, from + n);
    if (to < 0) to = std::max(0LL, to + n);
    if (to > n) to = n;
    if (from >= to) return {};
    return std::vector<T>(v.begin() + from, v.begin() + to);
}
long long count_of(const std::string& s, std::string_view part);
std::string join(const std::vector<std::string>& v, std::string_view sep);
std::string spaces(long long n);
inline bool is_digits(const std::string& s) { return !s.empty() && std::ranges::all_of(s, is_digit_char); }
bool is_number(const std::string& s);                                                         // 0x1f or 123: a whole operand
long long parse_number(const std::string& s);                                                 // that, as a value; FormatError otherwise
long long parse_hex(const std::string& s);
std::optional<long long> try_parse_hex(const std::string& s);                                  // the same, nothing for text that is not hex                                                    // hex digits, 0x and a sign allowed; FormatError otherwise
std::string hex(long long v);                                                                 // lower-case, no prefix, '-' for a negative
std::string decimal(long long v);
std::string utf8_char(long long code_point);
long long floor_quotient(long long a, long long b);

// v[k] with a negative k from the end; FormatError past either end.
template <class T>
T& at(std::vector<T>& v, long long k) {
    const long long n = static_cast<long long>(v.size());
    if (k < 0) k += n;
    if (k < 0 || k >= n) fail("an index past the end of the code");
    return v[static_cast<std::size_t>(k)];
}
template <class T>
const T& at(const std::vector<T>& v, long long k) {
    const long long n = static_cast<long long>(v.size());
    if (k < 0) k += n;
    if (k < 0 || k >= n) fail("an index past the end of the code");
    return v[static_cast<std::size_t>(k)];
}

// ---- Value ----------------------------------------------------------------------------------------------------------------------------

class Value {
public:
    Value() = default;                                   // nothing
    static Value intern(const std::string& s);           // one Value for every use of the same text (a name the model writes itself)
    static Value make(const std::string& s);             // a new expression (one character long: the interned one)
    static Value from_int(long long v);
    bool empty() const { return !p_; }
    const std::string& s() const { return *p_; }                 // the text; the Value must not be empty
    std::string text() const { return p_ ? *p_ : "None"; }       // the text, "None" for nothing (what a statement says of an unset register)
    bool same_as(const Value& o) const { return p_ == o.p_; }    // the very expression, not an equal one
    bool equals(const Value& o) const { return p_ && o.p_ ? *p_ == *o.p_ : (!p_ && !o.p_); }
    bool nonblank() const { return p_ && !p_->empty(); }
    bool is(std::string_view t) const { return p_ && *p_ == t; }

private:
    explicit Value(std::shared_ptr<const std::string> p) : p_(std::move(p)) {}
    std::shared_ptr<const std::string> p_;
};
inline std::string text_of(const Value& v) { return v.text(); }
inline Value or_else(const Value& v, std::string_view fallback) { return v.nonblank() ? v : Value::intern(std::string(fallback)); }

// ---- Rx -------------------------------------------------------------------------------------------------------------------------------

// One match: the text it matched is owned by the Hit.
class Hit {
public:
    Hit() = default;
    explicit operator bool() const { return subject_ != nullptr; }
    bool has(int group) const { return subject_ && static_cast<std::size_t>(group) * 2 < caps_.size() && caps_[static_cast<std::size_t>(group) * 2] >= 0; }
    std::string group(int g = 0) const;                 // empty where the group took no part
    Value value(int g) const { return has(g) ? Value::make(group(g)) : Value(); }
    long start(int g = 0) const { return caps_[static_cast<std::size_t>(g) * 2]; }
    long end(int g = 0) const { return caps_[static_cast<std::size_t>(g) * 2 + 1]; }

private:
    friend class Rx;
    std::shared_ptr<const std::string> subject_;
    std::vector<long> caps_;
};

class Rx {
public:
    explicit Rx(const std::string& pattern);
    Hit match(const std::string& s) const;               // anchored at the start
    Hit fullmatch(const std::string& s) const;
    Hit search(const std::string& s) const;
    bool found(const std::string& s) const;
    std::vector<Hit> all(const std::string& s) const;    // every non-overlapping match
    std::vector<std::string> find_all(const std::string& s) const;   // each match's group 1 if the pattern has a group, else the whole match
    std::string replace_all(const std::string& s, const std::string& with) const;   // `\1`..`\9` in `with` are those groups
    template <class F>
    std::string replace_each(const std::string& s, F&& make) const {
        std::string out;
        long last = 0;
        for (const Hit& h : all(s)) {
            out.append(s, static_cast<std::size_t>(last), static_cast<std::size_t>(h.start() - last));
            out += make(h);
            last = h.end();
        }
        out.append(s, static_cast<std::size_t>(last), std::string::npos);
        return out;
    }

private:
    Hit wrap(const std::string& s, const std::optional<RegexMatch>& m) const;
    std::shared_ptr<const Regex> re_;
};

// The pattern text compiled once per thread and kept: RX("...").search(s).
const Rx& RX(const std::string& pattern);
inline Hit mt(const std::string& pattern, const std::string& s) { return RX(pattern).match(s); }
inline Hit sr(const std::string& pattern, const std::string& s) { return RX(pattern).search(s); }
inline Hit fm(const std::string& pattern, const std::string& s) { return RX(pattern).fullmatch(s); }
std::string escape(const std::string& literal);          // a literal as a pattern

}  // namespace cb::invert::model
