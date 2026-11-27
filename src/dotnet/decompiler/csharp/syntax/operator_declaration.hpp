// dotnet/decompiler/csharp/syntax/operator_declaration.hpp -- OperatorDeclaration and OperatorType: `[modifiers] type operator
// +(parameters) { body }`, conversions included.
//
//     auto* op = make<OperatorDeclaration>(attributes, result_type, nullptr, OperatorType::Addition);
//     std::string token = OperatorDeclaration::get_token(OperatorType::Addition);   // "+"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: OperatorDeclaration, from TypeMembers/OperatorDeclaration.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// The operators a C# type can declare (the order is the order of OperatorDeclaration's name table).
enum class OperatorType : std::uint8_t {
    LogicalNot,
    OnesComplement,
    Increment,
    CheckedIncrement,
    Decrement,
    CheckedDecrement,
    True,
    False,
    UnaryPlus,
    UnaryNegation,
    CheckedUnaryNegation,
    Addition,
    CheckedAddition,
    Subtraction,
    CheckedSubtraction,
    Multiply,
    CheckedMultiply,
    Division,
    CheckedDivision,
    Modulus,
    BitwiseAnd,
    BitwiseOr,
    ExclusiveOr,
    LeftShift,
    RightShift,
    UnsignedRightShift,
    Equality,
    Inequality,
    GreaterThan,
    LessThan,
    GreaterThanOrEqual,
    LessThanOrEqual,
    Implicit,
    Explicit,
    CheckedExplicit,
};

class OperatorDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::OperatorDeclaration;
    static constexpr std::string_view operator_keyword = "operator";
    static constexpr std::string_view checked_keyword = "checked";
    static constexpr std::string_view explicit_keyword = "explicit";
    static constexpr std::string_view implicit_keyword = "implicit";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<AstType> private_implementation_type_slot{"PrivateImplementationType", false, &Slots::private_implementation_type, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, true};

    OperatorDeclaration();
    OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value);
    OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value, const std::vector<ParameterDeclaration*>& parameters_value);
    OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value, const std::vector<ParameterDeclaration*>& parameters_value, BlockStatement* body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstType* private_implementation_type() const { return private_implementation_type_; }
    void set_private_implementation_type(AstType* value);
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);
    OperatorType operator_type() const { return operator_type_; }
    void set_operator_type(OperatorType value) { operator_type_ = value; }
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Operator; }

    // The operator type of the metadata name ("op_Addition"), if it is the name of one.
    static std::optional<OperatorType> get_operator_type(const std::string& method_name);
    // The metadata name of an operator type ("op_Addition"); nullopt for no type.
    static std::optional<std::string> get_name(std::optional<OperatorType> type);
    static std::string get_name(OperatorType type);
    // Whether the operator is the checked variant.
    static bool is_checked(OperatorType type);
    // The token the operator is written with ("+"; "implicit"/"explicit" for conversions).
    static std::string get_token(OperatorType type);

    std::string name() const override { return get_name(operator_type_); }
    void set_name(const std::string&) override { throw std::logic_error("not supported"); }
    Identifier* name_token() const override { return nullptr; }
    void set_name_token(Identifier*) override { throw std::logic_error("not supported"); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_operator_declaration(*this); }
    AstNode* shallow_clone() const override { return make<OperatorDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    ChildPtr<AstType> private_implementation_type_;
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 3, false};
    ChildPtr<BlockStatement> body_;
    OperatorType operator_type_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
