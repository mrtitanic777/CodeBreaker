// SimplifyExprentsHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE
// in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <map>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/fast_sets.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/structs.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

MatchEngine SimplifyExprentsHelper::class14_builder;

namespace {
// The original parses the 1.4 class literal pattern in a static initializer; here at first use (the shared VarType constants it refers to are
// then certainly built).
void init_class14_builder() {
    static const bool done = [] {
        SimplifyExprentsHelper::class14_builder.parse(
            "statement type:if iftype:if exprsize:-1\n"
            " exprent position:head type:if\n"
            "  exprent type:function functype:eq\n"
            "   exprent type:field name:$field$\n"
            "   exprent type:constant consttype:null\n"
            " statement type:basicblock\n"
            "  exprent position:-1 type:assignment ret:$assignfield$\n"
            "   exprent type:var index:$var$\n"
            "   exprent type:field name:$field$\n"
            " statement type:sequence statsize:2\n"
            "  statement type:trycatch\n"
            "   statement type:basicblock exprsize:1\n"
            "    exprent type:assignment\n"
            "     exprent type:var index:$var$\n"
            "     exprent type:invocation invclass:java/lang/Class signature:forName(Ljava/lang/String;)Ljava/lang/Class;\n"
            "      exprent position:0 type:constant consttype:string constvalue:$classname$\n"
            "   statement type:basicblock exprsize:1\n"
            "    exprent type:exit exittype:throw\n"
            "  statement type:basicblock exprsize:1\n"
            "   exprent type:assignment\n"
            "    exprent type:field name:$field$\n"
            "    exprent type:var index:$var$");
        return true;
    }();
    (void)done;
}

ClassNode* root_class(const std::string& name) {
    ClassesProcessor* proc = DecompilerContext::current().class_processor;
    if (proc == nullptr) throw std::runtime_error("NullPointerException: no class processor");
    ClassNode** found = proc->get_map_root_classes().get(name);
    return found == nullptr ? nullptr : *found;
}
}  // namespace

bool SimplifyExprentsHelper::simplify_stack_vars_statement(Statement* stat, std::set<int>& set_reordered_ifs, SSAConstructorSparseEx* ssa,
                                                           const StructClass* cl) {
    bool res = false;

    if (!stat->get_exprents()) {
        bool process_class14 = DecompilerContext::option(pref::DECOMPILE_CLASS_1_4);

        while (true) {
            bool changed = false;

            for (Statement* st : stat->get_stats()) {
                res |= simplify_stack_vars_statement(st, set_reordered_ifs, ssa, cl);

                // collapse composed if's
                if ((changed = IfHelper::merge_ifs(st, set_reordered_ifs))) {
                    break;
                }

                // collapse iff ?: statement
                if ((changed = build_iff(st, ssa))) {
                    break;
                }

                // collapse inlined .class property in version 1.4 and before
                if (process_class14 && (changed = collapse_inlined_class14(st))) {
                    break;
                }
            }

            res |= changed;

            if (!changed) {
                break;
            }
        }
    } else {
        res |= simplify_stack_vars_exprents(*stat->get_exprents(), cl);
    }

    return res;
}

bool SimplifyExprentsHelper::simplify_stack_vars_exprents(std::vector<Exprent*>& list, const StructClass* cl) {
    bool res = false;

    int index = 0;

    auto erase_at = [&list](int i) { list.erase(list.begin() + i); };

    while (index < static_cast<int>(list.size())) {
        Exprent* current = list[static_cast<std::size_t>(index)];

        Exprent* ret = is_simple_constructor_invocation(current);
        if (ret != nullptr) {
            list[static_cast<std::size_t>(index)] = ret;
            res = true;

            continue;
        }

        // lambda expression (Java 8)
        ret = is_lambda(current, cl);
        if (ret != nullptr) {
            list[static_cast<std::size_t>(index)] = ret;
            res = true;

            continue;
        }

        // remove monitor exit
        if (is_monitor_exit(current)) {
            erase_at(index);
            res = true;

            continue;
        }

        // trivial assignment of a stack variable
        if (is_trivial_stack_assignment(current)) {
            erase_at(index);
            res = true;

            continue;
        }

        if (index == static_cast<int>(list.size()) - 1) {
            break;
        }

        Exprent* next = list[static_cast<std::size_t>(index) + 1];

        // constructor invocation
        if (is_constructor_invocation_remote(list, index)) {
            erase_at(index);
            res = true;

            continue;
        }

        // remove getClass() invocation, which is part of a qualified new
        if (DecompilerContext::option(pref::REMOVE_GET_CLASS_NEW)) {
            if (is_qualified_new_get_class(current, next)) {
                erase_at(index);
                res = true;

                continue;
            }
        }

        // direct initialization of an array
        int arrcount = is_array_initializer(list, index);
        if (arrcount > 0) {
            for (int i = 0; i < arrcount; i++) {
                erase_at(index + 1);
            }
            res = true;

            continue;
        }

        // add array initializer expression
        if (add_array_initializer(current, next)) {
            erase_at(index + 1);
            res = true;

            continue;
        }

        // integer ++expr and --expr  (except for vars!)
        Exprent* func = is_ppi_or_mmi(current);
        if (func != nullptr) {
            list[static_cast<std::size_t>(index)] = func;
            res = true;

            continue;
        }

        // expr++ and expr--
        if (is_ipp_or_imm(current, next)) {
            erase_at(index + 1);
            res = true;

            continue;
        }

        // assignment on stack
        if (is_stack_assignement(current, next)) {
            erase_at(index + 1);
            res = true;

            continue;
        }

        if (!first_invocation_ && is_stack_assignement2(current, next)) {
            erase_at(index + 1);
            res = true;

            continue;
        }

        index++;
    }

    return res;
}

bool SimplifyExprentsHelper::add_array_initializer(Exprent* first, Exprent* second) {
    if (first->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(first);

        if (as->get_right()->type == Exprent::EXPRENT_NEW && as->get_left()->type == Exprent::EXPRENT_VAR) {
            NewExprent* newex = static_cast<NewExprent*>(as->get_right());

            if (!newex->get_lst_array_elements().empty()) {
                VarExprent* arrvar = static_cast<VarExprent*>(as->get_left());

                if (second->type == Exprent::EXPRENT_ASSIGNMENT) {
                    AssignmentExprent* aas = static_cast<AssignmentExprent*>(second);
                    if (aas->get_left()->type == Exprent::EXPRENT_ARRAY) {
                        ArrayExprent* arrex = static_cast<ArrayExprent*>(aas->get_left());
                        if (arrex->get_array()->type == Exprent::EXPRENT_VAR && arrvar->equals(arrex->get_array()) &&
                            arrex->get_index()->type == Exprent::EXPRENT_CONST) {
                            int constvalue = static_cast<ConstExprent*>(arrex->get_index())->get_int_value();

                            if (constvalue < static_cast<int>(newex->get_lst_array_elements().size())) {
                                Exprent* init = at(newex->get_lst_array_elements(), constvalue);
                                if (init->type == Exprent::EXPRENT_CONST) {
                                    ConstExprent* cinit = static_cast<ConstExprent*>(init);

                                    VarType arrtype = newex->get_new_type().decrease_array_dim();

                                    ConstExprent* defaultval = ExprProcessor::get_default_array_value(arrtype);

                                    if (cinit->equals(defaultval)) {
                                        Exprent* tempexpr = aas->get_right();

                                        if (!tempexpr->contains_exprent(arrvar)) {
                                            newex->get_lst_array_elements()[static_cast<std::size_t>(constvalue)] = tempexpr;

                                            if (tempexpr->type == Exprent::EXPRENT_NEW) {
                                                NewExprent* tempnewex = static_cast<NewExprent*>(tempexpr);
                                                int dims = newex->get_new_type().array_dim;
                                                if (dims > 1 && !tempnewex->get_lst_array_elements().empty()) {
                                                    tempnewex->set_direct_array_init(true);
                                                }
                                            }

                                            return true;
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

    return false;
}

int SimplifyExprentsHelper::is_array_initializer(std::vector<Exprent*>& list, int index) {
    Exprent* current = list[static_cast<std::size_t>(index)];
    if (current->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(current);

        if (as->get_right()->type == Exprent::EXPRENT_NEW && as->get_left()->type == Exprent::EXPRENT_VAR) {
            NewExprent* newex = static_cast<NewExprent*>(as->get_right());

            if (newex->get_expr_type().array_dim > 0 && newex->get_lst_dims().size() == 1 && newex->get_lst_array_elements().empty() &&
                newex->get_lst_dims()[0]->type == Exprent::EXPRENT_CONST) {
                int size = std::get<std::int32_t>(static_cast<ConstExprent*>(newex->get_lst_dims()[0])->get_value());
                if (size == 0) {
                    return 0;
                }

                VarExprent* arrvar = static_cast<VarExprent*>(as->get_left());

                std::map<int, Exprent*> map_init;

                int i = 1;
                while (index + i < static_cast<int>(list.size()) && i <= size) {
                    bool found = false;

                    Exprent* expr = list[static_cast<std::size_t>(index + i)];
                    if (expr->type == Exprent::EXPRENT_ASSIGNMENT) {
                        AssignmentExprent* aas = static_cast<AssignmentExprent*>(expr);
                        if (aas->get_left()->type == Exprent::EXPRENT_ARRAY) {
                            ArrayExprent* arrex = static_cast<ArrayExprent*>(aas->get_left());
                            if (arrex->get_array()->type == Exprent::EXPRENT_VAR && arrvar->equals(arrex->get_array()) &&
                                arrex->get_index()->type == Exprent::EXPRENT_CONST) {
                                int constvalue = static_cast<ConstExprent*>(arrex->get_index())->get_int_value();  // TODO: check for a number type. Failure extremely improbable, but nevertheless...

                                if (constvalue < size && !map_init.contains(constvalue)) {
                                    if (!aas->get_right()->contains_exprent(arrvar)) {
                                        map_init[constvalue] = aas->get_right();
                                        found = true;
                                    }
                                }
                            }
                        }
                    }

                    if (!found) {
                        break;
                    }

                    i++;
                }

                double fraction = static_cast<double>(map_init.size()) / size;

                if ((arrvar->is_stack() && fraction > 0) || (size <= 7 && fraction >= 0.3) || (size > 7 && fraction >= 0.7)) {
                    std::vector<Exprent*> lst_ret;

                    VarType arrtype = newex->get_new_type().decrease_array_dim();

                    ConstExprent* defaultval = ExprProcessor::get_default_array_value(arrtype);

                    for (int j = 0; j < size; j++) {
                        lst_ret.push_back(defaultval->copy());
                    }

                    int dims = newex->get_new_type().array_dim;
                    for (const auto& ent : map_init) {
                        Exprent* tempexpr = ent.second;
                        lst_ret[static_cast<std::size_t>(ent.first)] = tempexpr;

                        if (tempexpr->type == Exprent::EXPRENT_NEW) {
                            NewExprent* tempnewex = static_cast<NewExprent*>(tempexpr);
                            if (dims > 1 && !tempnewex->get_lst_array_elements().empty()) {
                                tempnewex->set_direct_array_init(true);
                            }
                        }
                    }

                    newex->set_lst_array_elements(lst_ret);

                    return static_cast<int>(map_init.size());
                }
            }
        }
    }

    return 0;
}

bool SimplifyExprentsHelper::is_trivial_stack_assignment(Exprent* first) {
    if (first->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* asf = static_cast<AssignmentExprent*>(first);

        if (asf->get_left()->type == Exprent::EXPRENT_VAR && asf->get_right()->type == Exprent::EXPRENT_VAR) {
            VarExprent* varleft = static_cast<VarExprent*>(asf->get_left());
            VarExprent* varright = static_cast<VarExprent*>(asf->get_right());

            if (varleft->get_index() == varright->get_index() && varleft->is_stack() && varright->is_stack()) {
                return true;
            }
        }
    }

    return false;
}

bool SimplifyExprentsHelper::is_stack_assignement2(Exprent* first, Exprent* second) {  // e.g. 1.4-style class invocation
    if (first->type == Exprent::EXPRENT_ASSIGNMENT && second->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* asf = static_cast<AssignmentExprent*>(first);
        AssignmentExprent* ass = static_cast<AssignmentExprent*>(second);

        if (asf->get_left()->type == Exprent::EXPRENT_VAR && ass->get_right()->type == Exprent::EXPRENT_VAR && asf->get_left()->equals(ass->get_right()) &&
            static_cast<VarExprent*>(asf->get_left())->is_stack()) {
            if (ass->get_left()->type != Exprent::EXPRENT_VAR || !static_cast<VarExprent*>(ass->get_left())->is_stack()) {
                asf->set_right(make<AssignmentExprent>(ass->get_left(), asf->get_right(), ass->bytecode));
                return true;
            }
        }
    }

    return false;
}

bool SimplifyExprentsHelper::is_stack_assignement(Exprent* first, Exprent* second) {
    if (first->type == Exprent::EXPRENT_ASSIGNMENT && second->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* asf = static_cast<AssignmentExprent*>(first);
        AssignmentExprent* ass = static_cast<AssignmentExprent*>(second);

        while (true) {
            if (asf->get_right()->equals(ass->get_right())) {
                if ((asf->get_left()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(asf->get_left())->is_stack()) &&
                    (ass->get_left()->type != Exprent::EXPRENT_VAR || !static_cast<VarExprent*>(ass->get_left())->is_stack())) {
                    if (!ass->get_left()->contains_exprent(asf->get_left())) {
                        asf->set_right(ass);
                        return true;
                    }
                }
            }
            if (asf->get_right()->type == Exprent::EXPRENT_ASSIGNMENT) {
                asf = static_cast<AssignmentExprent*>(asf->get_right());
            } else {
                break;
            }
        }
    }

    return false;
}

Exprent* SimplifyExprentsHelper::is_ppi_or_mmi(Exprent* first) {
    if (first->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(first);

        if (as->get_right()->type == Exprent::EXPRENT_FUNCTION) {
            FunctionExprent* func = static_cast<FunctionExprent*>(as->get_right());

            if (func->get_func_type() == FunctionExprent::FUNCTION_ADD || func->get_func_type() == FunctionExprent::FUNCTION_SUB) {
                Exprent* econd = at(func->get_lst_operands(), 0);
                Exprent* econst = at(func->get_lst_operands(), 1);

                if (econst->type != Exprent::EXPRENT_CONST && econd->type == Exprent::EXPRENT_CONST && func->get_func_type() == FunctionExprent::FUNCTION_ADD) {
                    econd = econst;
                    econst = at(func->get_lst_operands(), 0);
                }

                if (econst->type == Exprent::EXPRENT_CONST && static_cast<ConstExprent*>(econst)->has_value_one()) {
                    Exprent* left = as->get_left();

                    if (left->type != Exprent::EXPRENT_VAR && left->equals(econd)) {
                        FunctionExprent* ret = make<FunctionExprent>(
                            func->get_func_type() == FunctionExprent::FUNCTION_ADD ? FunctionExprent::FUNCTION_PPI : FunctionExprent::FUNCTION_MMI, econd,
                            func->bytecode);
                        ret->set_implicit_type(VarType::INT);
                        return ret;
                    }
                }
            }
        }
    }

    return nullptr;
}

bool SimplifyExprentsHelper::is_ipp_or_imm(Exprent* first, Exprent* second) {
    if (first->type == Exprent::EXPRENT_ASSIGNMENT && second->type == Exprent::EXPRENT_FUNCTION) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(first);
        FunctionExprent* in = static_cast<FunctionExprent*>(second);

        if ((in->get_func_type() == FunctionExprent::FUNCTION_MMI || in->get_func_type() == FunctionExprent::FUNCTION_PPI) &&
            at(in->get_lst_operands(), 0)->equals(as->get_right())) {
            if (in->get_func_type() == FunctionExprent::FUNCTION_MMI) {
                in->set_func_type(FunctionExprent::FUNCTION_IMM);
            } else {
                in->set_func_type(FunctionExprent::FUNCTION_IPP);
            }
            as->set_right(in);

            return true;
        }
    }

    return false;
}

bool SimplifyExprentsHelper::is_monitor_exit(Exprent* first) {
    if (first->type == Exprent::EXPRENT_MONITOR) {
        MonitorExprent* monexpr = static_cast<MonitorExprent*>(first);
        if (monexpr->get_mon_type() == MonitorExprent::MONITOR_EXIT && monexpr->get_value()->type == Exprent::EXPRENT_VAR &&
            !static_cast<VarExprent*>(monexpr->get_value())->is_stack()) {
            return true;
        }
    }

    return false;
}

bool SimplifyExprentsHelper::is_qualified_new_get_class(Exprent* first, Exprent* second) {
    if (first->type == Exprent::EXPRENT_INVOCATION) {
        InvocationExprent* invexpr = static_cast<InvocationExprent*>(first);

        if (!invexpr->is_static() && nn(invexpr->get_instance())->type == Exprent::EXPRENT_VAR && invexpr->get_name() == "getClass" &&
            invexpr->get_string_descriptor() == "()Ljava/lang/Class;") {
            std::vector<Exprent*> lst_exprents = second->get_all_exprents();
            lst_exprents.push_back(second);

            for (Exprent* expr : lst_exprents) {
                if (expr->type == Exprent::EXPRENT_NEW) {
                    NewExprent* nexpr = static_cast<NewExprent*>(expr);
                    if (nexpr->get_constructor() != nullptr && !nexpr->get_constructor()->get_lst_parameters().empty() &&
                        nexpr->get_constructor()->get_lst_parameters()[0]->equals(invexpr->get_instance())) {
                        const std::string classname = nexpr->get_new_type().value.value_or(std::string());
                        ClassNode* node = root_class(classname);
                        if (node != nullptr && node->type != ClassNode::CLASS_ROOT) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

// propagate (var = new X) forward to the <init> invokation
bool SimplifyExprentsHelper::is_constructor_invocation_remote(std::vector<Exprent*>& list, int index) {
    Exprent* current = list[static_cast<std::size_t>(index)];

    if (current->type == Exprent::EXPRENT_ASSIGNMENT) {
        AssignmentExprent* as = static_cast<AssignmentExprent*>(current);

        if (as->get_left()->type == Exprent::EXPRENT_VAR && as->get_right()->type == Exprent::EXPRENT_NEW) {
            NewExprent* newexpr = static_cast<NewExprent*>(as->get_right());
            const VarType& newtype = newexpr->get_new_type();
            VarVersionPair left_paar(static_cast<VarExprent*>(as->get_left()));

            if (newtype.type == cc::TYPE_OBJECT && newtype.array_dim == 0 && newexpr->get_constructor() == nullptr) {
                for (int i = index + 1; i < static_cast<int>(list.size()); i++) {
                    Exprent* remote = list[static_cast<std::size_t>(i)];

                    // <init> invocation
                    if (remote->type == Exprent::EXPRENT_INVOCATION) {
                        InvocationExprent* in = static_cast<InvocationExprent*>(remote);

                        if (in->get_functype() == InvocationExprent::TYP_INIT && nn(in->get_instance())->type == Exprent::EXPRENT_VAR &&
                            as->get_left()->equals(in->get_instance())) {
                            newexpr->set_constructor(in);
                            in->set_instance(nullptr);

                            list[static_cast<std::size_t>(i)] = as->copy();

                            return true;
                        }
                    }

                    // check for variable in use
                    const auto set_vars = remote->get_all_variables();
                    if (set_vars.contains(left_paar)) {  // variable used somewhere in between -> exit, need a better reduced code
                        return false;
                    }
                }
            }
        }
    }

    return false;
}

Exprent* SimplifyExprentsHelper::is_lambda(Exprent* exprent, const StructClass* cl) {
    std::vector<Exprent*> lst = exprent->get_all_exprents();
    for (Exprent* expr : lst) {
        Exprent* ret = is_lambda(expr, cl);
        if (ret != nullptr) {
            exprent->replace_exprent(expr, ret);
        }
    }

    if (exprent->type == Exprent::EXPRENT_INVOCATION) {
        InvocationExprent* in = static_cast<InvocationExprent*>(exprent);

        if (in->get_invocation_typ() == InvocationExprent::INVOKE_DYNAMIC) {
            const std::optional<std::string>& suffix = in->get_invoke_dynamic_class_suffix();
            std::string lambda_class_name = nn(cl)->qualified_name + (suffix ? *suffix : std::string("null"));
            ClassNode* lambda_class = root_class(lambda_class_name);

            if (lambda_class != nullptr) {  // real lambda class found, replace invocation with an anonymous class
                ListStack<Exprent*> no_stack;
                NewExprent* newexp = make<NewExprent>(VarType::parse(lambda_class_name, true), no_stack, 0, in->bytecode);
                newexp->set_constructor(in);
                // note: we don't set the instance to null with in.setInstance(null) like it is done for a common constructor invokation
                // lambda can also be a reference to a virtual method (e.g. String x; ...(x::toString);)
                // in this case instance will hold the corresponding object

                return newexp;
            }
        }
    }

    return nullptr;
}

Exprent* SimplifyExprentsHelper::is_simple_constructor_invocation(Exprent* exprent) {
    std::vector<Exprent*> lst = exprent->get_all_exprents();
    for (Exprent* expr : lst) {
        Exprent* ret = is_simple_constructor_invocation(expr);
        if (ret != nullptr) {
            exprent->replace_exprent(expr, ret);
        }
    }

    if (exprent->type == Exprent::EXPRENT_INVOCATION) {
        InvocationExprent* in = static_cast<InvocationExprent*>(exprent);
        if (in->get_functype() == InvocationExprent::TYP_INIT && nn(in->get_instance())->type == Exprent::EXPRENT_NEW) {
            NewExprent* newexp = static_cast<NewExprent*>(in->get_instance());
            newexp->set_constructor(in);
            in->set_instance(nullptr);
            return newexp;
        }
    }

    return nullptr;
}

bool SimplifyExprentsHelper::build_iff(Statement* stat, SSAConstructorSparseEx* ssa) {
    if (stat->type == Statement::TYPE_IF && !stat->get_exprents()) {
        IfStatement* stif = static_cast<IfStatement*>(stat);

        Exprent* ifheadexpr = stif->get_headexprent();
        Offsets ifheadexpr_bytecode = (ifheadexpr == nullptr ? Offsets{} : ifheadexpr->bytecode);

        if (stif->iftype == IfStatement::IFTYPE_IFELSE) {
            Statement* ifstat = stif->get_ifstat();
            Statement* elsestat = stif->get_elsestat();

            if (ifstat->get_exprents() && ifstat->get_exprents()->size() == 1 && elsestat->get_exprents() && elsestat->get_exprents()->size() == 1 &&
                ifstat->get_all_successor_edges().size() == 1 && elsestat->get_all_successor_edges().size() == 1 &&
                ifstat->get_all_successor_edges()[0]->get_destination() == elsestat->get_all_successor_edges()[0]->get_destination()) {
                Exprent* ifexpr = (*ifstat->get_exprents())[0];
                Exprent* elseexpr = (*elsestat->get_exprents())[0];

                if (ifexpr->type == Exprent::EXPRENT_ASSIGNMENT && elseexpr->type == Exprent::EXPRENT_ASSIGNMENT) {
                    AssignmentExprent* ifas = static_cast<AssignmentExprent*>(ifexpr);
                    AssignmentExprent* elseas = static_cast<AssignmentExprent*>(elseexpr);

                    if (ifas->get_left()->type == Exprent::EXPRENT_VAR && elseas->get_left()->type == Exprent::EXPRENT_VAR) {
                        VarExprent* ifvar = static_cast<VarExprent*>(ifas->get_left());
                        VarExprent* elsevar = static_cast<VarExprent*>(elseas->get_left());

                        if (ifvar->get_index() == elsevar->get_index() && ifvar->is_stack()) {  // ifvar.getIndex() >= VarExprent.STACK_BASE) {
                            bool found = false;

                            for (const auto& ent : nn(ssa)->get_phi().entries()) {
                                if (ent.first.var == ifvar->get_index()) {
                                    if (ent.second->contains(ifvar->get_version()) && ent.second->contains(elsevar->get_version())) {
                                        found = true;
                                        break;
                                    }
                                }
                            }

                            if (found) {
                                std::vector<Exprent*> data;
                                data.insert(data.end(), exprents_of(stif->get_first()).begin(), exprents_of(stif->get_first()).end());

                                FunctionExprent* iif = make<FunctionExprent>(
                                    FunctionExprent::FUNCTION_IIF,
                                    std::vector<Exprent*>{stif->get_headexprent()->get_condition(), ifas->get_right(), elseas->get_right()}, ifheadexpr_bytecode);
                                data.push_back(make<AssignmentExprent>(ifvar, iif, ifheadexpr_bytecode));
                                stif->set_exprents(data);

                                if (stif->get_all_successor_edges().empty()) {
                                    StatEdge* ifedge = ifstat->get_all_successor_edges()[0];
                                    StatEdge* edge = make<StatEdge>(ifedge->get_type(), stif, ifedge->get_destination());

                                    stif->add_successor(edge);
                                    if (ifedge->closure != nullptr) {
                                        ifedge->closure->add_labeled_edge(edge);
                                    }
                                }

                                SequenceHelper::destroy_and_flatten_statement(stif);

                                return true;
                            }
                        }
                    }
                } else if (ifexpr->type == Exprent::EXPRENT_EXIT && elseexpr->type == Exprent::EXPRENT_EXIT) {
                    ExitExprent* ifex = static_cast<ExitExprent*>(ifexpr);
                    ExitExprent* elseex = static_cast<ExitExprent*>(elseexpr);

                    if (ifex->get_exit_type() == elseex->get_exit_type() && ifex->get_value() != nullptr && elseex->get_value() != nullptr &&
                        ifex->get_exit_type() == ExitExprent::EXIT_RETURN) {
                        // throw is dangerous, because of implicit casting to a common superclass
                        // e.g. throws IOException and throw true?new RuntimeException():new IOException(); won't work
                        if (ifex->get_exit_type() == ExitExprent::EXIT_THROW && !(ifex->get_value()->get_expr_type() == elseex->get_value()->get_expr_type())) {  // note: getExprType unreliable at this point!
                            return false;
                        }

                        std::vector<Exprent*> data;
                        data.insert(data.end(), exprents_of(stif->get_first()).begin(), exprents_of(stif->get_first()).end());

                        FunctionExprent* iif = make<FunctionExprent>(
                            FunctionExprent::FUNCTION_IIF, std::vector<Exprent*>{stif->get_headexprent()->get_condition(), ifex->get_value(), elseex->get_value()},
                            ifheadexpr_bytecode);
                        data.push_back(make<ExitExprent>(ifex->get_exit_type(), iif, ifex->get_ret_type(), ifheadexpr_bytecode));
                        stif->set_exprents(data);

                        StatEdge* retedge = ifstat->get_all_successor_edges()[0];
                        stif->add_successor(make<StatEdge>(StatEdge::TYPE_BREAK, stif, retedge->get_destination(),
                                                           retedge->closure == stif ? stif->get_parent() : retedge->closure));

                        SequenceHelper::destroy_and_flatten_statement(stif);

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool SimplifyExprentsHelper::collapse_inlined_class14(Statement* stat) {
    init_class14_builder();

    bool ret = class14_builder.match(stat);
    if (ret) {
        std::string class_name = std::get<std::string>(class14_builder.get_variable_value("$classname$"));
        AssignmentExprent* assfirst = static_cast<AssignmentExprent*>(std::get<IMatchable*>(class14_builder.get_variable_value("$assignfield$")));

        assfirst->replace_exprent(assfirst->get_right(), make<ConstExprent>(VarType::CLASS, ConstExprent::Value{class_name}, Offsets{}));

        std::vector<Exprent*> data;
        data.insert(data.end(), exprents_of(stat->get_first()).begin(), exprents_of(stat->get_first()).end());

        stat->set_exprents(data);

        SequenceHelper::destroy_and_flatten_statement(stat);
    }

    return ret;
}

}  // namespace cb::java::decompiler
