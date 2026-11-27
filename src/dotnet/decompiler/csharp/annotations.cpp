// dotnet/decompiler/csharp/annotations.cpp -- see annotations.hpp.
#include "dotnet/decompiler/csharp/annotations.hpp"

#include "dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/semantics/local_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp {

using semantics::ConversionResolveResult;
using semantics::ErrorResolveResult;

std::vector<ILInstruction*> il_instructions_of(const AstNode* node) {
    std::vector<ILInstruction*> result;
    for (il::ArenaObject* annotation : node->annotations())
        if (auto* inst = dynamic_cast<ILInstruction*>(annotation)) result.push_back(inst);
    return result;
}

UseImplicitlyTypedOutAnnotation* UseImplicitlyTypedOutAnnotation::instance() { return make<UseImplicitlyTypedOutAnnotation>(); }

ExpressionWithILInstruction with_il_instruction(Expression* expression, ILInstruction* instruction) {
    expression->add_annotation(instruction);
    return ExpressionWithILInstruction(expression);
}

ExpressionWithILInstruction with_il_instruction(Expression* expression, const std::vector<ILInstruction*>& instructions) {
    for (ILInstruction* inst : instructions) expression->add_annotation(inst);
    return ExpressionWithILInstruction(expression);
}

ExpressionWithILInstruction without_il_instruction(Expression* expression) { return ExpressionWithILInstruction(expression); }

TranslatedStatement with_il_instruction(Statement* statement, ILInstruction* instruction) {
    statement->add_annotation(instruction);
    return TranslatedStatement(statement);
}

TranslatedStatement with_il_instruction(Statement* statement, const std::vector<ILInstruction*>& instructions) {
    for (ILInstruction* inst : instructions) statement->add_annotation(inst);
    return TranslatedStatement(statement);
}

TranslatedStatement without_il_instruction(Statement* statement) { return TranslatedStatement(statement); }

ExpressionWithResolveResult with_rr(Expression* expression, ResolveResult* resolve_result) {
    expression->add_annotation(resolve_result);
    return ExpressionWithResolveResult(expression, resolve_result);
}

// ---- the helper structs of translated_expression.hpp that are annotation helpers --------------------------------------------

ExpressionWithILInstruction::ExpressionWithILInstruction(Expression* expr) : expression(expr) { assert(expr != nullptr); }

TranslatedExpression ExpressionWithILInstruction::with_rr(ResolveResult* resolve_result) const {
    expression->add_annotation(resolve_result);
    return TranslatedExpression(expression, resolve_result);
}

ExpressionWithResolveResult::ExpressionWithResolveResult(Expression* expr) : expression(expr) {
    assert(expr != nullptr);
    resolve_result = expr->annotation<ResolveResult>();
    if (resolve_result == nullptr) resolve_result = ErrorResolveResult::unknown_error();
}

ExpressionWithResolveResult::ExpressionWithResolveResult(Expression* expr, ResolveResult* rr) : expression(expr), resolve_result(rr) {
    assert(expr != nullptr && rr != nullptr);
    assert(expr->annotation<ResolveResult>() == rr);
}

TranslatedExpression ExpressionWithResolveResult::with_il_instruction(ILInstruction* instruction) const {
    expression->add_annotation(instruction);
    return TranslatedExpression(expression, resolve_result);
}

TranslatedExpression ExpressionWithResolveResult::with_il_instruction(const std::vector<ILInstruction*>& instructions) const {
    for (ILInstruction* inst : instructions) expression->add_annotation(inst);
    return TranslatedExpression(expression, resolve_result);
}

TranslatedExpression ExpressionWithResolveResult::without_il_instruction() const { return TranslatedExpression(expression, resolve_result); }

TranslatedExpression TranslatedExpression::with_il_instruction(ILInstruction* instruction) const {
    expression->add_annotation(instruction);
    return *this;
}

TranslatedExpression TranslatedExpression::with_il_instruction(const std::vector<ILInstruction*>& instructions) const {
    for (ILInstruction* inst : instructions) expression->add_annotation(inst);
    return *this;
}

// ---- reading annotations ------------------------------------------------------------------------------------------------------

namespace {
// The symbol a resolve result stands for (upstream's ResolveResult.GetSymbol), for the result classes that are ported.
const typesystem::ISymbol* symbol_of(const ResolveResult* rr) {
    if (auto* local = dynamic_cast<const semantics::LocalResolveResult*>(rr)) return local->variable();
    if (auto* member = dynamic_cast<const semantics::MemberResolveResult*>(rr)) return member->member();
    if (auto* type = dynamic_cast<const semantics::TypeResolveResult*>(rr)) return type->type()->get_definition();
    if (auto* conversion = dynamic_cast<const ConversionResolveResult*>(rr)) return symbol_of(conversion->input);
    if (auto* dynamic_member = dynamic_cast<const resolver::DynamicMemberResolveResult*>(rr)) return dynamic_member->symbol;
    if (auto* dynamic_invocation = dynamic_cast<const resolver::DynamicInvocationResolveResult*>(rr)) return dynamic_invocation->symbol;
    return nullptr;
}

ILVariable* variable_of_annotation(const AstNode* node) {
    if (auto* rr = dynamic_cast<ILVariableResolveResult*>(node->annotation<ResolveResult>())) return rr->variable;
    return nullptr;
}
}  // namespace

const typesystem::ISymbol* get_symbol(const AstNode* node) {
    ResolveResult* rr = node->annotation<ResolveResult>();
    if (auto* group = dynamic_cast<resolver::MethodGroupResolveResult*>(rr)) return group->chosen_method();
    return rr != nullptr ? symbol_of(rr) : nullptr;
}

ResolveResult* get_resolve_result(const AstNode* node) {
    ResolveResult* rr = node->annotation<ResolveResult>();
    return rr != nullptr ? rr : ErrorResolveResult::unknown_error();
}

ILVariable* get_il_variable(const IdentifierExpression* expr) { return variable_of_annotation(expr); }
ILVariable* get_il_variable(const VariableInitializer* initializer) { return variable_of_annotation(initializer); }
ILVariable* get_il_variable(const ForeachStatement* loop) { return variable_of_annotation(loop); }

VariableInitializer* with_il_variable(VariableInitializer* initializer, ILVariable* v) {
    initializer->add_annotation(make<ILVariableResolveResult>(v, v->type()));
    return initializer;
}

ForeachStatement* with_il_variable(ForeachStatement* loop, ILVariable* v) {
    loop->add_annotation(make<ILVariableResolveResult>(v, v->type()));
    return loop;
}

namespace detail {
std::vector<il::ArenaObject*> copyable_annotations(const AstNode* other) {
    // The only annotation that is an ICloneable is the node's trivia holder (annotations are otherwise shared by copies).
    std::vector<il::ArenaObject*> result;
    for (il::ArenaObject* annotation : other->annotations())
        if (dynamic_cast<syntax::ICloneable*>(annotation) == nullptr) result.push_back(annotation);
    return result;
}
}  // namespace detail

}  // namespace cb::dotnet::decompiler::csharp
