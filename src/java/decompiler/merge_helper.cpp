// MergeHelper, ClearStructHelper and new_basic_block: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

namespace {
struct OwnedBlock : ArenaObject {
    BasicBlock block;
};

BasicBlockStatement* new_empty_block_statement() {
    BasicBlockStatement* bstat = make<BasicBlockStatement>(
        new_basic_block(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER)));
    bstat->set_exprents(std::vector<Exprent*>{});
    return bstat;
}
}  // namespace

BasicBlock* new_basic_block(int id) {
    OwnedBlock* owned = make<OwnedBlock>();
    owned->block.id = id;
    return &owned->block;
}

void ClearStructHelper::clear_statements(RootStatement* root) {
    std::deque<Statement*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Statement* stat = stack.front();
        stack.pop_front();

        stat->clear_temp_information();

        for (Statement* st : stat->get_stats()) stack.push_back(st);
    }
}

void MergeHelper::enhance_loops(Statement* root) {
    while (enhance_loops_rec(root)) {
    }

    SequenceHelper::condense_sequences(root);
}

bool MergeHelper::enhance_loops_rec(Statement* stat) {
    bool res = false;

    for (Statement* st : stat->get_stats()) {
        if (!st->get_exprents()) {
            res |= enhance_loops_rec(st);
        }
    }

    if (stat->type == Statement::TYPE_DO) {
        res |= enhance_loop(static_cast<DoStatement*>(stat));
    }

    return res;
}

bool MergeHelper::enhance_loop(DoStatement* stat) {
    int oldloop = stat->get_looptype();

    switch (oldloop) {
        case DoStatement::LOOP_DO:
            // identify a while loop
            if (match_while(stat)) {
                // identify a for loop - subtype of while
                match_for(stat);
            } else {
                // identify a do{}while loop
                match_do_while(stat);
            }
            break;
        case DoStatement::LOOP_WHILE:
            match_for(stat);
    }

    return stat->get_looptype() != oldloop;
}

bool MergeHelper::match_do_while(DoStatement* stat) {
    // search for an if condition at the end of the loop
    Statement* last = stat->get_first();
    while (last->type == Statement::TYPE_SEQUENCE) {
        last = last->get_stats().last();
    }

    if (last->type == Statement::TYPE_IF) {
        IfStatement* lastif = static_cast<IfStatement*>(last);
        if (lastif->iftype == IfStatement::IFTYPE_IF && lastif->get_ifstat() == nullptr) {
            StatEdge* ifedge = lastif->get_if_edge();
            StatEdge* elseedge = at(lastif->get_all_successor_edges(), 0);

            if ((ifedge->get_type() == StatEdge::TYPE_BREAK && elseedge->get_type() == StatEdge::TYPE_CONTINUE && elseedge->closure == stat &&
                 is_direct_path(stat, ifedge->get_destination())) ||
                (ifedge->get_type() == StatEdge::TYPE_CONTINUE && elseedge->get_type() == StatEdge::TYPE_BREAK && ifedge->closure == stat &&
                 is_direct_path(stat, elseedge->get_destination()))) {
                LinkedSet<Statement*> set = stat->get_neighbours_set(StatEdge::TYPE_CONTINUE, Statement::DIRECTION_BACKWARD);
                set.remove(last);

                if (!set.empty()) {
                    return false;
                }

                stat->set_looptype(DoStatement::LOOP_DOWHILE);

                IfExprent* ifexpr = static_cast<IfExprent*>(lastif->get_headexprent()->copy());
                if (ifedge->get_type() == StatEdge::TYPE_BREAK) {
                    ifexpr->negate_if();
                }
                stat->set_condition_exprent(ifexpr->get_condition());
                lastif->get_first()->remove_successor(ifedge);
                lastif->remove_successor(elseedge);

                // remove empty if
                if (exprents_of(lastif->get_first()).empty()) {
                    remove_last_empty_statement(stat, lastif);
                } else {
                    lastif->set_exprents(exprents_of(lastif->get_first()));

                    StatEdge* newedge = make<StatEdge>(StatEdge::TYPE_CONTINUE, lastif, stat);
                    lastif->add_successor(newedge);
                    stat->add_labeled_edge(newedge);
                }

                if (stat->get_all_successor_edges().empty()) {
                    StatEdge* edge = elseedge->get_type() == StatEdge::TYPE_CONTINUE ? ifedge : elseedge;

                    edge->set_source(stat);
                    if (edge->closure == stat) {
                        edge->closure = stat->get_parent();
                    }
                    stat->add_successor(edge);
                }

                return true;
            }
        }
    }
    return false;
}

bool MergeHelper::match_while(DoStatement* stat) {
    // search for an if condition at the entrance of the loop
    Statement* first = stat->get_first();
    while (first->type == Statement::TYPE_SEQUENCE) {
        first = first->get_first();
    }

    // found an if statement
    if (first->type == Statement::TYPE_IF) {
        IfStatement* firstif = static_cast<IfStatement*>(first);

        if (exprents_of(firstif->get_first()).empty()) {
            if (firstif->iftype == IfStatement::IFTYPE_IF) {
                if (firstif->get_ifstat() == nullptr) {
                    StatEdge* ifedge = firstif->get_if_edge();
                    if (is_direct_path(stat, ifedge->get_destination())) {
                        // exit condition identified
                        stat->set_looptype(DoStatement::LOOP_WHILE);

                        // negate condition (while header)
                        IfExprent* ifexpr = static_cast<IfExprent*>(firstif->get_headexprent()->copy());
                        ifexpr->negate_if();
                        stat->set_condition_exprent(ifexpr->get_condition());

                        // remove edges
                        firstif->get_first()->remove_successor(ifedge);
                        firstif->remove_successor(at(firstif->get_all_successor_edges(), 0));

                        if (stat->get_all_successor_edges().empty()) {
                            ifedge->set_source(stat);
                            if (ifedge->closure == stat) {
                                ifedge->closure = stat->get_parent();
                            }
                            stat->add_successor(ifedge);
                        }

                        // remove empty if statement as it is now part of the loop
                        if (firstif == stat->get_first()) {
                            BasicBlockStatement* bstat = new_empty_block_statement();
                            stat->replace_statement(firstif, bstat);
                        } else {
                            // precondition: sequence must contain more than one statement!
                            Statement* sequence = firstif->get_parent();
                            sequence->get_stats().remove_with_key(firstif->id);
                            sequence->set_first(sequence->get_stats().get(0));
                        }

                        return true;
                    }
                } else {
                StatEdge* elseedge = at(firstif->get_all_successor_edges(), 0);
                if (is_direct_path(stat, elseedge->get_destination())) {
                    // exit condition identified
                    stat->set_looptype(DoStatement::LOOP_WHILE);

                    // no need to negate the while condition
                    stat->set_condition_exprent(static_cast<IfExprent*>(firstif->get_headexprent()->copy())->get_condition());

                    // remove edges
                    StatEdge* ifedge = firstif->get_if_edge();
                    firstif->get_first()->remove_successor(ifedge);
                    firstif->remove_successor(elseedge);

                    if (stat->get_all_successor_edges().empty()) {
                        elseedge->set_source(stat);
                        if (elseedge->closure == stat) {
                            elseedge->closure = stat->get_parent();
                        }
                        stat->add_successor(elseedge);
                    }

                    if (firstif->get_ifstat() == nullptr) {
                        BasicBlockStatement* bstat = new_empty_block_statement();

                        ifedge->set_source(bstat);
                        bstat->add_successor(ifedge);

                        stat->replace_statement(firstif, bstat);
                    } else {
                        // replace the if statement with its content
                        first->get_parent()->replace_statement(first, firstif->get_ifstat());

                        // lift closures
                        for (StatEdge* prededge : elseedge->get_destination()->get_predecessor_edges(StatEdge::TYPE_BREAK)) {
                            if (stat->contains_statement_strict(prededge->closure)) {
                                stat->add_labeled_edge(prededge);
                            }
                        }

                        LabelHelper::low_closures(stat);
                    }

                    return true;
                }
                }
            }
        }
    }
    return false;
}

bool MergeHelper::is_direct_path(Statement* stat, Statement* endstat) {
    LinkedSet<Statement*> set_stat = stat->get_neighbours_set(Statement::STATEDGE_DIRECT_ALL, Statement::DIRECTION_FORWARD);
    if (set_stat.empty()) {
        Statement* parent = stat->get_parent();
        if (parent == nullptr) {
            return false;
        }
        switch (parent->type) {
            case Statement::TYPE_ROOT:
                return endstat->type == Statement::TYPE_DUMMYEXIT;
            case Statement::TYPE_DO:
                return endstat == parent;
            case Statement::TYPE_SWITCH: {
                SwitchStatement* swst = static_cast<SwitchStatement*>(parent);
                for (int i = 0; i < static_cast<int>(swst->get_case_statements().size()) - 1; i++) {
                    Statement* stt = swst->get_case_statements()[static_cast<std::size_t>(i)];
                    if (stt == stat) {
                        Statement* stnext = swst->get_case_statements()[static_cast<std::size_t>(i) + 1];

                        if (stnext->get_exprents() && stnext->get_exprents()->empty()) {
                            stnext = at(stnext->get_all_successor_edges(), 0)->get_destination();
                        }
                        return endstat == stnext;
                    }
                }
            }
                [[fallthrough]];
            default:
                return is_direct_path(parent, endstat);
        }
    }
    return set_stat.contains(endstat);
}

bool MergeHelper::match_for(DoStatement* stat) {
    Exprent* last_do_exprent = nullptr;
    Exprent* init_do_exprent = nullptr;
    Statement* last_data = nullptr;
    Statement* pre_data = nullptr;

    // get last exprent
    last_data = get_last_direct_data(stat->get_first());
    if (last_data == nullptr || exprents_of(last_data).empty()) {
        return false;
    }

    std::vector<Exprent*>& lst_expr = exprents_of(last_data);
    last_do_exprent = lst_expr.back();

    bool issingle = false;
    if (lst_expr.size() == 1) {  // single exprent
        if (last_data->get_all_predecessor_edges().size() > 1) {  // break edges
            issingle = true;
        }
    }

    bool haslast = issingle || (last_do_exprent->type == Exprent::EXPRENT_ASSIGNMENT || last_do_exprent->type == Exprent::EXPRENT_FUNCTION);

    if (!haslast) {
        return false;
    }

    bool hasinit = false;

    // search for an initializing exprent
    Statement* current = stat;
    while (true) {
        Statement* parent = current->get_parent();
        if (parent == nullptr) {
            break;
        }

        if (parent->type == Statement::TYPE_SEQUENCE) {
            if (current == parent->get_first()) {
                current = parent;
            } else {
                pre_data = at(current->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD), 0);
                pre_data = get_last_direct_data(pre_data);
                if (pre_data != nullptr && !exprents_of(pre_data).empty()) {
                    init_do_exprent = exprents_of(pre_data).back();
                    if (init_do_exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                        hasinit = true;
                    }
                }
                break;
            }
        } else {
            break;
        }
    }

    if ((hasinit && haslast) || issingle) {  // FIXME: issingle sufficient?
        LinkedSet<Statement*> set = stat->get_neighbours_set(StatEdge::TYPE_CONTINUE, Statement::DIRECTION_BACKWARD);
        set.remove(last_data);

        if (!set.empty()) {
            return false;
        }

        stat->set_looptype(DoStatement::LOOP_FOR);
        if (hasinit) {
            std::vector<Exprent*>& pre = exprents_of(pre_data);
            Exprent* removed = pre.back();
            pre.pop_back();
            stat->set_init_exprent(removed);
        }
        std::vector<Exprent*>& last = exprents_of(last_data);
        Exprent* removed = last.back();
        last.pop_back();
        stat->set_inc_exprent(removed);
    }

    if (exprents_of(last_data).empty()) {
        std::vector<StatEdge*> lst = last_data->get_all_successor_edges();
        if (!lst.empty()) {
            last_data->remove_successor(lst[0]);
        }
        remove_last_empty_statement(stat, last_data);
    }

    return true;
}

void MergeHelper::remove_last_empty_statement(DoStatement* dostat, Statement* stat) {
    if (stat == dostat->get_first()) {
        BasicBlockStatement* bstat = new_empty_block_statement();
        dostat->replace_statement(stat, bstat);
    } else {
        for (StatEdge* edge : stat->get_all_predecessor_edges()) {
            edge->get_source()->change_edge_type(Statement::DIRECTION_FORWARD, edge, StatEdge::TYPE_CONTINUE);

            stat->remove_predecessor(edge);
            edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, dostat);
            dostat->add_predecessor(edge);

            dostat->add_labeled_edge(edge);
        }

        // parent is a sequence statement
        stat->get_parent()->get_stats().remove_with_key(stat->id);
    }
}

Statement* MergeHelper::get_last_direct_data(Statement* stat) {
    if (stat->get_exprents()) {
        return stat;
    }

    switch (stat->type) {
        case Statement::TYPE_SEQUENCE:
            for (int i = static_cast<int>(stat->get_stats().size()) - 1; i >= 0; i--) {
                Statement* tmp = get_last_direct_data(stat->get_stats()[static_cast<std::size_t>(i)]);
                if (tmp == nullptr || !exprents_of(tmp).empty()) {
                    return tmp;
                }
            }
    }
    return nullptr;
}

}  // namespace cb::java::decompiler
