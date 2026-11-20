// java/decompiler/statements.hpp -- the statements a method's graph is folded into: basic blocks, sequences, ifs, loops (do/while/for), switches, try/catch,
// try/finally (catch-all), synchronized blocks, the root; the edges between them (regular, exception, break, continue, finally exit) with their labels;
// and their Java text.
//
//     Statement* if_stat = IfStatement::is_head(head);   // an if headed by `head`, or null
//     root->to_java(1, tracer);
//
// Ported from Fernflower (de.fernflower.modules.decompiler.stats, modules.decompiler.StatEdge), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0
// (see NOTICE in this folder). Statements and edges are objects of the run's arena; the original's identity-hashed sets of them are insertion-ordered
// here (see testdata/oracle/README.txt).
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/match.hpp"
#include "java/decompiler/keyed_list.hpp"

namespace cb::java::decompiler {

class Statement;
class BasicBlockStatement;

class StatEdge : public ArenaObject {
public:
    static constexpr int TYPE_ALL = 0xFF, TYPE_REGULAR = 1, TYPE_EXCEPTION = 2, TYPE_BREAK = 4, TYPE_CONTINUE = 8, TYPE_FINALLYEXIT = 32;
    static constexpr int TYPES[5] = {TYPE_REGULAR, TYPE_EXCEPTION, TYPE_BREAK, TYPE_CONTINUE, TYPE_FINALLYEXIT};

    StatEdge(int type, Statement* source, Statement* destination, Statement* closure_stat)
        : type_(type), source_(source), destination_(destination), closure(closure_stat) {}
    StatEdge(int type, Statement* source, Statement* destination) : StatEdge(type, source, destination, nullptr) {}
    // An exception edge, of these exception types (none: any).
    StatEdge(Statement* source, Statement* destination, const std::optional<std::vector<std::string>>& exceptions)
        : StatEdge(TYPE_EXCEPTION, source, destination) {
        if (exceptions) exceptions_ = *exceptions;
    }

    int get_type() const { return type_; }
    void set_type(int t) { type_ = t; }
    Statement* get_source() const { return source_; }
    void set_source(Statement* s) { source_ = s; }
    Statement* get_destination() const { return destination_; }
    void set_destination(Statement* d) { destination_ = d; }
    const std::optional<std::vector<std::string>>& get_exceptions() const { return exceptions_; }

private:
    int type_;
    Statement* source_;
    Statement* destination_;
    std::optional<std::vector<std::string>> exceptions_;

public:
    Statement* closure = nullptr;   // the statement a break/continue edge leaves (its label's owner)
    bool labeled = true;
    bool explicit_ = true;          // (the original's `explicit`)
};

// An element of a statement's sequential objects: a statement or an expression (the original's List<Object>).
struct SeqObject {
    Statement* stat = nullptr;
    Exprent* expr = nullptr;
};

class Statement : public ArenaObject, public IMatchable {
public:
    static constexpr int STATEDGE_ALL = static_cast<int>(1u << 31), STATEDGE_DIRECT_ALL = 1 << 30;
    static constexpr int DIRECTION_BACKWARD = 0, DIRECTION_FORWARD = 1;
    static constexpr int TYPE_GENERAL = 0, TYPE_IF = 2, TYPE_DO = 5, TYPE_SWITCH = 6, TYPE_TRYCATCH = 7, TYPE_BASICBLOCK = 8, TYPE_FINALLY = 9,
                         TYPE_SYNCRONIZED = 10, TYPE_PLACEHOLDER = 11, TYPE_CATCHALL = 12, TYPE_ROOT = 13, TYPE_DUMMYEXIT = 14, TYPE_SEQUENCE = 15;
    static constexpr int LASTBASICTYPE_IF = 0, LASTBASICTYPE_SWITCH = 1, LASTBASICTYPE_GENERAL = 2;

    int type = 0;
    int id;

    Statement();

    void clear_temp_information();
    void collapse_nodes_to_statement(Statement* stat);
    void set_all_parent();
    void add_labeled_edge(StatEdge* edge);
    void add_predecessor(StatEdge* edge);
    void remove_predecessor(StatEdge* edge);
    void add_successor(StatEdge* edge);
    void remove_successor(StatEdge* edge);
    void remove_all_successors(Statement* stat);
    // The statements a continue edge inside this one needs (insertion-ordered). The set is null after clear_temp_information: then these throw
    // std::runtime_error (the original's NullPointerException).
    LinkedSet<Statement*>& build_continue_set();
    void build_monitor_flags();
    std::vector<Statement*> get_reverse_post_order_list() { return get_reverse_post_order_list(first_); }
    std::vector<Statement*> get_reverse_post_order_list(Statement* stat);
    std::vector<Statement*> get_post_reverse_post_order_list() { return get_post_reverse_post_order_list(std::nullopt); }
    std::vector<Statement*> get_post_reverse_post_order_list(std::optional<std::vector<Statement*>> lstexits);
    bool contains_statement(const Statement* stat) const { return this == stat || contains_statement_strict(stat); }
    bool contains_statement_strict(const Statement* stat) const;
    TextBuffer to_java();
    virtual TextBuffer to_java(int indent, BytecodeMappingTracer& tracer);
    virtual std::vector<SeqObject> get_sequential_objects();
    // IMatchable (struct/match): the statement or expression at `index` under a match node; whether this statement fits the node's rules.
    // (Once match.hpp exists, Statement derives IMatchable and these override its virtuals.)
    IMatchable* find_object(const MatchNode* match_node, int index) override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    virtual void init_exprents() {}
    virtual void replace_exprent(Exprent* oldexpr, Exprent* newexpr) { (void)oldexpr, (void)newexpr; }
    virtual Statement* get_simple_copy();
    virtual void init_simple_copy();
    virtual void replace_statement(Statement* oldstat, Statement* newstat);
    void change_edge_node(int direction, StatEdge* edge, Statement* value);
    void change_edge_type(int direction, StatEdge* edge, int newtype);
    std::vector<Statement*> get_neighbours(int type, int direction);
    LinkedSet<Statement*> get_neighbours_set(int type, int direction);   // (the original's HashSet of statements: insertion-ordered, each once)
    std::vector<StatEdge*> get_successor_edges(int edge_type) { return get_edges(edge_type, DIRECTION_FORWARD); }
    std::vector<StatEdge*> get_predecessor_edges(int edge_type) { return get_edges(edge_type, DIRECTION_BACKWARD); }
    std::vector<StatEdge*> get_all_successor_edges() { return get_edges(STATEDGE_ALL, DIRECTION_FORWARD); }
    std::vector<StatEdge*> get_all_predecessor_edges() { return get_edges(STATEDGE_ALL, DIRECTION_BACKWARD); }
    Statement* get_first() const { return first_; }
    void set_first(Statement* s) { first_ = s; }
    Statement* get_post() const { return post_; }
    void set_post(Statement* s) { post_ = s; }
    KeyedList<Statement*, int>& get_stats() { return stats_; }
    int get_last_basic_type() const { return last_basic_type_; }
    LinkedSet<Statement*>& get_continue_set();
    bool is_contains_monitor_exit() const { return contains_monitor_exit_; }
    bool is_monitor_enter() const { return is_monitor_enter_; }
    BasicBlockStatement* get_basichead();
    bool is_labeled() const;
    bool has_basic_succ_edge() const;
    Statement* get_parent() const { return parent_; }
    void set_parent(Statement* p) { parent_ = p; }
    LinkedSet<StatEdge*>& get_label_edges() { return label_edges_; }
    std::vector<Exprent*>& get_var_definitions() { return var_definitions_; }
    // The block's expressions (null for a statement that has none: the original's null list).
    std::optional<std::vector<Exprent*>>& get_exprents() { return exprents_; }
    void set_exprents(std::optional<std::vector<Exprent*>> e) { exprents_ = std::move(e); }
    bool is_copied() const { return copied_; }
    void set_copied(bool c) { copied_ = c; }
    std::string to_string() const { return std::to_string(id); }

protected:
    KeyedList<Statement*, int> stats_;
    Statement* parent_ = nullptr;
    Statement* first_ = nullptr;
    std::optional<std::vector<Exprent*>> exprents_;
    LinkedSet<StatEdge*> label_edges_;
    std::vector<Exprent*> var_definitions_;
    Statement* post_ = nullptr;
    int last_basic_type_ = LASTBASICTYPE_GENERAL;
    bool is_monitor_enter_ = false;
    bool contains_monitor_exit_ = false;
    std::optional<LinkedSet<Statement*>> continue_set_ = LinkedSet<Statement*>{};

private:
    template <class T>
    static void process_map(std::map<int, std::vector<T>>& map);
    static void add_to_reverse_post_order_list_iterative(Statement* root, std::vector<Statement*>& lst);
    static void add_to_post_reverse_post_order_list(Statement* stat, std::vector<Statement*>& lst, LinkedSet<Statement*>& set_visited);
    std::vector<StatEdge*> get_edges(int type, int direction);
    void add_edge_direct_internal(int direction, StatEdge* edge, int edgetype);
    void add_edge_internal(int direction, StatEdge* edge);
    void remove_edge_direct_internal(int direction, StatEdge* edge, int edgetype);
    void remove_edge_internal(int direction, StatEdge* edge);

    std::map<int, std::vector<StatEdge*>> map_succ_edges_, map_pred_edges_;
    std::map<int, std::vector<Statement*>> map_succ_states_, map_pred_states_;
    bool copied_ = false;
};

class BasicBlockStatement : public Statement {
public:
    explicit BasicBlockStatement(BasicBlock* block);
    // A statement of a block of its own (a simple copy's, or the empty one a switch makes for its exits): the original's `new BasicBlock(id)`.
    static BasicBlockStatement* make_owning(std::unique_ptr<BasicBlock> block);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    Statement* get_simple_copy() override;
    BasicBlock* get_block() const { return block_; }

private:
    BasicBlock* block_;
    std::unique_ptr<BasicBlock> own_block_;   // the block of a simple copy (the statement owns it: no graph does)
};

class GeneralStatement : public Statement {
public:
    GeneralStatement() { type = TYPE_GENERAL; }
    // `statements` is the original's Collection (a HashSet here: each once, the head left out of it).
    GeneralStatement(Statement* head, const std::vector<Statement*>& statements, Statement* post);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
};

class RootStatement : public Statement {
public:
    RootStatement(Statement* head, Statement* dummy_exit);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    Statement* get_dummy_exit() const { return dummy_exit_; }
    void set_dummy_exit(Statement* s) { dummy_exit_ = s; }

private:
    Statement* dummy_exit_;
};

class SequenceStatement : public Statement {
public:
    SequenceStatement() { type = TYPE_SEQUENCE; }
    explicit SequenceStatement(const std::vector<Statement*>& lst);
    SequenceStatement(Statement* head, Statement* tail);   // (the original's private one)
    static Statement* is_head2_block(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    Statement* get_simple_copy() override;
};

class IfStatement : public Statement {
public:
    static constexpr int IFTYPE_IF = 0, IFTYPE_IFELSE = 1;
    int iftype = 0;

    IfStatement();
    IfStatement(Statement* head, int regedges, Statement* postst);   // (the original's private one; public so make<T> reaches it)
    static Statement* is_head(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void init_exprents() override;
    std::vector<SeqObject> get_sequential_objects() override;
    void replace_exprent(Exprent* oldexpr, Exprent* newexpr) override;
    void replace_statement(Statement* oldstat, Statement* newstat) override;
    Statement* get_simple_copy() override;
    void init_simple_copy() override;
    IMatchable* find_object(const MatchNode* match_node, int index) override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    Statement* get_elsestat() const { return elsestat_; }
    void set_elsestat(Statement* s) { elsestat_ = s; }
    Statement* get_ifstat() const { return ifstat_; }
    void set_ifstat(Statement* s) { ifstat_ = s; }
    bool is_negated() const { return negated_; }
    void set_negated(bool n) { negated_ = n; }
    std::vector<Exprent*>& get_headexprent_list() { return headexprent_; }
    IfExprent* get_headexprent() const { return static_cast<IfExprent*>(headexprent_[0]); }
    bool is_iffflag() const { return iffflag_; }
    void set_iffflag(bool f) { iffflag_ = f; }
    void set_else_edge(StatEdge* e) { elseedge_ = e; }
    void set_if_edge(StatEdge* e) { ifedge_ = e; }
    StatEdge* get_if_edge() const { return ifedge_; }
    StatEdge* get_else_edge() const { return elseedge_; }

private:
    Statement* ifstat_ = nullptr;
    Statement* elsestat_ = nullptr;
    StatEdge* ifedge_ = nullptr;
    StatEdge* elseedge_ = nullptr;
    bool negated_ = false;
    bool iffflag_ = false;
    std::vector<Exprent*> headexprent_{nullptr};
};

class DoStatement : public Statement {
public:
    static constexpr int LOOP_DO = 0, LOOP_DOWHILE = 1, LOOP_WHILE = 2, LOOP_FOR = 3;
    DoStatement() { type = TYPE_DO; }
    explicit DoStatement(Statement* head);   // (the original's private one)
    static Statement* is_head(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    std::vector<SeqObject> get_sequential_objects() override;
    void replace_exprent(Exprent* oldexpr, Exprent* newexpr) override;
    Statement* get_simple_copy() override;
    std::vector<Exprent*>& get_init_exprent_list() { return init_exprent_; }
    std::vector<Exprent*>& get_condition_exprent_list() { return condition_exprent_; }
    std::vector<Exprent*>& get_inc_exprent_list() { return inc_exprent_; }
    Exprent* get_condition_exprent() const { return condition_exprent_[0]; }
    void set_condition_exprent(Exprent* e) { condition_exprent_[0] = e; }
    Exprent* get_inc_exprent() const { return inc_exprent_[0]; }
    void set_inc_exprent(Exprent* e) { inc_exprent_[0] = e; }
    Exprent* get_init_exprent() const { return init_exprent_[0]; }
    void set_init_exprent(Exprent* e) { init_exprent_[0] = e; }
    int get_looptype() const { return looptype_; }
    void set_looptype(int t) { looptype_ = t; }

private:
    int looptype_ = LOOP_DO;
    std::vector<Exprent*> init_exprent_{nullptr}, condition_exprent_{nullptr}, inc_exprent_{nullptr};
};

class SwitchStatement : public Statement {
public:
    SwitchStatement() { type = TYPE_SWITCH; }
    SwitchStatement(Statement* head, Statement* poststat);   // (the original's private one)
    static Statement* is_head(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void init_exprents() override;
    std::vector<SeqObject> get_sequential_objects() override;
    void replace_exprent(Exprent* oldexpr, Exprent* newexpr) override;
    void replace_statement(Statement* oldstat, Statement* newstat) override;
    Statement* get_simple_copy() override;
    void init_simple_copy() override;
    void sort_edges_and_nodes();
    std::vector<Exprent*>& get_headexprent_list() { return headexprent_; }
    Exprent* get_headexprent() const { return headexprent_[0]; }
    std::vector<std::vector<StatEdge*>>& get_case_edges() { return case_edges_; }
    std::vector<Statement*>& get_case_statements() { return case_statements_; }
    StatEdge* get_default_edge() const { return default_edge_; }   // (the original's getDefault_edge)
    std::vector<std::vector<ConstExprent*>>& get_case_values() { return case_values_; }

private:
    std::vector<Statement*> case_statements_;
    std::vector<std::vector<StatEdge*>> case_edges_;
    std::vector<std::vector<ConstExprent*>> case_values_;
    StatEdge* default_edge_ = nullptr;
    std::vector<Exprent*> headexprent_{nullptr};
};

class SynchronizedStatement : public Statement {
public:
    SynchronizedStatement() { type = TYPE_SYNCRONIZED; }
    SynchronizedStatement(Statement* head, Statement* body, Statement* exc);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void init_exprents() override;
    std::vector<SeqObject> get_sequential_objects() override;
    void replace_exprent(Exprent* oldexpr, Exprent* newexpr) override;
    void replace_statement(Statement* oldstat, Statement* newstat) override;
    void remove_exc();
    Statement* get_simple_copy() override;
    void init_simple_copy() override;
    Statement* get_body() const { return body_; }
    void set_body(Statement* b) { body_ = b; }
    std::vector<Exprent*>& get_headexprent_list() { return headexprent_; }
    Exprent* get_headexprent() const { return headexprent_[0]; }

private:
    Statement* body_ = nullptr;
    std::vector<Exprent*> headexprent_{nullptr};
};

class CatchStatement : public Statement {
public:
    CatchStatement() { type = TYPE_TRYCATCH; }
    CatchStatement(Statement* head, Statement* next, const LinkedSet<Statement*>& set_handlers);   // (the original's private one)
    static Statement* is_head(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    Statement* get_simple_copy() override;
    std::vector<VarExprent*>& get_vars() { return vars_; }

private:
    std::vector<std::vector<std::string>> exctstrings_;   // the caught exception types of each handler
    std::vector<VarExprent*> vars_;
};

class CatchAllStatement : public Statement {
public:
    CatchAllStatement() { type = TYPE_CATCHALL; }
    CatchAllStatement(Statement* head, Statement* handler);   // (the original's private one)
    static Statement* is_head(Statement* head);
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_statement(Statement* oldstat, Statement* newstat) override;
    Statement* get_simple_copy() override;
    void init_simple_copy() override;
    Statement* get_handler() const { return handler_; }
    void set_handler(Statement* h) { handler_ = h; }
    bool is_finally() const { return is_finally_; }
    void set_finally(bool f) { is_finally_ = f; }
    VarExprent* get_monitor() const { return monitor_; }
    void set_monitor(VarExprent* m) { monitor_ = m; }
    std::vector<VarExprent*>& get_vars() { return vars_; }

private:
    Statement* handler_ = nullptr;
    bool is_finally_ = false;
    VarExprent* monitor_ = nullptr;
    std::vector<VarExprent*> vars_;
};

}  // namespace cb::java::decompiler
