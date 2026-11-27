// dotnet/decompiler/csharp/annotations.hpp -- the annotations the C# AST carries beside its children, and the helpers that read and add
// them (the ILInstruction an expression came from, its resolve result, the variable it names ...).
//
//     auto result = without_il_instruction(expr).with_rr(rr);       // upstream's expr.WithoutILInstruction().WithRR(rr)
//     ILVariable* v = get_il_variable(identifier_expression);        // the variable an identifier names, or nullptr
//     ResolveResult* rr = get_resolve_result(node);                  // never null (the unknown error when there is none)
//
// Annotations: syntax nodes are annotated with the ILInstruction they came from, nodes that refer to entities with the IEntity (as a
// resolve result), pointer arithmetic with IntroduceUnsafeModifier's annotation and integer arithmetic with AddCheckedBlocks' checked and
// unchecked annotations. The classes here are arena objects like the nodes (il::ArenaObject), made with il::make.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: AnnotationExtensions, LdTokenAnnotation, ILVariableResolveResult, ForeachAnnotation,
// ImplicitReturnAnnotation, MemberInitializerInOtherConstructorsAnnotation, ImplicitConversionAnnotation, QueryGroupClauseAnnotation,
// QueryJoinClauseAnnotation, UseImplicitlyTypedOutAnnotation, from Annotations.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Upstream's GetSymbol(AstNode) is get_symbol here; it is ResolveResult's GetSymbol (an extension
// of TypeSystemExtensions), which annotations.cpp writes out for the result classes.
#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/translated_expression.hpp"
#include "dotnet/decompiler/csharp/translated_statement.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"

namespace cb::translate::ir {
struct Node;
}

namespace cb::dotnet::decompiler::csharp {

using il::ILFunction;
using il::ILInstruction;
using il::ILVariable;
using il::make;
using syntax::AstNode;
using syntax::ForeachStatement;
using syntax::IdentifierExpression;
using syntax::VariableInitializer;

// Currently unused; the LdToken ILInstruction will probably be the annotation when LdToken support is reimplemented.
class LdTokenAnnotation final : public il::ArenaObject {};

// The L2 node an expression was translated from (stage 14: the kinds the ExpressionBuilder reads from L2), beside the instruction it was
// lifted to, which stays the annotation the C# transforms read until they read L2 too. The node lives in the method's L2Body.
class L2NodeAnnotation final : public il::ArenaObject {
public:
    explicit L2NodeAnnotation(const translate::ir::Node* n) : node(n) {}
    const translate::ir::Node* node;
};

// A reference to a local variable.
class ILVariableResolveResult final : public ResolveResult {
public:
    explicit ILVariableResolveResult(ILVariable* v) : ResolveResult(v->type()), variable(v) {}
    // std::invalid_argument for a null variable.
    ILVariableResolveResult(ILVariable* v, const IType* type) : ResolveResult(type), variable(v) {
        if (v == nullptr) throw std::invalid_argument("v");
    }

    ILVariable* variable;

    std::string type_name() const override { return "ILVariableResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ILVariableResolveResult>(*this); }
};

// Annotates a ForeachStatement with the instructions for the GetEnumerator, MoveNext and get_Current calls.
class ForeachAnnotation final : public il::ArenaObject {
public:
    ForeachAnnotation(ILInstruction* get_enumerator, ILInstruction* move_next, ILInstruction* get_current)
        : get_enumerator_call(get_enumerator), move_next_call(move_next), get_current_call(get_current) {}

    ILInstruction* get_enumerator_call;
    ILInstruction* move_next_call;
    ILInstruction* get_current_call;
};

// Annotates the top-level block statement of a function with the implicitly executed return/yield break.
class ImplicitReturnAnnotation final : public il::ArenaObject {
public:
    explicit ImplicitReturnAnnotation(il::Leave* leave_instruction) : leave(leave_instruction) {}

    il::Leave* leave;
};

// Annotates a field/auto-property/event initializer that the decompiler lifted to the declaration site with the copies of the same
// initializer found in the other constructors. In IL a member initializer runs in every instance constructor that does not chain to
// this(...), so its expression appears once per such constructor; the decompiler lifts it from one constructor and discards the rest.
// This annotation keeps the discarded copies (still carrying that constructor's IL annotations) so the sequence point builder can map the
// initializer's source location onto every constructor that runs it.
class MemberInitializerInOtherConstructorsAnnotation final : public il::ArenaObject {
public:
    explicit MemberInitializerInOtherConstructorsAnnotation(std::vector<Expression*> initializer_list)
        : initializers(std::move(initializer_list)) {}

    std::vector<Expression*> initializers;
};

// Annotates an expression when an implicit user-defined conversion was omitted.
class ImplicitConversionAnnotation final : public il::ArenaObject {
public:
    explicit ImplicitConversionAnnotation(semantics::ConversionResolveResult* resolve_result) : conversion_resolve_result(resolve_result) {}

    semantics::ConversionResolveResult* conversion_resolve_result;
    const IType* target_type() const { return conversion_resolve_result->type(); }
};

// Annotates a QueryGroupClause with the ILFunctions of each (implicit lambda) expression.
class QueryGroupClauseAnnotation final : public il::ArenaObject {
public:
    QueryGroupClauseAnnotation(ILFunction* key, ILFunction* projection) : key_lambda(key), projection_lambda(projection) {}

    ILFunction* key_lambda;
    ILFunction* projection_lambda;
};

// Annotates a QueryJoinClause with the ILFunctions of each (implicit lambda) expression.
class QueryJoinClauseAnnotation final : public il::ArenaObject {
public:
    QueryJoinClauseAnnotation(ILFunction* on, ILFunction* equals) : on_lambda(on), equals_lambda(equals) {}

    ILFunction* on_lambda;
    ILFunction* equals_lambda;
};

// Annotates an out DirectionExpression if the out variable can be declared implicitly typed.
class UseImplicitlyTypedOutAnnotation final : public il::ArenaObject {
public:
    // A new annotation (upstream shares one instance; it is only ever looked up by its type).
    static UseImplicitlyTypedOutAnnotation* instance();
};

// ---- the extension methods ---------------------------------------------------------------------------------------------------

// Annotates the expression with the instruction(s) it came from.
ExpressionWithILInstruction with_il_instruction(Expression* expression, ILInstruction* instruction);
ExpressionWithILInstruction with_il_instruction(Expression* expression, const std::vector<ILInstruction*>& instructions);
// Wraps the expression as one that deliberately has no ILInstruction.
ExpressionWithILInstruction without_il_instruction(Expression* expression);

TranslatedStatement with_il_instruction(Statement* statement, ILInstruction* instruction);
TranslatedStatement with_il_instruction(Statement* statement, const std::vector<ILInstruction*>& instructions);
TranslatedStatement without_il_instruction(Statement* statement);

// Adds the resolve result as an annotation of the expression.
ExpressionWithResolveResult with_rr(Expression* expression, ResolveResult* resolve_result);

// The symbol associated with the node, or nullptr (the chosen method of a method group, else the resolve result's symbol).
const typesystem::ISymbol* get_symbol(const AstNode* node);

// The resolve result of the node, or the unknown error when it has none.
ResolveResult* get_resolve_result(const AstNode* node);

// The variable the identifier / initializer / foreach statement is annotated with (an ILVariableResolveResult), or nullptr.
ILVariable* get_il_variable(const IdentifierExpression* expr);
ILVariable* get_il_variable(const VariableInitializer* initializer);
ILVariable* get_il_variable(const ForeachStatement* loop);

// Annotates the initializer / foreach statement with the variable and returns it.
VariableInitializer* with_il_variable(VariableInitializer* initializer, ILVariable* v);
ForeachStatement* with_il_variable(ForeachStatement* loop, ILVariable* v);

namespace detail {
// The annotations of `other` that are not its trivia holder (see copy_annotations_from).
std::vector<il::ArenaObject*> copyable_annotations(const AstNode* other);
}  // namespace detail

// Copies all annotations of `other` to `node` and returns the node. The trivia holder is not shared (each trivia points at its one owner):
// the trivia is copied onto the node instead.
template <class T>
T* copy_annotations_from(T* node, const AstNode* other) {
    for (il::ArenaObject* annotation : detail::copyable_annotations(other)) node->add_annotation(annotation);
    node->copy_trivia_from(*other);
    return node;
}

// Copies the ILInstruction annotations of `other` to `node` and returns it.
template <class T>
T* copy_instructions_from(T* node, const AstNode* other) {
    for (ILInstruction* inst : il_instructions_of(other)) node->add_annotation(inst);
    return node;
}

}  // namespace cb::dotnet::decompiler::csharp
