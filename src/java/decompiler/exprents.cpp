// java/decompiler/exprent.cpp -- see exprents.hpp: Exprent, VarVersionPair, and the expressions of array, assignment, assert, annotation, exit, if, monitor and
// switch. Ported from Fernflower (de.fernflower.modules.decompiler.exps), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <regex>
#include <stdexcept>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

// ---- VarVersionPair

VarVersionPair::VarVersionPair(const VarExprent* v) : var(v->get_index()), version(v->get_version()) {}

// ---- Exprent

Exprent::Exprent(int t) : type(t), id(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::EXPRENT_COUNTER)) {}

bool Exprent::contains_exprent(const Exprent* e) {
    std::vector<Exprent*> list_temp = get_all_exprents(true);
    list_temp.push_back(this);
    for (Exprent* lst_expr : list_temp) {
        if (lst_expr->equals(e)) return true;
    }
    return false;
}

std::vector<Exprent*> Exprent::get_all_exprents(bool recursive) {
    std::vector<Exprent*> lst = get_all_exprents();
    if (recursive) {
        for (int i = static_cast<int>(lst.size()) - 1; i >= 0; i--) {
            const std::vector<Exprent*> sub = lst[static_cast<std::size_t>(i)]->get_all_exprents(true);
            lst.insert(lst.end(), sub.begin(), sub.end());
        }
    }
    return lst;
}

std::set<VarVersionPair> Exprent::get_all_variables() {
    std::vector<Exprent*> lst_all_exprents = get_all_exprents(true);
    lst_all_exprents.push_back(this);
    std::set<VarVersionPair> result;
    for (Exprent* expr : lst_all_exprents) {
        if (expr->type == EXPRENT_VAR) result.insert(VarVersionPair(static_cast<const VarExprent*>(expr)));
    }
    return result;
}

std::vector<Exprent*> Exprent::get_all_exprents() { throw std::runtime_error("not implemented"); }

Exprent* Exprent::copy() const { throw std::runtime_error("not implemented"); }

TextBuffer Exprent::to_java(int, BytecodeMappingTracer&) { throw std::runtime_error("not implemented"); }

void Exprent::add_bytecode_offsets(const Offsets& offsets) {
    if (offsets && !offsets->empty()) {
        if (!bytecode) bytecode = *offsets;
        else bytecode->insert(offsets->begin(), offsets->end());
    }
}

Exprent* Exprent::find_object(const MatchNode* match_node, int index) {
    if (match_node->get_type() != MatchNode::MATCHNODE_EXPRENT) return nullptr;
    const std::vector<Exprent*> lst_all_exprents = get_all_exprents();
    if (lst_all_exprents.empty()) return nullptr;
    const MatchValue position = match_node->get_rule_value(MatchProperties::EXPRENT_POSITION);
    if (!std::holds_alternative<std::monostate>(position)) {
        const std::string& pos = std::get<std::string>(position);
        static const std::regex numeric("-?\\d+");
        if (std::regex_match(pos, numeric)) {
            const int size = static_cast<int>(lst_all_exprents.size());
            const int at = (size + std::stoi(pos)) % size;   // care for negative positions
            return lst_all_exprents.at(static_cast<std::size_t>(at < 0 ? size : at));
        }
    } else if (index < static_cast<int>(lst_all_exprents.size())) {   // use 'index' parameter
        return lst_all_exprents[static_cast<std::size_t>(index)];
    }
    return nullptr;
}

bool Exprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (match_node->get_type() != MatchNode::MATCHNODE_EXPRENT) return false;
    for (const auto& [key, rule] : match_node->get_rules()) {
        switch (key) {
            case MatchProperties::EXPRENT_TYPE:
                if (this->type != std::get<std::int32_t>(rule.value)) return false;
                break;
            case MatchProperties::EXPRENT_RET:
                if (!engine.check_and_set_variable_value(std::get<std::string>(rule.value), MatchValue(static_cast<IMatchable*>(this)))) return false;
                break;
            default:
                break;
        }
    }
    return true;
}

bool Exprent::equals_matchable(const IMatchable* other) const {
    const Exprent* e = dynamic_cast<const Exprent*>(other);
    return e != nullptr && equals(e);
}

bool equal_exprents(const Exprent* a, const Exprent* b) { return a == nullptr ? b == nullptr : a->equals(b); }

bool equal_exprent_lists(const std::vector<Exprent*>& a, const std::vector<Exprent*>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); i++) {
        if (!equal_exprents(a[i], b[i])) return false;
    }
    return true;
}

// ---- ArrayExprent

ArrayExprent::ArrayExprent(Exprent* array, Exprent* index, VarType hard_type, const Offsets& offsets)
    : Exprent(EXPRENT_ARRAY), array_(array), index_(index), hard_type_(std::move(hard_type)) {
    add_bytecode_offsets(offsets);
}

Exprent* ArrayExprent::copy() const {
    Exprent* array = array_->copy();   // (Java evaluates the arguments left to right; C++ does not say: the order the ids are taken in is Java's)
    Exprent* index = index_->copy();
    return make<ArrayExprent>(array, index, hard_type_, bytecode);
}

VarType ArrayExprent::get_expr_type() const {
    const VarType expr_type = array_->get_expr_type();
    if (expr_type == VarType::NULL_TYPE) return hard_type_.copy();
    return expr_type.decrease_array_dim();
}

int ArrayExprent::get_exprent_use() const { return array_->get_exprent_use() & index_->get_exprent_use() & Exprent::MULTIPLE_USES; }

CheckTypesResult ArrayExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    result.add_min_type_exprent(index_, VarType::BYTECHAR);
    result.add_max_type_exprent(index_, VarType::INT);
    return result;
}

TextBuffer ArrayExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer res = array_->to_java(indent, tracer);
    if (array_->get_precedence() > get_precedence()) {   // array precedence equals 0
        res.enclose("(", ")");
    }
    const VarType arr_type = array_->get_expr_type();
    if (arr_type.array_dim == 0) {
        const VarType obj_arr = VarType::OBJECT.resize_array_dim(1);   // type family does not change
        res.enclose("((" + ExprProcessor::get_cast_type_name(obj_arr) + ")", ")");
    }
    tracer.add_mapping(bytecode);
    return res.append("[").append(index_->to_java(indent, tracer)).append("]");
}

void ArrayExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == array_) array_ = new_expr;
    if (old_expr == index_) index_ = new_expr;
}

bool ArrayExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const ArrayExprent* arr = dynamic_cast<const ArrayExprent*>(o);
    if (arr == nullptr) return false;
    return equal_exprents(array_, arr->get_array()) && equal_exprents(index_, arr->get_index());
}

// ---- AssertExprent

TextBuffer AssertExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buffer;
    buffer.append("assert ");
    tracer.add_mapping(bytecode);
    if (parameters_.at(0) == nullptr) buffer.append("false");
    else buffer.append(parameters_[0]->to_java(indent, tracer));
    if (parameters_.size() > 1) {
        buffer.append(" : ");
        buffer.append(parameters_[1]->to_java(indent, tracer));
    }
    return buffer;
}

// ---- AnnotationExprent

TextBuffer AnnotationExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buffer;
    buffer.append_indent(indent);
    buffer.append("@");
    buffer.append(DecompilerContext::current().import_collector->get_short_name(ExprProcessor::build_java_class_name(class_name_)));
    if (!par_names_.empty()) {
        buffer.append("(");
        if (par_names_.size() == 1 && par_names_[0] == "value") {
            buffer.append(par_values_.at(0)->to_java(indent + 1, tracer));
        } else {
            for (std::size_t i = 0; i < par_names_.size(); i++) {
                buffer.append_line_separator().append_indent(indent + 1);
                buffer.append(par_names_[i]);
                buffer.append(" = ");
                buffer.append(par_values_.at(i)->to_java(indent + 2, tracer));
                if (i < par_names_.size() - 1) buffer.append(",");
            }
            buffer.append_line_separator().append_indent(indent);
        }
        buffer.append(")");
    }
    return buffer;
}

int AnnotationExprent::get_annotation_type() const {
    if (par_names_.empty()) return ANNOTATION_MARKER;
    if (par_names_.size() == 1 && par_names_[0] == "value") return ANNOTATION_SINGLE_ELEMENT;
    return ANNOTATION_NORMAL;
}

bool AnnotationExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const AnnotationExprent* ann = dynamic_cast<const AnnotationExprent*>(o);
    if (ann == nullptr) return false;
    return class_name_ == ann->class_name_ && par_names_ == ann->par_names_ && equal_exprent_lists(par_values_, ann->par_values_);
}

// ---- AssignmentExprent

namespace {
const char* const ASSIGNMENT_OPERATORS[] = {
    " += ",    // FUNCTION_ADD
    " -= ",    // FUNCTION_SUB
    " *= ",    // FUNCTION_MUL
    " /= ",    // FUNCTION_DIV
    " &= ",    // FUNCTION_AND
    " |= ",    // FUNCTION_OR
    " ^= ",    // FUNCTION_XOR
    " %= ",    // FUNCTION_REM
    " <<= ",   // FUNCTION_SHL
    " >>= ",   // FUNCTION_SHR
    " >>>= "   // FUNCTION_USHR
};
}  // namespace

AssignmentExprent::AssignmentExprent(Exprent* left, Exprent* right, const Offsets& offsets) : Exprent(EXPRENT_ASSIGNMENT), left_(left), right_(right) {
    add_bytecode_offsets(offsets);
}

VarType AssignmentExprent::get_expr_type() const { return left_->get_expr_type(); }

CheckTypesResult AssignmentExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    const VarType type_left = left_->get_expr_type();
    const VarType type_right = right_->get_expr_type();
    if (type_left.type_family > type_right.type_family) {
        result.add_min_type_exprent(right_, VarType::min_type_in_family(type_left.type_family));
    } else if (type_left.type_family < type_right.type_family) {
        result.add_min_type_exprent(left_, type_right);
    } else {
        result.add_min_type_exprent(left_, VarType::common_supertype(type_left, type_right).value());
    }
    return result;
}

Exprent* AssignmentExprent::copy() const {
    Exprent* left = left_->copy();   // (Java's left-to-right order of the arguments: the ids are taken in it)
    Exprent* right = right_->copy();
    return make<AssignmentExprent>(left, right, bytecode);
}

TextBuffer AssignmentExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    const VarType left_type = left_->get_expr_type();
    const VarType right_type = right_->get_expr_type();

    bool field_in_class_init = false, hidden_field = false;
    if (left_->type == Exprent::EXPRENT_FIELD) {   // first assignment to a final field. Field name without "this" in front of it
        const FieldExprent* field = static_cast<const FieldExprent*>(left_);
        ClassNode* node = DecompilerContext::current().current_class_node;
        if (node != nullptr) {
            const StructField* fd = node->class_struct->get_field(field->get_name(), field->get_descriptor().descriptor_string);
            if (fd != nullptr) {
                if (field->is_static() && fd->has_modifier(cc::ACC_FINAL)) field_in_class_init = true;
                if (node->get_wrapper() != nullptr &&
                    node->get_wrapper()->get_hidden_members().contains(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()))) {
                    hidden_field = true;
                }
            }
        }
    }

    if (hidden_field) return TextBuffer();

    TextBuffer buffer;
    if (field_in_class_init) buffer.append(static_cast<const FieldExprent*>(left_)->get_name());
    else buffer.append(left_->to_java(indent, tracer));

    TextBuffer res = right_->to_java(indent, tracer);

    if (cond_type_ == CONDITION_NONE && !left_type.is_superset(right_type) && (right_type == VarType::OBJECT || left_type.type != cc::TYPE_OBJECT)) {
        if (right_->get_precedence() >= FunctionExprent::get_precedence(FunctionExprent::FUNCTION_CAST)) res.enclose("(", ")");
        res.prepend("(" + ExprProcessor::get_cast_type_name(left_type) + ")");
    }

    buffer.append(cond_type_ == CONDITION_NONE ? " = " : ASSIGNMENT_OPERATORS[cond_type_]).append(res);
    tracer.add_mapping(bytecode);
    return buffer;
}

void AssignmentExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == left_) left_ = new_expr;
    if (old_expr == right_) right_ = new_expr;
}

bool AssignmentExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const AssignmentExprent* as = dynamic_cast<const AssignmentExprent*>(o);
    if (as == nullptr) return false;
    return equal_exprents(left_, as->get_left()) && equal_exprents(right_, as->get_right()) && cond_type_ == as->get_cond_type();
}

// ---- ExitExprent

ExitExprent::ExitExprent(int exit_type, Exprent* value, VarType ret_type, const Offsets& offsets)
    : Exprent(EXPRENT_EXIT), exit_type_(exit_type), value_(value), ret_type_(std::move(ret_type)) {
    add_bytecode_offsets(offsets);
}

Exprent* ExitExprent::copy() const { return make<ExitExprent>(exit_type_, value_ == nullptr ? nullptr : value_->copy(), ret_type_, bytecode); }

CheckTypesResult ExitExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    if (exit_type_ == EXIT_RETURN && ret_type_.type != cc::TYPE_VOID) {
        result.add_min_type_exprent(value_, VarType::min_type_in_family(ret_type_.type_family));
        result.add_max_type_exprent(value_, ret_type_);
    }
    return result;
}

std::vector<Exprent*> ExitExprent::get_all_exprents() {
    std::vector<Exprent*> lst;
    if (value_ != nullptr) lst.push_back(value_);
    return lst;
}

TextBuffer ExitExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    tracer.add_mapping(bytecode);

    if (exit_type_ == EXIT_RETURN) {
        TextBuffer buffer;
        if (ret_type_.type != cc::TYPE_VOID) {
            buffer.append(" ");
            ExprProcessor::get_casted_exprent(value_, ret_type_, buffer, indent, false, tracer);
        }
        return buffer.prepend("return");
    }
    MethodWrapper* method = DecompilerContext::current().current_method_wrapper;
    ClassNode* node = DecompilerContext::current().current_class_node;

    if (method != nullptr && node != nullptr) {
        const ExceptionsAttribute* exc_attr = method->method_struct->attribute<ExceptionsAttribute>(attr::EXCEPTIONS);
        if (exc_attr != nullptr) {
            std::optional<std::string> classname;
            for (std::size_t i = 0; i < exc_attr->throws_exceptions().size(); i++) {
                const std::string ex_class_name = exc_attr->exception_class_name(i, node->class_struct->pool());
                if (ex_class_name == "java/lang/Throwable") {
                    classname = ex_class_name;
                    break;
                }
                if (ex_class_name == "java/lang/Exception") classname = ex_class_name;
            }

            if (classname) {
                const VarType ex_type = VarType::parse(*classname, true);
                TextBuffer buffer;
                ExprProcessor::get_casted_exprent(value_, ex_type, buffer, indent, false, tracer);
                return buffer.prepend("throw ");
            }
        }
    }

    return value_->to_java(indent, tracer).prepend("throw ");
}

void ExitExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == value_) value_ = new_expr;
}

bool ExitExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const ExitExprent* et = dynamic_cast<const ExitExprent*>(o);
    if (et == nullptr) return false;
    return exit_type_ == et->get_exit_type() && equal_exprents(value_, et->get_value());
}

bool ExitExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;
    const MatchValue exit_type = match_node->get_rule_value(MatchProperties::EXPRENT_EXITTYPE);
    if (!std::holds_alternative<std::monostate>(exit_type)) {
        if (this->exit_type_ != std::get<std::int32_t>(exit_type)) return false;
    }
    return true;
}

// ---- IfExprent

namespace {
const int IF_FUNC_TYPES[] = {
    FunctionExprent::FUNCTION_EQ,  FunctionExprent::FUNCTION_NE,  FunctionExprent::FUNCTION_LT,       FunctionExprent::FUNCTION_GE,
    FunctionExprent::FUNCTION_GT,  FunctionExprent::FUNCTION_LE,  FunctionExprent::FUNCTION_EQ,       FunctionExprent::FUNCTION_NE,
    FunctionExprent::FUNCTION_EQ,  FunctionExprent::FUNCTION_NE,  FunctionExprent::FUNCTION_LT,       FunctionExprent::FUNCTION_GE,
    FunctionExprent::FUNCTION_GT,  FunctionExprent::FUNCTION_LE,  FunctionExprent::FUNCTION_EQ,       FunctionExprent::FUNCTION_NE,
    FunctionExprent::FUNCTION_CADD, FunctionExprent::FUNCTION_COR, FunctionExprent::FUNCTION_BOOL_NOT, -1};
}  // namespace

IfExprent::IfExprent(int if_type, ListStack<Exprent*>& stack, const Offsets& offsets) : IfExprent(nullptr, offsets) {
    if (if_type <= IF_LE) {
        stack.push(make<ConstExprent>(0, true, std::nullopt));
    } else if (if_type <= IF_NONNULL) {
        stack.push(make<ConstExprent>(VarType::NULL_TYPE, ConstExprent::Value{}, std::nullopt));
    }

    if (if_type == IF_VALUE) {
        condition_ = stack.pop();
    } else {
        condition_ = make<FunctionExprent>(IF_FUNC_TYPES[if_type], stack, std::nullopt);
    }
}

IfExprent::IfExprent(Exprent* condition, const Offsets& offsets) : Exprent(EXPRENT_IF), condition_(condition) { add_bytecode_offsets(offsets); }

Exprent* IfExprent::copy() const { return make<IfExprent>(condition_->copy(), bytecode); }

TextBuffer IfExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    tracer.add_mapping(bytecode);
    return condition_->to_java(indent, tracer).enclose("if(", ")");
}

void IfExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == condition_) condition_ = new_expr;
}

bool IfExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const IfExprent* ie = dynamic_cast<const IfExprent*>(o);
    if (ie == nullptr) return false;
    return equal_exprents(condition_, ie->get_condition());
}

IfExprent* IfExprent::negate_if() {
    condition_ = make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, condition_, condition_->bytecode);
    return this;
}

// ---- MonitorExprent

MonitorExprent::MonitorExprent(int mon_type, Exprent* value, const Offsets& offsets) : Exprent(EXPRENT_MONITOR), mon_type_(mon_type), value_(value) {
    add_bytecode_offsets(offsets);
}

Exprent* MonitorExprent::copy() const { return make<MonitorExprent>(mon_type_, value_->copy(), bytecode); }

TextBuffer MonitorExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    tracer.add_mapping(bytecode);
    if (mon_type_ == MONITOR_ENTER) return value_->to_java(indent, tracer).enclose("synchronized(", ")");
    return TextBuffer();
}

void MonitorExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == value_) value_ = new_expr;
}

bool MonitorExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const MonitorExprent* me = dynamic_cast<const MonitorExprent*>(o);
    if (me == nullptr) return false;
    return mon_type_ == me->get_mon_type() && equal_exprents(value_, me->get_value());
}

// ---- SwitchExprent

SwitchExprent::SwitchExprent(Exprent* value, const Offsets& offsets) : Exprent(EXPRENT_SWITCH), value_(value) { add_bytecode_offsets(offsets); }

Exprent* SwitchExprent::copy() const {
    SwitchExprent* sw_expr = make<SwitchExprent>(value_->copy(), bytecode);
    sw_expr->set_case_values(case_values_);
    return sw_expr;
}

CheckTypesResult SwitchExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    result.add_min_type_exprent(value_, VarType::BYTECHAR);
    result.add_max_type_exprent(value_, VarType::INT);

    VarType val_type = value_->get_expr_type();
    for (const std::vector<ConstExprent*>& lst : case_values_) {
        for (ConstExprent* expr : lst) {
            if (expr != nullptr) {
                const VarType case_type = expr->get_expr_type();
                if (!(case_type == val_type)) {
                    val_type = VarType::common_supertype(case_type, val_type).value();
                    result.add_min_type_exprent(value_, val_type);
                }
            }
        }
    }
    return result;
}

TextBuffer SwitchExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    tracer.add_mapping(bytecode);
    return value_->to_java(indent, tracer).enclose("switch(", ")");
}

void SwitchExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == value_) value_ = new_expr;
}

bool SwitchExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const SwitchExprent* sw = dynamic_cast<const SwitchExprent*>(o);
    if (sw == nullptr) return false;
    return equal_exprents(value_, sw->get_value());
}

}  // namespace cb::java::decompiler
