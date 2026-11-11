// cb/port/old_sizes.cpp -- see old_sizes.hpp.

#include "cb/port/old_sizes.hpp"

#include <array>

#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

namespace {

struct Entry {
    std::string_view name;
    std::uint32_t size;
};

template <std::size_t N>
std::optional<std::uint32_t> lookup(const std::array<Entry, N>& table, std::string_view name) {
    for (const Entry& e : table)
        if (e.name == name) return e.size;
    return std::nullopt;
}

}  // namespace

namespace {

constexpr std::array<Entry, 23> kSizes = {{
    {"char", 1}, {"signed char", 1}, {"unsigned char", 1}, {"int16_t", 2}, {"uint16_t", 2}, {"int32_t", 4},
    {"uint32_t", 4}, {"int", 2}, {"unsigned", 2}, {"short", 2}, {"long", 4}, {"float", 4}, {"double", 8},
    {"unsigned int", 2}, {"unsigned short", 2}, {"short int", 2}, {"unsigned long", 4}, {"long int", 4},
    {"unsigned long int", 4}, {"signed int", 2}, {"signed long", 4}, {"int8_t", 1}, {"uint8_t", 1}}};

constexpr std::array<Entry, 16> kOld = {{
    {"char", 1}, {"signed char", 1}, {"unsigned char", 1}, {"short", 2}, {"short int", 2}, {"unsigned short", 2},
    {"int", 2}, {"unsigned", 2}, {"unsigned int", 2}, {"signed", 2}, {"long", 4}, {"long int", 4},
    {"unsigned long", 4}, {"unsigned long int", 4}, {"float", 4}, {"double", 8}}};

template <std::size_t N>
std::vector<std::pair<std::string_view, std::uint32_t>> listed(const std::array<Entry, N>& table) {
    std::vector<std::pair<std::string_view, std::uint32_t>> out;
    for (const Entry& e : table) out.emplace_back(e.name, e.size);
    return out;
}

}  // namespace

std::optional<std::uint32_t> scalar_size(std::string_view type) { return lookup(kSizes, type); }
std::optional<std::uint32_t> old_scalar_size(std::string_view type) { return lookup(kOld, type); }
std::vector<std::pair<std::string_view, std::uint32_t>> scalar_sizes() { return listed(kSizes); }
std::vector<std::pair<std::string_view, std::uint32_t>> old_scalar_sizes() { return listed(kOld); }

std::optional<std::uint64_t> array_count(std::string_view declarator) {
    // every `[ digits ]`; a bracket with anything else in it (an expression, nothing) is not a count
    std::uint64_t n = 1;
    std::size_t i = 0;
    while ((i = declarator.find('[', i)) != std::string_view::npos) {
        const std::size_t close = declarator.find(']', i);
        if (close == std::string_view::npos) break;
        const std::string_view inside = text::trim(declarator.substr(i + 1, close - i - 1));
        if (!inside.empty() && inside.find_first_not_of("0123456789") == std::string_view::npos) {
            const auto v = text::parse_int<std::uint64_t>(inside);
            if (!v || (*v != 0 && n > UINT64_MAX / *v)) return std::nullopt;   // (a count past 64 bits: no size)
            n *= *v;
        }
        i = close + 1;
    }
    return n;
}

std::optional<std::uint64_t> size_of(std::string_view base, std::string_view decl, std::string_view name, unsigned ptr,
                                     const StructSizeFn& struct_size) {
    // the declarator without its name (the first one)
    std::string d(decl);
    if (const std::size_t at = d.find(name); at != std::string::npos && !name.empty()) d.erase(at, name.size());
    const auto n = array_count(d);
    if (!n) return std::nullopt;
    if (d.find('*') != std::string::npos || d.find('(') != std::string::npos) return static_cast<std::uint64_t>(ptr) * *n;
    // the type without its storage words
    static const Regex storage(R"(\b(?:register|static|extern|auto|const|volatile)\s+)");
    const std::string b(text::trim(collapse_space(storage.replace_all(base, ""))));
    if (const auto s = scalar_size(b)) return *s * *n;
    if (const auto s = struct_size(b)) return *s * *n;
    return std::nullopt;
}

std::optional<std::uint64_t> size_of(std::string_view base, std::string_view decl, std::string_view name, unsigned ptr,
                                     const StructLayouts& structs) {
    return size_of(base, decl, name, ptr, StructSizeFn([&structs](std::string_view tag) -> std::optional<std::uint64_t> {
                       const StructLayout* l = structs.find(std::string(tag));
                       if (l == nullptr) return std::nullopt;
                       return l->size;
                   }));
}

}  // namespace cb::port::pre_ansi
