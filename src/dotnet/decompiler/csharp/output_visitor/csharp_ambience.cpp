// dotnet/decompiler/csharp/output_visitor/csharp_ambience.cpp -- CSharpAmbience; see csharp_ambience.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpAmbience), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/csharp_ambience.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/typesystem/extension_info.hpp"
#include "dotnet/decompiler/typesystem/local_function_method.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IProperty;
using typesystem::ISymbol;
using typesystem::ITypeDefinition;
using typesystem::IType;
using typesystem::SymbolKind;

namespace {

bool is_fixed_field(const IField* field, const IType** type, int* element_count) { return CSharpDecompiler::is_fixed_field(field, type, element_count); }

bool is_kind(const ISymbol& symbol, SymbolKind kind) { return symbol.symbol_kind() == kind; }

}  // namespace

// ---- ConvertSymbol ------------------------------------------------------------------------------------------------------------

std::string CSharpAmbience::convert_symbol(const ISymbol& symbol) {
    StringTextWriter writer;
    TextWriterTokenWriter token_writer(writer);
    convert_symbol(symbol, token_writer, FormattingOptionsFactory::create_empty());
    return writer.take_text();
}

void CSharpAmbience::convert_symbol(const ISymbol& symbol, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    ExtensionGroup extension_group;
    if (is_extension(dynamic_cast<const ITypeDefinition*>(&symbol), &extension_group)) {
        ast_builder.show_parameter_names = true;
    } else {
        extension_group = ExtensionGroup();
    }
    AstNode* node = ast_builder.convert_symbol(&symbol);
    writer.start_node(*node);
    if (auto* entity_decl = as<EntityDeclaration>(node)) print_modifiers(entity_decl->modifiers(), writer);
    if (has_all(conversion_flags_, ConversionFlags::ShowDefinitionKeyword)) {
        if (auto* type_declaration = as<TypeDeclaration>(node)) {
            switch (type_declaration->class_type()) {
                case ClassType::Class:
                    writer.write_keyword("class");
                    break;
                case ClassType::Struct:
                    writer.write_keyword("struct");
                    break;
                case ClassType::Interface:
                    writer.write_keyword("interface");
                    break;
                case ClassType::Enum:
                    writer.write_keyword("enum");
                    break;
                case ClassType::RecordClass:
                    writer.write_keyword("record");
                    break;
                case ClassType::RecordStruct:
                    writer.write_keyword("record");
                    writer.space();
                    writer.write_keyword("struct");
                    break;
                default:
                    throw std::logic_error("Invalid value for ClassType");
            }
            writer.space();
        } else if (is<DelegateDeclaration>(node)) {
            writer.write_keyword("delegate");
            writer.space();
        } else if (is<EventDeclaration>(node)) {
            writer.write_keyword("event");
            writer.space();
        } else if (is<NamespaceDeclaration>(node)) {
            writer.write_keyword("namespace");
            writer.space();
        }
    }
    if (!has_all(conversion_flags_, ConversionFlags::PlaceReturnTypeAfterParameterList) && has_all(conversion_flags_, ConversionFlags::ShowReturnType)) {
        AstType* rt = node->get_child(Slots::type);
        if (rt != nullptr) {
            CSharpOutputVisitor visitor(writer, formatting_policy);
            rt->accept_visitor(visitor);
            writer.space();
        }
    }
    if (auto* type_definition = dynamic_cast<const ITypeDefinition*>(&symbol)) {
        write_type_declaration_name(*type_definition, writer, formatting_policy);
    } else if (auto* member = dynamic_cast<const IMember*>(&symbol)) {
        write_member_declaration_name(*member, writer, formatting_policy);
    } else {
        writer.write_identifier(*Identifier::create(symbol.name()));
    }
    if (show_parameter_list(symbol)) {
        writer.write_token(symbol.symbol_kind() == SymbolKind::Indexer ? "[" : "(");
        bool first = true;
        std::vector<ParameterDeclaration*> parameters;
        const IProperty* parameterized_property = dynamic_cast<const IProperty*>(&symbol);
        if (extension_group.marker_method != nullptr) {
            std::vector<const IType*> class_type_arguments(extension_group.type_parameters.begin(), extension_group.type_parameters.end());
            typesystem::TypeParameterSubstitution subst(class_type_arguments, std::nullopt);
            for (const typesystem::IParameter* p : extension_group.marker_method->specialize_method(subst)->parameters())
                parameters.push_back(ast_builder.convert_parameter(p));
        } else if (parameterized_property != nullptr && parameterized_property->symbol_kind() == SymbolKind::Property) {
            for (const typesystem::IParameter* p : parameterized_property->parameters()) parameters.push_back(ast_builder.convert_parameter(p));
        } else {
            for (ParameterDeclaration* p : node->get_children(Slots::parameter)) parameters.push_back(p);
        }
        for (ParameterDeclaration* param : parameters) {
            if (!has_any(conversion_flags_, ConversionFlags::ShowParameterModifiers)) {
                param->set_parameter_modifier(typesystem::ReferenceKind::None);
                param->set_is_scoped_ref(false);
                param->set_is_params(false);
            }
            if (!has_any(conversion_flags_, ConversionFlags::ShowParameterDefaultValues)) {
                if (param->default_expression() != nullptr) param->default_expression()->remove();
            }
            if (first) {
                first = false;
            } else {
                writer.write_token(",");
                writer.space();
            }
            CSharpOutputVisitor visitor(writer, formatting_policy);
            param->accept_visitor(visitor);
        }
        writer.write_token(symbol.symbol_kind() == SymbolKind::Indexer ? "]" : ")");
    }
    if (has_all(conversion_flags_, ConversionFlags::PlaceReturnTypeAfterParameterList) && has_all(conversion_flags_, ConversionFlags::ShowReturnType)) {
        AstType* rt = node->get_child(Slots::type);
        if (rt != nullptr) {
            writer.space();
            writer.write_token(":");
            writer.space();
            const IField* f = dynamic_cast<const IField*>(&symbol);
            const IType* fixed_type = nullptr;
            int element_count = 0;
            if (f != nullptr && is_fixed_field(f, &fixed_type, &element_count)) {
                rt = ast_builder.convert_type(fixed_type);
                Expression* element_count_expression = ast_builder.convert_constant_value(
                    f->compilation().find_type(typesystem::KnownTypeCode::Int32), typesystem::ConstantValue(static_cast<std::int32_t>(element_count)));
                Expression* type_reference = make<TypeReferenceExpression>(rt);
                auto* indexer = make<IndexerExpression>(type_reference, std::vector<Expression*>{element_count_expression});
                CSharpOutputVisitor visitor(writer, formatting_policy);
                indexer->accept_visitor(visitor);
            } else {
                CSharpOutputVisitor visitor(writer, formatting_policy);
                rt->accept_visitor(visitor);
            }
        }
    }
    if (has_all(conversion_flags_, ConversionFlags::ShowBody) && !is<TypeDeclaration>(node)) {
        if (auto* property = dynamic_cast<const IProperty*>(&symbol)) {
            writer.space();
            writer.write_token("{");
            writer.space();
            if (property->can_get()) {
                writer.write_keyword("get");
                writer.write_token(";");
                writer.space();
            }
            if (property->can_set()) {
                if (has_any(conversion_flags_, ConversionFlags::SupportInitAccessors) && property->setter()->is_init_only())
                    writer.write_keyword("init");
                else
                    writer.write_keyword("set");
                writer.write_token(";");
                writer.space();
            }
            writer.write_token("}");
        } else {
            writer.write_token(";");
        }
        writer.end_node(*node);
    }
}

bool CSharpAmbience::is_extension(const ITypeDefinition* type_def, ExtensionGroup* extension_group) const {
    *extension_group = ExtensionGroup();
    if (!has_any(conversion_flags_, ConversionFlags::SupportExtensionDeclarations)) return false;
    if (type_def == nullptr) return false;
    const typesystem::ExtensionInfo* extension_info = nullptr;
    if (const ITypeDefinition* declaring = type_def->declaring_type_definition()) {
        extension_info = declaring->extension_info();
        if (extension_info == nullptr && declaring->declaring_type_definition() != nullptr)
            extension_info = declaring->declaring_type_definition()->extension_info();
    }
    if (extension_info == nullptr) return false;
    typesystem::ExtensionGroupInfo group;
    if (!extension_info->is_extension_marker_type(type_def, &group)) return false;
    extension_group->marker_method = group.marker_method;
    extension_group->type_parameters = group.type_parameters;
    return true;
}

bool CSharpAmbience::show_parameter_list(const ISymbol& e) const {
    switch (e.symbol_kind()) {
        case SymbolKind::TypeDefinition: {
            auto* type_def = dynamic_cast<const ITypeDefinition*>(&e);
            if (type_def != nullptr && type_def->kind() == typesystem::TypeKind::Delegate) return has_any(conversion_flags_, ConversionFlags::ShowParameterList);
            ExtensionGroup group;
            if (type_def != nullptr && is_extension(type_def, &group)) return has_any(conversion_flags_, ConversionFlags::SupportExtensionDeclarations);
            return false;
        }
        case SymbolKind::Indexer:
        case SymbolKind::Method:
        case SymbolKind::Operator:
        case SymbolKind::Constructor:
        case SymbolKind::Destructor:
            return has_any(conversion_flags_, ConversionFlags::ShowParameterList);
        case SymbolKind::Property: {
            auto* property = dynamic_cast<const IProperty*>(&e);
            if (property != nullptr && property->parameters().size() > 0) return has_any(conversion_flags_, ConversionFlags::ShowParameterList);
            return false;
        }
        default:
            return false;
    }
}

TypeSystemAstBuilder CSharpAmbience::create_ast_builder() const {
    TypeSystemAstBuilder ast_builder;
    ast_builder.add_resolve_result_annotations = true;
    ast_builder.show_type_parameters_for_unbound_types = true;
    ast_builder.show_modifiers = has_all(conversion_flags_, ConversionFlags::ShowModifiers);
    ast_builder.show_accessibility = has_all(conversion_flags_, ConversionFlags::ShowAccessibility);
    ast_builder.use_private_protected_accessibility = has_all(conversion_flags_, ConversionFlags::UsePrivateProtectedAccessibility);
    ast_builder.always_use_short_type_names = !has_all(conversion_flags_, ConversionFlags::UseFullyQualifiedTypeNames);
    ast_builder.show_parameter_names = has_all(conversion_flags_, ConversionFlags::ShowParameterNames);
    ast_builder.use_nullable_specifier_for_value_types = has_any(conversion_flags_, ConversionFlags::UseNullableSpecifierForValueTypes);
    ast_builder.support_init_accessors = has_any(conversion_flags_, ConversionFlags::SupportInitAccessors);
    ast_builder.support_record_classes = has_any(conversion_flags_, ConversionFlags::SupportRecordClasses);
    ast_builder.support_record_structs = has_any(conversion_flags_, ConversionFlags::SupportRecordStructs);
    ast_builder.support_unsigned_right_shift = has_any(conversion_flags_, ConversionFlags::SupportUnsignedRightShift);
    ast_builder.support_operator_checked = has_any(conversion_flags_, ConversionFlags::SupportOperatorChecked);
    ast_builder.support_extension_declarations = has_any(conversion_flags_, ConversionFlags::SupportExtensionDeclarations);
    return ast_builder;
}

void CSharpAmbience::write_type_declaration_name(const ITypeDefinition& type_def, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    EntityDeclaration* node = ast_builder.convert_entity(&type_def);
    if (type_def.declaring_type_definition() != nullptr &&
        (has_all(conversion_flags_, ConversionFlags::ShowDeclaringType) || has_all(conversion_flags_, ConversionFlags::UseFullyQualifiedEntityNames))) {
        write_type_declaration_name(*type_def.declaring_type_definition(), writer, formatting_policy);
        writer.write_token(".");
    } else if (has_all(conversion_flags_, ConversionFlags::UseFullyQualifiedEntityNames)) {
        if (!type_def.namespace_().empty()) {
            write_qualified_name(type_def.namespace_(), writer, formatting_policy);
            writer.write_token(".");
        }
    }
    ExtensionGroup group;
    if (is_extension(&type_def, &group)) {
        writer.write_keyword("extension");
        std::vector<TypeParameterDeclaration*> parameters;
        for (const typesystem::ITypeParameter* tp : group.type_parameters) parameters.push_back(ast_builder.convert_type_parameter(tp));
        write_type_parameters(parameters, writer, formatting_policy);
    } else {
        writer.write_identifier(*node->name_token());
        std::vector<TypeParameterDeclaration*> parameters;
        for (TypeParameterDeclaration* tp : node->get_children(Slots::type_parameter)) parameters.push_back(tp);
        write_type_parameters(parameters, writer, formatting_policy);
    }
}

void CSharpAmbience::write_member_declaration_name(const IMember& member, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    EntityDeclaration* node = ast_builder.convert_entity(&member);
    if (has_all(conversion_flags_, ConversionFlags::ShowDeclaringType) && member.declaring_type() != nullptr &&
        dynamic_cast<const typesystem::LocalFunctionMethod*>(&member) == nullptr) {
        convert_type(*member.declaring_type(), writer, formatting_policy);
        writer.write_token(".");
    }
    const IType* explicit_interface_type = get_explicit_interface_type(member);
    std::string name = member.name();
    if (explicit_interface_type != nullptr) {
        name = name.substr(name.rfind('.') == std::string::npos ? 0 : name.rfind('.') + 1);
    }
    switch (member.symbol_kind()) {
        case SymbolKind::Indexer:
            if (explicit_interface_type != nullptr) {
                convert_type(*explicit_interface_type, writer, formatting_policy);
                writer.write_token(".");
            }
            writer.write_keyword("this");
            break;
        case SymbolKind::Constructor:
            write_qualified_name(member.declaring_type()->name(), writer, formatting_policy);
            break;
        case SymbolKind::Destructor:
            writer.write_token("~");
            write_qualified_name(member.declaring_type()->name(), writer, formatting_policy);
            break;
        case SymbolKind::Operator:
            if (name == "op_Implicit") {
                writer.write_keyword("implicit");
                writer.space();
                if (explicit_interface_type != nullptr) {
                    convert_type(*explicit_interface_type, writer, formatting_policy);
                    writer.write_token(".");
                }
                writer.write_keyword("operator");
                writer.space();
                convert_type(*member.return_type(), writer, formatting_policy);
            } else if (name == "op_Explicit" || name == "op_CheckedExplicit") {
                writer.write_keyword("explicit");
                writer.space();
                if (explicit_interface_type != nullptr) {
                    convert_type(*explicit_interface_type, writer, formatting_policy);
                    writer.write_token(".");
                }
                writer.write_keyword("operator");
                writer.space();
                if (name == "op_CheckedExplicit") {
                    writer.write_token("checked");
                    writer.space();
                }
                convert_type(*member.return_type(), writer, formatting_policy);
            } else {
                if (explicit_interface_type != nullptr) {
                    convert_type(*explicit_interface_type, writer, formatting_policy);
                    writer.write_token(".");
                }
                writer.write_keyword("operator");
                writer.space();
                std::optional<OperatorType> operator_type = OperatorDeclaration::get_operator_type(name);
                if (operator_type.has_value() &&
                    !(!has_any(conversion_flags_, ConversionFlags::SupportOperatorChecked) && OperatorDeclaration::is_checked(*operator_type))) {
                    if (OperatorDeclaration::is_checked(*operator_type)) {
                        writer.write_token("checked");
                        writer.space();
                    }
                    writer.write_token(OperatorDeclaration::get_token(*operator_type));
                } else {
                    writer.write_identifier(*node->name_token());
                }
            }
            break;
        default:
            if (explicit_interface_type != nullptr) {
                convert_type(*explicit_interface_type, writer, formatting_policy);
                writer.write_token(".");
            }
            writer.write_identifier(*Identifier::create(name));
            break;
    }
    std::vector<TypeParameterDeclaration*> parameters;
    for (TypeParameterDeclaration* tp : node->get_children(Slots::type_parameter)) parameters.push_back(tp);
    write_type_parameters(parameters, writer, formatting_policy);
}

void CSharpAmbience::write_type_parameters(const std::vector<TypeParameterDeclaration*>& type_parameters, TokenWriter& writer,
                                           const CSharpFormattingOptions& formatting_policy) {
    if (has_all(conversion_flags_, ConversionFlags::ShowTypeParameterList)) {
        CSharpOutputVisitor output_visitor(writer, formatting_policy);
        if (!has_any(conversion_flags_, ConversionFlags::ShowTypeParameterVarianceModifier)) {
            for (TypeParameterDeclaration* decl : type_parameters) decl->set_variance(typesystem::VarianceModifier::Invariant);
        }
        output_visitor.write_type_parameters(std::vector<AstNode*>(type_parameters.begin(), type_parameters.end()));
    }
}

void CSharpAmbience::print_modifiers(Modifiers modifiers, TokenWriter& writer) {
    for (Modifiers m : CSharpModifiers::all_modifiers()) {
        if ((modifiers & m) == m) {
            writer.write_keyword(CSharpModifiers::get_modifier_name(m));
            writer.space();
        }
    }
}

void CSharpAmbience::write_qualified_name(const std::string& name, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy) {
    AstType* node = AstType::create(name);
    CSharpOutputVisitor output_visitor(writer, formatting_policy);
    node->accept_visitor(output_visitor);
}

// ---- the other conversions ------------------------------------------------------------------------------------------------------

std::string CSharpAmbience::convert_variable(const typesystem::IVariable& v) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    AstNode* ast_node = ast_builder.convert_variable(&v);
    std::string text = ast_node->to_string();
    // TrimEnd(';', '\r', '\n', (char)8232)
    while (true) {
        if (!text.empty() && (text.back() == ';' || text.back() == '\r' || text.back() == '\n')) {
            text.pop_back();
        } else if (text.size() >= 3 && text.compare(text.size() - 3, 3, "\xE2\x80\xA8") == 0) {
            text.resize(text.size() - 3);
        } else {
            break;
        }
    }
    return text;
}

std::string CSharpAmbience::convert_type(const IType& type) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    ast_builder.always_use_short_type_names = !has_all(conversion_flags_, ConversionFlags::UseFullyQualifiedEntityNames);
    AstType* ast_type = ast_builder.convert_type(&type);
    return ast_type->to_string();
}

void CSharpAmbience::convert_type(const IType& type, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy) {
    TypeSystemAstBuilder ast_builder = create_ast_builder();
    ast_builder.always_use_short_type_names = !has_all(conversion_flags_, ConversionFlags::UseFullyQualifiedEntityNames);
    AstType* ast_type = ast_builder.convert_type(&type);
    CSharpOutputVisitor visitor(writer, formatting_policy);
    ast_type->accept_visitor(visitor);
}

const IType* CSharpAmbience::get_explicit_interface_type(const IMember& member) {
    if (member.is_explicit_interface_implementation()) {
        std::vector<const IMember*> implemented = member.explicitly_implemented_interface_members();
        if (!implemented.empty()) return typesystem::get_interface_as_implemented_by(implemented.front()->declaring_type(), member.declaring_type());
    }
    return nullptr;
}

std::string CSharpAmbience::convert_constant_value(const typesystem::ConstantValue& constant_value) {
    return TextWriterTokenWriter::print_primitive_value(constant_value);
}

std::string CSharpAmbience::wrap_comment(const std::string& comment) { return "// " + comment; }

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
