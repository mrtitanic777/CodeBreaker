// java/decompiler/function_exprent.cpp -- see exprents.hpp: FunctionExprent. Ported from Fernflower (de.fernflower.modules.decompiler.exps.FunctionExprent),
// Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <set>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"

namespace cb::java::decompiler {

namespace {

// FUNCTION_I2L ... FUNCTION_I2S: the type each converts to.
const VarType* function_types(int index) {
    static const VarType* const types[] = {&VarType::LONG, &VarType::FLOAT, &VarType::DOUBLE, &VarType::INT,   &VarType::FLOAT,
                                           &VarType::DOUBLE, &VarType::INT,  &VarType::LONG,   &VarType::DOUBLE, &VarType::INT,
                                           &VarType::LONG,   &VarType::FLOAT, &VarType::BYTE,  &VarType::CHAR,   &VarType::SHORT};
    return types[index];
}

const char* const OPERATORS[] = {" + ",  " - ",  " * ",  " / ",  " & ",  " | ",  " ^ ",  " % ",  " << ", " >> ",
                                 " >>> ", " == ", " != ", " < ",  " >= ", " > ",  " <= ", " && ", " || ", " + "};

const int PRECEDENCE[] = {
    3,   // FUNCTION_ADD
    3,   // FUNCTION_SUB
    2,   // FUNCTION_MUL
    2,   // FUNCTION_DIV
    7,   // FUNCTION_AND
    9,   // FUNCTION_OR
    8,   // FUNCTION_XOR
    2,   // FUNCTION_REM
    4,   // FUNCTION_SHL
    4,   // FUNCTION_SHR
    4,   // FUNCTION_USHR
    1,   // FUNCTION_BIT_NOT
    1,   // FUNCTION_BOOL_NOT
    1,   // FUNCTION_NEG
    1,   // FUNCTION_I2L
    1,   // FUNCTION_I2F
    1,   // FUNCTION_I2D
    1,   // FUNCTION_L2I
    1,   // FUNCTION_L2F
    1,   // FUNCTION_L2D
    1,   // FUNCTION_F2I
    1,   // FUNCTION_F2L
    1,   // FUNCTION_F2D
    1,   // FUNCTION_D2I
    1,   // FUNCTION_D2L
    1,   // FUNCTION_D2F
    1,   // FUNCTION_I2B
    1,   // FUNCTION_I2C
    1,   // FUNCTION_I2S
    1,   // FUNCTION_CAST
    6,   // FUNCTION_INSTANCEOF
    0,   // FUNCTION_ARRAY_LENGTH
    1,   // FUNCTION_IMM
    1,   // FUNCTION_MMI
    1,   // FUNCTION_IPP
    1,   // FUNCTION_PPI
    12,  // FUNCTION_IIF
    -1,  // FUNCTION_LCMP
    -1,  // FUNCTION_FCMPL
    -1,  // FUNCTION_FCMPG
    -1,  // FUNCTION_DCMPL
    -1,  // FUNCTION_DCMPG
    6,   // FUNCTION_EQ
    6,   // FUNCTION_NE
    5,   // FUNCTION_LT
    5,   // FUNCTION_GE
    5,   // FUNCTION_GT
    5,   // FUNCTION_LE
    10,  // FUNCTION_CADD
    11,  // FUNCTION_COR
    3    // FUNCTION_STR_CONCAT
};

bool is_associative(int func) {
    static const std::set<int> associativity = {FunctionExprent::FUNCTION_ADD, FunctionExprent::FUNCTION_MUL,  FunctionExprent::FUNCTION_AND,
                                                FunctionExprent::FUNCTION_OR,  FunctionExprent::FUNCTION_XOR,  FunctionExprent::FUNCTION_CADD,
                                                FunctionExprent::FUNCTION_COR, FunctionExprent::FUNCTION_STR_CONCAT};
    return associativity.contains(func);
}

VarType get_max_var_type(const std::vector<const VarType*>& arr) {
    static const int types[] = {cc::TYPE_DOUBLE, cc::TYPE_FLOAT, cc::TYPE_LONG};
    static const VarType* const vartypes[] = {&VarType::DOUBLE, &VarType::FLOAT, &VarType::LONG};
    for (int i = 0; i < 3; i++) {
        for (const VarType* t : arr) {
            if (t->type == types[i]) return *vartypes[i];
        }
    }
    return VarType::INT;
}

}  // namespace

FunctionExprent::FunctionExprent(int func_type, ListStack<Exprent*>& stack, const Offsets& offsets)
    : FunctionExprent(func_type, std::vector<Exprent*>{}, offsets) {
    if (func_type >= FUNCTION_BIT_NOT && func_type <= FUNCTION_PPI && func_type != FUNCTION_CAST && func_type != FUNCTION_INSTANCEOF) {
        lst_operands_.push_back(stack.pop());
    } else if (func_type == FUNCTION_IIF) {
        throw std::runtime_error("no direct instantiation possible");
    } else {
        Exprent* expr = stack.pop();
        lst_operands_.push_back(stack.pop());
        lst_operands_.push_back(expr);
    }
}

FunctionExprent::FunctionExprent(int func_type, std::vector<Exprent*> operands, const Offsets& offsets)
    : Exprent(EXPRENT_FUNCTION), func_type_(func_type), lst_operands_(std::move(operands)) {
    add_bytecode_offsets(offsets);
}

FunctionExprent::FunctionExprent(int func_type, Exprent* operand, const Offsets& offsets) : FunctionExprent(func_type, std::vector<Exprent*>{}, offsets) {
    lst_operands_.push_back(operand);
}

VarType FunctionExprent::get_expr_type() const {
    std::optional<VarType> expr_type;
    if (func_type_ <= FUNCTION_NEG || func_type_ == FUNCTION_IPP || func_type_ == FUNCTION_PPI || func_type_ == FUNCTION_IMM || func_type_ == FUNCTION_MMI) {
        const VarType type1 = lst_operands_.at(0)->get_expr_type();
        std::optional<VarType> type2;
        if (lst_operands_.size() > 1) type2 = lst_operands_[1]->get_expr_type();

        switch (func_type_) {
            case FUNCTION_IMM:
            case FUNCTION_MMI:
            case FUNCTION_IPP:
            case FUNCTION_PPI:
                expr_type = implicit_type_;
                break;
            case FUNCTION_BOOL_NOT:
                expr_type = VarType::BOOLEAN;
                break;
            case FUNCTION_SHL:
            case FUNCTION_SHR:
            case FUNCTION_USHR:
            case FUNCTION_BIT_NOT:
            case FUNCTION_NEG:
                expr_type = get_max_var_type({&type1});
                break;
            case FUNCTION_ADD:
            case FUNCTION_SUB:
            case FUNCTION_MUL:
            case FUNCTION_DIV:
            case FUNCTION_REM:
                expr_type = get_max_var_type({&type1, &type2.value()});
                break;
            case FUNCTION_AND:
            case FUNCTION_OR:
            case FUNCTION_XOR:
                if (type1.type == cc::TYPE_BOOLEAN && type2.value().type == cc::TYPE_BOOLEAN) {
                    expr_type = VarType::BOOLEAN;
                } else {
                    expr_type = get_max_var_type({&type1, &type2.value()});
                }
                break;
            default:
                break;
        }
    } else if (func_type_ == FUNCTION_CAST) {
        expr_type = lst_operands_.at(1)->get_expr_type();
    } else if (func_type_ == FUNCTION_IIF) {
        Exprent* param1 = lst_operands_.at(1);
        Exprent* param2 = lst_operands_.at(2);

        const VarType supertype = VarType::common_supertype(param1->get_expr_type(), param2->get_expr_type()).value();

        if (param1->type == Exprent::EXPRENT_CONST && param2->type == Exprent::EXPRENT_CONST && supertype.type != cc::TYPE_BOOLEAN &&
            VarType::INT.is_superset(supertype)) {
            expr_type = VarType::INT;
        } else {
            expr_type = supertype;
        }
    } else if (func_type_ == FUNCTION_STR_CONCAT) {
        expr_type = VarType::STRING;
    } else if (func_type_ >= FUNCTION_EQ || func_type_ == FUNCTION_INSTANCEOF) {
        expr_type = VarType::BOOLEAN;
    } else if (func_type_ >= FUNCTION_ARRAY_LENGTH) {
        expr_type = VarType::INT;
    } else {
        expr_type = *function_types(func_type_ - FUNCTION_I2L);
    }
    // (A null type, which the original returns for an increment without an implicit type, throws here.)
    return expr_type.value();
}

int FunctionExprent::get_exprent_use() const {
    if (func_type_ >= FUNCTION_IMM && func_type_ <= FUNCTION_PPI) return 0;
    int ret = Exprent::MULTIPLE_USES | Exprent::SIDE_EFFECTS_FREE;
    for (const Exprent* expr : lst_operands_) ret &= expr->get_exprent_use();
    return ret;
}

CheckTypesResult FunctionExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    Exprent* param1 = lst_operands_.at(0);
    const VarType type1 = param1->get_expr_type();
    Exprent* param2 = nullptr;
    std::optional<VarType> type2;
    if (lst_operands_.size() > 1) {
        param2 = lst_operands_[1];
        type2 = param2->get_expr_type();
    }

    switch (func_type_) {
        case FUNCTION_IIF: {
            const VarType supertype = get_expr_type();
            result.add_min_type_exprent(param1, VarType::BOOLEAN);
            result.add_min_type_exprent(param2, VarType::min_type_in_family(supertype.type_family));
            result.add_min_type_exprent(lst_operands_.at(2), VarType::min_type_in_family(supertype.type_family));
            break;
        }
        case FUNCTION_I2L:
        case FUNCTION_I2F:
        case FUNCTION_I2D:
        case FUNCTION_I2B:
        case FUNCTION_I2C:
        case FUNCTION_I2S:
            result.add_min_type_exprent(param1, VarType::BYTECHAR);
            result.add_max_type_exprent(param1, VarType::INT);
            break;
        case FUNCTION_IMM:
        case FUNCTION_IPP:
        case FUNCTION_MMI:
        case FUNCTION_PPI:
            result.add_min_type_exprent(param1, implicit_type_.value());
            result.add_max_type_exprent(param1, implicit_type_.value());
            break;
        case FUNCTION_ADD:
        case FUNCTION_SUB:
        case FUNCTION_MUL:
        case FUNCTION_DIV:
        case FUNCTION_REM:
        case FUNCTION_SHL:
        case FUNCTION_SHR:
        case FUNCTION_USHR:
        case FUNCTION_LT:
        case FUNCTION_GE:
        case FUNCTION_GT:
        case FUNCTION_LE:
            result.add_min_type_exprent(param2, VarType::BYTECHAR);
            [[fallthrough]];
        case FUNCTION_BIT_NOT:
        // case FUNCTION_BOOL_NOT:
        case FUNCTION_NEG:
            result.add_min_type_exprent(param1, VarType::BYTECHAR);
            break;
        case FUNCTION_AND:
        case FUNCTION_OR:
        case FUNCTION_XOR:
        case FUNCTION_EQ:
        case FUNCTION_NE: {
            if (type1.type == cc::TYPE_BOOLEAN) {
                if (type2.value().is_strict_superset(type1)) {
                    result.add_min_type_exprent(param1, VarType::BYTECHAR);
                } else {  // both are booleans
                    const bool param1_false_boolean =
                        type1.is_false_boolean() || (param1->type == Exprent::EXPRENT_CONST && !static_cast<ConstExprent*>(param1)->has_boolean_value());
                    const bool param2_false_boolean =
                        type1.is_false_boolean() || (param2->type == Exprent::EXPRENT_CONST && !static_cast<ConstExprent*>(param2)->has_boolean_value());
                    if (param1_false_boolean || param2_false_boolean) {
                        result.add_min_type_exprent(param1, VarType::BYTECHAR);
                        result.add_min_type_exprent(param2, VarType::BYTECHAR);
                    }
                }
            } else if (type2.value().type == cc::TYPE_BOOLEAN) {
                if (type1.is_strict_superset(type2.value())) result.add_min_type_exprent(param2, VarType::BYTECHAR);
            }
            break;
        }
        default:
            break;
    }
    return result;
}

Exprent* FunctionExprent::copy() const {
    std::vector<Exprent*> lst;
    for (const Exprent* expr : lst_operands_) lst.push_back(expr->copy());
    FunctionExprent* func = make<FunctionExprent>(func_type_, lst, bytecode);
    func->set_implicit_type(implicit_type_);
    return func;
}

bool FunctionExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const FunctionExprent* fe = dynamic_cast<const FunctionExprent*>(o);
    if (fe == nullptr) return false;
    return func_type_ == fe->get_func_type() && equal_exprent_lists(lst_operands_, fe->get_lst_operands());   // TODO: order of operands insignificant
}

void FunctionExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    for (std::size_t i = 0; i < lst_operands_.size(); i++) {
        if (old_expr == lst_operands_[i]) lst_operands_[i] = new_expr;
    }
}

TextBuffer FunctionExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    tracer.add_mapping(bytecode);

    if (func_type_ <= FUNCTION_USHR) {
        return wrap_operand_string(lst_operands_.at(0), false, indent, tracer)
            .append(OPERATORS[func_type_])
            .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer));
    }

    if (func_type_ >= FUNCTION_EQ) {
        return wrap_operand_string(lst_operands_.at(0), false, indent, tracer)
            .append(OPERATORS[func_type_ - FUNCTION_EQ + 11])
            .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer));
    }

    switch (func_type_) {
        case FUNCTION_BIT_NOT:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("~");
        case FUNCTION_BOOL_NOT:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("!");
        case FUNCTION_NEG:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("-");
        case FUNCTION_CAST:
            return lst_operands_.at(1)->to_java(indent, tracer).enclose("(", ")").append(wrap_operand_string(lst_operands_[0], true, indent, tracer));
        case FUNCTION_ARRAY_LENGTH: {
            Exprent* arr = lst_operands_.at(0);
            TextBuffer res = wrap_operand_string(arr, false, indent, tracer);
            if (arr->get_expr_type().array_dim == 0) {
                const VarType obj_arr = VarType::OBJECT.resize_array_dim(1);   // type family does not change
                res.enclose("((" + ExprProcessor::get_cast_type_name(obj_arr) + ")", ")");
            }
            return res.append(".length");
        }
        case FUNCTION_IIF:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .append(" ? ")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(" : ")
                .append(wrap_operand_string(lst_operands_.at(2), true, indent, tracer));
        case FUNCTION_IPP:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).append("++");
        case FUNCTION_PPI:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("++");
        case FUNCTION_IMM:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).append("--");
        case FUNCTION_MMI:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("--");
        case FUNCTION_INSTANCEOF:
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .append(" instanceof ")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer));
        case FUNCTION_LCMP:   // shouldn't appear in the final code
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .prepend("__lcmp__(")
                .append(",")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(")");
        case FUNCTION_FCMPL:   // shouldn't appear in the final code
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .prepend("__fcmpl__(")
                .append(",")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(")");
        case FUNCTION_FCMPG:   // shouldn't appear in the final code
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .prepend("__fcmpg__(")
                .append(",")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(")");
        case FUNCTION_DCMPL:   // shouldn't appear in the final code
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .prepend("__dcmpl__(")
                .append(",")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(")");
        case FUNCTION_DCMPG:   // shouldn't appear in the final code
            return wrap_operand_string(lst_operands_.at(0), true, indent, tracer)
                .prepend("__dcmpg__(")
                .append(",")
                .append(wrap_operand_string(lst_operands_.at(1), true, indent, tracer))
                .append(")");
        default:
            break;
    }

    if (func_type_ <= FUNCTION_I2S) {
        return wrap_operand_string(lst_operands_.at(0), true, indent, tracer).prepend("(" + ExprProcessor::get_type_name(*function_types(func_type_ - FUNCTION_I2L)) + ")");
    }

    throw std::runtime_error("invalid function");
}

int FunctionExprent::get_precedence(int func) { return PRECEDENCE[func]; }

VarType FunctionExprent::get_simple_cast_type() const { return *function_types(func_type_ - FUNCTION_I2L); }

TextBuffer FunctionExprent::wrap_operand_string(Exprent* expr, bool eq, int indent, BytecodeMappingTracer& tracer) {
    const int myprec = get_precedence();
    const int exprprec = expr->get_precedence();

    bool parentheses = exprprec > myprec;
    if (!parentheses && eq) {
        parentheses = (exprprec == myprec);
        if (parentheses) {
            if (expr->type == Exprent::EXPRENT_FUNCTION && static_cast<FunctionExprent*>(expr)->get_func_type() == func_type_) {
                parentheses = !is_associative(func_type_);
            }
        }
    }

    TextBuffer res = expr->to_java(indent, tracer);

    if (parentheses) res.enclose("(", ")");

    return res;
}

bool FunctionExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;
    const MatchValue func = match_node->get_rule_value(MatchProperties::EXPRENT_FUNCTYPE);
    if (!std::holds_alternative<std::monostate>(func)) {
        if (this->func_type_ != std::get<std::int32_t>(func)) return false;
    }
    return true;
}

}  // namespace cb::java::decompiler
