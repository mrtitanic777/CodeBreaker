// stat_base.cpp -- Statement (the edges, the nesting, the traversals), BasicBlockStatement, GeneralStatement, RootStatement, SequenceStatement.
// Ported from Fernflower (de.fernflower.modules.decompiler.stats), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <regex>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

namespace {

// (type & (type - 1)) == 0 as Java computes it on ints (a single edge type): STATEDGE_ALL is the sign bit.
bool is_single_type(int type) {
    const auto u = static_cast<unsigned>(type);
    return (u & (u - 1)) == 0;
}

bool is_integer_text(const std::string& s) {
    static const std::regex re("-?\\d+");
    return std::regex_match(s, re);
}

}  // namespace

// ---- Statement

Statement::Statement() : id(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER)) {}

void Statement::clear_temp_information() {
    post_ = nullptr;
    continue_set_.reset();
    copied_ = false;
    is_monitor_enter_ = false;
    contains_monitor_exit_ = false;

    process_map(map_succ_edges_);
    process_map(map_pred_edges_);
    process_map(map_succ_states_);
    process_map(map_pred_states_);
}

template <class T>
void Statement::process_map(std::map<int, std::vector<T>>& map) {
    map.erase(StatEdge::TYPE_EXCEPTION);
    const auto it = map.find(STATEDGE_DIRECT_ALL);
    if (it != map.end()) {
        map[STATEDGE_ALL] = std::vector<T>(it->second);
    } else {
        map.erase(STATEDGE_ALL);
    }
}

void Statement::collapse_nodes_to_statement(Statement* stat) {
    Statement* head = stat->get_first();
    Statement* post = stat->get_post();

    KeyedList<Statement*, int>& set_nodes = stat->get_stats();

    if (post != nullptr) {
        for (StatEdge* edge : post->get_edges(STATEDGE_DIRECT_ALL, DIRECTION_BACKWARD)) {
            if (stat->contains_statement_strict(edge->get_source())) {
                edge->get_source()->change_edge_type(DIRECTION_FORWARD, edge, StatEdge::TYPE_BREAK);
                stat->add_labeled_edge(edge);
            }
        }
    }

    for (StatEdge* prededge : head->get_all_predecessor_edges()) {
        if (prededge->get_type() != StatEdge::TYPE_EXCEPTION && stat->contains_statement_strict(prededge->get_source())) {
            prededge->get_source()->change_edge_type(DIRECTION_FORWARD, prededge, StatEdge::TYPE_CONTINUE);
            stat->add_labeled_edge(prededge);
        }

        head->remove_predecessor(prededge);
        prededge->get_source()->change_edge_node(DIRECTION_FORWARD, prededge, stat);
        stat->add_predecessor(prededge);
    }

    if (set_nodes.contains_key(first_->id)) {
        first_ = stat;
    }

    LinkedSet<Statement*> set_handlers = LinkedSet<Statement*>::of(head->get_neighbours(StatEdge::TYPE_EXCEPTION, DIRECTION_FORWARD));
    for (Statement* node : set_nodes) {
        set_handlers.retain_all(LinkedSet<Statement*>::of(node->get_neighbours(StatEdge::TYPE_EXCEPTION, DIRECTION_FORWARD)));
    }

    if (!set_handlers.empty()) {
        for (StatEdge* edge : head->get_edges(StatEdge::TYPE_EXCEPTION, DIRECTION_FORWARD)) {
            Statement* handler = edge->get_destination();
            if (set_handlers.contains(handler)) {
                if (!set_nodes.contains_key(handler->id)) {
                    stat->add_successor(make<StatEdge>(stat, handler, edge->get_exceptions()));
                }
            }
        }

        for (Statement* node : set_nodes) {
            for (StatEdge* edge : node->get_edges(StatEdge::TYPE_EXCEPTION, DIRECTION_FORWARD)) {
                if (set_handlers.contains(edge->get_destination())) {
                    node->remove_successor(edge);
                }
            }
        }
    }

    if (post != nullptr) {
        const std::vector<Statement*> exc = stat->get_neighbours(StatEdge::TYPE_EXCEPTION, DIRECTION_FORWARD);
        if (std::find(exc.begin(), exc.end(), post) == exc.end()) {   // TODO: second condition redundant?
            stat->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, stat, post));
        }
    }

    for (Statement* st : set_nodes) {
        stats_.remove_with_key(st->id);
    }

    stats_.add_with_key(stat, stat->id);

    stat->set_all_parent();
    stat->set_parent(this);

    stat->build_continue_set();
    stat->build_monitor_flags();

    if (stat->type == TYPE_SWITCH) {
        static_cast<SwitchStatement*>(stat)->sort_edges_and_nodes();
    }
}

void Statement::set_all_parent() {
    for (Statement* st : stats_) st->set_parent(this);
}

void Statement::add_labeled_edge(StatEdge* edge) {
    if (edge->closure != nullptr) edge->closure->get_label_edges().remove(edge);
    edge->closure = this;
    get_label_edges().add(edge);
}

void Statement::add_edge_direct_internal(int direction, StatEdge* edge, int edgetype) {
    auto& map_edges = direction == DIRECTION_BACKWARD ? map_pred_edges_ : map_succ_edges_;
    auto& map_states = direction == DIRECTION_BACKWARD ? map_pred_states_ : map_succ_states_;

    map_edges[edgetype].push_back(edge);
    map_states[edgetype].push_back(direction == DIRECTION_BACKWARD ? edge->get_source() : edge->get_destination());
}

void Statement::add_edge_internal(int direction, StatEdge* edge) {
    const int etype = edge->get_type();

    if (etype == StatEdge::TYPE_EXCEPTION) {
        const int arrtypes[] = {STATEDGE_ALL, StatEdge::TYPE_EXCEPTION};
        for (const int edgetype : arrtypes) add_edge_direct_internal(direction, edge, edgetype);
    } else {
        const int arrtypes[] = {STATEDGE_ALL, STATEDGE_DIRECT_ALL, etype};
        for (const int edgetype : arrtypes) add_edge_direct_internal(direction, edge, edgetype);
    }
}

void Statement::remove_edge_direct_internal(int direction, StatEdge* edge, int edgetype) {
    auto& map_edges = direction == DIRECTION_BACKWARD ? map_pred_edges_ : map_succ_edges_;
    auto& map_states = direction == DIRECTION_BACKWARD ? map_pred_states_ : map_succ_states_;

    const auto it = map_edges.find(edgetype);
    if (it != map_edges.end()) {
        auto& lst = it->second;
        const auto pos = std::find(lst.begin(), lst.end(), edge);
        if (pos != lst.end()) {
            const auto index = pos - lst.begin();
            lst.erase(pos);
            auto& states = map_states.at(edgetype);
            states.erase(states.begin() + index);
        }
    }
}

void Statement::remove_edge_internal(int direction, StatEdge* edge) {
    const int etype = edge->get_type();

    if (etype == StatEdge::TYPE_EXCEPTION) {
        const int arrtypes[] = {STATEDGE_ALL, StatEdge::TYPE_EXCEPTION};
        for (const int edgetype : arrtypes) remove_edge_direct_internal(direction, edge, edgetype);
    } else {
        const int arrtypes[] = {STATEDGE_ALL, STATEDGE_DIRECT_ALL, etype};
        for (const int edgetype : arrtypes) remove_edge_direct_internal(direction, edge, edgetype);
    }
}

void Statement::add_predecessor(StatEdge* edge) { add_edge_internal(DIRECTION_BACKWARD, edge); }

void Statement::remove_predecessor(StatEdge* edge) {
    if (edge == nullptr) return;   // FIXME: redundant?
    remove_edge_internal(DIRECTION_BACKWARD, edge);
}

void Statement::add_successor(StatEdge* edge) {
    add_edge_internal(DIRECTION_FORWARD, edge);

    if (edge->closure != nullptr) edge->closure->get_label_edges().add(edge);

    edge->get_destination()->add_predecessor(edge);
}

void Statement::remove_successor(StatEdge* edge) {
    if (edge == nullptr) return;

    remove_edge_internal(DIRECTION_FORWARD, edge);

    if (edge->closure != nullptr) edge->closure->get_label_edges().remove(edge);

    if (edge->get_destination() != nullptr) {   // TODO: redundant?
        edge->get_destination()->remove_predecessor(edge);
    }
}

void Statement::remove_all_successors(Statement* stat) {
    if (stat == nullptr) return;

    for (StatEdge* edge : get_all_successor_edges()) {
        if (edge->get_destination() == stat) remove_successor(edge);
    }
}

LinkedSet<Statement*>& Statement::build_continue_set() {
    LinkedSet<Statement*>& cs = continue_set_.value();   // (null after clear_temp_information: the original's NullPointerException)
    cs.clear();

    for (Statement* st : stats_) {
        cs.add_all(st->build_continue_set());
        if (st != first_) cs.remove(st->get_basichead());
    }

    for (StatEdge* edge : get_edges(StatEdge::TYPE_CONTINUE, DIRECTION_FORWARD)) {
        cs.add(edge->get_destination()->get_basichead());
    }

    if (type == TYPE_DO) cs.remove(first_->get_basichead());

    return cs;
}

void Statement::build_monitor_flags() {
    for (Statement* st : stats_) st->build_monitor_flags();

    switch (type) {
        case TYPE_BASICBLOCK: {
            BasicBlockStatement* bblock = static_cast<BasicBlockStatement*>(this);
            const std::shared_ptr<InstructionSequence>& seq = bblock->get_block()->seq;

            if (seq != nullptr && seq->length() > 0) {
                for (std::size_t i = 0; i < seq->length(); i++) {
                    if (seq->instr(i)->opcode == cc::opc_monitorexit) {
                        contains_monitor_exit_ = true;
                        break;
                    }
                }

                is_monitor_enter_ = (seq->last_instr()->opcode == cc::opc_monitorenter);
            }
            break;
        }
        case TYPE_SEQUENCE:
        case TYPE_IF:
            contains_monitor_exit_ = false;
            for (Statement* st : stats_) contains_monitor_exit_ |= st->is_contains_monitor_exit();
            break;
        case TYPE_SYNCRONIZED:
        case TYPE_ROOT:
        case TYPE_GENERAL:
            break;
        default:
            contains_monitor_exit_ = false;
            for (Statement* st : stats_) contains_monitor_exit_ |= st->is_contains_monitor_exit();
    }
}

std::vector<Statement*> Statement::get_reverse_post_order_list(Statement* stat) {
    std::vector<Statement*> res;
    add_to_reverse_post_order_list_iterative(stat, res);
    return res;
}

std::vector<Statement*> Statement::get_post_reverse_post_order_list(std::optional<std::vector<Statement*>> lstexits) {
    std::vector<Statement*> res;

    if (!lstexits) {
        StrongConnectivityHelper schelper(this);
        lstexits = StrongConnectivityHelper::get_exit_reps(schelper.get_components());
    }

    LinkedSet<Statement*> set_visited;

    for (Statement* exit : *lstexits) {
        add_to_post_reverse_post_order_list(exit, res, set_visited);
    }

    if (res.size() != stats_.size()) throw std::runtime_error("computing post reverse post order failed!");

    return res;
}

bool Statement::contains_statement_strict(const Statement* stat) const {
    for (std::size_t i = 0; i < stats_.size(); i++) {
        if (stats_[i] == stat) return true;
    }

    for (std::size_t i = 0; i < stats_.size(); i++) {
        if (stats_[i]->contains_statement_strict(stat)) return true;
    }

    return false;
}

TextBuffer Statement::to_java() {
    BytecodeMappingTracer tracer;
    return to_java(0, tracer);
}

TextBuffer Statement::to_java(int indent, BytecodeMappingTracer& tracer) {
    (void)indent, (void)tracer;
    throw std::runtime_error("not implemented");
}

std::vector<SeqObject> Statement::get_sequential_objects() {
    std::vector<SeqObject> res;
    for (Statement* st : stats_) res.push_back({st, nullptr});
    return res;
}

Statement* Statement::get_simple_copy() { throw std::runtime_error("not implemented"); }

void Statement::init_simple_copy() {
    if (!stats_.empty()) first_ = stats_.get(0);
}

void Statement::replace_statement(Statement* oldstat, Statement* newstat) {
    for (StatEdge* edge : oldstat->get_all_predecessor_edges()) {
        oldstat->remove_predecessor(edge);
        edge->get_source()->change_edge_node(DIRECTION_FORWARD, edge, newstat);
        newstat->add_predecessor(edge);
    }

    for (StatEdge* edge : oldstat->get_all_successor_edges()) {
        oldstat->remove_successor(edge);
        edge->set_source(newstat);
        newstat->add_successor(edge);
    }

    const int statindex = static_cast<int>(stats_.index_by_key(oldstat->id));
    stats_.remove_with_key(oldstat->id);
    stats_.add_with_key_and_index(static_cast<std::size_t>(statindex), newstat, newstat->id);

    newstat->set_parent(this);
    newstat->post_ = oldstat->post_;

    if (first_ == oldstat) first_ = newstat;

    const std::vector<StatEdge*> lst = oldstat->get_label_edges().items();
    for (int i = static_cast<int>(lst.size()) - 1; i >= 0; i--) {
        StatEdge* edge = lst[static_cast<std::size_t>(i)];
        if (edge->get_source() != newstat) {
            newstat->add_labeled_edge(edge);
        } else {
            if (this == edge->get_destination() || this->contains_statement_strict(edge->get_destination())) {
                edge->closure = nullptr;
            } else {
                this->add_labeled_edge(edge);
            }
        }
    }
    oldstat->get_label_edges().clear();
}

void Statement::add_to_reverse_post_order_list_iterative(Statement* root, std::vector<Statement*>& lst) {
    std::vector<Statement*> stack_node;
    std::vector<int> stack_index;
    LinkedSet<Statement*> set_visited;

    stack_node.push_back(root);
    stack_index.push_back(0);

    while (!stack_node.empty()) {
        Statement* node = stack_node.back();
        int index = stack_index.back();
        stack_index.pop_back();

        set_visited.add(node);

        const std::vector<StatEdge*> lst_edges = node->get_all_successor_edges();

        for (; index < static_cast<int>(lst_edges.size()); index++) {
            StatEdge* edge = lst_edges[static_cast<std::size_t>(index)];
            Statement* succ = edge->get_destination();

            if (!set_visited.contains(succ) &&
                (edge->get_type() == StatEdge::TYPE_REGULAR || edge->get_type() == StatEdge::TYPE_EXCEPTION)) {   // TODO: edge filter?
                stack_index.push_back(index + 1);

                stack_node.push_back(succ);
                stack_index.push_back(0);

                break;
            }
        }

        if (index == static_cast<int>(lst_edges.size())) {
            lst.insert(lst.begin(), node);

            stack_node.pop_back();
        }
    }
}

void Statement::add_to_post_reverse_post_order_list(Statement* stat, std::vector<Statement*>& lst, LinkedSet<Statement*>& set_visited) {
    if (set_visited.contains(stat)) {   // because of not considered exception edges, s. isExitComponent. Should be rewritten, if possible.
        return;
    }

    set_visited.add(stat);

    for (StatEdge* prededge : stat->get_edges(StatEdge::TYPE_REGULAR | StatEdge::TYPE_EXCEPTION, DIRECTION_BACKWARD)) {
        Statement* pred = prededge->get_source();
        if (!set_visited.contains(pred)) add_to_post_reverse_post_order_list(pred, lst, set_visited);
    }

    lst.insert(lst.begin(), stat);
}

void Statement::change_edge_node(int direction, StatEdge* edge, Statement* value) {
    auto& map_edges = direction == DIRECTION_BACKWARD ? map_pred_edges_ : map_succ_edges_;
    auto& map_states = direction == DIRECTION_BACKWARD ? map_pred_states_ : map_succ_states_;

    const int etype = edge->get_type();

    auto change = [&](int edgetype) {
        const auto it = map_edges.find(edgetype);
        if (it != map_edges.end()) {
            const auto& lst = it->second;
            const auto pos = std::find(lst.begin(), lst.end(), edge);
            if (pos != lst.end()) map_states.at(edgetype)[static_cast<std::size_t>(pos - lst.begin())] = value;
        }
    };

    if (etype == StatEdge::TYPE_EXCEPTION) {
        const int arrtypes[] = {STATEDGE_ALL, StatEdge::TYPE_EXCEPTION};
        for (const int edgetype : arrtypes) change(edgetype);
    } else {
        const int arrtypes[] = {STATEDGE_ALL, STATEDGE_DIRECT_ALL, etype};
        for (const int edgetype : arrtypes) change(edgetype);
    }

    if (direction == DIRECTION_BACKWARD) {
        edge->set_source(value);
    } else {
        edge->set_destination(value);
    }
}

void Statement::change_edge_type(int direction, StatEdge* edge, int newtype) {
    const int oldtype = edge->get_type();
    if (oldtype == newtype) return;

    if (oldtype == StatEdge::TYPE_EXCEPTION || newtype == StatEdge::TYPE_EXCEPTION) throw std::runtime_error("Invalid edge type!");

    remove_edge_direct_internal(direction, edge, oldtype);
    add_edge_direct_internal(direction, edge, newtype);

    if (direction == DIRECTION_FORWARD) edge->get_destination()->change_edge_type(DIRECTION_BACKWARD, edge, newtype);

    edge->set_type(newtype);
}

std::vector<StatEdge*> Statement::get_edges(int edge_type, int direction) {
    const auto& map = direction == DIRECTION_BACKWARD ? map_pred_edges_ : map_succ_edges_;

    std::vector<StatEdge*> res;

    if (is_single_type(edge_type)) {
        const auto it = map.find(edge_type);
        if (it != map.end()) res = it->second;
    } else {
        for (const int edgetype : StatEdge::TYPES) {
            if ((edge_type & edgetype) != 0) {
                const auto it = map.find(edgetype);
                if (it != map.end()) res.insert(res.end(), it->second.begin(), it->second.end());
            }
        }
    }

    return res;
}

std::vector<Statement*> Statement::get_neighbours(int edge_type, int direction) {
    const auto& map = direction == DIRECTION_BACKWARD ? map_pred_states_ : map_succ_states_;

    std::vector<Statement*> res;

    if (is_single_type(edge_type)) {
        const auto it = map.find(edge_type);
        if (it != map.end()) res = it->second;
    } else {
        for (const int edgetype : StatEdge::TYPES) {
            if ((edge_type & edgetype) != 0) {
                const auto it = map.find(edgetype);
                if (it != map.end()) res.insert(res.end(), it->second.begin(), it->second.end());
            }
        }
    }

    return res;
}

LinkedSet<Statement*> Statement::get_neighbours_set(int edge_type, int direction) {
    return LinkedSet<Statement*>::of(get_neighbours(edge_type, direction));
}

LinkedSet<Statement*>& Statement::get_continue_set() {
    if (!continue_set_) throw std::runtime_error("continue set is null");
    return *continue_set_;
}

BasicBlockStatement* Statement::get_basichead() {
    if (type == TYPE_BASICBLOCK) return static_cast<BasicBlockStatement*>(this);
    return first_->get_basichead();
}

bool Statement::is_labeled() const {
    for (const StatEdge* edge : label_edges_) {
        if (edge->labeled && edge->explicit_) return true;   // FIXME: consistent setting
    }
    return false;
}

bool Statement::has_basic_succ_edge() const {
    return type == TYPE_BASICBLOCK || (type == TYPE_IF && static_cast<const IfStatement*>(this)->iftype == IfStatement::IFTYPE_IF) ||
           (type == TYPE_DO && static_cast<const DoStatement*>(this)->get_looptype() != DoStatement::LOOP_DO);
}

IMatchable* Statement::find_object(const MatchNode* match_node, int index) {
    const int node_type = match_node->get_type();

    if (node_type == MatchNode::MATCHNODE_STATEMENT && !stats_.empty()) {
        const MatchValue value = match_node->get_rule_value(MatchProperties::STATEMENT_POSITION);
        const std::string* position = std::get_if<std::string>(&value);
        if (position != nullptr) {
            if (is_integer_text(*position)) {
                const int size = static_cast<int>(stats_.size());
                return stats_.get(static_cast<std::size_t>((size + std::stoi(*position)) % size));   // care for negative positions
            }
        } else if (index < static_cast<int>(stats_.size())) {   // use 'index' parameter
            return stats_.get(static_cast<std::size_t>(index));
        }
    } else if (node_type == MatchNode::MATCHNODE_EXPRENT && exprents_ && !exprents_->empty()) {
        const MatchValue value = match_node->get_rule_value(MatchProperties::EXPRENT_POSITION);
        const std::string* position = std::get_if<std::string>(&value);
        if (position != nullptr) {
            if (is_integer_text(*position)) {
                const int size = static_cast<int>(exprents_->size());
                return exprents_->at(static_cast<std::size_t>((size + std::stoi(*position)) % size));   // care for negative positions
            }
        } else if (index < static_cast<int>(exprents_->size())) {   // use 'index' parameter
            return exprents_->at(static_cast<std::size_t>(index));
        }
    }

    return nullptr;
}

bool Statement::match(const MatchNode* match_node, MatchEngine& engine) {
    if (match_node->get_type() != MatchNode::MATCHNODE_STATEMENT) return false;

    for (const auto& [key, rule] : match_node->get_rules()) {
        switch (key) {
            case MatchProperties::STATEMENT_TYPE:
                if (this->type != std::get<std::int32_t>(rule.value)) return false;
                break;
            case MatchProperties::STATEMENT_STATSIZE:
                if (static_cast<int>(this->stats_.size()) != std::get<std::int32_t>(rule.value)) return false;
                break;
            case MatchProperties::STATEMENT_EXPRSIZE: {
                const int exprsize = std::get<std::int32_t>(rule.value);
                if (exprsize == -1) {
                    if (this->exprents_) return false;
                } else {
                    if (!this->exprents_ || static_cast<int>(this->exprents_->size()) != exprsize) return false;
                }
                break;
            }
            case MatchProperties::STATEMENT_RET:
                if (!engine.check_and_set_variable_value(std::get<std::string>(rule.value), MatchValue(static_cast<IMatchable*>(this)))) return false;
                break;
            default:
                break;
        }
    }

    return true;
}

// ---- BasicBlockStatement

BasicBlockStatement::BasicBlockStatement(BasicBlock* block) : block_(block) {
    type = TYPE_BASICBLOCK;

    id = block->id;
    CounterContainer& coun = DecompilerContext::counter_container();
    if (id >= coun.get_counter(CounterContainer::STATEMENT_COUNTER)) coun.set_counter(CounterContainer::STATEMENT_COUNTER, id + 1);

    const std::shared_ptr<Instruction> instr = block->last_instruction();
    if (instr != nullptr) {
        if (instr->group == cc::GROUP_JUMP && instr->opcode != cc::opc_goto) {
            last_basic_type_ = LASTBASICTYPE_IF;
        } else if (instr->group == cc::GROUP_SWITCH) {
            last_basic_type_ = LASTBASICTYPE_SWITCH;
        }
    }

    // monitorenter and monitorexits
    build_monitor_flags();
}

TextBuffer BasicBlockStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    return ExprProcessor::list_to_java(var_definitions_, indent, tracer).append(ExprProcessor::list_to_java(exprents_, indent, tracer));
}

Statement* BasicBlockStatement::get_simple_copy() {
    auto newblock = std::make_unique<BasicBlock>();
    newblock->id = DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER);

    auto seq = std::make_shared<InstructionSequence>();
    for (std::size_t i = 0; i < block_->seq->length(); i++) {
        seq->add_instruction(block_->seq->instr(i)->clone(), -1);
    }
    newblock->seq = seq;

    return make_owning(std::move(newblock));
}

BasicBlockStatement* BasicBlockStatement::make_owning(std::unique_ptr<BasicBlock> block) {
    BasicBlockStatement* stat = make<BasicBlockStatement>(block.get());
    stat->own_block_ = std::move(block);
    return stat;
}

// ---- GeneralStatement

GeneralStatement::GeneralStatement(Statement* head, const std::vector<Statement*>& statements, Statement* post_stat) {
    type = TYPE_GENERAL;

    first_ = head;
    stats_.add_with_key(head, head->id);

    LinkedSet<Statement*> set = LinkedSet<Statement*>::of(statements);
    set.remove(head);

    for (Statement* st : set) stats_.add_with_key(st, st->id);

    post_ = post_stat;
}

TextBuffer GeneralStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    if (is_labeled()) buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();

    buf.append_indent(indent).append("abstract statement {").append_line_separator();
    for (Statement* stat : stats_) buf.append(stat->to_java(indent + 1, tracer));
    buf.append_indent(indent).append("}");

    return buf;
}

// ---- RootStatement

RootStatement::RootStatement(Statement* head, Statement* dummy_exit) : dummy_exit_(dummy_exit) {
    type = TYPE_ROOT;

    first_ = head;
    stats_.add_with_key(first_, first_->id);

    first_->set_parent(this);
}

TextBuffer RootStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    return ExprProcessor::list_to_java(var_definitions_, indent, tracer).append(first_->to_java(indent, tracer));
}

// ---- SequenceStatement

SequenceStatement::SequenceStatement(const std::vector<Statement*>& lst) {
    type = TYPE_SEQUENCE;

    last_basic_type_ = lst.at(lst.size() - 1)->get_last_basic_type();

    for (Statement* st : lst) stats_.add_with_key(st, st->id);

    first_ = stats_.get(0);
}

SequenceStatement::SequenceStatement(Statement* head, Statement* tail) : SequenceStatement(std::vector<Statement*>{head, tail}) {
    const std::vector<StatEdge*> lst_succs = tail->get_successor_edges(STATEDGE_DIRECT_ALL);
    if (!lst_succs.empty()) {
        StatEdge* edge = lst_succs[0];
        if (edge->get_type() == StatEdge::TYPE_REGULAR && edge->get_destination() != head) post_ = edge->get_destination();
    }
}

Statement* SequenceStatement::is_head2_block(Statement* head) {
    if (head->get_last_basic_type() != LASTBASICTYPE_GENERAL) return nullptr;

    StatEdge* edge = nullptr;
    const std::vector<StatEdge*> lst_succs = head->get_successor_edges(STATEDGE_DIRECT_ALL);
    if (!lst_succs.empty()) edge = lst_succs[0];

    if (edge != nullptr && edge->get_type() == StatEdge::TYPE_REGULAR) {
        Statement* stat = edge->get_destination();

        if (stat != head && stat->get_predecessor_edges(StatEdge::TYPE_REGULAR).size() == 1 && !stat->is_monitor_enter()) {
            if (stat->get_last_basic_type() == LASTBASICTYPE_GENERAL) {
                if (DecHelper::check_statement_exceptions({head, stat})) return make<SequenceStatement>(head, stat);
            }
        }
    }

    return nullptr;
}

TextBuffer SequenceStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;
    const bool islabeled = is_labeled();

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    if (islabeled) {
        buf.append_indent(indent++).append("label").append(std::to_string(id)).append(": {").append_line_separator();
        tracer.increment_current_source_line();
    }

    bool notempty = false;

    for (std::size_t i = 0; i < stats_.size(); i++) {
        Statement* st = stats_[i];

        if (i > 0 && notempty) {
            buf.append_line_separator();
            tracer.increment_current_source_line();
        }

        TextBuffer str = ExprProcessor::jmp_wrapper(st, indent, false, tracer);
        buf.append(str);

        notempty = !str.contains_only_whitespaces();
    }

    if (islabeled) {
        buf.append_indent(indent - 1).append("}").append_line_separator();
        tracer.increment_current_source_line();
    }

    return buf;
}

Statement* SequenceStatement::get_simple_copy() { return make<SequenceStatement>(); }

}  // namespace cb::java::decompiler
