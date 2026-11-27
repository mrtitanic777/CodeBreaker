// dotnet/decompiler/csharp/syntax/primitive_expression.cpp -- see primitive_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/primitive_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

namespace {
// .NET's object.Equals on boxed primitives: the same type and an equal value (NaN equals NaN for floating point).
bool constant_values_equal(const typesystem::ConstantValue& a, const typesystem::ConstantValue& b) {
    if (a.index() != b.index()) return false;
    if (const auto* da = std::get_if<double>(&a)) {
        double db = std::get<double>(b);
        return *da == db || (std::isnan(*da) && std::isnan(db));
    }
    if (const auto* fa = std::get_if<float>(&a)) {
        float fb = std::get<float>(b);
        return *fa == fb || (std::isnan(*fa) && std::isnan(fb));
    }
    return a == b;
}
}  // namespace

bool PrimitiveExpression::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<PrimitiveExpression>(other);
    if (o == nullptr) return false;
    if (is_any_value_) return true;
    return !o->is_any_value_ && constant_values_equal(value_, o->value_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
