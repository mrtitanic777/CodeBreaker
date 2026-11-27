// dotnet/decompiler/csharp/syntax/ast_fwd.hpp -- forward declarations of every class of the C# syntax tree, so the headers that only
// mention a node (the visitors, the slot constants, the kind table) need not include its definition.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: the node classes named in IAstVisitor), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

namespace cb::dotnet::decompiler::csharp::syntax {

namespace pattern_matching {
class INode;
class Pattern;
class Match;
}  // namespace pattern_matching

// The abstract bases.
class AstNode;
class Expression;
class Statement;
class AstType;
class EntityDeclaration;
class QueryClause;
class Trivia;
class InterpolatedStringContent;
class VariableDesignation;

// Everything else, in upstream's alphabetical order.
class Accessor;
class AnonymousMethodExpression;
class AnonymousTypeCreateExpression;
class ArrayCreateExpression;
class ArrayInitializerExpression;
class ArraySpecifier;
class AsExpression;
class AssignmentExpression;
class Attribute;
class AttributeSection;
class BaseReferenceExpression;
class BinaryOperatorExpression;
class BlockStatement;
class BreakStatement;
class CaseLabel;
class CastExpression;
class CatchClause;
class CheckedExpression;
class CheckedStatement;
class Comment;
class ComposedType;
class ConditionalExpression;
class Constraint;
class ConstructorDeclaration;
class ConstructorInitializer;
class ContinueStatement;
class CustomEventDeclaration;
class DeclarationExpression;
class DefaultValueExpression;
class DelegateDeclaration;
class DestructorDeclaration;
class DirectionExpression;
class DoWhileStatement;
class DocumentationReference;
class EmptyStatement;
class EnumMemberDeclaration;
class ErrorExpression;
class EventDeclaration;
class ExpressionStatement;
class ExtensionDeclaration;
class ExternAliasDeclaration;
class FieldDeclaration;
class FixedFieldDeclaration;
class FixedStatement;
class FixedVariableInitializer;
class ForStatement;
class ForeachStatement;
class FunctionPointerAstType;
class GotoCaseStatement;
class GotoDefaultStatement;
class GotoStatement;
class Identifier;
class IdentifierExpression;
class IfElseStatement;
class IndexerDeclaration;
class IndexerExpression;
class InterpolatedStringExpression;
class InterpolatedStringText;
class Interpolation;
class InvocationAstType;
class InvocationExpression;
class IsExpression;
class LabelStatement;
class LambdaExpression;
class LinePreprocessorDirective;
class LocalFunctionDeclarationStatement;
class LockStatement;
class MemberReferenceExpression;
class MemberType;
class MethodDeclaration;
class NamedArgumentExpression;
class NamedExpression;
class NamespaceDeclaration;
class NullReferenceExpression;
class ObjectCreateExpression;
class OperatorDeclaration;
class OutVarDeclarationExpression;
class ParameterDeclaration;
class ParenthesizedExpression;
class ParenthesizedVariableDesignation;
class PointerReferenceExpression;
class PragmaWarningPreprocessorDirective;
class PreProcessorDirective;
class PrimitiveExpression;
class PrimitiveType;
class PropertyDeclaration;
class QueryContinuationClause;
class QueryExpression;
class QueryFromClause;
class QueryGroupClause;
class QueryJoinClause;
class QueryLetClause;
class QueryOrderClause;
class QueryOrdering;
class QuerySelectClause;
class QueryWhereClause;
class RecursivePatternExpression;
class ReturnStatement;
class SimpleType;
class SingleVariableDesignation;
class SizeOfExpression;
class StackAllocExpression;
class SwitchExpression;
class SwitchExpressionSection;
class SwitchSection;
class SwitchStatement;
class SyntaxTree;
class ThisReferenceExpression;
class ThrowExpression;
class ThrowStatement;
class TryCatchStatement;
class TupleAstType;
class TupleExpression;
class TupleTypeElement;
class TypeDeclaration;
class TypeOfExpression;
class TypeParameterDeclaration;
class TypeReferenceExpression;
class UnaryOperatorExpression;
class UncheckedExpression;
class UncheckedStatement;
class UndocumentedExpression;
class UnsafeStatement;
class UsingAliasDeclaration;
class UsingDeclaration;
class UsingStatement;
class VariableDeclarationStatement;
class VariableInitializer;
class WhileStatement;
class WithInitializerExpression;
class YieldBreakStatement;
class YieldReturnStatement;

}  // namespace cb::dotnet::decompiler::csharp::syntax
