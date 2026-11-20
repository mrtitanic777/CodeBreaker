// java/decompiler/exprents.hpp -- the expressions the decompiler builds a method's code from: constants, variables, fields, array elements, assignments,
// operators and casts (functions), calls, object and array creation, conditions, switches, returns and throws, monitors, annotations and asserts; their
// types, the type bounds they put on their operands, their copies, comparisons, and their Java text.
//
//     Exprent* e = make<AssignmentExprent>(make<VarExprent>(1, VarType::INT, proc), make<ConstExprent>(5, false, std::nullopt), std::nullopt);
//     e->to_java(0, tracer).to_string();   // "var1 = 5"
//
// Ported from Fernflower (de.fernflower.modules.decompiler.exps, vars.VarVersionPair, vars.CheckTypesResult), Copyright 2000-2015 JetBrains s.r.o., Apache
// License 2.0 (see NOTICE in this folder). Expressions are objects of the run's arena (make<T>, context.hpp): pointers to them are the original's
// references (== is identity; equals() is the original's equals). A Java `null` type where the original would go on to dereference it throws here.
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/instructions.hpp"
#include "java/decompiler/match.hpp"
#include "java/decompiler/text_buffer.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class BytecodeMappingTracer;
struct LinkConstant;
class VarProcessor;
class Exprent;
class ConstExprent;
class InvocationExprent;
class VarExprent;

using Offsets = std::optional<std::set<int>>;   // the bytecode offsets an expression was made from (the original's null: none)

// A variable and its SSA version.
struct VarVersionPair {
    int var = 0, version = 0;
    VarVersionPair() = default;
    VarVersionPair(int var, int version) : var(var), version(version) {}
    explicit VarVersionPair(const VarExprent* var);
    friend bool operator==(const VarVersionPair& a, const VarVersionPair& b) = default;
    friend auto operator<=>(const VarVersionPair& a, const VarVersionPair& b) = default;
    std::int32_t java_hash() const { return var * 3 + version; }
    std::string to_string() const { return "(" + std::to_string(var) + "," + std::to_string(version) + ")"; }
};

struct ExprentTypePair {
    Exprent* exprent;
    VarType type;
    std::optional<VarType> desttype;
};

struct CheckTypesResult {
    std::vector<ExprentTypePair> lst_max_type_exprents, lst_min_type_exprents;
    void add_max_type_exprent(Exprent* e, const VarType& t) { lst_max_type_exprents.push_back({e, t, std::nullopt}); }
    void add_min_type_exprent(Exprent* e, const VarType& t) { lst_min_type_exprents.push_back({e, t, std::nullopt}); }
};

class Exprent : public ArenaObject, public IMatchable {
public:
    static constexpr int MULTIPLE_USES = 1, SIDE_EFFECTS_FREE = 2, BOTH_FLAGS = 3;
    static constexpr int EXPRENT_ARRAY = 1, EXPRENT_ASSIGNMENT = 2, EXPRENT_CONST = 3, EXPRENT_EXIT = 4, EXPRENT_FIELD = 5, EXPRENT_FUNCTION = 6,
                         EXPRENT_IF = 7, EXPRENT_INVOCATION = 8, EXPRENT_MONITOR = 9, EXPRENT_NEW = 10, EXPRENT_SWITCH = 11, EXPRENT_VAR = 12,
                         EXPRENT_ANNOTATION = 13, EXPRENT_ASSERT = 14;

    const int type;
    const int id;
    Offsets bytecode;

    explicit Exprent(int type);

    virtual int get_precedence() const { return 0; }
    virtual VarType get_expr_type() const { return VarType::VOID; }
    virtual int get_exprent_use() const { return 0; }
    virtual CheckTypesResult check_expr_type_bounds() { return {}; }
    bool contains_exprent(const Exprent* e);
    std::vector<Exprent*> get_all_exprents(bool recursive);
    // The variables this expression and every one inside it uses (the original's HashSet of pairs, only ever searched).
    std::set<VarVersionPair> get_all_variables();
    virtual std::vector<Exprent*> get_all_exprents();   // std::runtime_error "not implemented" in the kinds that have none
    virtual Exprent* copy() const;
    virtual TextBuffer to_java(int indent, BytecodeMappingTracer& tracer);
    virtual void replace_exprent(Exprent* old_expr, Exprent* new_expr) { (void)old_expr, (void)new_expr; }
    // The original's equals (identity first): the same kind and the same contents.
    virtual bool equals(const Exprent* o) const { return o == this; }
    void add_bytecode_offsets(const Offsets& offsets);

    // The original's IMatchable (match.hpp).
    // find_object only ever answers one of the expression's own sub-expressions (nullptr: none).
    Exprent* find_object(const MatchNode* match_node, int index) override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    bool equals_matchable(const IMatchable* other) const override;
};

// The original's InterpreterUtil.equalObjects / equalLists over expressions.
bool equal_exprents(const Exprent* a, const Exprent* b);
bool equal_exprent_lists(const std::vector<Exprent*>& a, const std::vector<Exprent*>& b);

class ArrayExprent : public Exprent {
public:
    ArrayExprent(Exprent* array, Exprent* index, VarType hard_type, const Offsets& offsets);
    Exprent* copy() const override;
    VarType get_expr_type() const override;
    int get_exprent_use() const override;
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {array_, index_}; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    Exprent* get_array() const { return array_; }
    Exprent* get_index() const { return index_; }

private:
    Exprent* array_;
    Exprent* index_;
    VarType hard_type_;
};

class AssignmentExprent : public Exprent {
public:
    static constexpr int CONDITION_NONE = -1;
    AssignmentExprent(Exprent* left, Exprent* right, const Offsets& offsets);
    VarType get_expr_type() const override;
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {left_, right_}; }
    Exprent* copy() const override;
    int get_precedence() const override { return 13; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    Exprent* get_left() const { return left_; }
    void set_left(Exprent* e) { left_ = e; }
    Exprent* get_right() const { return right_; }
    void set_right(Exprent* e) { right_ = e; }
    int get_cond_type() const { return cond_type_; }
    void set_cond_type(int t) { cond_type_ = t; }

private:
    Exprent* left_;
    Exprent* right_;
    int cond_type_ = CONDITION_NONE;
};

class AssertExprent : public Exprent {
public:
    explicit AssertExprent(std::vector<Exprent*> parameters) : Exprent(EXPRENT_ASSERT), parameters_(std::move(parameters)) {}
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;

private:
    std::vector<Exprent*> parameters_;   // the condition (null: false), and the message if any
};

class AnnotationExprent : public Exprent {
public:
    static constexpr int ANNOTATION_NORMAL = 1, ANNOTATION_MARKER = 2, ANNOTATION_SINGLE_ELEMENT = 3;
    AnnotationExprent(std::string class_name, std::vector<std::string> par_names, std::vector<Exprent*> par_values)
        : Exprent(EXPRENT_ANNOTATION), class_name_(std::move(class_name)), par_names_(std::move(par_names)), par_values_(std::move(par_values)) {}
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    const std::string& get_class_name() const { return class_name_; }
    int get_annotation_type() const;
    bool equals(const Exprent* o) const override;

private:
    std::string class_name_;
    std::vector<std::string> par_names_;
    std::vector<Exprent*> par_values_;
};

class ConstExprent : public Exprent {
public:
    using Value = std::variant<std::monostate, std::int32_t, std::int64_t, float, double, std::string>;   // monostate: null
    ConstExprent(int val, bool bool_permitted, const Offsets& offsets);
    ConstExprent(VarType const_type, Value value, const Offsets& offsets);
    Exprent* copy() const override;
    VarType get_expr_type() const override { return const_type_; }
    int get_exprent_use() const override { return MULTIPLE_USES | SIDE_EFFECTS_FREE; }
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {}; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    bool has_boolean_value() const;
    bool has_value_one() const;
    static ConstExprent* get_zero_constant(int type);
    const VarType& get_const_type() const { return const_type_; }
    void set_const_type(const VarType& t) { const_type_ = t; }
    const Value& get_value() const { return value_; }
    std::int32_t get_int_value() const { return std::get<std::int32_t>(value_); }
    bool is_bool_permitted() const { return bool_permitted_; }

private:
    ConstExprent(VarType const_type, Value value, bool bool_permitted, const Offsets& offsets);
    VarType const_type_;
    Value value_;
    bool bool_permitted_;
};
// Java's toString of a constant's value (Integer, Long, Float, Double, String; "null").
std::string java_value_string(const ConstExprent::Value& v);
// Java's Double.toString and Float.toString (shortest digits that read back the same, Java's layout).
std::string java_double_string(double d);
std::string java_float_string(float f);

class ExitExprent : public Exprent {
public:
    static constexpr int EXIT_RETURN = 0, EXIT_THROW = 1;
    ExitExprent(int exit_type, Exprent* value, VarType ret_type, const Offsets& offsets);
    Exprent* copy() const override;
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override;
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    int get_exit_type() const { return exit_type_; }
    Exprent* get_value() const { return value_; }
    const VarType& get_ret_type() const { return ret_type_; }

private:
    int exit_type_;
    Exprent* value_;
    VarType ret_type_;
};

class FieldExprent : public Exprent {
public:
    FieldExprent(const LinkConstant& cn, Exprent* instance, const Offsets& offsets);
    FieldExprent(std::string name, std::string classname, bool is_static, Exprent* instance, FieldDescriptor descriptor, const Offsets& offsets);
    VarType get_expr_type() const override { return descriptor_.type; }
    int get_exprent_use() const override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override;
    Exprent* copy() const override;
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    const std::string& get_classname() const { return classname_; }
    const FieldDescriptor& get_descriptor() const { return descriptor_; }
    Exprent* get_instance() const { return instance_; }
    bool is_static() const { return is_static_; }
    const std::string& get_name() const { return name_; }

private:
    std::string name_, classname_;
    bool is_static_;
    Exprent* instance_;
    FieldDescriptor descriptor_;
};

class FunctionExprent : public Exprent {
public:
    static constexpr int FUNCTION_ADD = 0, FUNCTION_SUB = 1, FUNCTION_MUL = 2, FUNCTION_DIV = 3, FUNCTION_AND = 4, FUNCTION_OR = 5, FUNCTION_XOR = 6,
                         FUNCTION_REM = 7, FUNCTION_SHL = 8, FUNCTION_SHR = 9, FUNCTION_USHR = 10, FUNCTION_BIT_NOT = 11, FUNCTION_BOOL_NOT = 12,
                         FUNCTION_NEG = 13, FUNCTION_I2L = 14, FUNCTION_I2F = 15, FUNCTION_I2D = 16, FUNCTION_L2I = 17, FUNCTION_L2F = 18,
                         FUNCTION_L2D = 19, FUNCTION_F2I = 20, FUNCTION_F2L = 21, FUNCTION_F2D = 22, FUNCTION_D2I = 23, FUNCTION_D2L = 24,
                         FUNCTION_D2F = 25, FUNCTION_I2B = 26, FUNCTION_I2C = 27, FUNCTION_I2S = 28, FUNCTION_CAST = 29, FUNCTION_INSTANCEOF = 30,
                         FUNCTION_ARRAY_LENGTH = 31, FUNCTION_IMM = 32, FUNCTION_MMI = 33, FUNCTION_IPP = 34, FUNCTION_PPI = 35, FUNCTION_IIF = 36,
                         FUNCTION_LCMP = 37, FUNCTION_FCMPL = 38, FUNCTION_FCMPG = 39, FUNCTION_DCMPL = 40, FUNCTION_DCMPG = 41, FUNCTION_EQ = 42,
                         FUNCTION_NE = 43, FUNCTION_LT = 44, FUNCTION_GE = 45, FUNCTION_GT = 46, FUNCTION_LE = 47, FUNCTION_CADD = 48, FUNCTION_COR = 49,
                         FUNCTION_STR_CONCAT = 50;

    FunctionExprent(int func_type, ListStack<Exprent*>& stack, const Offsets& offsets);
    FunctionExprent(int func_type, std::vector<Exprent*> operands, const Offsets& offsets);
    FunctionExprent(int func_type, Exprent* operand, const Offsets& offsets);
    VarType get_expr_type() const override;
    int get_exprent_use() const override;
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return lst_operands_; }
    Exprent* copy() const override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    int get_precedence() const override { return get_precedence(func_type_); }
    static int get_precedence(int func);
    VarType get_simple_cast_type() const;
    int get_func_type() const { return func_type_; }
    void set_func_type(int t) { func_type_ = t; }
    std::vector<Exprent*>& get_lst_operands() { return lst_operands_; }
    const std::vector<Exprent*>& get_lst_operands() const { return lst_operands_; }
    void set_implicit_type(const std::optional<VarType>& t) { implicit_type_ = t; }

private:
    TextBuffer wrap_operand_string(Exprent* expr, bool eq, int indent, BytecodeMappingTracer& tracer);
    int func_type_;
    std::optional<VarType> implicit_type_;
    std::vector<Exprent*> lst_operands_;
};

class IfExprent : public Exprent {
public:
    static constexpr int IF_EQ = 0, IF_NE = 1, IF_LT = 2, IF_GE = 3, IF_GT = 4, IF_LE = 5, IF_NULL = 6, IF_NONNULL = 7, IF_ICMPEQ = 8, IF_ICMPNE = 9,
                         IF_ICMPLT = 10, IF_ICMPGE = 11, IF_ICMPGT = 12, IF_ICMPLE = 13, IF_ACMPEQ = 14, IF_ACMPNE = 15, IF_CAND = 16, IF_COR = 17,
                         IF_NOT = 18, IF_VALUE = 19;
    IfExprent(int if_type, ListStack<Exprent*>& stack, const Offsets& offsets);
    IfExprent(Exprent* condition, const Offsets& offsets);   // (private in the original, which only copy() uses; public here as make<T> needs it)
    Exprent* copy() const override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {condition_}; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    IfExprent* negate_if();
    Exprent* get_condition() const { return condition_; }
    void set_condition(Exprent* c) { condition_ = c; }

private:
    Exprent* condition_;
};

class InvocationExprent : public Exprent {
public:
    static constexpr int INVOKE_SPECIAL = 1, INVOKE_VIRTUAL = 2, INVOKE_STATIC = 3, INVOKE_INTERFACE = 4, INVOKE_DYNAMIC = 5;
    static constexpr int TYP_GENERAL = 1, TYP_INIT = 2, TYP_CLINIT = 3;

    InvocationExprent() : Exprent(EXPRENT_INVOCATION) {}
    InvocationExprent(int opcode, const LinkConstant& cn, ListStack<Exprent*>& stack, int dynamic_invocation_type, const Offsets& offsets);
    // The original's private copy constructor (a new expression with a new id; instance and parameters copied): copy() is its only caller, public as
    // make<T> needs it.
    explicit InvocationExprent(const InvocationExprent& expr);
    VarType get_expr_type() const override { return descriptor_.ret; }
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override;
    Exprent* copy() const override;
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;

    std::vector<Exprent*>& get_lst_parameters() { return lst_parameters_; }
    const std::vector<Exprent*>& get_lst_parameters() const { return lst_parameters_; }
    void set_lst_parameters(std::vector<Exprent*> v) { lst_parameters_ = std::move(v); }
    const MethodDescriptor& get_descriptor() const { return descriptor_; }
    void set_descriptor(const MethodDescriptor& d) { descriptor_ = d; }
    const std::string& get_classname() const { return classname_; }
    void set_classname(const std::string& c) { classname_ = c; }
    int get_functype() const { return functype_; }
    void set_functype(int f) { functype_ = f; }
    Exprent* get_instance() const { return instance_; }
    void set_instance(Exprent* e) { instance_ = e; }
    bool is_static() const { return is_static_; }
    void set_static(bool s) { is_static_ = s; }
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& n) { name_ = n; }
    const std::string& get_string_descriptor() const { return string_descriptor_; }
    void set_string_descriptor(const std::string& d) { string_descriptor_ = d; }
    int get_invocation_typ() const { return invocation_typ_; }
    const std::optional<std::string>& get_invoke_dynamic_class_suffix() const { return invoke_dynamic_class_suffix_; }

private:
    std::vector<bool> get_ambiguous_parameters() const;   // empty: none ambiguous
    std::string name_, classname_;
    bool is_static_ = false;
    int functype_ = TYP_GENERAL;
    Exprent* instance_ = nullptr;
    MethodDescriptor descriptor_;
    std::string string_descriptor_;
    std::optional<std::string> invoke_dynamic_class_suffix_;
    int invocation_typ_ = INVOKE_VIRTUAL;
    std::vector<Exprent*> lst_parameters_;
};

class MonitorExprent : public Exprent {
public:
    static constexpr int MONITOR_ENTER = 0, MONITOR_EXIT = 1;
    MonitorExprent(int mon_type, Exprent* value, const Offsets& offsets);
    Exprent* copy() const override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {value_}; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    int get_mon_type() const { return mon_type_; }
    Exprent* get_value() const { return value_; }

private:
    int mon_type_;
    Exprent* value_;
};

class NewExprent : public Exprent {
public:
    NewExprent(VarType new_type, ListStack<Exprent*>& stack, int array_dim, const Offsets& offsets);
    NewExprent(VarType new_type, std::vector<Exprent*> lst_dims, const Offsets& offsets);
    VarType get_expr_type() const override;
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override;
    Exprent* copy() const override;
    int get_precedence() const override { return 1; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;

    InvocationExprent* get_constructor() const { return constructor_; }
    void set_constructor(InvocationExprent* c) { constructor_ = c; }
    std::vector<Exprent*>& get_lst_dims() { return lst_dims_; }
    const VarType& get_new_type() const { return new_type_; }
    std::vector<Exprent*>& get_lst_array_elements() { return lst_array_elements_; }
    void set_lst_array_elements(std::vector<Exprent*> v) { lst_array_elements_ = std::move(v); }
    void set_direct_array_init(bool b) { direct_array_init_ = b; }
    bool is_lambda() const { return lambda_; }
    bool is_anonymous() const { return anonymous_; }
    void set_anonymous(bool b) { anonymous_ = b; }
    void set_enum_const(bool b) { enum_const_ = b; }

private:
    static std::vector<Exprent*> get_dimensions(int array_dim, ListStack<Exprent*>& stack);   // the top `array_dim` entries, deepest first
    // The text of the enclosing instance of a qualified `new` of an inner class ("outer.new Inner()"), none when the class is not one (the original's null).
    static std::optional<std::string> get_qualified_new_instance(const std::string& classname, const std::vector<Exprent*>& lst_params, int indent,
                                                                  BytecodeMappingTracer& tracer);
    InvocationExprent* constructor_ = nullptr;
    VarType new_type_;
    std::vector<Exprent*> lst_dims_, lst_array_elements_;
    bool direct_array_init_ = false, anonymous_ = false, lambda_ = false, enum_const_ = false;
};

class SwitchExprent : public Exprent {
public:
    SwitchExprent(Exprent* value, const Offsets& offsets);
    Exprent* copy() const override;
    VarType get_expr_type() const override { return value_->get_expr_type(); }
    CheckTypesResult check_expr_type_bounds() override;
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {value_}; }
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    void replace_exprent(Exprent* old_expr, Exprent* new_expr) override;
    bool equals(const Exprent* o) const override;
    Exprent* get_value() const { return value_; }
    void set_case_values(std::vector<std::vector<ConstExprent*>> v) { case_values_ = std::move(v); }

private:
    Exprent* value_;
    std::vector<std::vector<ConstExprent*>> case_values_;
};

class VarExprent : public Exprent {
public:
    static constexpr int STACK_BASE = 10000;
    static constexpr std::string_view VAR_NAMELESS_ENCLOSURE = "<VAR_NAMELESS_ENCLOSURE>";
    VarExprent(int index, std::optional<VarType> var_type, VarProcessor* processor);
    VarType get_expr_type() const override { return get_var_type(); }
    int get_exprent_use() const override { return MULTIPLE_USES | SIDE_EFFECTS_FREE; }
    using Exprent::get_all_exprents;
    std::vector<Exprent*> get_all_exprents() override { return {}; }
    Exprent* copy() const override;
    TextBuffer to_java(int indent, BytecodeMappingTracer& tracer) override;
    bool equals(const Exprent* o) const override;
    bool match(const MatchNode* match_node, MatchEngine& engine) override;
    int get_index() const { return index_; }
    void set_index(int i) { index_ = i; }
    VarType get_var_type() const;
    void set_var_type(const std::optional<VarType>& t) { var_type_ = t; }
    bool is_definition() const { return definition_; }
    void set_definition(bool b) { definition_ = b; }
    VarProcessor* get_processor() const { return processor_; }
    void set_processor(VarProcessor* p) { processor_ = p; }
    int get_version() const { return version_; }
    void set_version(int v) { version_ = v; }
    bool is_class_def() const { return class_def_; }
    void set_class_def(bool b) { class_def_ = b; }
    bool is_stack() const { return stack_; }
    void set_stack(bool b) { stack_ = b; }

private:
    int index_;
    std::optional<VarType> var_type_;
    bool definition_ = false;
    VarProcessor* processor_;
    int version_ = 0;
    bool class_def_ = false, stack_ = false;
};

}  // namespace cb::java::decompiler
