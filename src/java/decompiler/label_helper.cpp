// LabelHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>

#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

void LabelHelper::clean_up_edges(RootStatement* root) {
    reset_all_edges(root);

    remove_non_immediate_edges(root);

    lift_closures(root);

    LinkedSet<StatEdge*> edges;
    low_continue_labels(root, edges);

    low_closures(root);
}

void LabelHelper::identify_labels(RootStatement* root) {
    set_explicit_edges(root);

    hide_default_switch_edges(root);

    process_statement_label(root);

    set_ret_edges_unlabeled(root);
}

void LabelHelper::lift_closures(Statement* stat) {
    for (StatEdge* edge : stat->get_all_successor_edges()) {
        switch (edge->get_type()) {
            case StatEdge::TYPE_CONTINUE:
                if (edge->get_destination() != edge->closure) {
                    edge->get_destination()->add_labeled_edge(edge);
                }
                break;
            case StatEdge::TYPE_BREAK: {
                Statement* dest = edge->get_destination();
                if (dest->type != Statement::TYPE_DUMMYEXIT) {
                    Statement* parent = dest->get_parent();

                    std::vector<Statement*> lst;
                    if (parent->type == Statement::TYPE_SEQUENCE) {
                        lst = parent->get_stats().items();
                    } else if (parent->type == Statement::TYPE_SWITCH) {
                        lst = static_cast<SwitchStatement*>(parent)->get_case_statements();
                    }

                    for (int i = 0; i < static_cast<int>(lst.size()); i++) {
                        if (lst[static_cast<std::size_t>(i)] == dest) {
                            at(lst, i - 1)->add_labeled_edge(edge);
                            break;
                        }
                    }
                }
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        lift_closures(st);
    }
}

void LabelHelper::remove_non_immediate_edges(Statement* stat) {
    for (Statement* st : stat->get_stats()) {
        remove_non_immediate_edges(st);
    }

    if (!stat->has_basic_succ_edge()) {
        for (StatEdge* edge : stat->get_successor_edges(StatEdge::TYPE_CONTINUE | StatEdge::TYPE_BREAK)) {
            stat->remove_successor(edge);
        }
    }
}

void LabelHelper::low_continue_labels(Statement* stat, LinkedSet<StatEdge*>& edges) {
    bool ok = (stat->type != Statement::TYPE_DO);
    if (!ok) {
        DoStatement* dostat = static_cast<DoStatement*>(stat);
        ok = dostat->get_looptype() == DoStatement::LOOP_DO || dostat->get_looptype() == DoStatement::LOOP_WHILE ||
             (dostat->get_looptype() == DoStatement::LOOP_FOR && dostat->get_inc_exprent() == nullptr);
    }

    if (ok) {
        edges.add_all(stat->get_predecessor_edges(StatEdge::TYPE_CONTINUE));
    }

    if (ok && stat->type == Statement::TYPE_DO) {
        for (StatEdge* edge : edges) {
            if (stat->contains_statement_strict(edge->get_source())) {
                edge->get_destination()->remove_predecessor(edge);
                edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, stat);
                stat->add_predecessor(edge);

                stat->add_labeled_edge(edge);
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        if (st == stat->get_first()) {
            low_continue_labels(st, edges);
        } else {
            LinkedSet<StatEdge*> fresh;
            low_continue_labels(st, fresh);
        }
    }
}

void LabelHelper::low_closures(Statement* stat) {
    for (StatEdge* edge : stat->get_label_edges().items()) {
        if (edge->get_type() == StatEdge::TYPE_BREAK) {  // FIXME: ?
            for (Statement* st : stat->get_stats()) {
                if (st->contains_statement_strict(edge->get_source())) {
                    if (MergeHelper::is_direct_path(st, edge->get_destination())) {
                        st->add_labeled_edge(edge);
                    }
                }
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        low_closures(st);
    }
}

void LabelHelper::reset_all_edges(Statement* stat) {
    for (Statement* st : stat->get_stats()) {
        reset_all_edges(st);
    }

    for (StatEdge* edge : stat->get_all_successor_edges()) {
        edge->explicit_ = true;
        edge->labeled = true;
    }
}

void LabelHelper::set_ret_edges_unlabeled(RootStatement* root) {
    Statement* exit = root->get_dummy_exit();
    for (StatEdge* edge : exit->get_all_predecessor_edges()) {
        std::optional<std::vector<Exprent*>>& lst = edge->get_source()->get_exprents();
        if (edge->get_type() == StatEdge::TYPE_FINALLYEXIT || (lst && !lst->empty() && lst->back()->type == Exprent::EXPRENT_EXIT)) {
            edge->labeled = false;
        }
    }
}

namespace {
// mapEdges.get(key).addAll(value) / mapEdges.put(key, value), as the original merges one map into another.
template <class Map>
void merge_edge_maps(Map& into, Map& from) {
    for (auto& entr : from) {
        if (into.contains_key(entr.first)) {
            std::vector<StatEdge*>& dst = *into.get(entr.first);
            dst.insert(dst.end(), entr.second.begin(), entr.second.end());
        } else {
            into.put(entr.first, entr.second);
        }
    }
}
}  // namespace

LabelHelper::EdgeMap LabelHelper::set_explicit_edges(Statement* stat) {
    EdgeMap map_edges;

    if (stat->get_exprents()) {
        return map_edges;
    }

    switch (stat->type) {
        case Statement::TYPE_TRYCATCH:
        case Statement::TYPE_CATCHALL:
            for (Statement* st : stat->get_stats()) {
                EdgeMap map_edges1 = set_explicit_edges(st);
                process_edges_with_next(st, map_edges1, nullptr);

                if (stat->type == Statement::TYPE_TRYCATCH || st == stat->get_first()) {  // edges leaving a finally catch block are always explicit
                    // merge the maps
                    merge_edge_maps(map_edges, map_edges1);
                }
            }

            break;
        case Statement::TYPE_DO:
            map_edges = set_explicit_edges(stat->get_first());
            process_edges_with_next(stat->get_first(), map_edges, stat);
            break;
        case Statement::TYPE_IF: {
            IfStatement* ifstat = static_cast<IfStatement*>(stat);
            // head statement is a basic block
            if (ifstat->get_ifstat() == nullptr) {  // empty if
                process_edges_with_next(ifstat->get_first(), map_edges, nullptr);
            } else {
                map_edges = set_explicit_edges(ifstat->get_ifstat());
                process_edges_with_next(ifstat->get_ifstat(), map_edges, nullptr);

                std::optional<EdgeMap> map_edges1;
                if (ifstat->get_elsestat() != nullptr) {
                    map_edges1 = set_explicit_edges(ifstat->get_elsestat());
                    process_edges_with_next(ifstat->get_elsestat(), *map_edges1, nullptr);
                }

                // merge the maps
                if (map_edges1) {
                    merge_edge_maps(map_edges, *map_edges1);
                }
            }
            break;
        }
        case Statement::TYPE_ROOT:
            map_edges = set_explicit_edges(stat->get_first());
            process_edges_with_next(stat->get_first(), map_edges, static_cast<RootStatement*>(stat)->get_dummy_exit());
            break;
        case Statement::TYPE_SEQUENCE: {
            int index = 0;
            while (index < static_cast<int>(stat->get_stats().size()) - 1) {
                Statement* st = stat->get_stats()[static_cast<std::size_t>(index)];
                EdgeMap m = set_explicit_edges(st);
                process_edges_with_next(st, m, stat->get_stats()[static_cast<std::size_t>(index) + 1]);
                index++;
            }

            Statement* st = at(stat->get_stats().items(), index);
            map_edges = set_explicit_edges(st);
            process_edges_with_next(st, map_edges, nullptr);
            break;
        }
        case Statement::TYPE_SWITCH: {
            SwitchStatement* swst = static_cast<SwitchStatement*>(stat);

            for (int i = 0; i < static_cast<int>(swst->get_case_statements().size()) - 1; i++) {
                Statement* stt = swst->get_case_statements()[static_cast<std::size_t>(i)];
                Statement* stnext = swst->get_case_statements()[static_cast<std::size_t>(i) + 1];

                if (stnext->get_exprents() && stnext->get_exprents()->empty()) {
                    stnext = at(stnext->get_all_successor_edges(), 0)->get_destination();
                }
                EdgeMap m = set_explicit_edges(stt);
                process_edges_with_next(stt, m, stnext);
            }

            int last = static_cast<int>(swst->get_case_statements().size()) - 1;
            if (last >= 0) {  // empty switch possible
                Statement* stlast = swst->get_case_statements()[static_cast<std::size_t>(last)];
                if (stlast->get_exprents() && stlast->get_exprents()->empty()) {
                    StatEdge* edge = at(stlast->get_all_successor_edges(), 0);
                    map_edges.put(edge->get_destination(), std::vector<StatEdge*>{edge});
                } else {
                    map_edges = set_explicit_edges(stlast);
                    process_edges_with_next(stlast, map_edges, nullptr);
                }
            }

            break;
        }
        case Statement::TYPE_SYNCRONIZED: {
            SynchronizedStatement* synstat = static_cast<SynchronizedStatement*>(stat);

            EdgeMap m = set_explicit_edges(stat->get_first());
            process_edges_with_next(synstat->get_first(), m, synstat->get_body());  // FIXME: basic block?
            map_edges = set_explicit_edges(synstat->get_body());
            process_edges_with_next(synstat->get_body(), map_edges, nullptr);
        }
    }

    return map_edges;
}

void LabelHelper::process_edges_with_next(Statement* stat, EdgeMap& map_edges, Statement* next) {
    StatEdge* statedge = nullptr;

    std::vector<StatEdge*> lst_succs = stat->get_all_successor_edges();
    if (!lst_succs.empty()) {
        statedge = lst_succs[0];

        if (statedge->get_destination() == next) {
            statedge->explicit_ = false;
            statedge = nullptr;
        } else {
            next = statedge->get_destination();
        }
    }

    // no next for a do statement
    if (stat->type == Statement::TYPE_DO && static_cast<DoStatement*>(stat)->get_looptype() == DoStatement::LOOP_DO) {
        next = nullptr;
    }

    if (next == nullptr) {
        if (map_edges.size() == 1) {
            const auto& only = *map_edges.begin();
            std::vector<StatEdge*> lst_edges = only.second;
            if (lst_edges.size() > 1 && only.first->type != Statement::TYPE_DUMMYEXIT) {
                StatEdge* edge_example = lst_edges[0];

                Statement* closure = stat->get_parent();
                if (!closure->contains_statement_strict(edge_example->closure)) {
                    closure = edge_example->closure;
                }

                StatEdge* newedge = make<StatEdge>(edge_example->get_type(), stat, edge_example->get_destination(), closure);
                stat->add_successor(newedge);

                for (StatEdge* edge : lst_edges) {
                    edge->explicit_ = false;
                }

                map_edges.put(newedge->get_destination(), std::vector<StatEdge*>{newedge});
            }
        }
    } else {
        bool implfound = false;

        for (auto& entr : map_edges) {
            if (entr.first == next) {
                for (StatEdge* edge : entr.second) {
                    if (stat->type == Statement::TYPE_DO) {
                        // Edges that contain it's own loop as a closure are always explicit, as removing them can alter control flow
                        if (edge->closure == stat) {
                            continue;
                        }
                    }

                    if (edge->closure != nullptr && edge->closure->type == Statement::TYPE_DO && stat->type == Statement::TYPE_IF &&
                        edge->get_destination()->type == Statement::TYPE_DUMMYEXIT) {
                        continue;
                    }
                    edge->explicit_ = false;
                }
                implfound = true;
                break;
            }
        }

        if (stat->get_all_successor_edges().empty() && !implfound) {
            const std::vector<StatEdge*>* lst_edges = nullptr;
            for (auto& entr : map_edges) {
                if (entr.first->type != Statement::TYPE_DUMMYEXIT && (lst_edges == nullptr || entr.second.size() > lst_edges->size())) {
                    lst_edges = &entr.second;
                }
            }

            if (lst_edges != nullptr && lst_edges->size() > 1) {
                StatEdge* edge_example = (*lst_edges)[0];

                Statement* closure = stat->get_parent();
                if (!closure->contains_statement_strict(edge_example->closure)) {
                    closure = edge_example->closure;
                }

                StatEdge* newedge = make<StatEdge>(edge_example->get_type(), stat, edge_example->get_destination(), closure);
                stat->add_successor(newedge);

                for (StatEdge* edge : *lst_edges) {
                    edge->explicit_ = false;
                }
            }
        }

        map_edges.clear();
    }

    if (statedge != nullptr) {
        map_edges.put(statedge->get_destination(), std::vector<StatEdge*>{statedge});
    }
}

void LabelHelper::hide_default_switch_edges(Statement* stat) {
    if (stat->type == Statement::TYPE_SWITCH) {
        SwitchStatement* swst = static_cast<SwitchStatement*>(stat);

        int last = static_cast<int>(swst->get_case_statements().size()) - 1;
        if (last >= 0) {  // empty switch possible
            Statement* stlast = swst->get_case_statements()[static_cast<std::size_t>(last)];

            if (stlast->get_exprents() && stlast->get_exprents()->empty()) {
                if (!at(stlast->get_all_successor_edges(), 0)->explicit_) {
                    std::vector<StatEdge*>& lst_edges = at(swst->get_case_edges(), last);
                    auto it = std::find(lst_edges.begin(), lst_edges.end(), swst->get_default_edge());
                    if (it != lst_edges.end()) lst_edges.erase(it);

                    if (lst_edges.empty()) {
                        swst->get_case_statements().erase(swst->get_case_statements().begin() + last);
                        swst->get_case_edges().erase(swst->get_case_edges().begin() + last);
                    }
                }
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        hide_default_switch_edges(st);
    }
}

std::array<LinkedSet<Statement*>, 2> LabelHelper::process_statement_label(Statement* stat) {
    LinkedSet<Statement*> set_break;
    LinkedSet<Statement*> set_continue;

    if (!stat->get_exprents()) {
        for (Statement* st : stat->get_stats()) {
            std::array<LinkedSet<Statement*>, 2> arr = process_statement_label(st);

            set_break.add_all(arr[0]);
            set_continue.add_all(arr[1]);
        }

        bool shield_type = (stat->type == Statement::TYPE_DO || stat->type == Statement::TYPE_SWITCH);
        if (shield_type) {
            for (StatEdge* edge : stat->get_label_edges()) {
                if (edge->explicit_ && ((edge->get_type() == StatEdge::TYPE_BREAK && set_break.contains(edge->get_source())) ||
                                        (edge->get_type() == StatEdge::TYPE_CONTINUE && set_continue.contains(edge->get_source())))) {
                    edge->labeled = false;
                }
            }
        }

        switch (stat->type) {
            case Statement::TYPE_DO:
                set_continue.clear();
                [[fallthrough]];
            case Statement::TYPE_SWITCH:
                set_break.clear();
        }
    }

    set_break.add(stat);
    set_continue.add(stat);

    std::array<LinkedSet<Statement*>, 2> result;
    result[0] = std::move(set_break);
    result[1] = std::move(set_continue);
    return result;
}

void LabelHelper::replace_continue_with_break(Statement* stat) {
    if (stat->type == Statement::TYPE_DO) {
        std::vector<StatEdge*> lst = stat->get_predecessor_edges(StatEdge::TYPE_CONTINUE);

        for (StatEdge* edge : lst) {
            if (edge->explicit_) {
                Statement* minclosure = get_min_continue_closure(edge);

                if (minclosure != edge->closure && !InlineSingleBlockHelper::is_break_edge_labeled(edge->get_source(), minclosure)) {
                    edge->get_source()->change_edge_type(Statement::DIRECTION_FORWARD, edge, StatEdge::TYPE_BREAK);
                    edge->labeled = false;
                    minclosure->add_labeled_edge(edge);
                }
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        replace_continue_with_break(st);
    }
}

Statement* LabelHelper::get_min_continue_closure(StatEdge* edge) {
    Statement* closure = edge->closure;
    while (true) {
        bool found = false;

        for (Statement* st : closure->get_stats()) {
            if (st->contains_statement_strict(edge->get_source())) {
                if (MergeHelper::is_direct_path(st, edge->get_destination())) {
                    closure = st;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    return closure;
}

}  // namespace cb::java::decompiler
