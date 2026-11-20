// LowBreakHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

void LowBreakHelper::low_break_labels(Statement* root) {
    low_break_labels_rec(root);

    lift_break_labels(root);
}

void LowBreakHelper::low_break_labels_rec(Statement* stat) {
    while (true) {
        bool found = false;

        for (StatEdge* edge : stat->get_label_edges()) {
            if (edge->get_type() == StatEdge::TYPE_BREAK) {
                Statement* minclosure = get_min_closure(stat, edge->get_source());
                if (minclosure != stat) {
                    minclosure->add_labeled_edge(edge);
                    edge->labeled = is_break_edge_labeled(edge->get_source(), minclosure);
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    for (Statement* st : stat->get_stats()) {
        low_break_labels_rec(st);
    }
}

bool LowBreakHelper::is_break_edge_labeled(Statement* source, Statement* closure) {
    if (closure->type == Statement::TYPE_DO || closure->type == Statement::TYPE_SWITCH) {
        Statement* parent = source->get_parent();

        if (parent == closure) {
            return false;
        }
        return is_break_edge_labeled(parent, closure) || (parent->type == Statement::TYPE_DO || parent->type == Statement::TYPE_SWITCH);
    }
    return true;
}

Statement* LowBreakHelper::get_min_closure(Statement* closure, Statement* source) {
    while (true) {
        Statement* newclosure = nullptr;

        switch (closure->type) {
            case Statement::TYPE_SEQUENCE: {
                Statement* last = closure->get_stats().last();

                if (is_ok_closure(closure, source, last)) {
                    newclosure = last;
                }
                break;
            }
            case Statement::TYPE_IF: {
                IfStatement* ifclosure = static_cast<IfStatement*>(closure);
                if (is_ok_closure(closure, source, ifclosure->get_ifstat())) {
                    newclosure = ifclosure->get_ifstat();
                } else if (is_ok_closure(closure, source, ifclosure->get_elsestat())) {
                    newclosure = ifclosure->get_elsestat();
                }
                break;
            }
            case Statement::TYPE_TRYCATCH:
                for (Statement* st : closure->get_stats()) {
                    if (is_ok_closure(closure, source, st)) {
                        newclosure = st;
                        break;
                    }
                }
                break;
            case Statement::TYPE_SYNCRONIZED: {
                Statement* body = static_cast<SynchronizedStatement*>(closure)->get_body();

                if (is_ok_closure(closure, source, body)) {
                    newclosure = body;
                }
            }
        }

        if (newclosure == nullptr) {
            break;
        }

        closure = newclosure;
    }

    return closure;
}

bool LowBreakHelper::is_ok_closure(Statement* closure, Statement* source, Statement* stat) {
    bool ok = false;

    if (stat != nullptr && stat->contains_statement_strict(source)) {
        std::vector<StatEdge*> lst = stat->get_all_successor_edges();

        ok = lst.empty();
        if (!ok) {
            StatEdge* edge = lst[0];
            ok = (edge->closure == closure && edge->get_type() == StatEdge::TYPE_BREAK);
        }
    }

    return ok;
}

void LowBreakHelper::lift_break_labels(Statement* stat) {
    for (Statement* st : stat->get_stats()) {
        lift_break_labels(st);
    }

    while (true) {
        bool found = false;

        for (StatEdge* edge : stat->get_label_edges()) {
            if (edge->explicit_ && edge->labeled && edge->get_type() == StatEdge::TYPE_BREAK) {
                Statement* newclosure = get_max_break_lift(stat, edge);

                if (newclosure != nullptr) {
                    newclosure->add_labeled_edge(edge);
                    edge->labeled = is_break_edge_labeled(edge->get_source(), newclosure);

                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            break;
        }
    }
}

Statement* LowBreakHelper::get_max_break_lift(Statement* stat, StatEdge* edge) {
    Statement* closure = nullptr;
    Statement* newclosure = stat;

    while ((newclosure = get_next_break_lift(newclosure, edge)) != nullptr) {
        closure = newclosure;
    }

    return closure;
}

Statement* LowBreakHelper::get_next_break_lift(Statement* stat, StatEdge* edge) {
    Statement* closure = stat->get_parent();

    while (closure != nullptr && !closure->contains_statement_strict(edge->get_destination())) {
        bool labeled = is_break_edge_labeled(edge->get_source(), closure);
        if (closure->is_labeled() || !labeled) {
            return closure;
        }

        closure = closure->get_parent();
    }

    return nullptr;
}

}  // namespace cb::java::decompiler
