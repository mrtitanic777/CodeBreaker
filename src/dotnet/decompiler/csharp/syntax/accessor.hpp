// dotnet/decompiler/csharp/syntax/accessor.hpp -- Accessor: a `get`, `set`, `init`, `add` or `remove` accessor of a property, indexer or
// event.
//
//     auto* getter = make<Accessor>(AccessorKind::Getter);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Accessor, from TypeMembers/Accessor.cs with the members ILSpy 11's source
// generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// Which accessor an Accessor is (Any matches every kind in a pattern).
enum class AccessorKind {
    Any,
    Getter,
    Setter,
    Init,
    Adder,
    Remover,
};

class Accessor final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Accessor;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, true};

    Accessor();
    explicit Accessor(AccessorKind kind_value);
    Accessor(AccessorKind kind_value, const std::vector<AttributeSection*>& attributes_value);
    Accessor(AccessorKind kind_value, const std::vector<AttributeSection*>& attributes_value, BlockStatement* body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);
    AccessorKind accessor_kind() const { return accessor_kind_; }
    void set_accessor_kind(AccessorKind value) { accessor_kind_ = value; }
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Method; }

    // An accessor has no name of its own.
    std::string name() const override { return std::string(); }
    void set_name(const std::string&) override {}
    Identifier* name_token() const override { return nullptr; }
    void set_name_token(Identifier*) override {}

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_accessor(*this); }
    AstNode* shallow_clone() const override { return make<Accessor>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<BlockStatement> body_;
    AccessorKind accessor_kind_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
