// dotnet/decompiler/csharp/syntax/ast_node.cpp -- see ast_node.hpp.
#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>

#include "core/error.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/trivia.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

namespace {
AstNode::ToStringFunction to_string_function = nullptr;
}  // namespace

bool AstNode::is_listed_trivia() const {
    return kind_of<Trivia>::range.contains(node_kind()) && static_cast<const Trivia*>(this)->trivia_siblings_ != nullptr;
}

// ---- the enumerators ---------------------------------------------------------------------------------------------------------------

bool ChildEnumerator::move_next() {
    current_ = started_ ? next_ : node_->first_child();
    started_ = true;
    if (current_ == nullptr) return false;
    // Remember the successor before the child is handed out, so the loop body may remove or replace the current child.
    next_ = current_->next_sibling();
    return true;
}

void ChildEnumerator::reset() {
    current_ = nullptr;
    next_ = nullptr;
    started_ = false;
}

int ChildrenCollection::count() const {
    int count = 0;
    for (ChildEnumerator e(node_); e.move_next();) count++;
    return count;
}

AstNode* ChildrenCollection::operator[](int index) const {
    int i = 0;
    for (AstNode* child : *this) {
        if (i++ == index) return child;
    }
    throw std::out_of_range("index");
}

std::vector<AstNode*> ChildrenCollection::to_vector() const {
    std::vector<AstNode*> result;
    for (AstNode* child : *this) result.push_back(child);
    return result;
}

bool AncestorEnumerator::move_next() {
    if (!started_) {
        started_ = true;
        current_ = next_;
    } else {
        current_ = current_ != nullptr ? current_->parent() : nullptr;
    }
    return current_ != nullptr;
}

bool DescendantEnumerator::move_next() {
    for (;;) {
        switch (state_) {
            case State::Start:
                if (include_self_) {
                    current_ = root_;
                    state_ = State::AfterSelf;
                    return true;
                }
                next_stack_.push(nullptr);
                pos_ = root_->first_child();
                state_ = State::InLoop;
                break;
            case State::AfterSelf:
                if (descend_ && !descend_(root_)) {
                    state_ = State::Done;
                    break;
                }
                next_stack_.push(nullptr);
                pos_ = root_->first_child();
                state_ = State::InLoop;
                break;
            case State::InLoop: {
                if (pos_ == nullptr) {
                    state_ = State::Done;
                    break;
                }
                // Remember next before handing out pos: this allows removing or replacing nodes while iterating.
                AstNode* pos_next = pos_->next_sibling();
                if (pos_next != nullptr) next_stack_.push(pos_next);
                current_ = pos_;
                state_ = State::AfterYield;
                return true;
            }
            case State::AfterYield: {
                // Resumed after the caller used the node: its children are read now, so a replaced node's old children are still seen.
                AstNode* pos_first_child = pos_->first_child();
                if (pos_first_child != nullptr && (!descend_ || descend_(pos_))) {
                    pos_ = pos_first_child;
                } else {
                    pos_ = next_stack_.top();
                    next_stack_.pop();
                }
                state_ = State::InLoop;
                break;
            }
            case State::Done:
                current_ = nullptr;
                return false;
        }
    }
}

// ---- construction ------------------------------------------------------------------------------------------------------------------

AstNode::AstNode(const AstNode& other)
    : il::ArenaObject(),
      AbstractAnnotatable(other),
      pattern_matching::INode(),
      ICloneable(),
      parent_(nullptr),
      child_index_(-1),
      child_indices_valid_(other.child_indices_valid_),
      start_location_(other.start_location_),
      end_location_(other.end_location_) {}

// ---- trivia ------------------------------------------------------------------------------------------------------------------------

// Comments and preprocessor directives attached to a node, kept off the child-index space. The holder lives among the annotations, so
// a node without trivia (the overwhelmingly common case) costs nothing extra, and cloning a node's annotations clones it.
struct AstNode::NodeTrivia final : il::ArenaObject, ICloneable {
    std::vector<Trivia*> leading;
    std::vector<Trivia*> trailing;

    il::ArenaObject* clone_object() const override {
        auto* copy = make<NodeTrivia>();
        for (const Trivia* t : leading) copy->leading.push_back(static_cast<Trivia*>(t->clone()));
        for (const Trivia* t : trailing) copy->trailing.push_back(static_cast<Trivia*>(t->clone()));
        return copy;
    }
};

std::vector<Trivia*> AstNode::leading_trivia() const {
    const NodeTrivia* holder = annotation<NodeTrivia>();
    return holder != nullptr ? holder->leading : std::vector<Trivia*>();
}

std::vector<Trivia*> AstNode::trailing_trivia() const {
    const NodeTrivia* holder = annotation<NodeTrivia>();
    return holder != nullptr ? holder->trailing : std::vector<Trivia*>();
}

void AstNode::add_leading_trivia(Trivia* trivia) {
    NodeTrivia* holder = get_or_create_trivia();
    insert_trivia(holder->leading, holder->leading.size(), trivia);
}

void AstNode::prepend_leading_trivia(Trivia* trivia) {
    NodeTrivia* holder = get_or_create_trivia();
    insert_trivia(holder->leading, 0, trivia);
}

void AstNode::add_trailing_trivia(Trivia* trivia) {
    NodeTrivia* holder = get_or_create_trivia();
    insert_trivia(holder->trailing, holder->trailing.size(), trivia);
}

// The single way trivia is attached: validate, insert, then reindex from the insertion point so every trivia's parent, list and index
// are consistent whatever the position.
void AstNode::insert_trivia(std::vector<Trivia*>& list, std::size_t index, Trivia* trivia) {
    validate_new_trivia(trivia);
    list.insert(list.begin() + static_cast<std::ptrdiff_t>(index), trivia);
    reindex_trivia(this, list, index);
}

void AstNode::validate_new_trivia(Trivia* trivia) const {
    if (trivia == nullptr) throw std::invalid_argument("trivia");
    if (trivia == this) throw std::invalid_argument("Cannot add a node to itself as trivia.");
    if (static_cast<AstNode*>(trivia)->parent_ != nullptr) throw std::invalid_argument("Node is already used in another tree.");
}

void AstNode::reindex_trivia(AstNode* parent, std::vector<Trivia*>& list, std::size_t start) {
    for (std::size_t i = start; i < list.size(); i++) list[i]->set_trivia_parent(parent, &list, static_cast<int>(i));
}

// Points the trivia held in this node's annotation back at this node. Clone needs this: the cloned holder's trivia still carry the
// source owner's parent state.
void AstNode::reparent_trivia() {
    NodeTrivia* holder = annotation<NodeTrivia>();
    if (holder == nullptr) return;
    reindex_trivia(this, holder->leading, 0);
    reindex_trivia(this, holder->trailing, 0);
}

// Deep-copies another node's trivia onto this node, appending to any already present. The holder is never shared between nodes: each
// trivia's parent is its single owner, so the trivia is cloned.
void AstNode::copy_trivia_from(const AstNode& other) {
    const NodeTrivia* other_holder = other.annotation<NodeTrivia>();
    if (other_holder == nullptr) return;
    for (const Trivia* trivia : std::vector<Trivia*>(other_holder->leading)) add_leading_trivia(static_cast<Trivia*>(trivia->clone()));
    for (const Trivia* trivia : std::vector<Trivia*>(other_holder->trailing)) add_trailing_trivia(static_cast<Trivia*>(trivia->clone()));
}

AstNode::NodeTrivia* AstNode::get_or_create_trivia() {
    NodeTrivia* holder = annotation<NodeTrivia>();
    if (holder == nullptr) {
        holder = make<NodeTrivia>();
        add_annotation(holder);
    }
    return holder;
}

// ---- navigation --------------------------------------------------------------------------------------------------------------------

AstNode* AstNode::next_sibling() const {
    if (parent_ == nullptr) return nullptr;
    if (is_listed_trivia()) {
        const std::vector<Trivia*>& siblings = *static_cast<const Trivia*>(this)->trivia_siblings_;
        return static_cast<std::size_t>(child_index_ + 1) < siblings.size() ? siblings[static_cast<std::size_t>(child_index_ + 1)] : nullptr;
    }
    if (!parent_->child_indices_valid_) parent_->ensure_child_indices();
    int count = parent_->get_child_count();
    for (int i = child_index_ + 1; i < count; i++) {
        AstNode* c = parent_->get_child_by_index(i);
        if (c != nullptr) return c;
    }
    return nullptr;
}

AstNode* AstNode::prev_sibling() const {
    if (parent_ == nullptr) return nullptr;
    if (is_listed_trivia()) {
        const std::vector<Trivia*>& siblings = *static_cast<const Trivia*>(this)->trivia_siblings_;
        return child_index_ > 0 ? siblings[static_cast<std::size_t>(child_index_ - 1)] : nullptr;
    }
    if (!parent_->child_indices_valid_) parent_->ensure_child_indices();
    for (int i = child_index_ - 1; i >= 0; i--) {
        AstNode* c = parent_->get_child_by_index(i);
        if (c != nullptr) return c;
    }
    return nullptr;
}

AstNode* AstNode::first_child() const {
    int count = get_child_count();
    for (int i = 0; i < count; i++) {
        AstNode* c = get_child_by_index(i);
        if (c != nullptr) return c;
    }
    return nullptr;
}

AstNode* AstNode::last_child() const {
    for (int i = get_child_count() - 1; i >= 0; i--) {
        AstNode* c = get_child_by_index(i);
        if (c != nullptr) return c;
    }
    return nullptr;
}

AstNode* AstNode::get_parent(const std::function<bool(AstNode*)>& pred) const {
    for (AstNode* cur = parent_; cur != nullptr; cur = cur->parent_)
        if (!pred || pred(cur)) return cur;
    return nullptr;
}

AstNode* AstNode::get_next_node() const {
    if (AstNode* next = next_sibling()) return next;
    // Trivia navigation ends at its owning list: leading trivia precedes its owner in document order, so continuing into the owner's
    // sibling space would skip the owner's whole subtree.
    if (kind_of<Trivia>::range.contains(node_kind())) return nullptr;
    if (parent_ != nullptr) return parent_->get_next_node();
    return nullptr;
}

AstNode* AstNode::get_next_node(const std::function<bool(AstNode*)>& pred) const {
    AstNode* next = get_next_node();
    while (next != nullptr && !pred(next)) next = next->get_next_node();
    return next;
}

AstNode* AstNode::get_prev_node() const {
    if (AstNode* prev = prev_sibling()) return prev;
    // Trivia navigation ends at its owning list; see get_next_node.
    if (kind_of<Trivia>::range.contains(node_kind())) return nullptr;
    if (parent_ != nullptr) return parent_->get_prev_node();
    return nullptr;
}

AstNode* AstNode::get_prev_node(const std::function<bool(AstNode*)>& pred) const {
    AstNode* prev = get_prev_node();
    while (prev != nullptr && !pred(prev)) prev = prev->get_prev_node();
    return prev;
}

AstNode* AstNode::get_next_sibling(const std::function<bool(AstNode*)>& pred) const {
    AstNode* next = next_sibling();
    while (next != nullptr && !pred(next)) next = next->next_sibling();
    return next;
}

AstNode* AstNode::get_prev_sibling(const std::function<bool(AstNode*)>& pred) const {
    AstNode* prev = prev_sibling();
    while (prev != nullptr && !pred(prev)) prev = prev->prev_sibling();
    return prev;
}

const CSharpSlotInfo* AstNode::slot() const {
    if (parent_ == nullptr) return nullptr;
    // Trivia occupies no child slot; its child index indexes its owning trivia list instead.
    if (is_listed_trivia()) return nullptr;
    parent_->ensure_child_indices();
    return &parent_->get_child_slot_info(child_index_);
}

// ---- the slot storage defaults -------------------------------------------------------------------------------------------------------

AstNode* AstNode::get_child_by_index(int) const { throw std::out_of_range("index"); }
void AstNode::set_child_by_index(int, AstNode*) { throw std::out_of_range("index"); }
const CSharpSlotInfo& AstNode::get_child_slot_info(int) const { throw std::out_of_range("index"); }

// Whether this node's children carry correct indices is restored lazily on the first index read, so bulk construction (many appends
// with no interleaved index reads) renumbers once instead of once per change. The arithmetic lives in each class's
// get_child_by_index / get_child_count, which depend only on its fields, so it is well defined whatever the stored indices are.
void AstNode::ensure_child_indices() {
    if (child_indices_valid_) return;
    child_indices_valid_ = true;
    int count = get_child_count();
    for (int i = 0; i < count; i++) {
        AstNode* c = get_child_by_index(i);
        if (c != nullptr) c->child_index_ = i;
    }
}

void AstNode::check_invariant() {
    ensure_child_indices();
#ifndef NDEBUG
    int count = get_child_count();
    for (int i = 0; i < count; i++) {
        const CSharpSlotInfo& slot_info = get_child_slot_info(i);
        AstNode* child = get_child_by_index(i);
        if (child == nullptr) {
            // A single slot reads null only when empty; that is valid only if the slot is optional. (A collection never yields a
            // null index, so this is always a single slot.)
            assert(slot_info.is_optional() && "a required slot must not be empty");
            continue;
        }
        assert(child->parent_ == this && "child's parent must point back to this node");
        assert(child->child_index_ == i && "child's flattened index must match its slot position");
        assert(slot_info.is_instance(child) && "child's type must be valid in its slot");
        child->check_invariant();
    }
    if (NodeTrivia* trivia = annotation<NodeTrivia>()) {
        check_trivia_invariant(&trivia->leading);
        check_trivia_invariant(&trivia->trailing);
    }
#endif
}

// Trivia lives outside the slot space but carries the same parent and index invariants, scoped to the list that holds it.
void AstNode::check_trivia_invariant([[maybe_unused]] const std::vector<Trivia*>* list) {
#ifndef NDEBUG
    for (std::size_t i = 0; i < list->size(); i++) {
        Trivia* t = (*list)[i];
        assert(t->parent_ == this && "trivia's parent must point back to its owning node");
        assert(t->trivia_siblings_ == list && "trivia's sibling list must be the list that holds it");
        assert(static_cast<std::size_t>(t->child_index_) == i && "trivia's index must match its position in the trivia list");
    }
#endif
}

// ---- changing the tree ---------------------------------------------------------------------------------------------------------------

// Self-reference and two-tree guards shared by the single-slot setters; lifts a node out of the subtree currently being replaced
// (e.g. `x.right = x.right.right`).
void AstNode::validate_new_single_child(AstNode* value, AstNode* old_child) {
    if (value == nullptr) return;
    if (value == this) throw std::invalid_argument("Cannot add a node to itself as a child.");
    if (value->parent_ != nullptr) {
        bool inside_old_child = false;
        if (old_child != nullptr) {
            for (AstNode* a = value->parent_; a != nullptr; a = a->parent_) {
                if (a == old_child) {
                    inside_old_child = true;
                    break;
                }
            }
        }
        if (inside_old_child)
            value->remove();
        else
            throw std::invalid_argument("Node is already used in another tree.");
    }
}

void AstNode::clear_parent_and_index() {
    parent_ = nullptr;
    if (kind_of<Trivia>::range.contains(node_kind())) static_cast<Trivia*>(this)->trivia_siblings_ = nullptr;
    child_index_ = -1;
}

void AstNode::set_child_by_kind(const CSharpSlotInfo& kind, AstNode* child) {
    int count = get_child_count();
    for (int i = 0; i < count; i++) {
        if (get_child_slot_info(i).kind() == &kind) {
            set_child_by_index(i, child);
            return;
        }
    }
    throw UsageError(std::format("node of kind {} has no slot of kind '{}'", static_cast<int>(node_kind()), kind.name()));
}

void AstNode::add_child(AstNode* child, const CSharpSlotInfo& kind) {
    if (child == nullptr) return;
    if (AstNodeCollectionBase* collection = get_collection_by_kind(kind))
        collection->add_node(child);
    else
        set_child_by_kind(kind, child);
}

void AstNode::insert_child_before(AstNode* next, AstNode* child, const CSharpSlotInfo& kind) {
    if (child == nullptr) return;
    if (AstNodeCollectionBase* collection = get_collection_by_kind(kind))
        collection->insert_node_before(next, child);
    else
        set_child_by_kind(kind, child);
}

void AstNode::insert_child_after(AstNode* prev, AstNode* child, const CSharpSlotInfo& kind) {
    if (child == nullptr) return;
    if (AstNodeCollectionBase* collection = get_collection_by_kind(kind))
        collection->insert_node_after(prev, child);
    else
        set_child_by_kind(kind, child);
}

void AstNode::remove() {
    if (parent_ == nullptr) return;
    if (is_listed_trivia()) {
        AstNode* old_parent = parent_;
        int old_index = child_index_;
        std::vector<Trivia*>* siblings = static_cast<Trivia*>(this)->trivia_siblings_;
        siblings->erase(siblings->begin() + old_index);
        clear_parent_and_index();
        reindex_trivia(old_parent, *siblings, static_cast<std::size_t>(old_index));
        return;
    }
    parent_->ensure_child_indices();
    const CSharpSlotInfo* kind = parent_->get_child_slot_info(child_index_).kind();
    AstNodeCollectionBase* collection = parent_->get_collection_by_kind(*kind);
    if (collection != nullptr)
        collection->remove_node(this);
    else
        parent_->set_child_by_index(child_index_, nullptr);
}

// Attached trivia has no child slot to substitute into: its child index indexes the owning trivia list, which the slot arithmetic in
// replace_with must never touch. Trivia is replaced by removing it and attaching new trivia to the owning node.
void AstNode::throw_if_trivia() const {
    if (is_listed_trivia()) throw UsageError("Cannot replace trivia; remove it and attach the replacement to the owning node instead.");
}

void AstNode::replace_with(AstNode* new_node) {
    if (new_node == nullptr) {
        remove();
        return;
    }
    if (new_node == this) return;  // nothing to do
    if (parent_ == nullptr) throw UsageError("Cannot replace the root node");
    throw_if_trivia();
    parent_->ensure_child_indices();
    const CSharpSlotInfo& slot_info = parent_->get_child_slot_info(child_index_);
    // Because this method does not statically check the new node's type against the slot, we test at run time.
    if (!slot_info.is_instance(new_node))
        throw std::invalid_argument(std::format("The new node of kind {} is not valid in the slot {}", static_cast<int>(new_node->node_kind()),
                                                slot_info.name()));
    if (new_node->parent_ != nullptr) {
        // Is new_node used within this tree (`parenthesized.replace_with(parenthesized.expression())`)? Then take it out first.
        bool inside_this = false;
        for (AstNode* a = new_node->parent_; a != nullptr; a = a->parent_) {
            if (a == this) {
                inside_this = true;
                break;
            }
        }
        if (inside_this)
            new_node->remove();
        else
            throw std::invalid_argument("Node is already used in another tree.");
    }
    parent_->set_child_by_index(child_index_, new_node);
}

AstNode* AstNode::replace_with_function(const std::function<AstNode*(AstNode*)>& replace_function) {
    if (!replace_function) throw std::invalid_argument("replace_function");
    if (parent_ == nullptr) throw UsageError("Cannot replace the root node");
    throw_if_trivia();
    AstNode* old_parent = parent_;
    AstNode* old_successor = next_sibling();
    const CSharpSlotInfo* old_slot = slot();
    const CSharpSlotInfo* old_kind = old_slot != nullptr ? old_slot->kind() : nullptr;
    remove();
    AstNode* replacement = replace_function(this);
    if (old_successor != nullptr && old_successor->parent_ != old_parent)
        throw UsageError("replace function changed nextSibling of node being replaced?");
    if (replacement != nullptr && old_kind != nullptr) {
        if (replacement->parent_ != nullptr) throw UsageError("replace function must return the root of a tree");
        if (old_slot != nullptr && !old_slot->is_instance(replacement))
            throw UsageError(std::format("The new node of kind {} is not valid in the slot {}", static_cast<int>(replacement->node_kind()),
                                         old_slot->name()));
        if (old_successor != nullptr)
            old_parent->insert_child_before(old_successor, replacement, *old_kind);
        else
            old_parent->add_child(replacement, *old_kind);
    }
    return replacement;
}

AstNode* AstNode::clone() const {
    // The shallow copy has no parent and no children (copy constructors); fill it with clones of the children, then clone the
    // annotations that need it, then point the cloned trivia at the copy.
    AstNode* copy = shallow_clone();
    clone_children_into(copy);
    copy->clone_annotations();
    copy->reparent_trivia();
    return copy;
}

// ---- pattern matching ------------------------------------------------------------------------------------------------------------------

bool AstNode::match_one(pattern_matching::INode* other, const pattern_matching::Match& match) const {
    AstNode* o = dynamic_cast<AstNode*>(other);
    // try matching if other is null, or if other is an AstNode
    return (other == nullptr || o != nullptr) && do_match(o, match);
}

bool AstNode::match_in_collection(const pattern_matching::INodeList& other, std::size_t pos, const pattern_matching::Match& match,
                                  pattern_matching::BacktrackingInfo&) const {
    pattern_matching::INode* raw = pos < other.size() ? other.node_at(pos) : nullptr;
    AstNode* o = dynamic_cast<AstNode*>(raw);
    // matches a single element: succeeds only if the element is absent or an AstNode
    return (raw == nullptr || o != nullptr) && do_match(o, match);
}

AstNode* AstNode::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<AstNode, AstNodeKind::AstNodePlaceholder>>(pattern);
}

// ---- text ------------------------------------------------------------------------------------------------------------------------------

void AstNode::set_to_string_function(ToStringFunction function) { to_string_function = function; }

std::string AstNode::to_string(const output_visitor::CSharpFormattingOptions* formatting_options) const {
    if (to_string_function == nullptr) throw UsageError("AstNode::to_string needs the C# output visitor, which installs it with set_to_string_function");
    return to_string_function(*this, formatting_options);
}

std::string AstNode::debug_to_string() const {
    std::string text = to_string();
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.pop_back();
    std::string result;
    for (std::size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '\t') continue;
        if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') continue;
        result += c == '\n' ? ' ' : c;
    }
    if (result.size() > 100) return result.substr(0, 97) + "...";
    return result;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
