// LoopExtractHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool LoopExtractHelper::extract_loops(Statement* root) {
    bool res = (extract_loops_rec(root) != 0);

    if (res) {
        SequenceHelper::condense_sequences(root);
    }

    return res;
}

int LoopExtractHelper::extract_loops_rec(Statement* stat) {
    bool res = false;

    while (true) {
        bool updated = false;

        for (Statement* st : stat->get_stats()) {
            int extr = extract_loops_rec(st);
            res |= (extr != 0);

            if (extr == 2) {
                updated = true;
                break;
            }
        }

        if (!updated) {
            break;
        }
    }

    if (stat->type == Statement::TYPE_DO) {
        if (extract_loop(static_cast<DoStatement*>(stat))) {
            return 2;
        }
    }

    return res ? 1 : 0;
}

bool LoopExtractHelper::extract_loop(DoStatement* stat) {
    if (stat->get_looptype() != DoStatement::LOOP_DO) {
        return false;
    }

    for (StatEdge* edge : stat->get_label_edges()) {
        if (edge->get_type() != StatEdge::TYPE_CONTINUE && edge->get_destination()->type != Statement::TYPE_DUMMYEXIT) {
            return false;
        }
    }

    if (!extract_last_if(stat)) {
        return extract_first_if(stat);
    }
    return true;
}

bool LoopExtractHelper::extract_last_if(DoStatement* stat) {
    // search for an if condition at the end of the loop
    Statement* last = stat->get_first();
    while (last->type == Statement::TYPE_SEQUENCE) {
        last = last->get_stats().last();
    }

    if (last->type == Statement::TYPE_IF) {
        IfStatement* lastif = static_cast<IfStatement*>(last);
        if (lastif->iftype == IfStatement::IFTYPE_IF && lastif->get_ifstat() != nullptr) {
            Statement* ifstat = lastif->get_ifstat();
            StatEdge* elseedge = at(lastif->get_all_successor_edges(), 0);

            if (elseedge->get_type() == StatEdge::TYPE_CONTINUE && elseedge->closure == stat) {
                LinkedSet<Statement*> set = stat->get_neighbours_set(StatEdge::TYPE_CONTINUE, Statement::DIRECTION_BACKWARD);
                set.remove(last);

                if (set.empty()) {  // no direct continues in a do{}while loop
                    if (is_extern_statement(stat, ifstat, ifstat)) {
                        extract_if_block(stat, lastif);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool LoopExtractHelper::extract_first_if(DoStatement* stat) {
    // search for an if condition at the entrance of the loop
    Statement* first = stat->get_first();
    while (first->type == Statement::TYPE_SEQUENCE) {
        first = first->get_first();
    }

    // found an if statement
    if (first->type == Statement::TYPE_IF) {
        IfStatement* firstif = static_cast<IfStatement*>(first);

        if (exprents_of(firstif->get_first()).empty()) {
            if (firstif->iftype == IfStatement::IFTYPE_IF && firstif->get_ifstat() != nullptr) {
                Statement* ifstat = firstif->get_ifstat();

                if (is_extern_statement(stat, ifstat, ifstat)) {
                    extract_if_block(stat, firstif);
                    return true;
                }
            }
        }
    }
    return false;
}

bool LoopExtractHelper::is_extern_statement(DoStatement* loop, Statement* block, Statement* stat) {
    for (StatEdge* edge : stat->get_all_successor_edges()) {
        if (loop->contains_statement(edge->get_destination()) && !block->contains_statement(edge->get_destination())) {
            return false;
        }
    }

    for (Statement* st : stat->get_stats()) {
        if (!is_extern_statement(loop, block, st)) {
            return false;
        }
    }

    return true;
}

void LoopExtractHelper::extract_if_block(DoStatement* loop, IfStatement* ifstat) {
    Statement* target = ifstat->get_ifstat();
    StatEdge* ifedge = ifstat->get_if_edge();

    ifstat->set_ifstat(nullptr);
    ifedge->get_source()->change_edge_type(Statement::DIRECTION_FORWARD, ifedge, StatEdge::TYPE_BREAK);
    ifedge->closure = loop;
    ifstat->get_stats().remove_with_key(target->id);

    loop->add_labeled_edge(ifedge);

    SequenceStatement* block = make<SequenceStatement>(std::vector<Statement*>{loop, target});
    loop->get_parent()->replace_statement(loop, block);
    block->set_all_parent();

    loop->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, loop, target));

    for (StatEdge* edge : block->get_label_edges().items()) {
        if (edge->get_type() == StatEdge::TYPE_CONTINUE || edge == ifedge) {
            loop->add_labeled_edge(edge);
        }
    }

    for (StatEdge* edge : block->get_predecessor_edges(StatEdge::TYPE_CONTINUE)) {
        if (loop->contains_statement_strict(edge->get_source())) {
            block->remove_predecessor(edge);
            edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, loop);
            loop->add_predecessor(edge);
        }
    }
}

}  // namespace cb::java::decompiler
