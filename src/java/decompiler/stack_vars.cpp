// StackVarsProcessor (de.fernflower.modules.decompiler.StackVarsProcessor).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>
#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/stack_vars.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

namespace {
// Integer.equals over two values that may be null
bool equal_ints(const int* a, const int* b) { return a == nullptr ? b == nullptr : (b != nullptr && *a == *b); }

SFormsFastMapDirect* need_live_map(SSAUConstructorSparseEx* ssau, const VarVersionPair& pair) {
    SFormsFastMapDirect* m = ssau->get_live_var_versions_map(pair);
    if (m == nullptr) throw std::runtime_error("no live map for " + pair.to_string());
    return m;
}
}  // namespace

void StackVarsProcessor::simplify_stack_vars(RootStatement* root, const StructMethod* mt, const StructClass* cl) {
    std::set<int> set_reordered_ifs;
    std::unique_ptr<SSAUConstructorSparseEx> ssau;

    while (true) {
        bool found = false;

        SSAConstructorSparseEx ssa;
        ssa.split_variables(root, mt);

        SimplifyExprentsHelper sehelper(ssau == nullptr);
        while (sehelper.simplify_stack_vars_statement(root, set_reordered_ifs, &ssa, cl)) found = true;

        set_versions_to_null(root);

        SequenceHelper::condense_sequences(root);

        ssau = std::make_unique<SSAUConstructorSparseEx>();
        ssau->split_variables(root, mt);

        if (iterate_statements(root, ssau.get())) found = true;

        set_versions_to_null(root);

        if (!found) break;
    }

    // remove unused assignments
    ssau = std::make_unique<SSAUConstructorSparseEx>();
    ssau->split_variables(root, mt);

    iterate_statements(root, ssau.get());

    set_versions_to_null(root);
}

void StackVarsProcessor::set_versions_to_null(Statement* stat) {
    if (!stat->get_exprents()) {
        for (const SeqObject& obj : stat->get_sequential_objects()) {
            if (obj.stat != nullptr) set_versions_to_null(obj.stat);
            else if (obj.expr != nullptr) set_exprent_versions_to_null(obj.expr);
        }
    } else {
        for (Exprent* exprent : *stat->get_exprents()) set_exprent_versions_to_null(exprent);
    }
}

void StackVarsProcessor::set_exprent_versions_to_null(Exprent* exprent) {
    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
    lst.push_back(exprent);

    for (Exprent* expr : lst) {
        if (expr->type == Exprent::EXPRENT_VAR) static_cast<VarExprent*>(expr)->set_version(0);
    }
}

bool StackVarsProcessor::iterate_statements(RootStatement* root, SSAUConstructorSparseEx* ssa) {
    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    bool res = false;

    std::unordered_set<DirectNode*> set_visited;
    std::deque<DirectNode*> stack;
    std::deque<VarValueMap> stack_maps;

    stack.push_back(dgraph->first);
    stack_maps.push_back(VarValueMap());

    while (!stack.empty()) {
        DirectNode* nd = stack.front();
        stack.pop_front();
        VarValueMap map_var_values = std::move(stack_maps.front());
        stack_maps.pop_front();

        if (set_visited.contains(nd)) continue;
        set_visited.insert(nd);

        std::vector<std::vector<Exprent*>*> lst_lists;

        if (!nd->exprents->empty()) lst_lists.push_back(nd->exprents);

        if (nd->succs.size() == 1) {
            DirectNode* ndsucc = nd->succs[0];
            if (ndsucc->type == DirectNode::NODE_TAIL && !ndsucc->exprents->empty()) {
                lst_lists.push_back(nd->succs[0]->exprents);
                nd = ndsucc;
            }
        }

        for (std::size_t i = 0; i < lst_lists.size(); i++) {
            std::vector<Exprent*>& lst = *lst_lists[i];

            int index = 0;
            while (index < static_cast<int>(lst.size())) {
                Exprent* next = nullptr;
                if (index == static_cast<int>(lst.size()) - 1) {
                    if (i < lst_lists.size() - 1) next = lst_lists[i + 1]->at(0);
                } else {
                    next = lst[static_cast<std::size_t>(index + 1)];
                }

                const IterateResult ret = iterate_exprent(lst, index, next, map_var_values, ssa);

                if (ret.index >= 0) index = ret.index;
                else index++;

                res |= (ret.changed == 1);
            }
        }

        for (DirectNode* ndx : nd->succs) {
            stack.push_back(ndx);
            stack_maps.push_back(map_var_values);
        }

        // make sure the 3 special exprent lists in a loop (init, condition, increment) are not empty
        // change loop type if necessary
        if (nd->exprents->empty() &&
            (nd->type == DirectNode::NODE_INIT || nd->type == DirectNode::NODE_CONDITION || nd->type == DirectNode::NODE_INCREMENT)) {
            nd->exprents->push_back(nullptr);

            if (nd->statement->type == Statement::TYPE_DO) {
                DoStatement* loop = static_cast<DoStatement*>(nd->statement);
                if (loop->get_looptype() == DoStatement::LOOP_FOR && loop->get_init_exprent() == nullptr && loop->get_inc_exprent() == nullptr) {  // "downgrade" loop to 'while'
                    loop->set_looptype(DoStatement::LOOP_WHILE);
                }
            }
        }
    }

    return res;
}

Exprent* StackVarsProcessor::is_replaceable_var(Exprent* exprent, const VarValueMap& map_var_values, SSAUConstructorSparseEx*) {
    Exprent* dest = nullptr;

    if (exprent->type == Exprent::EXPRENT_VAR) {
        VarExprent* var = static_cast<VarExprent*>(exprent);
        const auto it = map_var_values.find(VarVersionPair(var));
        if (it != map_var_values.end()) dest = it->second;
    }

    return dest;
}

void StackVarsProcessor::replace_single_var(Exprent* parent, VarExprent* var, Exprent* dest, SSAUConstructorSparseEx* ssau) {
    parent->replace_exprent(var, dest);

    // live sets
    SFormsFastMapDirect* livemap = need_live_map(ssau, VarVersionPair(var));
    const VarVersionSet set_vars = get_all_versions(dest);

    for (const VarVersionPair& varpaar : set_vars.items()) {
        VarVersionNode* node = ssau->get_ssuversions()->nodes.get_with_key(varpaar);
        if (node == nullptr) throw std::runtime_error("no version node " + varpaar.to_string());

        // (the original removes from the list entryList() made, which changes nothing in the map: only the complement is left)
        for (const SFormsFastMapDirect::Entry& ent : node->live->entry_list()) {
            const int key = ent.first;

            if (livemap->contains_key(key)) {
                FastSparseSet<int>* set = ent.second;
                set->complement(livemap->get(key));
            }
        }
    }
}

StackVarsProcessor::IterateResult StackVarsProcessor::iterate_exprent(std::vector<Exprent*>& lst_exprents, int index, Exprent* next, VarValueMap& map_var_values,
                                                                      SSAUConstructorSparseEx* ssau) {
    Exprent* exprent = lst_exprents.at(static_cast<std::size_t>(index));
    int changed = 0;

    for (Exprent* expr_in : exprent->get_all_exprents()) {
        Exprent* expr = expr_in;

        while (true) {
            const ChildResult arr = iterate_child_exprent(expr, exprent, next, map_var_values, ssau);

            Exprent* retexpr = arr.exprent;
            changed |= arr.changed ? 1 : 0;

            const bool is_replaceable = arr.replaceable;

            if (retexpr != nullptr) {
                if (is_replaceable) {
                    replace_single_var(exprent, static_cast<VarExprent*>(expr), retexpr, ssau);
                    expr = retexpr;
                } else {
                    exprent->replace_exprent(expr, retexpr);
                }
                changed = 1;
            }

            if (!is_replaceable) break;
        }
    }

    // no var on the highest level, so no replacing

    VarExprent* left = nullptr;
    Exprent* right = nullptr;

    if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(exprent);
        if (as->get_left()->type == Exprent::EXPRENT_VAR) {
            left = static_cast<VarExprent*>(as->get_left());
            right = as->get_right();
        }
    }

    if (left == nullptr) return {-1, changed};

    const VarVersionPair leftpaar(left);

    std::vector<VarVersionNode*> used_vers;
    const bool notdom = get_used_versions(ssau, leftpaar, used_vers);

    if (!notdom && used_vers.empty()) {
        if (left->is_stack() && (right->type == Exprent::EXPRENT_INVOCATION || right->type == Exprent::EXPRENT_ASSIGNMENT || right->type == Exprent::EXPRENT_NEW)) {
            if (right->type == Exprent::EXPRENT_NEW) {
                // new Object(); permitted
                NewExprent* nexpr = static_cast<NewExprent*>(right);
                if (nexpr->is_anonymous() || nexpr->get_new_type().array_dim > 0 || nexpr->get_new_type().type != cc::TYPE_OBJECT) return {-1, changed};
            }

            lst_exprents[static_cast<std::size_t>(index)] = right;
            return {index + 1, 1};
        } else if (right->type == Exprent::EXPRENT_VAR) {
            lst_exprents.erase(lst_exprents.begin() + index);
            return {index, 1};
        } else {
            return {-1, changed};
        }
    }

    const int useflags = right->get_exprent_use();

    // stack variables only
    if (!left->is_stack() && (right->type != Exprent::EXPRENT_VAR || static_cast<VarExprent*>(right)->is_stack())) {  // special case catch(... ex)
        return {-1, changed};
    }

    if ((useflags & Exprent::MULTIPLE_USES) == 0 && (notdom || used_vers.size() > 1)) return {-1, changed};

    VarVersionsMap map_vars = get_all_var_versions(leftpaar, right, ssau);

    const bool is_self_reference = map_vars.contains_key(leftpaar.var);
    if (is_self_reference && notdom) return {-1, changed};

    std::optional<VarVersionSet> set_next_vars;
    if (next != nullptr) set_next_vars = get_all_versions(next);

    // FIXME: fix the entire method!
    if (right->type != Exprent::EXPRENT_CONST && right->type != Exprent::EXPRENT_VAR && set_next_vars && map_vars.contains_key(leftpaar.var)) {
        for (VarVersionNode* usedvar : used_vers) {
            if (!set_next_vars->contains(VarVersionPair(usedvar->var, usedvar->version))) return {-1, changed};
        }
    }

    map_vars.remove(leftpaar.var);

    bool vernotreplaced = false;
    bool verreplaced = false;

    VarVersionSet set_temp_used_vers;
    for (VarVersionNode* usedvar : used_vers) {
        const VarVersionPair usedver(usedvar->var, usedvar->version);

        if (is_version_to_be_replaced(usedver, map_vars, ssau, leftpaar) &&
            (right->type == Exprent::EXPRENT_CONST || right->type == Exprent::EXPRENT_VAR || right->type == Exprent::EXPRENT_FIELD || !set_next_vars ||
             set_next_vars->contains(usedver))) {
            set_temp_used_vers.add(usedver);
            verreplaced = true;
        } else {
            vernotreplaced = true;
        }
    }

    if (is_self_reference && vernotreplaced) {
        return {-1, changed};
    } else {
        for (const VarVersionPair& usedver : set_temp_used_vers.items()) {
            Exprent* copy = right->copy();

            if (right->type == Exprent::EXPRENT_FIELD && ssau->get_map_field_vars().contains(right->id)) {
                ssau->get_map_field_vars()[copy->id] = ssau->get_map_field_vars().at(right->id);
            }

            map_var_values[usedver] = copy;
        }
    }

    if (!notdom && !vernotreplaced) {
        // remove assignment
        lst_exprents.erase(lst_exprents.begin() + index);
        return {index, 1};
    } else if (verreplaced) {
        return {index + 1, changed};
    } else {
        return {-1, changed};
    }
}

VarVersionSet StackVarsProcessor::get_all_versions(Exprent* exprent) {
    VarVersionSet res;

    std::vector<Exprent*> list_temp = exprent->get_all_exprents(true);
    list_temp.push_back(exprent);

    for (Exprent* expr : list_temp) {
        if (expr->type == Exprent::EXPRENT_VAR) res.add(VarVersionPair(static_cast<VarExprent*>(expr)));
    }

    return res;
}

StackVarsProcessor::ChildResult StackVarsProcessor::iterate_child_exprent(Exprent* exprent, Exprent* parent, Exprent* next, VarValueMap& map_var_values,
                                                                          SSAUConstructorSparseEx* ssau) {
    bool changed = false;

    for (Exprent* expr_in : exprent->get_all_exprents()) {
        Exprent* expr = expr_in;

        while (true) {
            const ChildResult arr = iterate_child_exprent(expr, parent, next, map_var_values, ssau);

            Exprent* retexpr = arr.exprent;
            changed |= arr.changed;

            const bool is_replaceable = arr.replaceable;

            if (retexpr != nullptr) {
                if (is_replaceable) {
                    replace_single_var(exprent, static_cast<VarExprent*>(expr), retexpr, ssau);
                    expr = retexpr;
                } else {
                    exprent->replace_exprent(expr, retexpr);
                }
                changed = true;
            }

            if (!is_replaceable) break;
        }
    }

    Exprent* dest = is_replaceable_var(exprent, map_var_values, ssau);
    if (dest != nullptr) return {dest, true, true};

    VarExprent* left = nullptr;
    Exprent* right = nullptr;

    if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(exprent);
        if (as->get_left()->type == Exprent::EXPRENT_VAR) {
            left = static_cast<VarExprent*>(as->get_left());
            right = as->get_right();
        }
    }

    if (left == nullptr) return {nullptr, changed, false};

    bool is_head_synchronized = false;
    if (next == nullptr && parent->type == Exprent::EXPRENT_MONITOR) {
        MonitorExprent* monexpr = static_cast<MonitorExprent*>(parent);
        if (monexpr->get_mon_type() == MonitorExprent::MONITOR_ENTER && exprent->equals(monexpr->get_value())) is_head_synchronized = true;
    }

    // stack variable or synchronized head exprent
    if (!left->is_stack() && !is_head_synchronized) return {nullptr, changed, false};

    const VarVersionPair leftpaar(left);

    std::vector<VarVersionNode*> used_vers;
    const bool notdom = get_used_versions(ssau, leftpaar, used_vers);

    if (!notdom && used_vers.empty()) return {right, changed, false};

    // stack variables only
    if (!left->is_stack()) return {nullptr, changed, false};

    const int useflags = right->get_exprent_use();

    if ((useflags & Exprent::BOTH_FLAGS) != Exprent::BOTH_FLAGS) return {nullptr, changed, false};

    VarVersionsMap map_vars = get_all_var_versions(leftpaar, right, ssau);

    if (map_vars.contains_key(leftpaar.var) && notdom) return {nullptr, changed, false};

    map_vars.remove(leftpaar.var);

    VarVersionSet set_allowed_vars = get_all_versions(parent);
    if (next != nullptr) {
        for (const VarVersionPair& p : get_all_versions(next).items()) set_allowed_vars.add(p);
    }

    bool vernotreplaced = false;

    VarVersionSet set_temp_used_vers;
    for (VarVersionNode* usedvar : used_vers) {
        const VarVersionPair usedver(usedvar->var, usedvar->version);

        if (is_version_to_be_replaced(usedver, map_vars, ssau, leftpaar) && (right->type == Exprent::EXPRENT_VAR || set_allowed_vars.contains(usedver))) {
            set_temp_used_vers.add(usedver);
        } else {
            vernotreplaced = true;
        }
    }

    if (!notdom && !vernotreplaced) {
        for (const VarVersionPair& usedver : set_temp_used_vers.items()) {
            Exprent* copy = right->copy();

            if (right->type == Exprent::EXPRENT_FIELD && ssau->get_map_field_vars().contains(right->id)) {
                ssau->get_map_field_vars()[copy->id] = ssau->get_map_field_vars().at(right->id);
            }

            map_var_values[usedver] = copy;
        }

        // remove assignment
        return {right, changed, false};
    }

    return {nullptr, changed, false};
}

bool StackVarsProcessor::get_used_versions(SSAUConstructorSparseEx* ssa, const VarVersionPair& var, std::vector<VarVersionNode*>& res) {
    VarVersionsGraph* ssuversions = ssa->get_ssuversions();

    VarVersionNode* varnode = ssuversions->nodes.get_with_key(var);
    if (varnode == nullptr) throw std::runtime_error("no version node " + var.to_string());

    std::unordered_set<VarVersionNode*> set_visited;
    std::unordered_set<VarVersionNode*> set_not_doms;

    std::deque<VarVersionNode*> stack;
    stack.push_back(varnode);

    while (!stack.empty()) {
        VarVersionNode* nd = stack.front();
        stack.pop_front();

        set_visited.insert(nd);

        if (nd != varnode && (nd->flags & VarVersionNode::FLAG_PHANTOM_FINEXIT) == 0) res.push_back(nd);

        for (VarVersionEdge* edge : nd->succs) {
            VarVersionNode* succ = edge->dest;

            if (!set_visited.contains(edge->dest)) {
                bool is_dominated = true;
                for (VarVersionEdge* prededge : succ->preds) {
                    if (!set_visited.contains(prededge->source)) {
                        is_dominated = false;
                        break;
                    }
                }

                if (is_dominated) stack.push_back(succ);
                else set_not_doms.insert(succ);
            }
        }
    }

    for (VarVersionNode* visited : set_visited) set_not_doms.erase(visited);

    return !set_not_doms.empty();
}

bool StackVarsProcessor::is_version_to_be_replaced(const VarVersionPair& usedvar, const VarVersionsMap& map_vars, SSAUConstructorSparseEx* ssau,
                                                   const VarVersionPair& leftpaar) {
    VarVersionsGraph* ssuversions = ssau->get_ssuversions();

    SFormsFastMapDirect* map_live_vars = ssau->get_live_var_versions_map(usedvar);
    if (map_live_vars == nullptr) {
        // dummy version, predecessor of a phi node
        return false;
    }

    // compare protected ranges
    const std::map<VarVersionPair, int>& first_range = ssau->get_map_version_first_range();
    const auto left_it = first_range.find(leftpaar);
    const auto used_it = first_range.find(usedvar);
    if (!equal_ints(left_it == first_range.end() ? nullptr : &left_it->second, used_it == first_range.end() ? nullptr : &used_it->second)) return false;

    for (const auto& ent : map_vars.entries()) {
        FastSparseSet<int>* liveverset = map_live_vars->get(ent.first);
        if (liveverset == nullptr) return false;

        if (!ent.second) throw std::runtime_error("StackVarsProcessor: the versions of the assigned variable");

        LinkedSet<VarVersionNode*> domset;
        for (const VarVersionPair& verpaar : ent.second->items()) domset.add(ssuversions->nodes.get_with_key(verpaar));

        bool isdom = false;

        auto it = liveverset->iterator();
        while (it.has_next()) {
            const int livever = it.next();
            VarVersionNode* node = ssuversions->nodes.get_with_key(VarVersionPair(ent.first, livever));

            if (ssuversions->is_dominator_set(node, domset)) {
                isdom = true;
                break;
            }
        }

        if (!isdom) return false;
    }

    return true;
}

StackVarsProcessor::VarVersionsMap StackVarsProcessor::get_all_var_versions(const VarVersionPair& leftvar, Exprent* exprent, SSAUConstructorSparseEx* ssau) {
    VarVersionsMap map;

    SFormsFastMapDirect* map_live_vars = need_live_map(ssau, leftvar);

    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
    lst.push_back(exprent);

    for (Exprent* expr : lst) {
        if (expr->type == Exprent::EXPRENT_VAR) {
            const int varindex = static_cast<VarExprent*>(expr)->get_index();
            if (leftvar.var != varindex) {
                if (map_live_vars->contains_key(varindex)) {
                    VarVersionSet verset;
                    auto it = map_live_vars->get(varindex)->iterator();
                    while (it.has_next()) verset.add(VarVersionPair(varindex, it.next()));
                    map.put(varindex, verset);
                } else {
                    throw std::runtime_error("inkonsistent live map!");
                }
            } else {
                map.put(varindex, std::nullopt);
            }
        } else if (expr->type == Exprent::EXPRENT_FIELD) {
            if (ssau->get_map_field_vars().contains(expr->id)) {
                const int varindex = ssau->get_map_field_vars().at(expr->id);
                if (map_live_vars->contains_key(varindex)) {
                    VarVersionSet verset;
                    auto it = map_live_vars->get(varindex)->iterator();
                    while (it.has_next()) verset.add(VarVersionPair(varindex, it.next()));
                    map.put(varindex, verset);
                }
            }
        }
    }

    return map;
}

}  // namespace cb::java::decompiler
