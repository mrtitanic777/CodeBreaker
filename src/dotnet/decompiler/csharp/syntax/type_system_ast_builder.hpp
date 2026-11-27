// dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp -- turns the type system's types, entities, attributes and constants into
// C# syntax nodes: a type as the shortest name that still means that type where it is written, a method or a type as its declaration,
// an attribute as `[Name(args)]`, a constant as a literal (or `int.MaxValue`, `A | B` for flag enums, `1f / 3`, `Math.PI` ...).
//
//     TypeSystemAstBuilder builder(&resolver);          // names are made short where `resolver`'s scope allows it
//     builder.show_attributes = true;
//     AstType* t = builder.convert_type(list_of_int);   // List<int>
//     EntityDeclaration* m = builder.convert_entity(method);
//     Expression* c = builder.convert_constant_value(int_type, ConstantValue(std::int32_t(42)));
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TypeSystemAstBuilder, from TypeSystemAstBuilder.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// How it is held: upstream's options are properties with plain getters and setters; here they are public data members with the same
// names in snake_case (`show_attributes`, `use_aliases` ...) and the same defaults. Every node is made in the thread's current ILArena
// (il::make); the builder keeps no state but its options and the resolver, so one builder can convert any number of things. A constant
// is a typesystem::ConstantValue; an attribute argument, which can also be a type (typeof) or an array, goes through
// convert_attribute_argument_value. Upstream's `IEnumerable<T>` results are std::vector. The value tuple of ConvertExtension is
// ExtensionGroup.
//
// Not ported (waits on the pieces it needs): none of TypeSystemAstBuilder; the documentation lookups of NRExtensions wait on
// ICSharpCode.Decompiler.Documentation.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/resolver/name_lookup_mode.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/modifiers.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"
#include "dotnet/decompiler/typesystem/attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::semantics {
class NamespaceResolveResult;
}
namespace cb::dotnet::decompiler::csharp::resolver {
class CSharpResolver;
class MemberLookup;
}

namespace cb::dotnet::decompiler::csharp::syntax {

class AstType;
class Attribute;
class AttributeSection;
class Constraint;
class EntityDeclaration;
class Expression;
class ParameterDeclaration;
class TypeParameterDeclaration;
class VariableDeclarationStatement;

// What ConvertExtension takes: a C# 14 extension block's marker method and the type parameters of the block.
struct ExtensionGroup {
    const typesystem::IMethod* marker_method = nullptr;
    std::vector<const typesystem::ITypeParameter*> type_parameters;
};

class TypeSystemAstBuilder {
public:
    // `resolver` is initialized for the position where the type will be inserted (a null resolver is an error). Without a resolver every
    // type is written fully qualified.
    explicit TypeSystemAstBuilder(const resolver::CSharpResolver* resolver);
    TypeSystemAstBuilder();

    // ---- options (the defaults are upstream's) ------------------------------------------------------------------------------

    // Whether the builder adds annotations to type references (unused upstream as well).
    bool add_type_reference_annotations = false;
    // Whether the builder adds ResolveResult annotations to nodes.
    bool add_resolve_result_annotations = false;
    // Whether accessibility modifiers are shown.
    bool show_accessibility = true;
    // Whether "private protected" is written (else "protected").
    bool use_private_protected_accessibility = true;
    // Whether the modifiers other than accessibility are shown.
    bool show_modifiers = true;
    bool show_base_types = true;
    bool show_type_parameters = true;
    // Whether the names of the type parameters are shown for unbound types.
    bool show_type_parameters_for_unbound_types = false;
    // Whether the constraints of type parameters are shown (no effect when show_type_parameters is false).
    bool show_type_parameter_constraints = true;
    bool show_parameter_names = true;
    // Whether the default values of optional parameters and the values of constant fields are shown.
    bool show_constant_values = true;
    bool show_attributes = false;
    // Whether attributes are sorted (else they are in metadata order).
    bool sort_attributes = false;
    // Whether short type names are used without looking them up (else fully qualified names unless the resolver finds the short one).
    bool always_use_short_type_names = false;
    bool use_keywords_for_builtin_types = true;
    // Whether T? is written for nullable value types (else Nullable<T>).
    bool use_nullable_specifier_for_value_types = true;
    // How a type name is disambiguated (expression context by default).
    resolver::NameLookupMode name_lookup_mode = resolver::NameLookupMode::Expression;
    // Whether a method gets a body that throws System.NotImplementedException.
    bool generate_body = false;
    // Whether events are written as custom events (with accessors).
    bool use_custom_events = false;
    // Whether the names of unbound type arguments are inserted in the tree.
    bool convert_unbound_type_arguments = false;
    // Whether the aliases of the using scopes are used in type names.
    bool use_aliases = true;
    // Whether int.MaxValue and its like are written as member references (else as literals).
    bool use_special_constants = true;
    bool print_integral_values_as_hex = false;
    // C# 9 `init;` accessors (else `set /*init*/;`).
    bool support_init_accessors = false;
    // C# 9 `record` classes and C# 10 `record struct`s.
    bool support_record_classes = false;
    bool support_record_structs = false;
    // C# 11 `operator >>>` and `operator checked`.
    bool support_unsigned_right_shift = false;
    bool support_operator_checked = false;
    // Whether every fully qualified type name starts with "global::".
    bool always_use_global = false;
    // C# 14 `extension` declarations.
    bool support_extension_declarations = false;

    // ---- types ---------------------------------------------------------------------------------------------------------------

    AstType* convert_type(const typesystem::IType* type);
    // The type of that name: its definition when the resolver's compilation has one, else a name made from the parts.
    AstType* convert_type(const typesystem::FullTypeName& full_type_name);
    // The namespace as a name (`System.Collections`), with the NamespaceResolveResult of the last part in `*nrr` when it is known.
    AstType* convert_namespace(const std::string& namespace_name, semantics::NamespaceResolveResult** nrr);

    // ---- attributes ----------------------------------------------------------------------------------------------------------

    Attribute* convert_attribute(const typesystem::IAttribute* attribute);
    // The attributes as sections (each with the target `target` when it is not empty), sorted when sort_attributes is set.
    std::vector<AttributeSection*> convert_attributes(const std::vector<const typesystem::IAttribute*>& attributes, const std::string& target = {});
    AstType* convert_attribute_type(const typesystem::IType* type);

    // ---- constants -----------------------------------------------------------------------------------------------------------

    // The expression for a resolve result (an expression that is implicitly convertible to rr's type and has its value when it is
    // converted that way).
    Expression* convert_constant_value(semantics::ResolveResult* rr);
    // The expression for a constant of that type: a literal, not necessarily of the type itself (null for a string, 1 for a short),
    // but always implicitly convertible to it.
    Expression* convert_constant_value(const typesystem::IType* type, const typesystem::ConstantValue& constant_value);
    Expression* convert_constant_value(const typesystem::IType* expected_type, const typesystem::IType* type, const typesystem::ConstantValue& constant_value);
    // The same for an attribute's argument: besides a constant it can be a type (typeof(T)) or an array of arguments.
    Expression* convert_attribute_argument_value(const typesystem::IType* expected_type, const typesystem::IType* type,
                                                 const typesystem::AttributeArgumentValue& value);
    // A numeric value of an enum type as its members (`A | B`, `~A`), else as `(E)value`. With declaring_enum_member set (the member
    // being declared) unqualified references are used, self references are avoided and no cast is written.
    Expression* convert_enum_value(const typesystem::IType* type, std::int64_t value, const typesystem::IField* declaring_enum_member = nullptr);

    // ---- parameters, entities, type parameters, variables ---------------------------------------------------------------------

    ParameterDeclaration* convert_parameter(const typesystem::IParameter* parameter);
    // A namespace, variable, parameter, type parameter or entity as its declaration.
    AstNode* convert_symbol(const typesystem::ISymbol* symbol);
    EntityDeclaration* convert_entity(const typesystem::IEntity* entity);
    EntityDeclaration* convert_extension(const ExtensionGroup& group);
    TypeParameterDeclaration* convert_type_parameter(const typesystem::ITypeParameter* tp);
    // The `where` clause of a type parameter (nullptr when it has no constraint to write).
    Constraint* convert_type_parameter_constraint(const typesystem::ITypeParameter* tp);
    VariableDeclarationStatement* convert_variable(const typesystem::IVariable* variable);

    static Modifiers modifier_from_accessibility(typesystem::Accessibility accessibility, bool use_private_protected);

private:
    AstType* convert_type_helper(const typesystem::IType* type);
    AstType* convert_type_helper(const typesystem::IType* generic_type, const std::vector<const typesystem::IType*>& type_arguments);
    void add_type_annotation(AstType* ast_type, const typesystem::IType* type);
    bool type_matches(const typesystem::IType* type, const typesystem::ITypeDefinition* type_def, const std::vector<const typesystem::IType*>& type_arguments);
    static bool type_def_matches(const typesystem::ITypeDefinition* type_def, const typesystem::IType* type);
    void add_type_arguments(AstType* result, const std::vector<const typesystem::ITypeParameter*>& type_parameters,
                            const std::vector<const typesystem::IType*>& type_arguments, int start_index, int end_index);
    AstType* convert_namespace(const std::string& namespace_name, semantics::NamespaceResolveResult** nrr, bool requires_global_prefix);
    bool is_valid_namespace(const std::string& first_namespace_part, semantics::NamespaceResolveResult** nrr);
    SimpleType* make_global();

    void apply_short_attribute_name_if_possible(const typesystem::IType* type, AstType* ast_type, const std::optional<std::string>& short_name);
    static bool is_attribute_type(const typesystem::IType* type);
    static bool is_attribute_type(semantics::ResolveResult* rr);

    bool is_special_constant(const typesystem::IType* expected_type, const typesystem::ConstantValue& constant, Expression** expression);
    bool is_flags_enum(const typesystem::ITypeDefinition* type);
    Expression* convert_floating_point_literal(const typesystem::IType* type, const typesystem::ConstantValue& constant_value);
    Expression* make_constant(const typesystem::IType* type, std::int64_t c);
    Expression* try_extract_expression(const typesystem::IType* math_type, const typesystem::IType* type, const typesystem::ConstantValue& literal_value,
                                       const std::string& member_name, bool is_double);

    EntityDeclaration* convert_type_definition(const typesystem::ITypeDefinition* type_definition);
    bool base_type_accessible_from(const typesystem::IType* base_type, const typesystem::ITypeDefinition* current_type, const resolver::MemberLookup& lookup);
    DelegateDeclaration* convert_delegate(const typesystem::IMethod* invoke_method, Modifiers modifiers);
    FieldDeclaration* convert_field(const typesystem::IField* field);
    BlockStatement* generate_body_block();
    Accessor* convert_accessor(const typesystem::IMethod* accessor, typesystem::MethodSemanticsAttributes kind, typesystem::Accessibility owner_accessibility,
                               bool add_parameter_attribute);
    PropertyDeclaration* convert_property(const typesystem::IProperty* property);
    IndexerDeclaration* convert_indexer(const typesystem::IProperty* indexer);
    EntityDeclaration* convert_event(const typesystem::IEvent* ev);
    MethodDeclaration* convert_method(const typesystem::IMethod* method);
    EntityDeclaration* convert_operator(const typesystem::IMethod* op);
    ConstructorDeclaration* convert_constructor(const typesystem::IMethod* ctor);
    DestructorDeclaration* convert_destructor(const typesystem::IMethod* dtor);
    bool needs_accessibility(const typesystem::IMember* member);
    Modifiers get_member_modifiers(const typesystem::IMember* member);
    void add_nullability_disambiguating_constraints(MethodDeclaration* decl, const typesystem::IMethod* method);
    NamespaceDeclaration* convert_namespace_declaration(const typesystem::INamespace* ns);
    AstType* get_explicit_interface_type(const typesystem::IMember* member);

    const resolver::CSharpResolver* resolver_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
