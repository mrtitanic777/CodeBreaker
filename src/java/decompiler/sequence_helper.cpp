// SequenceHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/cfg.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

void SequenceHelper::condense_sequences(Statement* root) {
    condense_sequences_rec(root);
}

void SequenceHelper::condense_sequences_rec(Statement* stat) {
    if (stat->type == Statement::TYPE_SEQUENCE) {
        std::vector<Statement*> lst = stat->get_stats().items();

        bool unfolded = false;

        // unfold blocks
        for (int i = 0; i < static_cast<int>(lst.size()); i++) {
            Statement* st = lst[static_cast<std::size_t>(i)];
            if (st->type == Statement::TYPE_SEQUENCE) {
                remove_empty_statements(static_cast<SequenceStatement*>(st));

                if (i == static_cast<int>(lst.size()) - 1 || is_sequence_disbandable(st, lst[static_cast<std::size_t>(i) + 1])) {
                    // move predecessors
                    Statement* first = st->get_first();
                    for (StatEdge* edge : st->get_all_predecessor_edges()) {
                        st->remove_predecessor(edge);
                        edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, first);
                        first->add_predecessor(edge);
                    }

                    // move successors
                    Statement* last = st->get_stats().last();
                    if (last->get_all_successor_edges().empty() && i < static_cast<int>(lst.size()) - 1) {
                        last->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, last, lst[static_cast<std::size_t>(i) + 1]));
                    } else {
                        for (StatEdge* edge : last->get_all_successor_edges()) {
                            if (i == static_cast<int>(lst.size()) - 1) {
                                if (edge->closure == st) {
                                    stat->add_labeled_edge(edge);
                                }
                            } else {
                                edge->get_source()->change_edge_type(Statement::DIRECTION_FORWARD, edge, StatEdge::TYPE_REGULAR);
                                nn(edge->closure)->get_label_edges().remove(edge);
                                edge->closure = nullptr;
                            }
                        }
                    }

                    for (StatEdge* edge : st->get_all_successor_edges()) {
                        st->remove_successor(edge);
                    }

                    for (StatEdge* edge : st->get_label_edges().items()) {
                        if (edge->get_source() != last) {
                            last->add_labeled_edge(edge);
                        }
                    }

                    lst.erase(lst.begin() + i);
                    lst.insert(lst.begin() + i, st->get_stats().items().begin(), st->get_stats().items().end());
                    i--;

                    unfolded = true;
                }
            }
        }

        if (unfolded) {
            SequenceStatement* sequence = make<SequenceStatement>(lst);
            sequence->set_all_parent();

            stat->get_parent()->replace_statement(stat, sequence);

            stat = sequence;
        }
    }

    // sequence consisting of one statement -> disband
    if (stat->type == Statement::TYPE_SEQUENCE) {
        remove_empty_statements(static_cast<SequenceStatement*>(stat));

        if (stat->get_stats().size() == 1) {
            Statement* st = stat->get_first();

            bool ok = st->get_all_successor_edges().empty();
            if (!ok) {
                StatEdge* edge = at(st->get_all_successor_edges(), 0);

                ok = stat->get_all_successor_edges().empty();
                if (!ok) {
                    StatEdge* statedge = at(stat->get_all_successor_edges(), 0);
                    ok = (edge->get_destination() == statedge->get_destination());

                    if (ok) {
                        st->remove_successor(edge);
                    }
                }
            }

            if (ok) {
                stat->get_parent()->replace_statement(stat, st);
                stat = st;
            }
        }
    }

    // replace flat statements with synthetic basic blocks
    for (bool again = true; again;) {
        again = false;
        for (Statement* st : stat->get_stats()) {
            if ((st->get_stats().empty() || st->get_exprents()) && st->type != Statement::TYPE_BASICBLOCK) {
                destroy_and_flatten_statement(st);
                again = true;
                break;
            }
        }
    }

    // recursion
    for (int i = 0; i < static_cast<int>(stat->get_stats().size()); i++) {
        condense_sequences_rec(stat->get_stats()[static_cast<std::size_t>(i)]);
    }
}

bool SequenceHelper::is_sequence_disbandable(Statement* block, Statement* next) {
    Statement* last = block->get_stats().last();
    std::vector<StatEdge*> lst_succs = last->get_all_successor_edges();
    if (!lst_succs.empty()) {
        if (lst_succs[0]->get_destination() != next) {
            return false;
        }
    }

    for (StatEdge* edge : next->get_predecessor_edges(StatEdge::TYPE_BREAK)) {
        if (last != edge->get_source() && !last->contains_statement_strict(edge->get_source())) {
            return false;
        }
    }

    return true;
}

void SequenceHelper::remove_empty_statements(SequenceStatement* sequence) {
    if (sequence->get_stats().size() <= 1) {
        return;
    }

    merge_flat_statements(sequence);

    while (true) {
        bool found = false;

        for (Statement* st : sequence->get_stats()) {
            if (st->get_exprents() && st->get_exprents()->empty()) {
                if (st->get_all_successor_edges().empty()) {
                    std::vector<StatEdge*> lst_breaks = st->get_predecessor_edges(StatEdge::TYPE_BREAK);

                    if (lst_breaks.empty()) {
                        for (StatEdge* edge : st->get_all_predecessor_edges()) {
                            edge->get_source()->remove_successor(edge);
                        }
                        found = true;
                    }
                } else {
                    StatEdge* sucedge = at(st->get_all_successor_edges(), 0);
                    if (sucedge->get_type() != StatEdge::TYPE_FINALLYEXIT) {
                        st->remove_successor(sucedge);

                        for (StatEdge* edge : st->get_all_predecessor_edges()) {
                            if (sucedge->get_type() != StatEdge::TYPE_REGULAR) {
                                edge->get_source()->change_edge_type(Statement::DIRECTION_FORWARD, edge, sucedge->get_type());
                            }

                            st->remove_predecessor(edge);
                            edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, sucedge->get_destination());
                            sucedge->get_destination()->add_predecessor(edge);

                            if (sucedge->closure != nullptr) {
                                sucedge->closure->add_labeled_edge(edge);
                            }
                        }
                        found = true;
                    }
                }

                if (found) {
                    sequence->get_stats().remove_with_key(st->id);
                    break;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    sequence->set_first(sequence->get_stats().get(0));
}

void SequenceHelper::merge_flat_statements(SequenceStatement* sequence) {
    while (true) {
        Statement* next = nullptr;
        Statement* current = nullptr;

        bool found = false;

        for (int i = static_cast<int>(sequence->get_stats().size()) - 1; i >= 0; i--) {
            next = current;
            current = sequence->get_stats()[static_cast<std::size_t>(i)];

            if (next != nullptr && current->get_exprents() && !current->get_exprents()->empty()) {
                if (next->get_exprents()) {
                    std::vector<Exprent*>& dst = *next->get_exprents();
                    std::vector<Exprent*>& src = *current->get_exprents();
                    dst.insert(dst.begin(), src.begin(), src.end());
                    src.clear();
                    found = true;
                } else {
                    Statement* first = get_first_exprentlist(next);
                    if (first != nullptr) {
                        std::vector<Exprent*>& dst = *first->get_exprents();
                        std::vector<Exprent*>& src = *current->get_exprents();
                        dst.insert(dst.begin(), src.begin(), src.end());
                        src.clear();
                        found = true;
                    }
                }
            }
        }

        if (!found) {
            break;
        }
    }
}

Statement* SequenceHelper::get_first_exprentlist(Statement* stat) {
    if (stat->get_exprents()) {
        return stat;
    }

    switch (stat->type) {
        case Statement::TYPE_IF:
        case Statement::TYPE_SEQUENCE:
        case Statement::TYPE_SWITCH:
        case Statement::TYPE_SYNCRONIZED:
            return get_first_exprentlist(stat->get_first());
    }

    return nullptr;
}

void SequenceHelper::destroy_and_flatten_statement(Statement* stat) {
    destroy_statement_content(stat, false);

    BasicBlockStatement* bstat = make<BasicBlockStatement>(
        new_basic_block(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER)));
    if (!stat->get_exprents()) {
        bstat->set_exprents(std::vector<Exprent*>{});
    } else {
        bstat->set_exprents(DecHelper::copy_exprent_list(*stat->get_exprents()));
    }

    stat->get_parent()->replace_statement(stat, bstat);
}

void SequenceHelper::destroy_statement_content(Statement* stat, bool self) {
    for (Statement* st : stat->get_stats()) {
        destroy_statement_content(st, true);
    }
    stat->get_stats().clear();

    if (self) {
        for (StatEdge* edge : stat->get_all_successor_edges()) {
            stat->remove_successor(edge);
        }
    }
}

}  // namespace cb::java::decompiler
