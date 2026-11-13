// cb/invert/term.hpp -- the construct IR of the Microsoft C/C++ 2.0 (32-bit) model: a function as a small tree of tagged nodes.
//
// A node is a tuple whose first item names its kind, and a function is a dictionary of named parts:
//
//     ("const", 5)  ("arg", 1)  ("binop", "+", ("arg", 1), ("const", 5))  ("call", "__stdcall", [args])  ("deref", base)  ("index", base, i)
//     {nargs: 2, expr: ("binop", "+", ("arg", 1), ("arg", 2))}                  one `return expression;`
//     {nargs: 1, stmts: [("voidcall", "__stdcall", [("arg", 1)])], ret: ...}    statements, then a return
//
// The model both writes bytes from a node (the forward model: what the compiler emits for each construct) and reads bytes into candidate nodes
// (the inverse, accepted only when the forward model gives the bytes back), so the nodes are what the two halves agree on. A Term is a tree of
// None, bool, integer, string, tuple, list and dictionary values; reading a part that is not there (a missing key, an index past the end, an
// integer where a tuple was meant) throws FormatError -- a node the model did not build.
#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "cb/invert/model_text.hpp"

namespace cb::invert {

class Term {
public:
    enum class Kind : unsigned char { None, Bool, Int, Str, Tuple, List, Dict };

    Term() = default;
    static Term boolean(bool b);
    static Term integer(long long v);
    static Term string(std::string s);
    static Term tuple(std::initializer_list<Term> items);
    static Term tuple(std::vector<Term> items);
    static Term list(std::vector<Term> items = {});
    static Term dict();

    Kind kind() const { return kind_; }
    bool none() const { return kind_ == Kind::None; }
    bool is_number() const { return kind_ == Kind::Int || kind_ == Kind::Bool; }
    bool is_sequence() const { return kind_ == Kind::Tuple || kind_ == Kind::List; }
    bool is(const char* text) const { return kind_ == Kind::Str && str_ == text; }          // the string `text`
    bool is_number(long long v) const { return is_number() && int_ == v; }
    long long number() const;                                                                // the integer; FormatError for anything else
    const std::string& text() const { return str_; }
    std::size_t size() const;                                                                // items of a sequence or dict; characters of a string
    const Term& operator[](long long j) const;                                               // item j of a sequence (negative from the end)
    const std::vector<Term>& items() const { return items_; }
    const Term* find(const std::string& key) const;                                          // a dict's value, or null
    const Term& get(const std::string& key) const;                                           // a dict's value, None where absent
    const Term& at(const std::string& key) const;                                            // a dict's value; FormatError where absent
    void set(const std::string& key, const Term& value);
    bool truthy() const;
    friend bool operator==(const Term& x, const Term& y);

private:
    Kind kind_ = Kind::None;
    long long int_ = 0;
    std::string str_;
    std::vector<Term> items_;        // a sequence's items; a dict's values
    std::vector<std::string> keys_;  // a dict's keys, items_[j] is keys_[j]'s value
};

bool operator==(const Term& x, const Term& y);
inline bool operator!=(const Term& x, const Term& y) { return !(x == y); }

}  // namespace cb::invert
