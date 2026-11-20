// java/decompiler/helpers.hpp -- the passes that shape a method's statement tree after it is built, one class of static functions each: loops made into
// while/for/do-while (MergeHelper), ifs merged and reordered (IfHelper), labels and break/continue edges (LabelHelper, LowBreakHelper, ExitHelper),
// sequences flattened (SequenceHelper), single blocks inlined (InlineSingleBlockHelper), loops lifted or removed (LoopExtractHelper,
// EliminateLoopsHelper), and the expression-level rewrites: string concatenation (ConcatenationHelper), boolean/comparison forms
// (SecondaryFunctionsHelper), the stack-variable simplifications, array initializers and 1.4 class literals (SimplifyExprentsHelper), ++/-- (PPandMMHelper)
// and the IDEA not-null checks (IdeaNotNullHelper).
//
//     MergeHelper::enhance_loops(root);                       // a `for(;;)` whose body starts with `if (!c) break;` becomes `while (c)`
//     bool changed = IfHelper::merge_all_ifs(root);           // true: something was merged; the caller repeats until it is not
//
// Ported from Fernflower (de.fernflower.modules.decompiler: MergeHelper, IfHelper, LabelHelper, ExitHelper, SequenceHelper, LoopExtractHelper,
// InlineSingleBlockHelper, LowBreakHelper, ClearStructHelper, EliminateLoopsHelper, ConcatenationHelper, SecondaryFunctionsHelper,
// SimplifyExprentsHelper, PPandMMHelper, IdeaNotNullHelper, SwitchHelper), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder). The original's SwitchHelper is an empty class: it has no counterpart here. Statements, edges and expressions are arena objects (context.hpp):
// pointers, == is identity. The original's identity-hashed sets and maps are insertion-ordered (LinkedSet / LinkedMap); its HashSet<Integer> that is only
// looked up is a std::set.
#pragma once

#include <array>
#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/match.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

// Other layers' classes (the statements, expressions and SSA data the passes work on).
class Statement;
class StatEdge;
class RootStatement;
class SequenceStatement;
class IfStatement;
class DoStatement;
class Exprent;
class InvocationExprent;
class NewExprent;
class SSAConstructorSparseEx;
class BasicBlock;
class StructClass;
class StructMethod;

// The original's `new BasicBlock(id)`: a block the run keeps (it lives as long as the arena), for the empty statements the passes make.
BasicBlock* new_basic_block(int id);

// Resets what the statement building left on the statements (their temporary sets and edges) before the passes run.
class ClearStructHelper {
public:
    static void clear_statements(RootStatement* root);
};

// Do-statements into while, do-while and for loops; whether a path from a statement reaches another directly.
class MergeHelper {
public:
    static void enhance_loops(Statement* root);
    static bool is_direct_path(Statement* stat, Statement* endstat);

private:
    static bool enhance_loops_rec(Statement* stat);
    static bool enhance_loop(DoStatement* stat);
    static bool match_do_while(DoStatement* stat);
    static bool match_while(DoStatement* stat);
    static bool match_for(DoStatement* stat);
    static void remove_last_empty_statement(DoStatement* dostat, Statement* stat);
    static Statement* get_last_direct_data(Statement* stat);
};

// Merges nested ifs into && / || conditions, and else branches into else-ifs; reorders an if so its exits come out as the original's.
class IfHelper {
public:
    static bool merge_all_ifs(RootStatement* root);
    static bool merge_ifs(Statement* statement, std::set<int>& set_reordered_ifs);

private:
    // A node of the graph of an if's branches (the original's private IfNode): the statement, its successors, and the edge type to each.
    struct IfNode : ArenaObject {
        Statement* value;
        std::vector<IfNode*> succs;
        std::vector<int> edgetypes;

        explicit IfNode(Statement* value) : value(value) {}
        void add_child(IfNode* child, int type) {
            succs.push_back(child);
            edgetypes.push_back(type);
        }
    };

    static bool merge_all_ifs_rec(Statement* stat, std::set<int>& set_reordered_ifs);
    static bool collapse_if_if(IfNode* rtnode);
    static bool collapse_if_else(IfNode* rtnode);
    static bool collapse_else(IfNode* rtnode);
    static IfNode* build_graph(IfStatement* stat, bool stsingle);
    static bool reorder_if(IfStatement* ifstat);
    static bool has_direct_end_edge(Statement* stat, Statement* from);
    static Statement* get_next_statement(Statement* stat);
    static bool exists_path(Statement* from, Statement* to);
};

// Decides which edges need labels (and which statements get them), lifts closures and lowers continues.
class LabelHelper {
public:
    static void clean_up_edges(RootStatement* root);
    static void identify_labels(RootStatement* root);
    static void low_continue_labels(Statement* stat, LinkedSet<StatEdge*>& edges);
    static void low_closures(Statement* stat);
    static void replace_continue_with_break(Statement* stat);

private:
    static void lift_closures(Statement* stat);
    static void remove_non_immediate_edges(Statement* stat);
    static void reset_all_edges(Statement* stat);
    static void set_ret_edges_unlabeled(RootStatement* root);
    // Each statement and the edges that leave to it as its successor (the original's HashMap, insertion-ordered).
    using EdgeMap = LinkedMap<Statement*, std::vector<StatEdge*>>;
    static EdgeMap set_explicit_edges(Statement* stat);
    static void process_edges_with_next(Statement* stat, EdgeMap& map_edges, Statement* next);
    static void hide_default_switch_edges(Statement* stat);
    // The break targets and the continue targets in the statement: [0] and [1].
    static std::array<LinkedSet<Statement*>, 2> process_statement_label(Statement* stat);
    static Statement* get_min_continue_closure(StatEdge* edge);
};

// Break edges whose label can go: the closure lowered to the nearest statement a plain break reaches.
class LowBreakHelper {
public:
    static void low_break_labels(Statement* root);
    static bool is_break_edge_labeled(Statement* source, Statement* closure);
    static Statement* get_min_closure(Statement* closure, Statement* source);

private:
    static void low_break_labels_rec(Statement* stat);
    static bool is_ok_closure(Statement* closure, Statement* source, Statement* stat);
    static void lift_break_labels(Statement* stat);
    static Statement* get_max_break_lift(Statement* stat, StatEdge* edge);
    static Statement* get_next_break_lift(Statement* stat, StatEdge* edge);
};

// Exits (return/throw/break) folded into their branches; redundant returns removed; the return of an initializer.
class ExitHelper {
public:
    static bool condense_exits(RootStatement* root);
    static bool remove_redundant_returns(RootStatement* root);
    static bool handle_return_from_initializer(RootStatement* root);

private:
    static void clean_up_unreachable_blocks(Statement* stat);
    static int integrate_exits(Statement* stat);
    static Statement* is_exit_edge(StatEdge* edge);
    static bool is_only_edge(StatEdge* edge);
    static Statement* add_shared_initializer_return(RootStatement* root);
};

// Sequences within sequences flattened, empty statements removed, flat blocks merged.
class SequenceHelper {
public:
    static void condense_sequences(Statement* root);
    static void destroy_and_flatten_statement(Statement* stat);
    static void destroy_statement_content(Statement* stat, bool self);

private:
    static void condense_sequences_rec(Statement* stat);
    static bool is_sequence_disbandable(Statement* block, Statement* next);
    static void remove_empty_statements(SequenceStatement* sequence);
    static void merge_flat_statements(SequenceStatement* sequence);
    static Statement* get_first_exprentlist(Statement* stat);
};

// A block entered by a single edge put in place where it is used.
class InlineSingleBlockHelper {
public:
    static bool inline_single_blocks(RootStatement* root);
    static bool is_break_edge_labeled(Statement* source, Statement* closure);

private:
    static bool inline_single_blocks_rec(Statement* stat);
    static void inline_block(SequenceStatement* seq, int index);
    static bool is_inlineable(SequenceStatement* seq, int index);
    static bool same_catch_ranges(StatEdge* edge);
    static bool no_exit_labels(Statement* block, Statement* sequence);
};

// The first or last if of a loop's body that exits it moved out of the loop.
class LoopExtractHelper {
public:
    static bool extract_loops(Statement* root);

private:
    static int extract_loops_rec(Statement* stat);
    static bool extract_loop(DoStatement* stat);
    static bool extract_last_if(DoStatement* stat);
    static bool extract_first_if(DoStatement* stat);
    static bool is_extern_statement(DoStatement* loop, Statement* block, Statement* stat);
    static void extract_if_block(DoStatement* loop, IfStatement* ifstat);
};

// Loops that run once removed (their breaks made to leave the enclosing statement).
class EliminateLoopsHelper {
public:
    static bool eliminate_loops(Statement* root);

private:
    static bool eliminate_loops_rec(Statement* stat);
    static bool is_loop_redundant(DoStatement* loop);
    static Statement* get_max_break_lift(Statement* stat, StatEdge* edge, std::map<int, bool>& stat_labeled, Statement* max);
    static Statement* get_next_break_lift(Statement* stat, StatEdge* edge, std::map<int, bool>& stat_labeled, Statement* max);
    static void eliminate_loop(Statement* loop, Statement* parentloop);
};

// StringBuilder/StringBuffer append chains made into `a + b + c`.
class ConcatenationHelper {
public:
    // The concatenation the expression is, or the expression itself.
    static Exprent* contract_string_concat(Exprent* expr);

private:
    static bool is_append_concat(InvocationExprent* expr, const VarType& cltype);
    static bool is_new_concat(NewExprent* expr, const VarType& cltype);
    static Exprent* remove_string_value_of(Exprent* exprent);

    static constexpr std::string_view builder_class = "java/lang/StringBuilder";
    static constexpr std::string_view buffer_class = "java/lang/StringBuffer";
    static constexpr std::string_view string_class = "java/lang/String";
    static const VarType builder_type;   // object java/lang/StringBuilder
    static const VarType buffer_type;    // object java/lang/StringBuffer
};

// Comparisons with lcmp/fcmp made plain, boolean negations pushed in, `x == true` and the like simplified, and the other function-level rewrites.
class SecondaryFunctionsHelper {
public:
    static bool identify_secondary_functions(Statement* stat);
    // The expression with its boolean negations pushed to the leaves.
    static Exprent* propagate_bool_not(Exprent* exprent);

private:
    static Exprent* identify_secondary_functions(Exprent* exprent, bool statement_level);

    // The negated function of each of the original's eight (NE, EQ, GE, LT, LE, GT, COR, CADD), in its order.
    static const std::array<int, 8> funcsnot;
    // For a comparison, the comparison of a cmp result against 0 it stands for, by -1/0/1 (null: no such form).
    static const std::map<int, std::array<std::optional<int>, 3>> map_num_comparisons;
};

// The simplifications of the stack-variable pass: temporaries folded into their use, array initializers, ++/-- forms, 1.4 class literals, qualified
// `new`, lambdas, constructor calls, and conditions built from the iff pattern.
class SimplifyExprentsHelper {
public:
    explicit SimplifyExprentsHelper(bool first_invocation) : first_invocation_(first_invocation) {}

    // `ssa` and `cl` may be null (EliminateLoopsHelper calls it that way).
    bool simplify_stack_vars_statement(Statement* stat, std::set<int>& set_reordered_ifs, SSAConstructorSparseEx* ssa, const StructClass* cl);

    // The pattern of an inlined 1.4 class literal (parsed once by the original's static initializer: do the same at startup in the .cpp).
    static MatchEngine class14_builder;

private:
    bool simplify_stack_vars_exprents(std::vector<Exprent*>& list, const StructClass* cl);
    static bool add_array_initializer(Exprent* first, Exprent* second);
    static int is_array_initializer(std::vector<Exprent*>& list, int index);
    static bool is_trivial_stack_assignment(Exprent* first);
    static bool is_stack_assignement2(Exprent* first, Exprent* second);   // e.g. 1.4-style class invocation
    static bool is_stack_assignement(Exprent* first, Exprent* second);
    static Exprent* is_ppi_or_mmi(Exprent* first);   // isPPIorMMI
    static bool is_ipp_or_imm(Exprent* first, Exprent* second);   // isIPPorIMM
    static bool is_monitor_exit(Exprent* first);
    static bool is_qualified_new_get_class(Exprent* first, Exprent* second);
    static bool is_constructor_invocation_remote(std::vector<Exprent*>& list, int index);
    static Exprent* is_lambda(Exprent* exprent, const StructClass* cl);
    static Exprent* is_simple_constructor_invocation(Exprent* exprent);
    static bool build_iff(Statement* stat, SSAConstructorSparseEx* ssa);
    static bool collapse_inlined_class14(Statement* stat);

    bool first_invocation_;
};

// i = i + 1 and the like made ++ / -- (on the direct graph of the method).
class PPandMMHelper {
public:
    bool find_pp_and_mm(RootStatement* root);

private:
    bool process_exprent_list(std::vector<Exprent*>& lst);
    Exprent* process_exprent_recursive(Exprent* exprent);

    bool exprent_replaced_ = false;
};

// Removes the checks IntelliJ's @NotNull instrumentation adds (a parameter check at the start, a return check at the end).
class IdeaNotNullHelper {
public:
    static bool remove_hardcoded_checks(Statement* root, const StructMethod* mt);

private:
    static bool find_and_remove_parameter_check(Statement* stat, const StructMethod* mt);
    static void remove_parameter_check(Statement* stat, const StructMethod* mt);
    static bool find_and_remove_return_check(Statement* stat, const StructMethod* mt);
    static bool remove_return_check(Statement* stat, const StructMethod* mt);
};

}  // namespace cb::java::decompiler
