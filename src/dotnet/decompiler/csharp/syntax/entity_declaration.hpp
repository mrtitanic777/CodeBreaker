// dotnet/decompiler/csharp/syntax/entity_declaration.hpp -- EntityDeclaration, the base of the declarations of a type, a member or an
// accessor (TypeDeclaration, MethodDeclaration, PropertyDeclaration, FieldDeclaration ...): attributes, modifiers, a name and a return
// type. Each concrete declaration class is in its own header.
//
//     entity->set_name("Main");   entity->name();               // through the Identifier child (name_token)
//     entity->set_modifiers(Modifiers::Public | Modifiers::Static);
//     entity->has_modifier(Modifiers::Static);
//     entity->attributes().add(section);                         // the AttributeSection children
//
// A concrete class reports what it declares in symbol_kind() and may override name / name_token / return_type / attributes where its
// slots differ (a constructor's name is its type's; an accessor has no return type slot of its own).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: EntityDeclaration, from TypeMembers/EntityDeclaration.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/modifiers.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class EntityDeclaration : public AstNode {
public:
    // What this declares (upstream's SymbolKind property).
    virtual typesystem::SymbolKind symbol_kind() const = 0;

    // The attribute sections before the declaration.
    virtual AstNodeCollection<AttributeSection>& attributes() const { return get_children(Slots::attribute_section); }

    Modifiers modifiers() const { return modifiers_; }
    void set_modifiers(Modifiers value) { modifiers_ = value; }
    // Whether all of `mod` is in the modifiers.
    bool has_modifier(Modifiers mod) const { return has_flag(modifiers_, mod); }

    // Not upstream: a body that is one `return expr;` is written `=> expr;` (the C# back end's plain lowering, l2_syntax_builder, asks
    // for it where the source wrote it so; ILSpy's builders never set it). Not a slot: matching and the transforms do not see it.
    bool is_expression_bodied() const { return expression_bodied_; }
    void set_expression_bodied(bool value) { expression_bodied_ = value; }

    // The declared name (empty if there is no name token).
    virtual std::string name() const {
        Identifier* token = get_child(Slots::identifier);
        return token != nullptr ? token->name() : std::string();
    }
    virtual void set_name(const std::string& value) { set_child(Slots::identifier, Identifier::create(value, TextLocation::empty)); }

    virtual Identifier* name_token() const { return get_child(Slots::identifier); }
    virtual void set_name_token(Identifier* value) { set_child(Slots::identifier, value); }

    virtual AstType* return_type() const { return get_child(Slots::type); }
    virtual void set_return_type(AstType* value) { set_child(Slots::type, value); }

protected:
    EntityDeclaration() = default;

    // Whether this declaration's modifiers and attributes match `other`'s (Modifiers::Any in this matches any), for do_match.
    bool match_attributes_and_modifiers(const EntityDeclaration& other, const pattern_matching::Match& match) const {
        return (modifiers_ == Modifiers::Any || modifiers_ == other.modifiers_) && attributes().do_match(other.attributes(), match);
    }

private:
    Modifiers modifiers_ = Modifiers::None;
    bool expression_bodied_ = false;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
