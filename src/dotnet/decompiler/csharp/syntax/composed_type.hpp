// dotnet/decompiler/csharp/syntax/composed_type.hpp -- ComposedType: a base type with the decorations that make a different type of it
// (`ref`, `readonly`, `?`, `*`, `[]`), as in `ref int?*[,]`; and ArraySpecifier, one `[,]` (its dimension count).
//
//     AstType* t = make<SimpleType>("T")->make_array_type(2);       // T[,] : a ComposedType around T
//     auto* composed = as<ComposedType>(t);
//     composed->pointer_rank();   composed->array_specifiers();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ComposedType, ArraySpecifier, from ComposedType.cs and the members generated
// for them), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// One array rank specifier: `[]` is dimension 1, `[,]` is 2.
class ArraySpecifier final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ArraySpecifier;

    ArraySpecifier() = default;
    explicit ArraySpecifier(int dimension_count) : dimensions_(dimension_count) {}

    // The number of dimensions (at least 1; check_invariant asserts it).
    int dimensions() const { return dimensions_; }
    void set_dimensions(int value) { dimensions_ = value; }

    std::string to_string(const output_visitor::CSharpFormattingOptions* formatting_options = nullptr) const override;
    void check_invariant() override;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_array_specifier(*this); }
    AstNode* shallow_clone() const override { return make<ArraySpecifier>(*this); }

private:
    int dimensions_ = 1;
};

class ComposedType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ComposedType;

    static constexpr std::string_view ref_keyword = "ref";
    static constexpr std::string_view readonly_keyword = "readonly";
    static constexpr std::string_view nullable_token = "?";
    static constexpr std::string_view pointer_token = "*";

    // The slots, in order: attributes, the base type (one index after them), array specifiers.
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> base_type_slot{"BaseType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<ArraySpecifier> array_specifiers_slot{"ArraySpecifiers", true, &Slots::array_specifier, true};

    ComposedType() = default;
    ComposedType(const std::vector<AttributeSection*>& attribute_sections, AstType* base);
    ComposedType(const std::vector<AttributeSection*>& attribute_sections, AstType* base, const std::vector<ArraySpecifier*>& specifiers);

    AstNodeCollection<AttributeSection>& attributes() const { return attributes_; }

    bool has_ref_specifier() const { return has_ref_specifier_; }
    void set_has_ref_specifier(bool value) { has_ref_specifier_ = value; }
    bool has_read_only_specifier() const { return has_read_only_specifier_; }
    void set_has_read_only_specifier(bool value) { has_read_only_specifier_ = value; }

    AstType* base_type() const { return base_type_; }
    void set_base_type(AstType* value) { set_child_node(base_type_, value); }

    bool has_nullable_specifier() const { return has_nullable_specifier_; }
    void set_has_nullable_specifier(bool value) { has_nullable_specifier_ = value; }
    // The number of `*` (never negative: check_invariant asserts it).
    int pointer_rank() const { return pointer_rank_; }
    void set_pointer_rank(int value) { pointer_rank_ = value; }

    AstNodeCollection<ArraySpecifier>& array_specifiers() const { return array_specifiers_; }

    // As written: `ref readonly T?**[,]`.
    std::string to_string(const output_visitor::CSharpFormattingOptions* formatting_options = nullptr) const override;

    // Adds a `*` (in place) unless there are array specifiers, then wraps like any type.
    AstType* make_pointer_type() override;
    // Adds an array specifier in front of the others (in place).
    AstType* make_array_type(int dimensions = 1) override;
    AstType* make_ref_type() override;

    void check_invariant() override;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 1 + attributes_.count() + array_specifiers_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_composed_type(*this); }
    AstNode* shallow_clone() const override { return make<ComposedType>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    bool has_ref_specifier_ = false;
    bool has_read_only_specifier_ = false;
    ChildPtr<AstType> base_type_;
    bool has_nullable_specifier_ = false;
    int pointer_rank_ = 0;
    mutable AstNodeCollection<ArraySpecifier> array_specifiers_{this, Slots::array_specifier, 2, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
