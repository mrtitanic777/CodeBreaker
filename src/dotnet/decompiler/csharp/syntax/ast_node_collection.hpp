// dotnet/decompiler/csharp/syntax/ast_node_collection.hpp -- the children of a collection slot of a syntax node (a block's statements, a
// call's arguments, a type's members ...): an ordered list that keeps every child's parent and index current as it changes.
//
//     // in a node class: the collection is a member, bound to its node and slot (SimpleType's type arguments, after the identifier):
//     AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 1, true};
//     AstNodeCollection<AstType>& type_arguments() { return type_arguments_; }
//
//     for (Statement* s : block->statements()) ...                 // safe against the loop body removing or replacing `s`
//     block->statements().add(make<EmptyStatement>());
//     block->statements().insert_before(existing, replacement);
//     auto detached = list.detach();                               // empties it, giving the nodes back unattached
//
// The list itself is untyped (AstNodeCollectionBase, in the order of upstream's AstNodeCollection<T>); AstNodeCollection<T> only adds
// the casts, so a T need not be a defined class where a collection of T is declared, cloned or matched. An element's type is checked
// against the slot's child type when it is added (std::invalid_argument), as upstream's `(T)node` casts do.
//
// The flattened child index (AstNode::get_child_by_index) of a collection's elements is first_index + position when `first_index` and
// `incremental_indices` say the collection is the node's only collection and its last slot; then adding, removing and inserting
// keep the indices current without a rebuild. Any other collection invalidates the node's indices (they are renumbered on next use).
//
// Copying a collection makes an empty one that belongs to no node: a node's implicit copy constructor (AstNode::clone's shallow copy)
// leaves its collections empty, and clone_children_into fills them with `copy->items_.clone_from(copy, items_)`.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AstNodeCollection, AstNodeCollection<T>, from AstNodeCollection.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported: the ICollection<T> interface, Equals
// and GetHashCode (a collection is its parent and kind; compare with operator==).
#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/csharp_slot_info.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/inode.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class AstNodeCollectionBase : public il::ArenaObject, public pattern_matching::INodeList {
public:
    // Walks a list in order, in a way that survives the loop body removing, replacing or inserting nodes: it remembers the successor
    // of the current element before handing it out and finds it again by identity (upstream's struct Enumerator).
    class Enumerator {
    public:
        explicit Enumerator(const std::vector<AstNode*>* list) : list_(list) {}
        bool move_next();
        AstNode* current() const { return current_; }
        void reset();

    private:
        const std::vector<AstNode*>* list_;
        std::size_t pos_ = 0;
        AstNode* current_ = nullptr;
        AstNode* next_ = nullptr;
        bool started_ = false;
    };

    // A collection of `parent_node`'s children of the shared slot `slot_kind`; `first_index` is the flattened index of the first
    // element and `incremental_indices` says the collection may keep the indices current by itself (see the file comment).
    AstNodeCollectionBase(AstNode* parent_node, const CSharpSlotInfo* slot_kind, int first_index, bool incremental_indices);
    // An empty collection that belongs to no node, with the same kind and base index (see the file comment).
    AstNodeCollectionBase(const AstNodeCollectionBase& other);
    AstNodeCollectionBase& operator=(const AstNodeCollectionBase&) = delete;

    // INodeList: the elements as INodes, for pattern matching.
    std::size_t size() const override { return list_.size(); }
    pattern_matching::INode* node_at(std::size_t index) const override;

    int count() const { return static_cast<int>(list_.size()); }
    bool empty() const { return list_.empty(); }
    AstNode* parent() const { return parent_; }
    const CSharpSlotInfo* kind() const { return kind_; }

    // ---- the operations, on AstNode*; AstNodeCollection<T> gives them types.

    // Appends a node (nothing for nullptr). Throws std::invalid_argument for the collection's own node, a node that already has a
    // parent, or a node of a class the slot does not allow.
    void add_node(AstNode* node);
    // Inserts `new_item` before `existing` (appended if `existing` is not in this collection, as when it is the node after the last).
    void insert_node_before(AstNode* existing, AstNode* new_item);
    // Inserts `new_item` after `existing` (at the front if `existing` is not in this collection).
    void insert_node_after(AstNode* existing, AstNode* new_item);
    // Replaces the element at `index` (the old one becomes unattached).
    void set_node(std::size_t index, AstNode* value);
    // Whether the node was in this collection (it is now unattached).
    bool remove_node(AstNode* node);
    // Position of the node, or -1 when it is not in this collection.
    int index_of_node(const AstNode* node) const;
    bool contains_node(const AstNode* node) const;
    // Unattaches every element.
    void clear();
    // Unattaches every element and returns them in order.
    std::vector<AstNode*> detach_nodes();
    // The elements in order (a snapshot).
    const std::vector<AstNode*>& nodes() const { return list_; }

    // Makes this (empty) collection belong to `new_parent` and fills it with a clone of each element of `source`. The one call a
    // node's clone_children_into needs per collection.
    void clone_from(AstNode* new_parent, const AstNodeCollectionBase& source);

    // Whether the elements match `other`'s as a pattern (upstream's DoMatch(AstNodeCollection<T>, Match)); a pattern in this list may
    // stand for several elements or none.
    bool do_match(const AstNodeCollectionBase& other, const pattern_matching::Match& match) const;

    // Visits each element in order.
    void accept_visitor(IAstVisitor& visitor) const;

    // Same parent and same kind.
    friend bool operator==(const AstNodeCollectionBase& a, const AstNodeCollectionBase& b) {
        return a.parent_ == b.parent_ && a.kind_ == b.kind_;
    }

protected:
    const std::vector<AstNode*>& raw_list() const { return list_; }

private:
    void validate_new_child(AstNode* child) const;
    void reindex_from(std::size_t start);
    void insert_at(std::size_t index, AstNode* new_item);
    bool incremental() const;

    AstNode* parent_;
    const CSharpSlotInfo* kind_;
    // The collection occupies the flattened range [base_index_, base_index_ + size) with nothing after it (see the file comment).
    int base_index_;
    bool supports_incremental_;
    std::vector<AstNode*> list_;
};

// The iterator behind range-for over a collection: the enumerator, typed.
template <class T>
class AstNodeIterator {
public:
    using value_type = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    AstNodeIterator() = default;
    explicit AstNodeIterator(AstNodeCollectionBase::Enumerator enumerator) : enumerator_(enumerator), has_current_(enumerator_.move_next()) {}

    T* operator*() const { return static_cast<T*>(enumerator_.current()); }
    AstNodeIterator& operator++() {
        has_current_ = enumerator_.move_next();
        return *this;
    }
    AstNodeIterator operator++(int) {
        AstNodeIterator copy = *this;
        ++*this;
        return copy;
    }
    friend bool operator==(const AstNodeIterator& it, std::default_sentinel_t) { return !it.has_current_; }

private:
    AstNodeCollectionBase::Enumerator enumerator_{nullptr};
    bool has_current_ = false;
};

// A collection of T (a node class), as the children of one slot of a node.
template <class T>
class AstNodeCollection final : public AstNodeCollectionBase {
public:
    AstNodeCollection(AstNode* parent_node, const CSharpSlotInfoT<T>& slot_kind, int first_index = 0, bool incremental_indices = false)
        : AstNodeCollectionBase(parent_node, &slot_kind, first_index, incremental_indices) {}

    T* operator[](std::size_t index) const { return static_cast<T*>(raw_list()[index]); }

    // Replaces the element at `index`.
    void set(std::size_t index, T* value) { set_node(index, value); }
    // Appends (nothing for nullptr).
    void add(T* element) { add_node(element); }
    // Appends each of a range of T* (a copy of the range is taken first, so `list.add_range(list)` is fine).
    template <class Range>
    void add_range(const Range& nodes) {
        std::vector<T*> snapshot;
        for (T* node : nodes) snapshot.push_back(node);
        for (T* node : snapshot) add(node);
    }
    // Replaces the elements by those of a range (taken before the collection is cleared).
    template <class Range>
    void replace_with(const Range& nodes) {
        std::vector<T*> snapshot;
        for (T* node : nodes) snapshot.push_back(node);
        clear();
        for (T* node : snapshot) add(node);
    }
    // Moves every element, in order, to another collection (anything with add(T*)).
    template <class Target>
    void move_to(Target& target) {
        for (AstNode* node : std::vector<AstNode*>(raw_list())) {
            T* typed = static_cast<T*>(node);
            typed->remove();
            target.add(typed);
        }
    }

    bool contains(const T* element) const { return contains_node(element); }
    int index_of(const T* element) const { return index_of_node(element); }
    bool remove(T* element) { return remove_node(element); }
    // Unattaches every element and returns them.
    std::vector<T*> detach() {
        std::vector<T*> result;
        for (AstNode* node : detach_nodes()) result.push_back(static_cast<T*>(node));
        return result;
    }
    // The elements in order.
    std::vector<T*> to_vector() const {
        std::vector<T*> result;
        result.reserve(raw_list().size());
        for (AstNode* node : raw_list()) result.push_back(static_cast<T*>(node));
        return result;
    }

    // The first element (that `predicate` accepts), or nullptr.
    T* first_or_null() const { return empty() ? nullptr : (*this)[0]; }
    template <class Predicate>
    T* first_or_null(Predicate predicate) const {
        for (AstNode* node : raw_list())
            if (predicate(static_cast<T*>(node))) return static_cast<T*>(node);
        return nullptr;
    }
    // The last element (that `predicate` accepts), or nullptr.
    T* last_or_null() const { return empty() ? nullptr : (*this)[size() - 1]; }
    template <class Predicate>
    T* last_or_null(Predicate predicate) const {
        T* result = nullptr;
        for (AstNode* node : raw_list())
            if (predicate(static_cast<T*>(node))) result = static_cast<T*>(node);
        return result;
    }

    // Inserts `new_item` after `existing` (at the front if `existing` is nullptr or not in the collection).
    void insert_after(T* existing, T* new_item) { insert_node_after(existing, new_item); }
    // Inserts `new_item` before `existing` (at the end if `existing` is nullptr or not in the collection).
    void insert_before(T* existing, T* new_item) { insert_node_before(existing, new_item); }

    AstNodeIterator<T> begin() const { return AstNodeIterator<T>(Enumerator(&raw_list())); }
    std::default_sentinel_t end() const { return std::default_sentinel; }
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
