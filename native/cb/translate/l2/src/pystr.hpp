// pystr.hpp -- Python's str methods on UTF-8 std::strings, as the C back end's Python calls them:
// split() on Python's whitespace, strip, replace, partition, int(s, base), repr of a float and of a
// str. Positions are byte offsets (the C text is ASCII but for string literals, which these never cut).
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace py {

bool is_space_cp(uint32_t cp);                           // str.isspace() of one character
uint32_t decode_cp(const std::string& s, size_t& pos);    // the code point at pos (pos moves past it)
size_t prev_cp(const std::string& s, size_t pos);         // where the code point before pos starts

std::vector<std::string> split_ws(const std::string& s, int maxsplit = -1);         // s.split()
std::vector<std::string> split(const std::string& s, const std::string& sep, int maxsplit = -1);
std::vector<std::string> rsplit(const std::string& s, const std::string& sep, int maxsplit);
std::vector<std::string> splitlines(const std::string& s);
std::string strip(const std::string& s);
std::string lstrip(const std::string& s);
std::string rstrip(const std::string& s);
std::string strip_chars(const std::string& s, const std::string& chars);
std::string lstrip_chars(const std::string& s, const std::string& chars);
std::string rstrip_chars(const std::string& s, const std::string& chars);
bool startswith(const std::string& s, const std::string& p);
bool endswith(const std::string& s, const std::string& p);
std::string replace(const std::string& s, const std::string& old, const std::string& rep, int count = -1);
std::string join(const std::string& sep, const std::vector<std::string>& parts);
size_t count(const std::string& s, const std::string& sub);
std::string upper(const std::string& s);
std::string lower(const std::string& s);
struct Parts {
    std::string a, sep, b;
};
Parts partition(const std::string& s, const std::string& sep);
Parts rpartition(const std::string& s, const std::string& sep);
std::string slice(const std::string& s, int64_t a, int64_t b);   // s[a:b], negative counted from the end
std::string spaces(int64_t n);                                    // ' ' * n

// int(s, base): base 0 (a prefix says), 10, 16 -- ValueError (PyError) where Python raises it
int64_t int_parse(const std::string& s, int base);
std::string hex(uint64_t v);                                      // '%x'
std::string HEX(uint64_t v);                                      // '%X'
std::string hexpad(uint64_t v, int width, bool upper = false);    // '%0<width>x'
std::string dec(int64_t v);                                       // '%d'

std::string float_repr(double f);                                 // repr(float)
std::string str_repr(const std::string& s);                       // repr(str)

}  // namespace py
