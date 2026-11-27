// dotnet/decompiler/csharp/syntax/ast_node_collection.cpp -- see ast_node_collection.hpp.
#include "dotnet/decompiler/csharp/syntax/ast_node_collection.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// ---- the enumerator ------------------------------------------------------------------------------------------------------------------

bool AstNodeCollectionBase::Enumerator::move_next() {
    const std::vector<AstNode*>& list = *list_;
    if (!started_) {
        started_ = true;
        if (list.empty()) {
            current_ = nullptr;
            return false;
        }
        pos_ = 0;
    } else {
        if (next_ == nullptr) {
            current_ = nullptr;
            return false;
        }
        // Resume on the successor captured before the body ran, located by identity. The two fast paths cover the common shapes (the
        // successor still sits at pos + 1, or `current` was removed so the successor dropped into pos); any other change falls back to
        // a search by identity.
        std::size_t resume;
        auto found = [&]() { return static_cast<std::size_t>(std::find(list.begin(), list.end(), next_) - list.begin()); };
        if (pos_ + 1 < list.size() && list[pos_ + 1] == next_)
            resume = pos_ + 1;
        else if (pos_ < list.size() && list[pos_] == next_)
            resume = pos_;
        else
            resume = found();
        assert(resume == found() && "the enumerator lost track of its position during a change made while enumerating");
        if (resume >= list.size()) {
            current_ = nullptr;
            next_ = nullptr;
            return false;
        }
        pos_ = resume;
    }
    current_ = list[pos_];
    next_ = pos_ + 1 < list.size() ? list[pos_ + 1] : nullptr;
    return true;
}

void AstNodeCollectionBase::Enumerator::reset() {
    pos_ = 0;
    current_ = nullptr;
    next_ = nullptr;
    started_ = false;
}

// ---- the collection ------------------------------------------------------------------------------------------------------------------

AstNodeCollectionBase::AstNodeCollectionBase(AstNode* parent_node, const CSharpSlotInfo* slot_kind, int first_index, bool incremental_indices)
    : parent_(parent_node), kind_(slot_kind), base_index_(first_index), supports_incremental_(incremental_indices) {}

AstNodeCollectionBase::AstNodeCollectionBase(const AstNodeCollectionBase& other)
    : il::ArenaObject(),
      pattern_matching::INodeList(),
      parent_(nullptr),
      kind_(other.kind_),
      base_index_(other.base_index_),
      supports_incremental_(other.supports_incremental_) {}

pattern_matching::INode* AstNodeCollectionBase::node_at(std::size_t index) const { return list_[index]; }

// Reassigns the flattened child index of every element from `start` to the end after a shift, keeping the parent's indices valid
// without a full rebuild. Only meaningful on the incremental fast path.
void AstNodeCollectionBase::reindex_from(std::size_t start) {
    for (std::size_t i = start; i < list_.size(); i++) list_[i]->child_index_ = base_index_ + static_cast<int>(i);
}

bool AstNodeCollectionBase::incremental() const { return supports_incremental_ && parent_->child_indices_valid_; }

void AstNodeCollectionBase::validate_new_child(AstNode* child) const {
    if (child == nullptr) throw std::invalid_argument("child");
    if (child == parent_) throw std::invalid_argument("Cannot add a node to itself as a child.");
    if (child->parent() != nullptr) throw std::invalid_argument("Node is already used in another tree.");
    if (kind_ != nullptr && !kind_->is_instance(child))
        throw std::invalid_argument(std::format("The node of kind {} is not valid in the slot {}", static_cast<int>(child->node_kind()), kind_->name()));
}

void AstNodeCollectionBase::set_node(std::size_t index, AstNode* value) {
    AstNode* old = list_.at(index);
    if (old == value) return;
    validate_new_child(value);
    int old_child_index = old->child_index_;
    old->clear_parent_and_index();
    list_[index] = value;
    value->set_parent(parent_);
    // Replacing an element in place keeps its flattened position, so no index changes: carry the old element's index to the new one
    // rather than invalidating (and rebuilding) the parent's whole index set. A stale carried value is corrected by the next renumber.
    value->child_index_ = old_child_index;
}

void AstNodeCollectionBase::add_node(AstNode* element) {
    if (element == nullptr) return;
    validate_new_child(element);
    list_.push_back(element);
    element->set_parent(parent_);
    // Appending leaves every existing index unchanged, so just index the new element instead of invalidating; otherwise rebuild later.
    if (supports_incremental_ && parent_->child_indices_valid_)
        element->child_index_ = base_index_ + static_cast<int>(list_.size()) - 1;
    else
        parent_->invalidate_child_indices();
}

bool AstNodeCollectionBase::contains_node(const AstNode* element) const {
    return element != nullptr && element->parent() == parent_ && std::find(list_.begin(), list_.end(), element) != list_.end();
}

int AstNodeCollectionBase::index_of_node(const AstNode* element) const {
    if (element == nullptr || element->parent() != parent_) return -1;
    // O(1) when the indices are current and this collection owns a contiguous flattened range: the element's local position is its
    // child index minus our base.
    if (supports_incremental_ && parent_->child_indices_valid_) {
        int local = element->child_index_ - base_index_;
        if (local >= 0 && static_cast<std::size_t>(local) < list_.size() && list_[static_cast<std::size_t>(local)] == element) return local;
    }
    auto it = std::find(list_.begin(), list_.end(), element);
    return it == list_.end() ? -1 : static_cast<int>(it - list_.begin());
}

bool AstNodeCollectionBase::remove_node(AstNode* element) {
    int index = index_of_node(element);
    if (index < 0) return false;
    bool keep_current = incremental();
    list_.erase(list_.begin() + index);
    element->clear_parent_and_index();
    // The elements after the removed one shifted down by one; renumber just them rather than invalidating the parent's index set.
    if (keep_current)
        reindex_from(static_cast<std::size_t>(index));
    else
        parent_->invalidate_child_indices();
    return true;
}

void AstNodeCollectionBase::insert_node_before(AstNode* existing, AstNode* new_item) {
    // `existing` may belong to a different slot (e.g. the node after the last collection element); in that case it is not found here
    // and the new node is appended. A null `existing` appends as well.
    int index = index_of_node(existing);
    insert_at(index < 0 ? list_.size() : static_cast<std::size_t>(index), new_item);
}

void AstNodeCollectionBase::insert_node_after(AstNode* existing, AstNode* new_item) {
    // A null or foreign `existing` has index -1, so the new item is inserted at the front.
    insert_at(static_cast<std::size_t>(index_of_node(existing) + 1), new_item);
}

void AstNodeCollectionBase::insert_at(std::size_t index, AstNode* new_item) {
    if (new_item == nullptr) return;
    validate_new_child(new_item);
    bool keep_current = incremental();
    list_.insert(list_.begin() + static_cast<std::ptrdiff_t>(index), new_item);
    new_item->set_parent(parent_);
    // The new element and everything after it occupy fresh positions; renumber from the insertion point rather than invalidating the
    // parent's whole index set.
    if (keep_current)
        reindex_from(index);
    else
        parent_->invalidate_child_indices();
}

void AstNodeCollectionBase::clear() {
    if (list_.empty()) return;
    for (AstNode* item : list_) item->clear_parent_and_index();
    list_.clear();
    parent_->invalidate_child_indices();
}

std::vector<AstNode*> AstNodeCollectionBase::detach_nodes() {
    std::vector<AstNode*> result = list_;
    clear();
    return result;
}

void AstNodeCollectionBase::clone_from(AstNode* new_parent, const AstNodeCollectionBase& source) {
    parent_ = new_parent;
    for (AstNode* item : source.list_) add_node(item->clone());
}

bool AstNodeCollectionBase::do_match(const AstNodeCollectionBase& other, const pattern_matching::Match& match) const {
    // Both collections are already the per-slot child lists, so the matcher walks them by index.
    return pattern_matching::Pattern::match_collections(*this, other, match);
}

void AstNodeCollectionBase::accept_visitor(IAstVisitor& visitor) const {
    for (Enumerator e(&list_); e.move_next();) e.current()->accept_visitor(visitor);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
