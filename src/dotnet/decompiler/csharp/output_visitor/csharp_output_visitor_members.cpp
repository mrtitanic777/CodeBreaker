// dotnet/decompiler/csharp/output_visitor/csharp_output_visitor_members.cpp -- CSharpOutputVisitor: the visits of the general scope
// (attributes, namespaces, types, usings), the type members, the types (AstType), variable designations, trivia, the pattern nodes and
// documentation references. See csharp_output_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#include <cassert>
#include <climits>
#include <stdexcept>

#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/backreference.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/choice.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/identifier_expression_backreference.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/optional_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;
namespace pm = syntax::pattern_matching;

// ---- GeneralScope -----------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_attribute(syntax::Attribute& attribute) {
    start_node(&attribute);
    attribute.type()->accept_visitor(*this);
    if (attribute.arguments().count() != 0 || attribute.has_argument_list()) {
        space(policy_.space_before_method_call_parentheses);
        write_comma_separated_list_in_parenthesis(nodes_of(attribute.arguments()), policy_.space_within_method_call_parentheses);
    }
    end_node(&attribute);
}

void CSharpOutputVisitor::visit_attribute_section(AttributeSection& attribute_section) {
    start_node(&attribute_section);
    write_token(Tokens::l_bracket);
    if (!attribute_section.attribute_target().empty()) {
        write_keyword(attribute_section.attribute_target());
        write_token(Tokens::colon);
        space();
    }
    write_comma_separated_list(nodes_of(attribute_section.attributes()));
    write_token(Tokens::r_bracket);
    AstNode* parent = attribute_section.parent();
    if (auto* pd = as<ParameterDeclaration>(parent)) {
        if (pd->attributes().last_or_null() != &attribute_section)
            space(policy_.space_between_parameter_attribute_sections);
        else
            space();
    } else if (is<TypeParameterDeclaration>(parent) || is<ComposedType>(parent) || is<LambdaExpression>(parent)) {
        space();
    } else {
        new_line();
    }
    end_node(&attribute_section);
}

void CSharpOutputVisitor::visit_delegate_declaration(DelegateDeclaration& delegate_declaration) {
    start_node(&delegate_declaration);
    write_attributes(nodes_of(delegate_declaration.attributes()));
    write_modifiers(delegate_declaration.modifiers());
    write_keyword(Tokens::delegate_keyword);
    accept(delegate_declaration.return_type());
    space();
    write_identifier(delegate_declaration.name_token());
    write_type_parameters(nodes_of(delegate_declaration.type_parameters()));
    space(policy_.space_before_delegate_declaration_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(delegate_declaration.parameters()), policy_.space_within_method_declaration_parentheses);
    for (Constraint* constraint : delegate_declaration.constraints()) {
        constraint->accept_visitor(*this);
    }
    semicolon();
    end_node(&delegate_declaration);
}

void CSharpOutputVisitor::visit_namespace_declaration(NamespaceDeclaration& namespace_declaration) {
    start_node(&namespace_declaration);
    write_keyword(Tokens::namespace_keyword);
    namespace_declaration.namespace_name()->accept_visitor(*this);
    if (namespace_declaration.is_file_scoped()) {
        semicolon();
        new_line();
    } else {
        open_brace(policy_.namespace_brace_style);
    }
    for (AstNode* member : namespace_declaration.members()) {
        member->accept_visitor(*this);
        maybe_new_lines_after_usings(member);
    }
    if (!namespace_declaration.is_file_scoped()) {
        close_brace(policy_.namespace_brace_style);
        new_line();
    }
    end_node(&namespace_declaration);
}

void CSharpOutputVisitor::visit_type_declaration(TypeDeclaration& type_declaration) {
    start_node(&type_declaration);
    write_attributes(nodes_of(type_declaration.attributes()));
    write_modifiers(type_declaration.modifiers());
    BraceStyle brace_style;
    switch (type_declaration.class_type()) {
        case ClassType::Enum:
            write_keyword(Tokens::enum_keyword);
            brace_style = policy_.enum_brace_style;
            break;
        case ClassType::Interface:
            write_keyword(Tokens::interface_keyword);
            brace_style = policy_.interface_brace_style;
            break;
        case ClassType::Struct:
            write_keyword(Tokens::struct_keyword);
            brace_style = policy_.struct_brace_style;
            break;
        case ClassType::RecordClass:
            write_keyword(Tokens::record_keyword);
            brace_style = policy_.class_brace_style;
            break;
        case ClassType::RecordStruct:
            write_keyword(Tokens::record_struct_keyword);
            write_keyword(Tokens::struct_keyword);
            brace_style = policy_.struct_brace_style;
            break;
        default:
            write_keyword(Tokens::class_keyword);
            brace_style = policy_.class_brace_style;
            break;
    }
    write_identifier(type_declaration.name_token());
    write_type_parameters(nodes_of(type_declaration.type_parameters()));
    if (type_declaration.has_primary_constructor()) {
        space(policy_.space_before_method_declaration_parentheses);
        write_comma_separated_list_in_parenthesis(nodes_of(type_declaration.primary_constructor_parameters()),
                                                  policy_.space_within_method_declaration_parentheses);
    }
    if (!type_declaration.base_types().empty()) {
        space();
        write_token(Tokens::colon);
        space();
        write_comma_separated_list(nodes_of(type_declaration.base_types()));
    }
    for (Constraint* constraint : type_declaration.constraints()) {
        constraint->accept_visitor(*this);
    }
    if ((type_declaration.class_type() == ClassType::RecordClass || type_declaration.class_type() == ClassType::RecordStruct) &&
        type_declaration.members().count() == 0) {
        semicolon();
    } else {
        open_brace(brace_style);
        if (type_declaration.class_type() == ClassType::Enum) {
            bool first = true;
            for (EntityDeclaration* member : type_declaration.members()) {
                if (first) {
                    first = false;
                } else {
                    comma(member, true);
                    new_line();
                }
                member->accept_visitor(*this);
            }
            new_line();
        } else {
            bool first = true;
            for (EntityDeclaration* member : type_declaration.members()) {
                if (!first) {
                    for (int i = 0; i < policy_.minimum_blank_lines_between_members; i++) new_line();
                }
                first = false;
                member->accept_visitor(*this);
            }
        }
        close_brace(brace_style);
        new_line();
    }
    end_node(&type_declaration);
}

void CSharpOutputVisitor::visit_using_alias_declaration(UsingAliasDeclaration& using_alias_declaration) {
    start_node(&using_alias_declaration);
    write_keyword(UsingAliasDeclaration::using_keyword);
    write_identifier(using_alias_declaration.alias_token());
    space(policy_.space_around_equality_operator);
    write_token(Tokens::assign);
    space(policy_.space_around_equality_operator);
    using_alias_declaration.import()->accept_visitor(*this);
    semicolon();
    end_node(&using_alias_declaration);
}

void CSharpOutputVisitor::visit_using_declaration(UsingDeclaration& using_declaration) {
    start_node(&using_declaration);
    write_keyword(UsingDeclaration::using_keyword);
    using_declaration.import()->accept_visitor(*this);
    semicolon();
    end_node(&using_declaration);
}

void CSharpOutputVisitor::visit_extern_alias_declaration(ExternAliasDeclaration& extern_alias_declaration) {
    start_node(&extern_alias_declaration);
    write_keyword(Tokens::extern_keyword);
    space();
    write_keyword(Tokens::alias_keyword);
    space();
    write_identifier(extern_alias_declaration.name_token());
    semicolon();
    end_node(&extern_alias_declaration);
}

// ---- TypeMembers ------------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_accessor(Accessor& accessor) {
    start_node(&accessor);
    write_attributes(nodes_of(accessor.attributes()));
    write_modifiers(accessor.modifiers());
    BraceStyle style = policy_.statement_brace_style;
    const CSharpSlotInfo* kind = slot_kind_of(&accessor);
    if (kind == &Slots::getter) {
        write_keyword("get");
        style = policy_.property_get_brace_style;
    } else if (kind == &Slots::setter) {
        if (accessor.accessor_kind() == AccessorKind::Init) {
            write_keyword("init");
        } else {
            write_keyword("set");
        }
        style = policy_.property_set_brace_style;
    } else if (kind == &Slots::add_accessor) {
        write_keyword("add");
        style = policy_.event_add_brace_style;
    } else if (kind == &Slots::remove_accessor) {
        write_keyword("remove");
        style = policy_.event_remove_brace_style;
    }
    write_method_body(accessor.body(), style);
    end_node(&accessor);
}

void CSharpOutputVisitor::visit_constructor_declaration(ConstructorDeclaration& constructor_declaration) {
    start_node(&constructor_declaration);
    write_attributes(nodes_of(constructor_declaration.attributes()));
    write_modifiers(constructor_declaration.modifiers());
    TypeDeclaration* type = as<TypeDeclaration>(constructor_declaration.parent());
    if (type != nullptr && type->name() != constructor_declaration.name())
        write_identifier(clone_as(type->name_token()));
    else
        write_identifier(constructor_declaration.name_token());
    space(policy_.space_before_constructor_declaration_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(constructor_declaration.parameters()), policy_.space_within_method_declaration_parentheses);
    if (constructor_declaration.initializer() != nullptr) {
        new_line();
        writer_.indent();
        constructor_declaration.initializer()->accept_visitor(*this);
        writer_.unindent();
    }
    write_method_body(constructor_declaration.body(), policy_.constructor_brace_style);
    end_node(&constructor_declaration);
}

void CSharpOutputVisitor::visit_constructor_initializer(ConstructorInitializer& constructor_initializer) {
    start_node(&constructor_initializer);
    write_token(Tokens::colon);
    space();
    if (constructor_initializer.constructor_initializer_type() == ConstructorInitializerType::This) {
        write_keyword(ConstructorInitializer::this_keyword);
    } else {
        write_keyword(ConstructorInitializer::base_keyword);
    }
    space(policy_.space_before_method_call_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(constructor_initializer.arguments()), policy_.space_within_method_call_parentheses);
    end_node(&constructor_initializer);
}

void CSharpOutputVisitor::visit_destructor_declaration(DestructorDeclaration& destructor_declaration) {
    start_node(&destructor_declaration);
    write_attributes(nodes_of(destructor_declaration.attributes()));
    write_modifiers(destructor_declaration.modifiers());
    if (destructor_declaration.modifiers() != Modifiers::None) {
        space();
    }
    write_token(DestructorDeclaration::tilde_token);
    TypeDeclaration* type = as<TypeDeclaration>(destructor_declaration.parent());
    if (type != nullptr && type->name() != destructor_declaration.name())
        write_identifier(clone_as(type->name_token()));
    else
        write_identifier(destructor_declaration.name_token());
    space(policy_.space_before_constructor_declaration_parentheses);
    l_par();
    r_par();
    write_method_body(destructor_declaration.body(), policy_.destructor_brace_style);
    end_node(&destructor_declaration);
}

void CSharpOutputVisitor::visit_enum_member_declaration(EnumMemberDeclaration& enum_member_declaration) {
    start_node(&enum_member_declaration);
    write_attributes(nodes_of(enum_member_declaration.attributes()));
    write_modifiers(enum_member_declaration.modifiers());
    write_identifier(enum_member_declaration.name_token());
    if (enum_member_declaration.initializer() != nullptr) {
        space(policy_.space_around_assignment);
        write_token(Tokens::assign);
        space(policy_.space_around_assignment);
        enum_member_declaration.initializer()->accept_visitor(*this);
    }
    end_node(&enum_member_declaration);
}

void CSharpOutputVisitor::visit_extension_declaration(ExtensionDeclaration& extension_declaration) {
    start_node(&extension_declaration);
    write_attributes(nodes_of(extension_declaration.attributes()));
    write_modifiers(extension_declaration.modifiers());
    write_keyword(ExtensionDeclaration::extension_keyword);
    write_type_parameters(nodes_of(extension_declaration.type_parameters()));
    space(policy_.space_before_method_declaration_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(extension_declaration.receiver_parameters()), policy_.space_within_method_declaration_parentheses);
    for (Constraint* constraint : extension_declaration.constraints()) {
        constraint->accept_visitor(*this);
    }
    open_brace(policy_.class_brace_style);
    bool first = true;
    for (EntityDeclaration* member : extension_declaration.members()) {
        if (!first) {
            for (int i = 0; i < policy_.minimum_blank_lines_between_members; i++) new_line();
        }
        first = false;
        member->accept_visitor(*this);
    }
    close_brace(policy_.class_brace_style);
    new_line();
    end_node(&extension_declaration);
}

void CSharpOutputVisitor::visit_event_declaration(EventDeclaration& event_declaration) {
    start_node(&event_declaration);
    write_attributes(nodes_of(event_declaration.attributes()));
    write_modifiers(event_declaration.modifiers());
    write_keyword(EventDeclaration::event_keyword);
    accept(event_declaration.return_type());
    space();
    write_comma_separated_list(nodes_of(event_declaration.variables()));
    semicolon();
    end_node(&event_declaration);
}

void CSharpOutputVisitor::visit_custom_event_declaration(CustomEventDeclaration& custom_event_declaration) {
    start_node(&custom_event_declaration);
    write_attributes(nodes_of(custom_event_declaration.attributes()));
    write_modifiers(custom_event_declaration.modifiers());
    write_keyword(CustomEventDeclaration::event_keyword);
    accept(custom_event_declaration.return_type());
    space();
    write_private_implementation_type(custom_event_declaration.private_implementation_type());
    write_identifier(custom_event_declaration.name_token());
    open_brace(policy_.event_brace_style);

    for (AstNode* node = custom_event_declaration.first_child(); node != nullptr; node = node->next_sibling()) {
        const CSharpSlotInfo* kind = slot_kind_of(node);
        if (kind == &Slots::add_accessor || kind == &Slots::remove_accessor) {
            node->accept_visitor(*this);
        }
    }

    close_brace(policy_.event_brace_style);
    new_line();
    end_node(&custom_event_declaration);
}

void CSharpOutputVisitor::visit_field_declaration(FieldDeclaration& field_declaration) {
    start_node(&field_declaration);
    write_attributes(nodes_of(field_declaration.attributes()));
    write_modifiers(field_declaration.modifiers());
    accept(field_declaration.return_type());
    space();
    write_comma_separated_list(nodes_of(field_declaration.variables()));
    semicolon();
    end_node(&field_declaration);
}

void CSharpOutputVisitor::visit_fixed_field_declaration(FixedFieldDeclaration& fixed_field_declaration) {
    start_node(&fixed_field_declaration);
    write_attributes(nodes_of(fixed_field_declaration.attributes()));
    write_modifiers(fixed_field_declaration.modifiers());
    write_keyword(FixedFieldDeclaration::fixed_keyword);
    space();
    accept(fixed_field_declaration.return_type());
    space();
    write_comma_separated_list(nodes_of(fixed_field_declaration.variables()));
    semicolon();
    end_node(&fixed_field_declaration);
}

void CSharpOutputVisitor::visit_fixed_variable_initializer(FixedVariableInitializer& fixed_variable_initializer) {
    start_node(&fixed_variable_initializer);
    write_identifier(fixed_variable_initializer.name_token());
    if (fixed_variable_initializer.count_expression() != nullptr) {
        write_token(Tokens::l_bracket);
        space(policy_.spaces_within_brackets);
        fixed_variable_initializer.count_expression()->accept_visitor(*this);
        space(policy_.spaces_within_brackets);
        write_token(Tokens::r_bracket);
    }
    end_node(&fixed_variable_initializer);
}

void CSharpOutputVisitor::visit_indexer_declaration(IndexerDeclaration& indexer_declaration) {
    start_node(&indexer_declaration);
    write_attributes(nodes_of(indexer_declaration.attributes()));
    write_modifiers(indexer_declaration.modifiers());
    accept(indexer_declaration.return_type());
    space();
    write_private_implementation_type(indexer_declaration.private_implementation_type());
    write_keyword(IndexerDeclaration::this_keyword);
    space(policy_.space_before_method_declaration_parentheses);
    write_comma_separated_list_in_brackets(nodes_of(indexer_declaration.parameters()), policy_.space_within_method_declaration_parentheses);
    if (indexer_declaration.expression_body() == nullptr) {
        bool is_single_line = (policy_.auto_property_formatting == PropertyFormatting::SingleLine) &&
                              (indexer_declaration.getter() == nullptr || indexer_declaration.getter()->body() == nullptr) &&
                              (indexer_declaration.setter() == nullptr || indexer_declaration.setter()->body() == nullptr) &&
                              (indexer_declaration.getter() == nullptr || indexer_declaration.getter()->attributes().empty()) &&
                              (indexer_declaration.setter() == nullptr || indexer_declaration.setter()->attributes().empty());
        open_brace(is_single_line ? BraceStyle::EndOfLine : policy_.property_brace_style, !is_single_line);
        if (is_single_line) space();

        for (AstNode* node = indexer_declaration.first_child(); node != nullptr; node = node->next_sibling()) {
            const CSharpSlotInfo* kind = slot_kind_of(node);
            if (kind == &Slots::getter || kind == &Slots::setter) {
                node->accept_visitor(*this);
            }
        }

        close_brace(is_single_line ? BraceStyle::EndOfLine : policy_.property_brace_style, !is_single_line);
        new_line();
    } else {
        space();
        write_token(Tokens::arrow);
        space();
        indexer_declaration.expression_body()->accept_visitor(*this);
        semicolon();
    }
    end_node(&indexer_declaration);
}

void CSharpOutputVisitor::visit_method_declaration(MethodDeclaration& method_declaration) {
    start_node(&method_declaration);
    write_attributes(nodes_of(method_declaration.attributes()));
    write_modifiers(method_declaration.modifiers());
    accept(method_declaration.return_type());
    space();
    write_private_implementation_type(method_declaration.private_implementation_type());
    write_identifier(method_declaration.name_token());
    write_type_parameters(nodes_of(method_declaration.type_parameters()));
    space(policy_.space_before_method_declaration_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(method_declaration.parameters()), policy_.space_within_method_declaration_parentheses);
    for (Constraint* constraint : method_declaration.constraints()) {
        constraint->accept_visitor(*this);
    }
    write_method_body(method_declaration.body(), policy_.method_brace_style);
    end_node(&method_declaration);
}

void CSharpOutputVisitor::visit_operator_declaration(OperatorDeclaration& operator_declaration) {
    start_node(&operator_declaration);
    write_attributes(nodes_of(operator_declaration.attributes()));
    write_modifiers(operator_declaration.modifiers());
    if (operator_declaration.operator_type() == OperatorType::Explicit || operator_declaration.operator_type() == OperatorType::CheckedExplicit) {
        write_keyword(OperatorDeclaration::explicit_keyword);
    } else if (operator_declaration.operator_type() == OperatorType::Implicit) {
        write_keyword(OperatorDeclaration::implicit_keyword);
    } else {
        accept(operator_declaration.return_type());
    }
    space();
    write_private_implementation_type(operator_declaration.private_implementation_type());
    write_keyword(OperatorDeclaration::operator_keyword);
    space();
    if (OperatorDeclaration::is_checked(operator_declaration.operator_type())) {
        write_keyword(OperatorDeclaration::checked_keyword);
        space();
    }
    if (operator_declaration.operator_type() == OperatorType::Explicit || operator_declaration.operator_type() == OperatorType::CheckedExplicit ||
        operator_declaration.operator_type() == OperatorType::Implicit) {
        accept(operator_declaration.return_type());
    } else {
        write_token(OperatorDeclaration::get_token(operator_declaration.operator_type()));
    }
    space(policy_.space_before_method_declaration_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(operator_declaration.parameters()), policy_.space_within_method_declaration_parentheses);
    write_method_body(operator_declaration.body(), policy_.method_brace_style);
    end_node(&operator_declaration);
}

void CSharpOutputVisitor::visit_parameter_declaration(ParameterDeclaration& parameter_declaration) {
    start_node(&parameter_declaration);
    write_attributes(nodes_of(parameter_declaration.attributes()));
    if (parameter_declaration.has_this_modifier()) {
        write_keyword(ParameterDeclaration::this_modifier);
        space();
    }
    if (parameter_declaration.is_params()) {
        write_keyword(ParameterDeclaration::params_modifier);
        space();
    }
    if (parameter_declaration.is_scoped_ref()) {
        write_keyword(ParameterDeclaration::scoped_ref_keyword);
        space();
    }
    switch (parameter_declaration.parameter_modifier()) {
        case typesystem::ReferenceKind::Ref:
            write_keyword(ParameterDeclaration::ref_modifier);
            space();
            break;
        case typesystem::ReferenceKind::RefReadOnly:
            write_keyword(ParameterDeclaration::ref_modifier);
            write_keyword(ParameterDeclaration::readonly_modifier);
            space();
            break;
        case typesystem::ReferenceKind::Out:
            write_keyword(ParameterDeclaration::out_modifier);
            space();
            break;
        case typesystem::ReferenceKind::In:
            write_keyword(ParameterDeclaration::in_modifier);
            space();
            break;
        default:
            break;
    }
    accept(parameter_declaration.type());
    if (parameter_declaration.type() != nullptr && !parameter_declaration.name().empty()) {
        space();
    }
    if (!parameter_declaration.name().empty()) {
        write_identifier(parameter_declaration.name_token());
    }
    if (parameter_declaration.default_expression() != nullptr) {
        space(policy_.space_around_assignment);
        write_token(Tokens::assign);
        space(policy_.space_around_assignment);
        parameter_declaration.default_expression()->accept_visitor(*this);
    }
    end_node(&parameter_declaration);
}

void CSharpOutputVisitor::visit_property_declaration(PropertyDeclaration& property_declaration) {
    start_node(&property_declaration);
    write_attributes(nodes_of(property_declaration.attributes()));
    write_modifiers(property_declaration.modifiers());
    accept(property_declaration.return_type());
    space();
    write_private_implementation_type(property_declaration.private_implementation_type());
    write_identifier(property_declaration.name_token());
    if (property_declaration.expression_body() == nullptr) {
        bool is_single_line = (policy_.auto_property_formatting == PropertyFormatting::SingleLine) &&
                              (property_declaration.getter() == nullptr || property_declaration.getter()->body() == nullptr) &&
                              (property_declaration.setter() == nullptr || property_declaration.setter()->body() == nullptr) &&
                              (property_declaration.getter() == nullptr || property_declaration.getter()->attributes().empty()) &&
                              (property_declaration.setter() == nullptr || property_declaration.setter()->attributes().empty());
        open_brace(is_single_line ? BraceStyle::EndOfLine : policy_.property_brace_style, !is_single_line);
        if (is_single_line) space();

        for (AstNode* node = property_declaration.first_child(); node != nullptr; node = node->next_sibling()) {
            const CSharpSlotInfo* kind = slot_kind_of(node);
            if (kind == &Slots::getter || kind == &Slots::setter) {
                node->accept_visitor(*this);
            }
        }

        close_brace(is_single_line ? BraceStyle::EndOfLine : policy_.property_brace_style, !is_single_line);
        if (property_declaration.initializer() != nullptr) {
            space(policy_.space_around_assignment);
            write_token(Tokens::assign);
            space(policy_.space_around_assignment);
            property_declaration.initializer()->accept_visitor(*this);
            semicolon();
        } else {
            new_line();
        }
    } else {
        space();
        write_token(Tokens::arrow);
        space();
        property_declaration.expression_body()->accept_visitor(*this);
        semicolon();
    }
    end_node(&property_declaration);
}

// ---- Other nodes ------------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_variable_initializer(VariableInitializer& variable_initializer) {
    start_node(&variable_initializer);
    write_identifier(variable_initializer.name_token());
    if (variable_initializer.initializer() != nullptr) {
        space(policy_.space_around_assignment);
        write_token(Tokens::assign);
        space(policy_.space_around_assignment);
        variable_initializer.initializer()->accept_visitor(*this);
    }
    end_node(&variable_initializer);
}

void CSharpOutputVisitor::maybe_new_lines_after_usings(AstNode* node) {
    AstNode* next_sibling = node->next_sibling();
    if ((is<UsingDeclaration>(node) || is<UsingAliasDeclaration>(node)) && !(is<UsingDeclaration>(next_sibling) || is<UsingAliasDeclaration>(next_sibling))) {
        for (int i = 0; i < policy_.minimum_blank_lines_after_usings; i++) new_line();
    }
}

void CSharpOutputVisitor::visit_syntax_tree(SyntaxTree& syntax_tree) {
    // don't do node tracking as we visit all children directly
    for (Trivia* trivia : syntax_tree.leading_trivia()) trivia->accept_visitor(*this);
    for (AstNode* node = syntax_tree.first_child(); node != nullptr; node = node->next_sibling()) {
        node->accept_visitor(*this);
        maybe_new_lines_after_usings(node);
    }
}

void CSharpOutputVisitor::visit_simple_type(SimpleType& simple_type) {
    start_node(&simple_type);
    if (simple_type.identifier_token() != nullptr) write_identifier(simple_type.identifier_token());
    write_type_arguments(nodes_of(simple_type.type_arguments()));
    end_node(&simple_type);
}

void CSharpOutputVisitor::visit_member_type(MemberType& member_type) {
    start_node(&member_type);
    member_type.target()->accept_visitor(*this);
    if (member_type.is_double_colon()) {
        write_token(Tokens::double_colon);
    } else {
        write_token(Tokens::dot);
    }
    write_identifier(member_type.member_name_token());
    write_type_arguments(nodes_of(member_type.type_arguments()));
    end_node(&member_type);
}

void CSharpOutputVisitor::visit_tuple_type(TupleAstType& tuple_type) {
    assert(tuple_type.elements().count() >= 2);
    start_node(&tuple_type);
    l_par();
    write_comma_separated_list(nodes_of(tuple_type.elements()));
    r_par();
    end_node(&tuple_type);
}

void CSharpOutputVisitor::visit_tuple_type_element(TupleTypeElement& tuple_type_element) {
    start_node(&tuple_type_element);
    tuple_type_element.type()->accept_visitor(*this);
    if (tuple_type_element.name_token() != nullptr) {
        space();
        tuple_type_element.name_token()->accept_visitor(*this);
    }
    end_node(&tuple_type_element);
}

void CSharpOutputVisitor::visit_function_pointer_type(FunctionPointerAstType& function_pointer_type) {
    start_node(&function_pointer_type);
    write_keyword(Tokens::delegate_keyword);
    write_token(FunctionPointerAstType::pointer_token);
    if (function_pointer_type.has_unmanaged_calling_convention()) {
        space();
        write_keyword("unmanaged");
    }
    if (!function_pointer_type.calling_conventions().empty()) {
        write_token(Tokens::l_bracket);
        write_comma_separated_list(nodes_of(function_pointer_type.calling_conventions()));
        write_token(Tokens::r_bracket);
    }
    write_token(Tokens::l_chevron);
    std::vector<AstNode*> parameters_and_return_type = nodes_of(function_pointer_type.parameters());
    parameters_and_return_type.push_back(function_pointer_type.return_type());
    write_comma_separated_list(parameters_and_return_type);
    write_token(Tokens::r_chevron);
    end_node(&function_pointer_type);
}

void CSharpOutputVisitor::visit_invocation_type(InvocationAstType& invocation_type) {
    start_node(&invocation_type);
    invocation_type.base_type()->accept_visitor(*this);
    write_token(Tokens::l_par);
    write_comma_separated_list(nodes_of(invocation_type.arguments()));
    write_token(Tokens::r_par);
    end_node(&invocation_type);
}

void CSharpOutputVisitor::visit_composed_type(ComposedType& composed_type) {
    start_node(&composed_type);
    if (!composed_type.attributes().empty()) {
        for (AttributeSection* attr : composed_type.attributes()) {
            attr->accept_visitor(*this);
        }
    }
    if (composed_type.has_ref_specifier()) {
        write_keyword(ComposedType::ref_keyword);
    }
    if (composed_type.has_read_only_specifier()) {
        write_keyword(ComposedType::readonly_keyword);
    }
    composed_type.base_type()->accept_visitor(*this);
    if (composed_type.has_nullable_specifier()) {
        write_token(ComposedType::nullable_token);
    }
    for (int i = 0; i < composed_type.pointer_rank(); i++) {
        write_token(ComposedType::pointer_token);
    }
    for (ArraySpecifier* node : composed_type.array_specifiers()) {
        node->accept_visitor(*this);
    }
    end_node(&composed_type);
}

void CSharpOutputVisitor::visit_array_specifier(ArraySpecifier& array_specifier) {
    start_node(&array_specifier);
    write_token(Tokens::l_bracket);
    for (int i = 0; i < array_specifier.dimensions() - 1; i++) {
        writer_.write_token(",");
    }
    write_token(Tokens::r_bracket);
    end_node(&array_specifier);
}

void CSharpOutputVisitor::visit_primitive_type(PrimitiveType& primitive_type) {
    start_node(&primitive_type);
    writer_.write_primitive_type(primitive_type.keyword());
    is_after_space_ = false;
    end_node(&primitive_type);
}

void CSharpOutputVisitor::visit_single_variable_designation(SingleVariableDesignation& single_variable_designation) {
    start_node(&single_variable_designation);
    write_identifier(single_variable_designation.identifier_token());
    end_node(&single_variable_designation);
}

void CSharpOutputVisitor::visit_parenthesized_variable_designation(ParenthesizedVariableDesignation& parenthesized_variable_designation) {
    start_node(&parenthesized_variable_designation);
    l_par();
    write_comma_separated_list(nodes_of(parenthesized_variable_designation.variable_designations()));
    r_par();
    end_node(&parenthesized_variable_designation);
}

void CSharpOutputVisitor::visit_comment(Comment& comment) {
    writer_.start_node(comment);
    writer_.write_comment(comment.comment_type(), comment.content());
    writer_.end_node(comment);
}

void CSharpOutputVisitor::visit_pre_processor_directive(PreProcessorDirective& pre_processor_directive) {
    writer_.start_node(pre_processor_directive);
    writer_.write_pre_processor_directive(pre_processor_directive.type(), pre_processor_directive.argument());
    writer_.end_node(pre_processor_directive);
}

void CSharpOutputVisitor::visit_type_parameter_declaration(TypeParameterDeclaration& type_parameter_declaration) {
    start_node(&type_parameter_declaration);
    write_attributes(nodes_of(type_parameter_declaration.attributes()));
    switch (type_parameter_declaration.variance()) {
        case typesystem::VarianceModifier::Invariant:
            break;
        case typesystem::VarianceModifier::Covariant:
            write_keyword(TypeParameterDeclaration::out_variance_keyword);
            break;
        case typesystem::VarianceModifier::Contravariant:
            write_keyword(TypeParameterDeclaration::in_variance_keyword);
            break;
        default:
            throw std::logic_error("Invalid value for VarianceModifier");
    }
    write_identifier(type_parameter_declaration.name_token());
    end_node(&type_parameter_declaration);
}

void CSharpOutputVisitor::visit_constraint(Constraint& constraint) {
    start_node(&constraint);
    space();
    write_keyword(Tokens::where_keyword);
    constraint.type_parameter()->accept_visitor(*this);
    space();
    write_token(Tokens::colon);
    space();
    write_comma_separated_list(nodes_of(constraint.base_types()));
    end_node(&constraint);
}

void CSharpOutputVisitor::visit_identifier(Identifier& identifier) { write_identifier(&identifier); }

void CSharpOutputVisitor::visit_error_node(AstNode& error_node) {
    start_node(&error_node);
    end_node(&error_node);
}

// ---- Pattern Nodes ----------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_pattern_placeholder(AstNode& placeholder, pm::Pattern& pattern) {
    start_node(&placeholder);
    visit_node_in_pattern(&pattern);
    end_node(&placeholder);
}

void CSharpOutputVisitor::visit_any_node(pm::AnyNode& any_node) {
    if (!any_node.group_name().empty()) {
        write_identifier(any_node.group_name());
        write_token(Tokens::colon);
    }
}

void CSharpOutputVisitor::visit_backreference(pm::Backreference& backreference) {
    write_keyword("backreference");
    l_par();
    write_identifier(backreference.referenced_group_name());
    r_par();
}

void CSharpOutputVisitor::visit_identifier_expression_backreference(pm::IdentifierExpressionBackreference& identifier_expression_backreference) {
    write_keyword("identifierBackreference");
    l_par();
    write_identifier(identifier_expression_backreference.referenced_group_name());
    r_par();
}

void CSharpOutputVisitor::visit_choice(pm::Choice& choice) {
    write_keyword("choice");
    space();
    l_par();
    new_line();
    writer_.indent();
    pm::INode* last = nullptr;
    for (pm::INode* alternative : choice) last = alternative;
    for (pm::INode* alternative : choice) {
        visit_node_in_pattern(alternative);
        if (alternative != last) {
            write_token(Tokens::comma);
        }
        new_line();
    }
    writer_.unindent();
    r_par();
}

void CSharpOutputVisitor::visit_named_node(pm::NamedNode& named_node) {
    if (!named_node.group_name().empty()) {
        write_identifier(named_node.group_name());
        write_token(Tokens::colon);
    }
    visit_node_in_pattern(named_node.child_node());
}

void CSharpOutputVisitor::visit_repeat(pm::Repeat& repeat) {
    write_keyword("repeat");
    l_par();
    if (repeat.min_count() != 0 || repeat.max_count() != INT_MAX) {
        write_identifier(std::to_string(repeat.min_count()));
        write_token(Tokens::comma);
        write_identifier(std::to_string(repeat.max_count()));
        write_token(Tokens::comma);
    }
    visit_node_in_pattern(repeat.child_node());
    r_par();
}

void CSharpOutputVisitor::visit_optional_node(pm::OptionalNode& optional_node) {
    write_keyword("optional");
    l_par();
    visit_node_in_pattern(optional_node.child_node());
    r_par();
}

void CSharpOutputVisitor::visit_node_in_pattern(pm::INode* child_node) {
    if (auto* ast_node = dynamic_cast<AstNode*>(child_node)) {
        ast_node->accept_visitor(*this);
    } else if (auto* identifier_expression_backreference = dynamic_cast<pm::IdentifierExpressionBackreference*>(child_node)) {
        visit_identifier_expression_backreference(*identifier_expression_backreference);
    } else if (auto* choice = dynamic_cast<pm::Choice*>(child_node)) {
        visit_choice(*choice);
    } else if (auto* any_node = dynamic_cast<pm::AnyNode*>(child_node)) {
        visit_any_node(*any_node);
    } else if (auto* backreference = dynamic_cast<pm::Backreference*>(child_node)) {
        visit_backreference(*backreference);
    } else if (auto* named_node = dynamic_cast<pm::NamedNode*>(child_node)) {
        visit_named_node(*named_node);
    } else if (auto* optional_node = dynamic_cast<pm::OptionalNode*>(child_node)) {
        visit_optional_node(*optional_node);
    } else if (auto* repeat = dynamic_cast<pm::Repeat*>(child_node)) {
        visit_repeat(*repeat);
    } else {
        // upstream writes the node as a primitive value of unknown kind: its ToString() text, which for a pattern is its type name
        writer_.write_token(child_node != nullptr ? typeid(*child_node).name() : "null");
    }
}

// ---- Documentation Reference -------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_documentation_reference(DocumentationReference& documentation_reference) {
    start_node(&documentation_reference);
    if (documentation_reference.declaring_type() != nullptr) {
        documentation_reference.declaring_type()->accept_visitor(*this);
        if (documentation_reference.symbol_kind() != typesystem::SymbolKind::TypeDefinition) {
            write_token(Tokens::dot);
        }
    }
    switch (documentation_reference.symbol_kind()) {
        case typesystem::SymbolKind::TypeDefinition:
            break;
        case typesystem::SymbolKind::Indexer:
            write_keyword(IndexerDeclaration::this_keyword);
            break;
        case typesystem::SymbolKind::Operator: {
            OperatorType op_type = documentation_reference.operator_type();
            if (op_type == OperatorType::Explicit || op_type == OperatorType::CheckedExplicit) {
                write_keyword(OperatorDeclaration::explicit_keyword);
            } else if (op_type == OperatorType::Implicit) {
                write_keyword(OperatorDeclaration::implicit_keyword);
            }
            write_keyword(OperatorDeclaration::operator_keyword);
            space();
            if (OperatorDeclaration::is_checked(op_type)) {
                write_keyword(OperatorDeclaration::checked_keyword);
                space();
            }
            if (op_type == OperatorType::Explicit || op_type == OperatorType::Implicit || op_type == OperatorType::CheckedExplicit) {
                documentation_reference.conversion_operator_return_type()->accept_visitor(*this);
            } else {
                write_token(OperatorDeclaration::get_token(op_type));
            }
            break;
        }
        default:
            write_identifier(documentation_reference.name_token());
            break;
    }
    write_type_arguments(nodes_of(documentation_reference.type_arguments()));
    if (documentation_reference.has_parameter_list()) {
        space(policy_.space_before_method_declaration_parentheses);
        if (documentation_reference.symbol_kind() == typesystem::SymbolKind::Indexer) {
            write_comma_separated_list_in_brackets(nodes_of(documentation_reference.parameters()), policy_.space_within_method_declaration_parentheses);
        } else {
            write_comma_separated_list_in_parenthesis(nodes_of(documentation_reference.parameters()), policy_.space_within_method_declaration_parentheses);
        }
    }
    end_node(&documentation_reference);
}

std::string CSharpOutputVisitor::convert_string(std::string_view text) { return TextWriterTokenWriter::convert_string(text); }

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
