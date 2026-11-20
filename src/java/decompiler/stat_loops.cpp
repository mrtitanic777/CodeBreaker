// stat_loops.cpp -- DoStatement, SwitchStatement, SynchronizedStatement. Ported from Fernflower (de.fernflower.modules.decompiler.stats), Copyright
// 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

#include "java/decompiler/collectors.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

// ---- DoStatement

DoStatement::DoStatement(Statement* head) : DoStatement() {
    first_ = head;
    stats_.add_with_key(first_, first_->id);
}

Statement* DoStatement::is_head(Statement* head) {
    if (head->get_last_basic_type() == LASTBASICTYPE_GENERAL && !head->is_monitor_enter()) {
        StatEdge* edge = nullptr;
        const std::vector<StatEdge*> lst_succs = head->get_successor_edges(STATEDGE_DIRECT_ALL);
        if (!lst_succs.empty()) edge = lst_succs[0];

        if (edge != nullptr && edge->get_type() == StatEdge::TYPE_REGULAR && edge->get_destination() == head) {
            return make<DoStatement>(head);
        }

        if (head->type != TYPE_DO && (edge == nullptr || edge->get_type() != StatEdge::TYPE_REGULAR) &&
            head->get_continue_set().contains(head->get_basichead())) {
            return make<DoStatement>(head);
        }
    }

    return nullptr;
}

TextBuffer DoStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    if (is_labeled()) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    switch (looptype_) {
        case LOOP_DO:
            buf.append_indent(indent).append("while(true) {").append_line_separator();
            tracer.increment_current_source_line();
            buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));
            buf.append_indent(indent).append("}").append_line_separator();
            tracer.increment_current_source_line();
            break;
        case LOOP_DOWHILE:
            buf.append_indent(indent).append("do {").append_line_separator();
            tracer.increment_current_source_line();
            buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));
            buf.append_indent(indent).append("} while(").append(condition_exprent_[0]->to_java(indent, tracer)).append(");").append_line_separator();
            tracer.increment_current_source_line();
            break;
        case LOOP_WHILE:
            buf.append_indent(indent).append("while(").append(condition_exprent_[0]->to_java(indent, tracer)).append(") {").append_line_separator();
            tracer.increment_current_source_line();
            buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));
            buf.append_indent(indent).append("}").append_line_separator();
            tracer.increment_current_source_line();
            break;
        case LOOP_FOR:
            buf.append_indent(indent).append("for(");
            if (init_exprent_[0] != nullptr) buf.append(init_exprent_[0]->to_java(indent, tracer));
            buf.append("; ")
                .append(condition_exprent_[0]->to_java(indent, tracer))
                .append("; ")
                .append(inc_exprent_[0]->to_java(indent, tracer))
                .append(") {")
                .append_line_separator();
            tracer.increment_current_source_line();
            buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));
            buf.append_indent(indent).append("}").append_line_separator();
            tracer.increment_current_source_line();
            break;
        default:
            break;
    }

    return buf;
}

std::vector<SeqObject> DoStatement::get_sequential_objects() {
    std::vector<SeqObject> lst;

    switch (looptype_) {
        case LOOP_FOR:
            if (get_init_exprent() != nullptr) lst.push_back({nullptr, get_init_exprent()});
            [[fallthrough]];
        case LOOP_WHILE:
            lst.push_back({nullptr, get_condition_exprent()});
            break;
        default:
            break;
    }

    lst.push_back({first_, nullptr});

    switch (looptype_) {
        case LOOP_DOWHILE:
            lst.push_back({nullptr, get_condition_exprent()});
            break;
        case LOOP_FOR:
            lst.push_back({nullptr, get_inc_exprent()});
            break;
        default:
            break;
    }

    return lst;
}

void DoStatement::replace_exprent(Exprent* oldexpr, Exprent* newexpr) {
    if (init_exprent_[0] == oldexpr) init_exprent_[0] = newexpr;
    if (condition_exprent_[0] == oldexpr) condition_exprent_[0] = newexpr;
    if (inc_exprent_[0] == oldexpr) inc_exprent_[0] = newexpr;
}

Statement* DoStatement::get_simple_copy() { return make<DoStatement>(); }

// ---- SwitchStatement

SwitchStatement::SwitchStatement(Statement* head, Statement* poststat) : SwitchStatement() {
    first_ = head;
    stats_.add_with_key(head, head->id);

    LinkedSet<Statement*> lst_nodes = LinkedSet<Statement*>::of(head->get_neighbours(StatEdge::TYPE_REGULAR, DIRECTION_FORWARD));

    // the post statement of the switch
    if (poststat != nullptr) {
        post_ = poststat;
        lst_nodes.remove(post_);
    }

    default_edge_ = head->get_successor_edges(STATEDGE_DIRECT_ALL).at(0);

    for (Statement* st : lst_nodes) stats_.add_with_key(st, st->id);
}

Statement* SwitchStatement::is_head(Statement* head) {
    if (head->type == TYPE_BASICBLOCK && head->get_last_basic_type() == LASTBASICTYPE_SWITCH) {
        std::vector<Statement*> lst;
        if (DecHelper::is_choice_statement(head, lst)) {
            Statement* post = lst.front();
            lst.erase(lst.begin());

            for (Statement* st : lst) {
                if (st->is_monitor_enter()) return nullptr;
            }

            if (DecHelper::check_statement_exceptions(lst)) return make<SwitchStatement>(head, post);
        }
    }

    return nullptr;
}

TextBuffer SwitchStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    buf.append(first_->to_java(indent, tracer));

    if (is_labeled()) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append_indent(indent).append(headexprent_[0]->to_java(indent, tracer)).append(" {").append_line_separator();
    tracer.increment_current_source_line();

    const VarType switch_type = headexprent_[0]->get_expr_type();

    for (std::size_t i = 0; i < case_statements_.size(); i++) {
        Statement* stat = case_statements_[i];
        std::vector<StatEdge*>& edges = case_edges_[i];
        std::vector<ConstExprent*>& values = case_values_[i];

        for (std::size_t j = 0; j < edges.size(); j++) {
            if (edges[j] == default_edge_) {
                buf.append_indent(indent).append("default:").append_line_separator();
                tracer.increment_current_source_line();
            } else {
                ConstExprent* value = static_cast<ConstExprent*>(values[j]->copy());
                value->set_const_type(switch_type);

                buf.append_indent(indent).append("case ").append(value->to_java(indent, tracer)).append(":").append_line_separator();
                tracer.increment_current_source_line();
            }
        }

        buf.append(ExprProcessor::jmp_wrapper(stat, indent + 1, false, tracer));
    }

    buf.append_indent(indent).append("}").append_line_separator();
    tracer.increment_current_source_line();

    return buf;
}

void SwitchStatement::init_exprents() {
    std::vector<Exprent*>& lst = first_->get_exprents().value();
    SwitchExprent* swexpr = static_cast<SwitchExprent*>(lst.back());
    lst.pop_back();
    swexpr->set_case_values(case_values_);

    headexprent_[0] = swexpr;
}

std::vector<SeqObject> SwitchStatement::get_sequential_objects() {
    std::vector<SeqObject> lst = Statement::get_sequential_objects();

    lst.insert(lst.begin() + 1, SeqObject{nullptr, headexprent_[0]});

    return lst;
}

void SwitchStatement::replace_exprent(Exprent* oldexpr, Exprent* newexpr) {
    if (headexprent_[0] == oldexpr) headexprent_[0] = newexpr;
}

void SwitchStatement::replace_statement(Statement* oldstat, Statement* newstat) {
    for (std::size_t i = 0; i < case_statements_.size(); i++) {
        if (case_statements_[i] == oldstat) case_statements_[i] = newstat;
    }

    Statement::replace_statement(oldstat, newstat);
}

Statement* SwitchStatement::get_simple_copy() { return make<SwitchStatement>(); }

void SwitchStatement::init_simple_copy() {
    first_ = stats_.get(0);
    default_edge_ = first_->get_successor_edges(STATEDGE_DIRECT_ALL).at(0);

    sort_edges_and_nodes();
}

void SwitchStatement::sort_edges_and_nodes() {
    std::unordered_map<StatEdge*, int> map_edge_index;

    const std::vector<StatEdge*> lst_first_succs = first_->get_successor_edges(STATEDGE_DIRECT_ALL);
    for (std::size_t i = 0; i < lst_first_succs.size(); i++) {
        map_edge_index[lst_first_succs[i]] = i == 0 ? static_cast<int>(lst_first_succs.size()) : static_cast<int>(i);
    }

    BasicBlockStatement* bbstat = static_cast<BasicBlockStatement*>(first_);
    const std::vector<int> values = bbstat->get_block()->last_instruction()->values;

    std::vector<Statement*> nodes;
    std::vector<std::vector<int>> edges;

    // collect regular edges of 'default' branch
    for (std::size_t i = 1; i < stats_.size(); i++) {
        Statement* stat = stats_[i];

        std::vector<int> lst;
        for (StatEdge* edge : stat->get_predecessor_edges(StatEdge::TYPE_REGULAR)) {
            if (edge->get_source() == first_) lst.push_back(map_edge_index.at(edge));
        }

        std::sort(lst.begin(), lst.end());

        nodes.push_back(stat);
        edges.push_back(lst);
    }

    // collect exit edges
    std::vector<StatEdge*> lst_exit_edges = first_->get_successor_edges(StatEdge::TYPE_BREAK | StatEdge::TYPE_CONTINUE);
    while (!lst_exit_edges.empty()) {
        StatEdge* edge = lst_exit_edges[0];

        std::vector<int> lst;
        for (int i = static_cast<int>(lst_exit_edges.size()) - 1; i >= 0; i--) {
            StatEdge* edge_temp = lst_exit_edges[static_cast<std::size_t>(i)];

            if (edge_temp->get_destination() == edge->get_destination() && edge_temp->get_type() == edge->get_type()) {
                lst.push_back(map_edge_index.at(edge_temp));
                lst_exit_edges.erase(lst_exit_edges.begin() + i);
            }
        }

        std::sort(lst.begin(), lst.end());

        nodes.push_back(nullptr);
        edges.push_back(lst);
    }

    // sort edges (bubblesort)
    for (int i = 0; i < static_cast<int>(edges.size()) - 1; i++) {
        for (int j = static_cast<int>(edges.size()) - 1; j > i; j--) {
            if (edges[static_cast<std::size_t>(j - 1)].at(0) > edges[static_cast<std::size_t>(j)].at(0)) {
                std::swap(edges[static_cast<std::size_t>(j)], edges[static_cast<std::size_t>(j - 1)]);
                std::swap(nodes[static_cast<std::size_t>(j)], nodes[static_cast<std::size_t>(j - 1)]);
            }
        }
    }

    // sort statement cases
    for (int index = 0; index < static_cast<int>(nodes.size()); index++) {
        Statement* stat = nodes[static_cast<std::size_t>(index)];

        if (stat != nullptr) {
            LinkedSet<Statement*> set_preds = LinkedSet<Statement*>::of(stat->get_neighbours(StatEdge::TYPE_REGULAR, DIRECTION_BACKWARD));
            set_preds.remove(first_);

            if (!set_preds.empty()) {
                Statement* pred = set_preds.first();   // assumption: at most one predecessor node besides the head. May not hold true for obfuscated code.
                for (int j = 0; j < static_cast<int>(nodes.size()); j++) {
                    if (j != (index - 1) && nodes[static_cast<std::size_t>(j)] == pred) {
                        const std::vector<int> moved_edges = edges[static_cast<std::size_t>(index)];
                        nodes.insert(nodes.begin() + j + 1, stat);
                        edges.insert(edges.begin() + j + 1, moved_edges);

                        if (j > index) {
                            nodes.erase(nodes.begin() + index);
                            edges.erase(edges.begin() + index);
                            index--;
                        } else {
                            nodes.erase(nodes.begin() + index + 1);
                            edges.erase(edges.begin() + index + 1);
                        }

                        break;
                    }
                }
            }
        }
    }

    // translate indices back into edges
    std::vector<std::vector<StatEdge*>> lst_edges;
    std::vector<std::vector<ConstExprent*>> lst_values;

    for (const std::vector<int>& lst : edges) {
        std::vector<StatEdge*> lste;
        std::vector<ConstExprent*> lstv;

        const std::vector<StatEdge*> lst_succs = first_->get_successor_edges(STATEDGE_DIRECT_ALL);
        for (const int in : lst) {
            const int index = in == static_cast<int>(lst_succs.size()) ? 0 : in;

            lste.push_back(lst_succs.at(static_cast<std::size_t>(index)));
            lstv.push_back(index == 0 ? nullptr : make<ConstExprent>(values.at(static_cast<std::size_t>(index - 1)), false, Offsets{}));
        }

        lst_edges.push_back(lste);
        lst_values.push_back(lstv);
    }

    // replace null statements with dummy basic blocks
    for (std::size_t i = 0; i < nodes.size(); i++) {
        if (nodes[i] == nullptr) {
            auto block = std::make_unique<BasicBlock>();
            block->id = DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER);
            BasicBlockStatement* bstat = BasicBlockStatement::make_owning(std::move(block));

            StatEdge* sample_edge = lst_edges[i].at(0);
            bstat->add_successor(make<StatEdge>(sample_edge->get_type(), bstat, sample_edge->get_destination(), sample_edge->closure));

            for (StatEdge* edge : lst_edges[i]) {
                edge->get_source()->change_edge_type(DIRECTION_FORWARD, edge, StatEdge::TYPE_REGULAR);

                edge->closure->get_label_edges().remove(edge);

                edge->get_destination()->remove_predecessor(edge);
                edge->get_source()->change_edge_node(DIRECTION_FORWARD, edge, bstat);
                bstat->add_predecessor(edge);
            }

            nodes[i] = bstat;
            stats_.add_with_key(bstat, bstat->id);
            bstat->set_parent(this);
        }
    }

    case_statements_ = nodes;
    case_edges_ = lst_edges;
    case_values_ = lst_values;
}

// ---- SynchronizedStatement

SynchronizedStatement::SynchronizedStatement(Statement* head, Statement* body, Statement* exc) : SynchronizedStatement() {
    first_ = head;

    stats_.add_with_key(head, head->id);

    body_ = body;
    stats_.add_with_key(body, body->id);

    stats_.add_with_key(exc, exc->id);

    const std::vector<StatEdge*> lst_succs = body->get_successor_edges(STATEDGE_DIRECT_ALL);
    if (!lst_succs.empty()) {
        StatEdge* edge = lst_succs[0];
        if (edge->get_type() == StatEdge::TYPE_REGULAR) post_ = edge->get_destination();
    }
}

TextBuffer SynchronizedStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    buf.append(first_->to_java(indent, tracer));

    if (is_labeled()) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append_indent(indent).append(headexprent_[0]->to_java(indent, tracer)).append(" {").append_line_separator();
    tracer.increment_current_source_line();

    buf.append(ExprProcessor::jmp_wrapper(body_, indent + 1, false, tracer));

    buf.append_indent(indent).append("}").append_line_separator();
    tracer.increment_current_source_line();

    return buf;
}

void SynchronizedStatement::init_exprents() {
    std::vector<Exprent*>& lst = first_->get_exprents().value();
    headexprent_[0] = lst.back();
    lst.pop_back();
}

std::vector<SeqObject> SynchronizedStatement::get_sequential_objects() {
    std::vector<SeqObject> lst = Statement::get_sequential_objects();

    lst.insert(lst.begin() + 1, SeqObject{nullptr, headexprent_[0]});

    return lst;
}

void SynchronizedStatement::replace_exprent(Exprent* oldexpr, Exprent* newexpr) {
    if (headexprent_[0] == oldexpr) headexprent_[0] = newexpr;
}

void SynchronizedStatement::replace_statement(Statement* oldstat, Statement* newstat) {
    if (body_ == oldstat) body_ = newstat;

    Statement::replace_statement(oldstat, newstat);
}

void SynchronizedStatement::remove_exc() {
    Statement* exc = stats_.get(2);
    SequenceHelper::destroy_statement_content(exc, true);

    stats_.remove_with_key(exc->id);
}

Statement* SynchronizedStatement::get_simple_copy() { return make<SynchronizedStatement>(); }

void SynchronizedStatement::init_simple_copy() {
    first_ = stats_.get(0);
    body_ = stats_.get(1);
}

}  // namespace cb::java::decompiler
