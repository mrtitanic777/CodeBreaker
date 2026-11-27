// dotnet/decompiler/csharp/syntax/ast_node.hpp -- AstNode, the base of every node of the C# syntax tree the decompiler builds and then
// writes out. A node knows its parent, its place among its parent's children, its children by slot, and its annotations; the tree is
// changed in place (children are added, removed, replaced and cloned) and found again by walking it or by matching a pattern.
//
//     for (AstNode* child : node->children()) ...            // survives the body removing or replacing `child`
//     for (AstNode* d : node->descendants()) ...             // pre-order, same guarantee
//     if (auto* call = as<InvocationExpression>(node)) ...   // upstream's `node as InvocationExpression` / `node is X x`
//     node->replace_with(other);   node->remove();   Expression* copy = expr->clone();
//     Identifier* id = node->get_child(Slots::identifier);   // the child in the slot of that kind
//     match(pattern, node).success()                         // pattern_matching/inode.hpp
//
// The design is the ILAst's (il/il_instruction.hpp): nodes belong to the thread's ILArena (`using il::make`, `make<X>(...)`), the tree
// holds raw pointers, a node's children are in numbered slots (a single child takes one index even when empty, a collection a run of
// them) and a collection's elements know their index. A node class supplies, as upstream's generator does: its slot storage
// (get_child_count / get_child_by_index / set_child_by_index / get_child_slot_info / get_collection_by_kind), clone_children_into,
// shallow_clone, dispatch_to (visitors), do_match and node_kind. See README-port.md for a complete example.
//
// Source locations are not computed from children: the output visitor records each node's first and last printed position
// (store_print_start / store_print_end), and nodes that print as one token override start_location / end_location.
//
// Comments and preprocessor directives are Trivia nodes attached to a node's leading or trailing list, outside the slot space; they are
// held among the node's annotations.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AstNode, from AstNode.cs, with the members its generator adds), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported: the IEnumerable<T> returning
// ancestors/descendants as LINQ (they are ranges here); AstNode.ToString needs the output visitor, which installs it (see
// set_to_string_function).
#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_node_collection.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_node_kind.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/csharp_slot_info.hpp"
#include "dotnet/decompiler/csharp/syntax/iannotatable.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/inode.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"
#include "dotnet/decompiler/csharp/syntax/slots.hpp"
#include "dotnet/decompiler/csharp/syntax/text_location.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp {
namespace output_visitor {
class CSharpFormattingOptions;
}
namespace syntax {

using il::make;

// The field of a node class that holds one child (upstream's `slot_Left`). Reads as a T*; the node writes it through
// AstNode::set_child_node. Copying it gives an empty field, so a node's implicit copy constructor (the shallow copy clone() starts
// from) copies the scalars and leaves the children for clone_children_into to fill.
template <class T>
class ChildPtr {
public:
    ChildPtr() = default;
    ChildPtr(const ChildPtr&) noexcept {}
    ChildPtr& operator=(const ChildPtr&) = delete;

    T* get() const { return value_; }
    operator T*() const { return value_; }
    T* operator->() const { return value_; }

private:
    friend class AstNode;
    T* value_ = nullptr;
};

// A forward iterator over a lazily walked sequence of nodes: wraps an enumerator with move_next() and current().
template <class Enumerator>
class EnumeratorIterator {
public:
    using value_type = AstNode*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    EnumeratorIterator() = default;
    explicit EnumeratorIterator(Enumerator enumerator) : enumerator_(std::move(enumerator)), has_current_(enumerator_.move_next()) {}

    AstNode* operator*() const { return enumerator_.current(); }
    EnumeratorIterator& operator++() {
        has_current_ = enumerator_.move_next();
        return *this;
    }
    EnumeratorIterator operator++(int) {
        EnumeratorIterator copy = *this;
        ++*this;
        return copy;
    }
    friend bool operator==(const EnumeratorIterator& it, std::default_sentinel_t) { return !it.has_current_; }

private:
    Enumerator enumerator_;
    bool has_current_ = false;
};

// A sequence of nodes made by an enumerator type, usable in range-for (and `to_vector()`).
template <class Enumerator>
class NodeRange {
public:
    explicit NodeRange(Enumerator enumerator) : enumerator_(std::move(enumerator)) {}
    EnumeratorIterator<Enumerator> begin() const { return EnumeratorIterator<Enumerator>(enumerator_); }
    std::default_sentinel_t end() const { return std::default_sentinel; }
    std::vector<AstNode*> to_vector() const {
        std::vector<AstNode*> result;
        for (AstNode* node : *this) result.push_back(node);
        return result;
    }

private:
    Enumerator enumerator_;
};

// Walks the children of a node in order. It captures each child's successor before handing the child out, so the loop body may
// remove or replace the current child.
class ChildEnumerator {
public:
    explicit ChildEnumerator(AstNode* node) : node_(node) {}
    bool move_next();
    AstNode* current() const { return current_; }
    void reset();

private:
    AstNode* node_;
    AstNode* current_ = nullptr;
    AstNode* next_ = nullptr;
    bool started_ = false;
};

// Walks from `start` up through its ancestors: start, its parent, and so on. (The parent is read when the walk reaches it, so the
// loop body may re-parent the current node.)
class AncestorEnumerator {
public:
    explicit AncestorEnumerator(AstNode* start) : next_(start) {}
    bool move_next();
    AstNode* current() const { return current_; }

private:
    AstNode* next_;
    AstNode* current_ = nullptr;
    bool started_ = false;
};

// Walks the descendants of a node in pre-order, lazily, in a way that survives the loop body removing or replacing the current node
// (it knows the next sibling before the node is handed out). `descend_into_children`, if set, decides per node whether its children
// are visited.
class DescendantEnumerator {
public:
    DescendantEnumerator(AstNode* root, bool include_self, std::function<bool(AstNode*)> descend_into_children)
        : root_(root), include_self_(include_self), descend_(std::move(descend_into_children)) {}
    bool move_next();
    AstNode* current() const { return current_; }

private:
    enum class State { Start, AfterSelf, InLoop, AfterYield, Done };
    AstNode* root_;
    bool include_self_;
    std::function<bool(AstNode*)> descend_;
    State state_ = State::Start;
    std::stack<AstNode*> next_stack_;
    AstNode* pos_ = nullptr;
    AstNode* current_ = nullptr;
};

// The children of a node (AstNode::children()): a range, with the count and indexing upstream's collection has.
class ChildrenCollection {
public:
    explicit ChildrenCollection(AstNode* node) : node_(node) {}
    EnumeratorIterator<ChildEnumerator> begin() const { return EnumeratorIterator<ChildEnumerator>(ChildEnumerator(node_)); }
    std::default_sentinel_t end() const { return std::default_sentinel; }
    // The number of children (counted by walking them).
    int count() const;
    // The index-th child; std::out_of_range past the end.
    AstNode* operator[](int index) const;
    std::vector<AstNode*> to_vector() const;

private:
    AstNode* node_;
};

class Trivia;

class AstNode : public il::ArenaObject, public AbstractAnnotatable, public pattern_matching::INode, public ICloneable {
public:
    ~AstNode() override = default;
    AstNode& operator=(const AstNode&) = delete;

    // ---- the class of this node
    virtual AstNodeKind node_kind() const = 0;

    // ---- source locations (assigned as the output visitor prints the node)
    virtual TextLocation start_location() const { return start_location_; }
    virtual TextLocation end_location() const { return end_location_; }
    void store_print_start(TextLocation value) { start_location_ = value; }
    void store_print_end(TextLocation value) { end_location_ = value; }

    // ---- trivia: comments and preprocessor directives printed before or after this node, in the order they were added
    std::vector<Trivia*> leading_trivia() const;
    std::vector<Trivia*> trailing_trivia() const;
    void add_leading_trivia(Trivia* trivia);
    // Inserts before the leading trivia already attached (a file header comment that must precede generated directives).
    void prepend_leading_trivia(Trivia* trivia);
    void add_trailing_trivia(Trivia* trivia);
    // Appends a clone of the other node's trivia to this node's.
    void copy_trivia_from(const AstNode& other);

    // ---- navigation
    AstNode* parent() const { return parent_; }
    AstNode* next_sibling() const;
    AstNode* prev_sibling() const;
    AstNode* first_child() const;
    AstNode* last_child() const;
    bool has_children() const { return first_child() != nullptr; }
    ChildrenCollection children() const { return ChildrenCollection(const_cast<AstNode*>(this)); }

    // The ancestors, nearest first, without / with this node.
    NodeRange<AncestorEnumerator> ancestors() const { return NodeRange<AncestorEnumerator>(AncestorEnumerator(parent_)); }
    NodeRange<AncestorEnumerator> ancestors_and_self() const {
        return NodeRange<AncestorEnumerator>(AncestorEnumerator(const_cast<AstNode*>(this)));
    }
    // All descendants in pre-order, without / with this node.
    NodeRange<DescendantEnumerator> descendants() const {
        return NodeRange<DescendantEnumerator>(DescendantEnumerator(const_cast<AstNode*>(this), false, nullptr));
    }
    NodeRange<DescendantEnumerator> descendants_and_self() const {
        return NodeRange<DescendantEnumerator>(DescendantEnumerator(const_cast<AstNode*>(this), true, nullptr));
    }
    // The same, descending only into the children of nodes `descend_into_children` accepts (empty: all).
    NodeRange<DescendantEnumerator> descendant_nodes(std::function<bool(AstNode*)> descend_into_children = {}) const {
        return NodeRange<DescendantEnumerator>(DescendantEnumerator(const_cast<AstNode*>(this), false, std::move(descend_into_children)));
    }
    NodeRange<DescendantEnumerator> descendant_nodes_and_self(std::function<bool(AstNode*)> descend_into_children = {}) const {
        return NodeRange<DescendantEnumerator>(DescendantEnumerator(const_cast<AstNode*>(this), true, std::move(descend_into_children)));
    }

    // The nearest ancestor that is a T (upstream's GetParent<T>()), or nullptr.
    template <class T>
    T* get_parent() const;
    // The nearest ancestor `pred` accepts (an empty `pred`: the parent), or nullptr.
    AstNode* get_parent(const std::function<bool(AstNode*)>& pred) const;

    // The next / previous node in document order (the next sibling, else the parent's), optionally the first that `pred` accepts.
    // Trivia navigation ends at its list.
    AstNode* get_next_node() const;
    AstNode* get_next_node(const std::function<bool(AstNode*)>& pred) const;
    AstNode* get_prev_node() const;
    AstNode* get_prev_node(const std::function<bool(AstNode*)>& pred) const;
    // The next / previous sibling that `pred` accepts.
    AstNode* get_next_sibling(const std::function<bool(AstNode*)>& pred) const;
    AstNode* get_prev_sibling(const std::function<bool(AstNode*)>& pred) const;

    // ---- slots, by kind
    // The first child in the slot of the shared kind `slot_kind` (a Slots constant), typed; nullptr if empty or if this node has no such slot.
    template <class T>
    T* get_child(const CSharpSlotInfoT<T>& slot_kind) const;
    // The collection in the slot of the kind `slot_kind`. A node without such a collection gives a detached empty one (a write to it does not
    // reach the node).
    template <class T>
    AstNodeCollection<T>& get_children(const CSharpSlotInfoT<T>& slot_kind) const;

    // The slot this node occupies in its parent (compare with a node class's `x_slot` or `->kind()` with a Slots constant), or
    // nullptr when it has no parent (or is trivia, which occupies none).
    const CSharpSlotInfo* slot() const;

    // ---- changing the tree
    // Adds `child` to the slot of the kind (appends to a collection, fills a single slot); nothing for nullptr.
    void add_child(AstNode* child, const CSharpSlotInfo& kind);
    void insert_child_before(AstNode* next, AstNode* child, const CSharpSlotInfo& kind);
    void insert_child_after(AstNode* prev, AstNode* child, const CSharpSlotInfo& kind);

    // Removes this node from its parent (a single slot becomes empty).
    void remove();
    // Replaces this node by `new_node` (nullptr: removes it). Throws std::invalid_argument if the slot does not allow the node's class
    // or the node is in another tree, UsageError for a root. A node that is a descendant of this one (`paren->replace_with(
    // paren->expression())`) is taken out of the tree first.
    void replace_with(AstNode* new_node);
    // Replaces this node by what `replace_function` makes of it: the node is removed first, the function gets it (now parentless) and
    // returns the root of a tree (or nullptr), which is put where the node was. Returns the replacement.
    template <class F>
        requires std::invocable<F, AstNode*>
    AstNode* replace_with(F&& replace_function) {
        return replace_with_function(std::function<AstNode*(AstNode*)>(std::forward<F>(replace_function)));
    }

    // A deep copy of this subtree, unattached. Annotations are copied (the ones that are ICloneable are cloned), and so is trivia.
    AstNode* clone() const;
    il::ArenaObject* clone_object() const override { return clone(); }

    // ---- visitors
    void accept_visitor(IAstVisitor& visitor) { dispatch_to(visitor); }
    template <class S>
    S accept_visitor(IAstVisitorT<S>& visitor) {
        detail::ResultVisitor<S> adapter(visitor);
        dispatch_to(adapter);
        return adapter.take_result();
    }
    template <class T, class S>
    S accept_visitor(IAstVisitorTS<T, S>& visitor, T data) {
        detail::ResultDataVisitor<T, S> adapter(visitor, std::move(data));
        dispatch_to(adapter);
        return adapter.take_result();
    }

    // ---- pattern matching
    // Whether this node, as a pattern, matches `other` (upstream's generated DoMatch): a node of the same class with matching
    // children and scalars. `other` may be nullptr.
    virtual bool do_match(AstNode* other, const pattern_matching::Match& match) const = 0;
    // INode: matches `other` if it is absent or a node, by do_match.
    bool match_one(pattern_matching::INode* other, const pattern_matching::Match& match) const override;
    bool match_in_collection(const pattern_matching::INodeList& other, std::size_t pos, const pattern_matching::Match& match,
                             pattern_matching::BacktrackingInfo& backtracking_info) const override;
    // A node that stands for `pattern` in a tree (upstream's implicit conversion Pattern -> AstNode); nullptr for nullptr.
    static AstNode* from_pattern(pattern_matching::Pattern* pattern);

    // ---- text
    // This node written as C# (needs the output visitor: see set_to_string_function). Overridden by the nodes that write themselves.
    virtual std::string to_string(const output_visitor::CSharpFormattingOptions* formatting_options = nullptr) const;
    // The text a debugger shows: to_string() on one line, 100 characters at most.
    std::string debug_to_string() const;
    // The output visitor's hook for to_string (the output visitor installs it): until it does, to_string throws.
    using ToStringFunction = std::string (*)(const AstNode&, const output_visitor::CSharpFormattingOptions*);
    static void set_to_string_function(ToStringFunction function);

    // Whether the location is between start_location() (inclusive) and end_location() (exclusive) / both inclusive.
    bool contains(int line, int column) const { return contains(TextLocation(line, column)); }
    bool contains(TextLocation location) const { return start_location() <= location && location < end_location(); }
    bool is_inside(int line, int column) const { return is_inside(TextLocation(line, column)); }
    bool is_inside(TextLocation location) const { return start_location() <= location && location <= end_location(); }

    // ---- the slot storage contract (upstream: internal members the generator writes for each node class)
    // The children are numbered in slot declaration order; a single slot takes one index (get_child_by_index returns nullptr while it
    // is empty), a collection a run of its current length. All throw std::out_of_range past the end.
    virtual int get_child_count() const { return 0; }
    virtual AstNode* get_child_by_index(int index) const;
    virtual void set_child_by_index(int index, AstNode* value);
    virtual const CSharpSlotInfo& get_child_slot_info(int index) const;
    // The collection occupying the slot of the shared kind `kind`, or nullptr if this node has none.
    virtual AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo&) const { return nullptr; }
    // Fills `copy` (a shallow copy of this node: scalars copied, no children) with clones of this node's children.
    virtual void clone_children_into(AstNode*) const {}

    // Recursively checks the slot structure of this subtree (asserts, so only in debug builds): required slots are filled, each child
    // points back at this node, has the right index and a class the slot allows. A node class overrides it (calling the base) for its
    // own scalar invariants.
    virtual void check_invariant();

protected:
    AstNode() = default;
    // A shallow copy: the scalars, the locations and the annotation list, but no parent, no index and (by the children's own copy
    // rules) no children. This is upstream's MemberwiseClone; clone() builds on it through shallow_clone().
    AstNode(const AstNode& other);

    // ---- what a node class implements
    // Calls the visit method of this class on `visitor` (`visitor.visit_x(*this)`).
    virtual void dispatch_to(IAstVisitor& visitor) = 0;
    // A shallow copy of this node: `return make<X>(*this);`.
    virtual AstNode* shallow_clone() const = 0;

    // ---- helpers for the node classes
    // Equal strings, or the pattern text is Pattern::any_string. (An absent string and an empty one are the same here.)
    static bool match_string(std::string_view pattern, std::string_view text) { return pattern_matching::Pattern::match_string(pattern, text); }
    // Both children absent, or both present and matching (a present pattern child decides, e.g. an OptionalNode accepts an absent one).
    static bool match_optional(const AstNode* this_child, AstNode* other_child, const pattern_matching::Match& match) {
        return this_child != nullptr ? this_child->do_match(other_child, match) : other_child == nullptr;
    }

    // A child the class requires (a non-optional slot): matches like match_optional, but a missing child on this side is an error
    // (upstream: a NullReferenceException), std::logic_error.
    static bool match_required(const AstNode* this_child, AstNode* other_child, const pattern_matching::Match& match) {
        if (this_child == nullptr) throw std::logic_error("a required child of a node used as a pattern is missing");
        return this_child->do_match(other_child, match);
    }

    // Sets the single child of a slot to `value`, reparenting: the new child's parent is this node, the old one is unattached. The
    // first form is for a slot whose flattened index is not fixed (a single slot following a collection), the second for one whose
    // index is `index`. Throws std::invalid_argument for this node itself or a node that is in another tree (a node inside the child
    // being replaced is taken out of it first, so `x.right = x.right.right` works).
    template <class T>
    void set_child_node(ChildPtr<T>& field, T* value);
    template <class T>
    void set_child_node(ChildPtr<T>& field, T* value, int index);

    // Sets the single child in the slot of the kind `slot_kind` (throws UsageError if this node has no such slot).
    template <class T>
    void set_child(const CSharpSlotInfoT<T>& slot_kind, T* new_child) {
        set_child_by_kind(slot_kind, new_child);
    }
    void set_child_by_kind(const CSharpSlotInfo& kind, AstNode* child);

    // For a collection to tell this node its children's indices are stale.
    void invalidate_child_indices() { child_indices_valid_ = false; }

private:
    template <class T>
    friend class AstNodeCollection;
    friend class AstNodeCollectionBase;
    friend class Trivia;
    friend class ChildEnumerator;
    friend class ChildrenCollection;

    struct NodeTrivia;
    NodeTrivia* get_or_create_trivia();
    void insert_trivia(std::vector<Trivia*>& list, std::size_t index, Trivia* trivia);
    void validate_new_trivia(Trivia* trivia) const;
    static void reindex_trivia(AstNode* parent, std::vector<Trivia*>& list, std::size_t start);
    void reparent_trivia();

    void ensure_child_indices();
    void validate_new_single_child(AstNode* value, AstNode* old_child);
    void set_parent(AstNode* new_parent) { parent_ = new_parent; }
    void clear_parent_and_index();
    void throw_if_trivia() const;
    // Whether this is trivia that sits in its owner's trivia list (rather than in a slot).
    bool is_listed_trivia() const;
    AstNode* replace_with_function(const std::function<AstNode*(AstNode*)>& replace_function);
    void check_trivia_invariant(const std::vector<Trivia*>* list);

    AstNode* parent_ = nullptr;
    // The flattened index of this node within its parent's children (-1 when unparented); recomputed lazily by the parent.
    int child_index_ = -1;
    // Whether this node's children currently carry correct flattened indices (cleared by structural changes, restored on first read).
    bool child_indices_valid_ = true;
    TextLocation start_location_;
    TextLocation end_location_;
};

// ---- casts -----------------------------------------------------------------------------------------------------------------------

// Whether `node` is a T (upstream's `node is T`): by kind, so it works for the abstract bases and for classes not yet defined.
template <class T>
bool is(const AstNode* node) {
    return node != nullptr && kind_of<T>::range.contains(node->node_kind());
}
// `node` as a T, or nullptr (upstream's `node as T`).
template <class T>
T* as(AstNode* node) {
    return is<T>(node) ? static_cast<T*>(node) : nullptr;
}
template <class T>
const T* as(const AstNode* node) {
    return is<T>(node) ? static_cast<const T*>(node) : nullptr;
}
// The same for an INode that may be a pattern: nullptr when it is not a syntax node of that class.
template <class T>
T* as(pattern_matching::INode* node) {
    return as<T>(dynamic_cast<AstNode*>(node));
}

// `node` as a T, the way C# writes a cast `(T)node`: nullptr stays nullptr, a node of another class throws std::bad_cast.
template <class T>
T* cast(AstNode* node) {
    if (node == nullptr) return nullptr;
    T* result = as<T>(node);
    if (result == nullptr) throw std::bad_cast();
    return result;
}

// A deep copy of `node` (AstNode::clone) as its own class: `clone_as(left_)` where upstream writes `(Identifier)slot.Clone()`.
template <class T>
T* clone_as(const T* node) {
    return static_cast<T*>(node->clone());
}

// ---- templates of AstNode ----------------------------------------------------------------------------------------------------------

template <class T>
T* AstNode::get_parent() const {
    for (AstNode* cur = parent_; cur != nullptr; cur = cur->parent_)
        if (is<T>(cur)) return static_cast<T*>(cur);
    return nullptr;
}

template <class T>
T* AstNode::get_child(const CSharpSlotInfoT<T>& slot_kind) const {
    int count = get_child_count();
    for (int i = 0; i < count; i++) {
        if (get_child_slot_info(i).kind() == &slot_kind) return as<T>(get_child_by_index(i));
    }
    return nullptr;
}

template <class T>
AstNodeCollection<T>& AstNode::get_children(const CSharpSlotInfoT<T>& slot_kind) const {
    AstNodeCollectionBase* collection = get_collection_by_kind(slot_kind);
    // A node has no children of a kind it does not declare a collection slot for. A read of such a kind gets a detached empty
    // collection; writes go through add_child / set_child, which reject a missing slot.
    if (collection != nullptr) return static_cast<AstNodeCollection<T>&>(*collection);
    return *make<AstNodeCollection<T>>(const_cast<AstNode*>(this), slot_kind);
}

template <class T>
void AstNode::set_child_node(ChildPtr<T>& field, T* value) {
    T* old_field = field.get();
    if (old_field == value) return;
    validate_new_single_child(value, old_field);
    int old_child_index = old_field != nullptr ? static_cast<AstNode*>(old_field)->child_index_ : -1;
    if (old_field != nullptr) static_cast<AstNode*>(old_field)->clear_parent_and_index();
    field.value_ = value;
    if (value != nullptr) static_cast<AstNode*>(value)->set_parent(this);
    if (old_field != nullptr && value != nullptr)
        static_cast<AstNode*>(value)->child_index_ = old_child_index;
    else
        invalidate_child_indices();
}

template <class T>
void AstNode::set_child_node(ChildPtr<T>& field, T* value, int index) {
    T* old_field = field.get();
    if (old_field == value) return;
    validate_new_single_child(value, old_field);
    if (old_field != nullptr) static_cast<AstNode*>(old_field)->clear_parent_and_index();
    field.value_ = value;
    if (value != nullptr) {
        AstNode* node = value;
        node->set_parent(this);
        node->child_index_ = index;
    }
}

}  // namespace syntax
}  // namespace cb::dotnet::decompiler::csharp
