// dotnet/decompiler/il/il_instruction.cpp -- see il_instruction.hpp.
#include "dotnet/decompiler/il/il_instruction.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/patterns.hpp"

namespace cb::dotnet::decompiler::il {

// ---- the arena

namespace {
thread_local ILArena* current_arena = nullptr;
}

ILArena& ILArena::current() {
    if (current_arena == nullptr) throw UsageError("no ILArena is current on this thread (make an ILArenaScope)");
    return *current_arena;
}
ILArena* ILArena::current_or_null() { return current_arena; }

ILArenaScope::ILArenaScope(ILArena& arena) : previous_(current_arena) { current_arena = &arena; }
ILArenaScope::~ILArenaScope() { current_arena = previous_; }

const SlotInfo SlotInfo::none{"<no slot>"};

// ---- ILInstruction

ILInstruction::ILInstruction(const ILInstruction& other)
    : ArenaObject(), op_code(other.op_code), parent_(nullptr), child_index_(other.child_index_), flags_(invalid_flags), il_range_(other.il_range_),
      ref_count_(0) {}

void ILInstruction::validate_child(ILInstruction* inst) const {
    if (inst == nullptr) throw std::invalid_argument("inst");
    assert(!is_descendant_of(inst) && "ILAst must form a tree");
}

void ILInstruction::check_invariant(ILPhase phase) {
    for (ILInstruction* child : children()) {
        assert(child->parent() == this);
        assert(get_child(child->child_index()) == child);
        // if a child's flags are invalid, the parent's must be too (except nested functions, lambdas)
        assert(op_code == OpCode::ILFunction || child->flags_ != invalid_flags || flags_ == invalid_flags);
        assert(child->is_connected() == is_connected());
        child->check_invariant(phase);
    }
    assert(!any(direct_flags() & ~flags()) && "All DirectFlags must also appear in this.Flags");
    (void)phase;
}

bool ILInstruction::is_descendant_of(const ILInstruction* possible_ancestor) const {
    for (const ILInstruction* ancestor = this; ancestor != nullptr; ancestor = ancestor->parent_)
        if (ancestor == possible_ancestor) return true;
    return false;
}

ILInstruction* ILInstruction::get_common_parent(ILInstruction* other) {
    if (other == nullptr) throw std::invalid_argument("other");
    ILInstruction* a = this;
    ILInstruction* b = other;
    int level_a = a->count_ancestors();
    int level_b = b->count_ancestors();
    while (level_a > level_b) {
        a = a->parent_;
        --level_a;
    }
    while (level_b > level_a) {
        b = b->parent_;
        --level_b;
    }
    while (a != b) {
        a = a->parent_;
        b = b->parent_;
    }
    return a;
}

bool ILInstruction::is_before(const ILInstruction* other) const {
    if (other == nullptr) throw std::invalid_argument("other");
    const ILInstruction* a = this;
    const ILInstruction* b = other;
    int level_a = a->count_ancestors();
    int level_b = b->count_ancestors();
    int original_level_a = level_a;
    int original_level_b = level_b;
    while (level_a > level_b) {
        a = a->parent_;
        --level_a;
    }
    while (level_b > level_a) {
        b = b->parent_;
        --level_b;
    }
    // one is a descendant of the other: the deeper one comes first in a post-order walk
    if (a == b) return original_level_a > original_level_b;
    while (a->parent_ != b->parent_) {
        a = a->parent_;
        b = b->parent_;
    }
    // now a and b have the same parent or are both roots
    return a->child_index_ < b->child_index_;
}

int ILInstruction::count_ancestors() const {
    int level = 0;
    for (const ILInstruction* ancestor = this; ancestor != nullptr; ancestor = ancestor->parent_) ++level;
    return level;
}

StackType ILInstruction::common_result_type(StackType a, StackType b) {
    if (a == StackType::I || b == StackType::I) return StackType::I;
    assert(a == b);
    return a;
}

InstructionFlags ILInstruction::flags() const {
    if (flags_ == invalid_flags) flags_ = compute_flags();
    return flags_;
}

void ILInstruction::invalidate_flags() {
    for (ILInstruction* inst = this; inst != nullptr && inst->flags_ != invalid_flags; inst = inst->parent_) inst->flags_ = invalid_flags;
}

Interval ILInstruction::combine_il_range(Interval old_range, Interval new_range) {
    if (old_range.is_empty()) return new_range;
    if (new_range.is_empty()) return old_range;
    if (new_range.start <= old_range.start) {
        if (new_range.end < old_range.start) return new_range;  // the earlier range
        return Interval(new_range.start, std::max(new_range.end, old_range.end));  // join overlapping ranges
    }
    if (new_range.start <= old_range.end) return Interval(old_range.start, std::max(new_range.end, old_range.end));
    return old_range;
}

void ILInstruction::write_to(ITextOutput& output, const ILAstWritingOptions& options) const { write_to_core(output, options); }

std::string ILInstruction::to_string() const {
    PlainTextOutput output;
    write_to(output, ILAstWritingOptions{});
    if (!il_range_.is_empty()) output.write(std::format(" at IL_{:04x}", il_range_.start));
    return output.text();
}

const SlotInfo* ILInstruction::slot_info() const {
    if (parent_ == nullptr) return nullptr;
    assert(parent_->get_child(child_index_) == this);
    return &parent_->get_child_slot(child_index_);
}

void ILInstruction::replace_with(ILInstruction* replacement) {
    assert(parent_->get_child(child_index_) == this);
    if (replacement == this) return;
    parent_->set_child(child_index_, replacement);
}

std::vector<ILInstruction*> ILInstruction::ancestors() {
    std::vector<ILInstruction*> result;
    for (ILInstruction* node = this; node != nullptr; node = node->parent_) result.push_back(node);
    return result;
}

// Upstream's Descendants: a stack of child enumerators, each with its end fixed when it starts; a node is yielded after its children.
ILInstruction::DescendantsRange::iterator::iterator(ILInstruction* root) : root_(root), done_(false) {
    stack_.push_back({root, -1, root->get_child_count()});
    advance();
}

void ILInstruction::DescendantsRange::iterator::advance() {
    if (done_) return;
    if (stack_.empty()) {  // the root was the last
        done_ = true;
        current_ = nullptr;
        return;
    }
    while (true) {
        Frame& top = stack_.back();
        if (++top.pos < top.end) {
            ILInstruction* element = top.inst->get_child(top.pos);
            stack_.push_back({element, -1, element->get_child_count()});
            continue;
        }
        // this enumerator is done: its node comes next, or the root after everything
        stack_.pop_back();
        if (!stack_.empty()) {
            Frame& parent = stack_.back();
            current_ = parent.inst->get_child(parent.pos);
        } else {
            current_ = root_;
        }
        return;
    }
}

void ILInstruction::add_ref() {
    if (ref_count_++ == 0) connected();
}

void ILInstruction::release_ref() {
    assert(ref_count_ > 0);
    if (--ref_count_ == 0) disconnected();
}

void ILInstruction::connected() {
    for (ILInstruction* child : children()) child->add_ref();
}

void ILInstruction::disconnected() {
    for (ILInstruction* child : children()) child->release_ref();
}

void ILInstruction::instruction_collection_added(ILInstruction* new_child) {
    assert(get_child(new_child->child_index_) == new_child);
    assert(!is_descendant_of(new_child) && "ILAst must form a tree");
    new_child->parent_ = this;
    if (ref_count_ > 0) new_child->add_ref();
}

void ILInstruction::instruction_collection_removed(ILInstruction* old_child) {
    if (ref_count_ > 0) old_child->release_ref();
}

void ILInstruction::instruction_collection_update_complete() { invalidate_flags(); }

patterns::Match ILInstruction::match(ILInstruction* node) {
    patterns::Match match;
    match.set_success(perform_match(node, match));
    return match;
}

bool ILInstruction::perform_match(patterns::ListMatch& list_match, patterns::Match& match) {
    // a node matches one element (patterns matching none or several override this)
    if (list_match.syntax_index < static_cast<int>(list_match.syntax_list.size())) {
        if (perform_match(list_match.syntax_list[static_cast<std::size_t>(list_match.syntax_index)], match)) {
            ++list_match.syntax_index;
            return true;
        }
    }
    return false;
}

bool ILInstruction::can_inline_into_slot(int child_index, ILInstruction*) { return get_child_slot(child_index).can_inline_into; }

bool ILInstruction::satisfies_slot_restriction_for_inlining(int, ILInstruction*) { return true; }

}  // namespace cb::dotnet::decompiler::il
