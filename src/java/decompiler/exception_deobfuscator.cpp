// java/decompiler/exception_deobfuscator.cpp -- the exception ranges of a graph made regular before statements are built: circular ranges cut, the pop
// that starts a handler split off where another range covers it, empty ranges dropped, and an empty block put before every handler. See cfg.hpp.
// Ported from Fernflower (de.fernflower.modules.decompiler.deobfuscator.ExceptionDeobfuscator), Copyright 2000-2015 JetBrains s.r.o., Apache License
// 2.0 (see NOTICE in this folder).
#include <algorithm>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/dominators.hpp"

namespace cb::java::decompiler::exception_deobfuscator {

namespace {

template <class T> bool ed_contains(const std::vector<T>& v, const T& x) { return std::find(v.begin(), v.end(), x) != v.end(); }
template <class T> void ed_add_unique(std::vector<T>& v, const T& x) {
    if (!ed_contains(v, x)) v.push_back(x);
}

}  // namespace

void restore_pop_ranges(ControlFlowGraph& graph) {
    struct Range {
        BasicBlock* handler;
        std::optional<std::string> unique_str;
        std::vector<BasicBlock*> protected_range;   // (a set: only its members and emptiness are read)
        ExceptionRangeCFG* range_cfg;
    };
    std::vector<Range> ranges;
    for (const auto& range : graph.exceptions()) {
        bool found = false;
        const std::optional<std::string> unique = range->unique_exceptions_string();
        for (Range& arr : ranges)
            if (arr.handler == range->handler && unique == arr.unique_str) {
                for (BasicBlock* b : range->protected_range) ed_add_unique(arr.protected_range, b);
                found = true;
                break;
            }
        if (!found) {
            std::vector<BasicBlock*> set;
            for (BasicBlock* b : range->protected_range) ed_add_unique(set, b);
            ranges.push_back({range->handler, unique, std::move(set), range.get()});
        }
    }
    for (Range& range : ranges) {
        if (!range.unique_str) continue;
        BasicBlock* handler = range.handler;
        std::shared_ptr<InstructionSequence> seq = handler->seq;
        if (seq->length() == 0) continue;
        const std::shared_ptr<Instruction> first_instr = seq->instr(0);
        if (first_instr->opcode != cc::opc_pop && first_instr->opcode != cc::opc_astore) continue;
        const std::vector<BasicBlock*> set_range = range.protected_range;
        for (Range& range_super : ranges) {
            if (&range == &range_super) continue;
            std::vector<BasicBlock*> set_super = range_super.protected_range;
            const bool contains_all = std::all_of(set_range.begin(), set_range.end(), [&](BasicBlock* b) { return ed_contains(set_super, b); });
            if (ed_contains(set_range, range_super.handler) || ed_contains(set_super, handler) || !(!range_super.unique_str || contains_all)) continue;
            if (!range_super.unique_str) std::erase_if(set_super, [&](BasicBlock* b) { return !ed_contains(set_range, b); });
            else std::erase_if(set_super, [&](BasicBlock* b) { return ed_contains(set_range, b); });
            if (set_super.empty()) continue;
            BasicBlock* new_block = handler;
            if (seq->length() > 1) {   // the handler split: its first pop in a block of its own
                new_block = graph.make_block();
                new_block->id = ++graph.last_id;
                new_block->seq->add_instruction(first_instr->clone(), -1);
                graph.blocks().add_with_key(new_block, new_block->id);
                std::vector<BasicBlock*> temp = handler->preds;
                temp.insert(temp.end(), handler->pred_exceptions.begin(), handler->pred_exceptions.end());
                for (BasicBlock* pred : temp) pred->replace_successor(handler, new_block);
                for (const auto& range_ext : graph.exceptions()) {
                    if (range_ext->handler == handler) {
                        range_ext->handler = new_block;
                    } else if (ed_contains(range_ext->protected_range, handler)) {
                        new_block->add_successor_exception(range_ext->handler);
                        range_ext->protected_range.push_back(new_block);
                    }
                }
                new_block->add_successor(handler);
                if (graph.first() == handler) graph.set_first(new_block);
                seq->remove_instruction(0);
            }
            new_block->add_successor_exception(range_super.handler);
            range_super.range_cfg->protected_range.push_back(new_block);
            handler = range.range_cfg->handler;
            seq = handler->seq;
        }
    }
}

void insert_empty_exception_handler_blocks(ControlFlowGraph& graph) {
    std::vector<BasicBlock*> visited;
    for (std::size_t r = 0; r < graph.exceptions().size(); ++r) {
        BasicBlock* handler = graph.exceptions()[r]->handler;
        if (ed_contains(visited, handler)) continue;
        visited.push_back(handler);
        BasicBlock* empty = graph.make_block();
        empty->id = ++graph.last_id;
        graph.blocks().add_with_key(empty, empty->id);
        const std::vector<BasicBlock*> temp = handler->pred_exceptions;   // (only the exception predecessors)
        for (BasicBlock* pred : temp) pred->replace_successor(handler, empty);
        for (const auto& range_ext : graph.exceptions()) {
            if (range_ext->handler == handler) {
                range_ext->handler = empty;
            } else if (ed_contains(range_ext->protected_range, handler)) {
                empty->add_successor_exception(range_ext->handler);
                range_ext->protected_range.push_back(empty);
            }
        }
        empty->add_successor(handler);
        if (graph.first() == handler) graph.set_first(empty);
    }
}

void remove_empty_ranges(ControlFlowGraph& graph) {
    auto& ranges = graph.exceptions();
    for (std::size_t i = ranges.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *ranges[i];
        const bool empty = std::all_of(range.protected_range.begin(), range.protected_range.end(), [](BasicBlock* b) { return b->seq->empty(); });
        if (!empty) continue;
        for (BasicBlock* block : range.protected_range) block->remove_successor_exception(range.handler);
        ranges.erase(ranges.begin() + static_cast<std::ptrdiff_t>(i));
    }
}

void remove_circular_ranges(ControlFlowGraph& graph) {
    const GenericDominatorEngine<BasicBlock*> engine(graph.reverse_post_order(), {graph.first()}, [](BasicBlock* b) { return b->predecessors(); });
    auto& ranges = graph.exceptions();
    for (std::size_t i = ranges.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *ranges[i];
        BasicBlock* handler = range.handler;
        std::vector<BasicBlock*>& list = range.protected_range;
        if (!ed_contains(list, handler)) continue;
        // the blocks of the range reachable from the handler that it dominates
        std::vector<BasicBlock*> rem, visited, stack{handler};
        while (!stack.empty()) {
            BasicBlock* block = stack.front();
            stack.erase(stack.begin());
            ed_add_unique(visited, block);
            if (ed_contains(list, block) && engine.is_dominator(block, handler)) {
                rem.push_back(block);
                std::vector<BasicBlock*> succs = block->succs;
                succs.insert(succs.end(), block->succ_exceptions.begin(), block->succ_exceptions.end());
                for (BasicBlock* succ : succs)
                    if (!ed_contains(visited, succ)) stack.push_back(succ);
            }
        }
        if (rem.size() < list.size() || list.size() == 1)
            for (BasicBlock* block : rem) {
                block->remove_successor_exception(handler);
                if (const auto it = std::find(list.begin(), list.end(), block); it != list.end()) list.erase(it);
            }
        if (list.empty()) ranges.erase(ranges.begin() + static_cast<std::ptrdiff_t>(i));
    }
}

bool has_obfuscated_exceptions(const ControlFlowGraph& graph) {
    std::vector<std::pair<BasicBlock*, std::vector<BasicBlock*>>> map;
    for (const auto& range : graph.exceptions()) {
        auto it = std::find_if(map.begin(), map.end(), [&](const auto& e) { return e.first == range->handler; });
        if (it == map.end()) {
            map.emplace_back(range->handler, std::vector<BasicBlock*>{});
            it = map.end() - 1;
        }
        for (BasicBlock* b : range->protected_range) ed_add_unique(it->second, b);
    }
    for (const auto& [handler, set] : map) {
        int entries = 0;
        for (BasicBlock* block : set)
            if (std::any_of(block->preds.begin(), block->preds.end(), [&](BasicBlock* p) { return !ed_contains(set, p); })) ++entries;
        if (entries > 1) return true;
    }
    return false;
}

}  // namespace cb::java::decompiler::exception_deobfuscator
