// dotnet/decompiler/csharp/syntax/ast_node_kind.hpp -- which class a syntax node is: one AstNodeKind value per concrete node class,
// ordered so that every abstract base class (Expression, Statement, AstType ...) owns one unbroken run of values. Upstream asks
// `node is Expression`, `slot.ChildType.IsInstanceOfType(node)` and `node as BinaryOperatorExpression`; here each of those is a
// comparison of the node's kind with a range, with no dynamic_cast and without needing the class to be defined:
//
//     if (auto* binary = as<BinaryOperatorExpression>(node)) ...   // as<T>() and is<T>() are in ast_node.hpp
//     kind_of<Expression>::range.contains(node->node_kind())       // "node is Expression"
//     kind_of<ParameterDeclaration>::range                         // {ParameterDeclaration, ParameterDeclarationPlaceholder}
//
// A concrete class returns its own value from node_kind() (every node class declares `static constexpr AstNodeKind kind = ...`). The
// classes upstream marks with a pattern placeholder (AstNode, Expression, Statement, AstType, and the seven concrete ones such as
// BlockStatement) have a second value for the placeholder subclass, placed right after the class's own so that "is a BlockStatement"
// is still one range.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: the class hierarchy of the node classes), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

enum class AstNodeKind : std::uint8_t {
    // ---- direct subclasses of AstNode
    ArraySpecifier, Attribute, AttributeSection, AttributeSectionPlaceholder, CaseLabel, CatchClause, CatchClausePlaceholder, Constraint,
    ConstructorInitializer, DocumentationReference, ExternAliasDeclaration, FixedVariableInitializer, Identifier, NamespaceDeclaration,
    ParameterDeclaration, ParameterDeclarationPlaceholder, QueryOrdering, SwitchExpressionSection, SwitchSection,
    SwitchSectionPlaceholder, SyntaxTree, TupleTypeElement, TypeParameterDeclaration, UsingAliasDeclaration, UsingDeclaration,
    VariableInitializer, VariableInitializerPlaceholder,
    // the placeholder that stands for any node in a pattern
    AstNodePlaceholder,

    // ---- Trivia
    Comment, PreProcessorDirective, LinePreprocessorDirective, PragmaWarningPreprocessorDirective,

    // ---- VariableDesignation
    SingleVariableDesignation, ParenthesizedVariableDesignation,

    // ---- InterpolatedStringContent
    InterpolatedStringText, Interpolation,

    // ---- QueryClause
    QueryContinuationClause, QueryFromClause, QueryGroupClause, QueryJoinClause, QueryLetClause, QueryOrderClause, QuerySelectClause,
    QueryWhereClause,

    // ---- EntityDeclaration
    Accessor, ConstructorDeclaration, CustomEventDeclaration, DelegateDeclaration, DestructorDeclaration, EnumMemberDeclaration,
    EventDeclaration, ExtensionDeclaration, FieldDeclaration, FixedFieldDeclaration, IndexerDeclaration, MethodDeclaration,
    OperatorDeclaration, PropertyDeclaration, TypeDeclaration,

    // ---- AstType
    ComposedType, FunctionPointerAstType, InvocationAstType, MemberType, PrimitiveType, SimpleType, TupleAstType, AstTypePlaceholder,

    // ---- Statement
    BlockStatement, BlockStatementPlaceholder, BreakStatement, CheckedStatement, ContinueStatement, DoWhileStatement, EmptyStatement,
    ExpressionStatement, FixedStatement, ForStatement, ForeachStatement, GotoCaseStatement, GotoDefaultStatement, GotoStatement,
    IfElseStatement, LabelStatement, LocalFunctionDeclarationStatement, LockStatement, ReturnStatement, SwitchStatement, ThrowStatement,
    TryCatchStatement, UncheckedStatement, UnsafeStatement, UsingStatement, VariableDeclarationStatement, WhileStatement,
    YieldBreakStatement, YieldReturnStatement, StatementPlaceholder,

    // ---- Expression
    AnonymousMethodExpression, AnonymousTypeCreateExpression, ArrayCreateExpression, ArrayInitializerExpression,
    ArrayInitializerExpressionPlaceholder, AsExpression, AssignmentExpression, BaseReferenceExpression, BinaryOperatorExpression,
    CastExpression, CheckedExpression, ConditionalExpression, DeclarationExpression, DefaultValueExpression, DirectionExpression,
    ErrorExpression, IdentifierExpression, IndexerExpression, InterpolatedStringExpression, InvocationExpression, IsExpression,
    LambdaExpression, MemberReferenceExpression, NamedArgumentExpression, NamedExpression, NullReferenceExpression,
    ObjectCreateExpression, OutVarDeclarationExpression, ParenthesizedExpression, PointerReferenceExpression, PrimitiveExpression,
    QueryExpression, RecursivePatternExpression, SizeOfExpression, StackAllocExpression, SwitchExpression, ThisReferenceExpression,
    ThrowExpression, TupleExpression, TypeOfExpression, TypeReferenceExpression, UnaryOperatorExpression, UncheckedExpression,
    UndocumentedExpression, WithInitializerExpression, ExpressionPlaceholder,
};

// An inclusive run of kinds: the kinds of every instance of one class (upstream's "node is T").
struct KindRange {
    AstNodeKind first;
    AstNodeKind last;
    constexpr bool contains(AstNodeKind kind) const { return kind >= first && kind <= last; }
};

// The kinds of every instance of T. Defined for each class of the tree below; asking for any other type does not compile.
template <class T>
struct kind_of;

#define CB_AST_KIND_RANGE(Class, First, Last)                                    \
    template <>                                                                  \
    struct kind_of<Class> {                                                      \
        static constexpr KindRange range{AstNodeKind::First, AstNodeKind::Last}; \
    };

// The abstract bases (and AstNode, which is every kind).
CB_AST_KIND_RANGE(AstNode, ArraySpecifier, ExpressionPlaceholder)
CB_AST_KIND_RANGE(Trivia, Comment, PragmaWarningPreprocessorDirective)
CB_AST_KIND_RANGE(VariableDesignation, SingleVariableDesignation, ParenthesizedVariableDesignation)
CB_AST_KIND_RANGE(InterpolatedStringContent, InterpolatedStringText, Interpolation)
CB_AST_KIND_RANGE(QueryClause, QueryContinuationClause, QueryWhereClause)
CB_AST_KIND_RANGE(EntityDeclaration, Accessor, TypeDeclaration)
CB_AST_KIND_RANGE(AstType, ComposedType, AstTypePlaceholder)
CB_AST_KIND_RANGE(Statement, BlockStatement, StatementPlaceholder)
CB_AST_KIND_RANGE(Expression, AnonymousMethodExpression, ExpressionPlaceholder)

// The concrete classes that have a pattern placeholder subclass (so a placeholder is also one of them), and PreProcessorDirective,
// which has two subclasses.
CB_AST_KIND_RANGE(AttributeSection, AttributeSection, AttributeSectionPlaceholder)
CB_AST_KIND_RANGE(CatchClause, CatchClause, CatchClausePlaceholder)
CB_AST_KIND_RANGE(ParameterDeclaration, ParameterDeclaration, ParameterDeclarationPlaceholder)
CB_AST_KIND_RANGE(SwitchSection, SwitchSection, SwitchSectionPlaceholder)
CB_AST_KIND_RANGE(VariableInitializer, VariableInitializer, VariableInitializerPlaceholder)
CB_AST_KIND_RANGE(BlockStatement, BlockStatement, BlockStatementPlaceholder)
CB_AST_KIND_RANGE(ArrayInitializerExpression, ArrayInitializerExpression, ArrayInitializerExpressionPlaceholder)
CB_AST_KIND_RANGE(PreProcessorDirective, PreProcessorDirective, PragmaWarningPreprocessorDirective)

// Every other concrete class: exactly its own kind.
#define CB_AST_EXACT_KINDS(X)                                                                                                         \
    X(ArraySpecifier) X(Attribute) X(CaseLabel) X(Constraint) X(ConstructorInitializer) X(DocumentationReference)                    \
    X(ExternAliasDeclaration) X(FixedVariableInitializer) X(Identifier) X(NamespaceDeclaration) X(QueryOrdering)                     \
    X(SwitchExpressionSection) X(SyntaxTree) X(TupleTypeElement) X(TypeParameterDeclaration) X(UsingAliasDeclaration)                \
    X(UsingDeclaration) X(Comment) X(LinePreprocessorDirective) X(PragmaWarningPreprocessorDirective) X(SingleVariableDesignation)  \
    X(ParenthesizedVariableDesignation) X(InterpolatedStringText) X(Interpolation) X(QueryContinuationClause) X(QueryFromClause)    \
    X(QueryGroupClause) X(QueryJoinClause) X(QueryLetClause) X(QueryOrderClause) X(QuerySelectClause) X(QueryWhereClause)           \
    X(Accessor) X(ConstructorDeclaration) X(CustomEventDeclaration) X(DelegateDeclaration) X(DestructorDeclaration)                  \
    X(EnumMemberDeclaration) X(EventDeclaration) X(ExtensionDeclaration) X(FieldDeclaration) X(FixedFieldDeclaration)               \
    X(IndexerDeclaration) X(MethodDeclaration) X(OperatorDeclaration) X(PropertyDeclaration) X(TypeDeclaration) X(ComposedType)      \
    X(FunctionPointerAstType) X(InvocationAstType) X(MemberType) X(PrimitiveType) X(SimpleType) X(TupleAstType) X(BreakStatement)    \
    X(CheckedStatement) X(ContinueStatement) X(DoWhileStatement) X(EmptyStatement) X(ExpressionStatement) X(FixedStatement)         \
    X(ForStatement) X(ForeachStatement) X(GotoCaseStatement) X(GotoDefaultStatement) X(GotoStatement) X(IfElseStatement)            \
    X(LabelStatement) X(LocalFunctionDeclarationStatement) X(LockStatement) X(ReturnStatement) X(SwitchStatement)                   \
    X(ThrowStatement) X(TryCatchStatement) X(UncheckedStatement) X(UnsafeStatement) X(UsingStatement)                               \
    X(VariableDeclarationStatement) X(WhileStatement) X(YieldBreakStatement) X(YieldReturnStatement) X(AnonymousMethodExpression)   \
    X(AnonymousTypeCreateExpression) X(ArrayCreateExpression) X(AsExpression) X(AssignmentExpression) X(BaseReferenceExpression)    \
    X(BinaryOperatorExpression) X(CastExpression) X(CheckedExpression) X(ConditionalExpression) X(DeclarationExpression)            \
    X(DefaultValueExpression) X(DirectionExpression) X(ErrorExpression) X(IdentifierExpression) X(IndexerExpression)                \
    X(InterpolatedStringExpression) X(InvocationExpression) X(IsExpression) X(LambdaExpression) X(MemberReferenceExpression)        \
    X(NamedArgumentExpression) X(NamedExpression) X(NullReferenceExpression) X(ObjectCreateExpression)                               \
    X(OutVarDeclarationExpression) X(ParenthesizedExpression) X(PointerReferenceExpression) X(PrimitiveExpression)                  \
    X(QueryExpression) X(RecursivePatternExpression) X(SizeOfExpression) X(StackAllocExpression) X(SwitchExpression)                \
    X(ThisReferenceExpression) X(ThrowExpression) X(TupleExpression) X(TypeOfExpression) X(TypeReferenceExpression)                 \
    X(UnaryOperatorExpression) X(UncheckedExpression) X(UndocumentedExpression) X(WithInitializerExpression)

#define CB_AST_EXACT_KIND(Class) CB_AST_KIND_RANGE(Class, Class, Class)
CB_AST_EXACT_KINDS(CB_AST_EXACT_KIND)
#undef CB_AST_EXACT_KIND
#undef CB_AST_KIND_RANGE

}  // namespace cb::dotnet::decompiler::csharp::syntax
