#include "dotnet/decompiler/partial_type_info.hpp"

#include <format>

namespace cb::dotnet::decompiler {

std::string PartialTypeInfo::debug_output() const {
    std::string result;
    for (const cb::dotnet::metadata::EntityHandle member : declared_members_) {
        if (!result.empty()) result += ", ";
        result += std::format("{:X}", cb::dotnet::metadata::get_token(member));
    }
    return result;
}

}  // namespace cb::dotnet::decompiler
