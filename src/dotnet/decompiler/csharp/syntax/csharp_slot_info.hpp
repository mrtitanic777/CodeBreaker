// dotnet/decompiler/csharp/syntax/csharp_slot_info.hpp -- the description of one child position of a syntax node: its name, the kind of
// node that may stand there, whether it holds one child or a collection, and whether it may be empty.
//
//     // in a node class (upstream's generated BinaryOperatorExpression.LeftSlot):
//     static constexpr CSharpSlotInfoT<Expression> left_slot{"Left", false, &Slots::left, true};
//     bool is_left = node->slot() == &BinaryOperatorExpression::left_slot;   // one object per slot, compared by address
//     bool in_initializer = node->slot()->kind() == &Slots::initializer;     // the shared kind of the position, across node types
//
// Two kinds of slot info exist: a node class's own slots (name, optional flag; `kind()` points at the shared one), and the shared
// kinds in `Slots` (slots.hpp; their own `kind()` is nullptr). AstNode::get_child<T>(slot) and get_children<T>(slot) take a shared kind
// and give back a T* / AstNodeCollection<T>&.
//
// Where upstream keeps the child type as a System.Type and asks `ChildType.IsInstanceOfType(node)`, the slot keeps the KindRange of
// that class (ast_node_kind.hpp) and asks whether the node's kind is in it; CSharpSlotInfoT<T> fills it in from T without needing T to
// be defined, so the shared kinds can be declared before the node classes.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: CSharpSlotInfo, CSharpSlotInfo<T>, from CSharpSlotInfo.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <string_view>

#include "dotnet/decompiler/csharp/syntax/ast_node_kind.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class CSharpSlotInfo {
public:
    // The slot's property name ("Left", "Body", "Parameters").
    constexpr std::string_view name() const { return name_; }
    // The kinds of node that may stand in the slot (upstream's ChildType; for a collection slot, of an element).
    constexpr KindRange child_type() const { return child_type_; }
    // Whether the slot holds a collection of children rather than one child.
    constexpr bool is_collection() const { return is_collection_; }
    // The shared slot (a `Slots` constant) naming this position across node types; nullptr for a `Slots` constant itself.
    constexpr const CSharpSlotInfo* kind() const { return kind_; }
    // Whether the slot may be empty: a single slot whose child is nullable, or any collection slot.
    constexpr bool is_optional() const { return is_optional_; }

    // Upstream's `ChildType.IsInstanceOfType(node)`; false for nullptr.
    bool is_instance(const AstNode* node) const;

    std::string to_string() const { return std::string(name_); }

    CSharpSlotInfo(const CSharpSlotInfo&) = delete;
    CSharpSlotInfo& operator=(const CSharpSlotInfo&) = delete;

protected:
    constexpr CSharpSlotInfo(std::string_view slot_name, KindRange allowed_children, bool collection, const CSharpSlotInfo* shared_kind,
                             bool optional)
        : name_(slot_name), child_type_(allowed_children), is_collection_(collection), kind_(shared_kind), is_optional_(optional) {}

private:
    std::string_view name_;
    KindRange child_type_;
    bool is_collection_;
    const CSharpSlotInfo* kind_;
    bool is_optional_;
};

// A slot info that carries its child type as a template argument, so the typed accessors infer T from it. T (the node class the slot
// holds, or the element class of a collection slot) need not be defined yet.
template <class T>
class CSharpSlotInfoT final : public CSharpSlotInfo {
public:
    using child_type_t = T;
    constexpr CSharpSlotInfoT(std::string_view slot_name, bool collection, const CSharpSlotInfo* shared_kind, bool optional)
        : CSharpSlotInfo(slot_name, kind_of<T>::range, collection, shared_kind, optional) {}
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
