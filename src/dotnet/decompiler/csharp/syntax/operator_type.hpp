// dotnet/decompiler/csharp/syntax/operator_type.hpp -- the operator kinds of the C# syntax tree's unary, binary and assignment expressions,
// and what each means for the LINQ expression kinds the resolver's operator results carry.
//
//     BinaryOperatorType op = BinaryOperatorType::Add;
//     il::ExpressionType e = get_linq_node_type(op, /*check_for_overflow*/ true);    // AddChecked
//     auto bop = get_corresponding_binary_operator(AssignmentOperatorType::Add);     // BinaryOperatorType::Add
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: BinaryOperatorType, UnaryOperatorType, AssignmentOperatorType and the static
// GetLinqNodeType / GetCorrespondingBinaryOperator members of BinaryOperatorExpression, UnaryOperatorExpression and AssignmentExpression, from
// BinaryOperatorExpression.cs, UnaryOperatorExpression.cs and AssignmentExpression.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). The expression classes include this header; their static members forward to the functions here.
#pragma once

#include <cstdint>
#include <optional>

#include "dotnet/decompiler/il/advanced_instructions.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

enum class BinaryOperatorType : std::uint8_t {
    // Any binary operator (used in pattern matching)
    Any,

    // 'Logical or' is avoided on purpose, because it is not clear if that refers to the bitwise or to the short-circuiting (conditional)
    // operator: the C# spec says logical='|', conditional='||'.
    BitwiseAnd,           // left & right
    BitwiseOr,            // left | right
    ConditionalAnd,       // left && right
    ConditionalOr,        // left || right
    ExclusiveOr,          // left ^ right

    GreaterThan,          // left > right
    GreaterThanOrEqual,   // left >= right
    Equality,             // left == right
    InEquality,           // left != right
    LessThan,             // left < right
    LessThanOrEqual,      // left <= right

    Add,                  // left + right
    Subtract,             // left - right
    Multiply,             // left * right
    Divide,               // left / right
    Modulus,              // left % right

    ShiftLeft,            // left << right
    ShiftRight,           // left >> right
    UnsignedShiftRight,   // left >>> right

    NullCoalescing,       // left ?? right
    Range,                // left .. right (left and right are optional and may be null)

    IsPattern,            // left is right (right must be a pattern)

    // Not upstream: the C# 9 pattern combinator `left or right` (the plain lowering, l2_syntax_builder, writes a switch-expression arm of
    // several constants with it; ILSpy's builders never make it).
    PatternOr,
};

enum class UnaryOperatorType : std::uint8_t {
    // Any unary operator (used in pattern matching)
    Any,

    Not,              // Logical not (!a)
    BitNot,           // Bitwise not (~a)
    Minus,            // Unary minus (-a)
    Plus,             // Unary plus (+a)
    Increment,        // Pre increment (++a)
    Decrement,        // Pre decrement (--a)
    PostIncrement,    // Post increment (a++)
    PostDecrement,    // Post decrement (a--)
    Dereference,      // Dereferencing (*a)
    AddressOf,        // Get address (&a)
    Await,            // C# 5.0 await
    // C# 6 null-conditional operator. Occurs as target of member reference or indexer expressions to indicate ?. or ?[]. Corresponds to
    // nullable.unwrap in the ILAst.
    NullConditional,
    // Wrapper around a primary expression containing a null conditional operator. Corresponds to nullable.rewrap in the ILAst. This has no
    // syntax in C#, but the node is used to ensure parentheses are inserted where necessary.
    NullConditionalRewrap,
    // Implicit call of "operator true".
    IsTrue,
    // C# 8 postfix ! operator (dammit operator)
    SuppressNullableWarning,
    // C# 8 prefix ^ operator
    IndexFromEnd,
    // C# 9 not pattern
    PatternNot,
    // C# 9 relational patterns
    PatternRelationalLessThan,
    PatternRelationalLessThanOrEqual,
    PatternRelationalGreaterThan,
    PatternRelationalGreaterThanOrEqual,
};

enum class AssignmentOperatorType : std::uint8_t {
    Assign,               // left = right

    Add,                  // left += right
    Subtract,             // left -= right
    Multiply,             // left *= right
    Divide,               // left /= right
    Modulus,              // left %= right

    ShiftLeft,            // left <<= right
    ShiftRight,           // left >>= right
    UnsignedShiftRight,   // left >>>= right

    BitwiseAnd,           // left &= right
    BitwiseOr,            // left |= right
    ExclusiveOr,          // left ^= right

    Any,                  // Any operator (for pattern matching)
};

// The LINQ expression kind of an operator (Extension for those LINQ has no kind for); std::invalid_argument for a value that has none (Any ...).
il::ExpressionType get_linq_node_type(BinaryOperatorType op, bool check_for_overflow);
il::ExpressionType get_linq_node_type(UnaryOperatorType op, bool check_for_overflow);
il::ExpressionType get_linq_node_type(AssignmentOperatorType op, bool check_for_overflow);

// The binary operator a compound assignment applies (nullopt for plain assignment).
std::optional<BinaryOperatorType> get_corresponding_binary_operator(AssignmentOperatorType op);

}  // namespace cb::dotnet::decompiler::csharp::syntax
