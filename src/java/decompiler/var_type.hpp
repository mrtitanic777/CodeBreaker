// java/decompiler/var_type.hpp -- a value's type as the decompiler reasons about it: one of the JVM's types (or one of the decompiler's own: the
// boolean-or-int "byte-char", "short-char", null, unknown ...), its array dimension, and for an object its class name; how types relate (a superset,
// the common subtype and supertype); and the field and method descriptors made of them.
//
//     const VarType t = VarType::parse("[Ljava/lang/String;");    // type TYPE_OBJECT, array_dim 1, value "java/lang/String"
//     const MethodDescriptor m = MethodDescriptor::parse("(IJ)V"); // params {int, long}, ret void
//
// Ported from Fernflower (de.fernflower.struct.gen: VarType, FieldDescriptor, MethodDescriptor, NewClassNameBuilder), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0 (see NOTICE in this folder). A Java `null` type is std::nullopt.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::decompiler {

class VarType {
public:
    int type = 0;
    int array_dim = 0;
    std::optional<std::string> value;   // the class name of an object, the one-letter code of the others; none for null
    int type_family = 0;
    int stack_size = 0;
    bool false_boolean = false;
    // Which shared constant this is (VarType::NULL_TYPE ...), 0 for any other: the original compares one by identity (`== VarType.VARTYPE_NULL`), which
    // a copy of the constant passes and an equal new type does not. A copy() or a new type is not the constant.
    int constant_id = 0;
    bool is_constant(const VarType& constant) const { return constant_id != 0 && constant_id == constant.constant_id; }

    VarType() = default;
    explicit VarType(int type, int array_dim = 0);
    VarType(int type, int array_dim, std::optional<std::string> value);
    // A type from its descriptor or signature ("I", "[J", "Ljava/lang/String;"); `class_type`: a bare name is a class ("java/lang/String").
    static VarType parse(std::string_view signature, bool class_type = false);

    VarType decrease_array_dim() const;
    VarType resize_array_dim(int new_array_dim) const;
    VarType copy(bool force_false_boolean = false) const;
    bool is_false_boolean() const { return false_boolean; }
    bool is_superset(const VarType& val) const;
    bool is_strict_superset(const VarType& val) const;

    // Equal: the same type, dimension and class (the false-boolean mark is not part of it).
    friend bool operator==(const VarType& a, const VarType& b) { return a.type == b.type && a.array_dim == b.array_dim && a.value == b.value; }
    // The descriptor form ("[Ljava/lang/String;", "I"); a null type's value is written as "null", as Java's string building does.
    std::string to_string() const;

    // The common subtype and supertype of two types (nullopt: none).
    static std::optional<VarType> common_min_type(const VarType& type1, const VarType& type2);
    static std::optional<VarType> common_supertype(const VarType& type1, const VarType& type2);
    static VarType min_type_in_family(int family);
    // The type of a descriptor letter; std::invalid_argument for one that is none.
    static int type_of(char c);

    // The shared constants.
    static const VarType UNKNOWN, INT, FLOAT, LONG, DOUBLE, BYTE, CHAR, SHORT, BOOLEAN, BYTECHAR, SHORTCHAR, NULL_TYPE, STRING, CLASS, OBJECT, VOID;
};

// A new name for a class, or nullopt to keep it (the renamer's).
using NewClassNameBuilder = std::function<std::optional<std::string>(const std::string& class_name)>;

class FieldDescriptor {
public:
    VarType type;
    std::string descriptor_string;

    static FieldDescriptor parse(std::string_view descriptor);
    std::optional<std::string> build_new_descriptor(const NewClassNameBuilder& builder) const;
    friend bool operator==(const FieldDescriptor& a, const FieldDescriptor& b) { return a.type == b.type; }

    static const FieldDescriptor INTEGER_DESCRIPTOR, LONG_DESCRIPTOR, FLOAT_DESCRIPTOR, DOUBLE_DESCRIPTOR;
};

class MethodDescriptor {
public:
    std::vector<VarType> params;
    VarType ret;

    // std::invalid_argument for a string that is not a method descriptor.
    static MethodDescriptor parse(std::string_view descriptor);
    std::optional<std::string> build_new_descriptor(const NewClassNameBuilder& builder) const;
    friend bool operator==(const MethodDescriptor& a, const MethodDescriptor& b) { return a.ret == b.ret && a.params == b.params; }
};

}  // namespace cb::java::decompiler
