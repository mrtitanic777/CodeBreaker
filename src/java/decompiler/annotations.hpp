// java/decompiler/annotations.hpp -- the annotation attributes of a class, field or method read into expressions: an annotation is an AnnotationExprent
// (its class, and each element's name and value), an element value a constant, an enum constant (a static field), a class (X.class), an array
// ({a, b}) or a nested annotation.
//
//     const auto* a = method.attribute<AnnotationAttribute>(attr::RUNTIME_VISIBLE_ANNOTATIONS);
//     for (AnnotationExprent* ann : a->get_annotations()) ann->to_java(1, tracer);
//
// Ported from Fernflower (de.fernflower.struct.attr: StructAnnotationAttribute, StructAnnotationParameterAttribute, StructAnnotationTypeAttribute,
// StructAnnDefaultAttribute), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The attributes keep their bytes
// (attributes.hpp) and are read into expressions on first use, against the run's context (the expressions are objects of its arena): the original
// reads them as the class is loaded, which a class loaded outside a run has no arena for. The original's StructXxxAttribute names are aliases.
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/io.hpp"
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/exprents.hpp"

namespace cb::java::decompiler {

// RuntimeVisibleAnnotations, RuntimeInvisibleAnnotations.
class AnnotationAttribute : public Attribute {
public:
    // The annotations of the attribute (read on the first call; the same objects after).
    const std::vector<AnnotationExprent*>& get_annotations() const;

    // u2 count, then that many annotations (an empty list for a count of 0). FormatError when the bytes are cut off.
    static std::vector<AnnotationExprent*> parse_annotations(const ConstantPool& pool, io::ByteReader& data);
    // u2 class name index (a descriptor: its class name is kept), u2 element count, then each element's name index and value.
    static AnnotationExprent* parse_annotation(io::ByteReader& data, const ConstantPool& pool);
    // An element value: its tag ('e' enum constant, 'c' class, '[' array, '@' annotation, 'B' 'C' 'D' 'F' 'I' 'J' 'S' 'Z' 's' a constant) and content;
    // std::runtime_error for another tag or a class of no known type.
    static Exprent* parse_annotation_element(io::ByteReader& data, const ConstantPool& pool);

protected:
    void init_content(const ConstantPool& pool) override { pool_ = &pool; }

private:
    const ConstantPool* pool_ = nullptr;   // (the class's: it outlives its attributes)
    mutable std::optional<std::vector<AnnotationExprent*>> annotations_;
};

// RuntimeVisibleParameterAnnotations, RuntimeInvisibleParameterAnnotations: the annotations of each parameter.
class AnnotationParameterAttribute : public Attribute {
public:
    const std::vector<std::vector<AnnotationExprent*>>& get_param_annotations() const;

protected:
    void init_content(const ConstantPool& pool) override { pool_ = &pool; }

private:
    const ConstantPool* pool_ = nullptr;
    mutable std::optional<std::vector<std::vector<AnnotationExprent*>>> param_annotations_;
};

// RuntimeVisibleTypeAnnotations, RuntimeInvisibleTypeAnnotations: read for their annotations and where they are (the original keeps them and uses neither).
class AnnotationTypeAttribute : public Attribute {
public:
    // Where an annotation sits: the kind of target, how its data is laid out, the data, and the path into the type.
    struct AnnotationLocation {
        int target_type = 0;
        int target_union = 0;
        std::vector<int> data;
        std::vector<int> target_path_kind;
        std::vector<int> target_argument_index;
    };

    static constexpr int ANNOTATION_TARGET_TYPE_GENERIC_CLASS = 0x00, ANNOTATION_TARGET_TYPE_GENERIC_METHOD = 0x01,
                         ANNOTATION_TARGET_TYPE_EXTENDS_IMPLEMENTS = 0x10, ANNOTATION_TARGET_TYPE_GENERIC_CLASS_BOUND = 0x11,
                         ANNOTATION_TARGET_TYPE_GENERIC_METHOD_BOUND = 0x12, ANNOTATION_TARGET_TYPE_FIELD = 0x13, ANNOTATION_TARGET_TYPE_RETURN = 0x14,
                         ANNOTATION_TARGET_TYPE_RECEIVER = 0x15, ANNOTATION_TARGET_TYPE_FORMAL = 0x16, ANNOTATION_TARGET_TYPE_THROWS = 0x17,
                         ANNOTATION_TARGET_TYPE_LOCAL_VARIABLE = 0x40, ANNOTATION_TARGET_TYPE_RESOURCE_VARIABLE = 0x41,
                         ANNOTATION_TARGET_TYPE_EXCEPTION = 0x42, ANNOTATION_TARGET_TYPE_INSTANCEOF = 0x43, ANNOTATION_TARGET_TYPE_NEW = 0x44,
                         ANNOTATION_TARGET_TYPE_DOUBLE_COLON_NEW = 0x45, ANNOTATION_TARGET_TYPE_DOUBLE_COLON_ID = 0x46, ANNOTATION_TARGET_TYPE_CAST = 0x47,
                         ANNOTATION_TARGET_TYPE_INVOCATION_CONSTRUCTOR = 0x48, ANNOTATION_TARGET_TYPE_INVOCATION_METHOD = 0x49,
                         ANNOTATION_TARGET_TYPE_GENERIC_DOUBLE_COLON_NEW = 0x4A, ANNOTATION_TARGET_TYPE_GENERIC_DOUBLE_COLON_ID = 0x4B;
    static constexpr int ANNOTATION_TARGET_UNION_TYPE_PARAMETER = 1, ANNOTATION_TARGET_UNION_SUPERTYPE = 2, ANNOTATION_TARGET_UNION_TYPE_PARAMETER_BOUND = 3,
                         ANNOTATION_TARGET_UNION_EMPTY = 4, ANNOTATION_TARGET_UNION_FORMAL_PARAMETER = 5, ANNOTATION_TARGET_UNION_THROWS = 6,
                         ANNOTATION_TARGET_UNION_LOCAL_VAR = 7, ANNOTATION_TARGET_UNION_CATCH = 8, ANNOTATION_TARGET_UNION_OFFSET = 9,
                         ANNOTATION_TARGET_UNION_TYPE_ARGUMENT = 10;

    const std::vector<AnnotationLocation>& get_locations() const;
    const std::vector<AnnotationExprent*>& get_annotations() const;

protected:
    void init_content(const ConstantPool& pool) override { pool_ = &pool; }

private:
    void read() const;
    // std::runtime_error for a target type that is none.
    static AnnotationLocation parse_annotation_location(io::ByteReader& data);

    const ConstantPool* pool_ = nullptr;
    mutable bool read_ = false;
    mutable std::vector<AnnotationLocation> locations_;
    mutable std::vector<AnnotationExprent*> annotations_;
};

// AnnotationDefault: the default value of an annotation method.
class AnnotationDefaultAttribute : public Attribute {
public:
    Exprent* get_default_value() const;

protected:
    void init_content(const ConstantPool& pool) override { pool_ = &pool; }

private:
    const ConstantPool* pool_ = nullptr;
    mutable Exprent* default_value_ = nullptr;
};

using StructAnnotationAttribute = AnnotationAttribute;
using StructAnnotationParameterAttribute = AnnotationParameterAttribute;
using StructAnnotationTypeAttribute = AnnotationTypeAttribute;
using StructAnnDefaultAttribute = AnnotationDefaultAttribute;

}  // namespace cb::java::decompiler
