// dotnet/decompiler/csharp/resolver/csharp_operators.hpp -- the operators C# has built in (unary plus and minus, !, ~, * / % + - << >> >>>,
// == != < <= > >=, & | ^, string concatenation) as methods overload resolution can pick among, each with its parameter and return types, its
// lifted form over nullable operands and, for constants, the function that evaluates it; and the lifted form of a user-defined operator.
//
//     CSharpOperators& operators = CSharpOperators::get(compilation);
//     for (CSharpOperators::OperatorMethod* m : operators.addition_operators()) overload_resolution.add_candidate(m);
//     const IMethod* lifted = CSharpOperators::lift_user_defined_operator(op_addition);   // nullptr when it cannot be lifted
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: CSharpOperators with its OperatorMethod classes, ILiftedOperator, from
// CSharpOperators.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// One instance serves a compilation (kept by the compilation's arena, as CSharpConversions is); the operator arrays are made on first
// use, behind a lock. Upstream's operators are member objects with no entity behind them: here OperatorMethod is a FakeMember (declaring
// type unknown, parameters made up). Evaluating an operator on constants follows C#'s semantics for every operand type, throwing
// OverflowError for a checked overflow and DivideByZeroError for a division by zero (both ArithmeticError, upstream's ArithmeticException,
// which the resolver catches). The arithmetic of decimal is util::Decimal's (.NET's own algorithms).
// OperatorDeclaration.GetOperatorType (CSharp/Syntax, which maps an operator method's name to its type) is not needed: the comparison
// operators are told by their method names.
#pragma once

#include <array>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <vector>

#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/util/arithmetic_error.hpp"
#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using typesystem::ConstantValue;
using typesystem::ICompilation;
using typesystem::IMethod;
using typesystem::IParameter;
using typesystem::IParameterizedMember;
using typesystem::IType;

class CSharpResolver;

// System.ArithmeticException, OverflowException and DivideByZeroException (util/arithmetic_error.hpp, shared with util::Decimal).
using util::ArithmeticError;
using util::DivideByZeroError;
using util::OverflowError;

// Gives overload resolution a hint that the member represents a lifted operator, which is used in the tie-breaking rules.
class ILiftedOperator : public virtual IParameterizedMember {
public:
    virtual const IType* non_lifted_return_type() const = 0;
    virtual const std::vector<const IParameter*>& non_lifted_parameters() const = 0;
};

class CSharpOperators {
public:
    // The instance of a compilation (made on the first ask).
    static CSharpOperators& get(const ICompilation& compilation);

    // ---- the operator methods ---------------------------------------------------------------------------------------------

    // A built-in operator as a member with parameters and a return type.
    class OperatorMethod : public typesystem::FakeMember, public virtual IParameterizedMember {
    public:
        typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Operator; }
        const std::vector<const IParameter*>& parameters() const override { return parameters_value; }
        const typesystem::IModule* parent_module() const override;
        const IMember* specialize(const typesystem::TypeParameterSubstitution& substitution) const override;
        std::string full_name() const override { return "operator"; }
        std::string reflection_name() const override { return "operator"; }
        std::string to_string() const override;

        // The lifted form of the operator over nullable operands; null when it has none.
        virtual std::unique_ptr<OperatorMethod> lift(CSharpOperators& operators) const;

        // The parameters (upstream: internal).
        std::vector<const IParameter*> parameters_value;

    protected:
        explicit OperatorMethod(const ICompilation& compilation);
    };

    class UnaryOperatorMethod : public OperatorMethod {
    public:
        explicit UnaryOperatorMethod(const ICompilation& compilation) : OperatorMethod(compilation) {}
        virtual bool can_evaluate_at_compile_time() const { return false; }
        // The result of the operator on a constant (null in, null out). Throws ArithmeticError when there is none.
        virtual ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& input) const;
    };

    class BinaryOperatorMethod : public OperatorMethod {
    public:
        explicit BinaryOperatorMethod(const ICompilation& compilation) : OperatorMethod(compilation) {}
        virtual bool can_evaluate_at_compile_time() const { return false; }
        virtual ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& lhs, const ConstantValue& rhs) const;
    };

    // ---- unary operators --------------------------------------------------------------------------------------------------

    // C# 4.0 spec: 7.7.1 Unary plus operator
    const std::vector<OperatorMethod*>& unary_plus_operators();
    // C# 4.0 spec: 7.7.2 Unary minus operator
    const std::vector<OperatorMethod*>& unchecked_unary_minus_operators();
    const std::vector<OperatorMethod*>& checked_unary_minus_operators();
    // C# spec (draft-v11): 12.9.4 Logical negation operator
    const std::vector<OperatorMethod*>& logical_negation_operators();
    // C# 4.0 spec: 7.7.4 Bitwise complement operator
    const std::vector<OperatorMethod*>& bitwise_complement_operators();

    // ---- arithmetic operators ---------------------------------------------------------------------------------------------

    // C# 4.0 spec: 7.8.1 Multiplication operator
    const std::vector<OperatorMethod*>& multiplication_operators();
    // 7.8.2 Division operator
    const std::vector<OperatorMethod*>& division_operators();
    // 7.8.3 Remainder operator
    const std::vector<OperatorMethod*>& remainder_operators();
    // 7.8.3 Addition operator (not in this list, but handled manually: enum addition, delegate combination)
    const std::vector<OperatorMethod*>& addition_operators();
    // 7.8.4 Subtraction operator
    const std::vector<OperatorMethod*>& subtraction_operators();
    // 7.8.5 Shift operators
    const std::vector<OperatorMethod*>& shift_left_operators();
    const std::vector<OperatorMethod*>& shift_right_operators();
    const std::vector<OperatorMethod*>& unsigned_shift_right_operators();

    // ---- equality and relational operators --------------------------------------------------------------------------------

    // 7.10 Relational and type-testing operators
    const std::vector<OperatorMethod*>& value_equality_operators();
    const std::vector<OperatorMethod*>& value_inequality_operators();
    const std::vector<OperatorMethod*>& reference_equality_operators();
    const std::vector<OperatorMethod*>& reference_inequality_operators();
    const std::vector<OperatorMethod*>& less_than_operators();
    const std::vector<OperatorMethod*>& less_than_or_equal_operators();
    const std::vector<OperatorMethod*>& greater_than_operators();
    const std::vector<OperatorMethod*>& greater_than_or_equal_operators();

    // ---- bitwise operators ------------------------------------------------------------------------------------------------

    const std::vector<OperatorMethod*>& logical_and_operators();
    const std::vector<OperatorMethod*>& bitwise_and_operators();
    const std::vector<OperatorMethod*>& logical_or_operators();
    const std::vector<OperatorMethod*>& bitwise_or_operators();
    // Note: the logic for the lifted bool? bitwise operators is wrong; we produce "true | null" = "null" when it should be true. However,
    // this is irrelevant because bool? cannot be a compile-time type.
    const std::vector<OperatorMethod*>& bitwise_xor_operators();

    // ---- user-defined operators -------------------------------------------------------------------------------------------

    // The lifted form of a user-defined operator (over nullable operands); null when it cannot be lifted.
    static const IMethod* lift_user_defined_operator(const IMethod* m);
    // Whether the method is one of the comparison operators (op_Equality, op_Inequality, op_GreaterThan, op_LessThan, op_GreaterThanOrEqual,
    // op_LessThanOrEqual).
    static bool is_comparison_operator(const IMethod* m);

    // ---- for the operator classes (upstream: private or internal) ---------------------------------------------------------

    const ICompilation& compilation() const { return compilation_; }
    // The parameter of a System.TypeCode type (Object ... String); made once.
    const IParameter* make_parameter(typesystem::KnownTypeCode code) const;
    // The parameter of the nullable version of a parameter made by make_parameter.
    const IParameter* make_nullable_parameter(const IParameter* normal_parameter) const;
    // Keeps an operator for the life of this object.
    OperatorMethod* own(std::unique_ptr<OperatorMethod> method);

    CSharpOperators(const CSharpOperators&) = delete;
    CSharpOperators& operator=(const CSharpOperators&) = delete;

private:
    explicit CSharpOperators(const ICompilation& compilation);

    // The methods and their lifted forms.
    std::vector<OperatorMethod*> lift(std::vector<OperatorMethod*> methods);
    template <class Make>
    const std::vector<OperatorMethod*>& lazy(std::optional<std::vector<OperatorMethod*>>& slot, Make&& make);

    const ICompilation& compilation_;
    std::array<const IParameter*, 18> normal_parameters_{};    // Object ... String
    std::array<const IParameter*, 13> nullable_parameters_{};  // Boolean ... Decimal
    std::vector<std::unique_ptr<typesystem::DefaultParameter>> owned_parameters_;
    std::vector<std::unique_ptr<OperatorMethod>> owned_methods_;
    std::recursive_mutex mutex_;

    std::optional<std::vector<OperatorMethod*>> unary_plus_operators_, unchecked_unary_minus_operators_, checked_unary_minus_operators_,
        logical_negation_operators_, bitwise_complement_operators_, multiplication_operators_, division_operators_, remainder_operators_,
        addition_operators_, subtraction_operators_, shift_left_operators_, shift_right_operators_, unsigned_shift_right_operators_,
        value_equality_operators_, value_inequality_operators_, reference_equality_operators_, reference_inequality_operators_,
        less_than_operators_, less_than_or_equal_operators_, greater_than_operators_, greater_than_or_equal_operators_, logical_and_operators_,
        bitwise_and_operators_, logical_or_operators_, bitwise_or_operators_, bitwise_xor_operators_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
