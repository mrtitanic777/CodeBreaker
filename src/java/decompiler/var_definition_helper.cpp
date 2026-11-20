// VarDefinitionHelper (de.fernflower.modules.decompiler.vars).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

VarDefinitionHelper::VarDefinitionHelper(Statement* root, const StructMethod* mt, VarProcessor* varproc) : varproc_(varproc) {
    DecompilerContext& ctx = DecompilerContext::current();
    VarNamesCollector* vc = ctx.var_names_collector;

    const bool thisvar = !mt->has_modifier(cc::ACC_STATIC);

    const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());

    int paramcount = 0;
    if (thisvar) paramcount = 1;
    paramcount += static_cast<int>(md.params.size());

    // method parameters are implicitly defined
    int varindex = 0;
    for (int i = 0; i < paramcount; i++) {
        impl_def_vars_.insert(varindex);
        varproc->set_var_name(VarVersionPair(varindex, 0), vc->get_free_name(varindex));

        if (thisvar) {
            if (i == 0) varindex++;
            else varindex += md.params[static_cast<std::size_t>(i - 1)].stack_size;
        } else {
            varindex += md.params[static_cast<std::size_t>(i)].stack_size;
        }
    }

    if (thisvar) {
        const StructClass* current_class = ctx.current_class;
        varproc->get_this_vars().put(VarVersionPair(0, 0), current_class->qualified_name);
        varproc->set_var_name(VarVersionPair(0, 0), "this");
        vc->add_name("this");
    }

    // catch variables are implicitly defined
    std::deque<Statement*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Statement* st = stack.front();
        stack.pop_front();

        std::vector<VarExprent*>* lst_vars = nullptr;
        if (st->type == Statement::TYPE_CATCHALL) lst_vars = &static_cast<CatchAllStatement*>(st)->get_vars();
        else if (st->type == Statement::TYPE_TRYCATCH) lst_vars = &static_cast<CatchStatement*>(st)->get_vars();

        if (lst_vars != nullptr) {
            for (VarExprent* var : *lst_vars) {
                impl_def_vars_.insert(var->get_index());
                varproc->set_var_name(VarVersionPair(var), vc->get_free_name(var->get_index()));
                var->set_definition(true);
            }
        }

        for (Statement* s : st->get_stats()) stack.push_back(s);
    }

    init_statement(root);
}

void VarDefinitionHelper::set_var_definitions() {
    VarNamesCollector* vc = DecompilerContext::current().var_names_collector;

    for (const auto& en : map_var_def_statements_.entries()) {
        Statement* stat = en.second;
        const int index = en.first;

        if (impl_def_vars_.contains(index)) {
            // already implicitly defined
            continue;
        }

        varproc_->set_var_name(VarVersionPair(index, 0), vc->get_free_name(index));

        // special case for
        if (stat->type == Statement::TYPE_DO) {
            DoStatement* dstat = static_cast<DoStatement*>(stat);
            if (dstat->get_looptype() == DoStatement::LOOP_FOR) {
                if (dstat->get_init_exprent() != nullptr && set_definition(dstat->get_init_exprent(), index)) {
                    continue;
                } else {
                    const std::vector<Exprent*> lst_special = {dstat->get_condition_exprent(), dstat->get_inc_exprent()};
                    for (VarExprent* var : get_all_vars(lst_special)) {
                        if (var->get_index() == index) {
                            stat = stat->get_parent();
                            break;
                        }
                    }
                }
            }
        }

        Statement* first = find_first_block(stat, index);
        std::vector<Exprent*>* lst;

        if (first == nullptr) {
            lst = &stat->get_var_definitions();
        } else if (!first->get_exprents()) {
            lst = &first->get_var_definitions();
        } else {
            lst = &*first->get_exprents();
        }

        bool defset = false;

        // search for the first assignement to var [index]
        int addindex = 0;
        for (Exprent* expr : *lst) {
            if (set_definition(expr, index)) {
                defset = true;
                break;
            } else {
                bool foundvar = false;
                for (Exprent* exp : expr->get_all_exprents(true)) {
                    if (exp->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(exp)->get_index() == index) {
                        foundvar = true;
                        break;
                    }
                }

                if (foundvar) break;
            }

            addindex++;
        }

        if (!defset) {
            VarExprent* var = make<VarExprent>(index, varproc_->get_var_type(VarVersionPair(index, 0)), varproc_);
            var->set_definition(true);

            lst->insert(lst->begin() + addindex, var);
        }
    }
}

// *****************************************************************************
// private methods
// *****************************************************************************

Statement* VarDefinitionHelper::find_first_block(Statement* stat, int varindex) {
    std::deque<Statement*> stack;
    stack.push_back(stat);

    while (!stack.empty()) {
        Statement* st = stack.front();
        stack.pop_front();

        if (stack.empty() || map_statement_vars_.at(st->id).contains(varindex)) {
            if (st->is_labeled() && !stack.empty()) return st;

            if (st->get_exprents()) {
                return st;
            } else {
                stack.clear();

                switch (st->type) {
                    case Statement::TYPE_SEQUENCE:
                        stack.insert(stack.begin(), st->get_stats().begin(), st->get_stats().end());
                        break;
                    case Statement::TYPE_IF:
                    case Statement::TYPE_ROOT:
                    case Statement::TYPE_SWITCH:
                    case Statement::TYPE_SYNCRONIZED:
                        stack.push_back(st->get_first());
                        break;
                    default:
                        return st;
                }
            }
        }
    }

    return nullptr;
}

IntHashSet VarDefinitionHelper::init_statement(Statement* stat) {
    IntHashMap<int> map_count;
    std::vector<VarExprent*> condlst;

    if (!stat->get_exprents()) {
        // recurse on children statements
        std::vector<int> child_vars;
        std::vector<Exprent*> curr_vars;

        for (const SeqObject& obj : stat->get_sequential_objects()) {
            if (obj.stat != nullptr) {
                Statement* st = obj.stat;

                for (int v : init_statement(st).items()) child_vars.push_back(v);

                if (st->type == Statement::TYPE_DO) {
                    DoStatement* dost = static_cast<DoStatement*>(st);
                    if (dost->get_looptype() != DoStatement::LOOP_FOR && dost->get_looptype() != DoStatement::LOOP_DO) {
                        curr_vars.push_back(dost->get_condition_exprent());
                    }
                } else if (st->type == Statement::TYPE_CATCHALL) {
                    CatchAllStatement* fin = static_cast<CatchAllStatement*>(st);
                    if (fin->is_finally() && fin->get_monitor() != nullptr) curr_vars.push_back(fin->get_monitor());
                }
            } else if (obj.expr != nullptr) {
                curr_vars.push_back(obj.expr);
            }
        }

        // children statements
        for (int index : child_vars) {
            const int* count = map_count.get(index);
            map_count.put(index, (count == nullptr ? 0 : *count) + 1);
        }

        condlst = get_all_vars(curr_vars);
    } else {
        condlst = get_all_vars(*stat->get_exprents());
    }

    // this statement
    for (VarExprent* var : condlst) map_count.put(var->get_index(), 2);

    IntHashSet set = IntHashSet::of(map_count.keys());

    // put all variables defined in this statement into the set
    for (const auto& en : map_count.entries()) {
        if (en.second > 1) map_var_def_statements_.put(en.first, stat);
    }

    map_statement_vars_[stat->id] = set;

    return set;
}

std::vector<VarExprent*> VarDefinitionHelper::get_all_vars(const std::vector<Exprent*>& lst) {
    std::vector<VarExprent*> res;
    std::vector<Exprent*> list_temp;

    for (Exprent* expr : lst) {
        if (expr == nullptr) throw std::runtime_error("VarDefinitionHelper: null expression");
        for (Exprent* e : expr->get_all_exprents(true)) list_temp.push_back(e);
        list_temp.push_back(expr);
    }

    for (Exprent* exprent : list_temp) {
        if (exprent->type == Exprent::EXPRENT_VAR) res.push_back(static_cast<VarExprent*>(exprent));
    }

    return res;
}

bool VarDefinitionHelper::set_definition(Exprent* expr, int index) {
    if (expr->type == Exprent::EXPRENT_ASSIGNMENT) {
        Exprent* left = static_cast<AssignmentExprent*>(expr)->get_left();
        if (left->type == Exprent::EXPRENT_VAR) {
            VarExprent* var = static_cast<VarExprent*>(left);
            if (var->get_index() == index) {
                var->set_definition(true);
                return true;
            }
        }
    }
    return false;
}

}  // namespace cb::java::decompiler
