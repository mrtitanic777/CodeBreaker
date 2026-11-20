// java/decompiler/finally_processor.hpp -- try/finally recovery: a compiler copies the finally block's code to every exit of the try and catch blocks and
// to a catch-all handler; this finds the catch-all statements whose handler is a finally, checks that every inlined copy equals the handler's code,
// removes the copies (the graph is cut and the statement tree rebuilt after it, hence "iterate"), or, where they cannot all be shown equal, puts a
// semaphore variable in the graph that says which way the finally was entered.
//
//     FinallyProcessor processor(var_processor);
//     while (processor.iterate_graph(method, root, graph)) { /* the graph changed: build the statements again */ }
//
// Ported from Fernflower (de.fernflower.modules.decompiler.FinallyProcessor), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder). The identity-hashed sets and maps of blocks are insertion-ordered here (LinkedSet, LinkedMap).
//
// Calls into other areas (class::method): ExprProcessor::process_statement(RootStatement*, const StructClass*) (an instance method, as the original's),
// SSAConstructorSparseEx::split_variables(RootStatement*, const StructMethod*), FlattenStatementsHelper::build_direct_graph(RootStatement*) -> DirectGraph*
// (DirectGraph::first, DirectNode::succs/preds/block_statement/exprents ...), the VarVersionPair of a VarExprent.
#pragma once

#include <array>
#include <optional>
#include <unordered_map>
#include <vector>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/instructions.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

class StructMethod;
class VarProcessor;

class FinallyProcessor {
public:
    explicit FinallyProcessor(VarProcessor* varprocessor) : varprocessor_(varprocessor) {}

    // One finally found and handled (true: the graph changed, so the caller builds the statements again), or none left (false).
    bool iterate_graph(const StructMethod* mt, RootStatement* root, ControlFlowGraph& graph);
    // Whether two instructions are the same, a store to a local counting as the same where the pair of locals was met before (lst_store_vars: {catch's, sample's}).
    bool equal_instructions(const Instruction& first, const Instruction& second, const std::vector<std::array<int, 2>>& lst_store_vars);

private:
    // What a finally's handler looks like: how it begins (0: nothing special, 1: a pop, 2: an astore, 3: an empty finally) and which of its blocks end
    // the finally, mapped to whether the end is a true exit (the original's Record).
    struct Record {
        int first_code;
        LinkedMap<BasicBlock*, bool> map_last;
    };
    // A copy of the finally found in the try/catch: where it starts, its blocks and the block it continues in (the original's Area).
    struct Area {
        BasicBlock* start;
        LinkedSet<BasicBlock*> sample;
        BasicBlock* next;
    };
    // {the sample's block, its successor, that successor when it is a true last block (else null)}: the original's BasicBlock[3].
    using NextEntry = std::array<BasicBlock*, 3>;
    // The original's local BlockStackEntry of compareSubgraphsEx.
    struct BlockStackEntry {
        BasicBlock* block_catch;
        BasicBlock* block_sample;
        std::vector<std::array<int, 2>> lst_store_vars;
    };

    bool process_statement_ex(const StructMethod* mt, RootStatement* root, ControlFlowGraph& graph);
    static std::optional<Record> get_finally_information(const StructMethod* mt, RootStatement* root, CatchAllStatement* fstat);   // none: inconsistent finally
    static void insert_semaphore(ControlFlowGraph& graph, const LinkedSet<BasicBlock*>& set_try, BasicBlock* head, BasicBlock* handler, int var,
                                 const Record& information, int bytecode_version);
    static void insert_block_before(ControlFlowGraph& graph, BasicBlock* oldblock, BasicBlock* newblock);
    static LinkedSet<BasicBlock*> get_all_basic_blocks(Statement* stat);
    bool verify_finally_ex(ControlFlowGraph& graph, CatchAllStatement* fstat, const Record& information);
    std::optional<Area> compare_subgraphs_ex(ControlFlowGraph& graph, BasicBlock* start_sample, const LinkedSet<BasicBlock*>& catch_blocks,
                                             BasicBlock* start_catch, int finallytype, const LinkedMap<BasicBlock*, bool>& map_last, bool skipped_first);
    static BasicBlock* get_unique_next(ControlFlowGraph& graph, const std::vector<NextEntry>& set_next);   // (the original's identity HashSet<BasicBlock[]>: each once, in the order of the map's values)
    bool compare_basic_blocks_ex(ControlFlowGraph& graph, BasicBlock* pattern, BasicBlock* sample, int type, int finallytype,
                                 std::vector<std::array<int, 2>>& lst_store_vars);
    static void delete_area(ControlFlowGraph& graph, const Area& area);
    static void remove_exception_instructions_ex(BasicBlock* block, int blocktype, int finallytype);

    std::unordered_map<int, std::optional<int>> finally_block_ids_;   // handler block id -> the semaphore variable (none: a finally removed outright)
    std::unordered_map<int, std::optional<int>> catchall_block_ids_;  // handler block id -> none (a catch-all that is no finally)
    VarProcessor* varprocessor_;
};

}  // namespace cb::java::decompiler
