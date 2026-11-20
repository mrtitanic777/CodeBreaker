// java/decompiler/constant_pool.cpp -- see constant_pool.hpp. Ported from Fernflower (de.fernflower.struct.consts), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0.
#include "java/decompiler/constant_pool.hpp"

#include <bit>
#include <stdexcept>

#include "core/error.hpp"
#include "java/decompiler/code_constants.hpp"

namespace cb::java::decompiler {

int PooledConstant::pool_type_to_intern(int type) {
    switch (type) {
        case cc::CONSTANT_Integer: return vte::INT;
        case cc::CONSTANT_Float: return vte::FLOAT;
        case cc::CONSTANT_Long: return vte::LONG;
        case cc::CONSTANT_Double: return vte::DOUBLE;
        case cc::CONSTANT_String:
        case cc::CONSTANT_Class: return vte::REFERENCE;   // (1.5: ldc of a class)
        default: throw std::runtime_error("Huh?? What are you trying to load?");
    }
}

// ---- PrimitiveConstant

PrimitiveConstant::PrimitiveConstant(int type_, Value value_) : value(std::move(value_)) {
    type = type_;
    init_constant();
}

std::unique_ptr<PrimitiveConstant> PrimitiveConstant::indexed(int type_, int index_) {
    auto p = std::make_unique<PrimitiveConstant>(0, Value{});   // (no value yet: nothing to initialise from)
    p->type = type_;
    p->index = index_;
    return p;
}

void PrimitiveConstant::resolve(const ConstantPool& pool) {
    if (type == cc::CONSTANT_Class || type == cc::CONSTANT_String || type == cc::CONSTANT_MethodType) {
        const PrimitiveConstant* utf8 = pool.primitive(static_cast<std::size_t>(index));
        value = utf8 != nullptr && std::holds_alternative<std::string>(utf8->value) ? utf8->get_string() : std::string();
        init_constant();
    }
}

bool PrimitiveConstant::equals(const PrimitiveConstant& o) const {
    if (type != o.type || is_array != o.is_array || value.index() != o.value.index()) return false;
    if (const float* f = std::get_if<float>(&value)) return std::bit_cast<std::uint32_t>(*f) == std::bit_cast<std::uint32_t>(std::get<float>(o.value));
    if (const double* d = std::get_if<double>(&value)) return std::bit_cast<std::uint64_t>(*d) == std::bit_cast<std::uint64_t>(std::get<double>(o.value));
    return value == o.value;
}

void PrimitiveConstant::init_constant() {
    if (type == cc::CONSTANT_Class) {
        const std::string& name = get_string();
        is_array = !name.empty() && name[0] == '[';   // (an empty class name happens in some Android files)
    }
}

// ---- LinkConstant

LinkConstant::LinkConstant(int type_, std::string classname_, std::string elementname_, std::string descriptor_)
    : classname(std::move(classname_)), elementname(std::move(elementname_)), descriptor(std::move(descriptor_)) {
    type = type_;
    init_constant();
}

LinkConstant::LinkConstant(int type_, int index1_, int index2_) : index1(index1_), index2(index2_) { type = type_; }

void LinkConstant::resolve(const ConstantPool& pool) {
    const auto text_at = [&pool](int i) {
        const PrimitiveConstant* p = pool.primitive(static_cast<std::size_t>(i));
        return p != nullptr && std::holds_alternative<std::string>(p->value) ? p->get_string() : std::string();
    };
    if (type == cc::CONSTANT_NameAndType) {
        elementname = text_at(index1);
        descriptor = text_at(index2);
    } else if (type == cc::CONSTANT_MethodHandle) {
        if (const LinkConstant* ref = pool.link(static_cast<std::size_t>(index2))) {
            classname = ref->classname;
            elementname = ref->elementname;
            descriptor = ref->descriptor;
        }
    } else {
        if (type != cc::CONSTANT_InvokeDynamic) classname = text_at(index1);
        if (const LinkConstant* name_type = pool.link(static_cast<std::size_t>(index2))) {
            elementname = name_type->elementname;
            descriptor = name_type->descriptor;
        }
    }
    init_constant();
}

bool LinkConstant::equals(const LinkConstant& o) const {
    return type == o.type && elementname == o.elementname && descriptor == o.descriptor && (type != cc::CONSTANT_NameAndType || classname == o.classname);
}

void LinkConstant::init_constant() {
    if (type == cc::CONSTANT_Methodref || type == cc::CONSTANT_InterfaceMethodref || type == cc::CONSTANT_InvokeDynamic || type == cc::CONSTANT_MethodHandle) {
        resolve_descriptor(descriptor);
    } else if (type == cc::CONSTANT_Fieldref) {
        return_category2 = descriptor == "D" || descriptor == "J";
    }
}

void LinkConstant::resolve_descriptor(const std::string& descr) {
    // "(params)ret": the parameters counted (an object or an array of any depth is one), whether it returns nothing, or a long or a double
    const std::size_t open = descr.find('('), close = descr.find(')');
    if (open == std::string::npos || close == std::string::npos || close < open) return;   // (a field handle: no parameters)
    const std::string par = descr.substr(open + 1, close - open - 1);
    const std::string ret = descr.substr(close + 1);
    int counter = 0;
    for (std::size_t index = 0; index < par.size();) {
        const char c = par[index];
        if (c == 'L') {
            index = par.find(';', index);
            if (index == std::string::npos) break;
        } else if (c == '[') {
            ++index;
            continue;
        }
        ++counter;
        ++index;
    }
    param_count = counter;
    is_void = ret == "V";
    return_category2 = ret == "D" || ret == "J";
}

// ---- ConstantPool

ConstantPool::ConstantPool(const classfile::ClassFile& c, std::shared_ptr<const PoolInterceptor> interceptor) {
    using classfile::Tag;
    const std::size_t size = c.pool.size();
    std::vector<int> pass(size, 0);
    pool_.resize(size);
    for (std::size_t i = 1; i < size; ++i) {
        const classfile::Constant& k = c.pool[i];
        const int tag = static_cast<int>(k.tag);
        switch (k.tag) {
            case Tag::Utf8: pool_[i] = std::make_unique<PrimitiveConstant>(tag, PrimitiveConstant::Value(classfile::text(k.bytes))); break;
            case Tag::Integer: pool_[i] = std::make_unique<PrimitiveConstant>(tag, PrimitiveConstant::Value(static_cast<std::int32_t>(static_cast<std::uint32_t>(k.value)))); break;
            case Tag::Float: pool_[i] = std::make_unique<PrimitiveConstant>(tag, PrimitiveConstant::Value(std::bit_cast<float>(static_cast<std::uint32_t>(k.value)))); break;
            case Tag::Long: pool_[i] = std::make_unique<PrimitiveConstant>(tag, PrimitiveConstant::Value(static_cast<std::int64_t>(k.value))); break;
            case Tag::Double: pool_[i] = std::make_unique<PrimitiveConstant>(tag, PrimitiveConstant::Value(std::bit_cast<double>(k.value))); break;
            case Tag::Class:
            case Tag::String:
            case Tag::MethodType:
                pool_[i] = PrimitiveConstant::indexed(tag, static_cast<int>(k.a));
                pass[i] = 1;
                break;
            case Tag::Fieldref:
            case Tag::Methodref:
            case Tag::InterfaceMethodref:
            case Tag::NameAndType:
            case Tag::InvokeDynamic:
                pool_[i] = std::make_unique<LinkConstant>(tag, static_cast<int>(k.a), static_cast<int>(k.b));
                pass[i] = k.tag == Tag::NameAndType ? 1 : 2;
                break;
            case Tag::MethodHandle:
                pool_[i] = std::make_unique<LinkConstant>(tag, static_cast<int>(k.kind), static_cast<int>(k.a));
                pass[i] = 3;
                break;
            case Tag::None: break;   // (the slot after a long or a double)
            default:   // Dynamic, Module, Package: the decompiler does not read them (its reading of the pool goes wrong on them; a class holding one is refused)
                throw FormatError("class", 0, std::string("a constant the decompiler does not read: ") + std::string(classfile::tag_name(k.tag)));
        }
    }
    for (int pass_index = 1; pass_index <= 3; ++pass_index)
        for (std::size_t i = 1; i < size; ++i)
            if (pass[i] == pass_index) pool_[i]->resolve(*this);
    interceptor_ = std::move(interceptor);   // (set after the resolving, as the original takes it then)
}

const PooledConstant* ConstantPool::constant(std::size_t index) const { return index < pool_.size() ? pool_[index].get() : nullptr; }

const PrimitiveConstant* ConstantPool::primitive(std::size_t index) const {
    const PrimitiveConstant* cn = dynamic_cast<const PrimitiveConstant*>(constant(index));
    if (cn != nullptr && interceptor_ && cn->type == cc::CONSTANT_Class)
        if (const std::optional<std::string> name = build_new_classname(cn->get_string())) {
            renamed_.push_back(std::make_unique<PrimitiveConstant>(cc::CONSTANT_Class, PrimitiveConstant::Value(*name)));
            return static_cast<const PrimitiveConstant*>(renamed_.back().get());
        }
    return cn;
}

const LinkConstant* ConstantPool::link(std::size_t index) const {
    const LinkConstant* ln = dynamic_cast<const LinkConstant*>(constant(index));
    if (ln != nullptr && interceptor_ &&
        (ln->type == cc::CONSTANT_Fieldref || ln->type == cc::CONSTANT_Methodref || ln->type == cc::CONSTANT_InterfaceMethodref)) {
        const std::optional<std::string> new_class = build_new_classname(ln->classname);
        const std::optional<std::string> new_element = interceptor_->get_name(ln->classname + " " + ln->elementname + " " + ln->descriptor);
        const std::optional<std::string> new_descriptor = build_new_descriptor(ln->type == cc::CONSTANT_Fieldref, ln->descriptor);
        if (new_class || new_element || new_descriptor) {
            std::string element = ln->elementname;
            if (new_element) {
                const std::size_t a = new_element->find(' '), b = a == std::string::npos ? a : new_element->find(' ', a + 1);
                element = a == std::string::npos ? *new_element : new_element->substr(a + 1, b == std::string::npos ? std::string::npos : b - a - 1);
            }
            renamed_.push_back(std::make_unique<LinkConstant>(ln->type, new_class.value_or(ln->classname), element, new_descriptor.value_or(ln->descriptor)));
            return static_cast<const LinkConstant*>(renamed_.back().get());
        }
    }
    return ln;
}

std::pair<std::string, std::string> ConstantPool::class_element(int element_type, const std::string& class_name, int name_index, int descriptor_index) const {
    const auto text_at = [this](int i) {
        const PrimitiveConstant* p = dynamic_cast<const PrimitiveConstant*>(constant(static_cast<std::size_t>(i)));
        return p != nullptr && std::holds_alternative<std::string>(p->value) ? p->get_string() : std::string();
    };
    std::string element = text_at(name_index), descriptor = text_at(descriptor_index);
    if (interceptor_) {
        if (const std::optional<std::string> n = interceptor_->get_name(class_name + " " + element + " " + descriptor)) {
            const std::size_t a = n->find(' '), b = a == std::string::npos ? a : n->find(' ', a + 1);
            if (a != std::string::npos) element = n->substr(a + 1, b == std::string::npos ? std::string::npos : b - a - 1);
        }
        if (const std::optional<std::string> d = build_new_descriptor(element_type == FIELD, descriptor)) descriptor = *d;
    }
    return {element, descriptor};
}

std::optional<std::string> ConstantPool::build_new_classname(const std::string& class_name) const {
    if (!interceptor_) return std::nullopt;
    const VarType vt = VarType::parse(class_name, true);
    const std::optional<std::string> name = interceptor_->get_name(vt.value.value_or(""));
    if (!name) return std::nullopt;
    if (vt.array_dim > 0) return std::string(static_cast<std::size_t>(vt.array_dim), '[') + "L" + *name + ";";
    return name;
}

std::optional<std::string> ConstantPool::build_new_descriptor(bool is_field, const std::string& descriptor) const {
    const NewClassNameBuilder builder = [this](const std::string& name) { return build_new_classname(name); };
    if (is_field) return FieldDescriptor::parse(descriptor).build_new_descriptor(builder);
    return MethodDescriptor::parse(descriptor).build_new_descriptor(builder);
}

}  // namespace cb::java::decompiler
