#include "dotnet/decompiler/semantics/resolve_result.hpp"

#include <cstdint>
#include <variant>

#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::semantics {

std::string to_text(const ConstantValue& value) {
    struct Visitor {
        std::string operator()(std::monostate) const { return {}; }
        std::string operator()(bool v) const { return v ? "True" : "False"; }
        std::string operator()(char16_t v) const { return util::utf8(std::u16string_view(&v, 1)); }
        std::string operator()(std::int8_t v) const { return std::to_string(v); }
        std::string operator()(std::uint8_t v) const { return std::to_string(v); }
        std::string operator()(std::int16_t v) const { return std::to_string(v); }
        std::string operator()(std::uint16_t v) const { return std::to_string(v); }
        std::string operator()(std::int32_t v) const { return std::to_string(v); }
        std::string operator()(std::uint32_t v) const { return std::to_string(v); }
        std::string operator()(std::int64_t v) const { return std::to_string(v); }
        std::string operator()(std::uint64_t v) const { return std::to_string(v); }
        std::string operator()(float v) const { return util::format_r(v); }
        std::string operator()(double v) const { return util::format_r(v); }
        std::string operator()(const std::u16string& v) const { return util::utf8(v); }
        std::string operator()(const util::Decimal& v) const { return v.to_string(); }
    };
    return std::visit(Visitor{}, value);
}

}  // namespace cb::dotnet::decompiler::semantics
