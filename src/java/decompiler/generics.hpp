// java/decompiler/generics.hpp -- Java generic signatures: a class's, a field's or a method's Signature attribute read into types with their type
// arguments, wildcards, enclosing classes and type variables, and a type written back as Java source ("Map<String, ? extends List<T>>[]").
//
//     const auto m = generics::parse_method_signature("<T:Ljava/lang/Object;>(TT;)Ljava/util/List<TT;>;");   // fparameters {"T"}, params {T}, ret List<T>
//
// Ported from Fernflower (de.fernflower.struct.gen.generics: GenericMain, GenericType and the three descriptors), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder). A signature that cannot be read is nullopt (the original logs "Invalid signature: ..." and returns
// null; the caller logs here). Writing a class name as source asks the class writer's import collector, given as NameContext.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::decompiler {

class GenericType {
public:
    static constexpr int WILDCARD_EXTENDS = 1, WILDCARD_SUPER = 2, WILDCARD_UNBOUND = 3, WILDCARD_NO = 4;

    int type = 0;                 // a cc::TYPE_*: TYPE_OBJECT, TYPE_GENVAR (a type variable), or a primitive
    int array_dim = 0;
    std::optional<std::string> value;   // the class name, the variable's name, or the primitive's letter
    std::vector<GenericType> enclosing_classes;               // Outer<A>.Inner<B>: Outer<A>
    std::vector<std::shared_ptr<GenericType>> arguments;      // null for an unbounded wildcard ("?")
    std::vector<int> wildcards;                               // one per argument

    GenericType() = default;
    GenericType(int type, int array_dim, std::optional<std::string> value) : type(type), array_dim(array_dim), value(std::move(value)) {}
    // A type from a signature ("Ljava/util/List<TT;>;", "[TT;", "I"). std::out_of_range or std::invalid_argument for one that cannot be read.
    static GenericType parse(std::string signature);
    // The first type at the start of a list of types ("TT;Ljava/lang/String;" -> "TT;").
    static std::string next_type(const std::string& value);

    GenericType decrease_array_dim() const { return GenericType(type, array_dim - 1, value); }
};

struct GenericClassDescriptor {
    GenericType superclass;
    std::vector<GenericType> superinterfaces;
    std::vector<std::string> fparameters;
    std::vector<std::vector<GenericType>> fbounds;
};

struct GenericFieldDescriptor {
    GenericType type;
};

struct GenericMethodDescriptor {
    std::vector<std::string> fparameters;
    std::vector<std::vector<GenericType>> fbounds;
    std::vector<GenericType> params;
    GenericType ret;
    std::vector<GenericType> exceptions;
};

namespace generics {

std::optional<GenericClassDescriptor> parse_class_signature(const std::string& signature);
std::optional<GenericFieldDescriptor> parse_field_signature(const std::string& signature);
std::optional<GenericMethodDescriptor> parse_method_signature(const std::string& signature);

// How a class name is written: the import collector's short name for a dotted name, and whether a class (in internal form, "a/B$C") is one being
// decompiled (an inner class of one keeps its '$' as is; another's becomes '.').
struct NameContext {
    std::function<std::string(const std::string& java_class_name)> short_name;
    std::function<bool(const std::string& internal_name)> is_own_class;
};
// The type as Java source, "[]" for each dimension. std::runtime_error for a type that has no name.
std::string cast_type_name(const GenericType& type, const NameContext& names);

}  // namespace generics

}  // namespace cb::java::decompiler
