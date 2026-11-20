// IdeaNotNullHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/annotations.hpp"
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/structs.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool IdeaNotNullHelper::remove_hardcoded_checks(Statement* root, const StructMethod* mt) {
    bool checks_removed = false;

    // parameter @NotNull annotations
    while (find_and_remove_parameter_check(root, mt)) {  // iterate until nothing found. Each invocation removes one parameter check.
        checks_removed = true;
    }

    // method @NotNull annotation
    while (find_and_remove_return_check(root, mt)) {  // iterate until nothing found. Each invocation handles one method exit check.
        checks_removed = true;
    }

    return checks_removed;
}

bool IdeaNotNullHelper::find_and_remove_parameter_check(Statement* stat, const StructMethod* mt) {
    Statement* st = stat->get_first();
    while (st->type == Statement::TYPE_SEQUENCE) {
        st = st->get_first();
    }

    if (st->type == Statement::TYPE_IF) {
        IfStatement* ifstat = static_cast<IfStatement*>(st);
        Statement* ifbranch = ifstat->get_ifstat();

        Exprent* if_condition = ifstat->get_headexprent()->get_condition();

        bool is_notnull_check = false;

        // TODO: FUNCTION_NE also possible if reversed order (in theory)
        if (ifbranch != nullptr && if_condition->type == Exprent::EXPRENT_FUNCTION &&
            static_cast<FunctionExprent*>(if_condition)->get_func_type() == FunctionExprent::FUNCTION_EQ && ifbranch->type == Statement::TYPE_BASICBLOCK &&
            exprents_of(ifbranch).size() == 1 && exprents_of(ifbranch)[0]->type == Exprent::EXPRENT_EXIT) {
            FunctionExprent* func = static_cast<FunctionExprent*>(if_condition);
            Exprent* first_param = at(func->get_lst_operands(), 0);
            Exprent* second_param = at(func->get_lst_operands(), 1);

            if (second_param->type == Exprent::EXPRENT_CONST && second_param->get_expr_type().type == cc::TYPE_NULL) {  // TODO: reversed parameter order
                if (first_param->type == Exprent::EXPRENT_VAR) {
                    VarExprent* var = static_cast<VarExprent*>(first_param);

                    bool thisvar = !mt->has_modifier(cc::ACC_STATIC);

                    MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());

                    // parameter annotations
                    const AnnotationParameterAttribute* param_annotations =
                        mt->attribute<AnnotationParameterAttribute>(attr::RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS);
                    if (param_annotations != nullptr) {
                        const std::vector<std::vector<AnnotationExprent*>>& param_annotations_lists = param_annotations->get_param_annotations();
                        int method_param_number = static_cast<int>(md.params.size());

                        int index = thisvar ? 1 : 0;
                        for (int i = 0; i < method_param_number; i++) {
                            if (index == var->get_index()) {
                                if (static_cast<int>(param_annotations_lists.size()) >= method_param_number - i) {
                                    int shift = method_param_number - static_cast<int>(param_annotations_lists.size());  // NOTE: workaround for compiler bug, count annotations starting with the last parameter

                                    const std::vector<AnnotationExprent*>& annotations = at(param_annotations_lists, i - shift);

                                    for (AnnotationExprent* ann : annotations) {
                                        if (ann->get_class_name() == "org/jetbrains/annotations/NotNull") {
                                            is_notnull_check = true;
                                        }
                                    }
                                }

                                break;
                            }

                            index += md.params[static_cast<std::size_t>(i)].stack_size;
                        }
                    }
                }
            }
        }

        if (!is_notnull_check) {
            return false;
        }

        remove_parameter_check(stat, mt);

        return true;
    }

    return false;
}

void IdeaNotNullHelper::remove_parameter_check(Statement* stat, const StructMethod* mt) {
    (void)mt;

    Statement* st = stat->get_first();
    while (st->type == Statement::TYPE_SEQUENCE) {
        st = st->get_first();
    }

    IfStatement* ifstat = static_cast<IfStatement*>(st);

    if (ifstat->get_elsestat() == nullptr) {  // if
        // TODO:
    } else {  // if - else
        StatEdge* ifedge = ifstat->get_if_edge();
        StatEdge* elseedge = ifstat->get_else_edge();

        Statement* ifbranch = ifstat->get_ifstat();
        Statement* elsebranch = ifstat->get_elsestat();

        ifstat->get_first()->remove_successor(ifedge);
        ifstat->get_first()->remove_successor(elseedge);

        ifstat->get_stats().remove_with_key(ifbranch->id);
        ifstat->get_stats().remove_with_key(elsebranch->id);

        if (!ifbranch->get_all_successor_edges().empty()) {
            ifbranch->remove_successor(ifbranch->get_all_successor_edges()[0]);
        }

        ifstat->get_parent()->replace_statement(ifstat, elsebranch);
        ifstat->get_parent()->set_all_parent();
    }
}

bool IdeaNotNullHelper::find_and_remove_return_check(Statement* stat, const StructMethod* mt) {
    bool is_notnull_check = false;

    // method annotation, refers to the return value
    const AnnotationAttribute* attribute = mt->attribute<AnnotationAttribute>(attr::RUNTIME_INVISIBLE_ANNOTATIONS);
    if (attribute != nullptr) {
        const std::vector<AnnotationExprent*>& annotations = attribute->get_annotations();

        for (AnnotationExprent* ann : annotations) {
            if (ann->get_class_name() == "org/jetbrains/annotations/NotNull") {
                is_notnull_check = true;
            }
        }
    }

    if (is_notnull_check) {
        return remove_return_check(stat, mt);
    }

    return false;
}

bool IdeaNotNullHelper::remove_return_check(Statement* stat, const StructMethod* mt) {
    Statement* parent = stat->get_parent();

    if (parent != nullptr && parent->type == Statement::TYPE_IF && stat->type == Statement::TYPE_BASICBLOCK && exprents_of(stat).size() == 1) {
        Exprent* exprent = exprents_of(stat)[0];
        if (exprent->type == Exprent::EXPRENT_EXIT) {
            ExitExprent* exit_exprent = static_cast<ExitExprent*>(exprent);
            if (exit_exprent->get_exit_type() == ExitExprent::EXIT_RETURN) {
                Exprent* exprent_value = exit_exprent->get_value();

                IfStatement* ifparent = static_cast<IfStatement*>(parent);
                Exprent* if_condition = ifparent->get_headexprent()->get_condition();

                if (ifparent->get_elsestat() == stat && if_condition->type == Exprent::EXPRENT_FUNCTION &&
                    static_cast<FunctionExprent*>(if_condition)->get_func_type() == FunctionExprent::FUNCTION_EQ) {  // TODO: reversed order possible (in theory)
                    FunctionExprent* func = static_cast<FunctionExprent*>(if_condition);
                    Exprent* first_param = at(func->get_lst_operands(), 0);
                    Exprent* second_param = at(func->get_lst_operands(), 1);

                    StatEdge* ifedge = ifparent->get_if_edge();
                    StatEdge* elseedge = ifparent->get_else_edge();

                    Statement* ifbranch = ifparent->get_ifstat();
                    Statement* elsebranch = ifparent->get_elsestat();

                    if (second_param->type == Exprent::EXPRENT_CONST && second_param->get_expr_type().type == cc::TYPE_NULL) {  // TODO: reversed parameter order
                        if (first_param->equals(exprent_value)) {  // TODO: check for absence of side effects like method invocations etc.
                            if (ifbranch->type == Statement::TYPE_BASICBLOCK && exprents_of(ifbranch).size() == 1 &&
                                // TODO: special check for IllegalStateException
                                exprents_of(ifbranch)[0]->type == Exprent::EXPRENT_EXIT) {
                                ifparent->get_first()->remove_successor(ifedge);
                                ifparent->get_first()->remove_successor(elseedge);

                                ifparent->get_stats().remove_with_key(ifbranch->id);
                                ifparent->get_stats().remove_with_key(elsebranch->id);

                                if (!ifbranch->get_all_successor_edges().empty()) {
                                    ifbranch->remove_successor(ifbranch->get_all_successor_edges()[0]);
                                }

                                if (!exprents_of(ifparent->get_first()).empty()) {
                                    std::vector<Exprent*>& dst = exprents_of(elsebranch);
                                    const std::vector<Exprent*>& src = exprents_of(ifparent->get_first());
                                    dst.insert(dst.begin(), src.begin(), src.end());
                                }

                                ifparent->get_parent()->replace_statement(ifparent, elsebranch);
                                ifparent->get_parent()->set_all_parent();

                                return true;
                            }
                        }
                    }
                }
            }
        }
    } else if (parent != nullptr && parent->type == Statement::TYPE_SEQUENCE && stat->type == Statement::TYPE_BASICBLOCK && exprents_of(stat).size() == 1) {
        Exprent* exprent = exprents_of(stat)[0];
        if (exprent->type == Exprent::EXPRENT_EXIT) {
            ExitExprent* exit_exprent = static_cast<ExitExprent*>(exprent);
            if (exit_exprent->get_exit_type() == ExitExprent::EXIT_RETURN) {
                Exprent* exprent_value = exit_exprent->get_value();

                SequenceStatement* sequence = static_cast<SequenceStatement*>(parent);
                int sequence_stats_number = static_cast<int>(sequence->get_stats().size());

                if (sequence_stats_number > 1 && sequence->get_stats().last() == stat &&
                    sequence->get_stats()[static_cast<std::size_t>(sequence_stats_number) - 2]->type == Statement::TYPE_IF) {
                    IfStatement* ifstat = static_cast<IfStatement*>(sequence->get_stats()[static_cast<std::size_t>(sequence_stats_number) - 2]);
                    Exprent* if_condition = ifstat->get_headexprent()->get_condition();

                    if (ifstat->iftype == IfStatement::IFTYPE_IF && if_condition->type == Exprent::EXPRENT_FUNCTION &&
                        static_cast<FunctionExprent*>(if_condition)->get_func_type() == FunctionExprent::FUNCTION_EQ) {  // TODO: reversed order possible (in theory)
                        FunctionExprent* func = static_cast<FunctionExprent*>(if_condition);
                        Exprent* first_param = at(func->get_lst_operands(), 0);
                        Exprent* second_param = at(func->get_lst_operands(), 1);

                        Statement* ifbranch = ifstat->get_ifstat();

                        if (second_param->type == Exprent::EXPRENT_CONST && second_param->get_expr_type().type == cc::TYPE_NULL) {  // TODO: reversed parameter order
                            if (first_param->equals(exprent_value)) {  // TODO: check for absence of side effects like method invocations etc.
                                if (ifbranch->type == Statement::TYPE_BASICBLOCK && exprents_of(ifbranch).size() == 1 &&
                                    // TODO: special check for IllegalStateException
                                    exprents_of(ifbranch)[0]->type == Exprent::EXPRENT_EXIT) {
                                    ifstat->remove_successor(at(ifstat->get_all_successor_edges(), 0));  // remove 'else' edge

                                    if (!exprents_of(ifstat->get_first()).empty()) {
                                        std::vector<Exprent*>& dst = exprents_of(stat);
                                        const std::vector<Exprent*> src = exprents_of(ifstat->get_first());
                                        dst.insert(dst.begin(), src.begin(), src.end());
                                    }

                                    for (StatEdge* edge : ifstat->get_all_predecessor_edges()) {
                                        ifstat->remove_predecessor(edge);
                                        edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, stat);
                                        stat->add_predecessor(edge);
                                    }

                                    sequence->get_stats().remove_with_key(ifstat->id);
                                    sequence->set_first(sequence->get_stats().get(0));

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (Statement* st : stat->get_stats()) {
        if (remove_return_check(st, mt)) {
            return true;
        }
    }

    return false;
}

}  // namespace cb::java::decompiler
