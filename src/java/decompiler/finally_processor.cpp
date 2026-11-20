// finally_processor.cpp -- FinallyProcessor. Ported from Fernflower (de.fernflower.modules.decompiler.FinallyProcessor), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <stdexcept>
#include <unordered_set>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/finally_processor.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {

// The original's HashSet<BasicBlock> finallyExits: a vector here, kept without duplicates.
void add_finally_exit(ControlFlowGraph& graph, BasicBlock* block) {
    std::vector<BasicBlock*>& exits = graph.finally_exits();
    if (std::find(exits.begin(), exits.end(), block) == exits.end()) exits.push_back(block);
}

bool has_finally_exit(ControlFlowGraph& graph, BasicBlock* block) {
    const std::vector<BasicBlock*>& exits = graph.finally_exits();
    return std::find(exits.begin(), exits.end(), block) != exits.end();
}

void remove_finally_exit(ControlFlowGraph& graph, BasicBlock* block) {
    std::vector<BasicBlock*>& exits = graph.finally_exits();
    exits.erase(std::remove(exits.begin(), exits.end(), block), exits.end());
}

bool list_contains(const std::vector<BasicBlock*>& lst, const BasicBlock* block) { return std::find(lst.begin(), lst.end(), block) != lst.end(); }

// The original's InterpreterUtil.equalSets over two lists.
bool equal_sets(const std::vector<BasicBlock*>& c1, const std::vector<BasicBlock*>& c2) {
    if (c1.size() != c2.size()) return false;

    for (BasicBlock* b : c1) {
        if (!list_contains(c2, b)) return false;
    }
    return true;
}

std::shared_ptr<InstructionSequence> store_sequence(int value, int var, int bytecode_version) {
    auto seq = std::make_shared<InstructionSequence>();
    seq->add_instruction(Instruction::make(cc::opc_bipush, false, cc::GROUP_GENERAL, bytecode_version, {value}), -1);
    seq->add_instruction(Instruction::make(cc::opc_istore, false, cc::GROUP_GENERAL, bytecode_version, {var}), -1);
    return seq;
}

}  // namespace

bool FinallyProcessor::iterate_graph(const StructMethod* mt, RootStatement* root, ControlFlowGraph& graph) {
    return process_statement_ex(mt, root, graph);
}

bool FinallyProcessor::process_statement_ex(const StructMethod* mt, RootStatement* root, ControlFlowGraph& graph) {
    const int bytecode_version = mt->class_struct().bytecode_version();

    std::vector<Statement*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Statement* stat = stack.back();
        stack.pop_back();

        Statement* parent = stat->get_parent();
        if (parent != nullptr && parent->type == Statement::TYPE_CATCHALL && stat == parent->get_first() && !parent->is_copied()) {
            CatchAllStatement* fin = static_cast<CatchAllStatement*>(parent);
            BasicBlock* head = fin->get_basichead()->get_block();
            BasicBlock* handler = fin->get_handler()->get_basichead()->get_block();

            if (catchall_block_ids_.contains(handler->id)) {
                // do nothing
            } else if (finally_block_ids_.contains(handler->id)) {
                fin->set_finally(true);

                const std::optional<int> var = finally_block_ids_.at(handler->id);
                fin->set_monitor(!var ? nullptr : make<VarExprent>(*var, VarType::INT, varprocessor_));
            } else {
                std::optional<Record> inf = get_finally_information(mt, root, fin);

                if (!inf) {   // inconsistent finally
                    catchall_block_ids_[handler->id] = std::nullopt;
                } else {
                    if (DecompilerContext::option(pref::FINALLY_DEINLINE) && verify_finally_ex(graph, fin, *inf)) {
                        finally_block_ids_[handler->id] = std::nullopt;
                    } else {
                        const int varindex = DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::VAR_COUNTER);
                        insert_semaphore(graph, get_all_basic_blocks(fin->get_first()), head, handler, varindex, *inf, bytecode_version);

                        finally_block_ids_[handler->id] = varindex;
                    }

                    dead_code::remove_dead_blocks(graph);   // e.g. multiple return blocks after a nested finally
                    dead_code::remove_empty_blocks(graph);
                    dead_code::merge_basic_blocks(graph);
                }

                return true;
            }
        }

        for (Statement* st : stat->get_stats()) stack.push_back(st);
    }

    return false;
}

std::optional<FinallyProcessor::Record> FinallyProcessor::get_finally_information(const StructMethod* mt, RootStatement* root, CatchAllStatement* fstat) {
    LinkedMap<BasicBlock*, bool> map_last;

    BasicBlockStatement* first_block_statement = fstat->get_handler()->get_basichead();
    BasicBlock* first_basic_block = first_block_statement->get_block();
    const std::shared_ptr<Instruction> instr_first = first_basic_block->instruction(0);

    int firstcode = 0;

    switch (instr_first->opcode) {
        case cc::opc_pop:
            firstcode = 1;
            break;
        case cc::opc_astore:
            firstcode = 2;
            break;
        default:
            break;
    }

    ExprProcessor proc;
    proc.process_statement(root, &mt->class_struct());

    SSAConstructorSparseEx ssa;
    ssa.split_variables(root, mt);

    std::vector<Exprent*>& lst_exprents = first_block_statement->get_exprents().value();

    const VarVersionPair varpaar(static_cast<VarExprent*>(static_cast<AssignmentExprent*>(lst_exprents.at(firstcode == 2 ? 1 : 0))->get_left()));

    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    // find finally exits
    std::deque<DirectNode*> stack;
    stack.push_back(dgraph->first);

    LinkedSet<DirectNode*> set_visited;

    while (!stack.empty()) {
        DirectNode* node = stack.front();
        stack.pop_front();

        if (set_visited.contains(node)) continue;

        set_visited.add(node);

        BasicBlockStatement* block_statement = nullptr;
        if (node->block != nullptr) {
            block_statement = node->block;
        } else if (node->preds.size() == 1) {
            block_statement = node->preds.at(0)->block;
        }

        bool is_true_exit = true;

        if (firstcode != 1) {
            is_true_exit = false;

            for (std::size_t i = 0; i < node->exprents->size(); i++) {
                Exprent* exprent = (*node->exprents)[i];

                if (firstcode == 0) {
                    std::vector<Exprent*> lst = exprent->get_all_exprents();
                    lst.push_back(exprent);

                    bool found = false;

                    for (Exprent* expr : lst) {
                        if (expr->type == Exprent::EXPRENT_VAR && VarVersionPair(static_cast<VarExprent*>(expr)) == varpaar) {
                            found = true;
                            break;
                        }
                    }

                    if (found) {
                        found = false;

                        if (exprent->type == Exprent::EXPRENT_EXIT) {
                            ExitExprent* exexpr = static_cast<ExitExprent*>(exprent);
                            if (exexpr->get_exit_type() == ExitExprent::EXIT_THROW && exexpr->get_value()->type == Exprent::EXPRENT_VAR) {
                                found = true;
                            }
                        }

                        if (!found) {
                            return std::nullopt;
                        } else {
                            is_true_exit = true;
                        }
                    }
                } else if (firstcode == 2) {
                    // search for a load instruction
                    if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                        AssignmentExprent* assexpr = static_cast<AssignmentExprent*>(exprent);
                        if (assexpr->get_right()->type == Exprent::EXPRENT_VAR && VarVersionPair(static_cast<VarExprent*>(assexpr->get_right())) == varpaar) {
                            Exprent* next = nullptr;
                            if (i == node->exprents->size() - 1) {
                                if (node->succs.size() == 1) {
                                    DirectNode* nd = node->succs.at(0);
                                    if (!nd->exprents->empty()) next = (*nd->exprents)[0];
                                }
                            } else {
                                next = (*node->exprents)[i + 1];
                            }

                            bool found = false;

                            if (next != nullptr && next->type == Exprent::EXPRENT_EXIT) {
                                ExitExprent* exexpr = static_cast<ExitExprent*>(next);
                                if (exexpr->get_exit_type() == ExitExprent::EXIT_THROW && exexpr->get_value()->type == Exprent::EXPRENT_VAR &&
                                    assexpr->get_left()->equals(exexpr->get_value())) {
                                    found = true;
                                }
                            }

                            if (!found) {
                                return std::nullopt;
                            } else {
                                is_true_exit = true;
                            }
                        }
                    }
                }
            }
        }

        // find finally exits
        if (block_statement != nullptr && block_statement->get_block() != nullptr) {
            Statement* handler = fstat->get_handler();

            for (StatEdge* edge : block_statement->get_successor_edges(Statement::STATEDGE_DIRECT_ALL)) {
                if (edge->get_type() != StatEdge::TYPE_REGULAR && handler->contains_statement(block_statement) &&
                    !handler->contains_statement(edge->get_destination())) {
                    const bool* existing_flag = map_last.get(block_statement->get_block());
                    if (existing_flag == nullptr || !*existing_flag) {
                        map_last.put(block_statement->get_block(), is_true_exit);
                        break;
                    }
                }
            }
        }

        // continue
        for (DirectNode* succ : node->succs) stack.push_back(succ);
    }

    // empty finally handler
    if (fstat->get_handler()->type == Statement::TYPE_BASICBLOCK) {
        bool is_empty = false;
        const bool is_first_last = map_last.contains_key(first_basic_block);
        const std::shared_ptr<InstructionSequence>& seq = first_basic_block->seq;

        switch (firstcode) {
            case 0:
                is_empty = is_first_last && seq->length() == 1;
                break;
            case 1:
                is_empty = seq->length() == 1;
                break;
            case 2:
                is_empty = is_first_last ? seq->length() == 3 : seq->length() == 1;
                break;
            default:
                break;
        }

        if (is_empty) firstcode = 3;
    }

    return Record{firstcode, map_last};
}

void FinallyProcessor::insert_semaphore(ControlFlowGraph& graph, const LinkedSet<BasicBlock*>& set_try, BasicBlock* head, BasicBlock* handler, int var,
                                        const Record& information, int bytecode_version) {
    LinkedSet<BasicBlock*> set_copy = set_try;

    const int finallytype = information.first_code;
    const LinkedMap<BasicBlock*, bool>& map_last = information.map_last;

    // first and last statements
    remove_exception_instructions_ex(handler, 1, finallytype);
    for (const auto& [last, is_true] : map_last) {
        if (is_true) {
            remove_exception_instructions_ex(last, 2, finallytype);
            add_finally_exit(graph, last);
        }
    }

    // disable semaphore at statement exit points
    for (BasicBlock* block : set_try) {
        const std::vector<BasicBlock*> lst_succ = block->succs;

        for (BasicBlock* dest : lst_succ) {
            // last block of the protected range
            if (!set_copy.contains(dest) && dest != graph.last()) {
                BasicBlock* newblock = graph.make_block();
                newblock->id = ++graph.last_id;
                newblock->seq = store_sequence(0, var, bytecode_version);

                block->replace_successor(dest, newblock);
                newblock->add_successor(dest);
                set_copy.add(newblock);

                graph.blocks().add_with_key(newblock, newblock->id);

                // copy exception edges and extend protected ranges
                for (std::size_t j = 0; j < block->succ_exceptions.size(); j++) {
                    BasicBlock* hd = block->succ_exceptions[j];
                    newblock->add_successor_exception(hd);

                    ExceptionRangeCFG* range = graph.exception_range(hd, block);
                    range->protected_range.push_back(newblock);
                }
            }
        }
    }

    // enable semaphore at the statement entrance
    BasicBlock* newhead = graph.make_block();
    newhead->id = ++graph.last_id;
    newhead->seq = store_sequence(1, var, bytecode_version);

    insert_block_before(graph, head, newhead);

    // initialize semaphor with false
    BasicBlock* newheadinit = graph.make_block();
    newheadinit->id = ++graph.last_id;
    newheadinit->seq = store_sequence(0, var, bytecode_version);

    insert_block_before(graph, newhead, newheadinit);

    set_copy.add(newhead);
    set_copy.add(newheadinit);

    for (BasicBlock* hd : LinkedSet<BasicBlock*>::of(newheadinit->succ_exceptions).items()) {
        ExceptionRangeCFG* range = graph.exception_range(hd, newheadinit);

        bool contains_all = true;
        for (BasicBlock* b : range->protected_range) {
            if (!set_copy.contains(b)) {
                contains_all = false;
                break;
            }
        }

        if (contains_all) {
            newheadinit->remove_successor_exception(hd);
            range->protected_range.erase(std::remove(range->protected_range.begin(), range->protected_range.end(), newheadinit), range->protected_range.end());
        }
    }
}

void FinallyProcessor::insert_block_before(ControlFlowGraph& graph, BasicBlock* oldblock, BasicBlock* newblock) {
    std::vector<BasicBlock*> lst_temp;

    lst_temp.insert(lst_temp.end(), oldblock->preds.begin(), oldblock->preds.end());
    lst_temp.insert(lst_temp.end(), oldblock->pred_exceptions.begin(), oldblock->pred_exceptions.end());

    // replace predecessors
    for (BasicBlock* pred : lst_temp) pred->replace_successor(oldblock, newblock);

    // copy exception edges and extend protected ranges
    for (BasicBlock* hd : oldblock->succ_exceptions) {
        newblock->add_successor_exception(hd);

        ExceptionRangeCFG* range = graph.exception_range(hd, oldblock);
        range->protected_range.push_back(newblock);
    }

    // replace handler
    for (const std::unique_ptr<ExceptionRangeCFG>& range : graph.exceptions()) {
        if (range->handler == oldblock) range->handler = newblock;
    }

    newblock->add_successor(oldblock);
    graph.blocks().add_with_key(newblock, newblock->id);

    if (graph.first() == oldblock) graph.set_first(newblock);
}

LinkedSet<BasicBlock*> FinallyProcessor::get_all_basic_blocks(Statement* stat) {
    std::vector<Statement*> lst;
    lst.push_back(stat);

    std::size_t index = 0;
    do {
        Statement* st = lst.at(index);

        if (st->type == Statement::TYPE_BASICBLOCK) {
            index++;
        } else {
            for (Statement* child : st->get_stats()) lst.push_back(child);
            lst.erase(lst.begin() + static_cast<std::ptrdiff_t>(index));
        }
    } while (index < lst.size());

    LinkedSet<BasicBlock*> res;

    for (Statement* st : lst) res.add(static_cast<BasicBlockStatement*>(st)->get_block());

    return res;
}

bool FinallyProcessor::verify_finally_ex(ControlFlowGraph& graph, CatchAllStatement* fstat, const Record& information) {
    const LinkedSet<BasicBlock*> try_blocks = get_all_basic_blocks(fstat->get_first());
    const LinkedSet<BasicBlock*> catch_blocks = get_all_basic_blocks(fstat->get_handler());

    const int finallytype = information.first_code;
    const LinkedMap<BasicBlock*, bool>& map_last = information.map_last;

    BasicBlock* first = fstat->get_handler()->get_basichead()->get_block();
    bool skipped_first = false;

    if (finallytype == 3) {
        // empty finally
        remove_exception_instructions_ex(first, 3, finallytype);

        if (map_last.contains_key(first)) add_finally_exit(graph, first);

        return true;
    } else {
        if (first->seq->length() == 1 && finallytype > 0) {
            BasicBlock* firstsuc = first->succs.at(0);
            if (catch_blocks.contains(firstsuc)) {
                first = firstsuc;
                skipped_first = true;
            }
        }
    }

    // identify start blocks
    LinkedSet<BasicBlock*> start_blocks;
    for (BasicBlock* block : try_blocks) start_blocks.add_all(block->succs);
    // throw in the try body will point directly to the dummy exit
    // so remove dummy exit if there are other successors
    start_blocks.remove(graph.last());
    start_blocks.remove_all(try_blocks.items());

    std::vector<Area> lst_areas;

    for (BasicBlock* start : start_blocks) {
        std::optional<Area> arr = compare_subgraphs_ex(graph, start, catch_blocks, first, finallytype, map_last, skipped_first);

        if (!arr) return false;

        lst_areas.push_back(*arr);
    }

    // delete areas
    for (const Area& area : lst_areas) delete_area(graph, area);

    // INSTANCE: only when the finally block was fully verified
    for (const auto& [last, is_true] : map_last) {
        if (is_true) {
            remove_exception_instructions_ex(last, 2, finallytype);
            add_finally_exit(graph, last);
        }
    }

    remove_exception_instructions_ex(fstat->get_handler()->get_basichead()->get_block(), 1, finallytype);

    return true;
}

std::optional<FinallyProcessor::Area> FinallyProcessor::compare_subgraphs_ex(ControlFlowGraph& graph, BasicBlock* start_sample,
                                                                           const LinkedSet<BasicBlock*>& catch_blocks, BasicBlock* start_catch,
                                                                           int finallytype, const LinkedMap<BasicBlock*, bool>& map_last,
                                                                           bool skipped_first) {
    std::deque<BlockStackEntry> stack;

    LinkedSet<BasicBlock*> set_sample;

    JavaHashMap<std::string, NextEntry, JavaStringHash> map_next;

    stack.push_back(BlockStackEntry{start_catch, start_sample, {}});

    while (!stack.empty()) {
        const BlockStackEntry entry = stack.front();
        stack.pop_front();

        BasicBlock* block_catch = entry.block_catch;
        BasicBlock* block_sample = entry.block_sample;

        const bool is_first_block = !skipped_first && block_catch == start_catch;
        const bool is_last_block = map_last.contains_key(block_catch);
        const bool is_true_last_block = is_last_block && *map_last.get(block_catch);

        std::vector<std::array<int, 2>> store_vars = entry.lst_store_vars;
        if (!compare_basic_blocks_ex(graph, block_catch, block_sample, (is_first_block ? 1 : 0) | (is_true_last_block ? 2 : 0), finallytype, store_vars)) {
            return std::nullopt;
        }

        if (block_sample->succs.size() != block_catch->succs.size()) return std::nullopt;

        set_sample.add(block_sample);

        // direct successors
        for (std::size_t i = 0; i < block_catch->succs.size(); i++) {
            BasicBlock* suc_catch = block_catch->succs[i];
            BasicBlock* suc_sample = block_sample->succs.at(i);

            if (catch_blocks.contains(suc_catch) && !set_sample.contains(suc_sample)) {
                stack.push_back(BlockStackEntry{suc_catch, suc_sample, entry.lst_store_vars});
            }
        }

        // exception successors
        if (is_last_block && block_sample->seq->empty()) {
            // do nothing, blockSample will be removed anyway
        } else {
            if (block_catch->succ_exceptions.size() == block_sample->succ_exceptions.size()) {
                for (std::size_t i = 0; i < block_catch->succ_exceptions.size(); i++) {
                    BasicBlock* suc_catch = block_catch->succ_exceptions[i];
                    BasicBlock* suc_sample = block_sample->succ_exceptions[i];

                    const std::optional<std::string> exc_catch = graph.exception_range(suc_catch, block_catch)->unique_exceptions_string();
                    const std::optional<std::string> exc_sample = graph.exception_range(suc_sample, block_sample)->unique_exceptions_string();

                    const bool equalexc = exc_catch == exc_sample;

                    if (equalexc) {
                        if (catch_blocks.contains(suc_catch) && !set_sample.contains(suc_sample)) {
                            std::vector<std::array<int, 2>> lst = entry.lst_store_vars;

                            // exception variable assignment
                            if (suc_catch->seq->length() > 0 && suc_sample->seq->length() > 0) {
                                const std::shared_ptr<Instruction> instr_catch = suc_catch->seq->instr(0);
                                const std::shared_ptr<Instruction> instr_sample = suc_sample->seq->instr(0);

                                if (instr_catch->opcode == cc::opc_astore && instr_sample->opcode == cc::opc_astore) {
                                    lst.push_back({instr_catch->operand(0), instr_sample->operand(0)});
                                }
                            }

                            stack.push_back(BlockStackEntry{suc_catch, suc_sample, lst});
                        }
                    } else {
                        return std::nullopt;
                    }
                }
            } else {
                return std::nullopt;
            }
        }

        if (is_last_block) {
            LinkedSet<BasicBlock*> set_succs = LinkedSet<BasicBlock*>::of(block_sample->succs);
            set_succs.remove_all(set_sample.items());

            for (const BlockStackEntry& stackent : stack) set_succs.remove(stackent.block_sample);

            for (BasicBlock* succ : set_succs) {
                if (graph.last() != succ) {   // FIXME: why?
                    map_next.put(std::to_string(block_sample->id) + "#" + std::to_string(succ->id),
                                 NextEntry{block_sample, succ, is_true_last_block ? succ : nullptr});
                }
            }
        }
    }

    return Area{start_sample, set_sample, get_unique_next(graph, map_next.values())};
}

BasicBlock* FinallyProcessor::get_unique_next(ControlFlowGraph& graph, const std::vector<NextEntry>& set_next) {
    // precondition: there is at most one true exit path in a finally statement

    BasicBlock* next = nullptr;
    bool multiple = false;

    for (const NextEntry& arr : set_next) {
        if (arr[2] != nullptr) {
            next = arr[1];
            multiple = false;
            break;
        } else {
            if (next == nullptr) {
                next = arr[1];
            } else if (next != arr[1]) {
                multiple = true;
            }

            if (arr[1]->preds.size() == 1) next = arr[1];
        }
    }

    if (multiple) {   // TODO: generic solution
        // condition: next block is the same in all cases
        for (const NextEntry& arr : set_next) {
            BasicBlock* block = arr[1];

            if (block != next) {
                if (equal_sets(next->succs, block->succs)) {
                    const std::shared_ptr<InstructionSequence>& seq_next = next->seq;
                    const std::shared_ptr<InstructionSequence>& seq_block = block->seq;

                    if (seq_next->length() == seq_block->length()) {
                        for (std::size_t i = 0; i < seq_next->length(); i++) {
                            const std::shared_ptr<Instruction> instr_next = seq_next->instr(i);
                            const std::shared_ptr<Instruction> instr_block = seq_block->instr(i);

                            if (instr_next->opcode != instr_block->opcode || instr_next->wide != instr_block->wide ||
                                instr_next->operands_count() != instr_block->operands_count()) {
                                return nullptr;
                            }

                            for (int j = 0; j < instr_next->operands_count(); j++) {
                                if (instr_next->operand(j) != instr_block->operand(j)) return nullptr;
                            }
                        }
                    } else {
                        return nullptr;
                    }
                } else {
                    return nullptr;
                }
            }
        }

        for (const NextEntry& arr : set_next) {
            if (arr[1] != next) {
                arr[0]->remove_successor(arr[1]);
                arr[0]->add_successor(next);
            }
        }

        dead_code::remove_dead_blocks(graph);
    }

    return next;
}

bool FinallyProcessor::compare_basic_blocks_ex(ControlFlowGraph& graph, BasicBlock* pattern, BasicBlock* sample, int type, int finallytype,
                                               std::vector<std::array<int, 2>>& lst_store_vars) {
    std::shared_ptr<InstructionSequence> seq_pattern = pattern->seq;
    const std::shared_ptr<InstructionSequence> seq_sample = sample->seq;

    if (type != 0) {
        seq_pattern = seq_pattern->clone();

        if ((type & 1) > 0) {   // first
            if (finallytype > 0) seq_pattern->remove_instruction(0);
        }

        if ((type & 2) > 0) {   // last
            if (finallytype == 0 || finallytype == 2) seq_pattern->remove_instruction(seq_pattern->length() - 1);

            if (finallytype == 2) {
                // exception variable
                seq_pattern->remove_instruction(seq_pattern->length() - 1);
            }
        }
    }

    if (seq_pattern->length() > seq_sample->length()) return false;

    for (std::size_t i = 0; i < seq_pattern->length(); i++) {
        const std::shared_ptr<Instruction> instr_pattern = seq_pattern->instr(i);
        const std::shared_ptr<Instruction> instr_sample = seq_sample->instr(i);

        if (!equal_instructions(*instr_pattern, *instr_sample, lst_store_vars)) return false;
    }

    if (seq_pattern->length() < seq_sample->length()) {   // split in two blocks
        auto seq = std::make_shared<InstructionSequence>();
        for (int i = static_cast<int>(seq_sample->length()) - 1; i >= static_cast<int>(seq_pattern->length()); i--) {
            seq->add_instruction(0, seq_sample->instr(static_cast<std::size_t>(i)), -1);
            seq_sample->remove_instruction(static_cast<std::size_t>(i));
        }

        BasicBlock* newblock = graph.make_block();
        newblock->id = ++graph.last_id;
        newblock->seq = seq;

        const std::vector<BasicBlock*> lst_temp = sample->succs;

        for (BasicBlock* suc : lst_temp) {
            sample->remove_successor(suc);
            newblock->add_successor(suc);
        }

        sample->add_successor(newblock);

        graph.blocks().add_with_key(newblock, newblock->id);

        if (has_finally_exit(graph, sample)) {
            remove_finally_exit(graph, sample);
            add_finally_exit(graph, newblock);
        }

        // copy exception edges and extend protected ranges
        for (std::size_t j = 0; j < sample->succ_exceptions.size(); j++) {
            BasicBlock* hd = sample->succ_exceptions[j];
            newblock->add_successor_exception(hd);

            ExceptionRangeCFG* range = graph.exception_range(hd, sample);
            range->protected_range.push_back(newblock);
        }
    }

    return true;
}

bool FinallyProcessor::equal_instructions(const Instruction& first, const Instruction& second, const std::vector<std::array<int, 2>>& lst_store_vars) {
    if (first.opcode != second.opcode || first.wide != second.wide || first.operands_count() != second.operands_count()) return false;

    if (first.group != cc::GROUP_JUMP) {   // FIXME: switch comparison
        for (int i = 0; i < first.operands_count(); i++) {
            const int first_op = first.operand(i);
            const int second_op = second.operand(i);

            if (first_op != second_op) {
                // a-load/store instructions
                if (first.opcode == cc::opc_aload || first.opcode == cc::opc_astore) {
                    for (const std::array<int, 2>& arr : lst_store_vars) {
                        if (arr[0] == first_op && arr[1] == second_op) return true;
                    }
                }

                return false;
            }
        }
    }

    return true;
}

void FinallyProcessor::delete_area(ControlFlowGraph& graph, const Area& area) {
    BasicBlock* start = area.start;
    BasicBlock* next = area.next;

    if (start == next) {
        return;
    }

    if (next == nullptr) {
        // dummy exit block
        next = graph.last();
    }

    LinkedSet<BasicBlock*> set_common_exception_handlers = LinkedSet<BasicBlock*>::of(next->succ_exceptions);
    for (BasicBlock* pred : start->preds) {
        set_common_exception_handlers.retain_all(LinkedSet<BasicBlock*>::of(pred->succ_exceptions));
    }

    // Check if the area is within the exception range
    bool is_outside_range = false;

    const LinkedSet<BasicBlock*> set_predecessors = LinkedSet<BasicBlock*>::of(start->preds);
    for (BasicBlock* pred : set_predecessors.items()) {
        pred->replace_successor(start, next);
    }

    const LinkedSet<BasicBlock*>& set_blocks = area.sample;

    std::optional<LinkedSet<ExceptionRangeCFG*>> set_common_removed_exception_ranges;

    for (BasicBlock* block : set_blocks) {
        if (graph.blocks().contains_key(block->id)) {
            bool contains_all = true;
            for (BasicBlock* h : set_common_exception_handlers) {
                if (!list_contains(block->succ_exceptions, h)) {
                    contains_all = false;
                    break;
                }
            }
            if (!contains_all) is_outside_range = true;

            LinkedSet<ExceptionRangeCFG*> set_removed_exception_ranges;
            for (BasicBlock* handler : block->succ_exceptions) {
                set_removed_exception_ranges.add(graph.exception_range(handler, block));
            }

            if (!set_common_removed_exception_ranges) {
                set_common_removed_exception_ranges = set_removed_exception_ranges;
            } else {
                set_common_removed_exception_ranges->retain_all(set_removed_exception_ranges);
            }

            // shift successors
            if (block->seq->empty() && block->succs.size() == 1) {
                BasicBlock* succs = block->succs.at(0);

                for (BasicBlock* pred : std::vector<BasicBlock*>(block->preds)) {
                    if (!set_blocks.contains(pred)) pred->replace_successor(block, succs);
                }

                if (graph.first() == block) graph.set_first(succs);
            }

            graph.remove_block(block);
        }
    }

    if (is_outside_range) {
        // new empty block
        BasicBlock* emptyblock = graph.make_block();
        emptyblock->id = ++graph.last_id;
        emptyblock->seq = std::make_shared<InstructionSequence>();

        graph.blocks().add_with_key(emptyblock, emptyblock->id);

        // add to ranges if necessary
        if (set_common_removed_exception_ranges) {
            for (ExceptionRangeCFG* range : *set_common_removed_exception_ranges) {
                emptyblock->add_successor_exception(range->handler);
                range->protected_range.push_back(emptyblock);
            }
        }

        // insert between predecessors and next
        emptyblock->add_successor(next);
        for (BasicBlock* pred : set_predecessors) pred->replace_successor(next, emptyblock);
    }
}

void FinallyProcessor::remove_exception_instructions_ex(BasicBlock* block, int blocktype, int finallytype) {
    const std::shared_ptr<InstructionSequence>& seq = block->seq;

    if (finallytype == 3) {   // empty finally handler
        for (int i = static_cast<int>(seq->length()) - 1; i >= 0; i--) seq->remove_instruction(static_cast<std::size_t>(i));
    } else {
        if ((blocktype & 1) > 0) {   // first
            if (finallytype == 2 || finallytype == 1) {   // astore or pop
                seq->remove_instruction(0);
            }
        }

        if ((blocktype & 2) > 0) {   // last
            if (finallytype == 2 || finallytype == 0) {
                seq->remove_instruction(seq->length() - 1);
            }

            if (finallytype == 2) {   // astore
                seq->remove_instruction(seq->length() - 1);
            }
        }
    }
}

}  // namespace cb::java::decompiler
