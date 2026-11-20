// InlineSingleBlockHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see
// NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool InlineSingleBlockHelper::inline_single_blocks(RootStatement* root) {
    bool res = inline_single_blocks_rec(root);

    if (res) {
        SequenceHelper::condense_sequences(root);
    }

    return res;
}

bool InlineSingleBlockHelper::inline_single_blocks_rec(Statement* stat) {
    bool res = false;

    for (Statement* st : stat->get_stats()) {
        res |= inline_single_blocks_rec(st);
    }

    if (stat->type == Statement::TYPE_SEQUENCE) {
        SequenceStatement* seq = static_cast<SequenceStatement*>(stat);
        for (int i = 1; i < static_cast<int>(seq->get_stats().size()); i++) {
            if (is_inlineable(seq, i)) {
                inline_block(seq, i);
                return true;
            }
        }
    }

    return res;
}

void InlineSingleBlockHelper::inline_block(SequenceStatement* seq, int index) {
    Statement* first = seq->get_stats()[static_cast<std::size_t>(index)];
    Statement* pre = seq->get_stats()[static_cast<std::size_t>(index) - 1];
    pre->remove_successor(at(pre->get_all_successor_edges(), 0));   // single regular edge

    StatEdge* edge = at(first->get_predecessor_edges(StatEdge::TYPE_BREAK), 0);
    Statement* source = edge->get_source();
    Statement* parent = source->get_parent();
    source->remove_successor(edge);

    std::vector<Statement*> lst;
    for (int i = static_cast<int>(seq->get_stats().size()) - 1; i >= index; i--) {
        lst.insert(lst.begin(), seq->get_stats().remove(static_cast<std::size_t>(i)));
    }

    if (parent->type == Statement::TYPE_IF && static_cast<IfStatement*>(parent)->iftype == IfStatement::IFTYPE_IF && source == parent->get_first()) {
        IfStatement* ifparent = static_cast<IfStatement*>(parent);
        SequenceStatement* block = make<SequenceStatement>(lst);
        block->set_all_parent();

        StatEdge* newedge = make<StatEdge>(StatEdge::TYPE_REGULAR, source, block);
        source->add_successor(newedge);
        ifparent->set_if_edge(newedge);
        ifparent->set_ifstat(block);

        ifparent->get_stats().add_with_key(block, block->id);
        block->set_parent(ifparent);
    } else {
        lst.insert(lst.begin(), source);

        SequenceStatement* block = make<SequenceStatement>(lst);
        block->set_all_parent();

        parent->replace_statement(source, block);

        // LabelHelper.lowContinueLabels not applicable because of forward continue edges
        // LabelHelper.lowContinueLabels(block, new HashSet<StatEdge>());
        // do it by hand
        for (StatEdge* prededge : block->get_predecessor_edges(StatEdge::TYPE_CONTINUE)) {
            block->remove_predecessor(prededge);
            prededge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, prededge, source);
            source->add_predecessor(prededge);

            source->add_labeled_edge(prededge);
        }

        if (parent->type == Statement::TYPE_SWITCH) {
            static_cast<SwitchStatement*>(parent)->sort_edges_and_nodes();
        }

        source->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, source, first));
    }
}

bool InlineSingleBlockHelper::is_inlineable(SequenceStatement* seq, int index) {
    Statement* first = seq->get_stats()[static_cast<std::size_t>(index)];
    Statement* pre = seq->get_stats()[static_cast<std::size_t>(index) - 1];

    if (pre->has_basic_succ_edge()) {
        return false;
    }

    std::vector<StatEdge*> lst = first->get_predecessor_edges(StatEdge::TYPE_BREAK);

    if (lst.size() == 1) {
        StatEdge* edge = lst[0];

        if (same_catch_ranges(edge)) {
            if (edge->explicit_) {
                return true;
            }
            for (int i = index; i < static_cast<int>(seq->get_stats().size()); i++) {
                if (!no_exit_labels(seq->get_stats()[static_cast<std::size_t>(i)], seq)) {
                    return false;
                }
            }
            return true;
        }
        // FIXME: count labels properly
    }

    return false;
}

bool InlineSingleBlockHelper::same_catch_ranges(StatEdge* edge) {
    Statement* from = edge->get_source();
    Statement* to = edge->get_destination();

    while (true) {
        Statement* parent = from->get_parent();
        if (parent->contains_statement_strict(to)) {
            break;
        }

        if (parent->type == Statement::TYPE_TRYCATCH || parent->type == Statement::TYPE_CATCHALL) {
            if (parent->get_first() == from) {
                return false;
            }
        } else if (parent->type == Statement::TYPE_SYNCRONIZED) {
            if (parent->get_stats().get(1) == from) {
                return false;
            }
        }

        from = parent;
    }

    return true;
}

bool InlineSingleBlockHelper::no_exit_labels(Statement* block, Statement* sequence) {
    for (StatEdge* edge : block->get_all_successor_edges()) {
        if (edge->get_type() != StatEdge::TYPE_REGULAR && edge->get_destination()->type != Statement::TYPE_DUMMYEXIT) {
            if (!sequence->contains_statement_strict(edge->get_destination())) {
                return false;
            }
        }
    }

    for (Statement* st : block->get_stats()) {
        if (!no_exit_labels(st, sequence)) {
            return false;
        }
    }

    return true;
}

bool InlineSingleBlockHelper::is_break_edge_labeled(Statement* source, Statement* closure) {
    if (closure->type == Statement::TYPE_DO || closure->type == Statement::TYPE_SWITCH) {
        Statement* parent = source->get_parent();

        if (parent == closure) {
            return false;
        }
        return parent->type == Statement::TYPE_DO || parent->type == Statement::TYPE_SWITCH || is_break_edge_labeled(parent, closure);
    }
    return true;
}

}  // namespace cb::java::decompiler
