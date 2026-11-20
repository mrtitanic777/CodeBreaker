// java/decompiler/cfg.cpp -- see cfg.hpp. Ported from Fernflower (de.fernflower.code.cfg, modules.code.DeadCodeHelper), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0.
#include "java/decompiler/cfg.hpp"

#include <algorithm>
#include <deque>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

namespace {

template <class T> bool cf_contains(const std::vector<T>& v, const T& x) { return std::find(v.begin(), v.end(), x) != v.end(); }
template <class T> bool cf_remove_first(std::vector<T>& v, const T& x) {
    const auto it = std::find(v.begin(), v.end(), x);
    if (it == v.end()) return false;
    v.erase(it);
    return true;
}
template <class T> void cf_remove_all(std::vector<T>& v, const T& x) { v.erase(std::remove(v.begin(), v.end(), x), v.end()); }
template <class T> void cf_add_unique(std::vector<T>& v, const T& x) {
    if (!cf_contains(v, x)) v.push_back(x);
}
// Java's retainAll/removeAll over insertion-ordered sets.
template <class T> void cf_retain(std::vector<T>& v, const std::vector<T>& keep) {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const T& x) { return !cf_contains(keep, x); }), v.end());
}
template <class T> void cf_remove_each(std::vector<T>& v, const std::vector<T>& drop) {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const T& x) { return cf_contains(drop, x); }), v.end());
}
template <class T> std::vector<T> cf_set_of(const std::vector<T>& v) {
    std::vector<T> out;
    for (const T& x : v) cf_add_unique(out, x);
    return out;
}

}  // namespace

// ---- Java's hash order

std::int32_t java_string_hash(std::string_view s) {
    std::uint32_t h = 0;
    for (std::size_t i = 0; i < s.size();) {   // UTF-8 to UTF-16 code units
        const auto b = static_cast<unsigned char>(s[i]);
        std::uint32_t cp = b;
        std::size_t n = 1;
        if ((b & 0xE0) == 0xC0) n = 2, cp = b & 0x1F;
        else if ((b & 0xF0) == 0xE0) n = 3, cp = b & 0x0F;
        else if ((b & 0xF8) == 0xF0) n = 4, cp = b & 0x07;
        for (std::size_t k = 1; k < n && i + k < s.size(); ++k) cp = (cp << 6) | (static_cast<unsigned char>(s[i + k]) & 0x3F);
        i += n;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            h = 31 * h + (0xD800 + (cp >> 10));
            h = 31 * h + (0xDC00 + (cp & 0x3FF));
        } else {
            h = 31 * h + cp;
        }
    }
    return static_cast<std::int32_t>(h);
}

std::vector<std::string> java_hash_set_order(const std::vector<std::string>& inserted) {
    std::vector<std::string> unique;
    std::size_t capacity = 16;
    for (const std::string& s : inserted) {
        if (cf_contains(unique, s)) continue;
        unique.push_back(s);
        if (unique.size() > capacity * 3 / 4) capacity *= 2;
    }
    const auto bucket = [capacity](const std::string& s) {
        const auto h = static_cast<std::uint32_t>(java_string_hash(s));
        return (h ^ (h >> 16)) & static_cast<std::uint32_t>(capacity - 1);
    };
    std::stable_sort(unique.begin(), unique.end(), [&](const std::string& a, const std::string& b) { return bucket(a) < bucket(b); });
    return unique;
}

// ---- BasicBlock

void BasicBlock::free() {
    preds.clear();
    succs.clear();
    instr_old_offsets.clear();
    succ_exceptions.clear();
    seq = std::make_shared<InstructionSequence>();
}

void BasicBlock::remove_predecessor(BasicBlock* block) { cf_remove_all(preds, block); }

void BasicBlock::add_successor(BasicBlock* block) {
    succs.push_back(block);
    block->add_predecessor(this);
}

void BasicBlock::remove_successor(BasicBlock* block) {
    cf_remove_all(succs, block);
    block->remove_predecessor(this);
}

void BasicBlock::replace_successor(BasicBlock* old_block, BasicBlock* new_block) {
    for (BasicBlock*& s : succs)
        if (s->id == old_block->id) {
            s = new_block;
            old_block->remove_predecessor(this);
            new_block->add_predecessor(this);
        }
    for (BasicBlock*& s : succ_exceptions)
        if (s->id == old_block->id) {
            s = new_block;
            old_block->remove_predecessor_exception(this);
            new_block->add_predecessor_exception(this);
        }
}

void BasicBlock::remove_predecessor_exception(BasicBlock* block) { cf_remove_all(pred_exceptions, block); }

void BasicBlock::add_successor_exception(BasicBlock* block) {
    if (!cf_contains(succ_exceptions, block)) {
        succ_exceptions.push_back(block);
        block->add_predecessor_exception(this);
    }
}

void BasicBlock::remove_successor_exception(BasicBlock* block) {
    cf_remove_all(succ_exceptions, block);
    block->remove_predecessor_exception(this);
}

bool BasicBlock::is_successor(const BasicBlock* block) const {
    return std::any_of(succs.begin(), succs.end(), [&](const BasicBlock* s) { return s->id == block->id; });
}

bool BasicBlock::is_predecessor(const BasicBlock* block) const {
    return std::any_of(preds.begin(), preds.end(), [&](const BasicBlock* p) { return p->id == block->id; });
}

std::vector<BasicBlock*> BasicBlock::predecessors() const {
    std::vector<BasicBlock*> out = preds;
    out.insert(out.end(), pred_exceptions.begin(), pred_exceptions.end());
    return out;
}

std::string BasicBlock::to_string(int indent) const { return std::to_string(id) + ":" + DecompilerContext::new_line_separator() + seq->to_string(indent); }

// ---- ExceptionRangeCFG

ExceptionRangeCFG::ExceptionRangeCFG(std::vector<BasicBlock*> range, BasicBlock* handler_, std::optional<std::vector<std::string>> types)
    : protected_range(std::move(range)), handler(handler_), exception_types(std::move(types)) {}

bool ExceptionRangeCFG::is_circular() const { return cf_contains(protected_range, handler); }

void ExceptionRangeCFG::add_exception_type(const std::optional<std::string>& type) {
    if (!exception_types) return;
    if (!type) exception_types.reset();
    else exception_types->push_back(*type);
}

std::optional<std::string> ExceptionRangeCFG::unique_exceptions_string() const {
    if (!exception_types) return std::nullopt;
    std::string ret;
    for (const std::string& e : java_hash_set_order(*exception_types)) {
        if (!ret.empty()) ret += ':';
        ret += e;
    }
    return ret;
}

// ---- ControlFlowGraph

BasicBlock* ControlFlowGraph::make_block() {
    arena_.push_back(std::make_unique<BasicBlock>());
    return arena_.back().get();
}

BasicBlock* ControlFlowGraph::clone_block(const BasicBlock& block) {
    BasicBlock* b = make_block();
    b->id = block.id;
    b->seq = block.seq->clone();
    b->instr_old_offsets = block.instr_old_offsets;
    return b;
}

ControlFlowGraph::ControlFlowGraph(const InstructionSequence& seq) { build_blocks(seq); }

void ControlFlowGraph::free() {
    for (BasicBlock* b : blocks_) b->free();
    blocks_.clear();
    first_ = nullptr;
    last_ = nullptr;
    exceptions_.clear();
    finally_exits_.clear();
}

void ControlFlowGraph::remove_markers() {
    for (BasicBlock* b : blocks_) b->mark = 0;
}

std::string ControlFlowGraph::to_string() const {
    const std::string nl = DecompilerContext::new_line_separator();
    std::string buf;
    for (const BasicBlock* block : blocks_) {
        buf += "----- Block " + std::to_string(block->id) + " -----" + nl;
        buf += block->to_string();
        buf += "----- Edges -----" + nl;
        for (const BasicBlock* s : block->succs) buf += ">>>>>>>>(regular) Block " + std::to_string(s->id) + nl;
        for (const BasicBlock* h : block->succ_exceptions) {
            const ExceptionRangeCFG* range = exception_range(h, block);
            if (range == nullptr) buf += ">>>>>>>>(exception) Block " + std::to_string(h->id) + "\tERROR: range not found!" + nl;
            else if (!range->exception_types) buf += ">>>>>>>>(exception) Block " + std::to_string(h->id) + "\tNULL" + nl;
            else
                for (const std::string& t : *range->exception_types) buf += ">>>>>>>>(exception) Block " + std::to_string(h->id) + "\t" + t + nl;
        }
        buf += "----- ----- -----" + nl;
    }
    return buf;
}

void ControlFlowGraph::inline_jsr(const StructMethod& method) {
    process_jsr();
    remove_jsr(method);
    remove_markers();
    dead_code::remove_empty_blocks(*this);
}

void ControlFlowGraph::remove_block(BasicBlock* block) {
    while (!block->succs.empty()) block->remove_successor(block->succs.front());
    while (!block->succ_exceptions.empty()) block->remove_successor_exception(block->succ_exceptions.front());
    while (!block->preds.empty()) block->preds.front()->remove_successor(block);
    while (!block->pred_exceptions.empty()) block->pred_exceptions.front()->remove_successor_exception(block);
    last_->remove_predecessor(block);
    blocks_.remove_with_key(block->id);
    for (std::size_t i = exceptions_.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *exceptions_[i];
        if (range.handler == block) {
            exceptions_.erase(exceptions_.begin() + static_cast<std::ptrdiff_t>(i));
        } else {
            cf_remove_first(range.protected_range, block);
            if (range.protected_range.empty()) exceptions_.erase(exceptions_.begin() + static_cast<std::ptrdiff_t>(i));
        }
    }
    subroutines_.erase(std::remove_if(subroutines_.begin(), subroutines_.end(), [block](const auto& e) { return e.first == block || e.second == block; }),
                       subroutines_.end());
}

ExceptionRangeCFG* ControlFlowGraph::exception_range(const BasicBlock* handler, const BasicBlock* block) const {
    for (std::size_t i = exceptions_.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *exceptions_[i];
        if (range.handler == handler && std::find(range.protected_range.begin(), range.protected_range.end(), block) != range.protected_range.end()) return &range;
    }
    return nullptr;
}

void ControlFlowGraph::build_blocks(const InstructionSequence& seq) {
    // the instructions that start a block
    const std::size_t len = seq.length();
    std::vector<short> states(len, 0);
    std::vector<int> exc_set;
    for (const auto& h : seq.exception_table()->handlers) {
        exc_set.push_back(h->from_instr);
        exc_set.push_back(h->to_instr);
        exc_set.push_back(h->handler_instr);
    }
    const auto mark = [&states](int i) { states.at(static_cast<std::size_t>(i)) = 1; };
    for (std::size_t i = 0; i < len; ++i) {
        if (cf_contains(exc_set, static_cast<int>(i))) states[i] = 1;
        const Instruction& instr = *seq.instr(i);
        switch (instr.group) {
            case cc::GROUP_JUMP:
                mark(instr.destination);
                [[fallthrough]];
            case cc::GROUP_RETURN:
                if (i + 1 < len) states[i + 1] = 1;
                break;
            case cc::GROUP_SWITCH:
                for (std::size_t j = instr.destinations.size(); j-- > 0;) mark(instr.destinations[j]);
                mark(instr.default_dest);
                if (i + 1 < len) states[i + 1] = 1;
                break;
            default: break;
        }
    }
    states.at(0) = 1;

    // the blocks
    std::vector<BasicBlock*> instr_blocks(len, nullptr);
    BasicBlock* current = nullptr;
    short counter = 0;
    int block_offset = 0;
    for (std::size_t i = 0; i < len; ++i) {
        if (states[i] == 1) {
            current = make_block();
            current->id = ++counter;
            blocks_.add_with_key(current, current->id);
            block_offset = seq.offset(i);
        }
        instr_blocks[i] = current;
        current->seq->add_instruction(seq.instr(i), seq.offset(i) - block_offset);
        current->instr_old_offsets.push_back(seq.offset(i));
    }
    last_id = counter;

    // the regular edges
    const auto block_at = [&instr_blocks](int i) {
        if (i < 0 || static_cast<std::size_t>(i) >= instr_blocks.size()) throw std::out_of_range("jump outside the code");
        return instr_blocks[static_cast<std::size_t>(i)];
    };
    for (std::size_t i = 0; i < blocks_.size(); ++i) {
        BasicBlock* block = blocks_[i];
        const Instruction& instr = *block->last_instruction();
        const bool fallthrough = instr.can_fallthrough();
        if (instr.group == cc::GROUP_JUMP) {
            block->add_successor(block_at(instr.destination));
        } else if (instr.group == cc::GROUP_SWITCH) {
            block->add_successor(block_at(instr.default_dest));
            for (const int d : instr.destinations) block->add_successor(block_at(d));
        }
        if (fallthrough && i + 1 < blocks_.size()) block->add_successor(blocks_[i + 1]);
    }
    set_exception_edges(seq, instr_blocks);
    set_subroutine_edges();
    set_first_and_last_blocks();
}

void ControlFlowGraph::set_exception_edges(const InstructionSequence& seq, const std::vector<BasicBlock*>& instr_blocks) {
    exceptions_.clear();
    std::map<std::string, ExceptionRangeCFG*> ranges;
    const auto at = [&instr_blocks](int i) -> BasicBlock* {
        if (i < 0 || static_cast<std::size_t>(i) >= instr_blocks.size()) throw std::out_of_range("handler outside the code");
        return instr_blocks[static_cast<std::size_t>(i)];
    };
    for (const auto& h : seq.exception_table()->handlers) {
        BasicBlock* from = at(h->from_instr);
        BasicBlock* to = at(h->to_instr);
        BasicBlock* handle = at(h->handler_instr);
        const std::string key = std::to_string(from->id) + ":" + std::to_string(to->id) + ":" + std::to_string(handle->id);
        if (const auto it = ranges.find(key); it != ranges.end()) {
            it->second->add_exception_type(h->exception_class);
            continue;
        }
        std::vector<BasicBlock*> range;
        for (int j = from->id; j < to->id; ++j) {
            BasicBlock* block = blocks_.get_with_key(j);
            range.push_back(block);
            block->add_successor_exception(handle);
        }
        std::optional<std::vector<std::string>> types;
        if (h->exception_class) types = std::vector<std::string>{*h->exception_class};
        exceptions_.push_back(std::make_unique<ExceptionRangeCFG>(std::move(range), handle, std::move(types)));
        ranges[key] = exceptions_.back().get();
    }
}

void ControlFlowGraph::set_subroutine_edges() {
    std::vector<std::pair<BasicBlock*, BasicBlock*>> subs;
    const auto put = [&subs](BasicBlock* enter, BasicBlock* exit) {
        for (auto& e : subs)
            if (e.first == enter) {
                e.second = exit;
                return;
            }
        subs.emplace_back(enter, exit);
    };
    for (BasicBlock* block : blocks_) {
        if (block->seq->last_instr()->opcode != cc::opc_jsr) continue;
        std::deque<BasicBlock*> stack{block};
        std::deque<std::vector<BasicBlock*>> jsr_stacks{std::vector<BasicBlock*>{}};
        std::vector<BasicBlock*> visited;
        while (!stack.empty()) {
            BasicBlock* node = stack.front();
            stack.pop_front();
            std::vector<BasicBlock*> jsrstack = std::move(jsr_stacks.front());
            jsr_stacks.pop_front();
            cf_add_unique(visited, node);
            const int op = node->seq->last_instr()->opcode;
            if (op == cc::opc_jsr) {
                jsrstack.push_back(node);
            } else if (op == cc::opc_ret) {
                if (jsrstack.empty()) throw std::out_of_range("ret outside a subroutine");
                BasicBlock* enter = jsrstack.back();
                BasicBlock* exit = blocks_.get_with_key(enter->id + 1);
                if (exit == nullptr) throw std::runtime_error("ERROR: last instruction jsr");
                if (!node->is_successor(exit)) node->add_successor(exit);
                jsrstack.pop_back();
                put(enter, exit);
            }
            if (!jsrstack.empty())
                for (BasicBlock* succ : node->succs)
                    if (!cf_contains(visited, succ)) {
                        stack.push_back(succ);
                        jsr_stacks.push_back(jsrstack);
                    }
        }
    }
    subroutines_ = std::move(subs);
}

void ControlFlowGraph::process_jsr() {
    while (process_jsr_ranges() != 0) {
    }
}

int ControlFlowGraph::process_jsr_ranges() {
    struct JsrRecord {
        BasicBlock* jsr;
        std::vector<BasicBlock*> range;
        BasicBlock* ret;
    };
    std::vector<JsrRecord> all;
    for (const auto& [jsr, ret] : subroutines_) all.push_back({jsr, jsr_range(jsr, ret), ret});
    std::vector<JsrRecord> sorted;
    for (JsrRecord& arr : all) {
        std::size_t i = 0;
        for (; i < sorted.size(); ++i)
            if (cf_contains(sorted[i].range, arr.jsr)) break;
        sorted.insert(sorted.begin() + static_cast<std::ptrdiff_t>(i), std::move(arr));
    }
    for (std::size_t i = 0; i < sorted.size(); ++i) {
        const JsrRecord& arr = sorted[i];
        for (std::size_t j = i + 1; j < sorted.size(); ++j) {
            const JsrRecord& arr1 = sorted[j];
            if (!cf_contains(arr.range, arr1.jsr) && !cf_contains(arr1.range, arr.jsr)) {
                std::vector<BasicBlock*> common = arr.range;
                cf_retain(common, arr1.range);
                if (!common.empty()) {
                    split_jsr_range(arr.jsr, arr.ret, common);
                    return 1;
                }
            }
        }
    }
    return 0;
}

std::vector<BasicBlock*> ControlFlowGraph::jsr_range(BasicBlock* jsr, BasicBlock* ret) {
    std::vector<BasicBlock*> blocks;
    std::deque<BasicBlock*> nodes{jsr};
    BasicBlock* dom = jsr->succs.at(0);
    while (!nodes.empty()) {
        BasicBlock* node = nodes.front();
        nodes.pop_front();
        for (int j = 0; j < 2; ++j) {
            const std::vector<BasicBlock*>* lst;
            if (j == 0) {
                if (node->last_instruction()->opcode == cc::opc_ret && cf_contains(node->succs, ret)) continue;
                lst = &node->succs;
            } else {
                if (node == jsr) continue;
                lst = &node->succ_exceptions;
            }
            for (std::size_t i = lst->size(); i-- > 0;) {
                BasicBlock* child = (*lst)[i];
                if (cf_contains(blocks, child)) continue;
                if (node != jsr) {
                    bool dominated = true;
                    for (BasicBlock* p : child->preds)
                        if (!dead_code::is_dominator(*this, p, dom)) dominated = false;
                    for (BasicBlock* p : child->pred_exceptions)
                        if (!dead_code::is_dominator(*this, p, dom)) dominated = false;
                    if (!dominated) continue;
                }
                if (child != last_) blocks.push_back(child);   // (the last block is a dummy)
                nodes.push_back(child);
            }
        }
    }
    return blocks;
}

void ControlFlowGraph::split_jsr_range(BasicBlock* jsr, BasicBlock* ret, const std::vector<BasicBlock*>& common_blocks) {
    std::deque<BasicBlock*> nodes{jsr};
    std::map<int, BasicBlock*> new_nodes{{jsr->id, jsr}};
    while (!nodes.empty()) {
        BasicBlock* node = nodes.front();
        nodes.pop_front();
        for (int j = 0; j < 2; ++j) {
            std::vector<BasicBlock*>* lst;
            if (j == 0) {
                if (node->last_instruction()->opcode == cc::opc_ret && cf_contains(node->succs, ret)) continue;
                lst = &node->succs;
            } else {
                if (node == jsr) continue;
                lst = &node->succ_exceptions;
            }
            for (std::size_t i = lst->size(); i-- > 0;) {
                if (i >= lst->size()) continue;   // (the list shrank under replace_successor)
                BasicBlock* child = (*lst)[i];
                const int child_id = child->id;
                if (const auto it = new_nodes.find(child_id); it != new_nodes.end()) {
                    node->replace_successor(child, it->second);
                } else if (cf_contains(common_blocks, child)) {
                    BasicBlock* copy = clone_block(*child);
                    copy->id = ++last_id;
                    if (copy->last_instruction()->opcode == cc::opc_ret && cf_contains(child->succs, ret)) {
                        copy->add_successor(ret);
                        child->remove_successor(ret);
                    } else {
                        for (std::size_t k = 0; k < child->succs.size(); ++k) copy->add_successor(child->succs[k]);
                    }
                    for (std::size_t k = 0; k < child->succ_exceptions.size(); ++k) copy->add_successor_exception(child->succ_exceptions[k]);
                    nodes.push_back(copy);
                    new_nodes[child_id] = copy;
                    if (cf_contains(last_->preds, child)) last_->add_predecessor(copy);
                    node->replace_successor(child, copy);
                    blocks_.add_with_key(copy, copy->id);
                } else {
                    new_nodes[child_id] = child;   // (stop at the first fixed node)
                }
            }
        }
    }
    split_jsr_exception_ranges(common_blocks, new_nodes);
}

void ControlFlowGraph::split_jsr_exception_ranges(const std::vector<BasicBlock*>& common_blocks, const std::map<int, BasicBlock*>& new_nodes) {
    for (std::size_t i = exceptions_.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *exceptions_[i];
        std::vector<BasicBlock*> both = common_blocks;
        cf_retain(both, range.protected_range);
        if (both.empty()) continue;
        std::vector<BasicBlock*>* new_range;
        if (both.size() == range.protected_range.size()) {
            exceptions_.push_back(std::make_unique<ExceptionRangeCFG>(std::vector<BasicBlock*>{}, new_nodes.at(range.handler->id), range.exception_types));
            new_range = &exceptions_.back()->protected_range;
        } else {
            new_range = &exceptions_[i]->protected_range;
        }
        for (BasicBlock* block : both) new_range->push_back(new_nodes.at(block->id));
    }
}

namespace {

void cf_remove_jsr_instructions(const ConstantPool& pool, BasicBlock* block, DataPoint data) {
    InstructionSequence& seq = *block->seq;
    for (std::size_t i = 0; i < seq.length(); ++i) {
        const Instruction& instr = *seq.instr(i);
        std::optional<VarType> var;
        if (instr.opcode == cc::opc_astore || instr.opcode == cc::opc_pop) var = data.stack.get_by_offset(-1);
        step_types(data, instr, pool);
        switch (instr.opcode) {
            case cc::opc_jsr:
            case cc::opc_ret:
                seq.remove_instruction(i);
                --i;
                break;
            case cc::opc_astore:
            case cc::opc_pop:
                if (var->type == cc::TYPE_ADDRESS) {
                    seq.remove_instruction(i);
                    --i;
                }
                break;
            default: break;
        }
    }
    block->mark = 1;
    for (std::size_t i = 0; i < block->succs.size(); ++i) {
        BasicBlock* suc = block->succs[i];
        if (suc->mark != 1) cf_remove_jsr_instructions(pool, suc, data.copy());
    }
    for (std::size_t i = 0; i < block->succ_exceptions.size(); ++i) {
        BasicBlock* suc = block->succ_exceptions[i];
        if (suc->mark != 1) {
            DataPoint point;
            point.local_variables = data.local_variables;
            point.stack.push(VarType(cc::TYPE_OBJECT, 0, std::nullopt));
            cf_remove_jsr_instructions(pool, suc, point);
        }
    }
}

}  // namespace

void ControlFlowGraph::remove_jsr(const StructMethod& method) { cf_remove_jsr_instructions(method.class_struct().pool(), first_, DataPoint::initial(method)); }

void ControlFlowGraph::set_first_and_last_blocks() {
    first_ = blocks_.get(0);
    last_ = make_block();
    last_->id = ++last_id;
    for (BasicBlock* block : blocks_)
        if (block->succs.empty()) last_->add_predecessor(block);
}

std::vector<BasicBlock*> ControlFlowGraph::reverse_post_order() const {
    std::deque<BasicBlock*> res;
    std::vector<BasicBlock*> stack_node{first_};
    std::vector<std::size_t> stack_index{0};
    std::vector<BasicBlock*> visited;
    while (!stack_node.empty()) {
        BasicBlock* node = stack_node.back();
        std::size_t index = stack_index.back();
        stack_index.pop_back();
        cf_add_unique(visited, node);
        std::vector<BasicBlock*> succs = node->succs;
        succs.insert(succs.end(), node->succ_exceptions.begin(), node->succ_exceptions.end());
        for (; index < succs.size(); ++index) {
            BasicBlock* succ = succs[index];
            if (!cf_contains(visited, succ)) {
                stack_index.push_back(index + 1);
                stack_node.push_back(succ);
                stack_index.push_back(0);
                break;
            }
        }
        if (index == succs.size()) {
            res.push_front(node);
            stack_node.pop_back();
        }
    }
    return {res.begin(), res.end()};
}

// ---- DeadCodeHelper

namespace dead_code {

void remove_dead_blocks(ControlFlowGraph& graph) {
    std::deque<BasicBlock*> stack{graph.first()};
    std::vector<BasicBlock*> stacked{graph.first()};
    while (!stack.empty()) {
        BasicBlock* block = stack.front();
        stack.pop_front();
        std::vector<BasicBlock*> succs = block->succs;
        succs.insert(succs.end(), block->succ_exceptions.begin(), block->succ_exceptions.end());
        for (BasicBlock* succ : succs)
            if (!cf_contains(stacked, succ)) {
                stack.push_back(succ);
                stacked.push_back(succ);
            }
    }
    std::vector<BasicBlock*> all = cf_set_of(graph.blocks().items());
    cf_remove_each(all, stacked);
    for (BasicBlock* block : all) graph.remove_block(block);
}

namespace {

bool dc_remove_empty_block(ControlFlowGraph& graph, BasicBlock* block, bool merging) {
    bool deleted_ranges = false;
    if (!block->seq->empty()) return deleted_ranges;
    if (block->succs.size() > 1) {
        if (block->preds.size() > 1) throw std::runtime_error("ERROR: empty block with multiple predecessors and successors found");
        if (!merging) throw std::runtime_error("ERROR: empty block with multiple successors found");
    }
    const std::vector<BasicBlock*> exits = cf_set_of(graph.last()->preds);
    if (!block->pred_exceptions.empty() || (cf_contains(exits, block) && block->preds.size() != 1)) return deleted_ranges;
    if (cf_contains(exits, block)) {
        BasicBlock* pred = block->preds.at(0);
        if (pred->succs.size() != 1 || (!pred->seq->empty() && pred->seq->last_instr()->group == cc::GROUP_SWITCH)) return false;
    }
    const std::vector<BasicBlock*> preds = cf_set_of(block->preds);
    const std::vector<BasicBlock*> succs = cf_set_of(block->succs);
    // the exception handlers the predecessors and successors all have: the block must have each
    std::optional<std::vector<BasicBlock*>> common;
    for (int i = 0; i < 2; ++i)
        for (BasicBlock* p : i == 0 ? preds : succs) {
            if (!common) common = cf_set_of(p->succ_exceptions);
            else cf_retain(*common, p->succ_exceptions);
        }
    if (common && !common->empty())
        for (BasicBlock* handler : *common)
            if (!cf_contains(block->succ_exceptions, handler)) return false;
    // the ranges of this one block go
    auto& ranges = graph.exceptions();
    for (std::size_t i = ranges.size(); i-- > 0;) {
        ExceptionRangeCFG& range = *ranges[i];
        if (range.protected_range.size() == 1 && range.protected_range[0] == block) {
            if (DecompilerContext::option(pref::REMOVE_EMPTY_RANGES)) {
                block->remove_successor_exception(range.handler);
                ranges.erase(ranges.begin() + static_cast<std::ptrdiff_t>(i));
                deleted_ranges = true;
            } else {
                return false;
            }
        }
    }
    // the rest connected around it
    if (merging) {
        BasicBlock* pred = block->preds.at(0);
        pred->remove_successor(block);
        const std::vector<BasicBlock*> block_succs = block->succs;
        for (BasicBlock* succ : block_succs) {
            block->remove_successor(succ);
            pred->add_successor(succ);
        }
    } else {
        for (BasicBlock* pred : preds)
            for (BasicBlock* succ : succs) pred->replace_successor(block, succ);
    }
    std::vector<BasicBlock*>& finally_exits = graph.finally_exits();
    if (cf_contains(finally_exits, block)) {
        cf_remove_first(finally_exits, block);
        cf_add_unique(finally_exits, preds.at(0));
    }
    if (graph.first() == block) {
        if (succs.size() != 1) throw std::runtime_error("multiple or no entry blocks!");
        graph.set_first(succs[0]);
    }
    graph.remove_block(block);
    if (deleted_ranges) remove_dead_blocks(graph);
    return deleted_ranges;
}

}  // namespace

void remove_empty_blocks(ControlFlowGraph& graph) {
    bool cont;
    do {
        cont = false;
        for (std::size_t i = graph.blocks().size(); i-- > 0;)
            if (dc_remove_empty_block(graph, graph.blocks()[i], false)) {
                cont = true;
                break;
            }
    } while (cont);
}

bool is_dominator(const ControlFlowGraph& graph, BasicBlock* block, BasicBlock* dom) {
    if (block == dom) return true;
    std::vector<BasicBlock*> marked;
    std::deque<BasicBlock*> nodes{block};
    while (!nodes.empty()) {
        BasicBlock* node = nodes.front();
        nodes.pop_front();
        if (cf_contains(marked, node)) continue;
        marked.push_back(node);
        if (node == graph.first()) return false;
        for (BasicBlock* p : node->preds)
            if (!cf_contains(marked, p) && p != dom) nodes.push_back(p);
        for (BasicBlock* p : node->pred_exceptions)
            if (!cf_contains(marked, p) && p != dom) nodes.push_back(p);
    }
    return true;
}

void remove_gotos(ControlFlowGraph& graph) {
    for (BasicBlock* block : graph.blocks()) {
        const std::shared_ptr<Instruction> instr = block->last_instruction();
        if (instr && instr->opcode == cc::opc_goto) block->seq->remove_instruction(block->seq->length() - 1);
    }
    remove_empty_blocks(graph);
}

void connect_dummy_exit_block(ControlFlowGraph& graph) {
    BasicBlock* exit = graph.last();
    for (BasicBlock* block : cf_set_of(exit->preds)) {
        exit->remove_predecessor(block);
        block->add_successor(exit);
    }
}

void incorporate_value_returns(ControlFlowGraph& graph) {
    for (BasicBlock* block : graph.blocks()) {
        InstructionSequence& seq = *block->seq;
        const std::size_t len = seq.length();
        if (len == 0 || len >= 3) continue;
        bool ok = false;
        const int last = seq.last_instr()->opcode;
        if (last >= cc::opc_ireturn && last <= cc::opc_return) {
            if (len == 1) {
                ok = true;
            } else if (last != cc::opc_return) {
                switch (seq.instr(0)->opcode) {
                    case cc::opc_iload:
                    case cc::opc_lload:
                    case cc::opc_fload:
                    case cc::opc_dload:
                    case cc::opc_aload:
                    case cc::opc_aconst_null:
                    case cc::opc_bipush:
                    case cc::opc_sipush:
                    case cc::opc_lconst_0:
                    case cc::opc_lconst_1:
                    case cc::opc_fconst_0:
                    case cc::opc_fconst_1:
                    case cc::opc_fconst_2:
                    case cc::opc_dconst_0:
                    case cc::opc_dconst_1:
                    case cc::opc_ldc:
                    case cc::opc_ldc_w:
                    case cc::opc_ldc2_w: ok = true; break;
                    default: break;
                }
            }
        }
        if (!ok) continue;
        if (!block->preds.empty()) {
            std::vector<BasicBlock*> uni, inter;
            bool first_pred = true;
            for (BasicBlock* pred : block->preds) {
                if (first_pred) {
                    for (BasicBlock* h : pred->succ_exceptions) cf_add_unique(inter, h);
                    first_pred = false;
                } else {
                    cf_retain(inter, pred->succ_exceptions);
                }
                for (BasicBlock* h : pred->succ_exceptions) cf_add_unique(uni, h);
            }
            cf_remove_each(inter, block->succ_exceptions);
            BasicBlock* predecessor = block->preds.at(0);
            for (BasicBlock* handler : inter) {
                graph.exception_range(handler, predecessor)->protected_range.push_back(block);
                block->add_successor_exception(handler);
            }
            std::vector<BasicBlock*> to_remove = cf_set_of(block->succ_exceptions);
            cf_remove_each(to_remove, uni);
            for (BasicBlock* handler : to_remove) {
                ExceptionRangeCFG* range = graph.exception_range(handler, block);
                if (range->protected_range.size() > 1) {
                    cf_remove_first(range->protected_range, block);
                    block->remove_successor_exception(handler);
                }
            }
        }
        if (block->preds.size() == 1 && block->pred_exceptions.empty()) {
            BasicBlock* bpred = block->preds[0];
            if (bpred->succs.size() == 1) {
                for (BasicBlock* succ : bpred->succ_exceptions)
                    if (!cf_contains(block->succ_exceptions, succ)) {
                        graph.exception_range(succ, bpred)->protected_range.push_back(block);
                        block->add_successor_exception(succ);
                    }
                for (BasicBlock* succ : cf_set_of(block->succ_exceptions))
                    if (!cf_contains(bpred->succ_exceptions, succ)) {
                        ExceptionRangeCFG* range = graph.exception_range(succ, block);
                        if (range->protected_range.size() > 1) {
                            cf_remove_first(range->protected_range, block);
                            block->remove_successor_exception(succ);
                        }
                    }
            }
        }
    }
}

void merge_basic_blocks(ControlFlowGraph& graph) {
    for (;;) {
        bool merged = false;
        for (BasicBlock* block : graph.blocks()) {
            InstructionSequence& seq = *block->seq;
            if (block->succs.size() != 1) continue;
            BasicBlock* next = block->succs[0];
            if (next == graph.last() || (!seq.empty() && seq.last_instr()->group == cc::GROUP_SWITCH)) continue;
            if (next->preds.size() != 1 || !next->pred_exceptions.empty() || next == graph.first()) continue;
            bool same_ranges = true;
            for (const auto& range : graph.exceptions())
                if (cf_contains(range->protected_range, block) != cf_contains(range->protected_range, next)) {
                    same_ranges = false;
                    break;
                }
            if (!same_ranges) continue;
            seq.add_sequence(*next->seq);
            block->instr_old_offsets.insert(block->instr_old_offsets.end(), next->instr_old_offsets.begin(), next->instr_old_offsets.end());
            next->seq->clear();
            dc_remove_empty_block(graph, next, true);
            merged = true;
            break;
        }
        if (!merged) break;
    }
}

}  // namespace dead_code

std::unique_ptr<ControlFlowGraph> build_method_graph(const StructMethod& method) {
    const std::shared_ptr<InstructionSequence> seq = parse_bytecode(method);
    auto graph = std::make_unique<ControlFlowGraph>(*seq);
    dead_code::remove_dead_blocks(*graph);
    graph->inline_jsr(method);
    dead_code::connect_dummy_exit_block(*graph);
    dead_code::remove_gotos(*graph);
    exception_deobfuscator::remove_circular_ranges(*graph);
    exception_deobfuscator::restore_pop_ranges(*graph);
    if (DecompilerContext::option(pref::REMOVE_EMPTY_RANGES)) exception_deobfuscator::remove_empty_ranges(*graph);
    if (DecompilerContext::option(pref::NO_EXCEPTIONS_RETURN)) dead_code::incorporate_value_returns(*graph);
    exception_deobfuscator::insert_empty_exception_handler_blocks(*graph);
    dead_code::merge_basic_blocks(*graph);
    return graph;
}

}  // namespace cb::java::decompiler
