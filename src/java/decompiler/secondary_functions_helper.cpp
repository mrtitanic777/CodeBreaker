// SecondaryFunctionsHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see
// NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

const std::array<int, 8> SecondaryFunctionsHelper::funcsnot = {
    FunctionExprent::FUNCTION_NE, FunctionExprent::FUNCTION_EQ, FunctionExprent::FUNCTION_GE,   FunctionExprent::FUNCTION_LT,
    FunctionExprent::FUNCTION_LE, FunctionExprent::FUNCTION_GT, FunctionExprent::FUNCTION_COR, FunctionExprent::FUNCTION_CADD};

const std::map<int, std::array<std::optional<int>, 3>> SecondaryFunctionsHelper::map_num_comparisons = {
    {FunctionExprent::FUNCTION_EQ, {FunctionExprent::FUNCTION_LT, FunctionExprent::FUNCTION_EQ, FunctionExprent::FUNCTION_GT}},
    {FunctionExprent::FUNCTION_NE, {FunctionExprent::FUNCTION_GE, FunctionExprent::FUNCTION_NE, FunctionExprent::FUNCTION_LE}},
    {FunctionExprent::FUNCTION_GT, {FunctionExprent::FUNCTION_GE, FunctionExprent::FUNCTION_GT, std::nullopt}},
    {FunctionExprent::FUNCTION_GE, {std::nullopt, FunctionExprent::FUNCTION_GE, FunctionExprent::FUNCTION_GT}},
    {FunctionExprent::FUNCTION_LT, {std::nullopt, FunctionExprent::FUNCTION_LT, FunctionExprent::FUNCTION_LE}},
    {FunctionExprent::FUNCTION_LE, {FunctionExprent::FUNCTION_LT, FunctionExprent::FUNCTION_LE, std::nullopt}},
};

bool SecondaryFunctionsHelper::identify_secondary_functions(Statement* stat) {
    if (!stat->get_exprents()) {
        // if(){;}else{...} -> if(!){...}
        if (stat->type == Statement::TYPE_IF) {
            IfStatement* ifelsestat = static_cast<IfStatement*>(stat);
            Statement* ifstat = ifelsestat->get_ifstat();

            if (ifelsestat->iftype == IfStatement::IFTYPE_IFELSE && nn(ifstat)->get_exprents() && ifstat->get_exprents()->empty() &&
                (ifstat->get_all_successor_edges().empty() || !ifstat->get_all_successor_edges()[0]->explicit_)) {
                // move else to the if position
                ifelsestat->get_stats().remove_with_key(ifstat->id);

                ifelsestat->iftype = IfStatement::IFTYPE_IF;
                ifelsestat->set_ifstat(ifelsestat->get_elsestat());
                ifelsestat->set_elsestat(nullptr);

                if (ifelsestat->get_all_successor_edges().empty() && !ifstat->get_all_successor_edges().empty()) {
                    StatEdge* endedge = ifstat->get_all_successor_edges()[0];

                    ifstat->remove_successor(endedge);
                    endedge->set_source(ifelsestat);
                    if (endedge->closure != nullptr) {
                        ifelsestat->get_parent()->add_labeled_edge(endedge);
                    }
                    ifelsestat->add_successor(endedge);
                }

                ifelsestat->get_first()->remove_successor(ifelsestat->get_if_edge());

                ifelsestat->set_if_edge(ifelsestat->get_else_edge());
                ifelsestat->set_else_edge(nullptr);

                // negate head expression
                ifelsestat->set_negated(!ifelsestat->is_negated());
                ifelsestat->get_headexprent_list()[0] = static_cast<IfExprent*>(ifelsestat->get_headexprent()->copy())->negate_if();

                return true;
            }
        }
    }

    bool replaced = true;
    while (replaced) {
        replaced = false;

        std::vector<SeqObject> lst_objects;
        if (!stat->get_exprents()) {
            lst_objects = stat->get_sequential_objects();
        } else {
            for (Exprent* e : *stat->get_exprents()) {
                SeqObject o;
                o.expr = e;
                lst_objects.push_back(o);
            }
        }

        for (int i = 0; i < static_cast<int>(lst_objects.size()); i++) {
            const SeqObject obj = lst_objects[static_cast<std::size_t>(i)];

            if (obj.stat != nullptr) {
                if (identify_secondary_functions(obj.stat)) {
                    replaced = true;
                    break;
                }
            } else if (obj.expr != nullptr) {
                Exprent* retexpr = identify_secondary_functions(obj.expr, true);
                if (retexpr != nullptr) {
                    if (!stat->get_exprents()) {
                        // only head expressions can be replaced!
                        stat->replace_exprent(obj.expr, retexpr);
                    } else {
                        (*stat->get_exprents())[static_cast<std::size_t>(i)] = retexpr;
                    }
                    replaced = true;
                    break;
                }
            }
        }
    }

    return false;
}

Exprent* SecondaryFunctionsHelper::identify_secondary_functions(Exprent* exprent, bool statement_level) {
    if (exprent->type == Exprent::EXPRENT_FUNCTION) {
        FunctionExprent* fexpr = static_cast<FunctionExprent*>(exprent);

        switch (fexpr->get_func_type()) {
            case FunctionExprent::FUNCTION_BOOL_NOT: {
                Exprent* retparam = propagate_bool_not(fexpr);

                if (retparam != nullptr) {
                    return retparam;
                }

                break;
            }
            case FunctionExprent::FUNCTION_EQ:
            case FunctionExprent::FUNCTION_NE:
            case FunctionExprent::FUNCTION_GT:
            case FunctionExprent::FUNCTION_GE:
            case FunctionExprent::FUNCTION_LT:
            case FunctionExprent::FUNCTION_LE: {
                Exprent* expr1 = at(fexpr->get_lst_operands(), 0);
                Exprent* expr2 = at(fexpr->get_lst_operands(), 1);

                if (expr1->type == Exprent::EXPRENT_CONST) {
                    expr2 = expr1;
                    expr1 = at(fexpr->get_lst_operands(), 1);
                }

                if (expr1->type == Exprent::EXPRENT_FUNCTION && expr2->type == Exprent::EXPRENT_CONST) {
                    FunctionExprent* funcexpr = static_cast<FunctionExprent*>(expr1);
                    ConstExprent* cexpr = static_cast<ConstExprent*>(expr2);

                    int functype = funcexpr->get_func_type();
                    if (functype == FunctionExprent::FUNCTION_LCMP || functype == FunctionExprent::FUNCTION_FCMPG ||
                        functype == FunctionExprent::FUNCTION_FCMPL || functype == FunctionExprent::FUNCTION_DCMPG ||
                        functype == FunctionExprent::FUNCTION_DCMPL) {
                        int desttype = -1;

                        auto destcons = map_num_comparisons.find(fexpr->get_func_type());
                        if (destcons != map_num_comparisons.end()) {
                            int index = cexpr->get_int_value() + 1;
                            if (index >= 0 && index <= 2) {
                                const std::optional<int>& destcon = destcons->second[static_cast<std::size_t>(index)];
                                if (destcon) {
                                    desttype = *destcon;
                                }
                            }
                        }

                        if (desttype >= 0) {
                            return make<FunctionExprent>(desttype, funcexpr->get_lst_operands(), funcexpr->bytecode);
                        }
                    }
                }
            }
        }
    }

    bool replaced = true;
    while (replaced) {
        replaced = false;

        for (Exprent* expr : exprent->get_all_exprents()) {
            Exprent* retexpr = identify_secondary_functions(expr, false);
            if (retexpr != nullptr) {
                exprent->replace_exprent(expr, retexpr);
                replaced = true;
                break;
            }
        }
    }

    switch (exprent->type) {
        case Exprent::EXPRENT_FUNCTION: {
            FunctionExprent* fexpr = static_cast<FunctionExprent*>(exprent);
            std::vector<Exprent*>& lst_operands = fexpr->get_lst_operands();

            switch (fexpr->get_func_type()) {
                case FunctionExprent::FUNCTION_XOR:
                    for (int i = 0; i < 2; i++) {
                        Exprent* operand = at(lst_operands, i);
                        VarType operandtype = operand->get_expr_type();

                        if (operand->type == Exprent::EXPRENT_CONST && operandtype.type != cc::TYPE_BOOLEAN) {
                            ConstExprent* cexpr = static_cast<ConstExprent*>(operand);
                            std::int64_t val;
                            if (operandtype.type == cc::TYPE_LONG) {
                                val = std::get<std::int64_t>(cexpr->get_value());
                            } else {
                                val = std::get<std::int32_t>(cexpr->get_value());
                            }

                            if (val == -1) {
                                std::vector<Exprent*> lst_bit_not_operand;
                                lst_bit_not_operand.push_back(at(lst_operands, 1 - i));
                                return make<FunctionExprent>(FunctionExprent::FUNCTION_BIT_NOT, lst_bit_not_operand, fexpr->bytecode);
                            }
                        }
                    }
                    break;
                case FunctionExprent::FUNCTION_EQ:
                case FunctionExprent::FUNCTION_NE:
                    if (at(lst_operands, 0)->get_expr_type().type == cc::TYPE_BOOLEAN && at(lst_operands, 1)->get_expr_type().type == cc::TYPE_BOOLEAN) {
                        for (int i = 0; i < 2; i++) {
                            if (at(lst_operands, i)->type == Exprent::EXPRENT_CONST) {
                                ConstExprent* cexpr = static_cast<ConstExprent*>(at(lst_operands, i));
                                int val = std::get<std::int32_t>(cexpr->get_value());

                                if ((fexpr->get_func_type() == FunctionExprent::FUNCTION_EQ && val == 1) ||
                                    (fexpr->get_func_type() == FunctionExprent::FUNCTION_NE && val == 0)) {
                                    return at(lst_operands, 1 - i);
                                }
                                std::vector<Exprent*> lst_not_operand;
                                lst_not_operand.push_back(at(lst_operands, 1 - i));
                                return make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, lst_not_operand, fexpr->bytecode);
                            }
                        }
                    }
                    break;
                case FunctionExprent::FUNCTION_BOOL_NOT:
                    if (at(lst_operands, 0)->type == Exprent::EXPRENT_CONST) {
                        int val = static_cast<ConstExprent*>(at(lst_operands, 0))->get_int_value();
                        if (val == 0) {
                            return make<ConstExprent>(VarType::BOOLEAN, ConstExprent::Value{std::int32_t{1}}, fexpr->bytecode);
                        }
                        return make<ConstExprent>(VarType::BOOLEAN, ConstExprent::Value{std::int32_t{0}}, fexpr->bytecode);
                    }
                    break;
                case FunctionExprent::FUNCTION_IIF: {
                    Exprent* expr1 = at(lst_operands, 1);
                    Exprent* expr2 = at(lst_operands, 2);

                    if (expr1->type == Exprent::EXPRENT_CONST && expr2->type == Exprent::EXPRENT_CONST) {
                        ConstExprent* cexpr1 = static_cast<ConstExprent*>(expr1);
                        ConstExprent* cexpr2 = static_cast<ConstExprent*>(expr2);

                        if (cexpr1->get_expr_type().type == cc::TYPE_BOOLEAN && cexpr2->get_expr_type().type == cc::TYPE_BOOLEAN) {
                            if (cexpr1->get_int_value() == 0 && cexpr2->get_int_value() != 0) {
                                return make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, at(lst_operands, 0), fexpr->bytecode);
                            }
                            if (cexpr1->get_int_value() != 0 && cexpr2->get_int_value() == 0) {
                                return at(lst_operands, 0);
                            }
                        }
                    }
                    break;
                }
                case FunctionExprent::FUNCTION_LCMP:
                case FunctionExprent::FUNCTION_FCMPL:
                case FunctionExprent::FUNCTION_FCMPG:
                case FunctionExprent::FUNCTION_DCMPL:
                case FunctionExprent::FUNCTION_DCMPG: {
                    int var = DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::VAR_COUNTER);
                    VarType type = at(lst_operands, 0)->get_expr_type();
                    VarProcessor* processor = DecompilerContext::current().current_var_processor;

                    // (the original builds these inside out, left to right: keep the order, the expression ids follow it)
                    VarExprent* iff_var = make<VarExprent>(var, type, processor);
                    ConstExprent* iff_zero = ConstExprent::get_zero_constant(type.type);
                    FunctionExprent* iff_lt = make<FunctionExprent>(FunctionExprent::FUNCTION_LT, std::vector<Exprent*>{iff_var, iff_zero}, Offsets{});
                    ConstExprent* iff_m1 = make<ConstExprent>(VarType::INT, ConstExprent::Value{std::int32_t{-1}}, Offsets{});
                    ConstExprent* iff_p1 = make<ConstExprent>(VarType::INT, ConstExprent::Value{std::int32_t{1}}, Offsets{});
                    FunctionExprent* iff =
                        make<FunctionExprent>(FunctionExprent::FUNCTION_IIF, std::vector<Exprent*>{iff_lt, iff_m1, iff_p1}, Offsets{});

                    VarExprent* head_var = make<VarExprent>(var, type, processor);
                    FunctionExprent* head_sub =
                        make<FunctionExprent>(FunctionExprent::FUNCTION_SUB, std::vector<Exprent*>{at(lst_operands, 0), at(lst_operands, 1)}, Offsets{});
                    AssignmentExprent* head_assign = make<AssignmentExprent>(head_var, head_sub, Offsets{});
                    ConstExprent* head_zero = ConstExprent::get_zero_constant(type.type);
                    FunctionExprent* head =
                        make<FunctionExprent>(FunctionExprent::FUNCTION_EQ, std::vector<Exprent*>{head_assign, head_zero}, Offsets{});

                    nn(processor)->set_var_type(VarVersionPair(var, 0), type);

                    ConstExprent* zero = make<ConstExprent>(VarType::INT, ConstExprent::Value{std::int32_t{0}}, Offsets{});
                    return make<FunctionExprent>(FunctionExprent::FUNCTION_IIF, std::vector<Exprent*>{head, zero, iff}, fexpr->bytecode);
                }
            }
            break;
        }
        case Exprent::EXPRENT_ASSIGNMENT: {  // check for conditional assignment
            AssignmentExprent* asexpr = static_cast<AssignmentExprent*>(exprent);
            Exprent* right = asexpr->get_right();
            Exprent* left = asexpr->get_left();

            if (right->type == Exprent::EXPRENT_FUNCTION) {
                FunctionExprent* func = static_cast<FunctionExprent*>(right);

                std::optional<VarType> midlayer;
                if (func->get_func_type() >= FunctionExprent::FUNCTION_I2L && func->get_func_type() <= FunctionExprent::FUNCTION_I2S) {
                    right = at(func->get_lst_operands(), 0);
                    midlayer = func->get_simple_cast_type();
                    if (right->type == Exprent::EXPRENT_FUNCTION) {
                        func = static_cast<FunctionExprent*>(right);
                    } else {
                        return nullptr;
                    }
                }

                std::vector<Exprent*>& lst_func_operands = func->get_lst_operands();

                Exprent* cond = nullptr;

                switch (func->get_func_type()) {
                    case FunctionExprent::FUNCTION_ADD:
                    case FunctionExprent::FUNCTION_AND:
                    case FunctionExprent::FUNCTION_OR:
                    case FunctionExprent::FUNCTION_XOR:
                        if (left->equals(at(lst_func_operands, 1))) {
                            cond = at(lst_func_operands, 0);
                            break;
                        }
                        [[fallthrough]];
                    case FunctionExprent::FUNCTION_SUB:
                    case FunctionExprent::FUNCTION_MUL:
                    case FunctionExprent::FUNCTION_DIV:
                    case FunctionExprent::FUNCTION_REM:
                    case FunctionExprent::FUNCTION_SHL:
                    case FunctionExprent::FUNCTION_SHR:
                    case FunctionExprent::FUNCTION_USHR:
                        if (left->equals(at(lst_func_operands, 0))) {
                            cond = at(lst_func_operands, 1);
                        }
                }

                if (cond != nullptr && (!midlayer || *midlayer == cond->get_expr_type())) {
                    asexpr->set_right(cond);
                    asexpr->set_cond_type(func->get_func_type());
                }
            }
            break;
        }
        case Exprent::EXPRENT_INVOCATION:
            if (!statement_level) {  // simplify if exprent is a real expression. The opposite case is pretty absurd, can still happen however (and happened at least once).
                Exprent* retexpr = ConcatenationHelper::contract_string_concat(exprent);
                if (!exprent->equals(retexpr)) {
                    return retexpr;
                }
            }
    }

    return nullptr;
}

Exprent* SecondaryFunctionsHelper::propagate_bool_not(Exprent* exprent) {
    if (exprent->type == Exprent::EXPRENT_FUNCTION) {
        FunctionExprent* fexpr = static_cast<FunctionExprent*>(exprent);

        if (fexpr->get_func_type() == FunctionExprent::FUNCTION_BOOL_NOT) {
            Exprent* param = at(fexpr->get_lst_operands(), 0);

            if (param->type == Exprent::EXPRENT_FUNCTION) {
                FunctionExprent* fparam = static_cast<FunctionExprent*>(param);

                int ftype = fparam->get_func_type();
                switch (ftype) {
                    case FunctionExprent::FUNCTION_BOOL_NOT: {
                        Exprent* newexpr = at(fparam->get_lst_operands(), 0);
                        Exprent* retexpr = propagate_bool_not(newexpr);
                        return retexpr == nullptr ? newexpr : retexpr;
                    }
                    case FunctionExprent::FUNCTION_CADD:
                    case FunctionExprent::FUNCTION_COR: {
                        std::vector<Exprent*>& operands = fparam->get_lst_operands();
                        for (int i = 0; i < static_cast<int>(operands.size()); i++) {
                            Exprent* newparam =
                                make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, operands[static_cast<std::size_t>(i)], operands[static_cast<std::size_t>(i)]->bytecode);

                            Exprent* retparam = propagate_bool_not(newparam);
                            operands[static_cast<std::size_t>(i)] = retparam == nullptr ? newparam : retparam;
                        }
                    }
                        [[fallthrough]];
                    case FunctionExprent::FUNCTION_EQ:
                    case FunctionExprent::FUNCTION_NE:
                    case FunctionExprent::FUNCTION_LT:
                    case FunctionExprent::FUNCTION_GE:
                    case FunctionExprent::FUNCTION_GT:
                    case FunctionExprent::FUNCTION_LE:
                        fparam->set_func_type(funcsnot[static_cast<std::size_t>(ftype - FunctionExprent::FUNCTION_EQ)]);
                        return fparam;
                }
            }
        }
    }

    return nullptr;
}

}  // namespace cb::java::decompiler
