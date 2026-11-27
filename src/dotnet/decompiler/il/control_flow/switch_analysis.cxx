// dotnet/decompiler/il/control_flow/switch_analysis.cxx -- see switch_analysis.hpp.
#include "dotnet/decompiler/il/control_flow/switch_analysis.hpp"

#include <cassert>
#include <limits>
#include <stdexcept>

namespace cb::dotnet::decompiler::il::control_flow {

using util::LongInterval;
using util::LongSet;

bool SwitchAnalysis::analyze_block(Block* block) {
    switch_var_ = nullptr;
    root_block_ = block;
    target_block_to_section_index_.clear();
    target_container_to_section_index_.clear();
    sections.clear();
    inner_blocks.clear();
    contains_il_switch_ = false;
    return analyze_block(block, LongSet::universe(), true);
}

// Analyzes the tail end (last two instructions) of a block, or the whole block when `tail_only` is false.
// Sets switch_var_ if it is null, and adds the sections found. If the function returns false, `sections` is unmodified.
bool SwitchAnalysis::analyze_block(Block* block, LongSet input_values, bool tail_only) {
    if (block->instructions.count() == 0) {
        // might happen if the block was already marked for deletion in SwitchDetection
        return false;
    }
    if (tail_only) {
        assert(block == root_block_);
    } else {
        assert(switch_var_ != nullptr);  // the top-level call determines the switch variable
        if (block->incoming_edge_count() != 1 || block == root_block_) return false;  // for now, only if-structures that form a tree
        if (block->parent() != root_block_->parent()) return false;                   // all blocks should belong to the same container
    }
    LongSet true_values;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (block->instructions.count() >= 2 && block->instructions[block->instructions.count() - 2]->match_if_instruction(&condition, &true_inst) &&
        analyze_condition(condition, &true_values)) {
        if (!(tail_only || block->instructions.count() == 2)) return false;
        true_values = true_values.intersect_with(input_values);
        if (true_values.set_equals(input_values) || true_values.is_empty()) return false;
        Block* true_block = nullptr;
        if (true_inst->match_branch(&true_block) && analyze_block(true_block, true_values, false)) {
            // OK, true block was further analyzed.
            inner_blocks.push_back(true_block);
        } else {
            // Create switch section for trueInst.
            add_section(true_values, true_inst);
        }
    } else if (auto* switch_inst = as<SwitchInstruction>(block->instructions.last())) {
        if (!(tail_only || block->instructions.count() == 1)) return false;
        if (analyze_switch(switch_inst, input_values)) {
            contains_il_switch_ = true;  // OK
            return true;
        } else {
            // switch analysis failed (e.g. switchVar mismatch)
            return false;
        }
    } else {
        // unknown inst
        return false;
    }

    LongSet remaining_values = input_values.except_with(true_values);
    ILInstruction* false_inst = block->instructions.last();
    Block* false_block = nullptr;
    if (false_inst->match_branch(&false_block) && analyze_block(false_block, remaining_values, false)) {
        // OK, false block was further analyzed.
        inner_blocks.push_back(false_block);
    } else {
        // Create switch section for falseInst.
        add_section(remaining_values, false_inst);
    }
    return true;
}

bool SwitchAnalysis::analyze_switch(SwitchInstruction* inst, const LongSet& input_values) {
    assert(!inst->is_lifted);
    std::int64_t offset;
    if (match_switch_var(inst->value())) {
        offset = 0;
    } else if (auto* bop = as<BinaryNumericInstruction>(inst->value())) {
        if (bop->check_for_overflow) return false;
        std::int64_t val = 0;
        if (match_switch_var(bop->left()) && bop->right()->match_ldc_i(&val)) {
            switch (bop->op) {
                case BinaryNumericOperator::Add:
                    offset = static_cast<std::int64_t>(0 - static_cast<std::uint64_t>(val));
                    break;
                case BinaryNumericOperator::Sub:
                    offset = val;
                    break;
                default:  // unknown bop.Operator
                    return false;
            }
        } else {
            // unknown bop.Left
            return false;
        }
    } else {
        // unknown inst.Value
        return false;
    }
    for (SwitchSection* section : inst->sections) {
        LongSet match_values = section->labels().add_offset(offset).intersect_with(input_values);
        if (!allow_unreachable_cases && match_values.is_empty()) return false;
        Block* target_block = nullptr;
        if (match_values.count() > 1 && section->body()->match_branch(&target_block) && analyze_block(target_block, match_values, false)) {
            inner_blocks.push_back(target_block);
        } else {
            add_section(match_values, section->body());
        }
    }
    return true;
}

// Adds a new section to the sections list.
// If the instruction is a branch instruction, unify the new section with an existing section that also branches to the same target.
void SwitchAnalysis::add_section(LongSet values, ILInstruction* inst) {
    if (values.is_empty()) return;
    Block* target_block = nullptr;
    BlockContainer* target_container = nullptr;
    if (inst->match_branch(&target_block)) {
        auto it = target_block_to_section_index_.find(target_block);
        if (it != target_block_to_section_index_.end()) {
            int index = it->second;
            sections[static_cast<std::size_t>(index)] = {sections[static_cast<std::size_t>(index)].first.union_with(values), inst};
        } else {
            target_block_to_section_index_.emplace(target_block, static_cast<int>(sections.size()));
            sections.emplace_back(std::move(values), inst);
        }
    } else if (inst->match_leave(&target_container)) {
        auto it = target_container_to_section_index_.find(target_container);
        if (it != target_container_to_section_index_.end()) {
            int index = it->second;
            sections[static_cast<std::size_t>(index)] = {sections[static_cast<std::size_t>(index)].first.union_with(values), inst};
        } else {
            target_container_to_section_index_.emplace(target_container, static_cast<int>(sections.size()));
            sections.emplace_back(std::move(values), inst);
        }
    } else {
        sections.emplace_back(std::move(values), inst);
    }
}

bool SwitchAnalysis::match_switch_var(ILInstruction* inst) {
    if (switch_var_ != nullptr) return inst->match_ld_loc(switch_var_);
    return inst->match_ld_loc(&switch_var_);
}

bool SwitchAnalysis::match_switch_var(ILInstruction* inst, std::int64_t* sub) {
    auto* bn = as<BinaryNumericInstruction>(inst);
    if (bn != nullptr && bn->op == BinaryNumericOperator::Sub && !bn->check_for_overflow && !bn->is_lifted() && bn->right()->match_ldc_i(sub)) {
        return match_switch_var(bn->left());
    }
    *sub = 0;
    return match_switch_var(inst);
}

// Analyzes the boolean condition, returning the set of values of the interesting variable for which the condition evaluates to true.
bool SwitchAnalysis::analyze_condition(ILInstruction* condition, LongSet* true_values) {
    auto* comp = as<Comp>(condition);
    std::int64_t sub = 0;
    std::int64_t val = 0;
    ILInstruction* arg = nullptr;
    if (comp != nullptr && match_switch_var(comp->left(), &sub) && comp->right()->match_ldc_i(&val)) {
        // if (comp((V - sub) OP val))
        *true_values = make_set_where_comparison_is_true(comp->kind(), val, comp->sign);
        *true_values = true_values->add_offset(sub);
        return true;
    } else if (match_switch_var(condition)) {
        // if (ldloc V) --> branch for all values except 0
        *true_values = LongSet(std::int64_t{0}).invert();
        return true;
    } else if (condition->match_logic_not(&arg)) {
        // if (logic.not(X)) --> branch for all values where if (X) does not branch
        LongSet false_values;
        bool res = analyze_condition(arg, &false_values);
        *true_values = false_values.invert();
        return res;
    } else {
        *true_values = LongSet::empty_set();
        return false;
    }
}

// Creates the LongSet that contains a value x iff x compared with value is true.
LongSet SwitchAnalysis::make_set_where_comparison_is_true(ComparisonKind kind, std::int64_t val, Sign sign) {
    switch (kind) {
        case ComparisonKind::Equality: return LongSet(val);
        case ComparisonKind::Inequality: return LongSet(val).invert();
        case ComparisonKind::LessThan: return make_greater_than_or_equal_set(val, sign).invert();
        case ComparisonKind::LessThanOrEqual: return make_less_than_or_equal_set(val, sign);
        case ComparisonKind::GreaterThan: return make_less_than_or_equal_set(val, sign).invert();
        case ComparisonKind::GreaterThanOrEqual: return make_greater_than_or_equal_set(val, sign);
    }
    throw std::invalid_argument("Invalid ComparisonKind");
}

LongSet SwitchAnalysis::make_greater_than_or_equal_set(std::int64_t val, Sign sign) {
    if (sign == Sign::Signed) {
        return LongSet(LongInterval::inclusive(val, std::numeric_limits<std::int64_t>::max()));
    } else {
        assert(sign == Sign::Unsigned);
        if (val >= 0) {
            // The range val to ulong.MaxValue expressed with signed longs is not a single contiguous range, but two ranges:
            return LongSet(LongInterval::inclusive(val, std::numeric_limits<std::int64_t>::max()))
                .union_with(LongSet(LongInterval(std::numeric_limits<std::int64_t>::min(), 0)));
        } else {
            return LongSet(LongInterval(val, 0));
        }
    }
}

LongSet SwitchAnalysis::make_less_than_or_equal_set(std::int64_t val, Sign sign) {
    if (sign == Sign::Signed) {
        return LongSet(LongInterval::inclusive(std::numeric_limits<std::int64_t>::min(), val));
    } else {
        assert(sign == Sign::Unsigned);
        if (val >= 0) {
            return LongSet(LongInterval::inclusive(0, val));
        } else {
            // The range 0 to (ulong)val expressed with signed longs is not a single contiguous range, but two ranges:
            return LongSet(LongInterval::inclusive(0, std::numeric_limits<std::int64_t>::max()))
                .union_with(LongSet(LongInterval::inclusive(std::numeric_limits<std::int64_t>::min(), val)));
        }
    }
}

}  // namespace cb::dotnet::decompiler::il::control_flow
