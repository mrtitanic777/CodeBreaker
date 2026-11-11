#include "cb/translate/frontends/python/structfmt.hpp"

#include <limits>

#include "cb/core/text.hpp"

namespace cb::translate::frontends::python::structfmt {

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}

constexpr std::uint64_t kMaxRepeat = 1u << 20;        // a repeat count past this is not a real struct
constexpr std::uint64_t kMaxStructSize = 0x7FFFFFFFu;  // a struct larger than this is refused (CPython: past what a signed size holds)

struct IntCode {
    std::uint32_t size;
    bool is_signed;
};

std::optional<IntCode> int_code(char code) {
    switch (code) {
        case 'b': return IntCode{1, true};
        case 'B': return IntCode{1, false};
        case 'h': return IntCode{2, true};
        case 'H': return IntCode{2, false};
        case 'i': return IntCode{4, true};
        case 'I': return IntCode{4, false};
        case 'l': return IntCode{4, true};   // native on Windows x64: 4 bytes
        case 'L': return IntCode{4, false};
        case 'q': return IntCode{8, true};
        case 'Q': return IntCode{8, false};
        default: return std::nullopt;
    }
}

bool is_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// the item for one integer code
Item int_item(char code, IntCode ic) {
    Item item{.code = code, .size = ic.size, .kind = Kind::Int};
    const unsigned bits = 8 * ic.size;
    if (ic.is_signed) {
        item.lo = static_cast<std::int64_t>(0 - (std::uint64_t{1} << (bits - 1)));
        item.hi = (std::uint64_t{1} << (bits - 1)) - 1;
    } else {
        item.hi = bits == 64 ? std::numeric_limits<std::uint64_t>::max() : (std::uint64_t{1} << bits) - 1;
    }
    return item;
}

}  // namespace

std::string_view kind_word(Kind kind) {
    switch (kind) {
        case Kind::Pad: return "pad";
        case Kind::Bytes: return "bytes";
        case Kind::Bool: return "bool";
        case Kind::Float: return "float";
        case Kind::Int: return "int";
    }
    return "";
}

std::optional<std::vector<Item>> parse(std::string_view format) {
    char order = '@';
    if (!format.empty() && contains(std::string_view("<>!=@"), format.front())) {
        order = format.front();
        format.remove_prefix(1);
    }
    std::vector<Item> items;
    std::uint64_t total = 0;
    std::size_t pos = 0;
    for (;;) {
        while (pos < format.size() && text::is_space(format[pos])) ++pos;   // white space precedes an item
        if (pos == format.size()) break;
        std::uint64_t count = 0;
        const bool has_count = is_digit(format[pos]);
        while (pos < format.size() && is_digit(format[pos])) {
            count = count * 10 + static_cast<std::uint64_t>(format[pos++] - '0');
            if (count > kMaxStructSize) return std::nullopt;
        }
        if (pos == format.size()) return std::nullopt;   // a count with no code
        const char code = format[pos++];
        if (!is_letter(code) && code != '?') return std::nullopt;
        if (!has_count) count = 1;

        if (code == 'x' || code == 's') {
            // one item for the whole run: its size is the count
            items.push_back(code == 'x' ? Item{.code = 'x', .size = count, .kind = Kind::Pad}
                                        : Item{.code = 's', .size = count, .kind = Kind::Bytes,
                                               .lo = static_cast<std::int64_t>(count), .hi = count});
            total += count;
        } else {
            if (count > kMaxRepeat) return std::nullopt;
            Item one;
            if (code == 'c') {
                one = {.code = 'c', .size = 1, .kind = Kind::Bytes, .lo = 1, .hi = 1};
            } else if (code == '?') {
                one = {.code = '?', .size = 1, .kind = Kind::Bool, .lo = 0, .hi = 1};
            } else if (code == 'f' || code == 'd') {
                one = {.code = code, .size = code == 'f' ? 4u : 8u, .kind = Kind::Float};
            } else if (const auto ic = int_code(code)) {
                one = int_item(code, *ic);
            } else {
                return std::nullopt;
            }
            items.insert(items.end(), static_cast<std::size_t>(count), one);
            total += count * one.size;
        }
        if (total > kMaxStructSize) return std::nullopt;
    }
    if (order == '@' && items.size() > 1) {
        // native alignment pads between sized numbers; the support header implements the standard sizes only
        for (const Item& item : items) {
            if (item.kind == Kind::Int && item.size > 1) return std::nullopt;
            if (item.kind == Kind::Float) return std::nullopt;
        }
    }
    return items;
}

std::uint64_t size(const std::vector<Item>& items) {
    std::uint64_t total = 0;
    for (const Item& item : items) total += item.size;
    return total;
}

std::vector<Item> values(const std::vector<Item>& items) {
    std::vector<Item> out;
    for (const Item& item : items) {
        if (item.kind != Kind::Pad) out.push_back(item);
    }
    return out;
}

}  // namespace cb::translate::frontends::python::structfmt
