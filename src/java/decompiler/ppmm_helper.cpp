// PPandMMHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>
#include <unordered_set>

#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

bool PPandMMHelper::find_pp_and_mm(RootStatement* root) {
    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    std::deque<DirectNode*> stack;
    stack.push_back(dgraph->first);

    std::unordered_set<DirectNode*> set_visited;

    bool res = false;

    while (!stack.empty()) {
        DirectNode* node = stack.front();
        stack.pop_front();

        if (set_visited.contains(node)) {
            continue;
        }
        set_visited.insert(node);

        res |= process_exprent_list(*node->exprents);

        for (DirectNode* succ : node->succs) stack.push_back(succ);
    }

    return res;
}

bool PPandMMHelper::process_exprent_list(std::vector<Exprent*>& lst) {
    bool result = false;

    for (int i = 0; i < static_cast<int>(lst.size()); i++) {
        Exprent* exprent = lst[static_cast<std::size_t>(i)];
        exprent_replaced_ = false;

        Exprent* retexpr = process_exprent_recursive(exprent);
        if (retexpr != nullptr) {
            lst[static_cast<std::size_t>(i)] = retexpr;

            result = true;
            i--;  // process the same exprent again
        }

        result |= exprent_replaced_;
    }

    return result;
}

Exprent* PPandMMHelper::process_exprent_recursive(Exprent* exprent) {
    bool replaced = true;
    while (replaced) {
        replaced = false;

        for (Exprent* expr : exprent->get_all_exprents()) {
            Exprent* retexpr = process_exprent_recursive(expr);
            if (retexpr != nullptr) {
                exprent->replace_exprent(expr, retexpr);
                replaced = true;
                exprent_replaced_ = true;
                break;
            }
        }
    }

    if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(exprent);

        if (as->get_right()->type == Exprent::EXPRENT_FUNCTION) {
            FunctionExprent* func = static_cast<FunctionExprent*>(as->get_right());

            std::optional<VarType> midlayer;
            if (func->get_func_type() >= FunctionExprent::FUNCTION_I2L && func->get_func_type() <= FunctionExprent::FUNCTION_I2S) {
                midlayer = func->get_simple_cast_type();
                if (at(func->get_lst_operands(), 0)->type == Exprent::EXPRENT_FUNCTION) {
                    func = static_cast<FunctionExprent*>(at(func->get_lst_operands(), 0));
                } else {
                    return nullptr;
                }
            }

            if (func->get_func_type() == FunctionExprent::FUNCTION_ADD || func->get_func_type() == FunctionExprent::FUNCTION_SUB) {
                Exprent* econd = at(func->get_lst_operands(), 0);
                Exprent* econst = at(func->get_lst_operands(), 1);

                if (econst->type != Exprent::EXPRENT_CONST && econd->type == Exprent::EXPRENT_CONST && func->get_func_type() == FunctionExprent::FUNCTION_ADD) {
                    econd = econst;
                    econst = at(func->get_lst_operands(), 0);
                }

                if (econst->type == Exprent::EXPRENT_CONST && static_cast<ConstExprent*>(econst)->has_value_one()) {
                    Exprent* left = as->get_left();

                    VarType condtype = econd->get_expr_type();
                    if (left->equals(econd) && (!midlayer || *midlayer == condtype)) {
                        FunctionExprent* ret = make<FunctionExprent>(
                            func->get_func_type() == FunctionExprent::FUNCTION_ADD ? FunctionExprent::FUNCTION_PPI : FunctionExprent::FUNCTION_MMI, econd,
                            func->bytecode);
                        ret->set_implicit_type(condtype);

                        exprent_replaced_ = true;
                        return ret;
                    }
                }
            }
        }
    }

    return nullptr;
}

}  // namespace cb::java::decompiler
