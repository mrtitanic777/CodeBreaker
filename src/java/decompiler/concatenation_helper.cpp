// ConcatenationHelper: ported from Fernflower (de.fernflower.modules.decompiler), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE
// in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "helpers_internal.hpp"

namespace cb::java::decompiler {

using namespace helpers_internal;

const VarType ConcatenationHelper::builder_type(cc::TYPE_OBJECT, 0, "java/lang/StringBuilder");
const VarType ConcatenationHelper::buffer_type(cc::TYPE_OBJECT, 0, "java/lang/StringBuffer");

Exprent* ConcatenationHelper::contract_string_concat(Exprent* expr) {
    Exprent* expr_tmp = nullptr;
    std::optional<VarType> cltype;

    // first quick test
    if (expr->type == Exprent::EXPRENT_INVOCATION) {
        InvocationExprent* iex = static_cast<InvocationExprent*>(expr);
        if (iex->get_name() == "toString") {
            if (iex->get_classname() == builder_class) {
                cltype = builder_type;
            } else if (iex->get_classname() == buffer_class) {
                cltype = buffer_type;
            }
            if (cltype) {
                expr_tmp = iex->get_instance();
            }
        }
    }

    if (expr_tmp == nullptr) {
        return expr;
    }

    // iterate in depth, collecting possible operands
    std::vector<Exprent*> lst_operands;

    while (true) {
        int found = 0;

        switch (expr_tmp->type) {
            case Exprent::EXPRENT_INVOCATION: {
                InvocationExprent* iex = static_cast<InvocationExprent*>(expr_tmp);
                if (is_append_concat(iex, *cltype)) {
                    lst_operands.insert(lst_operands.begin(), at(iex->get_lst_parameters(), 0));
                    expr_tmp = iex->get_instance();
                    found = 1;
                }
                break;
            }
            case Exprent::EXPRENT_NEW: {
                NewExprent* nex = static_cast<NewExprent*>(expr_tmp);
                if (is_new_concat(nex, *cltype)) {
                    const std::vector<VarType>& params = nn(nex->get_constructor())->get_descriptor().params;
                    if (params.size() == 1) {
                        lst_operands.insert(lst_operands.begin(), at(nex->get_constructor()->get_lst_parameters(), 0));
                    }
                    found = 2;
                }
            }
        }

        if (found == 0) {
            return expr;
        }
        if (found == 2) {
            break;
        }
        nn(expr_tmp);
    }

    int first2str = 0;
    int index = 0;
    while (index < static_cast<int>(lst_operands.size()) && index < 2) {
        if (lst_operands[static_cast<std::size_t>(index)]->get_expr_type() == VarType::STRING) {
            first2str |= (index + 1);
        }
        index++;
    }

    if (first2str == 0) {
        lst_operands.insert(lst_operands.begin(), make<ConstExprent>(VarType::STRING, ConstExprent::Value{std::string()}, expr->bytecode));
    }

    // remove redundant String.valueOf
    for (int i = 0; i < static_cast<int>(lst_operands.size()); i++) {
        Exprent* rep = remove_string_value_of(lst_operands[static_cast<std::size_t>(i)]);

        bool ok = (i > 1);
        if (!ok) {
            bool isstr = rep->get_expr_type() == VarType::STRING;
            ok = isstr || first2str != i + 1;

            if (i == 0) {
                first2str &= 2;
            }
        }

        if (ok) {
            lst_operands[static_cast<std::size_t>(i)] = rep;
        }
    }

    // build exprent to return
    Exprent* func = lst_operands[0];

    for (int i = 1; i < static_cast<int>(lst_operands.size()); i++) {
        std::vector<Exprent*> lst_tmp;
        lst_tmp.push_back(func);
        lst_tmp.push_back(lst_operands[static_cast<std::size_t>(i)]);
        func = make<FunctionExprent>(FunctionExprent::FUNCTION_STR_CONCAT, lst_tmp, expr->bytecode);
    }

    return func;
}

bool ConcatenationHelper::is_append_concat(InvocationExprent* expr, const VarType& cltype) {
    if (expr->get_name() == "append") {
        const MethodDescriptor& md = expr->get_descriptor();
        if (md.ret == cltype && md.params.size() == 1) {
            const VarType& param = md.params[0];
            switch (param.type) {
                case cc::TYPE_OBJECT:
                    if (!(param == VarType::STRING) && !(param == VarType::OBJECT)) {
                        break;
                    }
                    [[fallthrough]];
                case cc::TYPE_BOOLEAN:
                case cc::TYPE_CHAR:
                case cc::TYPE_DOUBLE:
                case cc::TYPE_FLOAT:
                case cc::TYPE_INT:
                case cc::TYPE_LONG:
                    return true;
                default:;
            }
        }
    }

    return false;
}

bool ConcatenationHelper::is_new_concat(NewExprent* expr, const VarType& cltype) {
    if (expr->get_new_type() == cltype) {
        const std::vector<VarType>& params = nn(expr->get_constructor())->get_descriptor().params;
        if (params.empty() || (params.size() == 1 && params[0] == VarType::STRING)) {
            return true;
        }
    }

    return false;
}

Exprent* ConcatenationHelper::remove_string_value_of(Exprent* exprent) {
    if (exprent->type == Exprent::EXPRENT_INVOCATION) {
        InvocationExprent* iex = static_cast<InvocationExprent*>(exprent);
        if (iex->get_name() == "valueOf" && iex->get_classname() == string_class) {
            const MethodDescriptor& md = iex->get_descriptor();
            if (md.params.size() == 1) {
                const VarType& param = md.params[0];
                switch (param.type) {
                    case cc::TYPE_OBJECT:
                        if (!(param == VarType::OBJECT)) {
                            break;
                        }
                        [[fallthrough]];
                    case cc::TYPE_BOOLEAN:
                    case cc::TYPE_CHAR:
                    case cc::TYPE_DOUBLE:
                    case cc::TYPE_FLOAT:
                    case cc::TYPE_INT:
                    case cc::TYPE_LONG:
                        return at(iex->get_lst_parameters(), 0);
                }
            }
        }
    }

    return exprent;
}

}  // namespace cb::java::decompiler
