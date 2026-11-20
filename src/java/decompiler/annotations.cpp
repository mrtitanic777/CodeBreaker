// java/decompiler/annotations.cpp -- see annotations.hpp. Ported from Fernflower (de.fernflower.struct.attr: StructAnnotationAttribute,
// StructAnnotationParameterAttribute, StructAnnotationTypeAttribute, StructAnnDefaultAttribute), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/annotations.hpp"

#include <stdexcept>

#include "java/decompiler/code_constants.hpp"

namespace cb::java::decompiler {

namespace {

std::string pool_string(const ConstantPool& pool, int index) { return pool.primitive(static_cast<std::size_t>(index))->get_string(); }

io::ByteReader attribute_reader(const Attribute& a) { return io::ByteReader(a.info(), "class.attribute"); }

}  // namespace

// ---- AnnotationAttribute

const std::vector<AnnotationExprent*>& AnnotationAttribute::get_annotations() const {
    if (!annotations_) {
        io::ByteReader data = attribute_reader(*this);
        annotations_ = parse_annotations(*pool_, data);
    }
    return *annotations_;
}

std::vector<AnnotationExprent*> AnnotationAttribute::parse_annotations(const ConstantPool& pool, io::ByteReader& data) {
    const int len = data.u16_be();
    std::vector<AnnotationExprent*> annotations;
    if (len > 0) {
        annotations.reserve(static_cast<std::size_t>(len));
        for (int i = 0; i < len; i++) annotations.push_back(parse_annotation(data, pool));
    }
    return annotations;
}

AnnotationExprent* AnnotationAttribute::parse_annotation(io::ByteReader& data, const ConstantPool& pool) {
    const std::string class_name = pool_string(pool, data.u16_be());

    std::vector<std::string> names;
    std::vector<Exprent*> values;

    const int len = data.u16_be();
    if (len > 0) {
        names.reserve(static_cast<std::size_t>(len));
        values.reserve(static_cast<std::size_t>(len));
        for (int i = 0; i < len; i++) {
            names.push_back(pool_string(pool, data.u16_be()));
            values.push_back(parse_annotation_element(data, pool));
        }
    }

    return make<AnnotationExprent>(VarType::parse(class_name).value.value(), std::move(names), std::move(values));
}

Exprent* AnnotationAttribute::parse_annotation_element(io::ByteReader& data, const ConstantPool& pool) {
    const int tag = data.u8();

    switch (tag) {
        case 'e': {   // enum constant
            const std::string class_name = pool_string(pool, data.u16_be());
            const std::string const_name = pool_string(pool, data.u16_be());

            const FieldDescriptor descr = FieldDescriptor::parse(class_name);
            return make<FieldExprent>(const_name, descr.type.value.value(), true, nullptr, descr, std::nullopt);
        }

        case 'c': {   // class
            const std::string descriptor = pool_string(pool, data.u16_be());

            const VarType type = FieldDescriptor::parse(descriptor).type;

            std::string value;
            switch (type.type) {
                case cc::TYPE_OBJECT:
                    value = type.value.value();
                    break;
                case cc::TYPE_BYTE:
                    value = "byte";
                    break;
                case cc::TYPE_CHAR:
                    value = "char";
                    break;
                case cc::TYPE_DOUBLE:
                    value = "double";
                    break;
                case cc::TYPE_FLOAT:
                    value = "float";
                    break;
                case cc::TYPE_INT:
                    value = "int";
                    break;
                case cc::TYPE_LONG:
                    value = "long";
                    break;
                case cc::TYPE_SHORT:
                    value = "short";
                    break;
                case cc::TYPE_BOOLEAN:
                    value = "boolean";
                    break;
                case cc::TYPE_VOID:
                    value = "void";
                    break;
                default:
                    throw std::runtime_error("invalid class type: " + std::to_string(type.type));
            }

            return make<ConstExprent>(VarType::CLASS, ConstExprent::Value(value), std::nullopt);
        }

        case '[': {   // array
            std::vector<Exprent*> elements;

            const int len = data.u16_be();
            if (len > 0) {
                elements.reserve(static_cast<std::size_t>(len));
                for (int i = 0; i < len; i++) elements.push_back(parse_annotation_element(data, pool));
            }

            VarType new_type;
            if (elements.empty()) {
                new_type = VarType(cc::TYPE_OBJECT, 1, std::string("java/lang/Object"));
            } else {
                const VarType element_type = elements[0]->get_expr_type();
                new_type = VarType(element_type.type, 1, element_type.value);
            }

            NewExprent* new_expr = make<NewExprent>(new_type, std::vector<Exprent*>{}, std::nullopt);
            new_expr->set_direct_array_init(true);
            new_expr->set_lst_array_elements(std::move(elements));
            return new_expr;
        }

        case '@':   // annotation
            return parse_annotation(data, pool);

        default: {
            const PrimitiveConstant* cn = pool.primitive(data.u16_be());
            switch (tag) {
                case 'B':
                    return make<ConstExprent>(VarType::BYTE, cn->value, std::nullopt);
                case 'C':
                    return make<ConstExprent>(VarType::CHAR, cn->value, std::nullopt);
                case 'D':
                    return make<ConstExprent>(VarType::DOUBLE, cn->value, std::nullopt);
                case 'F':
                    return make<ConstExprent>(VarType::FLOAT, cn->value, std::nullopt);
                case 'I':
                    return make<ConstExprent>(VarType::INT, cn->value, std::nullopt);
                case 'J':
                    return make<ConstExprent>(VarType::LONG, cn->value, std::nullopt);
                case 'S':
                    return make<ConstExprent>(VarType::SHORT, cn->value, std::nullopt);
                case 'Z':
                    return make<ConstExprent>(VarType::BOOLEAN, cn->value, std::nullopt);
                case 's':
                    return make<ConstExprent>(VarType::STRING, cn->value, std::nullopt);
                default:
                    throw std::runtime_error("invalid element type!");
            }
        }
    }
}

// ---- AnnotationParameterAttribute

const std::vector<std::vector<AnnotationExprent*>>& AnnotationParameterAttribute::get_param_annotations() const {
    if (!param_annotations_) {
        io::ByteReader data = attribute_reader(*this);

        std::vector<std::vector<AnnotationExprent*>> param_annotations;
        const int len = data.u8();
        if (len > 0) {
            param_annotations.reserve(static_cast<std::size_t>(len));
            for (int i = 0; i < len; i++) param_annotations.push_back(AnnotationAttribute::parse_annotations(*pool_, data));
        }
        param_annotations_ = std::move(param_annotations);
    }
    return *param_annotations_;
}

// ---- AnnotationTypeAttribute

const std::vector<AnnotationTypeAttribute::AnnotationLocation>& AnnotationTypeAttribute::get_locations() const {
    read();
    return locations_;
}

const std::vector<AnnotationExprent*>& AnnotationTypeAttribute::get_annotations() const {
    read();
    return annotations_;
}

void AnnotationTypeAttribute::read() const {
    if (read_) return;
    io::ByteReader data = attribute_reader(*this);

    std::vector<AnnotationLocation> locations;
    std::vector<AnnotationExprent*> annotations;

    const int len = data.u8();
    if (len > 0) {
        for (int i = 0; i < len; i++) {
            locations.push_back(parse_annotation_location(data));
            annotations.push_back(AnnotationAttribute::parse_annotation(data, *pool_));
        }
    }

    locations_ = std::move(locations);
    annotations_ = std::move(annotations);
    read_ = true;
}

AnnotationTypeAttribute::AnnotationLocation AnnotationTypeAttribute::parse_annotation_location(io::ByteReader& data) {
    AnnotationLocation ann_location;

    // target type
    ann_location.target_type = data.u8();

    // target union
    switch (ann_location.target_type) {
        case ANNOTATION_TARGET_TYPE_GENERIC_CLASS:
        case ANNOTATION_TARGET_TYPE_GENERIC_METHOD:
            ann_location.target_union = ANNOTATION_TARGET_UNION_TYPE_PARAMETER;
            break;
        case ANNOTATION_TARGET_TYPE_EXTENDS_IMPLEMENTS:
            ann_location.target_union = ANNOTATION_TARGET_UNION_SUPERTYPE;
            break;
        case ANNOTATION_TARGET_TYPE_GENERIC_CLASS_BOUND:
        case ANNOTATION_TARGET_TYPE_GENERIC_METHOD_BOUND:
            ann_location.target_union = ANNOTATION_TARGET_UNION_TYPE_PARAMETER_BOUND;
            break;
        case ANNOTATION_TARGET_TYPE_FIELD:
        case ANNOTATION_TARGET_TYPE_RETURN:
        case ANNOTATION_TARGET_TYPE_RECEIVER:
            ann_location.target_union = ANNOTATION_TARGET_UNION_EMPTY;
            break;
        case ANNOTATION_TARGET_TYPE_FORMAL:
            ann_location.target_union = ANNOTATION_TARGET_UNION_FORMAL_PARAMETER;
            break;
        case ANNOTATION_TARGET_TYPE_THROWS:
            ann_location.target_union = ANNOTATION_TARGET_UNION_THROWS;
            break;
        case ANNOTATION_TARGET_TYPE_LOCAL_VARIABLE:
        case ANNOTATION_TARGET_TYPE_RESOURCE_VARIABLE:
            ann_location.target_union = ANNOTATION_TARGET_UNION_LOCAL_VAR;
            break;
        case ANNOTATION_TARGET_TYPE_EXCEPTION:
            ann_location.target_union = ANNOTATION_TARGET_UNION_CATCH;
            break;
        case ANNOTATION_TARGET_TYPE_INSTANCEOF:
        case ANNOTATION_TARGET_TYPE_NEW:
        case ANNOTATION_TARGET_TYPE_DOUBLE_COLON_NEW:
        case ANNOTATION_TARGET_TYPE_DOUBLE_COLON_ID:
            ann_location.target_union = ANNOTATION_TARGET_UNION_OFFSET;
            break;
        case ANNOTATION_TARGET_TYPE_CAST:
        case ANNOTATION_TARGET_TYPE_INVOCATION_CONSTRUCTOR:
        case ANNOTATION_TARGET_TYPE_INVOCATION_METHOD:
        case ANNOTATION_TARGET_TYPE_GENERIC_DOUBLE_COLON_NEW:
        case ANNOTATION_TARGET_TYPE_GENERIC_DOUBLE_COLON_ID:
            ann_location.target_union = ANNOTATION_TARGET_UNION_TYPE_ARGUMENT;
            break;
        default:
            throw std::runtime_error("Unknown target type in a type annotation!");
    }

    // target union data
    switch (ann_location.target_union) {
        case ANNOTATION_TARGET_UNION_TYPE_PARAMETER:
        case ANNOTATION_TARGET_UNION_FORMAL_PARAMETER:
            ann_location.data = {static_cast<int>(data.u8())};
            break;
        case ANNOTATION_TARGET_UNION_SUPERTYPE:
        case ANNOTATION_TARGET_UNION_THROWS:
        case ANNOTATION_TARGET_UNION_CATCH:
        case ANNOTATION_TARGET_UNION_OFFSET:
            ann_location.data = {static_cast<int>(data.u16_be())};
            break;
        case ANNOTATION_TARGET_UNION_TYPE_PARAMETER_BOUND: {
            const int first = data.u8();
            const int second = data.u8();
            ann_location.data = {first, second};
            break;
        }
        case ANNOTATION_TARGET_UNION_EMPTY:
            break;
        case ANNOTATION_TARGET_UNION_LOCAL_VAR: {
            const int table_length = data.u16_be();
            ann_location.data.assign(static_cast<std::size_t>(table_length) * 3 + 1, 0);
            ann_location.data[0] = table_length;
            for (int i = 0; i < table_length; ++i) {
                ann_location.data[static_cast<std::size_t>(3 * i + 1)] = data.u16_be();
                ann_location.data[static_cast<std::size_t>(3 * i + 2)] = data.u16_be();
                ann_location.data[static_cast<std::size_t>(3 * i + 3)] = data.u16_be();
            }
            break;
        }
        case ANNOTATION_TARGET_UNION_TYPE_ARGUMENT: {
            const int first = data.u16_be();
            const int second = data.u8();
            ann_location.data = {first, second};
            break;
        }
        default:
            break;
    }

    // target path
    const int path_length = data.u8();
    ann_location.target_path_kind.assign(static_cast<std::size_t>(path_length), 0);
    ann_location.target_argument_index.assign(static_cast<std::size_t>(path_length), 0);
    for (int i = 0; i < path_length; ++i) {
        ann_location.target_path_kind[static_cast<std::size_t>(i)] = data.u8();
        ann_location.target_argument_index[static_cast<std::size_t>(i)] = data.u8();
    }

    return ann_location;
}

// ---- AnnotationDefaultAttribute

Exprent* AnnotationDefaultAttribute::get_default_value() const {
    if (default_value_ == nullptr) {
        io::ByteReader data = attribute_reader(*this);
        default_value_ = AnnotationAttribute::parse_annotation_element(data, *pool_);
    }
    return default_value_;
}

}  // namespace cb::java::decompiler
