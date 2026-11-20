// java/decompiler/nested_member_access.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.rels.NestedMemberAccess), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>
#include <set>
#include <string>
#include <vector>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

void NestedMemberAccess::propagate_member_access(ClassNode* root) {
    if (root->nested.empty()) return;

    no_synth_flag_ = DecompilerContext::option(pref::SYNTHETIC_NOT_SET);

    compute_method_types(root);

    eliminate_static_access(root);
}

void NestedMemberAccess::compute_method_types(ClassNode* node) {
    if (node->type == ClassNode::CLASS_LAMBDA) return;

    for (ClassNode* nd : node->nested) compute_method_types(nd);

    for (MethodWrapper* method : node->get_wrapper()->get_methods()) compute_method_type(node, method);
}

void NestedMemberAccess::compute_method_type(ClassNode* node, MethodWrapper* method) {
    int type = METHOD_ACCESS_NORMAL;

    if (method->root != nullptr) {
        DirectGraph* graph = method->get_or_build_graph();

        const StructMethod* mt = method->method_struct;

        if ((no_synth_flag_ || mt->is_synthetic()) && mt->has_modifier(cc::ACC_STATIC)) {
            if (graph->nodes.size() == 2) {   // incl. dummy exit node
                if (graph->first->exprents->size() == 1) {
                    Exprent* exprent = (*graph->first->exprents)[0];

                    const MethodDescriptor mtdesc = MethodDescriptor::parse(mt->descriptor());
                    const int parcount = static_cast<int>(mtdesc.params.size());

                    Exprent* expr_core = exprent;
                    if (exprent->type == Exprent::EXPRENT_EXIT) {
                        auto* exexpr = static_cast<ExitExprent*>(exprent);
                        if (exexpr->get_exit_type() == ExitExprent::EXIT_RETURN && exexpr->get_value() != nullptr) {
                            expr_core = exexpr->get_value();
                        }
                    }

                    switch (expr_core->type) {
                        case Exprent::EXPRENT_FIELD: {
                            auto* fexpr = static_cast<FieldExprent*>(expr_core);
                            if ((parcount == 1 && !fexpr->is_static()) || (parcount == 0 && fexpr->is_static())) {
                                if (fexpr->get_classname() == node->class_struct->qualified_name) {   // FIXME (the original's): check for private flag of the field
                                    if (fexpr->is_static() ||
                                        (fexpr->get_instance()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(fexpr->get_instance())->get_index() == 0)) {
                                        type = METHOD_ACCESS_FIELD_GET;
                                    }
                                }
                            }
                            break;
                        }
                        case Exprent::EXPRENT_VAR:   // qualified this
                            if (parcount == 1) {
                                // this or final variable
                                if (static_cast<VarExprent*>(expr_core)->get_index() != 0) type = METHOD_ACCESS_FIELD_GET;
                            }
                            break;
                        case Exprent::EXPRENT_INVOCATION:
                            type = METHOD_ACCESS_METHOD;
                            break;
                        case Exprent::EXPRENT_ASSIGNMENT: {
                            auto* asexpr = static_cast<AssignmentExprent*>(expr_core);
                            if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD && asexpr->get_right()->type == Exprent::EXPRENT_VAR) {
                                auto* fexpras = static_cast<FieldExprent*>(asexpr->get_left());
                                if ((parcount == 2 && !fexpras->is_static()) || (parcount == 1 && fexpras->is_static())) {
                                    if (fexpras->get_classname() == node->class_struct->qualified_name) {   // FIXME (the original's): check for private flag of the field
                                        if (fexpras->is_static() || (fexpras->get_instance()->type == Exprent::EXPRENT_VAR &&
                                                                     static_cast<VarExprent*>(fexpras->get_instance())->get_index() == 0)) {
                                            if (static_cast<VarExprent*>(asexpr->get_right())->get_index() == parcount - 1) type = METHOD_ACCESS_FIELD_SET;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }

                    if (type == METHOD_ACCESS_METHOD) {   // FIXME (the original's): check for private flag of the method
                        type = METHOD_ACCESS_NORMAL;

                        auto* invexpr = static_cast<InvocationExprent*>(expr_core);

                        if ((invexpr->is_static() && static_cast<int>(invexpr->get_lst_parameters().size()) == parcount) ||
                            (!invexpr->is_static() && invexpr->get_instance()->type == Exprent::EXPRENT_VAR &&
                             static_cast<VarExprent*>(invexpr->get_instance())->get_index() == 0 &&
                             static_cast<int>(invexpr->get_lst_parameters().size()) == parcount - 1)) {
                            bool equalpars = true;

                            for (std::size_t i = 0; i < invexpr->get_lst_parameters().size(); ++i) {
                                Exprent* parexpr = invexpr->get_lst_parameters()[i];
                                if (parexpr->type != Exprent::EXPRENT_VAR ||
                                    static_cast<VarExprent*>(parexpr)->get_index() != static_cast<int>(i) + (invexpr->is_static() ? 0 : 1)) {
                                    equalpars = false;
                                    break;
                                }
                            }

                            if (equalpars) type = METHOD_ACCESS_METHOD;
                        }
                    }
                } else if (graph->first->exprents->size() == 2) {
                    Exprent* exprent_first = (*graph->first->exprents)[0];
                    Exprent* exprent_second = (*graph->first->exprents)[1];

                    if (exprent_first->type == Exprent::EXPRENT_ASSIGNMENT && exprent_second->type == Exprent::EXPRENT_EXIT) {
                        const MethodDescriptor mtdesc = MethodDescriptor::parse(mt->descriptor());
                        const int parcount = static_cast<int>(mtdesc.params.size());

                        auto* asexpr = static_cast<AssignmentExprent*>(exprent_first);

                        if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD && asexpr->get_right()->type == Exprent::EXPRENT_VAR) {
                            auto* fexpras = static_cast<FieldExprent*>(asexpr->get_left());

                            if ((parcount == 2 && !fexpras->is_static()) || (parcount == 1 && fexpras->is_static())) {
                                if (fexpras->get_classname() == node->class_struct->qualified_name) {   // FIXME (the original's): check for private flag of the field
                                    if (fexpras->is_static() ||
                                        (fexpras->get_instance()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(fexpras->get_instance())->get_index() == 0)) {
                                        if (static_cast<VarExprent*>(asexpr->get_right())->get_index() == parcount - 1) {
                                            auto* exexpr = static_cast<ExitExprent*>(exprent_second);
                                            if (exexpr->get_exit_type() == ExitExprent::EXIT_RETURN && exexpr->get_value() != nullptr) {
                                                if (exexpr->get_value()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(asexpr->get_right())->get_index() == parcount - 1) {
                                                    type = METHOD_ACCESS_FIELD_SET;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (type != METHOD_ACCESS_NORMAL) {
        map_method_type_.put(method, type);
    } else {
        map_method_type_.remove(method);
    }
}

void NestedMemberAccess::eliminate_static_access(ClassNode* node) {
    if (node->type == ClassNode::CLASS_LAMBDA) return;

    for (MethodWrapper* meth : node->get_wrapper()->get_methods()) {
        if (meth->root != nullptr) {
            bool replaced = false;

            DirectGraph* graph = meth->get_or_build_graph();

            std::set<DirectNode*> set_visited;
            std::deque<DirectNode*> stack;

            stack.push_back(graph->first);

            while (!stack.empty()) {   // TODO (the original's): replace with interface iterator?
                DirectNode* nd = stack.front();
                stack.pop_front();

                if (set_visited.contains(nd)) continue;
                set_visited.insert(nd);

                for (std::size_t i = 0; i < nd->exprents->size(); ++i) {
                    Exprent* exprent = (*nd->exprents)[i];

                    replaced |= replace_invocations(node, meth, exprent);

                    if (exprent->type == Exprent::EXPRENT_INVOCATION) {
                        Exprent* ret = replace_access_exprent(node, meth, static_cast<InvocationExprent*>(exprent));
                        if (ret != nullptr) {
                            (*nd->exprents)[i] = ret;
                            replaced = true;
                        }
                    }
                }

                for (DirectNode* ndx : nd->succs) stack.push_back(ndx);
            }

            if (replaced) compute_method_type(node, meth);
        }
    }

    for (ClassNode* child : node->nested) eliminate_static_access(child);
}

bool NestedMemberAccess::replace_invocations(ClassNode* caller, MethodWrapper* meth, Exprent* exprent) {
    bool res = false;

    for (Exprent* expr : exprent->get_all_exprents()) res |= replace_invocations(caller, meth, expr);

    while (true) {
        bool found = false;

        for (Exprent* expr : exprent->get_all_exprents()) {
            if (expr->type == Exprent::EXPRENT_INVOCATION) {
                Exprent* newexpr = replace_access_exprent(caller, meth, static_cast<InvocationExprent*>(expr));
                if (newexpr != nullptr) {
                    exprent->replace_exprent(expr, newexpr);
                    found = true;
                    res = true;
                    break;
                }
            }
        }

        if (!found) break;
    }

    return res;
}

bool NestedMemberAccess::same_tree(ClassNode* caller, ClassNode* callee) {
    if (caller->class_struct->qualified_name == callee->class_struct->qualified_name) return false;

    while (caller->parent != nullptr) caller = caller->parent;

    while (callee->parent != nullptr) callee = callee->parent;

    return caller == callee;
}

Exprent* NestedMemberAccess::replace_access_exprent(ClassNode* caller, MethodWrapper* methdest, InvocationExprent* invexpr) {
    ClassNode* node = DecompilerContext::current().class_processor->get_class_node(invexpr->get_classname());

    MethodWrapper* methsource = nullptr;
    if (node != nullptr && node->get_wrapper() != nullptr) {
        methsource = node->get_wrapper()->get_method_wrapper(invexpr->get_name(), invexpr->get_string_descriptor());
    }

    if (methsource == nullptr || !map_method_type_.contains_key(methsource)) return nullptr;

    // if same method, return
    if (node->class_struct->qualified_name == caller->class_struct->qualified_name && methsource->method_struct->name() == methdest->method_struct->name() &&
        methsource->method_struct->descriptor() == methdest->method_struct->descriptor()) {
        // no recursive invocations permitted!
        return nullptr;
    }

    const int type = *map_method_type_.get(methsource);

    //		// FIXME (the original's): impossible case. METHOD_ACCESS_NORMAL is not saved in the map
    //		if(type == METHOD_ACCESS_NORMAL) {
    //			return null;
    //		}

    if (!same_tree(caller, node)) return nullptr;

    DirectGraph* graph = methsource->get_or_build_graph();
    Exprent* source = (*graph->first->exprents)[0];

    Exprent* retexprent = nullptr;

    switch (type) {
        case METHOD_ACCESS_FIELD_GET: {
            auto* exsource = static_cast<ExitExprent*>(source);

            if (exsource->get_value()->type == Exprent::EXPRENT_VAR) {   // qualified this
                auto* var = static_cast<VarExprent*>(exsource->get_value());
                const std::string varname = methsource->varproc->get_var_name(VarVersionPair(var)).value();

                if (!methdest->set_outer_var_names.contains(varname)) {
                    VarNamesCollector vnc;
                    vnc.add_name(varname);
                    methdest->varproc->refresh_var_names(vnc);

                    methdest->set_outer_var_names.insert(varname);
                }

                const int index = methdest->counter->get_counter_and_increment(CounterContainer::VAR_COUNTER);
                VarExprent* ret = make<VarExprent>(index, var->get_var_type(), methdest->varproc);
                methdest->varproc->set_var_name(VarVersionPair(index, 0), varname);

                retexprent = ret;
            } else {   // field
                auto* ret = static_cast<FieldExprent*>(exsource->get_value()->copy());
                if (!ret->is_static()) ret->replace_exprent(ret->get_instance(), invexpr->get_lst_parameters()[0]);

                retexprent = ret;
            }
            break;
        }
        case METHOD_ACCESS_FIELD_SET: {
            AssignmentExprent* ret;
            if (source->type == Exprent::EXPRENT_EXIT) {
                auto* extex = static_cast<ExitExprent*>(source);
                ret = static_cast<AssignmentExprent*>(extex->get_value()->copy());
            } else {
                ret = static_cast<AssignmentExprent*>(source->copy());
            }

            auto* fexpr = static_cast<FieldExprent*>(ret->get_left());

            if (fexpr->is_static()) {
                ret->replace_exprent(ret->get_right(), invexpr->get_lst_parameters()[0]);
            } else {
                ret->replace_exprent(ret->get_right(), invexpr->get_lst_parameters()[1]);
                fexpr->replace_exprent(fexpr->get_instance(), invexpr->get_lst_parameters()[0]);
            }

            retexprent = ret;
            break;
        }
        case METHOD_ACCESS_METHOD: {
            if (source->type == Exprent::EXPRENT_EXIT) source = static_cast<ExitExprent*>(source)->get_value();

            auto* invret = static_cast<InvocationExprent*>(source->copy());

            std::size_t index = 0;
            if (!invret->is_static()) {
                invret->replace_exprent(invret->get_instance(), invexpr->get_lst_parameters()[0]);
                index = 1;
            }

            for (std::size_t i = 0; i < invret->get_lst_parameters().size(); ++i) {
                invret->replace_exprent(invret->get_lst_parameters()[i], invexpr->get_lst_parameters()[i + index]);
            }

            retexprent = invret;
            break;
        }
        default:
            break;
    }

    if (retexprent != nullptr) {
        // hide synthetic access method
        bool hide = true;

        if (node->type == ClassNode::CLASS_ROOT || (node->access & cc::ACC_STATIC) != 0) {
            const StructMethod* mt = methsource->method_struct;
            if (!mt->is_synthetic()) hide = false;
        }

        if (hide) {
            node->get_wrapper()->get_hidden_members().insert(InterpreterUtil::make_unique_key(invexpr->get_name(), invexpr->get_string_descriptor()));
        }
    }

    return retexprent;
}

}  // namespace cb::java::decompiler
