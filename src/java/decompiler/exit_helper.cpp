// ExitHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/cfg.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

namespace {
BasicBlockStatement* new_block_statement() {
    return make<BasicBlockStatement>(new_basic_block(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER)));
}
}  // namespace

bool ExitHelper::condense_exits(RootStatement* root) {
    int changed = integrate_exits(root);

    if (changed > 0) {
        clean_up_unreachable_blocks(root);

        SequenceHelper::condense_sequences(root);
    }

    return changed > 0;
}

void ExitHelper::clean_up_unreachable_blocks(Statement* stat) {
    bool found;
    do {
        found = false;

        for (int i = 0; i < static_cast<int>(stat->get_stats().size()); i++) {
            Statement* st = stat->get_stats()[static_cast<std::size_t>(i)];

            clean_up_unreachable_blocks(st);

            if (st->type == Statement::TYPE_SEQUENCE && st->get_stats().size() > 1) {
                Statement* last = st->get_stats().last();
                Statement* secondlast = st->get_stats()[st->get_stats().size() - 2];

                if (!last->get_exprents() || !last->get_exprents()->empty()) {
                    if (!secondlast->has_basic_succ_edge()) {
                        LinkedSet<Statement*> set = last->get_neighbours_set(Statement::STATEDGE_DIRECT_ALL, Statement::DIRECTION_BACKWARD);
                        set.remove(secondlast);

                        if (set.empty()) {
                            last->set_exprents(std::vector<Exprent*>{});
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
    } while (found);
}

int ExitHelper::integrate_exits(Statement* stat) {
    int ret = 0;
    Statement* dest = nullptr;

    if (!stat->get_exprents()) {
        while (true) {
            int changed = 0;

            for (Statement* st : stat->get_stats()) {
                changed = integrate_exits(st);
                if (changed > 0) {
                    ret = 1;
                    break;
                }
            }

            if (changed == 0) {
                break;
            }
        }

        switch (stat->type) {
            case Statement::TYPE_IF: {
                IfStatement* ifst = static_cast<IfStatement*>(stat);
                if (ifst->get_ifstat() == nullptr) {
                    StatEdge* ifedge = ifst->get_if_edge();
                    dest = is_exit_edge(ifedge);
                    if (dest != nullptr) {
                        BasicBlockStatement* bstat = new_block_statement();
                        bstat->set_exprents(DecHelper::copy_exprent_list(exprents_of(dest)));

                        ifst->get_first()->remove_successor(ifedge);
                        StatEdge* newedge = make<StatEdge>(StatEdge::TYPE_REGULAR, ifst->get_first(), bstat);
                        ifst->get_first()->add_successor(newedge);
                        ifst->set_if_edge(newedge);
                        ifst->set_ifstat(bstat);
                        ifst->get_stats().add_with_key(bstat, bstat->id);
                        bstat->set_parent(ifst);

                        StatEdge* oldexitedge = at(dest->get_all_successor_edges(), 0);
                        StatEdge* newexitedge = make<StatEdge>(StatEdge::TYPE_BREAK, bstat, oldexitedge->get_destination());
                        bstat->add_successor(newexitedge);
                        nn(oldexitedge->closure)->add_labeled_edge(newexitedge);
                        ret = 1;
                    }
                }
            }
        }
    }

    if (stat->get_all_successor_edges().size() == 1 && stat->get_all_successor_edges()[0]->get_type() == StatEdge::TYPE_BREAK &&
        stat->get_label_edges().empty()) {
        Statement* parent = stat->get_parent();
        if (stat != parent->get_first() || (parent->type != Statement::TYPE_IF && parent->type != Statement::TYPE_SWITCH)) {
            StatEdge* destedge = stat->get_all_successor_edges()[0];
            dest = is_exit_edge(destedge);
            if (dest != nullptr) {
                stat->remove_successor(destedge);

                BasicBlockStatement* bstat = new_block_statement();
                bstat->set_exprents(DecHelper::copy_exprent_list(exprents_of(dest)));

                StatEdge* oldexitedge = at(dest->get_all_successor_edges(), 0);
                StatEdge* newexitedge = make<StatEdge>(StatEdge::TYPE_BREAK, bstat, oldexitedge->get_destination());
                bstat->add_successor(newexitedge);
                nn(oldexitedge->closure)->add_labeled_edge(newexitedge);

                SequenceStatement* block = make<SequenceStatement>(std::vector<Statement*>{stat, bstat});
                block->set_all_parent();

                parent->replace_statement(stat, block);
                // LabelHelper.lowContinueLabels not applicable because of forward continue edges
                // LabelHelper.lowContinueLabels(block, new HashSet<StatEdge>());
                // do it by hand
                for (StatEdge* prededge : block->get_predecessor_edges(StatEdge::TYPE_CONTINUE)) {
                    block->remove_predecessor(prededge);
                    prededge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, prededge, stat);
                    stat->add_predecessor(prededge);

                    stat->add_labeled_edge(prededge);
                }

                stat->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, stat, bstat));

                for (StatEdge* edge : dest->get_all_predecessor_edges()) {
                    if (!edge->explicit_ && stat->contains_statement_strict(edge->get_source()) &&
                        MergeHelper::is_direct_path(edge->get_source()->get_parent(), bstat)) {
                        dest->remove_predecessor(edge);
                        edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, bstat);
                        bstat->add_predecessor(edge);

                        if (!stat->contains_statement_strict(edge->closure)) {
                            stat->add_labeled_edge(edge);
                        }
                    }
                }

                ret = 2;
            }
        }
    }

    return ret;
}

Statement* ExitHelper::is_exit_edge(StatEdge* edge) {
    Statement* dest = edge->get_destination();

    if (edge->get_type() == StatEdge::TYPE_BREAK && dest->type == Statement::TYPE_BASICBLOCK && edge->explicit_ && (edge->labeled || is_only_edge(edge))) {
        std::optional<std::vector<Exprent*>>& data = dest->get_exprents();

        if (data && data->size() == 1) {
            if ((*data)[0]->type == Exprent::EXPRENT_EXIT) {
                return dest;
            }
        }
    }

    return nullptr;
}

bool ExitHelper::is_only_edge(StatEdge* edge) {
    Statement* stat = edge->get_destination();

    for (StatEdge* ed : stat->get_all_predecessor_edges()) {
        if (ed != edge) {
            if (ed->get_type() == StatEdge::TYPE_REGULAR) {
                Statement* source = ed->get_source();

                if (source->type == Statement::TYPE_BASICBLOCK ||
                    (source->type == Statement::TYPE_IF && static_cast<IfStatement*>(source)->iftype == IfStatement::IFTYPE_IF) ||
                    (source->type == Statement::TYPE_DO && static_cast<DoStatement*>(source)->get_looptype() != DoStatement::LOOP_DO)) {
                    return false;
                }
            } else {
                return false;
            }
        }
    }

    return true;
}

bool ExitHelper::remove_redundant_returns(RootStatement* root) {
    bool res = false;

    for (StatEdge* edge : root->get_dummy_exit()->get_all_predecessor_edges()) {
        if (!edge->explicit_) {
            Statement* source = edge->get_source();
            std::optional<std::vector<Exprent*>>& lst_expr = source->get_exprents();
            if (lst_expr && !lst_expr->empty()) {
                Exprent* expr = lst_expr->back();
                if (expr->type == Exprent::EXPRENT_EXIT) {
                    ExitExprent* ex = static_cast<ExitExprent*>(expr);
                    if (ex->get_exit_type() == ExitExprent::EXIT_RETURN && ex->get_value() == nullptr) {
                        // remove redundant return
                        lst_expr->pop_back();
                        res = true;
                    }
                }
            }
        }
    }

    return res;
}

bool ExitHelper::handle_return_from_initializer(RootStatement* root) {
    bool res = false;

    Statement* exit = root->get_dummy_exit();
    Statement* top = root->get_first();
    Statement* newret = nullptr;

    bool sharedcreated = false;

    for (StatEdge* edge : exit->get_all_predecessor_edges()) {
        if (edge->explicit_) {
            if (!sharedcreated) {
                newret = add_shared_initializer_return(root);
                sharedcreated = true;
            }

            Statement* source = edge->get_source();
            std::optional<std::vector<Exprent*>>& lst_expr = source->get_exprents();
            if (lst_expr && !lst_expr->empty()) {
                Exprent* expr = lst_expr->back();
                if (expr->type == Exprent::EXPRENT_EXIT) {
                    ExitExprent* ex = static_cast<ExitExprent*>(expr);
                    if (ex->get_exit_type() == ExitExprent::EXIT_RETURN && ex->get_value() == nullptr) {
                        lst_expr->pop_back();

                        source->remove_successor(edge);
                        source->add_successor(make<StatEdge>(StatEdge::TYPE_BREAK, source, newret, top));

                        res = true;
                    }
                }
            }
        }
    }

    return res;
}

Statement* ExitHelper::add_shared_initializer_return(RootStatement* root) {
    Statement* exit = root->get_dummy_exit();
    Statement* top = root->get_first();

    // build a new statement with the single instruction 'return'
    BasicBlockStatement* bstat = new_block_statement();

    const std::optional<MethodDescriptor>& md = DecompilerContext::current().current_method_descriptor;
    if (!md) throw std::runtime_error("NullPointerException: no current method descriptor");
    ExitExprent* retexpr = make<ExitExprent>(ExitExprent::EXIT_RETURN, nullptr, md->ret, Offsets{});
    // a changeable list needed
    bstat->set_exprents(std::vector<Exprent*>{retexpr});

    // build sequence to replace the former top statement
    SequenceStatement* seq = make<SequenceStatement>(std::vector<Statement*>{top, bstat});
    top->set_parent(seq);
    bstat->set_parent(seq);
    seq->set_parent(root);

    root->get_stats().remove_with_key(top->id);
    root->get_stats().add_with_key_and_index(0, seq, seq->id);
    root->set_first(seq);

    for (StatEdge* succedge : top->get_all_successor_edges()) {
        top->remove_successor(succedge);
    }

    top->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, top, bstat));
    bstat->add_successor(make<StatEdge>(StatEdge::TYPE_BREAK, bstat, exit, seq));

    return bstat;
}

}  // namespace cb::java::decompiler
