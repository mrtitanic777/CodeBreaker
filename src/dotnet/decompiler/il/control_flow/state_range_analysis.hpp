// dotnet/decompiler/il/control_flow/state_range_analysis.hpp -- symbolically runs the code of a state machine's MoveNext (or Dispose) to
// find out for which values of the `state` field each block can be reached; the result maps each state to the block that handles it.
//
//     StateRangeAnalysis sra(StateRangeAnalysisMode::IteratorMoveNext, state_field);
//     sra.assign_state_ranges(body, util::LongSet::universe());
//     auto block_for_state = sra.get_block_state_set_mapping(body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: StateRangeAnalysisMode, StateRangeAnalysis, from
// ControlFlow/StateRangeAnalysis.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's
// Dictionary members are InsertionOrderedMap here, as .NET's Dictionary enumerates in insertion order while nothing was removed.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/control_flow/symbolic_execution.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/decompiler/util/long_dict.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

// A Dictionary that enumerates in the order its keys were added (what .NET's does when no entry was removed; a removed entry is skipped
// and a key added after a removal goes to the end, where .NET would reuse the freed slot, which the transforms never rely on).
template <class K, class V>
class InsertionOrderedMap {
public:
    bool contains_key(const K& key) const { return index_.contains(key); }
    // Adds an entry; false (and no change) if the key is there already.
    bool try_add(const K& key, V value) {
        if (index_.contains(key)) return false;
        index_.emplace(key, entries_.size());
        entries_.emplace_back(key, std::move(value));
        return true;
    }
    // The value of the key, or nullptr.
    V* find(const K& key) {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &entries_[it->second].second;
    }
    const V* find(const K& key) const {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &entries_[it->second].second;
    }
    // Upstream's `dict[key] = value`.
    void set(const K& key, V value) {
        auto it = index_.find(key);
        if (it == index_.end()) {
            index_.emplace(key, entries_.size());
            entries_.emplace_back(key, std::move(value));
        } else {
            entries_[it->second].second = std::move(value);
        }
    }
    bool remove(const K& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return false;
        std::size_t pos = it->second;
        entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(pos));
        index_.erase(it);
        for (auto& entry : index_)
            if (entry.second > pos) --entry.second;
        return true;
    }
    void clear() {
        entries_.clear();
        index_.clear();
    }
    std::size_t size() const { return entries_.size(); }
    const std::vector<std::pair<K, V>>& entries() const { return entries_; }
    auto begin() const { return entries_.begin(); }
    auto end() const { return entries_.end(); }

private:
    std::vector<std::pair<K, V>> entries_;
    std::unordered_map<K, std::size_t> index_;
};

enum class StateRangeAnalysisMode : std::uint8_t {
    IteratorMoveNext,
    IteratorDispose,
    AsyncMoveNext,
    AwaitInFinally,
};

// Symbolically executes code to determine which blocks are reachable for which values of the 'state' field.
//
// Assumption: there are no loops/backward jumps.
// We 'run' the code, with "state" being a symbolic variable so it can form expressions like "state + x" (when there's a sub
// instruction). For each block, we maintain the set of values for state for which the block is reachable. This is
// (int.MinValue, int.MaxValue) for the first instruction. These ranges are propagated depending on the conditional jumps performed by
// the code.
class StateRangeAnalysis {
public:
    util::CancellationToken cancellation_token;

    StateRangeAnalysis(StateRangeAnalysisMode mode, const IField* state_field, ILVariable* cached_state_var = nullptr,
                       bool legacy_visual_basic = false);

    // The variables found to hold a copy of the state.
    const std::vector<ILVariable*>& cached_state_vars() const { return eval_context_.state_variables(); }

    // Creates a new StateRangeAnalysis with the same settings, including any cached state vars discovered by this analysis. However,
    // the new analysis has a fresh set of result ranges.
    StateRangeAnalysis create_nested_analysis() const;

    // Assigns state ranges for all blocks within 'inst'. Returns the set of states for which the exit point of the instruction is
    // reached; this is a subset of the input set. Returns an empty set for unsupported instructions.
    util::LongSet assign_state_ranges(ILInstruction* inst, util::LongSet state_range);

    // Gets a mapping from states to blocks. Within the given container (which should be the container that was analyzed), the mapping
    // prefers the last block. Blocks outside of the given container are preferred over blocks within the container.
    util::LongDict<Block*> get_block_state_set_mapping(BlockContainer* container) const;
    util::LongDict<BlockContainer*> get_block_state_set_mapping_for_leave() const;

    // Used only for IteratorDispose: the set of states for which each finally method is called (empty: not that mode).
    std::optional<InsertionOrderedMap<const IMethod*, util::LongSet>> finally_method_to_state_range;
    ILVariable* do_finally_bodies = nullptr;
    ILVariable* skip_finally_bodies = nullptr;

private:
    void add_state_range(Block* block, util::LongSet state_range);
    void add_state_range_for_leave(BlockContainer* target, util::LongSet state_range);

    StateRangeAnalysisMode mode_;
    const IField* state_field_;
    bool legacy_visual_basic_;
    SymbolicEvaluationContext eval_context_;

    InsertionOrderedMap<Block*, util::LongSet> ranges_;
    // used only for AwaitInFinally
    std::optional<InsertionOrderedMap<BlockContainer*, util::LongSet>> ranges_for_leave_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
