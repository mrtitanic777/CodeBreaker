// EliminateLoopsHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE
// in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <set>

#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool EliminateLoopsHelper::eliminate_loops(Statement* root) {
    bool ret = eliminate_loops_rec(root);

    if (ret) {
        SequenceHelper::condense_sequences(root);

        std::set<int> set_reordered_ifs;

        SimplifyExprentsHelper sehelper(false);
        while (sehelper.simplify_stack_vars_statement(root, set_reordered_ifs, nullptr, nullptr)) {
            SequenceHelper::condense_sequences(root);
        }
    }

    return ret;
}

bool EliminateLoopsHelper::eliminate_loops_rec(Statement* stat) {
    for (Statement* st : stat->get_stats()) {
        if (eliminate_loops_rec(st)) {
            return true;
        }
    }

    if (stat->type == Statement::TYPE_DO && is_loop_redundant(static_cast<DoStatement*>(stat))) {
        return true;
    }

    return false;
}

bool EliminateLoopsHelper::is_loop_redundant(DoStatement* loop) {
    if (loop->get_looptype() != DoStatement::LOOP_DO) {
        return false;
    }

    // get parent loop if exists
    Statement* parentloop = loop->get_parent();
    while (parentloop != nullptr && parentloop->type != Statement::TYPE_DO) {
        parentloop = parentloop->get_parent();
    }

    if (parentloop == nullptr || parentloop->get_basichead() != loop->get_basichead()) {
        return false;
    }

    // collect relevant break edges
    std::vector<StatEdge*> lst_break_edges;
    for (StatEdge* edge : loop->get_label_edges()) {
        if (edge->get_type() == StatEdge::TYPE_BREAK) {  // all break edges are explicit because of LOOP_DO type
            lst_break_edges.push_back(edge);
        }
    }

    Statement* loopcontent = loop->get_first();

    bool firstok = loopcontent->get_all_successor_edges().empty();
    if (!firstok) {
        StatEdge* edge = at(loopcontent->get_all_successor_edges(), 0);
        firstok = (edge->closure == loop && edge->get_type() == StatEdge::TYPE_BREAK);
        if (firstok) {
            auto it = std::find(lst_break_edges.begin(), lst_break_edges.end(), edge);
            if (it != lst_break_edges.end()) lst_break_edges.erase(it);
        }
    }

    if (!lst_break_edges.empty()) {
        if (firstok) {
            std::map<int, bool> stat_labeled;
            std::vector<Statement*> lst_edge_closures;

            for (StatEdge* edge : lst_break_edges) {
                Statement* minclosure = LowBreakHelper::get_min_closure(loopcontent, edge->get_source());
                lst_edge_closures.push_back(minclosure);
            }

            int precount = loop->is_labeled() ? 1 : 0;
            for (Statement* st : lst_edge_closures) {
                if (!stat_labeled.contains(st->id)) {
                    bool btemp = st->is_labeled();
                    precount += btemp ? 1 : 0;
                    stat_labeled[st->id] = btemp;
                }
            }

            for (std::size_t i = 0; i < lst_break_edges.size(); i++) {
                Statement* st = lst_edge_closures[i];
                bool labeled = LowBreakHelper::is_break_edge_labeled(lst_break_edges[i]->get_source(), st);
                stat_labeled[st->id] = labeled | stat_labeled.at(st->id);
            }

            for (std::size_t i = 0; i < lst_break_edges.size(); i++) {
                lst_edge_closures[i] = get_max_break_lift(lst_edge_closures[i], lst_break_edges[i], stat_labeled, loop);
            }

            stat_labeled.clear();
            for (Statement* st : lst_edge_closures) {
                stat_labeled[st->id] = st->is_labeled();
            }

            for (std::size_t i = 0; i < lst_break_edges.size(); i++) {
                Statement* st = lst_edge_closures[i];
                bool labeled = LowBreakHelper::is_break_edge_labeled(lst_break_edges[i]->get_source(), st);
                stat_labeled[st->id] = labeled | stat_labeled.at(st->id);
            }

            int postcount = 0;
            for (const auto& entry : stat_labeled) {
                postcount += entry.second ? 1 : 0;
            }

            if (precount <= postcount) {
                return false;
            }
            for (std::size_t i = 0; i < lst_break_edges.size(); i++) {
                lst_edge_closures[i]->add_labeled_edge(lst_break_edges[i]);
            }
        } else {
            return false;
        }
    }

    eliminate_loop(loop, parentloop);

    return true;
}

Statement* EliminateLoopsHelper::get_max_break_lift(Statement* stat, StatEdge* edge, std::map<int, bool>& stat_labeled, Statement* max) {
    Statement* closure = stat;
    Statement* newclosure = stat;

    while ((newclosure = get_next_break_lift(newclosure, edge, stat_labeled, max)) != nullptr) {
        closure = newclosure;
    }

    return closure;
}

Statement* EliminateLoopsHelper::get_next_break_lift(Statement* stat, StatEdge* edge, std::map<int, bool>& stat_labeled, Statement* max) {
    Statement* closure = stat->get_parent();

    while (closure != nullptr && closure != max && !closure->contains_statement_strict(edge->get_destination())) {
        bool edge_labeled = LowBreakHelper::is_break_edge_labeled(edge->get_source(), closure);
        bool stat_labeled_value = stat_labeled.contains(closure->id) ? stat_labeled.at(closure->id) : closure->is_labeled();

        if (stat_labeled_value || !edge_labeled) {
            return closure;
        }

        closure = closure->get_parent();
    }

    return nullptr;
}

void EliminateLoopsHelper::eliminate_loop(Statement* loop, Statement* parentloop) {
    // move continue edges to the parent loop
    std::vector<StatEdge*> lst = loop->get_label_edges().items();
    for (StatEdge* edge : lst) {
        loop->remove_predecessor(edge);
        edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, parentloop);
        parentloop->add_predecessor(edge);

        parentloop->add_labeled_edge(edge);
    }

    // remove the last break edge, if exists
    Statement* loopcontent = loop->get_first();
    if (!loopcontent->get_all_successor_edges().empty()) {
        loopcontent->remove_successor(at(loopcontent->get_all_successor_edges(), 0));
    }

    // replace loop with its content
    loop->get_parent()->replace_statement(loop, loopcontent);
}

}  // namespace cb::java::decompiler
