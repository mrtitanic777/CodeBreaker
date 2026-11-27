// dotnet/decompiler/csharp/syntax/primitive_expression.hpp -- a literal value (number, string, character, bool, null) with the form it is written in.
//
//     auto* p = make<PrimitiveExpression>(ConstantValue(std::int32_t(5)), LiteralFormat::DecimalNumber);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: PrimitiveExpression, from Expressions/PrimitiveExpression.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/tokens.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

enum class LiteralFormat : std::uint8_t {
    None,
    DecimalNumber,
    HexadecimalNumber,
    BinaryNumber,
    StringLiteral,
    VerbatimStringLiteral,
    CharLiteral,
    Utf8Literal,
};

// A literal. The value is a typesystem::ConstantValue (null is the empty alternative); `any_value` makes a PrimitiveExpression that
// matches any literal when used as a pattern (upstream's PrimitiveExpression.AnyValue).
class PrimitiveExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::PrimitiveExpression;

    struct AnyValueTag {};
    static constexpr AnyValueTag any_value{};

    PrimitiveExpression() = default;
    explicit PrimitiveExpression(AnyValueTag) : is_any_value_(true) {}
    explicit PrimitiveExpression(typesystem::ConstantValue value, LiteralFormat format = LiteralFormat::None)
        : value_(std::move(value)), format_(format) {}

    // Where the literal is written: set by whoever creates it with a location (the start and end are not computed from children).
    TextLocation start_location() const override { return start_location_value_; }
    TextLocation end_location() const override { return end_location_value_; }
    void set_location(TextLocation start, TextLocation end) {
        start_location_value_ = start;
        end_location_value_ = end;
    }

    const typesystem::ConstantValue& value() const { return value_; }
    void set_value(typesystem::ConstantValue value) {
        value_ = std::move(value);
        is_any_value_ = false;
    }
    // Whether this is the pattern literal that matches any value.
    bool is_any_value() const { return is_any_value_; }

    LiteralFormat format() const { return format_; }
    void set_format(LiteralFormat value) { format_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_primitive_expression(*this); }
    AstNode* shallow_clone() const override { return make<PrimitiveExpression>(*this); }

private:
    TextLocation start_location_value_;
    TextLocation end_location_value_;
    typesystem::ConstantValue value_;
    bool is_any_value_ = false;
    LiteralFormat format_ = LiteralFormat::None;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
