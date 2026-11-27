// dotnet/decompiler/ilspycmd/types_parser.cpp -- see types_parser.hpp.
#include "dotnet/decompiler/ilspycmd/types_parser.hpp"

#include <array>
#include <utility>

#include "dotnet/decompiler/util/platform.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

std::set<typesystem::TypeKind> TypesParser::parse_selection(const std::vector<std::string>& values) {
    using typesystem::TypeKind;
    static const std::array<std::pair<const char*, TypeKind>, 5> possible_values = {{
        {"class", TypeKind::Class}, {"struct", TypeKind::Struct}, {"interface", TypeKind::Interface}, {"enum", TypeKind::Enum}, {"delegate", TypeKind::Delegate}}};
    const auto find = [&](const std::string& key) -> const std::pair<const char*, TypeKind>* {
        for (const auto& entry : possible_values)
            if (util::compare_ordinal_ignore_case(entry.first, key) == 0) return &entry;
        return nullptr;
    };
    const auto starts_with_key = [](const std::string& value, const char* key) {
        const std::string_view k(key);
        return value.size() >= k.size() && util::compare_ordinal_ignore_case(std::string_view(value).substr(0, k.size()), k) == 0;
    };

    std::set<TypeKind> kinds;
    bool starts_with_any = false;
    if (values.size() == 1)
        for (const auto& entry : possible_values) starts_with_any = starts_with_any || starts_with_key(values[0], entry.first);
    if (values.size() == 1 && !starts_with_any) {
        for (const char ch : values[0]) {
            switch (ch) {
                case 'c': kinds.insert(TypeKind::Class); break;
                case 'i': kinds.insert(TypeKind::Interface); break;
                case 's': kinds.insert(TypeKind::Struct); break;
                case 'd': kinds.insert(TypeKind::Delegate); break;
                case 'e': kinds.insert(TypeKind::Enum); break;
                default: break;
            }
        }
    } else {
        for (const std::string& value : values) {
            std::string v = value;
            while (!v.empty() && find(v) == nullptr) v.pop_back();
            if (const auto* entry = find(v)) kinds.insert(entry->second);
        }
    }
    return kinds;
}

}  // namespace cb::dotnet::decompiler::ilspycmd
