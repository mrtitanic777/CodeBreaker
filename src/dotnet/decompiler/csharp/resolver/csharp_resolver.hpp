// dotnet/decompiler/csharp/resolver/csharp_resolver.hpp -- the main logic of the C# resolver: what C# operators, names, member accesses,
// invocations, indexers, object creations, casts, conversions to bool and conditionals mean, as ResolveResults. The resolver knows where
// the code is (using scope, type definition, member, local variables, object initializer) and is immutable: the With* members make a changed
// copy (the same resolver when nothing changes).
//
//     CSharpResolver resolver(compilation);
//     ResolveResult* sum = resolver.resolve_binary_operator(BinaryOperatorType::Add, one, two);   // constant 3
//     resolver = resolver.with_current_type_definition(definition);        // (a resolver is copied with one thing changed)
//     ResolveResult* name = resolver.resolve_simple_name("Count", {});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: CSharpResolver, from CSharpResolver.cs; ICodeContext, from
// TypeSystem/ICodeContext.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// How the port holds it: a resolver is a small value (pointers and shared lists) copied by the With* members; the results it makes are
// arena objects (il::make<T> in the thread's current ILArena). A method group that wants to find extension methods later keeps an arena
// copy of the resolver. Upstream's immutable stack of local variable dictionaries and of object initializers are shared immutable lists; the per-type-definition cache of name lookups is shared by the resolvers of one definition. A
// C# `object` constant is a typesystem::ConstantValue (an empty one is null); a string[] of argument names that may hold nulls is a
// vector of optional strings, an empty vector standing for null. The members that mutate an argument array upstream (the "resolver may
// mutate this array" ones) take the vector by reference.
//
// Upstream's `await` of an operand that is not dynamic is not finished (its code throws NotImplementedException after the lookups, and
// does the same here).
#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/resolver/name_lookup_mode.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp"
#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"
#include "dotnet/decompiler/semantics/for_each_resolve_result.hpp"
#include "dotnet/decompiler/semantics/namespace_resolve_result.hpp"
#include "dotnet/decompiler/semantics/operator_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler::typesystem {

// The context code is resolved in, besides the type system's: what local variables are visible and whether the code is in a lambda.
class ICodeContext : public virtual ITypeResolveContext {
public:
    // All currently visible local variables and lambda parameters (not including method parameters).
    virtual std::vector<const IVariable*> local_variables() const = 0;
    // Whether the context is within a lambda expression or anonymous method.
    virtual bool is_within_lambda_expression() const = 0;
};

}  // namespace cb::dotnet::decompiler::typesystem

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::ArrayCreateResolveResult;
using semantics::ForEachResolveResult;
using syntax::AssignmentOperatorType;
using syntax::BinaryOperatorType;
using syntax::UnaryOperatorType;
using type_system::CSharpTypeResolveContext;
using type_system::UsingScope;
using typesystem::ConstantValue;

// Upstream's Dictionary<string, IVariable> of local variables; the order they were added in is the order they are listed in.
using VariableDictionary = util::InsertionOrderedMap<std::string, const typesystem::IVariable*>;
// Argument names: empty (no argument has a name) or one entry per argument (nullopt for a positional one).
using ArgumentNames = std::vector<std::optional<std::string>>;

// System.InvalidCastException: a constant cast that C# does not have (see util::csharp_primitive_cast).
class InvalidCastError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class CSharpResolver final : public il::ArenaObject, public typesystem::ICodeContext {
public:
    // ---- constructors ----------------------------------------------------------------------------------------------------

    // A resolver for the main module of the compilation.
    explicit CSharpResolver(const ICompilation& compilation);
    explicit CSharpResolver(const CSharpTypeResolveContext& context);
    // (The copy is how the With* members work: a copy with one thing changed.)
    CSharpResolver(const CSharpResolver&) = default;
    CSharpResolver& operator=(const CSharpResolver&) = default;

    // ---- properties ------------------------------------------------------------------------------------------------------

    const ICompilation& compilation() const override { return *compilation_; }
    // The compilation's conversions (upstream's internal field).
    CSharpConversions& conversions() const { return *conversions_; }
    const CSharpTypeResolveContext& current_type_resolve_context() const { return context_; }
    const typesystem::IModule* current_module() const override { return context_.current_module(); }

    // Whether the current context is `checked`.
    bool check_for_overflow() const { return check_for_overflow_; }
    CSharpResolver with_check_for_overflow(bool check_for_overflow) const;

    // Whether the resolver is currently within a lambda expression or anonymous method.
    bool is_within_lambda_expression() const override { return is_within_lambda_expression_; }
    CSharpResolver with_is_within_lambda_expression(bool is_within_lambda_expression) const;

    // The current member definition that is used to look up identifiers as parameters or type parameters. (Set the type definition too
    // when setting the member; setting one does not set the other.)
    const typesystem::IMember* current_member() const override { return context_.current_member(); }
    CSharpResolver with_current_member(const typesystem::IMember* member) const;

    // The current using scope that is used to look up identifiers as class names.
    const UsingScope* current_using_scope() const { return context_.current_using_scope(); }
    CSharpResolver with_current_using_scope(const UsingScope* using_scope) const;

    const typesystem::ITypeDefinition* current_type_definition() const override { return context_.current_type_definition(); }
    CSharpResolver with_current_type_definition(const typesystem::ITypeDefinition* type_definition) const;

    // ---- local variables -------------------------------------------------------------------------------------------------

    // Adds new variables or lambda parameters to the current block (the dictionary is shared, not copied).
    CSharpResolver add_variables(std::shared_ptr<VariableDictionary> variables) const;
    // All currently visible local variables and lambda parameters, the innermost block first. Does not include method parameters.
    std::vector<const typesystem::IVariable*> local_variables() const override;

    // ---- object initializer context --------------------------------------------------------------------------------------

    // Pushes the object that is currently being initialized.
    CSharpResolver push_object_initializer(ResolveResult* initialized_object) const;
    CSharpResolver pop_object_initializer() const;
    // Whether this context is within an object initializer.
    bool is_in_object_initializer() const { return object_initializer_stack_ != nullptr; }
    // The current object initializer: usually an InitializedObjectResolveResult or (for nested initializers) a semantic tree based on one;
    // an ErrorResolveResult if there is none.
    ResolveResult* current_object_initializer() const;
    // The type of the object currently being initialized (Unknown if there is no object initializer or its type is unknown).
    const IType* current_object_initializer_type() const { return current_object_initializer()->type(); }

    // ---- operators -------------------------------------------------------------------------------------------------------

    ResolveResult* resolve_unary_operator(UnaryOperatorType op, ResolveResult* expression) const;
    ResolveResult* resolve_binary_operator(BinaryOperatorType op, ResolveResult* lhs, ResolveResult* rhs) const;
    ResolveResult* resolve_assignment(AssignmentOperatorType op, ResolveResult* lhs, ResolveResult* rhs) const;
    // The user-defined operators `operator_name` of the type, with their lifted versions (none for the types whose operators C# has built in).
    std::vector<const typesystem::IParameterizedMember*> get_user_defined_operator_candidates(const IType* type, const std::string& operator_name) const;

    // ---- casts, conversions ----------------------------------------------------------------------------------------------

    ResolveResult* resolve_cast(const IType* target_type, ResolveResult* expression) const;
    // The cast of a constant between primitive types, with the overflow checking this resolver is in (upstream's internal method; a
    // CSharpOperators lambda calls it); the overflow and invalid cast errors are those of util::CSharpPrimitiveCast.
    ConstantValue csharp_primitive_cast(KnownTypeCode target_type, const ConstantValue& input) const;
    // Converts the input to bool using the rules for boolean expressions (`operator true` is used if a regular conversion to bool is not
    // possible).
    ResolveResult* resolve_condition(ResolveResult* input) const;
    // Converts the negated input to bool: !(bool)input if the implicit cast to bool is valid, else input.operator false().
    ResolveResult* resolve_condition_false(ResolveResult* input) const;
    ResolveResult* resolve_conditional(ResolveResult* condition, ResolveResult* true_expression, ResolveResult* false_expression) const;

    // ---- names -----------------------------------------------------------------------------------------------------------

    ResolveResult* resolve_simple_name(const std::string& identifier, const std::vector<const IType*>& type_arguments, bool is_invocation_target = false) const;
    ResolveResult* lookup_simple_name_or_type_name(const std::string& identifier, const std::vector<const IType*>& type_arguments, NameLookupMode lookup_mode) const;
    // Whether `rr` is a member or local and `identifier` as a type name has the same type (the type name result is set when it is a type).
    bool is_variable_reference_with_same_type(ResolveResult* rr, const std::string& identifier, semantics::TypeResolveResult** trr) const;
    // Looks up an alias (the identifier in front of the :: operator).
    ResolveResult* resolve_alias(const std::string& identifier) const;
    ResolveResult* resolve_member_access(ResolveResult* target, const std::string& identifier, const std::vector<const IType*>& type_arguments,
                                         NameLookupMode lookup_mode = NameLookupMode::Expression) const;
    ResolveResult* resolve_identifier_in_object_initializer(const std::string& identifier) const;

    // A MemberLookup using this resolver's settings.
    MemberLookup create_member_lookup() const;
    // The same; when looking up a base type reference the code counts as outside the current type definition (for accessibility).
    MemberLookup create_member_lookup(NameLookupMode lookup_mode) const;

    // ---- foreach, extension methods --------------------------------------------------------------------------------------

    ForEachResolveResult* resolve_foreach(ResolveResult* expression) const;

    // All extension methods available in the current context ("" for the name: all of them), grouped by using scope (the methods of the
    // innermost scope first). With a non-empty type argument list the results are the specialized methods that have that many type
    // parameters.
    std::vector<std::vector<const typesystem::IMethod*>> get_extension_methods(const std::string& name = {},
                                                                              const std::vector<const IType*>& type_arguments = {}) const;
    // The extension methods called `name` that are applicable with a first argument of `target_type` (null: any); with
    // substitute_inferred_types the method is specialized when type arguments could be inferred from the target type (no effect when
    // type arguments are given).
    std::vector<std::vector<const typesystem::IMethod*>> get_extension_methods(const IType* target_type, const std::string& name = {},
                                                                              const std::vector<const IType*>& type_arguments = {},
                                                                              bool substitute_inferred_types = false) const;
    // Whether the extension method is eligible on the target type. use_type_inference: whether to infer the method's type arguments (false
    // when it is already parameterized). out_inferred_types (may be null) receives the inferred type arguments when at least some could be
    // inferred (a slot with an uninferred argument holds the method's type parameter); it is cleared first.
    static bool is_eligible_extension_method(const IType* target_type, const typesystem::IMethod* method, bool use_type_inference,
                                             std::vector<const IType*>* out_inferred_types);

    // ---- invocations, indexers, creations --------------------------------------------------------------------------------

    // Resolves an invocation; the target is usually a MethodGroupResolveResult. Returns an InvocationResolveResult or an
    // UnknownMethodResolveResult.
    ResolveResult* resolve_invocation(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names = {}) const;
    // Resolves an indexer access: an ArrayAccessResolveResult, InvocationResolveResult or ErrorResolveResult.
    ResolveResult* resolve_indexer(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names = {}) const;
    // Resolves an object creation. allow_protected_access: whether to allow calling protected constructors (only when resolving
    // constructor initializers). initializer_statements: those of an object or collection initializer.
    ResolveResult* resolve_object_creation(const IType* type, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names = {},
                                           bool allow_protected_access = false, const std::vector<ResolveResult*>& initializer_statements = {}) const;
    // Resolves an array creation; element_type null: an implicitly typed array creation. The size arguments' count is the number of
    // dimensions; a negative size is an error. initializer_elements (null: no initializer) are wrapped in conversions.
    ArrayCreateResolveResult* resolve_array_creation(const IType* element_type, const std::vector<int>& size_arguments,
                                                     std::vector<ResolveResult*>* initializer_elements = nullptr) const;
    ArrayCreateResolveResult* resolve_array_creation(const IType* element_type, std::vector<ResolveResult*>& size_arguments,
                                                     std::vector<ResolveResult*>* initializer_elements = nullptr) const;

    // ---- the rest --------------------------------------------------------------------------------------------------------

    ResolveResult* resolve_size_of(const IType* type) const;
    ResolveResult* resolve_this_reference() const;
    ResolveResult* resolve_base_reference() const;
    // The constant of a C# value (null for an empty one).
    ResolveResult* resolve_primitive(const ConstantValue& value) const;
    ResolveResult* resolve_default_value(const IType* type) const;
    // The default value of a type as a constant (null for the types that have no primitive one).
    static ConstantValue get_default_value(const IType* type);
    ResolveResult* resolve_type_of(const IType* referenced_type) const;

    // Whether the call of an extension method as a static method can be written with extension method syntax.
    bool can_transform_to_extension_method_call(const typesystem::IMethod* method, const std::vector<const IType*>& type_arguments, ResolveResult* target,
                                                std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) const;
    bool can_transform_to_extension_method_call(const typesystem::IMethod* method, bool ignore_type_arguments = false, bool ignore_argument_names = true) const;

private:
    struct TypeDefinitionCache;
    struct VariableScopeNode;
    struct ObjectInitializerContext;

    CSharpResolver with_context(const CSharpTypeResolveContext& new_context) const;
    CSharpResolver with_local_variable_stack(std::shared_ptr<const VariableScopeNode> stack) const;
    CSharpResolver with_object_initializer_stack(std::shared_ptr<const ObjectInitializerContext> stack) const;

    // unary operators
    semantics::OperatorResolveResult* unary_operator_resolve_result(const IType* result_type, UnaryOperatorType op, ResolveResult* expression,
                                                           bool is_lifted = false) const;
    ResolveResult* unary_numeric_promotion(UnaryOperatorType op, const IType*& type, bool is_nullable, ResolveResult* expression) const;
    static std::string get_overloadable_operator_name(UnaryOperatorType op);

    // pointer arithmetic: the built-in operator on a pointer and an integer (upstream's PointerArithmeticOperator overloads)
    CSharpOperators::BinaryOperatorMethod* pointer_arithmetic_operator(const IType* result_type, const IType* input_type1, KnownTypeCode input_type2) const;
    CSharpOperators::BinaryOperatorMethod* pointer_arithmetic_operator(const IType* result_type, KnownTypeCode input_type1, const IType* input_type2) const;
    CSharpOperators::BinaryOperatorMethod* pointer_arithmetic_operator(const IType* result_type, const IType* input_type1, const IType* input_type2) const;

    // binary operators
    bool is_nullable_type_or_non_value_type(const IType* type) const;
    ResolveResult* binary_operator_resolve_result(const IType* result_type, ResolveResult* lhs, BinaryOperatorType op, ResolveResult* rhs,
                                                  bool is_lifted = false) const;
    const IType* get_enum_underlying_type(const IType* enum_type) const;
    ResolveResult* handle_enum_comparison(BinaryOperatorType op, const IType* enum_type, bool is_nullable, ResolveResult* lhs, ResolveResult* rhs) const;
    ResolveResult* handle_enum_subtraction(bool is_nullable, const IType* enum_type, ResolveResult* lhs, ResolveResult* rhs) const;
    ResolveResult* handle_enum_operator(bool is_nullable, const IType* enum_type, BinaryOperatorType op, ResolveResult* lhs, ResolveResult* rhs) const;
    const IType* make_nullable(const IType* type, bool is_nullable) const;
    bool binary_numeric_promotion(bool is_nullable, ResolveResult*& lhs, ResolveResult*& rhs, bool allow_nullable_constants) const;
    bool is_signed(KnownTypeCode code, ResolveResult* rr) const;
    ResolveResult* cast_to(KnownTypeCode target_type, bool is_nullable, ResolveResult* expression, bool allow_nullable_constants) const;
    ResolveResult* cast_to(const IType* target_type, bool is_nullable, ResolveResult* expression, bool allow_nullable_constants) const;
    static std::string get_overloadable_operator_name(BinaryOperatorType op);
    ResolveResult* resolve_null_coalescing_operator(ResolveResult* lhs, ResolveResult* rhs) const;
    void lift_user_defined_operators(std::vector<const typesystem::IMethod*>& operators) const;
    ResolveResult* create_resolve_result_for_user_defined_operator(OverloadResolution& r, il::ExpressionType operator_type) const;

    // conversions
    bool try_convert(ResolveResult*& rr, const IType* target_type) const;
    bool try_convert_enum(ResolveResult*& rr, const IType* target_type, bool& is_nullable, ResolveResult*& enum_rr,
                          bool allow_conversion_from_constant_zero = true) const;
    ResolveResult* convert(ResolveResult* rr, const IType* target_type) const;
    ResolveResult* convert(ResolveResult* rr, const IType* target_type, const semantics::Conversion* c) const;

    // names
    ResolveResult* look_in_current_type(const std::string& identifier, const std::vector<const IType*>& type_arguments, NameLookupMode lookup_mode,
                                        bool parameterize_result_type) const;
    ResolveResult* look_in_current_using_scope(const std::string& identifier, const std::vector<const IType*>& type_arguments, bool is_in_using_declaration,
                                               bool parameterize_result_type) const;
    ResolveResult* look_in_using_scope_namespace(const UsingScope* using_scope, const typesystem::INamespace* n, const std::string& identifier,
                                                 const std::vector<const IType*>& type_arguments, bool parameterize_result_type) const;
    bool top_level_type_definition_is_accessible(const typesystem::ITypeDefinition* type_def) const;
    ResolveResult* resolve_extern_alias(const std::string& alias) const;
    ResolveResult* resolve_member_access_on_namespace(semantics::NamespaceResolveResult* nrr, const std::string& identifier,
                                                      const std::vector<const IType*>& type_arguments, bool parameterize_result_type) const;

    // foreach
    void check_for_enumerable_interface(ResolveResult* expression, const IType*& collection_type, const IType*& enumerator_type, const IType*& element_type,
                                        ResolveResult*& get_enumerator_invocation) const;

    // extension methods
    static bool is_eligible_extension_method(const ICompilation& compilation, CSharpConversions& conversions, const IType* target_type,
                                             const typesystem::IMethod* method, bool use_type_inference, std::vector<const IType*>* out_inferred_types);
    std::vector<std::vector<const typesystem::IMethod*>> get_all_extension_methods(const MemberLookup& lookup) const;
    std::vector<const typesystem::IMethod*> get_extension_methods(const MemberLookup& lookup, const typesystem::INamespace* ns) const;

    // invocations
    ResolveResult* resolve_invocation(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names,
                                      bool allow_optional_parameters) const;
    static std::vector<ResolveResult*> add_argument_names_if_necessary(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names);
    std::vector<const typesystem::IParameter*> create_parameters(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) const;
    static std::string guess_parameter_name(ResolveResult* rr);
    static std::string make_parameter_name(const std::string& variable_name);
    OverloadResolution create_overload_resolution(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names = {},
                                                  const std::vector<const IType*>& type_arguments = {}) const;
    void adjust_array_access_arguments(std::vector<ResolveResult*>& arguments) const;
    ResolveResult* error_result() const;
    static bool has_type(ResolveResult* r);
    bool is_better_conditional_conversion(const semantics::Conversion* c1, const semantics::Conversion* c2) const;

    const ICompilation* compilation_;
    CSharpConversions* conversions_;
    CSharpTypeResolveContext context_;
    bool check_for_overflow_ = false;
    bool is_within_lambda_expression_ = false;
    std::shared_ptr<TypeDefinitionCache> current_type_definition_cache_;
    // The local variables as a stack of dictionaries (a new block pushes one), shared by the resolvers made from this one.
    std::shared_ptr<const VariableScopeNode> local_variable_stack_;
    std::shared_ptr<const ObjectInitializerContext> object_initializer_stack_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
