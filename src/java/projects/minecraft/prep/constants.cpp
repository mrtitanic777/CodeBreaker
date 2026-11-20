// The constants of constants.hpp: Constant and its kinds, ConstantBuilder, ConstantPool, replace_common_constants, and Java's number to text.
// Ported from RDI (org.mcphackers.rdi.injector.data.constants.*, injector.transform.Transform.replaceCommonConstants), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

#include "java/projects/minecraft/prep/constants.hpp"

namespace cb::java::minecraft::prep {

namespace {

using bc::InsnList;
using bc::InsnNode;
namespace op = bc::op;

// ---- Java's number to text

template <class F>
std::string fp_to_string(F v) {
    if (std::isnan(v)) return "NaN";
    if (std::isinf(v)) return v < 0 ? "-Infinity" : "Infinity";
    const bool neg = std::signbit(v);
    const F a = std::fabs(v);
    if (a == 0) return neg ? "-0.0" : "0.0";
    char buf[64];
    auto result = std::to_chars(buf, buf + sizeof buf, a, std::chars_format::scientific);
    std::string sci(buf, result.ptr);
    // a subnormal that one digit reads back as: Java picks the closest decimal of two digits
    if (sci.find('.') == std::string::npos && a < std::numeric_limits<F>::min()) {
        result = std::to_chars(buf, buf + sizeof buf, a, std::chars_format::scientific, 1);
        const std::string two(buf, result.ptr);
        F back{};
        std::from_chars(two.data(), two.data() + two.size(), back);
        if (back == a) sci = two;
    }
    const std::size_t e_at = sci.find('e');
    std::string digits;
    for (std::size_t i = 0; i < e_at; ++i)
        if (sci[i] != '.') digits += sci[i];
    const int exponent = std::stoi(sci.substr(e_at + 1));
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    std::string out = neg ? "-" : "";
    if (exponent >= -3 && exponent < 7) {
        if (exponent >= 0) {
            const std::size_t int_len = static_cast<std::size_t>(exponent) + 1;
            std::string int_part = digits.substr(0, std::min(int_len, digits.size()));
            if (int_part.size() < int_len) int_part.append(int_len - int_part.size(), '0');
            out += int_part + "." + (digits.size() > int_len ? digits.substr(int_len) : "0");
        } else {
            out += "0." + std::string(static_cast<std::size_t>(-exponent - 1), '0') + digits;
        }
    } else {
        out += digits.substr(0, 1) + "." + (digits.size() > 1 ? digits.substr(1) : "0") + "E" + std::to_string(exponent);
    }
    return out;
}

// the position of the first '.' (Java's indexOf; -1 when none)
int index_of_dot(const std::string& s) {
    const std::size_t p = s.find('.');
    return p == std::string::npos ? -1 : static_cast<int>(p);
}
int length_of(const std::string& s) { return static_cast<int>(s.size()); }
bool ends_with_5(const std::string& s) { return !s.empty() && s.back() == '5'; }

// ---- Java arithmetic the builder follows

std::int32_t d2i(double v) {
    if (std::isnan(v)) return 0;
    if (v >= 2147483647.0) return std::numeric_limits<std::int32_t>::max();
    if (v <= -2147483648.0) return std::numeric_limits<std::int32_t>::min();
    return static_cast<std::int32_t>(v);
}
std::int64_t d2l(double v) {
    if (std::isnan(v)) return 0;
    if (v >= 9223372036854775807.0) return std::numeric_limits<std::int64_t>::max();
    if (v <= -9223372036854775808.0) return std::numeric_limits<std::int64_t>::min();
    return static_cast<std::int64_t>(v);
}

std::int32_t wrap_op(int which, std::int32_t a, std::int32_t b) {   // 0 mul, 1 div, 2 add, 3 sub
    const auto ua = static_cast<std::uint32_t>(a), ub = static_cast<std::uint32_t>(b);
    switch (which) {
    case 0: return static_cast<std::int32_t>(ua * ub);
    case 1:
        if (b == 0) throw std::runtime_error("/ by zero");
        if (a == std::numeric_limits<std::int32_t>::min() && b == -1) return a;
        return a / b;
    case 2: return static_cast<std::int32_t>(ua + ub);
    default: return static_cast<std::int32_t>(ua - ub);
    }
}
std::int64_t wrap_op(int which, std::int64_t a, std::int64_t b) {
    const auto ua = static_cast<std::uint64_t>(a), ub = static_cast<std::uint64_t>(b);
    switch (which) {
    case 0: return static_cast<std::int64_t>(ua * ub);
    case 1:
        if (b == 0) throw std::runtime_error("/ by zero");
        if (a == std::numeric_limits<std::int64_t>::min() && b == -1) return a;
        return a / b;
    case 2: return static_cast<std::int64_t>(ua + ub);
    default: return static_cast<std::int64_t>(ua - ub);
    }
}
template <class F>
F wrap_op(int which, F a, F b) {
    switch (which) {
    case 0: return a * b;
    case 1: return a / b;
    case 2: return a + b;
    default: return a - b;
    }
}

// Float.equals / Double.equals: the same bits, every NaN one value
template <class F>
bool fp_equals(F a, F b) {
    if (std::isnan(a) && std::isnan(b)) return true;
    return std::memcmp(&a, &b, sizeof a) == 0;
}

// Integer.equals ... between a constant and a value (the same box type, and the same value)
bool box_equals(const bc::Constant& a, const bc::Constant& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<float>(a)) return fp_equals(std::get<float>(a), std::get<float>(b));
    if (std::holds_alternative<double>(a)) return fp_equals(std::get<double>(a), std::get<double>(b));
    if (std::holds_alternative<std::int32_t>(a)) return std::get<std::int32_t>(a) == std::get<std::int32_t>(b);
    if (std::holds_alternative<std::int64_t>(a)) return std::get<std::int64_t>(a) == std::get<std::int64_t>(b);
    return false;   // (only the four boxes are compared)
}

template <class T> constexpr int builder_type() {
    if constexpr (std::is_same_v<T, std::int32_t>) return ConstantBuilder::INT;
    else if constexpr (std::is_same_v<T, std::int64_t>) return ConstantBuilder::LONG;
    else if constexpr (std::is_same_v<T, float>) return ConstantBuilder::FLOAT;
    else return ConstantBuilder::DOUBLE;
}
template <class T> constexpr const char* field_desc() {
    if constexpr (std::is_same_v<T, std::int32_t>) return "I";
    else if constexpr (std::is_same_v<T, std::int64_t>) return "J";
    else if constexpr (std::is_same_v<T, float>) return "F";
    else return "D";
}
template <class T> InsnNode* value_insn(InsnList& owner, T v) {
    if constexpr (std::is_same_v<T, std::int32_t>) return InsnHelper::int_insn(owner, v);
    else if constexpr (std::is_same_v<T, std::int64_t>) return InsnHelper::long_insn(owner, v);
    else if constexpr (std::is_same_v<T, float>) return InsnHelper::float_insn(owner, v);
    else return InsnHelper::double_insn(owner, v);
}

// the arithmetic opcode of an operation on the type: imul lmul fmul dmul, idiv ..., iadd ..., isub ...
int arith_opcode(int which, int type) {
    static const int base[4] = {op::IMUL, op::IDIV, op::IADD, op::ISUB};
    return base[which] + type;
}

}  // namespace

std::string java_double_to_string(double value) { return fp_to_string(value); }
std::string java_float_to_string(float value) { return fp_to_string(value); }

// ---- Constant

bool Constant::replace(InsnList& source_list, InsnNode* constant) {
    if (constant->get_type() == InsnNode::LDC_INSN) return replace(source_list, static_cast<bc::LdcInsnNode*>(constant));
    return false;
}

// ---- FloatCastedToDouble

bool FloatCastedToDouble::replace(InsnList& source_list, bc::LdcInsnNode* constant) {
    if (std::holds_alternative<double>(constant->cst)) {
        const double cst_double = std::get<double>(constant->cst);
        const float cst_float = static_cast<float>(cst_double);
        const std::string d = java_double_to_string(cst_double);
        const std::string f = java_float_to_string(cst_float);
        if (cst_double == static_cast<double>(cst_float) && length_of(f) < length_of(d)) {
            InsnList list;
            list.add(list.make<bc::LdcInsnNode>(bc::Constant(cst_float)));
            list.add(list.make<bc::ZeroInsnNode>(op::F2D));
            source_list.insert(constant, list);
            source_list.remove(constant);
            return true;
        }
    }
    return false;
}

// ---- OneDivideByPowerOfTwo

bool OneDivideByPowerOfTwo::replace(InsnList& source_list, bc::LdcInsnNode* constant) {
    if (std::holds_alternative<double>(constant->cst)) {
        double divisor = 1.0;
        double cst = std::get<double>(constant->cst);
        if (cst > 1.0 / 16.0) return false;
        const std::string d = java_double_to_string(cst);
        if (!ends_with_5(d)) return false;
        while (cst < 1 && !std::isinf(cst)) {
            divisor *= 2.0;
            cst *= 2.0;
        }
        if (cst == 1.0) {
            InsnList list;
            list.add(list.make<bc::ZeroInsnNode>(op::DCONST_1));
            list.add(InsnHelper::double_insn(list, divisor));
            list.add(list.make<bc::ZeroInsnNode>(op::DDIV));
            source_list.insert(constant, list);
            source_list.remove(constant);
            return true;
        }
        return false;
    }
    if (std::holds_alternative<float>(constant->cst)) {
        float divisor = 1.0f;
        float cst = std::get<float>(constant->cst);
        if (cst > 1.0f / 16.0f) return false;
        const std::string f = java_float_to_string(cst);
        if (!ends_with_5(f)) return false;
        while (cst < 1 && std::isinf(cst)) {   // (as the original: it never loops, so a float is replaced only when it is already 1)
            divisor *= 2.0f;
            cst *= 2.0f;
        }
        if (cst == 1.0f) {
            InsnList list;
            list.add(list.make<bc::ZeroInsnNode>(op::FCONST_1));
            list.add(InsnHelper::float_insn(list, divisor));
            list.add(list.make<bc::ZeroInsnNode>(op::FDIV));
            source_list.insert(constant, list);
            source_list.remove(constant);
            return true;
        }
        return false;
    }
    return false;
}

// ---- InsnConstant

bool InsnConstant::replace(InsnList& source_list, bc::LdcInsnNode* constant) { return replace(source_list, static_cast<InsnNode*>(constant)); }

bool InsnConstant::replace(InsnList& source_list, InsnNode* constant) {
    bc::Constant const_value;
    if (constant->get_opcode() == op::LDC) {
        const_value = static_cast<bc::LdcInsnNode*>(constant)->cst;
    } else if (constant->get_opcode() == op::SIPUSH || constant->get_opcode() == op::BIPUSH) {
        const_value = static_cast<std::int32_t>(static_cast<bc::IntInsnNode*>(constant)->operand);
    } else {
        return false;
    }
    if (const_value.index() == value_.index()) {   // type.isInstance(constValue)
        if (box_equals(const_value, value_)) {
            InsnList list = InsnHelper::clone(insn_list_);
            replace(source_list, constant, list);
            return true;
        }
        const std::optional<bc::Constant> negative = get_negative();
        if (negative && box_equals(const_value, *negative)) {
            InsnList insert = InsnHelper::clone(insn_list_);
            insert.add(get_negate_instruction(insert));
            replace(source_list, constant, insert);
            return true;
        }
    }
    return false;
}

void InsnConstant::replace(InsnList& source_list, InsnNode* constant, InsnList& replace_list) {
    // (the original sets a list of one instruction in place; the result is the same list as inserting it and removing the constant)
    source_list.insert(constant, replace_list);
    source_list.remove(constant);
}

std::optional<bc::Constant> InsnConstant::get_negative() const {
    if (std::holds_alternative<std::int32_t>(value_)) return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(std::get<std::int32_t>(value_)));
    if (std::holds_alternative<std::int64_t>(value_)) return static_cast<std::int64_t>(0ull - static_cast<std::uint64_t>(std::get<std::int64_t>(value_)));
    if (std::holds_alternative<float>(value_)) return -std::get<float>(value_);
    if (std::holds_alternative<double>(value_)) return -std::get<double>(value_);
    return std::nullopt;
}

InsnNode* InsnConstant::get_negate_instruction(InsnList& owner) const {
    if (std::holds_alternative<std::int32_t>(value_)) return owner.make<bc::ZeroInsnNode>(op::INEG);
    if (std::holds_alternative<std::int64_t>(value_)) return owner.make<bc::ZeroInsnNode>(op::LNEG);
    if (std::holds_alternative<float>(value_)) return owner.make<bc::ZeroInsnNode>(op::FNEG);
    if (std::holds_alternative<double>(value_)) return owner.make<bc::ZeroInsnNode>(op::DNEG);
    return nullptr;
}

// ---- ConstantBuilder

ConstantBuilder& ConstantBuilder::set(std::int32_t value) {
    type = INT;
    int_value = value;
    insns_.add(InsnHelper::int_insn(insns_, value));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(std::int64_t value) {
    type = LONG;
    long_value = value;
    insns_.add(InsnHelper::long_insn(insns_, value));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(float value) {
    type = FLOAT;
    float_value = value;
    insns_.add(InsnHelper::float_insn(insns_, value));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(double value) {
    type = DOUBLE;
    double_value = value;
    insns_.add(InsnHelper::double_insn(insns_, value));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, std::int8_t value) {
    type = INT;
    int_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "B"));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, std::int16_t value) {
    type = INT;
    int_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "S"));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, std::int32_t value) {
    type = INT;
    int_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "I"));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, std::int64_t value) {
    type = LONG;
    long_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "J"));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, float value) {
    type = FLOAT;
    float_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "F"));
    return *this;
}
ConstantBuilder& ConstantBuilder::set(const std::string& owner, const std::string& name, double value) {
    type = DOUBLE;
    double_value = value;
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, "D"));
    return *this;
}

template <class T>
ConstantBuilder& ConstantBuilder::apply(Op op_kind, T value) {
    check_type(builder_type<T>());
    if constexpr (std::is_same_v<T, std::int32_t>) int_value = wrap_op(op_kind, int_value, value);
    else if constexpr (std::is_same_v<T, std::int64_t>) long_value = wrap_op(op_kind, long_value, value);
    else if constexpr (std::is_same_v<T, float>) float_value = wrap_op(op_kind, float_value, value);
    else double_value = wrap_op(op_kind, double_value, value);
    insns_.add(value_insn<T>(insns_, value));
    insns_.add(insns_.make<bc::ZeroInsnNode>(arith_opcode(op_kind, builder_type<T>())));
    return *this;
}

template <class T>
ConstantBuilder& ConstantBuilder::apply_field(Op op_kind, const std::string& owner, const std::string& name, T value) {
    check_type(builder_type<T>());
    if constexpr (std::is_same_v<T, std::int32_t>) int_value = wrap_op(op_kind, int_value, value);
    else if constexpr (std::is_same_v<T, std::int64_t>) long_value = wrap_op(op_kind, long_value, value);
    else if constexpr (std::is_same_v<T, float>) float_value = wrap_op(op_kind, float_value, value);
    else double_value = wrap_op(op_kind, double_value, value);
    insns_.add(insns_.make<bc::FieldInsnNode>(op::GETSTATIC, owner, name, field_desc<T>()));
    insns_.add(insns_.make<bc::ZeroInsnNode>(arith_opcode(op_kind, builder_type<T>())));
    return *this;
}

ConstantBuilder& ConstantBuilder::apply_builder(Op op_kind, const ConstantBuilder& builder) {
    check_type(builder.type);
    switch (type) {
    case INT: int_value = wrap_op(op_kind, int_value, builder.int_value); break;
    case LONG: long_value = wrap_op(op_kind, long_value, builder.long_value); break;
    case FLOAT: float_value = wrap_op(op_kind, float_value, builder.float_value); break;
    case DOUBLE: double_value = wrap_op(op_kind, double_value, builder.double_value); break;
    default: return *this;
    }
    InsnList other = InsnHelper::clone(builder.insns_);
    insns_.add(other);
    insns_.add(insns_.make<bc::ZeroInsnNode>(arith_opcode(op_kind, type)));
    return *this;
}

#define CB_ARITH(name, OP)                                                                                                                   \
    ConstantBuilder& ConstantBuilder::name(std::int32_t v) { return apply<std::int32_t>(OP, v); }                                           \
    ConstantBuilder& ConstantBuilder::name(std::int64_t v) { return apply<std::int64_t>(OP, v); }                                           \
    ConstantBuilder& ConstantBuilder::name(float v) { return apply<float>(OP, v); }                                                         \
    ConstantBuilder& ConstantBuilder::name(double v) { return apply<double>(OP, v); }                                                       \
    ConstantBuilder& ConstantBuilder::name(const std::string& o, const std::string& n, std::int32_t v) { return apply_field<std::int32_t>(OP, o, n, v); } \
    ConstantBuilder& ConstantBuilder::name(const std::string& o, const std::string& n, std::int64_t v) { return apply_field<std::int64_t>(OP, o, n, v); } \
    ConstantBuilder& ConstantBuilder::name(const std::string& o, const std::string& n, float v) { return apply_field<float>(OP, o, n, v); }  \
    ConstantBuilder& ConstantBuilder::name(const std::string& o, const std::string& n, double v) { return apply_field<double>(OP, o, n, v); } \
    ConstantBuilder& ConstantBuilder::name(const ConstantBuilder& b) { return apply_builder(OP, b); }

CB_ARITH(multiply, MUL)
CB_ARITH(divide, DIV)
CB_ARITH(add, ADD)
CB_ARITH(subtract, SUB)
#undef CB_ARITH

ConstantBuilder& ConstantBuilder::to_int() {
    insns_.add(cast(INT));
    type = INT;
    return *this;
}
ConstantBuilder& ConstantBuilder::to_long() {
    insns_.add(cast(LONG));
    type = LONG;
    return *this;
}
ConstantBuilder& ConstantBuilder::to_float() {
    insns_.add(cast(FLOAT));
    type = FLOAT;
    return *this;
}
ConstantBuilder& ConstantBuilder::to_double() {
    insns_.add(cast(DOUBLE));
    type = DOUBLE;
    return *this;
}

std::unique_ptr<Constant> ConstantBuilder::build() const {
    switch (type) {
    case INT: return std::make_unique<InsnConstant>(InsnHelper::clone(insns_), bc::Constant(int_value));
    case LONG: return std::make_unique<InsnConstant>(InsnHelper::clone(insns_), bc::Constant(long_value));
    case FLOAT: return std::make_unique<InsnConstant>(InsnHelper::clone(insns_), bc::Constant(float_value));
    case DOUBLE: return std::make_unique<InsnConstant>(InsnHelper::clone(insns_), bc::Constant(double_value));
    default: return nullptr;
    }
}

ConstantBuilder ConstantBuilder::copy() const {
    ConstantBuilder builder;
    builder.type = type;
    builder.int_value = int_value;
    builder.long_value = long_value;
    builder.float_value = float_value;
    builder.double_value = double_value;
    builder.insns_ = InsnHelper::clone(insns_);
    return builder;
}

InsnNode* ConstantBuilder::cast(int target_type) {
    if (type == INT && target_type == LONG) {
        long_value = int_value;
        return insns_.make<bc::ZeroInsnNode>(op::I2L);
    }
    if (type == INT && target_type == FLOAT) {
        float_value = static_cast<float>(int_value);
        return insns_.make<bc::ZeroInsnNode>(op::L2F);   // (as the original: l2f, not i2f)
    }
    if (type == INT && target_type == DOUBLE) {
        double_value = static_cast<double>(int_value);
        return insns_.make<bc::ZeroInsnNode>(op::I2D);
    }
    if (type == LONG && target_type == INT) {
        int_value = static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(long_value)));
        return insns_.make<bc::ZeroInsnNode>(op::L2I);
    }
    if (type == LONG && target_type == FLOAT) {
        float_value = static_cast<float>(long_value);
        return insns_.make<bc::ZeroInsnNode>(op::L2F);
    }
    if (type == LONG && target_type == DOUBLE) {
        double_value = static_cast<double>(long_value);
        return insns_.make<bc::ZeroInsnNode>(op::L2D);
    }
    if (type == FLOAT && target_type == INT) {
        int_value = d2i(static_cast<double>(float_value));
        return insns_.make<bc::ZeroInsnNode>(op::F2I);
    }
    if (type == FLOAT && target_type == LONG) {
        long_value = d2l(static_cast<double>(float_value));
        return insns_.make<bc::ZeroInsnNode>(op::F2L);
    }
    if (type == FLOAT && target_type == DOUBLE) {
        double_value = static_cast<double>(float_value);
        return insns_.make<bc::ZeroInsnNode>(op::F2D);
    }
    if (type == DOUBLE && target_type == INT) {
        int_value = d2i(double_value);
        return insns_.make<bc::ZeroInsnNode>(op::D2I);
    }
    if (type == DOUBLE && target_type == LONG) {
        long_value = d2l(double_value);
        return insns_.make<bc::ZeroInsnNode>(op::D2L);
    }
    if (type == DOUBLE && target_type == FLOAT) {
        float_value = static_cast<float>(double_value);
        return insns_.make<bc::ZeroInsnNode>(op::D2F);
    }
    throw std::runtime_error("Type cannot be casted");
}

void ConstantBuilder::check_type(int required_type) const {
    if (required_type != type) throw std::runtime_error("Cast required!");
}

// ---- PiConstants

PiConstants::PiConstants() {
    const double pi = 3.141592653589793;   // Math.PI
    pi_.set("java/lang/Math", "PI", pi);                                                // Math.PI
    pi_float_.set("java/lang/Math", "PI", pi).to_float();                               // (float)Math.PI
    pi_float_double_.set("java/lang/Math", "PI", pi).to_float().to_double();            // (double)(float)Math.PI
    add(pi_);
    add(pi_float_);
    add(pi_float_double_);
}

void PiConstants::add(const ConstantBuilder& constant) {
    if (constant.type == ConstantBuilder::DOUBLE) {
        pi_doubles.push_back(constant.build());
        pi_doubles.push_back(constant.copy().divide(180.0).build());
        pi_doubles.push_back(constant.copy().multiply(1.85).build());   // TODO in the original: dividing by PI loses precision sometimes
        pi_doubles.push_back(constant.copy().multiply(0.185).build());
        pi_doubles.push_back(constant.copy().multiply(3.0).divide(2.0).build());
        pi_doubles.push_back(constant.copy().multiply(2.0).divide(9.0).build());
        pi_doubles.push_back(constant.copy().multiply(4.0).divide(3.0).build());
        pi_doubles.push_back(ConstantPool::constant().set(180.0).divide(constant).build());
    } else if (constant.type == ConstantBuilder::FLOAT) {
        pi_floats.push_back(constant.build());
        pi_floats.push_back(constant.copy().divide(180.0f).build());
        pi_floats.push_back(constant.copy().multiply(1.85f).build());   // TODO in the original: dividing by PI loses precision sometimes
        pi_floats.push_back(constant.copy().multiply(0.185f).build());
        pi_floats.push_back(constant.copy().multiply(3.0f).divide(2.0f).build());
        pi_floats.push_back(constant.copy().multiply(2.0f).divide(9.0f).build());
        pi_floats.push_back(constant.copy().multiply(4.0f).divide(3.0f).build());
        pi_floats.push_back(ConstantPool::constant().set(180.0f).divide(constant).build());
    }
}

bool PiConstants::replace(InsnList& source_list, bc::LdcInsnNode* constant) {
    if (std::holds_alternative<double>(constant->cst)) {
        for (const auto& constant_replacer : pi_doubles) {
            if (constant_replacer->replace(source_list, constant)) return true;
        }
        const double const_d = std::get<double>(constant->cst);
        std::string d = java_double_to_string(const_d);
        if (length_of(d) - index_of_dot(d) > 5) {
            for (const ConstantBuilder* pi_const : {&pi_, &pi_float_double_}) {
                const double cst_double = const_d / pi_const->double_value;
                d = java_double_to_string(cst_double);
                if (length_of(d) - index_of_dot(d) - 1 <= 3) {
                    ConstantBuilder pi_mul = pi_const->copy();
                    if (cst_double != 1.0) pi_mul.multiply(cst_double);
                    const std::unique_ptr<Constant> pi_constant = pi_mul.build();
                    if (pi_constant->replace(source_list, constant)) return true;
                }
            }
        }
    } else if (std::holds_alternative<float>(constant->cst)) {
        for (const auto& constant_replacer : pi_floats) {
            if (constant_replacer->replace(source_list, constant)) return true;
        }
        const float const_f = std::get<float>(constant->cst);
        std::string f = java_float_to_string(const_f);
        if (length_of(f) - index_of_dot(f) > 5) {
            const float cst_float = const_f / pi_float_.float_value;
            f = java_float_to_string(cst_float);
            if (length_of(f) - index_of_dot(f) - 1 <= 3) {
                ConstantBuilder pi_mul = pi_float_.copy();
                if (cst_float != 1.0f) pi_mul.multiply(cst_float);
                const std::unique_ptr<Constant> pi_constant = pi_mul.build();
                if (pi_constant->replace(source_list, constant)) return true;
            }
        }
    }
    return false;
}

// ---- ConstantPool

const std::vector<std::unique_ptr<Constant>>& ConstantPool::constants() {
    static const std::vector<std::unique_ptr<Constant>> pool = [] {
        std::vector<std::unique_ptr<Constant>> list;
        const auto add = [&list](ConstantBuilder& b) { list.push_back(b.build()); };
        const auto constant = [] { return ConstantPool::constant(); };
        using lim_f = std::numeric_limits<float>;
        using lim_d = std::numeric_limits<double>;
        const double e = 2.718281828459045;   // Math.E
        constexpr std::int32_t i32_max = std::numeric_limits<std::int32_t>::max(), i32_min = std::numeric_limits<std::int32_t>::min();
        constexpr std::int64_t i64_max = std::numeric_limits<std::int64_t>::max(), i64_min = std::numeric_limits<std::int64_t>::min();
        const auto sh = [](int v) { return static_cast<std::int16_t>(v); };
        // (each `constant()` below is a builder of its own: the original's is one builder cleared at each call)
        { auto b = constant(); add(b.set("java/lang/Short", "MAX_VALUE", sh(32767))); }
        { auto b = constant(); add(b.set("java/lang/Short", "MIN_VALUE", sh(-32768))); }
        { auto b = constant(); add(b.set("java/lang/Integer", "MAX_VALUE", i32_max)); }
        { auto b = constant(); add(b.set("java/lang/Integer", "MIN_VALUE", i32_min)); }
        { auto b = constant(); add(b.set("java/lang/Long", "MAX_VALUE", i64_max)); }
        { auto b = constant(); add(b.set("java/lang/Long", "MIN_VALUE", i64_min)); }
        { auto b = constant(); add(b.set("java/lang/Float", "MAX_VALUE", lim_f::max())); }
        { auto b = constant(); add(b.set("java/lang/Float", "MIN_VALUE", lim_f::denorm_min())); }
        { auto b = constant(); add(b.set("java/lang/Float", "NaN", lim_f::quiet_NaN())); }
        { auto b = constant(); add(b.set("java/lang/Float", "POSITIVE_INFINITY", lim_f::infinity())); }
        { auto b = constant(); add(b.set("java/lang/Float", "NEGATIVE_INFINITY", -lim_f::infinity())); }
        { auto b = constant(); add(b.set("java/lang/Double", "MAX_VALUE", lim_d::max())); }
        { auto b = constant(); add(b.set("java/lang/Double", "MIN_VALUE", lim_d::denorm_min())); }
        { auto b = constant(); add(b.set("java/lang/Double", "NaN", lim_d::quiet_NaN())); }
        { auto b = constant(); add(b.set("java/lang/Double", "POSITIVE_INFINITY", lim_d::infinity())); }
        { auto b = constant(); add(b.set("java/lang/Double", "NEGATIVE_INFINITY", -lim_d::infinity())); }
        { auto b = constant(); add(b.set("java/lang/Math", "E", e)); }
        { auto b = constant(); add(b.set("java/lang/Math", "E", e).to_float()); }
        { auto b = constant(); add(b.set("java/lang/Math", "E", e).to_float().to_double()); }
        { auto b = constant(); add(b.set("java/lang/Integer", "MAX_VALUE", i32_max).to_double()); }

        for (const float n : {1.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f}) {
            auto b = constant();
            add(b.set(n).divide(16.0f));
        }
        { auto b = constant(); add(b.set(0.1f).divide(16.0f)); }
        { auto b = constant(); add(b.set(9.0f).divide(32.0f)); }
        { auto b = constant(); add(b.set(25.0f).divide(32.0f)); }

        { auto b = constant(); add(b.set(0.999f).divide(4.0f)); }
        { auto b = constant(); add(b.set(0.999f).divide(16.0f)); }
        { auto b = constant(); add(b.set(0.999f).divide(64.0f)); }
        // TODO in the original: custom behavior for color constants
        { auto b = constant(); add(b.set(216.0f).divide(255.0f)); }
        { auto b = constant(); add(b.set(192.0f).divide(255.0f)); }
        { auto b = constant(); add(b.set(224.0f).divide(360.0f)); }

        { auto b = constant(); add(b.set(0.1f).multiply(0.1f).multiply(100.0f)); }
        { auto b = constant(); add(b.set(0.1f).multiply(0.1f).multiply(106.0f)); }
        { auto b = constant(); add(b.set(0.1f).multiply(0.1f).multiply(58.8f)); }
        { auto b = constant(); add(b.set(546.0f).multiply(0.1f).multiply(0.1f).multiply(0.1f)); }
        { auto b = constant(); add(b.set(11.0f).divide(32.0f)); }
        { auto b = constant(); add(b.set(0.5f).divide(1024.0f)); }
        { auto b = constant(); add(b.set(1.0f).divide(1024.0f)); }
        { auto b = constant(); add(b.set(6.0).divide(256.0)); }
        { auto b = constant(); add(b.set(9.0).divide(256.0)); }
        { auto b = constant(); add(b.set(1.0).divide(3.0)); }
        { auto b = constant(); add(b.set(2.0).divide(3.0)); }
        { auto b = constant(); add(b.set(4.0).divide(3.0)); }
        { auto b = constant(); add(b.set(1.0f).divide(3.0f)); }
        { auto b = constant(); add(b.set(2.0f).divide(3.0f)); }
        { auto b = constant(); add(b.set(4.0f).divide(3.0f)); }
        { auto b = constant(); add(b.set(1.0f).divide(9.0f)); }
        { auto b = constant(); add(b.set(7.0f).divide(320.0f)); }
        { auto b = constant(); add(b.set(385.0f).divide(512.0f)); }
        { auto b = constant(); add(b.set(10.0f).multiply(0.01f)); }
        { auto b = constant(); add(b.set(0.1).multiply(0.1)); }
        { auto b = constant(); add(b.set(1.25).divide(64.0)); }
        { auto b = constant(); add(b.set(1.75).divide(64.0)); }
        { auto b = constant(); add(b.set(2.0f).divide(3.0f).multiply(0.01f)); }
        { auto b = constant(); add(b.set(1.0).divide(60.0).to_float()); }
        list.push_back(std::make_unique<PiConstants>());
        list.push_back(std::make_unique<FloatCastedToDouble>());
        list.push_back(std::make_unique<OneDivideByPowerOfTwo>());
        return list;
    }();
    return pool;
}

// ---- Transform.replaceCommonConstants

void replace_common_constants(ClassStorage& storage) {
    const auto& constants = ConstantPool::constants();
    for (bc::ClassNode* node : storage) {
        for (bc::MethodNode& method : node->methods) {
            InsnNode* insn = method.instructions.get_first();
            while (insn != nullptr) {
                bool replaced = false;
                if (insn->get_opcode() == op::LDC || insn->get_opcode() == op::SIPUSH) {
                    InsnNode* next = insn->get_next();
                    for (const auto& constant_replacer : constants) {
                        if (constant_replacer->replace(method.instructions, insn)) {
                            insn = next;
                            replaced = true;
                            break;
                        }
                    }
                }
                if (!replaced) insn = insn->get_next();
            }
        }
    }
}

}  // namespace cb::java::minecraft::prep
