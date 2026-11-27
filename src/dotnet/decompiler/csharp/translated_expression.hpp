// dotnet/decompiler/csharp/translated_expression.hpp -- the output of the C# ExpressionBuilder: a C# expression together with its resolve
// result and the ILInstructions it came from, plus the three small helper structs that make the builder add both annotations.
//
//     TranslatedExpression arg = with_rr(without_il_instruction(expr), rr);       // an Expression* is annotated, then wrapped
//     TranslatedExpression text = arg.convert_to(string_type, builder);            // adds the casts the conversion needs
//     TranslatedExpression flag = value.convert_to_boolean(builder, /*negate*/ true);
//
// A TranslatedExpression converts implicitly to its Expression* (and to the two helper structs), as upstream's struct does.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: ExpressionWithILInstruction, ExpressionWithResolveResult, TranslatedExpression, from
// TranslatedExpression.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The WithILInstruction /
// WithoutILInstruction / WithRR extension methods are in annotations.hpp. convert_to waits for AddCheckedBlocks (the checked and unchecked
// annotations) and for ExpressionBuilder (convert_type, convert_constant_value, logic_not, call_unsafe_intrinsic and its compilation,
// settings and resolver members).
#pragma once

#include <functional>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/semantics/conversion.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp {

class ExpressionBuilder;

using syntax::Expression;
using semantics::ResolveResult;
using typesystem::ICompilation;
using typesystem::IType;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IField;
using typesystem::IEvent;
using typesystem::IProperty;
using typesystem::IEntity;
using typesystem::IParameter;
using typesystem::ITypeDefinition;
using typesystem::ConstantValue;

struct TranslatedExpression;

// The ILInstructions carried as annotations by a node, in the order they were added.
std::vector<il::ILInstruction*> il_instructions_of(const syntax::AstNode* node);

// An expression that is annotated with its ILInstruction (or deliberately with none), but not yet with its resolve result.
struct ExpressionWithILInstruction {
    Expression* expression = nullptr;

    std::vector<il::ILInstruction*> il_instructions() const { return il_instructions_of(expression); }

    ExpressionWithILInstruction() = default;
    explicit ExpressionWithILInstruction(Expression* expr);
    operator Expression*() const { return expression; }

    // Adds the resolve result as an annotation (upstream's WithRR).
    TranslatedExpression with_rr(ResolveResult* resolve_result) const;
};

// An expression that is annotated with its resolve result, but not yet with its ILInstruction.
struct ExpressionWithResolveResult {
    Expression* expression = nullptr;
    // Kept here as well as in the annotations: the ExpressionBuilder reads it all the time.
    ResolveResult* resolve_result = nullptr;

    const IType* type() const { return resolve_result->type(); }

    // The resolve result is the expression's annotation (the unknown error without one).
    ExpressionWithResolveResult() = default;
    explicit ExpressionWithResolveResult(Expression* expr);
    ExpressionWithResolveResult(Expression* expr, ResolveResult* rr);
    operator Expression*() const { return expression; }

    TranslatedExpression with_il_instruction(il::ILInstruction* instruction) const;
    TranslatedExpression with_il_instruction(const std::vector<il::ILInstruction*>& instructions) const;
    TranslatedExpression without_il_instruction() const;
};

// A decompiled C# expression with both a resolve result and ILInstruction annotation. The resolve result is also always an annotation
// of the expression; the struct makes sure no case of the ExpressionBuilder forgets to add it.
struct TranslatedExpression {
    Expression* expression = nullptr;
    ResolveResult* resolve_result = nullptr;

    TranslatedExpression() = default;
    // The resolve result is the expression's annotation (the unknown error without one).
    explicit TranslatedExpression(Expression* expr);
    TranslatedExpression(Expression* expr, ResolveResult* rr);

    std::vector<il::ILInstruction*> il_instructions() const { return il_instructions_of(expression); }
    const IType* type() const { return resolve_result->type(); }

    operator Expression*() const { return expression; }
    operator ExpressionWithResolveResult() const { return ExpressionWithResolveResult(expression, resolve_result); }
    operator ExpressionWithILInstruction() const { return ExpressionWithILInstruction(expression); }

    // This expression with the instruction added to its annotations.
    TranslatedExpression with_il_instruction(il::ILInstruction* instruction) const;
    TranslatedExpression with_il_instruction(const std::vector<il::ILInstruction*>& instructions) const;

    // The TranslatedExpression of a descendant expression: every ILInstruction annotation on the way down is copied to it and it is
    // detached from the tree. std::invalid_argument if `descendant` is not one.
    TranslatedExpression unwrap_child(Expression* descendant) const;

    // Adds casts (if necessary) to convert this expression to the target type. A narrower target truncates the value, a wider one sign-
    // or zero-extends it by the sign of the source type, so `translate(stloc.value).convert_to(variable.type)` has the IL's meaning.
    // IntPtr and UIntPtr behave like C# integers of native size: every special case is handled here. The result's type equals the
    // target type (modulo identity conversions) unless `allow_implicit_conversion`, which gives an expression that evaluates to the
    // same value once the context converts it implicitly to the target type.
    TranslatedExpression convert_to(const IType* target_type, ExpressionBuilder& expression_builder, bool check_for_overflow = false,
                                    bool allow_implicit_conversion = false) const;

    // In conditions: removes the bool cast made for an `implicit operator bool` invocation (only when `type_filter` accepts the
    // converted input's type, if given).
    TranslatedExpression unwrap_implicit_bool_conversion(const std::function<bool(const IType*)>& type_filter = {}) const;

    // An integer expression as a boolean: true iff the value is not 0 (iff it is 0 when `negate`).
    TranslatedExpression convert_to_boolean(ExpressionBuilder& expression_builder, bool negate = false) const;

private:
    bool is_fixed_variable() const;
    static bool cast_can_be_made_implicit(const ICompilation& compilation, const semantics::Conversion* conversion, const IType* input_type,
                                          const IType* old_target_type, const IType* new_target_type);
    static TranslatedExpression ldc_i4(const ICompilation& compilation, int value);
};

}  // namespace cb::dotnet::decompiler::csharp
