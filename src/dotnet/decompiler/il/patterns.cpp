// dotnet/decompiler/il/patterns.cpp -- see patterns.hpp.
#include "dotnet/decompiler/il/patterns.hpp"

#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::il {

namespace patterns {

bool ListMatch::do_match(const std::vector<ILInstruction*>& patterns, const std::vector<ILInstruction*>& syntax_list, Match& match) {
    ListMatch list_match(syntax_list);
    do {
        if (perform_match_sequence(patterns, list_match, match)) {
            // a successful match of the whole list is the end
            if (list_match.syntax_index == static_cast<int>(syntax_list.size())) return true;
        }
        // otherwise restore a save point made by perform_match and resume the matching there
    } while (list_match.restore_save_point(match));
    return false;
}

bool ListMatch::perform_match_sequence(const std::vector<ILInstruction*>& patterns, ListMatch& list_match, Match& match) {
    // The patterns may create save points, so the loop variable is part of those save points.
    for (int i = list_match.pop_from_save_point().value_or(0); i < static_cast<int>(patterns.size()); i++) {
        int start_marker = list_match.get_save_point_start_marker();
        bool success = patterns[static_cast<std::size_t>(i)]->perform_match(list_match, match);
        list_match.push_to_save_points(start_marker, i);
        if (!success) return false;
    }
    return true;
}

void ListMatch::add_save_point(Match& match, int data) {
    SavePoint save_point{match.check_point(), syntax_index, {}};
    save_point.stack.push_back(data);
    backtracking_stack_.push_back(std::move(save_point));
}

void ListMatch::push_to_save_points(int start_marker, int data) {
    for (std::size_t i = static_cast<std::size_t>(start_marker); i < backtracking_stack_.size(); i++) backtracking_stack_[i].stack.push_back(data);
}

std::optional<int> ListMatch::pop_from_save_point() {
    if (restore_stack_.empty()) return std::nullopt;
    int value = restore_stack_.back();
    restore_stack_.pop_back();
    return value;
}

bool ListMatch::restore_save_point(Match& match) {
    if (backtracking_stack_.empty()) return false;
    SavePoint save_point = std::move(backtracking_stack_.back());
    backtracking_stack_.pop_back();
    match.restore_check_point(save_point.check_point);
    syntax_index = save_point.syntax_index;
    restore_stack_ = std::move(save_point.stack);
    return true;
}

}  // namespace patterns

// ---- PatternInstruction

InstructionFlags PatternInstruction::compute_flags() const { throw UsageError("a pattern has no flags (NotSupported)"); }
InstructionFlags PatternInstruction::direct_flags() const { throw UsageError("a pattern has no flags (NotSupported)"); }

// ---- AnyNode

ILInstruction* AnyNode::get_child(int) const { throw std::out_of_range("child index"); }
void AnyNode::set_child(int, ILInstruction*) { throw std::out_of_range("child index"); }
const SlotInfo& AnyNode::get_child_slot(int) const { throw std::out_of_range("child index"); }

ILInstruction* AnyNode::clone() const { return make<AnyNode>(*this); }

void AnyNode::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('(');
    output.write(')');
}

bool AnyNode::perform_match(ILInstruction* other, patterns::Match& match) {
    if (other == nullptr) return false;
    if (group_ != nullptr) match.add(group_, other);
    return true;
}

}  // namespace cb::dotnet::decompiler::il
