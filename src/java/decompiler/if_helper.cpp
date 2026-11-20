// IfHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool IfHelper::merge_all_ifs(RootStatement* root) {
    std::set<int> set_reordered_ifs;
    bool res = merge_all_ifs_rec(root, set_reordered_ifs);

    if (res) {
        SequenceHelper::condense_sequences(root);
    }

    return res;
}

bool IfHelper::merge_all_ifs_rec(Statement* stat, std::set<int>& set_reordered_ifs) {
    bool res = false;

    if (!stat->get_exprents()) {
        while (true) {
            bool changed = false;

            for (Statement* st : stat->get_stats()) {
                res |= merge_all_ifs_rec(st, set_reordered_ifs);

                // collapse composed if's
                if ((changed = merge_ifs(st, set_reordered_ifs))) {
                    break;
                }
            }

            res |= changed;

            if (!changed) {
                break;
            }
        }
    }

    return res;
}

bool IfHelper::merge_ifs(Statement* statement, std::set<int>& set_reordered_ifs) {
    if (statement->type != Statement::TYPE_IF && statement->type != Statement::TYPE_SEQUENCE) {
        return false;
    }

    bool res = false;

    while (true) {
        bool updated = false;

        std::vector<Statement*> lst;
        if (statement->type == Statement::TYPE_IF) {
            lst.push_back(statement);
        } else {
            lst = statement->get_stats().items();
        }

        bool stsingle = (lst.size() == 1);

        for (Statement* stat : lst) {
            if (stat->type == Statement::TYPE_IF) {
                IfNode* rtnode = build_graph(static_cast<IfStatement*>(stat), stsingle);

                if (rtnode == nullptr) {
                    continue;
                }

                if ((updated = collapse_if_if(rtnode))) {
                    break;
                }

                if (!set_reordered_ifs.contains(stat->id)) {
                    if ((updated = collapse_if_else(rtnode))) {
                        break;
                    }

                    if ((updated = collapse_else(rtnode))) {
                        break;
                    }
                }

                if ((updated = reorder_if(static_cast<IfStatement*>(stat)))) {
                    set_reordered_ifs.insert(stat->id);
                    break;
                }
            }
        }

        if (!updated) {
            break;
        }

        res |= updated;
    }

    return res;
}

bool IfHelper::collapse_if_if(IfNode* rtnode) {
    if (at(rtnode->edgetypes, 0) == 0) {
        IfNode* ifbranch = at(rtnode->succs, 0);
        if (ifbranch->succs.size() == 2) {
            // if-if branch
            if (at(ifbranch->succs, 1)->value == at(rtnode->succs, 1)->value) {
                IfStatement* ifparent = static_cast<IfStatement*>(rtnode->value);
                IfStatement* ifchild = static_cast<IfStatement*>(ifbranch->value);
                Statement* ifinner = at(ifbranch->succs, 0)->value;

                if (exprents_of(ifchild->get_first()).empty()) {
                    ifparent->get_first()->remove_successor(ifparent->get_if_edge());
                    ifchild->remove_successor(at(ifchild->get_all_successor_edges(), 0));
                    ifparent->get_stats().remove_with_key(ifchild->id);

                    if (at(ifbranch->edgetypes, 0) == 1) {  // target null
                        ifparent->set_ifstat(nullptr);

                        StatEdge* ifedge = ifchild->get_if_edge();

                        ifchild->get_first()->remove_successor(ifedge);
                        ifedge->set_source(ifparent->get_first());

                        if (ifedge->closure == ifchild) {
                            ifedge->closure = nullptr;
                        }
                        ifparent->get_first()->add_successor(ifedge);

                        ifparent->set_if_edge(ifedge);
                    } else {
                        ifchild->get_first()->remove_successor(ifchild->get_if_edge());

                        StatEdge* ifedge = make<StatEdge>(StatEdge::TYPE_REGULAR, ifparent->get_first(), ifinner);
                        ifparent->get_first()->add_successor(ifedge);
                        ifparent->set_if_edge(ifedge);
                        ifparent->set_ifstat(ifinner);

                        ifparent->get_stats().add_with_key(ifinner, ifinner->id);
                        ifinner->set_parent(ifparent);

                        if (!ifinner->get_all_successor_edges().empty()) {
                            StatEdge* edge = ifinner->get_all_successor_edges()[0];
                            if (edge->closure == ifchild) {
                                edge->closure = nullptr;
                            }
                        }
                    }

                    // merge if conditions
                    IfExprent* statexpr = ifparent->get_headexprent();

                    std::vector<Exprent*> lst_operands;
                    lst_operands.push_back(statexpr->get_condition());
                    lst_operands.push_back(ifchild->get_headexprent()->get_condition());

                    statexpr->set_condition(make<FunctionExprent>(FunctionExprent::FUNCTION_CADD, lst_operands, Offsets{}));
                    statexpr->add_bytecode_offsets(ifchild->get_headexprent()->bytecode);

                    return true;
                }
            }
        }
    }

    return false;
}

bool IfHelper::collapse_if_else(IfNode* rtnode) {
    if (at(rtnode->edgetypes, 0) == 0) {
        IfNode* ifbranch = at(rtnode->succs, 0);
        if (ifbranch->succs.size() == 2) {
            // if-else branch
            if (at(ifbranch->succs, 0)->value == at(rtnode->succs, 1)->value) {
                IfStatement* ifparent = static_cast<IfStatement*>(rtnode->value);
                IfStatement* ifchild = static_cast<IfStatement*>(ifbranch->value);

                if (exprents_of(ifchild->get_first()).empty()) {
                    ifparent->get_first()->remove_successor(ifparent->get_if_edge());
                    ifchild->get_first()->remove_successor(ifchild->get_if_edge());
                    ifparent->get_stats().remove_with_key(ifchild->id);

                    if (at(ifbranch->edgetypes, 1) == 1 && at(ifbranch->edgetypes, 0) == 1) {  // target null
                        ifparent->set_ifstat(nullptr);

                        StatEdge* ifedge = at(ifchild->get_all_successor_edges(), 0);

                        ifchild->remove_successor(ifedge);
                        ifedge->set_source(ifparent->get_first());
                        ifparent->get_first()->add_successor(ifedge);

                        ifparent->set_if_edge(ifedge);
                    } else {
                        throw std::runtime_error("inconsistent if structure!");
                    }

                    // merge if conditions
                    IfExprent* statexpr = ifparent->get_headexprent();

                    std::vector<Exprent*> lst_operands;
                    lst_operands.push_back(statexpr->get_condition());
                    lst_operands.push_back(make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, ifchild->get_headexprent()->get_condition(), Offsets{}));
                    statexpr->set_condition(make<FunctionExprent>(FunctionExprent::FUNCTION_CADD, lst_operands, Offsets{}));
                    statexpr->add_bytecode_offsets(ifchild->get_headexprent()->bytecode);

                    return true;
                }
            }
        }
    }

    return false;
}

bool IfHelper::collapse_else(IfNode* rtnode) {
    if (at(rtnode->edgetypes, 1) == 0) {
        IfNode* elsebranch = at(rtnode->succs, 1);
        if (elsebranch->succs.size() == 2) {
            // else-if or else-else branch
            int path = at(elsebranch->succs, 1)->value == at(rtnode->succs, 0)->value
                           ? 2
                           : (at(elsebranch->succs, 0)->value == at(rtnode->succs, 0)->value ? 1 : 0);

            if (path > 0) {
                IfStatement* firstif = static_cast<IfStatement*>(rtnode->value);
                IfStatement* secondif = static_cast<IfStatement*>(elsebranch->value);
                Statement* parent = firstif->get_parent();

                if (exprents_of(secondif->get_first()).empty()) {
                    firstif->get_first()->remove_successor(firstif->get_if_edge());

                    // remove first if
                    firstif->remove_all_successors(secondif);

                    for (StatEdge* edge : firstif->get_all_predecessor_edges()) {
                        if (!firstif->contains_statement_strict(edge->get_source())) {
                            firstif->remove_predecessor(edge);
                            edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, secondif);
                            secondif->add_predecessor(edge);
                        }
                    }

                    parent->get_stats().remove_with_key(firstif->id);
                    if (parent->get_first() == firstif) {
                        parent->set_first(secondif);
                    }

                    // merge if conditions
                    IfExprent* statexpr = secondif->get_headexprent();

                    std::vector<Exprent*> lst_operands;
                    lst_operands.push_back(firstif->get_headexprent()->get_condition());

                    if (path == 2) {
                        lst_operands[0] = make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, lst_operands[0], Offsets{});
                    }

                    lst_operands.push_back(statexpr->get_condition());

                    statexpr->set_condition(
                        make<FunctionExprent>(path == 1 ? FunctionExprent::FUNCTION_COR : FunctionExprent::FUNCTION_CADD, lst_operands, Offsets{}));

                    if (exprents_of(secondif->get_first()).empty() && !exprents_of(firstif->get_first()).empty()) {
                        secondif->replace_statement(secondif->get_first(), firstif->get_first());
                    }

                    return true;
                }
            }
        } else if (elsebranch->succs.size() == 1) {
            if (at(elsebranch->succs, 0)->value == at(rtnode->succs, 0)->value) {
                IfStatement* firstif = static_cast<IfStatement*>(rtnode->value);
                Statement* second = elsebranch->value;

                firstif->remove_all_successors(second);

                for (StatEdge* edge : second->get_all_successor_edges()) {
                    second->remove_successor(edge);
                    edge->set_source(firstif);
                    firstif->add_successor(edge);
                }

                StatEdge* ifedge = firstif->get_if_edge();
                firstif->get_first()->remove_successor(ifedge);

                second->add_successor(make<StatEdge>(ifedge->get_type(), second, ifedge->get_destination(), ifedge->closure));

                StatEdge* newifedge = make<StatEdge>(StatEdge::TYPE_REGULAR, firstif->get_first(), second);
                firstif->get_first()->add_successor(newifedge);
                firstif->set_ifstat(second);

                firstif->get_stats().add_with_key(second, second->id);
                second->set_parent(firstif);

                firstif->get_parent()->get_stats().remove_with_key(second->id);

                // negate the if condition
                IfExprent* statexpr = firstif->get_headexprent();
                statexpr->set_condition(make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, statexpr->get_condition(), Offsets{}));

                return true;
            }
        }
    }

    return false;
}

IfHelper::IfNode* IfHelper::build_graph(IfStatement* stat, bool stsingle) {
    if (stat->iftype == IfStatement::IFTYPE_IFELSE) {
        return nullptr;
    }

    IfNode* res = make<IfNode>(stat);

    // if branch
    Statement* ifchild = stat->get_ifstat();
    if (ifchild == nullptr) {
        StatEdge* edge = stat->get_if_edge();
        res->add_child(make<IfNode>(edge->get_destination()), 1);
    } else {
        IfNode* ifnode = make<IfNode>(ifchild);
        res->add_child(ifnode, 0);
        if (ifchild->type == Statement::TYPE_IF && static_cast<IfStatement*>(ifchild)->iftype == IfStatement::IFTYPE_IF) {
            IfStatement* stat2 = static_cast<IfStatement*>(ifchild);
            Statement* ifchild2 = stat2->get_ifstat();
            if (ifchild2 == nullptr) {
                StatEdge* edge = stat2->get_if_edge();
                ifnode->add_child(make<IfNode>(edge->get_destination()), 1);
            } else {
                ifnode->add_child(make<IfNode>(ifchild2), 0);
            }
        }

        if (!ifchild->get_all_successor_edges().empty()) {
            ifnode->add_child(make<IfNode>(ifchild->get_all_successor_edges()[0]->get_destination()), 1);
        }
    }

    // else branch
    StatEdge* edge = at(stat->get_all_successor_edges(), 0);
    Statement* elsechild = edge->get_destination();
    IfNode* elsenode = make<IfNode>(elsechild);

    if (stsingle || edge->get_type() != StatEdge::TYPE_REGULAR) {
        res->add_child(elsenode, 1);
    } else {
        res->add_child(elsenode, 0);
        if (elsechild->type == Statement::TYPE_IF && static_cast<IfStatement*>(elsechild)->iftype == IfStatement::IFTYPE_IF) {
            IfStatement* stat2 = static_cast<IfStatement*>(elsechild);
            Statement* ifchild2 = stat2->get_ifstat();
            if (ifchild2 == nullptr) {
                elsenode->add_child(make<IfNode>(stat2->get_if_edge()->get_destination()), 1);
            } else {
                elsenode->add_child(make<IfNode>(ifchild2), 0);
            }
        }

        if (!elsechild->get_all_successor_edges().empty()) {
            elsenode->add_child(make<IfNode>(elsechild->get_all_successor_edges()[0]->get_destination()), 1);
        }
    }

    return res;
}

// FIXME: rewrite the entire method!!! keep in mind finally exits!!
bool IfHelper::reorder_if(IfStatement* ifstat) {
    if (ifstat->iftype == IfStatement::IFTYPE_IFELSE) {
        return false;
    }

    bool ifdirect = false, elsedirect = false;
    bool noifstat = false, noelsestat = false;
    bool ifdirectpath = false, elsedirectpath = false;

    Statement* parent = ifstat->get_parent();
    Statement* from = parent->type == Statement::TYPE_SEQUENCE ? parent : ifstat;

    Statement* next = get_next_statement(from);

    if (ifstat->get_ifstat() == nullptr) {
        noifstat = true;

        if (ifstat->get_if_edge()->get_type() == StatEdge::TYPE_FINALLYEXIT) {
            ifdirect = true;
        } else {
            ifdirect = MergeHelper::is_direct_path(from, ifstat->get_if_edge()->get_destination());
        }
    } else {
        std::vector<StatEdge*> lst_succs = ifstat->get_ifstat()->get_all_successor_edges();
        if (!lst_succs.empty() && lst_succs[0]->get_type() == StatEdge::TYPE_FINALLYEXIT) {
            ifdirect = true;
        } else {
            ifdirect = has_direct_end_edge(ifstat->get_ifstat(), from);
        }
    }

    Statement* last = parent->type == Statement::TYPE_SEQUENCE ? parent->get_stats().last() : ifstat;
    noelsestat = (last == ifstat);

    if (!last->get_all_successor_edges().empty() && last->get_all_successor_edges()[0]->get_type() == StatEdge::TYPE_FINALLYEXIT) {
        elsedirect = true;
    } else {
        elsedirect = has_direct_end_edge(last, from);
    }

    if (!noelsestat && exists_path(ifstat, at(ifstat->get_all_successor_edges(), 0)->get_destination())) {
        return false;
    }

    if (!ifdirect && !noifstat) {
        ifdirectpath = exists_path(ifstat, next);
    }

    if (!elsedirect && !noelsestat) {
        SequenceStatement* sequence = static_cast<SequenceStatement*>(parent);

        for (int i = static_cast<int>(sequence->get_stats().size()) - 1; i >= 0; i--) {
            Statement* sttemp = sequence->get_stats()[static_cast<std::size_t>(i)];
            if (sttemp == ifstat) {
                break;
            }
            if ((elsedirectpath = exists_path(sttemp, next))) {
                break;
            }
        }
    }

    if ((ifdirect || ifdirectpath) && (elsedirect || elsedirectpath) && !noifstat && !noelsestat) {  // if - then - else
        SequenceStatement* sequence = static_cast<SequenceStatement*>(parent);

        // build and cut the new else statement
        std::vector<Statement*> lst;
        for (int i = static_cast<int>(sequence->get_stats().size()) - 1; i >= 0; i--) {
            Statement* sttemp = sequence->get_stats()[static_cast<std::size_t>(i)];
            if (sttemp == ifstat) {
                break;
            }
            lst.insert(lst.begin(), sttemp);
        }

        Statement* stelse;
        if (lst.size() == 1) {
            stelse = lst[0];
        } else {
            stelse = make<SequenceStatement>(lst);
            stelse->set_all_parent();
        }

        ifstat->remove_successor(at(ifstat->get_all_successor_edges(), 0));
        for (Statement* st : lst) {
            sequence->get_stats().remove_with_key(st->id);
        }

        StatEdge* elseedge = make<StatEdge>(StatEdge::TYPE_REGULAR, ifstat->get_first(), stelse);
        ifstat->get_first()->add_successor(elseedge);
        ifstat->set_elsestat(stelse);
        ifstat->set_else_edge(elseedge);

        ifstat->get_stats().add_with_key(stelse, stelse->id);
        stelse->set_parent(ifstat);

        ifstat->iftype = IfStatement::IFTYPE_IFELSE;
    } else if (ifdirect && (!elsedirect || (noifstat && !noelsestat))) {  // if - then
        // negate the if condition
        IfExprent* statexpr = ifstat->get_headexprent();
        statexpr->set_condition(make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, statexpr->get_condition(), Offsets{}));

        if (noelsestat) {
            StatEdge* ifedge = ifstat->get_if_edge();
            StatEdge* elseedge = at(ifstat->get_all_successor_edges(), 0);

            if (noifstat) {
                ifstat->get_first()->remove_successor(ifedge);
                ifstat->remove_successor(elseedge);

                ifedge->set_source(ifstat);
                elseedge->set_source(ifstat->get_first());

                ifstat->add_successor(ifedge);
                ifstat->get_first()->add_successor(elseedge);

                ifstat->set_if_edge(elseedge);
            } else {
                Statement* ifbranch = ifstat->get_ifstat();
                SequenceStatement* newseq = make<SequenceStatement>(std::vector<Statement*>{ifstat, ifbranch});

                ifstat->get_first()->remove_successor(ifedge);
                ifstat->get_stats().remove_with_key(ifbranch->id);
                ifstat->set_ifstat(nullptr);

                ifstat->remove_successor(elseedge);
                elseedge->set_source(ifstat->get_first());
                ifstat->get_first()->add_successor(elseedge);

                ifstat->set_if_edge(elseedge);

                ifstat->get_parent()->replace_statement(ifstat, newseq);
                newseq->set_all_parent();

                ifstat->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, ifstat, ifbranch));
            }
        } else {
            SequenceStatement* sequence = static_cast<SequenceStatement*>(parent);

            // build and cut the new else statement
            std::vector<Statement*> lst;
            for (int i = static_cast<int>(sequence->get_stats().size()) - 1; i >= 0; i--) {
                Statement* sttemp = sequence->get_stats()[static_cast<std::size_t>(i)];
                if (sttemp == ifstat) {
                    break;
                }
                lst.insert(lst.begin(), sttemp);
            }

            Statement* stelse;
            if (lst.size() == 1) {
                stelse = lst[0];
            } else {
                stelse = make<SequenceStatement>(lst);
                stelse->set_all_parent();
            }

            ifstat->remove_successor(at(ifstat->get_all_successor_edges(), 0));
            for (Statement* st : lst) {
                sequence->get_stats().remove_with_key(st->id);
            }

            if (noifstat) {
                StatEdge* ifedge = ifstat->get_if_edge();

                ifstat->get_first()->remove_successor(ifedge);
                ifedge->set_source(ifstat);
                ifstat->add_successor(ifedge);
            } else {
                Statement* ifbranch = ifstat->get_ifstat();

                ifstat->get_first()->remove_successor(ifstat->get_if_edge());
                ifstat->get_stats().remove_with_key(ifbranch->id);

                ifstat->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, ifstat, ifbranch));

                sequence->get_stats().add_with_key(ifbranch, ifbranch->id);
                ifbranch->set_parent(sequence);
            }

            StatEdge* newifedge = make<StatEdge>(StatEdge::TYPE_REGULAR, ifstat->get_first(), stelse);
            ifstat->get_first()->add_successor(newifedge);
            ifstat->set_ifstat(stelse);
            ifstat->set_if_edge(newifedge);

            ifstat->get_stats().add_with_key(stelse, stelse->id);
            stelse->set_parent(ifstat);
        }
    } else {
        return false;
    }

    return true;
}

bool IfHelper::has_direct_end_edge(Statement* stat, Statement* from) {
    for (StatEdge* edge : stat->get_all_successor_edges()) {
        if (MergeHelper::is_direct_path(from, edge->get_destination())) {
            return true;
        }
    }

    if (!stat->get_exprents()) {
        switch (stat->type) {
            case Statement::TYPE_SEQUENCE:
                return has_direct_end_edge(stat->get_stats().last(), from);
            case Statement::TYPE_CATCHALL:
            case Statement::TYPE_TRYCATCH:
                for (Statement* st : stat->get_stats()) {
                    if (has_direct_end_edge(st, from)) {
                        return true;
                    }
                }
                break;
            case Statement::TYPE_IF: {
                IfStatement* ifstat = static_cast<IfStatement*>(stat);
                if (ifstat->iftype == IfStatement::IFTYPE_IFELSE) {
                    return has_direct_end_edge(ifstat->get_ifstat(), from) || has_direct_end_edge(ifstat->get_elsestat(), from);
                }
                break;
            }
            case Statement::TYPE_SYNCRONIZED:
                return has_direct_end_edge(stat->get_stats().get(1), from);
            case Statement::TYPE_SWITCH:
                for (Statement* st : stat->get_stats()) {
                    if (has_direct_end_edge(st, from)) {
                        return true;
                    }
                }
        }
    }

    return false;
}

Statement* IfHelper::get_next_statement(Statement* stat) {
    Statement* parent = stat->get_parent();
    switch (parent->type) {
        case Statement::TYPE_ROOT:
            return static_cast<RootStatement*>(parent)->get_dummy_exit();
        case Statement::TYPE_DO:
            return parent;
        case Statement::TYPE_SEQUENCE: {
            SequenceStatement* sequence = static_cast<SequenceStatement*>(parent);
            if (sequence->get_stats().last() != stat) {
                for (int i = static_cast<int>(sequence->get_stats().size()) - 1; i >= 0; i--) {
                    if (sequence->get_stats()[static_cast<std::size_t>(i)] == stat) {
                        return sequence->get_stats().get(static_cast<std::size_t>(i) + 1);
                    }
                }
            }
        }
    }

    return get_next_statement(parent);
}

bool IfHelper::exists_path(Statement* from, Statement* to) {
    for (StatEdge* edge : to->get_all_predecessor_edges()) {
        if (from->contains_statement_strict(edge->get_source())) {
            return true;
        }
    }

    return false;
}

}  // namespace cb::java::decompiler
