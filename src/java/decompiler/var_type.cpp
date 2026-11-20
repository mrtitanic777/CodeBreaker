// java/decompiler/var_type.cpp -- see var_type.hpp. Ported from Fernflower (de.fernflower.struct.gen), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0.
#include "java/decompiler/var_type.hpp"

#include <stdexcept>
#include <string>

#include "java/decompiler/code_constants.hpp"

namespace cb::java::decompiler {

namespace {

std::optional<std::string> vt_char(int type) {
    switch (type) {
        case cc::TYPE_BYTE: return "B";
        case cc::TYPE_CHAR: return "C";
        case cc::TYPE_DOUBLE: return "D";
        case cc::TYPE_FLOAT: return "F";
        case cc::TYPE_INT: return "I";
        case cc::TYPE_LONG: return "J";
        case cc::TYPE_SHORT: return "S";
        case cc::TYPE_BOOLEAN: return "Z";
        case cc::TYPE_VOID: return "V";
        case cc::TYPE_GROUP2EMPTY: return "G";
        case cc::TYPE_NOTINITIALIZED: return "N";
        case cc::TYPE_ADDRESS: return "A";
        case cc::TYPE_BYTECHAR: return "X";
        case cc::TYPE_SHORTCHAR: return "Y";
        case cc::TYPE_UNKNOWN: return "U";
        case cc::TYPE_NULL:
        case cc::TYPE_OBJECT: return std::nullopt;
        default: throw std::runtime_error("Invalid type");
    }
}

int vt_stack_size(int type, int array_dim) {
    if (array_dim > 0) return 1;
    switch (type) {
        case cc::TYPE_DOUBLE:
        case cc::TYPE_LONG: return 2;
        case cc::TYPE_VOID:
        case cc::TYPE_GROUP2EMPTY: return 0;
        default: return 1;
    }
}

int vt_family(int type, int array_dim) {
    if (array_dim > 0) return cc::TYPE_FAMILY_OBJECT;
    switch (type) {
        case cc::TYPE_BYTE:
        case cc::TYPE_BYTECHAR:
        case cc::TYPE_SHORTCHAR:
        case cc::TYPE_CHAR:
        case cc::TYPE_SHORT:
        case cc::TYPE_INT: return cc::TYPE_FAMILY_INTEGER;
        case cc::TYPE_DOUBLE: return cc::TYPE_FAMILY_DOUBLE;
        case cc::TYPE_FLOAT: return cc::TYPE_FAMILY_FLOAT;
        case cc::TYPE_LONG: return cc::TYPE_FAMILY_LONG;
        case cc::TYPE_BOOLEAN: return cc::TYPE_FAMILY_BOOLEAN;
        case cc::TYPE_NULL:
        case cc::TYPE_OBJECT: return cc::TYPE_FAMILY_OBJECT;
        default: return cc::TYPE_FAMILY_UNKNOWN;
    }
}

VarType vt_constant(VarType t, int id) {
    t.constant_id = id;
    return t;
}

}  // namespace

VarType::VarType(int type_, int array_dim_) : VarType(type_, array_dim_, vt_char(type_)) {}

VarType::VarType(int type_, int array_dim_, std::optional<std::string> value_)
    : type(type_), array_dim(array_dim_), value(std::move(value_)), type_family(vt_family(type_, array_dim_)), stack_size(vt_stack_size(type_, array_dim_)) {}

VarType VarType::parse(std::string_view signature, bool class_type) {
    VarType t;
    for (std::size_t i = 0; i < signature.size(); ++i) {
        const char c = signature[i];
        if (c == '[') {
            ++t.array_dim;
            continue;
        }
        if (c == 'L' && signature.back() == ';') {
            t.type = cc::TYPE_OBJECT;
            t.value = std::string(signature.substr(i + 1, signature.size() - i - 2));
            break;
        }
        t.value = std::string(signature.substr(i));
        if ((class_type && i == 0) || t.value->size() > 1) t.type = cc::TYPE_OBJECT;
        else t.type = type_of((*t.value)[0]);
        break;
    }
    t.type_family = vt_family(t.type, t.array_dim);
    t.stack_size = vt_stack_size(t.type, t.array_dim);
    return t;
}

VarType VarType::decrease_array_dim() const {
    if (array_dim > 0) return VarType(type, array_dim - 1, value);
    return *this;
}

VarType VarType::resize_array_dim(int new_array_dim) const {
    VarType t = *this;
    t.constant_id = 0;
    t.array_dim = new_array_dim;   // (the family and stack size are kept, as the original does)
    return t;
}

VarType VarType::copy(bool force_false_boolean) const {
    VarType t = *this;
    t.constant_id = 0;   // (a new object)
    t.false_boolean = false_boolean || force_false_boolean;
    return t;
}

bool VarType::is_superset(const VarType& val) const { return *this == val || is_strict_superset(val); }

bool VarType::is_strict_superset(const VarType& val) const {
    const int val_type = val.type;
    if (val_type == cc::TYPE_UNKNOWN && type != cc::TYPE_UNKNOWN) return true;
    if (val.array_dim > 0) return *this == OBJECT;
    if (array_dim > 0) return val_type == cc::TYPE_NULL;
    bool res = false;
    switch (type) {   // (the cases fall through, as in the original)
        case cc::TYPE_INT:
            res = val_type == cc::TYPE_SHORT || val_type == cc::TYPE_CHAR;
            [[fallthrough]];
        case cc::TYPE_SHORT:
            res |= val_type == cc::TYPE_BYTE;
            [[fallthrough]];
        case cc::TYPE_CHAR:
            res |= val_type == cc::TYPE_SHORTCHAR;
            [[fallthrough]];
        case cc::TYPE_BYTE:
        case cc::TYPE_SHORTCHAR:
            res |= val_type == cc::TYPE_BYTECHAR;
            [[fallthrough]];
        case cc::TYPE_BYTECHAR:
            res |= val_type == cc::TYPE_BOOLEAN;
            break;
        case cc::TYPE_OBJECT:
            if (val_type == cc::TYPE_NULL) return true;
            if (*this == OBJECT) return val_type == cc::TYPE_OBJECT && !(val == OBJECT);
            break;
        default: break;
    }
    return res;
}

std::string VarType::to_string() const {
    std::string res(static_cast<std::size_t>(std::max(0, array_dim)), '[');
    if (type == cc::TYPE_OBJECT) res += 'L' + value.value_or("null") + ';';
    else res += value.value_or("null");
    return res;
}

std::optional<VarType> VarType::common_min_type(const VarType& type1, const VarType& type2) {
    if (type1.type == cc::TYPE_BOOLEAN && type2.type == cc::TYPE_BOOLEAN) return type1.is_false_boolean() ? type2 : type1;
    if (type1.is_superset(type2)) return type2;
    if (type2.is_superset(type1)) return type1;
    if (type1.type_family == type2.type_family) {
        switch (type1.type_family) {
            case cc::TYPE_FAMILY_INTEGER:
                if ((type1.type == cc::TYPE_CHAR && type2.type == cc::TYPE_SHORT) || (type1.type == cc::TYPE_SHORT && type2.type == cc::TYPE_CHAR)) return SHORTCHAR;
                return BYTECHAR;
            case cc::TYPE_FAMILY_OBJECT: return NULL_TYPE;
            default: break;
        }
    }
    return std::nullopt;
}

std::optional<VarType> VarType::common_supertype(const VarType& type1, const VarType& type2) {
    if (type1.type == cc::TYPE_BOOLEAN && type2.type == cc::TYPE_BOOLEAN) return type1.is_false_boolean() ? type1 : type2;
    if (type1.is_superset(type2)) return type1;
    if (type2.is_superset(type1)) return type2;
    if (type1.type_family == type2.type_family) {
        switch (type1.type_family) {
            case cc::TYPE_FAMILY_INTEGER:
                if ((type1.type == cc::TYPE_SHORTCHAR && type2.type == cc::TYPE_BYTE) || (type1.type == cc::TYPE_BYTE && type2.type == cc::TYPE_SHORTCHAR)) return SHORT;
                return INT;
            case cc::TYPE_FAMILY_OBJECT: return OBJECT;
            default: break;
        }
    }
    return std::nullopt;
}

VarType VarType::min_type_in_family(int family) {
    switch (family) {
        case cc::TYPE_FAMILY_BOOLEAN: return BOOLEAN;
        case cc::TYPE_FAMILY_INTEGER: return BYTECHAR;
        case cc::TYPE_FAMILY_OBJECT: return NULL_TYPE;
        case cc::TYPE_FAMILY_FLOAT: return FLOAT;
        case cc::TYPE_FAMILY_LONG: return LONG;
        case cc::TYPE_FAMILY_DOUBLE: return DOUBLE;
        case cc::TYPE_FAMILY_UNKNOWN: return UNKNOWN;
        default: throw std::invalid_argument("Invalid type family: " + std::to_string(family));
    }
}

int VarType::type_of(char c) {
    switch (c) {
        case 'B': return cc::TYPE_BYTE;
        case 'C': return cc::TYPE_CHAR;
        case 'D': return cc::TYPE_DOUBLE;
        case 'F': return cc::TYPE_FLOAT;
        case 'I': return cc::TYPE_INT;
        case 'J': return cc::TYPE_LONG;
        case 'S': return cc::TYPE_SHORT;
        case 'Z': return cc::TYPE_BOOLEAN;
        case 'V': return cc::TYPE_VOID;
        case 'G': return cc::TYPE_GROUP2EMPTY;
        case 'N': return cc::TYPE_NOTINITIALIZED;
        case 'A': return cc::TYPE_ADDRESS;
        case 'X': return cc::TYPE_BYTECHAR;
        case 'Y': return cc::TYPE_SHORTCHAR;
        case 'U': return cc::TYPE_UNKNOWN;
        default: throw std::invalid_argument(std::string("Invalid type: ") + c);
    }
}

const VarType VarType::UNKNOWN = vt_constant(VarType(cc::TYPE_UNKNOWN), 1);
const VarType VarType::INT = vt_constant(VarType(cc::TYPE_INT), 2);
const VarType VarType::FLOAT = vt_constant(VarType(cc::TYPE_FLOAT), 3);
const VarType VarType::LONG = vt_constant(VarType(cc::TYPE_LONG), 4);
const VarType VarType::DOUBLE = vt_constant(VarType(cc::TYPE_DOUBLE), 5);
const VarType VarType::BYTE = vt_constant(VarType(cc::TYPE_BYTE), 6);
const VarType VarType::CHAR = vt_constant(VarType(cc::TYPE_CHAR), 7);
const VarType VarType::SHORT = vt_constant(VarType(cc::TYPE_SHORT), 8);
const VarType VarType::BOOLEAN = vt_constant(VarType(cc::TYPE_BOOLEAN), 9);
const VarType VarType::BYTECHAR = vt_constant(VarType(cc::TYPE_BYTECHAR), 10);
const VarType VarType::SHORTCHAR = vt_constant(VarType(cc::TYPE_SHORTCHAR), 11);
const VarType VarType::NULL_TYPE = vt_constant(VarType(cc::TYPE_NULL, 0, std::nullopt), 12);
const VarType VarType::STRING = vt_constant(VarType(cc::TYPE_OBJECT, 0, "java/lang/String"), 13);
const VarType VarType::CLASS = vt_constant(VarType(cc::TYPE_OBJECT, 0, "java/lang/Class"), 14);
const VarType VarType::OBJECT = vt_constant(VarType(cc::TYPE_OBJECT, 0, "java/lang/Object"), 15);
const VarType VarType::VOID = vt_constant(VarType(cc::TYPE_VOID), 16);

// ---- descriptors

FieldDescriptor FieldDescriptor::parse(std::string_view descriptor) { return FieldDescriptor{VarType::parse(descriptor), std::string(descriptor)}; }

std::optional<std::string> FieldDescriptor::build_new_descriptor(const NewClassNameBuilder& builder) const {
    if (type.type == cc::TYPE_OBJECT)
        if (const std::optional<std::string> name = builder(type.value.value_or(""))) return VarType(type.type, type.array_dim, *name).to_string();
    return std::nullopt;
}

const FieldDescriptor FieldDescriptor::INTEGER_DESCRIPTOR = FieldDescriptor::parse("Ljava/lang/Integer;");
const FieldDescriptor FieldDescriptor::LONG_DESCRIPTOR = FieldDescriptor::parse("Ljava/lang/Long;");
const FieldDescriptor FieldDescriptor::FLOAT_DESCRIPTOR = FieldDescriptor::parse("Ljava/lang/Float;");
const FieldDescriptor FieldDescriptor::DOUBLE_DESCRIPTOR = FieldDescriptor::parse("Ljava/lang/Double;");

MethodDescriptor MethodDescriptor::parse(std::string_view descriptor) {
    const std::size_t parenth = descriptor.rfind(')');
    if (descriptor.size() < 2 || parenth == std::string_view::npos || descriptor[0] != '(')
        throw std::invalid_argument("Invalid descriptor: " + std::string(descriptor));
    MethodDescriptor md;
    if (parenth > 1) {
        const std::string_view parameters = descriptor.substr(1, parenth - 1);
        std::ptrdiff_t index_from = -1;
        for (std::size_t index = 0; index < parameters.size(); ++index) {
            switch (parameters[index]) {
                case '[':
                    if (index_from < 0) index_from = static_cast<std::ptrdiff_t>(index);
                    break;
                case 'L': {
                    const std::size_t ind = parameters.find(';', index);
                    const std::size_t from = index_from < 0 ? index : static_cast<std::size_t>(index_from);
                    md.params.push_back(VarType::parse(parameters.substr(from, ind + 1 - from)));
                    index = ind;
                    index_from = -1;
                    break;
                }
                default: {
                    const std::size_t from = index_from < 0 ? index : static_cast<std::size_t>(index_from);
                    md.params.push_back(VarType::parse(parameters.substr(from, index + 1 - from)));
                    index_from = -1;
                }
            }
        }
    }
    md.ret = VarType::parse(descriptor.substr(parenth + 1));
    return md;
}

std::optional<std::string> MethodDescriptor::build_new_descriptor(const NewClassNameBuilder& builder) const {
    const auto new_type = [&](const VarType& t) -> std::optional<VarType> {
        if (t.type == cc::TYPE_OBJECT)
            if (const std::optional<std::string> name = builder(t.value.value_or(""))) return VarType(t.type, t.array_dim, *name);
        return std::nullopt;
    };
    bool updated = false;
    std::vector<VarType> new_params = params;
    for (VarType& p : new_params)
        if (const std::optional<VarType> s = new_type(p)) {
            p = *s;
            updated = true;
        }
    VarType new_ret = ret;
    if (const std::optional<VarType> s = new_type(ret)) {
        new_ret = *s;
        updated = true;
    }
    if (!updated) return std::nullopt;
    std::string res = "(";
    for (const VarType& p : new_params) res += p.to_string();
    return res + ")" + new_ret.to_string();
}

}  // namespace cb::java::decompiler
