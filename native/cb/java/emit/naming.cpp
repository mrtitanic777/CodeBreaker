#include "cb/java/emit/naming.hpp"

#include <algorithm>
#include <array>

namespace cb::java::emit::naming {

namespace {

// Windows/CRT/xnamath macros, then C++ keywords and the min/max macros. A handful of names: a linear scan is fine.
constexpr auto kReserved = std::to_array<std::string_view>({
    "TRUE", "FALSE", "NULL", "NAN", "INFINITY", "ERROR", "DELETE", "IN", "OUT", "OPTIONAL", "CONST", "VOID", "NEAR",
    "FAR", "THIS", "INTERFACE", "DOMAIN",
    "this", "new", "delete", "class", "template", "operator", "default", "private", "public", "protected", "register",
    "export", "try", "throw", "namespace", "true", "false", "nullptr", "min", "max",
});

}  // namespace

std::string static_field_name(std::string_view name) {
    const std::size_t first = name.find_first_not_of('_');
    return "s_" + std::string(first == std::string_view::npos ? std::string_view() : name.substr(first));
}

std::string instance_field_name(std::string_view name) { return std::string(name); }

std::string field_name(std::string_view name, bool is_static) {
    return is_static ? static_field_name(name) : instance_field_name(name);
}

std::string guard_macro(std::string_view class_name) {
    std::string out;
    out.reserve(class_name.size() + 2);
    for (const char c : class_name) {
        const auto u = static_cast<unsigned char>(c);
        if ((u & 0xC0) == 0x80) continue;   // a UTF-8 continuation byte: its character already became one '_'
        if (u >= 'a' && u <= 'z') {
            out += static_cast<char>(u - 'a' + 'A');
        } else if ((u >= 'A' && u <= 'Z') || (u >= '0' && u <= '9')) {
            out += c;
        } else {
            out += '_';
        }
    }
    out += "_H";
    return out;
}

bool is_reserved_member(std::string_view name) {
    return std::find(kReserved.begin(), kReserved.end(), name) != kReserved.end();
}

std::string enum_member(std::string_view name) {
    std::string out(name);
    if (is_reserved_member(name)) out += '_';
    return out;
}

}  // namespace cb::java::emit::naming
