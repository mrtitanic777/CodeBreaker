// dotnet/decompiler/csharp/syntax/documentation_reference.hpp -- DocumentationReference: what a `cref` in an XML documentation comment
// refers to: a member (`List<T>.Add(T)`), a type, an operator or a conversion (`operator implicit int(Foo)`), as a declaring type, a name,
// type arguments and, when written, a parameter list.
//
//     auto* ref = make<DocumentationReference>();
//     ref->set_member_name("Add");   ref->set_has_parameter_list(true);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DocumentationReference, from DocumentationReference.cs and the members
// generated for it), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

#include "dotnet/decompiler/csharp/syntax/operator_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {


class DocumentationReference final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DocumentationReference;

    // The slots, in order: the declaring type (optional), the name token, the return type of a conversion operator, the type arguments
    // and the parameters.
    static constexpr CSharpSlotInfoT<AstType> declaring_type_slot{"DeclaringType", false, &Slots::declaring_type, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<AstType> conversion_operator_return_type_slot{"ConversionOperatorReturnType", false,
                                                                                    &Slots::conversion_operator_return_type, false};
    static constexpr CSharpSlotInfoT<AstType> type_arguments_slot{"TypeArguments", true, &Slots::type_argument, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};

    DocumentationReference() = default;
    DocumentationReference(typesystem::SymbolKind kind_of_symbol, OperatorType operator_kind, AstType* declaring, Identifier* name,
                           AstType* conversion_return_type, const std::vector<AstType*>& type_argument_list = {},
                           const std::vector<ParameterDeclaration*>& parameter_list = {});

    typesystem::SymbolKind symbol_kind() const { return symbol_kind_; }
    void set_symbol_kind(typesystem::SymbolKind value) { symbol_kind_ = value; }
    OperatorType operator_type() const { return operator_type_; }
    void set_operator_type(OperatorType value) { operator_type_ = value; }
    bool has_parameter_list() const { return has_parameter_list_; }
    void set_has_parameter_list(bool value) { has_parameter_list_ = value; }

    AstType* declaring_type() const { return declaring_type_; }
    void set_declaring_type(AstType* value) { set_child_node(declaring_type_, value, 0); }

    // The member's name (the name token must exist to read it).
    std::string member_name() const { return name_token_->name(); }
    void set_member_name(const std::string& value) { set_name_token(Identifier::create(value)); }

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value) { set_child_node(name_token_, value, 1); }

    AstType* conversion_operator_return_type() const { return conversion_operator_return_type_; }
    void set_conversion_operator_return_type(AstType* value) { set_child_node(conversion_operator_return_type_, value, 2); }

    AstNodeCollection<AstType>& type_arguments() const { return type_arguments_; }
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 3 + type_arguments_.count() + parameters_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_documentation_reference(*this); }
    AstNode* shallow_clone() const override { return make<DocumentationReference>(*this); }

private:
    typesystem::SymbolKind symbol_kind_ = typesystem::SymbolKind::None;
    OperatorType operator_type_{};
    bool has_parameter_list_ = false;
    ChildPtr<AstType> declaring_type_;
    ChildPtr<Identifier> name_token_;
    ChildPtr<AstType> conversion_operator_return_type_;
    mutable AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 3, false};
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 4, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
